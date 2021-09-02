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
boathlfabric.c contains functions to construct a raw transaction, perform it and 
wait for its receipt.
*/

/* self-header include */
#include "boatconfig.h"
#include "boathlfabric.h"

#if PROTOCOL_USE_HLHUAWEI == 1
#include "http2intf.h"
#include "boatplatform_internal.h"
/* protos header include */
#include "common/common.pb-c.h"
#include "peer/chaincode.pb-c.h"
#include "peer/proposal.pb-c.h"
#include "msp/identities.pb-c.h"
#include "peer/transaction.pb-c.h"
#include "peer/proposal_response.pb-c.h"
#include "common/transaction.pb-c-huawei.h"
#include "common/contract.pb-c.h"

/*! @ingroup fabricp protocol-fabric
 * @{
 */




/*!****************************************************************************
 * @brief  envelope payload packed
 * 
 * @details
 *   envelope payload is consist of header fields and data payload fields. Header
 *   fields include signature header and channel header; data payload is either
 *   proposal payload or transaction payload.
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param[out] output_ptr 
 *   A structure pointer to store signature header protobuf serialize data and length. 
 *   In internal of this function, the memory of store serialize data has been alloced,
 *   caller SHOULD NOT alloc memory for this pointer again.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code. 
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT hlhuaweiPayloadPacked(BoatHlfabricTx *tx_ptr,
											   BoatFieldVariable *output_ptr)
{
	Common__TxPayload txPayload = COMMON__TX_PAYLOAD__INIT;
	Common__TxHeader txhead = COMMON__TX_HEADER__INIT;
	Common__Identity identity_creator = COMMON__IDENTITY__INIT;
	Common__ContractInvocation contractInvocation = COMMON__CONTRACT_INVOCATION__INIT;
	BUINT32 packedLength;
	BUINT32 offset = 0;
	BoatHlfabricEndorserResponse *parsePtr = NULL;
	parsePtr = tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr;

	BOAT_RESULT result = BOAT_SUCCESS;
	// boat_try_declare;
	if (tx_ptr->var.type == HLFABRIC_TYPE_PROPOSAL)
	{
		contractInvocation.contract_name = tx_ptr->var.contract_name;
		contractInvocation.args = BoatMalloc((tx_ptr->var.args.nArgs - 1) * sizeof(ProtobufCBinaryData));
		contractInvocation.func_name = tx_ptr->var.args.args[0];
		contractInvocation.n_args = tx_ptr->var.args.nArgs - 1;

		for (size_t i = 0; i < tx_ptr->var.args.nArgs - 1; i++)
		{
			/* code */
			contractInvocation.args[i].data = (BUINT8*)tx_ptr->var.args.args[1 + i];
			contractInvocation.args[i].len = strlen(tx_ptr->var.args.args[1 + i]);
		}

		txPayload.data.len = common__contract_invocation__get_packed_size(&contractInvocation);
		// packedData = BoatMalloc(packedLength);
		// common__contract_invocation__pack(&contractInvocation, packedData);
		txPayload.data.data = BoatMalloc(txPayload.data.len);
		common__contract_invocation__pack(&contractInvocation, txPayload.data.data);
		// txPayload.data.data = BoatMalloc(txPayload.data.len);
		// common__contract_invocation__pack(&contractInvocation, txPayload.data.data);

		// protobuf_c_message_free_unpacked(&contractInvocation,NULL);
		/* -type */
		txhead.chain_id = tx_ptr->var.chaincodeId.name;
		// protobuf_c_message_free_unpacked(&contractI
		// txhead.nonce = UtilityBuint8Buf2Uint64(tx_ptr->var.nonce.field,sizeof(tx_ptr->var.nonce.field));
		// txhead.nonce = 0x3333333333333333;
		txhead.timestamp = (long)(((double)tx_ptr->var.timestamp.sec) * 60 * 1000000.0);
		// txhead.timestamp = 0x5555555555555555;
		// txhead.nonce = txhead.timestamp;
		txhead.nonce = UtilityBuint8Buf2Uint64(tx_ptr->var.nonce.field, sizeof(tx_ptr->var.nonce.field));
		identity_creator.id.data = (BUINT8*)tx_ptr->var.creator_id;
		identity_creator.id.len = strlen(tx_ptr->var.creator_id);
		identity_creator.org = tx_ptr->var.orgName;
		txhead.creator = &identity_creator;
		// txhead.type = COMMON__TX_TYPE__VOTE_TRANSACTION;

		txPayload.header = &txhead;

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
		offset =0;
		for (int i = 0; i < parsePtr->responseCount; i++)
		{
			memcpy(output_ptr->field_ptr + offset,parsePtr->response[i].payload.field_ptr,parsePtr->response[i].payload.field_len);
			offset += parsePtr->response[i].payload.field_len;
		}
		
	}

	// txPayload.data.data = packedData;

	/* boat catch handle */
	// boat_catch(hlhuaweiPayloadPacked_exception)
	// {
	// 	BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	// 	result = boat_exception;
	// }

	/* free malloc */
	BoatFree(txPayload.data.data);
	return result;
}

