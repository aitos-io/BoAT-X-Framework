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

#if PROTOCOL_USE_HLFABRIC == 1
#include "http2intf.h"
#include "boatplatform_internal.h"
/* protos header include */
#include "common/common.pb-c.h"
#include "peer/chaincode.pb-c.h"
#include "peer/proposal.pb-c.h"
#include "msp/identities.pb-c.h"
#include "peer/transaction.pb-c.h"
#include "peer/proposal_response.pb-c.h"

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
 * @see hlfabricChannelHeaderPacked
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT hlfabricSignatureHeaderPacked(const BoatHlfabricTx *tx_ptr,
													   BUINT8 *txIdBin,
													   BoatFieldVariable *output_ptr)
{
    Common__SignatureHeader signatureHeader     = COMMON__SIGNATURE_HEADER__INIT;
	Msp__SerializedIdentity serializedIdentity  = MSP__SERIALIZED_IDENTITY__INIT;
	BUINT8 *serializedIdentityBuffer            = NULL;
    BUINT32 packedLength;
	BoatFieldVariable txIdRawMaterial           = {NULL, 0};

	BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
	
	/* step-0: param in check */
	if( tx_ptr->wallet_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	
	/* step-1: signatureHeader packed */
	/* -creator */
	/* --------> organization name */
	serializedIdentity.mspid          = (BCHAR *)tx_ptr->var.orgName;
	/* --------> certificate */
	serializedIdentity.has_id_bytes   = true;
	serializedIdentity.id_bytes.len   = tx_ptr->wallet_ptr->account_info.cert.field_len;
	serializedIdentity.id_bytes.data  = tx_ptr->wallet_ptr->account_info.cert.field_ptr;
	/* --------> creator packed */
	packedLength             = msp__serialized_identity__get_packed_size(&serializedIdentity);
    serializedIdentityBuffer = BoatMalloc(packedLength);
	if(NULL == serializedIdentityBuffer)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate serializedIdentityBuffer.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricSignatureHeaderPacked_exception);
	}
    msp__serialized_identity__pack(&serializedIdentity, serializedIdentityBuffer);
	/* -------->creator value assignment */
	signatureHeader.has_creator   = true;
	signatureHeader.creator.len   = packedLength;
	signatureHeader.creator.data  = serializedIdentityBuffer;
	/* -nonce */
	signatureHeader.has_nonce     = true;
	signatureHeader.nonce.len     = tx_ptr->var.nonce.field_len;
	signatureHeader.nonce.data    = ((BoatHlfabricTx*)tx_ptr)->var.nonce.field;

	/* generate txID */
	txIdRawMaterial.field_len     = signatureHeader.nonce.len + signatureHeader.creator.len;
    txIdRawMaterial.field_ptr     = BoatMalloc(txIdRawMaterial.field_len);
    if(NULL == txIdRawMaterial.field_ptr)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate txIdRawMaterial buffer.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricSignatureHeaderPacked_exception);
    }
	
	memcpy( txIdRawMaterial.field_ptr, signatureHeader.nonce.data, signatureHeader.nonce.len );
	memcpy( txIdRawMaterial.field_ptr + signatureHeader.nonce.len, signatureHeader.creator.data, signatureHeader.creator.len );
	result = BoatHash( BOAT_HASH_SHA256, txIdRawMaterial.field_ptr, txIdRawMaterial.field_len, txIdBin, NULL, NULL ); 
	if( result != BOAT_SUCCESS )	
	{
		BoatLog( BOAT_LOG_CRITICAL, "Fail to exec BoatHash." );
        boat_throw( result, hlfabricSignatureHeaderPacked_exception );
	};	
	/* pack the signatureHeader */
	packedLength = common__signature_header__get_packed_size(&signatureHeader);
	output_ptr->field_ptr = BoatMalloc(packedLength);
	output_ptr->field_len = packedLength;
	common__signature_header__pack(&signatureHeader, output_ptr->field_ptr);
    
	/* boat catch handle */
	boat_catch(hlfabricSignatureHeaderPacked_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }
	
	/* free malloc */
	BoatFree(serializedIdentityBuffer);
	BoatFree(txIdRawMaterial.field_ptr);
	
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
__BOATSTATIC BOAT_RESULT hlfabricChannelHeaderPacked(const BoatHlfabricTx *tx_ptr,
													 const BUINT8 *txIdBin,
													 BoatFieldVariable *output_ptr)
{
    Common__ChannelHeader   channelHeader       = COMMON__CHANNEL_HEADER__INIT;
	Google__Protobuf__Timestamp timestamp       = GOOGLE__PROTOBUF__TIMESTAMP__INIT;
	Protos__ChaincodeHeaderExtension chaincodeHeaderExtension = PROTOS__CHAINCODE_HEADER_EXTENSION__INIT;
    Protos__ChaincodeID              chaincodeId              = PROTOS__CHAINCODE_ID__INIT;
    BUINT32 packedLength;
	BCHAR  txIdString[64 + 1];

	BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
	
	/* step-1: channelHeader packed */
	/* -type */
	channelHeader.has_type = true;
    channelHeader.type     = COMMON__HEADER_TYPE__ENDORSER_TRANSACTION;
	/* -timestramp */
	channelHeader.timestamp              = &timestamp;
	channelHeader.timestamp->has_seconds = true;
	channelHeader.timestamp->seconds     = tx_ptr->var.timestamp.sec;
	channelHeader.timestamp->has_nanos   = true;
	channelHeader.timestamp->nanos       = tx_ptr->var.timestamp.nanos;
	/* -channelID */
	channelHeader.channel_id             = (BCHAR *)tx_ptr->var.channelId;
	/* -txID */
	memset( txIdString, 0, sizeof(txIdString) );
	UtilityBinToHex( txIdString, txIdBin, 32, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE );
	channelHeader.tx_id                  = txIdString;
	/* -extension */
	chaincodeHeaderExtension.has_payload_visibility = false;
    chaincodeHeaderExtension.chaincode_id           = &chaincodeId;
    chaincodeHeaderExtension.chaincode_id->path     = (BCHAR *)tx_ptr->var.chaincodeId.path;
    chaincodeHeaderExtension.chaincode_id->name     = (BCHAR *)tx_ptr->var.chaincodeId.name;
    chaincodeHeaderExtension.chaincode_id->version  = (BCHAR *)tx_ptr->var.chaincodeId.version; 

    channelHeader.has_extension  = true;
    channelHeader.extension.len  = protos__chaincode_header_extension__get_packed_size(&chaincodeHeaderExtension);
    channelHeader.extension.data = BoatMalloc( channelHeader.extension.len );
    if( NULL == channelHeader.extension.data )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate channelHeader->extension.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricChannelHeaderPacked_exception);
	}
	protos__chaincode_header_extension__pack(&chaincodeHeaderExtension, channelHeader.extension.data);
	/* pack the channelHeader */
	packedLength = common__channel_header__get_packed_size(&channelHeader);
	output_ptr->field_ptr = BoatMalloc(packedLength);
	output_ptr->field_len = packedLength;
	common__channel_header__pack(&channelHeader, output_ptr->field_ptr);
    
	/* boat catch handle */
	boat_catch( hlfabricChannelHeaderPacked_exception )
	{
        BoatLog( BOAT_LOG_CRITICAL, "Exception: %d", boat_exception );
        result = boat_exception;
    }
	
	/* free malloc */
	BoatFree(channelHeader.extension.data);
	
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
	Protos__ChaincodeProposalPayload  chaincodeProposalPayload = PROTOS__CHAINCODE_PROPOSAL_PAYLOAD__INIT;
	Protos__ChaincodeInvocationSpec   chaincodeInvocationSpec  = PROTOS__CHAINCODE_INVOCATION_SPEC__INIT;
	Protos__ChaincodeSpec  chaincodeSpec  = PROTOS__CHAINCODE_SPEC__INIT;
    Protos__ChaincodeID    chaincodeId    = PROTOS__CHAINCODE_ID__INIT;
    Protos__ChaincodeInput input          = PROTOS__CHAINCODE_INPUT__INIT;
	ProtobufCBinaryData    argsTmp[BOAT_HLFABRIC_ARGS_MAX_NUM];
	BUINT8 *chaincodeInvocationSpecBuffer = NULL;
	BUINT16 packedLength;
	
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
	
	/* chaincodeInvocationSpec */
	chaincodeInvocationSpec.chaincode_spec = &chaincodeSpec;
	/* ------>chaincodeSpec */
	/* ------------>type */
    chaincodeSpec.has_type = true;
    chaincodeSpec.type     = PROTOS__CHAINCODE_SPEC__TYPE__GOLANG;
    /* ------------>chaincodeId */
	chaincodeInvocationSpec.chaincode_spec->chaincode_id = &chaincodeId;
    chaincodeId.path       = (BCHAR *)tx_ptr->var.chaincodeId.path;
    chaincodeId.name       = (BCHAR *)tx_ptr->var.chaincodeId.name;
    chaincodeId.version    = (BCHAR *)tx_ptr->var.chaincodeId.version;
    /* ------------>input */
	chaincodeInvocationSpec.chaincode_spec->input = &input;
    input.n_args           = tx_ptr->var.args.nArgs;
	for( int i = 0; i < BOAT_HLFABRIC_ARGS_MAX_NUM; i++ )
	{
		argsTmp[i].data = (BUINT8 *)tx_ptr->var.args.args[i];
		if(tx_ptr->var.args.args[i] != NULL)
		{
			argsTmp[i].len  = strlen(tx_ptr->var.args.args[i]);
		}
		else
		{
			argsTmp[i].len  = 0;
		}
	}
    input.args             = argsTmp;
	/* ------------>timeout */
	chaincodeInvocationSpec.chaincode_spec->has_timeout = false;
	/* chaincodeInvocationSpec pack */
	packedLength = protos__chaincode_invocation_spec__get_packed_size(&chaincodeInvocationSpec);
	chaincodeInvocationSpecBuffer = BoatMalloc(packedLength);
	if( NULL == chaincodeInvocationSpecBuffer )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate chaincodeInvocationSpecBuffer.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricProposalPayloadDataPacked_exception);
	}
	protos__chaincode_invocation_spec__pack(&chaincodeInvocationSpec, chaincodeInvocationSpecBuffer);
	
	/* chaincodeProposalPayload pack */
	chaincodeProposalPayload.has_input  = true;
	chaincodeProposalPayload.input.len  = packedLength;
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
	Protos__Endorsement  endorsement[BOAT_HLFABRIC_ENDORSER_MAX_NUM] = {PROTOS__ENDORSEMENT__INIT};
    Protos__Endorsement *endorsement_ptr[BOAT_HLFABRIC_ENDORSER_MAX_NUM];
	Protos__Transaction transaction                           = PROTOS__TRANSACTION__INIT;
	Protos__TransactionAction  transactionAction              = PROTOS__TRANSACTION_ACTION__INIT;
    Protos__TransactionAction *transactionAction_ptr          = NULL;
	Protos__ChaincodeActionPayload chaincodeActionPayload     = PROTOS__CHAINCODE_ACTION_PAYLOAD__INIT;
	Protos__ChaincodeEndorsedAction chaincodeEndorsedAction   = PROTOS__CHAINCODE_ENDORSED_ACTION__INIT;
	//BUINT8 *chaincodeProposalPayloadBuffer                    = NULL;
	BUINT8 *chaincodeActionPayloadBuffer                      = NULL;	
	BUINT32 transactionBufferLen;
	BUINT32 chaincodeActionPayloadBufferLen;
	BUINT8 txIdBin[32];
	BoatFieldVariable signatureHeadPacked;
	BoatFieldVariable payloadDataPacked;
	BUINT16      i = 0;
	BOAT_RESULT result = BOAT_SUCCESS;

    boat_try_declare;
	
	/* chaincode_proposal_payload */
	result = hlfabricProposalPayloadDataPacked(tx_ptr, &payloadDataPacked);
	if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricProposalPayloadDataPacked[size].");
        boat_throw(result, hlfabricTransactionPayloadDataPacked_exception);
    }
	
	/* chaincodeEndorsedAction */
    chaincodeEndorsedAction.has_proposal_response_payload  = true;
    chaincodeEndorsedAction.proposal_response_payload.len  = tx_ptr->endorserResponse.response[0].payload.field_len;
    chaincodeEndorsedAction.proposal_response_payload.data = tx_ptr->endorserResponse.response[0].payload.field_ptr;
	chaincodeEndorsedAction.n_endorsements                 = tx_ptr->endorserResponse.responseCount;
	for( i = 0; i < sizeof(endorsement)/sizeof(endorsement[0]); i++ )
	{
		endorsement[i]     = endorsement[0];
		endorsement_ptr[i] = &endorsement[i];
	}
    chaincodeEndorsedAction.endorsements                   = endorsement_ptr;
	for( i = 0; i < chaincodeEndorsedAction.n_endorsements; i++ )
	{
		endorsement[i].has_endorser    = true;   
		endorsement[i].endorser.len    = tx_ptr->endorserResponse.response[i].endorser.field_len;
		endorsement[i].endorser.data   = tx_ptr->endorserResponse.response[i].endorser.field_ptr;
		endorsement[i].has_signature   = true;
		endorsement[i].signature.len   = tx_ptr->endorserResponse.response[i].signature.field_len;
		endorsement[i].signature.data  = tx_ptr->endorserResponse.response[i].signature.field_ptr;
	}
	
	/* ChaincodeActionPayload */
	/* ------>chaincode_proposal_payload assignment */
    chaincodeActionPayload.has_chaincode_proposal_payload  = true;
    chaincodeActionPayload.chaincode_proposal_payload.len  = payloadDataPacked.field_len;
    chaincodeActionPayload.chaincode_proposal_payload.data = payloadDataPacked.field_ptr;
	/* ------>chaincodeEndorsedAction assignment */
	chaincodeActionPayload.action = &chaincodeEndorsedAction;
	/* ------>pack the chaincodeActionPayload */
	chaincodeActionPayloadBufferLen = protos__chaincode_action_payload__get_packed_size(&chaincodeActionPayload);
    chaincodeActionPayloadBuffer = BoatMalloc(chaincodeActionPayloadBufferLen);
	if( NULL == chaincodeActionPayloadBuffer )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate chaincodeActionPayloadBuffer.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricTransactionPayloadDataPacked_exception);
	}
    protos__chaincode_action_payload__pack(&chaincodeActionPayload, chaincodeActionPayloadBuffer);
	
	/* signature header */
	transactionAction.has_header  = true;
	result = hlfabricSignatureHeaderPacked(tx_ptr, txIdBin, &signatureHeadPacked);
	if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricSignatureHeaderPacked.");
        boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, hlfabricTransactionPayloadDataPacked_exception);
    }
	
	/* transactionAction */
    transactionAction.header.len   = signatureHeadPacked.field_len;
	transactionAction.header.data  = signatureHeadPacked.field_ptr;
	transactionAction.has_payload  = true;
    transactionAction.payload.len  = chaincodeActionPayloadBufferLen;
    transactionAction.payload.data = chaincodeActionPayloadBuffer;
	
	/* transaction */
	transaction.n_actions = 1;
	transactionAction_ptr = &transactionAction;
    transaction.actions   = &transactionAction_ptr;
	transactionBufferLen  = protos__transaction__get_packed_size(&transaction);
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
__BOATSTATIC BOAT_RESULT hlfabricPayloadPacked(BoatHlfabricTx *tx_ptr, 
		                                       BoatFieldVariable *output_ptr)
{
	Common__Header   header   = COMMON__HEADER__INIT;
	Common__Payload  payload  = COMMON__PAYLOAD__INIT;
	BoatFieldVariable signatureHeaderPacked = {NULL, 0};
	BoatFieldVariable channelHeaderPacked   = {NULL, 0};
	BoatFieldVariable payloadDataPacked     = {NULL, 0};
	BUINT8 txIdBin[32];
	BUINT32 packedLength;
	
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
	
	/* payload.header */
	payload.header = &header;
	/* ------>signature header */
	header.has_signature_header = true;
	result = hlfabricSignatureHeaderPacked( tx_ptr, txIdBin, &signatureHeaderPacked );
	header.signature_header.len  = signatureHeaderPacked.field_len;
	header.signature_header.data = signatureHeaderPacked.field_ptr;
	if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricSignatureHeaderPacked.");
        boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, hlfabricPayloadPacked_exception);
    }
	
	/* ------>channel header */
	header.has_channel_header = true;
	result = hlfabricChannelHeaderPacked(tx_ptr, txIdBin, &channelHeaderPacked);
	header.channel_header.len  = channelHeaderPacked.field_len;
	header.channel_header.data = channelHeaderPacked.field_ptr;
	if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricChannelHeaderPacked.");
        boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, hlfabricPayloadPacked_exception);
    }
	
	/* payload.data */
	payload.has_data = true;
	if(tx_ptr->var.type == HLFABRIC_TYPE_PROPOSAL)
	{
		result = hlfabricProposalPayloadDataPacked( tx_ptr, &payloadDataPacked );
		payload.data.len  = payloadDataPacked.field_len;
		payload.data.data = payloadDataPacked.field_ptr;
	}
	else if( tx_ptr->var.type == HLFABRIC_TYPE_TRANSACTION )
	{
		result = hlfabricTransactionPayloadDataPacked( tx_ptr, &payloadDataPacked );
		payload.data.len  = payloadDataPacked.field_len;
		payload.data.data = payloadDataPacked.field_ptr;
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "UNKNOWN hlfabric packed Command.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, hlfabricPayloadPacked_exception);
	}

	if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricChannelHeaderPacked[data].");
        boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, hlfabricPayloadPacked_exception);
    }

	/* pack the payload */
	packedLength = common__payload__get_packed_size( &payload );
	output_ptr->field_len = packedLength;
	output_ptr->field_ptr = BoatMalloc(packedLength);
	common__payload__pack( &payload, output_ptr->field_ptr );

	/* boat catch handle */
	boat_catch(hlfabricPayloadPacked_exception)
	{
        BoatLog( BOAT_LOG_CRITICAL, "Exception: %d", boat_exception );
        result = boat_exception;
    }
	
	/* free malloc */
	BoatFree(header.signature_header.data);
	BoatFree(header.channel_header.data);
	BoatFree(payload.data.data);
	
	return result;
}


