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
#include "transaction.pb-c-huawei.h"
#include "contract.pb-c.h"

/*! @ingroup fabricp protocol-fabric
 * @{
 */

/*!****************************************************************************
 * @brief signature header packed
 *
 * @details
 *   fabric has two types of headers: signature header and channel header, this
 *   function is the implemention of signature header protobuf pack.
 *   \n signathre header is consist of creator field and nonce field. Nonce 
 *   field is a 24 bytes length random number; creator field is made up sender's
 *   organization name and certificate.
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param[out] txIdBin 
 *   Binary format transaction id. Transaction id is a 32 bytes length hash, compute 
 *   by a formula that 'sha256(nonce + protobuf(creator))'. Caller should provide
 *   enough space to store it.
 *
 * @param[out] output_ptr 
 *   A structure pointer to store signature header protobuf serialize data and length. 
 *   In internal of this function, the memory of store serialize data has been alloced,
 *   caller SHOULD NOT alloc memory for this pointer again.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code. 
 *
 * @see hlhuaweiTxHeaderPacked
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT hlfabricSignatureHeaderPacked(const BoatHlfabricTx *tx_ptr,
													   BUINT8 *txIdBin,
													   BoatFieldVariable *output_ptr)
{
	Common__Approval approval_message = COMMON__APPROVAL__INIT;
	BUINT32 packedLength;
	BUINT8 hash[32];
	BoatSignatureResult signatureResult;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	/* step-0: param in check */
	if (tx_ptr->wallet_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr cannot be NULL.");
		return BOAT_ERROR;
	}

	/* step-1: signatureHeader packed */
	/* -creator */
	/* --------> organization name */
	// identity_message.org              = (BCHAR *)tx_ptr->var.orgName;
	/* --------> certificate */
	approval_message.identity.len = tx_ptr->wallet_ptr->account_info.cert.field_len;
	approval_message.identity.data = tx_ptr->wallet_ptr->account_info.cert.field_ptr;

	/* --------> creator packed */

	/* step-3: compute hash */
	result = BoatHash(BOAT_HASH_SHA256, tx_ptr->wallet_ptr->account_info.cert.field_ptr,
					  tx_ptr->wallet_ptr->account_info.cert.field_len, hash, NULL, NULL);
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
	// approval_message.type = COMMON__CONTRACT_RUN_ENV__Native;

	packedLength = common__approval__get_packed_size(&approval_message);
	output_ptr->field_ptr = BoatMalloc(packedLength);
	output_ptr->field_len = packedLength;
	common__approval__pack(&approval_message, output_ptr->field_ptr);

	/* boat catch handle */
	boat_catch(hlfabricSignatureHeaderPacked_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	return result;
}

/*!****************************************************************************
 * @brief channel header packed
 * 
 * @details
 *   fabric has two types of headers: signature header and channel header, this
 *   function is the implemention of signature header protobuf pack.
 *   \n channel header is consist of follow fields:
 *       1. type
 *       2. version
 *       3. timestamp
 *       4. channel Id
 *       5. transaction Id
 *       6. extension
 *   channel header packed that is protobuf serialize of above fields.
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param[in] txIdBin 
 *   generate transaction id in #hlfabricSignatureHeaderPacked
 *
 * @param[out] output_ptr 
 *   A structure pointer to store signature header protobuf serialize data and length. 
 *   In internal of this function, the memory of store serialize data has been alloced,
 *   caller SHOULD NOT alloc memory for this pointer again.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code. 
 *
 * @see hlfabricSignatureHeaderPacked
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT hlhuaweiTxHeaderPacked(const BoatHlfabricTx *tx_ptr,
												const BUINT8 *txIdBin,
												BoatFieldVariable *output_ptr)
{

	Common__TxHeader txhead = COMMON__TX_HEADER__INIT;
	BUINT32 packedLength;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	/* step-1: channelHeader packed */
	/* -type */
	txhead.chain_id = tx_ptr->var.chaincodeId.name;
	txhead.nonce = UtilityBuint8Buf2Uint64(tx_ptr->var.nonce.field,sizeof(tx_ptr->var.nonce.field));
	txhead.timestamp = tx_ptr->var.timestamp.nanos;
	txhead.creator->org = tx_ptr->var.orgName;
	txhead.creator->id.data = tx_ptr->var.channelId;
	txhead.creator->id.len = strlen(tx_ptr->var.channelId);

	/* pack the channelHeader */
	packedLength = common__tx_header__get_packed_size(&txhead);
	output_ptr->field_ptr = BoatMalloc(packedLength);
	output_ptr->field_len = packedLength;
	common__channel_header__pack(&txhead, output_ptr->field_ptr);

	/* boat catch handle */
	boat_catch(hlhuaweiTxHeaderPacked_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	/* free malloc */

	return result;
}

