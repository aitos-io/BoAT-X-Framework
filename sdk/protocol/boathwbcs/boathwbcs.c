/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/*!@brief Perform RAW transaction

@file
boathwbcs.c contains functions to construct a raw transaction, perform it and 
wait for its receipt.
*/

/* self-header include */
#include "boatconfig.h"
#include "boathwbcs.h"

#if PROTOCOL_USE_HWBCS == 1
#include "http2intf.h"
#include "boatplatform_internal.h"
/* protos header include */
#include "common/common.pb-c.h"
#include "peer/chaincode.pb-c.h"
#include "peer/proposal.pb-c.h"
#include "msp/identities.pb-c.h"
#include "peer/transaction.pb-c.h"
#include "peer/proposal_response.pb-c.h"
#include "common/transaction.pb-c-hwbcs.h"
#include "common/contract.pb-c.h"

/*!****************************************************************************
 * @brief  envelope payload packed
 * 
 * @details
 *   envelope payload is consist of header fields and data payload fields. Header
 *   fields include signature header and channel header; data payload is either
 *   proposal payload or transaction payload.
 *
 * @param tx_ptr 
 *   huawei transaction structure pointer
 *
 * @param[out] output_ptr 
 *   A structure pointer to store signature header protobuf serialize data and length. 
 *   In internal of this function, the memory of store serialize data has been alloced,
 *   caller SHOULD NOT alloc memory for this pointer again.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code. 
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT hwbcsPayloadPacked(BoatHwbcsTx *tx_ptr,
											   BoatFieldVariable *output_ptr)
{
	Common__TxPayload txPayload = COMMON__TX_PAYLOAD__INIT;
	Common__TxHeader txhead = COMMON__TX_HEADER__INIT;
	Common__Identity identity_creator = COMMON__IDENTITY__INIT;
	Common__ContractInvocation contractInvocation = COMMON__CONTRACT_INVOCATION__INIT;
	BUINT32 packedLength;
	BUINT32 offset = 0;
	BoatHwbcsEndorserResponse *parsePtr = NULL;
	parsePtr = tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr;

	BOAT_RESULT result = BOAT_SUCCESS;
	// boat_try_declare;
	if (tx_ptr->var.type == HWBCS_TYPE_PROPOSAL)
	{
		/* contractInvocation info*/
		contractInvocation.contract_name = tx_ptr->var.contract_name;
		contractInvocation.args = BoatMalloc((tx_ptr->var.args.nArgs - 1) * sizeof(ProtobufCBinaryData));
		contractInvocation.func_name = tx_ptr->var.args.args[0];
		contractInvocation.n_args = tx_ptr->var.args.nArgs - 1;

		for (size_t i = 0; i < tx_ptr->var.args.nArgs - 1; i++)
		{
			
			contractInvocation.args[i].data = (BUINT8 *)tx_ptr->var.args.args[1 + i];
			contractInvocation.args[i].len = strlen(tx_ptr->var.args.args[1 + i]);
		}

		txPayload.data.len = common__contract_invocation__get_packed_size(&contractInvocation);
		txPayload.data.data = BoatMalloc(txPayload.data.len);
		common__contract_invocation__pack(&contractInvocation, txPayload.data.data);
		/* payload head */
		txhead.chain_id = tx_ptr->var.chaincodeId.name;
		txhead.timestamp = (long)(((double)tx_ptr->var.timestamp.sec) * 60 * 1000000.0);
		txhead.nonce = UtilityBuint8Buf2Uint64(tx_ptr->var.nonce.field, sizeof(tx_ptr->var.nonce.field));
		identity_creator.id.data = (BUINT8 *)tx_ptr->var.creator_id;
		identity_creator.id.len = strlen(tx_ptr->var.creator_id);
		identity_creator.org = tx_ptr->var.orgName;
		txhead.creator = &identity_creator;

		txPayload.header = &txhead;

		/* payload*/
		packedLength = common__tx_payload__get_packed_size(&txPayload);
		output_ptr->field_len = packedLength;
		output_ptr->field_ptr = BoatMalloc(packedLength);
		common__tx_payload__pack(&txPayload, output_ptr->field_ptr);
	}
	else
	{
		packedLength = 0;
		for (int i = 0; i < parsePtr->responseCount; i++)
		{
			packedLength += parsePtr->response[i].payload.field_len;
		}
		output_ptr->field_len = packedLength;
		output_ptr->field_ptr = BoatMalloc(packedLength);
		offset = 0;
		for (int i = 0; i < parsePtr->responseCount; i++)
		{
			memcpy(output_ptr->field_ptr + offset, parsePtr->response[i].payload.field_ptr, parsePtr->response[i].payload.field_len);
			offset += parsePtr->response[i].payload.field_len;
		}
	}

	/* free malloc */
	BoatFree(txPayload.data.data);
	return result;
}