BOAT_RESULT hlfabricProposalTransactionPacked(BoatHlfabricTx *tx_ptr)
{
	Common__Envelope envelope       = COMMON__ENVELOPE__INIT;
	BoatFieldVariable payloadPacked = {NULL, 0};
	BoatSignatureResult signatureResult;
	BUINT8   grpcHeader[5];
	BUINT8   hash[32];
	BUINT32  packedLength;
	BUINT8  *packedData = NULL;
	
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
		
	/* step-0: param in check */
	if( ( tx_ptr == NULL ) || ( tx_ptr->wallet_ptr == NULL ) || \
		( tx_ptr->wallet_ptr->http2Context_ptr == NULL ) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter should not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* step-1: generate nonce once for proposal */
	if( tx_ptr->var.type == HLFABRIC_TYPE_PROPOSAL )
	{
		tx_ptr->var.nonce.field_len = sizeof(tx_ptr->var.nonce.field);
		result = BoatRandom(tx_ptr->var.nonce.field, tx_ptr->var.nonce.field_len, NULL);
	}
	if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricGenNonce.");
		boat_throw(BOAT_ERROR_COMMON_GEN_RAND_FAIL, hlfabricProposalTransactionPacked_exception);
	}
	
	/* step-2: compute payload packed length */
	result = hlfabricPayloadPacked(tx_ptr, &payloadPacked);

	/* step-3: compute hash */
	result = BoatHash( BOAT_HASH_SHA256,payloadPacked.field_ptr, 
					   payloadPacked.field_len, hash, NULL, NULL );
	if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		boat_throw(result, hlfabricProposalTransactionPacked_exception);
	}
	
	/* step-4: signature */
	result = BoatSignature( tx_ptr->wallet_ptr->account_info.prikeyCtx,
							hash, sizeof(hash), &signatureResult, NULL );
	if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
		boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, hlfabricProposalTransactionPacked_exception);
	}
	
	if( !signatureResult.pkcs_format_used )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
		boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, hlfabricProposalTransactionPacked_exception);
	}

	/* step-5: pack the envelope */
	envelope.has_payload     = true;
	envelope.payload.len     = payloadPacked.field_len;
	envelope.payload.data    = payloadPacked.field_ptr;
	envelope.has_signature   = signatureResult.pkcs_format_used;
	envelope.signature.len   = signatureResult.pkcs_sign_length;
	envelope.signature.data  = signatureResult.pkcs_sign;
	packedLength             = common__envelope__get_packed_size(&envelope);
	
	/* step-6: packed length assignment */
	tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len = packedLength + sizeof(grpcHeader);
	if( tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len > BOAT_HLFABRIC_HTTP2_SEND_BUF_MAX_LEN )
	{
		BoatLog(BOAT_LOG_CRITICAL, "packed length out of sendbuffer size limit.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricProposalTransactionPacked_exception);
	}
	
	/* step-7: packed data assignment */
	/* ---grpcHeader compute */
	grpcHeader[0] = 0x00;//uncompressed
	for(int i = 0 ; i < 4; i++)
	{
		grpcHeader[i + 1] = (packedLength >> (32 - 8*(i+1)))&0xFF;
	}
	/* ---generate packed data */
	packedData = tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr;
	memcpy(packedData, grpcHeader, sizeof(grpcHeader));
	common__envelope__pack(&envelope, &packedData[sizeof(grpcHeader)]);
	
	/* boat catch handle */
	boat_catch(hlfabricProposalTransactionPacked_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }
	
	/* free malloc */
	BoatFree(payloadPacked.field_ptr);
	
	return result;
}

/*! @}*/

#endif /* end of PROTOCOL_USE_HLFABRIC */