/*!****************************************************************************
 * @brief proposal payload data packed
 * 
 * @details
 *   proposal payload is consist of language type field , chaincode id field and 
 *   input field.
 *   \n Language type indicate chaincode coding language, e.g. the chaincode is 
 *   coding by go, or java, or other support languages. chaincode id is made up
 *   path, name, and version. Input field is made up chaincode command arguments, 
 *   e.g. "query xx ","invoke xx xx xx"
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
__BOATSTATIC BOAT_RESULT hlfabricProposalPayloadDataPacked(BoatHlfabricTx *tx_ptr,
														   BoatFieldVariable *output_ptr)
{
	Protos__ChaincodeProposalPayload chaincodeProposalPayload = PROTOS__CHAINCODE_PROPOSAL_PAYLOAD__INIT;
	Protos__ChaincodeInvocationSpec chaincodeInvocationSpec = PROTOS__CHAINCODE_INVOCATION_SPEC__INIT;
	Protos__ChaincodeSpec chaincodeSpec = PROTOS__CHAINCODE_SPEC__INIT;
	Protos__ChaincodeID chaincodeId = PROTOS__CHAINCODE_ID__INIT;
	Protos__ChaincodeInput input = PROTOS__CHAINCODE_INPUT__INIT;
	ProtobufCBinaryData argsTmp[BOAT_HLFABRIC_ARGS_MAX_NUM];
	BUINT8 *chaincodeInvocationSpecBuffer = NULL;
	BUINT16 packedLength;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	/* chaincodeInvocationSpec */
	chaincodeInvocationSpec.chaincode_spec = &chaincodeSpec;
	/* ------>chaincodeSpec */
	/* ------------>type */
	chaincodeSpec.has_type = true;
	chaincodeSpec.type = PROTOS__CHAINCODE_SPEC__TYPE__GOLANG;
	/* ------------>chaincodeId */
	chaincodeInvocationSpec.chaincode_spec->chaincode_id = &chaincodeId;
	chaincodeId.path = (BCHAR *)tx_ptr->var.chaincodeId.path;
	chaincodeId.name = (BCHAR *)tx_ptr->var.chaincodeId.name;
	chaincodeId.version = (BCHAR *)tx_ptr->var.chaincodeId.version;
	/* ------------>input */
	chaincodeInvocationSpec.chaincode_spec->input = &input;
	input.n_args = tx_ptr->var.args.nArgs;
	for (int i = 0; i < BOAT_HLFABRIC_ARGS_MAX_NUM; i++)
	{
		argsTmp[i].data = (BUINT8 *)tx_ptr->var.args.args[i];
		if (tx_ptr->var.args.args[i] != NULL)
		{
			argsTmp[i].len = strlen(tx_ptr->var.args.args[i]);
		}
		else
		{
			argsTmp[i].len = 0;
		}
	}
	input.args = argsTmp;
	/* ------------>timeout */
	chaincodeInvocationSpec.chaincode_spec->has_timeout = false;
	/* chaincodeInvocationSpec pack */
	packedLength = protos__chaincode_invocation_spec__get_packed_size(&chaincodeInvocationSpec);
	chaincodeInvocationSpecBuffer = BoatMalloc(packedLength);
	if (NULL == chaincodeInvocationSpecBuffer)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate chaincodeInvocationSpecBuffer.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, hlfabricProposalPayloadDataPacked_exception);
	}
	protos__chaincode_invocation_spec__pack(&chaincodeInvocationSpec, chaincodeInvocationSpecBuffer);

	/* chaincodeProposalPayload pack */
	chaincodeProposalPayload.has_input = true;
	chaincodeProposalPayload.input.len = packedLength;
	chaincodeProposalPayload.input.data = chaincodeInvocationSpecBuffer;
	packedLength = protos__chaincode_proposal_payload__get_packed_size(&chaincodeProposalPayload);
	output_ptr->field_ptr = BoatMalloc(packedLength);
	output_ptr->field_len = packedLength;
	protos__chaincode_proposal_payload__pack(&chaincodeProposalPayload, output_ptr->field_ptr);

	/* boat catch handle */
	boat_catch(hlfabricProposalPayloadDataPacked_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	/* free malloc */
	BoatFree(chaincodeInvocationSpecBuffer);

	return result;
}