BOAT_RESULT hwbcsProposalTransactionPacked(BoatHwbcsTx *tx_ptr)
{
	Common__Approval approval_message = COMMON__APPROVAL__INIT;
	Common__Transaction transaction = COMMON__TRANSACTION__INIT;
	Common__Approval *approval_messages;
	BoatFieldVariable payloadPacked = {NULL, 0};
	BoatSignatureResult signatureResult;
	BUINT8 grpcHeader[5];
	BUINT8 hash[32];
	BUINT32 packedLength;
	BUINT8 *packedData = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
	/* step-0: param in check */
	if ((tx_ptr == NULL) || (tx_ptr->wallet_ptr == NULL) ||
		(tx_ptr->wallet_ptr->http2Context_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter should not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* step-1: generate nonce once for proposal */
	if (tx_ptr->var.type == HWBCS_TYPE_PROPOSAL)
	{
		tx_ptr->var.nonce.field_len = 8;
		result = BoatRandom(tx_ptr->var.nonce.field, tx_ptr->var.nonce.field_len, NULL);
	}
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hwbcsGenNonce.");
		boat_throw(BOAT_ERROR_COMMON_GEN_RAND_FAIL, hwbcsProposalTransactionPacked_exception);
	}

	/* step-2:  payload packed  */
	result = hwbcsPayloadPacked(tx_ptr, &payloadPacked);



	/* --------> creator packed */

	/* step-3: compute hash */
	result = BoatHash(BOAT_HASH_SHA256, payloadPacked.field_ptr,
					  payloadPacked.field_len, hash, NULL, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		return BOAT_ERROR_COMMON_GEN_HASH_FAIL;
	}

	/* step-4: signature */
	result = BoatSignature(tx_ptr->wallet_ptr->account_info.prikeyCtx,
						   hash, sizeof(hash), &signatureResult, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
		return BOAT_ERROR_COMMON_GEN_SIGN_FAIL;
	}

	if (!signatureResult.pkcs_format_used)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
		return BOAT_ERROR_COMMON_GEN_SIGN_FAIL;
	}
	/* approvals */

	approval_message.identity.len = tx_ptr->wallet_ptr->account_info.cert.field_len;
	approval_message.identity.data = tx_ptr->wallet_ptr->account_info.cert.field_ptr;
	approval_message.sign.data = signatureResult.pkcs_sign;
	approval_message.sign.len = signatureResult.pkcs_sign_length;

	transaction.payload.data = payloadPacked.field_ptr;
	transaction.payload.len = payloadPacked.field_len;
	transaction.n_approvals = 0;
	transaction.n_approvals = 1;
	approval_messages = &approval_message;
	transaction.approvals = &approval_messages;
	packedLength = common__transaction__get_packed_size(&transaction);
	packedData = BoatMalloc(packedLength);
	common__transaction__pack(&transaction, packedData);
	/* ---grpcHeader compute */
	grpcHeader[0] = 0x00; //uncompressed
	for (int i = 0; i < 4; i++)
	{
		grpcHeader[i + 1] = (packedLength >> (32 - 8 * (i + 1))) & 0xFF;
	}
	/* ---generate packed data */
	tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len = packedLength + sizeof(grpcHeader);
	memcpy(tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr, grpcHeader, sizeof(grpcHeader));
	memcpy(&tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr[sizeof(grpcHeader)], packedData, packedLength);

	/* boat catch handle */
	boat_catch(hwbcsProposalTransactionPacked_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	/* free malloc */
	BoatFree(payloadPacked.field_ptr);
	BoatFree(packedData);

	return result;
}

/*! @}*/

#endif /* end of PROTOCOL_USE_HWBCS */
