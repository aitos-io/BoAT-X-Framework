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
chaninmaker.c contains functions to construct a raw transaction, perform it and 
wait for its receipt.
*/

/* self-header include */
#include "boatconfig.h"
#include "boatchainmaker.h"
#include "http2intf.h"
#include "boatplatform_internal.h"
#include "common/request.pb-c.h"
#include "common/transaction.pb-c.h"
#include "common/common.pb-c.h"

BOAT_RESULT generateTxRequestPayloadPack(BoatHlchainmakerTx *tx_ptr, char *method, char* contract_name, BoatFieldVariable *output_ptr)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	int i;

	BUINT32 packed_length_payload;
	Common__TransactPayload transactPayload = COMMON__TRANSACT_PAYLOAD__INIT;
	Common__KeyValuePair keyValuePair       = COMMON__KEY_VALUE_PAIR__INIT;
	transactPayload.contract_name           = contract_name;
	transactPayload.method                  = method;

	transactPayload.parameters = (Common__KeyValuePair**)BoatMalloc(sizeof(Common__KeyValuePair*) * tx_ptr->trans_para.n_parameters);
	for (i = 0; i < tx_ptr->trans_para.n_parameters; i++)
	{
		Common__KeyValuePair* key_value_pair = BoatMalloc(sizeof(Common__KeyValuePair));
		memcpy(key_value_pair, &keyValuePair, sizeof(Common__KeyValuePair));
		key_value_pair->key   = tx_ptr->trans_para.parameters[i].key;
		key_value_pair->value = tx_ptr->trans_para.parameters[i].value;
		transactPayload.parameters[i] = key_value_pair;
	}	

	transactPayload.n_parameters  = tx_ptr->trans_para.n_parameters;
	/* pack the Common__TransactPayload */
	packed_length_payload = common__transact_payload__get_packed_size(&transactPayload);
	output_ptr->field_ptr = BoatMalloc(packed_length_payload);
	common__transact_payload__pack(&transactPayload, output_ptr->field_ptr);
	output_ptr->field_len = packed_length_payload;

	for (i = 0; i < transactPayload.n_parameters; i++)
	{	
		BoatFree(transactPayload.parameters[i]);
	}	
	BoatFree(transactPayload.parameters);	

	return result;
}


BOAT_RESULT hlchainmakerTransactionPacked(BoatHlchainmakerTx *tx_ptr, BCHAR* method, BCHAR* contract_name, TxType tx_type, char* tx_id)
{
	Common__TxRequest  tx_request  = COMMON__TX_REQUEST__INIT;
	Common__TxHeader   tx_header   = COMMON__TX_HEADER__INIT;
	Accesscontrol__SerializedMember sender = ACCESSCONTROL__SERIALIZED_MEMBER__INIT;
	
	BoatFieldVariable payloadPacked = {NULL, 0};
	BoatFieldVariable hash_data = {NULL, 0};
	BoatSignatureResult signatureResult;
	BUINT8   grpcHeader[5];
	BUINT8   hash[32];
	BUINT32  packedLength;
	BUINT32  packedHeaderLength;
	BUINT8  *packedData = NULL;
	
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
		
	/* step-0: param in check */
	if ((tx_ptr == NULL ) || (tx_ptr->wallet_ptr == NULL) || \
		(tx_ptr->wallet_ptr->http2Context_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter should not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	sender.org_id             = tx_ptr->wallet_ptr->node_info.org_id_info;
	sender.member_info.len    = tx_ptr->wallet_ptr->user_cert_info.cert.field_len;
	sender.member_info.data   = tx_ptr->wallet_ptr->user_cert_info.cert.field_ptr;
	sender.is_full_cert       = true;

	tx_header.chain_id        = tx_ptr->wallet_ptr->node_info.chain_id_info;
	tx_header.tx_type         = tx_type;
	tx_header.tx_id           = tx_id;
	tx_header.timestamp       = BoatGetTimes();
	tx_header.sender          = &sender;
	result = generateTxRequestPayloadPack(tx_ptr, method, contract_name, &payloadPacked);

	/* step-2: compute payload packed length */
	packedHeaderLength = common__tx_header__get_packed_size__chainmaker(&tx_header);
	packedLength = packedHeaderLength + payloadPacked.field_len;

	hash_data.field_ptr = BoatMalloc(packedLength);
	hash_data.field_len = packedLength;
	common__tx_header__pack__chainmaker(&tx_header, hash_data.field_ptr);
	hash_data.field_ptr += packedHeaderLength;
	memcpy(hash_data.field_ptr, payloadPacked.field_ptr, payloadPacked.field_len);
	hash_data.field_ptr -= packedHeaderLength;

	/* step-3: compute hash */
	result = BoatHash(BOAT_HASH_SHA256,hash_data.field_ptr, 
					   hash_data.field_len, hash, NULL, NULL);

	if (result != BOAT_SUCCESS) {

		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		boat_throw(result, chainmakerProposalTransactionPacked_exception);
	}

	/* step-4: signature */
	result = BoatSignature(tx_ptr->wallet_ptr->user_cert_info.prikeyCtx, hash, sizeof(hash), &signatureResult, NULL );

	if( result != BOAT_SUCCESS ) {

		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
		boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, chainmakerProposalTransactionPacked_exception);
	}

	if (!signatureResult.pkcs_format_used) {

		BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
		boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, chainmakerProposalTransactionPacked_exception);
	}

	/* step-5: pack the envelope */
	tx_request.header          = &tx_header;
	tx_request.payload.len     = payloadPacked.field_len;
	tx_request.payload.data    = payloadPacked.field_ptr;
	tx_request.signature.len   = signatureResult.pkcs_sign_length;
	tx_request.signature.data  = signatureResult.pkcs_sign;
	packedLength               = common__tx_request__get_packed_size(&tx_request);

	/* step-6: packed length assignment */
	tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len = packedLength + sizeof(grpcHeader);
	if( tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len > BOAT_HLCHAINMAKER_HTTP2_SEND_BUF_MAX_LEN )
	{
		BoatLog(BOAT_LOG_CRITICAL, "packed length out of sendbuffer size limit.");
	}
	
	/* step-7: packed data assignment */
	/* ---grpcHeader compute */
	grpcHeader[0] = 0x00;//uncompressed
	for (int i = 0 ; i < 4; i++)
	{
		grpcHeader[i + 1] = (packedLength >> (32 - 8*(i+1)))&0xFF;
	}

	/* ---generate packed data */
	packedData = tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr;
	memcpy(packedData, grpcHeader, sizeof(grpcHeader));
	common__tx_request__pack(&tx_request, &packedData[sizeof(grpcHeader)]);

	/* boat catch handle */
	boat_catch(chainmakerProposalTransactionPacked_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
     }

	/* free malloc */
    if (payloadPacked.field_ptr != NULL) {

    	BoatFree(payloadPacked.field_ptr);
    }

    if (hash_data.field_ptr != NULL) {
    	
    	BoatFree(hash_data.field_ptr);
    }
	
	return result;
}