/*!****************************************************************************
 * @brief  transaction payload data packed
 *
 * @details
 *   The composition of transaction is shown as follow:
   @verbatim
   
                                   /signatureHeader             
   transaction -->transactionAction 
                                   \                       /chaincodeProposalPayload
                                    \chaincodeActionPayload
  								                           \chaincodeEndorsedAction
 * @endverbatim
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
__BOATSTATIC BOAT_RESULT hlfabricTransactionPayloadDataPacked(BoatHlfabricTx *tx_ptr,
															  BoatFieldVariable *output_ptr)
{
	Protos__Endorsement endorsement[BOAT_HLFABRIC_ENDORSER_MAX_NUM] = {PROTOS__ENDORSEMENT__INIT};
	Protos__Endorsement *endorsement_ptr[BOAT_HLFABRIC_ENDORSER_MAX_NUM];
	Protos__Transaction transaction = PROTOS__TRANSACTION__INIT;
	Protos__TransactionAction transactionAction = PROTOS__TRANSACTION_ACTION__INIT;
	Protos__TransactionAction *transactionAction_ptr = NULL;
	Protos__ChaincodeActionPayload chaincodeActionPayload = PROTOS__CHAINCODE_ACTION_PAYLOAD__INIT;
	Protos__ChaincodeEndorsedAction chaincodeEndorsedAction = PROTOS__CHAINCODE_ENDORSED_ACTION__INIT;
	//BUINT8 *chaincodeProposalPayloadBuffer                    = NULL;
	BUINT8 *chaincodeActionPayloadBuffer = NULL;
	BUINT32 transactionBufferLen;
	BUINT32 chaincodeActionPayloadBufferLen;
	BUINT8 txIdBin[32];
	BoatFieldVariable signatureHeadPacked;
	BoatFieldVariable payloadDataPacked;
	BUINT16 i = 0;
	BOAT_RESULT result = BOAT_SUCCESS;

	boat_try_declare;

	/* chaincode_proposal_payload */
	result = hlfabricProposalPayloadDataPacked(tx_ptr, &payloadDataPacked);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricProposalPayloadDataPacked[size].");
		boat_throw(result, hlfabricTransactionPayloadDataPacked_exception);
	}

	/* chaincodeEndorsedAction */
	chaincodeEndorsedAction.has_proposal_response_payload = true;
	chaincodeEndorsedAction.proposal_response_payload.len = tx_ptr->endorserResponse.response[0].payload.field_len;
	chaincodeEndorsedAction.proposal_response_payload.data = tx_ptr->endorserResponse.response[0].payload.field_ptr;
	chaincodeEndorsedAction.n_endorsements = tx_ptr->endorserResponse.responseCount;
	for (i = 0; i < sizeof(endorsement) / sizeof(endorsement[0]); i++)
	{
		endorsement[i] = endorsement[0];
		endorsement_ptr[i] = &endorsement[i];
	}
	chaincodeEndorsedAction.endorsements = endorsement_ptr;
	for (i = 0; i < chaincodeEndorsedAction.n_endorsements; i++)
	{
		endorsement[i].has_endorser = true;
		endorsement[i].endorser.len = tx_ptr->endorserResponse.response[i].endorser.field_len;
		endorsement[i].endorser.data = tx_ptr->endorserResponse.response[i].endorser.field_ptr;
		endorsement[i].has_signature = true;
		endorsement[i].signature.len = tx_ptr->endorserResponse.response[i].signature.field_len;
		endorsement[i].signature.data = tx_ptr->endorserResponse.response[i].signature.field_ptr;
	}

	/* ChaincodeActionPayload */
	/* ------>chaincode_proposal_payload assignment */
	chaincodeActionPayload.has_chaincode_proposal_payload = true;
	chaincodeActionPayload.chaincode_proposal_payload.len = payloadDataPacked.field_len;
	chaincodeActionPayload.chaincode_proposal_payload.data = payloadDataPacked.field_ptr;
	/* ------>chaincodeEndorsedAction assignment */
	chaincodeActionPayload.action = &chaincodeEndorsedAction;
	/* ------>pack the chaincodeActionPayload */
	chaincodeActionPayloadBufferLen = protos__chaincode_action_payload__get_packed_size(&chaincodeActionPayload);
	chaincodeActionPayloadBuffer = BoatMalloc(chaincodeActionPayloadBufferLen);
	if (NULL == chaincodeActionPayloadBuffer)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate chaincodeActionPayloadBuffer.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, hlfabricTransactionPayloadDataPacked_exception);
	}
	protos__chaincode_action_payload__pack(&chaincodeActionPayload, chaincodeActionPayloadBuffer);

	/* signature header */
	transactionAction.has_header = true;
	result = hlfabricSignatureHeaderPacked(tx_ptr, txIdBin, &signatureHeadPacked);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricSignatureHeaderPacked.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, hlfabricTransactionPayloadDataPacked_exception);
	}

	/* transactionAction */
	transactionAction.header.len = signatureHeadPacked.field_len;
	transactionAction.header.data = signatureHeadPacked.field_ptr;
	transactionAction.has_payload = true;
	transactionAction.payload.len = chaincodeActionPayloadBufferLen;
	transactionAction.payload.data = chaincodeActionPayloadBuffer;

	/* transaction */
	transaction.n_actions = 1;
	transactionAction_ptr = &transactionAction;
	transaction.actions = &transactionAction_ptr;
	transactionBufferLen = protos__transaction__get_packed_size(&transaction);
	output_ptr->field_ptr = BoatMalloc(transactionBufferLen);
	output_ptr->field_len = transactionBufferLen;
	protos__transaction__pack(&transaction, output_ptr->field_ptr);

	/* boat catch handle */
	boat_catch(hlfabricTransactionPayloadDataPacked_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	/* free malloc */
	BoatFree(signatureHeadPacked.field_ptr);
	// BoatFree(chaincodeProposalPayloadBuffer);
	BoatFree(chaincodeActionPayloadBuffer);
	BoatFree(payloadDataPacked.field_ptr);

	return result;
}

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
	BoatFieldVariable payloadDataPacked = {NULL, 0};
	BoatSignatureResult signatureResult;
	BUINT8 txIdBin[32];
	BUINT8 hash[32];
	BUINT32 packedLength;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
	contractInvocation.contract_name = tx_ptr->var.chaincodeId.name;
	contractInvocation.args = BoatMalloc(sizeof(ProtobufCBinaryData));
	contractInvocation.n_args = 1;
	contractInvocation.args[0].data = tx_ptr->var.args.args[1];
	contractInvocation.args[0].len = strlen(tx_ptr->var.args.args[1]);
	contractInvocation.func_name = tx_ptr->var.args.args[0];
	txPayload.data.len = common__contract_invocation__get_packed_size(&contractInvocation);

	txPayload.data.data = BoatMalloc(txPayload.data.len);
	common__contract_invocation__pack(&contractInvocation, txPayload.data.data);

	// protobuf_c_message_free_unpacked(&contractInvocation,NULL);
	/* -type */
	txhead.chain_id = tx_ptr->var.chaincodeId.path;
	// protobuf_c_message_free_unpacked(&contractI
	// txhead.nonce = UtilityBuint8Buf2Uint64(tx_ptr->var.nonce.field,sizeof(tx_ptr->var.nonce.field));
	// txhead.nonce = 0x3333333333333333;
	txhead.timestamp = (long)(((double)tx_ptr->var.timestamp.sec)*60*1000000.0);
	// txhead.timestamp = 0x5555555555555555;
	// txhead.nonce = txhead.timestamp;
	// txhead.nonce = UtilityBuint8Buf2Uint64(tx_ptr->var.nonce.field,sizeof(tx_ptr->var.nonce.field));
	identity_creator.id.data = tx_ptr->var.channelId;
	identity_creator.id.len = strlen(tx_ptr->var.channelId);
	identity_creator.org = tx_ptr->var.orgName;
	txhead.creator = &identity_creator;
	// txhead.type = COMMON__TX_TYPE__VOTE_TRANSACTION;

	txPayload.header = &txhead;

	packedLength = common__tx_payload__get_packed_size(&txPayload);
	output_ptr->field_len = packedLength;
	output_ptr->field_ptr = BoatMalloc(packedLength);
	common__tx_payload__pack(&txPayload, output_ptr->field_ptr);


	/* boat catch handle */
	boat_catch(hlhuaweiPayloadPacked_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	/* free malloc */
	BoatFree(txPayload.data.data);
	return result;
}

