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
boatchainmaker.c contains functions to construct a raw transaction, perform it and
wait for its receipt.
*/

/* self-header include */
#include "boatconfig.h"
#include "boatchainmaker.h"
#include "http2intf.h"
#include "boatplatform_internal.h"
#include "common/request.pb-c.h"
#include "common/transaction.pb-c.h"

BOAT_RESULT hlchainmakerTransactionPacked(BoatChainmakerTx *tx_ptr, BCHAR *method, BCHAR *contract_name, TxType tx_type, char *tx_id)
{
	BUINT32 i;
	boat_try_declare;
	BOAT_RESULT result = BOAT_SUCCESS;

	Common__TxRequest common_tx_request = COMMON__TX_REQUEST__INIT;
	Common__KeyValuePair common_key_value_pair = COMMON__KEY_VALUE_PAIR__INIT;
	Common__ChainmakerPayload common_payload = COMMON__CHAINMAKER_PAYLOAD__INIT;
	Common__Limit common_limit = COMMON__LIMIT__INIT;
	Common__EndorsementEntry common_endorsement_entry = COMMON__ENDORSEMENT_ENTRY__INIT;
	Accesscontrol__Member accesscontrol_member = ACCESSCONTROL__MEMBER__INIT;

	BoatFieldVariable hash_data = {NULL, 0};
	BoatSignatureResult signatureResult;
	BUINT8 grpcHeader[5];
	BUINT8 hash_result[32];
	BUINT32 packedLength;
	BUINT8 *packedData = NULL;

	/* step-0: param in check */
	if ((tx_ptr == NULL) || (tx_ptr->wallet_ptr == NULL) ||
		(tx_ptr->wallet_ptr->http2Context_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter should not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	/* step-1: payload data generate */
	common_payload.chain_id = tx_ptr->wallet_ptr->network_info.chain_id;
	common_payload.tx_type = tx_type;
	common_payload.tx_id = tx_id;
	common_payload.timestamp = BoatGetTimes();

	common_payload.contract_name = contract_name;
	common_payload.method = method;
	common_payload.sequence = 0;

	common_limit.gas_limit = tx_ptr->gas_limit;
	common_payload.limit = &common_limit;

	// Common__KeyValuePair parameters
	common_payload.n_parameters = tx_ptr->trans_para.n_parameters;
	common_payload.parameters = (Common__KeyValuePair **)BoatMalloc(sizeof(Common__KeyValuePair *) * tx_ptr->trans_para.n_parameters);
	if (common_payload.parameters == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail parameters malloc");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, chainmakerProposalTransactionPacked_exception);
	}

	for (i = 0; i < common_payload.n_parameters; i++)
	{
		Common__KeyValuePair *key_value_pair_ptr = BoatMalloc(sizeof(Common__KeyValuePair));
		if (key_value_pair_ptr == NULL)
		{
			boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, chainmakerProposalTransactionPacked_exception);
		}
		memcpy(key_value_pair_ptr, &common_key_value_pair, sizeof(Common__KeyValuePair));
		key_value_pair_ptr->key = tx_ptr->trans_para.parameters[i].key;
		key_value_pair_ptr->value.len = tx_ptr->trans_para.parameters[i].value.field_len;
		key_value_pair_ptr->value.data = tx_ptr->trans_para.parameters[i].value.field_ptr;
		common_payload.parameters[i] = key_value_pair_ptr;
	}

	/* step-2: compute payload packed length */
	packedLength = common__chainmaker_payload__get_packed_size(&common_payload);
	hash_data.field_ptr = BoatMalloc(packedLength);
	if (hash_data.field_ptr == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "Failed to allocate memory.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}

	hash_data.field_len = packedLength;
	common__chainmaker_payload__pack(&common_payload, hash_data.field_ptr);

	/* step-3: compute hash */
	result = BoatHash(BOAT_HASH_SHA256, hash_data.field_ptr,
					  hash_data.field_len, hash_result, NULL, NULL);

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		boat_throw(result, chainmakerProposalTransactionPacked_exception);
	}

	/* step-4: signature */
	result = BoatSignature(tx_ptr->wallet_ptr->account_info.prikeyCtx, hash_result, sizeof(hash_result), &signatureResult, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
		boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, chainmakerProposalTransactionPacked_exception);
	}

	if (!signatureResult.pkcs_format_used)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
		boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, chainmakerProposalTransactionPacked_exception);
	}

	/**************************  sender  start *************************/
	accesscontrol_member.org_id = tx_ptr->wallet_ptr->network_info.org_id;
	accesscontrol_member.member_type = tx_ptr->wallet_ptr->network_info.client_member_type;
	accesscontrol_member.member_info.len = tx_ptr->wallet_ptr->network_info.client_sign_cert_content.length;
	accesscontrol_member.member_info.data = (uint8_t *)tx_ptr->wallet_ptr->network_info.client_sign_cert_content.content;

	common_endorsement_entry.signer = &accesscontrol_member;
	common_endorsement_entry.signature.len = signatureResult.pkcs_sign_length;
	common_endorsement_entry.signature.data = signatureResult.pkcs_sign;

	/* step-5: pack the envelope */
	common_tx_request.payload = &common_payload;
	common_tx_request.sender = &common_endorsement_entry;
	packedLength = common__tx_request__get_packed_size(&common_tx_request);

	/* print chainmaker transaction message */
	BoatLog(BOAT_LOG_VERBOSE, "Transaction Message tx_id = %s", tx_id);
	BoatLog(BOAT_LOG_VERBOSE, "Transaction Message time = %lld", common_payload.timestamp);

	for (i = 0; i < common_payload.n_parameters; i++)
	{
		BoatLog(BOAT_LOG_VERBOSE, "Transaction Message key = %s, value = %s", common_payload.parameters[i]->key, common_payload.parameters[i]->value.data);
	}

	/* step-6: packed length assignment */
	((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->sendBuf.field_len = packedLength + sizeof(grpcHeader);
	if (((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->sendBuf.field_len > BOAT_HLCHAINMAKER_HTTP2_SEND_BUF_MAX_LEN)
	{
		BoatLog(BOAT_LOG_CRITICAL, "packed length out of sendbuffer size limit.");
	}

	/* step-7: packed data assignment */
	/* ---grpcHeader compute */
	grpcHeader[0] = 0x00; // uncompressed
	for (int i = 0; i < 4; i++)
	{
		grpcHeader[i + 1] = (packedLength >> (32 - 8 * (i + 1))) & 0xFF;
	}

	/* ---generate packed data */
	packedData = ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->sendBuf.field_ptr;
	memcpy(packedData, grpcHeader, sizeof(grpcHeader));

	common__tx_request__pack(&common_tx_request, &packedData[sizeof(grpcHeader)]);
	/* boat catch handle */
	boat_catch(chainmakerProposalTransactionPacked_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	/* free malloc */
	if (common_payload.parameters != NULL)
	{
		for (i = 0; i < common_payload.n_parameters; i++)
		{
			if (common_payload.parameters[i] != NULL)
			{
				BoatFree(common_payload.parameters[i]);
			}
		}
		BoatFree(common_payload.parameters);
	}

	if (hash_data.field_ptr != NULL)
	{
		BoatFree(hash_data.field_ptr);
	}

	return result;
}