BOAT_RESULT hlhuaweiProposalTransactionPacked(BoatHlfabricTx *tx_ptr)
{
	Common__Approval approval_message = COMMON__APPROVAL__INIT;
	Common__Transaction transaction = COMMON__TRANSACTION__INIT;
	// Common__Approval *approval_message_transaction_prt[BOAT_HLFABRIC_ENDORSER_MAX_NUM];
	// Common__Approval approval_message_transaction[BOAT_HLFABRIC_ENDORSER_MAX_NUM] = {COMMON__APPROVAL__INIT};
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
		return BOAT_ERROR;
	}

	/* step-1: generate nonce once for proposal */
	if (tx_ptr->var.type == HLFABRIC_TYPE_PROPOSAL)
	{
		tx_ptr->var.nonce.field_len = 8;
		result = BoatRandom(tx_ptr->var.nonce.field, tx_ptr->var.nonce.field_len, NULL);
	}
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricGenNonce.");
		boat_throw(result, hlhuaweiProposalTransactionPacked_exception);
	}

	/* step-2: compute payload packed length */
	result = hlhuaweiPayloadPacked(tx_ptr, &payloadPacked);

	/* ------>signature header */
	// result = hlfabricSignatureHeaderPacked( tx_ptr, txIdBin, &signatureHeaderPacked );

	approval_message.identity.len = tx_ptr->wallet_ptr->account_info.cert.field_len;
	approval_message.identity.data = tx_ptr->wallet_ptr->account_info.cert.field_ptr;
	// approval_message.type = COMMON__CONTRACT_RUN_ENV__Native;

	/* --------> creator packed */

	/* step-3: compute hash */
	result = BoatHash(BOAT_HASH_SHA256, payloadPacked.field_ptr,
					  payloadPacked.field_len, hash, NULL, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		return BOAT_ERROR;
	}

	/* step-4: signature */
	result = BoatSignature(tx_ptr->wallet_ptr->account_info.prikeyCtx,
						   hash, sizeof(hash), &signatureResult, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
		return BOAT_ERROR;
	}

	if (!signatureResult.pkcs_format_used)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
		return BOAT_ERROR;
	}

	approval_message.sign.data = signatureResult.pkcs_sign;
	approval_message.sign.len = signatureResult.pkcs_sign_length;

	transaction.payload.data = payloadPacked.field_ptr;
	transaction.payload.len = payloadPacked.field_len;
	transaction.n_approvals = 0;
	// if (tx_ptr->var.type == HLFABRIC_TYPE_PROPOSAL)
	// {
	transaction.n_approvals = 1;
	approval_messages = &approval_message;
	transaction.approvals = &approval_messages;
	// }
	// else if (tx_ptr->var.type == HLFABRIC_TYPE_TRANSACTION)
	// {

	// 	parsePtr = tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr;
	// 	transaction.n_approvals = parsePtr->responseCount + 1;

	// 	for (int i = 0; i < transaction.n_approvals; i++)
	// 	{
	// 		approval_message_transaction[i] = approval_message_transaction[0];
	// 		approval_message_transaction_prt[i] = &approval_message_transaction[i];
	// 	}
	// 	transaction.approvals = approval_message_transaction_prt;

	// 	// approval_messages = BoatMalloc(transaction.n_approvals * sizeof(Common__Approval));
	// 	approval_message_transaction[0] = approval_message;

	// 	for (int i = 0; i < parsePtr->responseCount; i++)
	// 	{
	// 		// approval_message.identity.data = parsePtr->response[i].endorser.field_ptr;
	// 		// approval_message.identity.len = parsePtr->response[i].endorser.field_len;
	// 		// approval_message.sign.data = parsePtr->response[i].signature.field_ptr;
	// 		// approval_message.sign.len = parsePtr->response[i].signature.field_len;
	// 		approval_message_transaction[i+1].sign.data = parsePtr->response[i].signature.field_ptr;
	// 		approval_message_transaction[i+1].sign.len = parsePtr->response[i].signature.field_len;
	// 		approval_message_transaction[i+1].identity.data = parsePtr->response[i].endorser.field_ptr;
	// 		approval_message_transaction[i+1].identity.len = parsePtr->response[i].endorser.field_len;
	// 		BoatLog(BOAT_LOG_CRITICAL, "signature.field_len = %d , endorser.field_len = %d ", parsePtr->response[i].signature.field_len, parsePtr->response[i].endorser.field_len);
	// 		// approval_messages[transaction.n_approvals++] = approval_message;
	// 	}

	// 	// approval_messages = &approval_message_transaction[0];
	// 	// transaction.n_approvals = 2;
	// }

	// packedLength = common__approval__get_packed_size(&approval_message);
	// packedData = BoatMalloc(packedLength);
	// common__approval__pack(&approval_message, packedData);

	// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "approval result",
	// 					 packedData,
	// 					 packedLength);

	packedLength = common__transaction__get_packed_size(&transaction);
	packedData = BoatMalloc(packedLength);
	common__transaction__pack(&transaction, packedData);
	/* step-7: packed data assignment */
	/* ---grpcHeader compute */
	grpcHeader[0] = 0x00; //uncompressed
	for (int i = 0; i < 4; i++)
	{
		grpcHeader[i + 1] = (packedLength >> (32 - 8 * (i + 1))) & 0xFF;
	}
	/* ---generate packed data */
	tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len = packedLength + sizeof(grpcHeader);
	memcpy(tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr, grpcHeader, sizeof(grpcHeader));
	// common__envelope__pack(&envelope, &packedData[sizeof(grpcHeader)]);
	memcpy(&tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr[sizeof(grpcHeader)], packedData, packedLength);

	/* boat catch handle */
	boat_catch(hlhuaweiProposalTransactionPacked_exception)
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

#endif /* end of PROTOCOL_USE_HLFABRIC */