BOAT_RESULT hlhuaweiProposalTransactionPacked(BoatHlfabricTx *tx_ptr)
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
	transaction.n_approvals = 1;
	// transaction.approvals = BoatMalloc(sizeof(Common__Approval));
	approval_messages = &approval_message;
	transaction.approvals = &approval_messages;

	// packedLength = common__approval__get_packed_size(&approval_message);
	// packedData = BoatMalloc(packedLength);
	// common__approval__pack(&approval_message, packedData);

	// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "approval result",
	// 					 packedData,
	// 					 packedLength);


	packedLength = common__transaction__get_packed_size(&transaction);
	packedData = BoatMalloc(packedLength);
	common__transaction__pack(&transaction, packedData);


#if 0
	/* step-3: compute hash */
	result = BoatHash( BOAT_HASH_SHA256,payloadPacked.field_ptr, 
					   payloadPacked.field_len, hash, NULL, NULL );
	if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		boat_throw(result, hlhuaweiProposalTransactionPacked_exception);
	}
	
	/* step-4: signature */
	result = BoatSignature( tx_ptr->wallet_ptr->account_info.prikeyCtx,
							hash, sizeof(hash), &signatureResult, NULL );
	if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
		boat_throw(BOAT_ERROR_GEN_SIGNATURE_FAILED, hlhuaweiProposalTransactionPacked_exception);
	}
	
	if( !signatureResult.pkcs_format_used )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
		boat_throw(BOAT_ERROR_GEN_SIGNATURE_FAILED, hlhuaweiProposalTransactionPacked_exception);
	}

	/* step-5: pack the envelope */
	envelope.has_payload     = true;
	envelope.payload.len     = payloadPacked.field_len;
	envelope.payload.data    = payloadPacked.field_ptr;
	// envelope.has_signature   = signatureResult.pkcs_format_used;
	// envelope.signature.len   = signatureResult.pkcs_sign_length;
	// envelope.signature.data  = signatureResult.pkcs_sign;
	packedLength             = common__envelope__get_packed_size(&envelope);
	
	/* step-6: packed length assignment */
	tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len = packedLength + sizeof(grpcHeader);
	if( tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len > BOAT_HLFABRIC_HTTP2_SEND_BUF_MAX_LEN )
	{
		BoatLog(BOAT_LOG_CRITICAL, "packed length out of sendbuffer size limit.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, hlhuaweiProposalTransactionPacked_exception);
	}
#endif
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
