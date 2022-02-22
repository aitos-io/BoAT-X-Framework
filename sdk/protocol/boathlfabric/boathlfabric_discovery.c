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

/*!@brief Ethereum wallet API for BoAT IoT SDK

@file
api_hlfabric.c defines the Ethereum wallet API for BoAT IoT SDK.
*/
#include "boatinternal.h"
#if (DISCOVERY_PEER_QUERY == 1)
/* self-header include */

#include "boatprotocols.h"
#include "http2intf.h"
/* protos include */
#include "http2intf.h"
#include "boatplatform_internal.h"
/* protos header include */
#include "common/common.pb-c.h"
#include "peer/chaincode.pb-c.h"
#include "peer/proposal.pb-c.h"
#include "msp/identities.pb-c.h"
#include "peer/transaction.pb-c.h"
#include "peer/proposal_response.pb-c.h"
#include "discovery/protocol.pb-c.h"

BUINT32 hlfabricDiscoveryGetURL(BUINT8 *data, BUINT32 datalen, BUINT32 *outOffset)
{
	BUINT32 len = 0;
	BUINT32 offset = 0;
	if (((data[offset] & 0x07) == 0) && ((data[offset + 1] & 0x80) == 0))
	{
		offset += 2;
	}
	else
	{
		offset += 3;
	}
	if (((data[offset] & 0x07) == 2) && ((data[offset + 1] & 0x80) == 0))
	{
		offset += 2;
	}
	else
	{
		offset += 3;
	}
	if (((data[offset] & 0x07) == 2) && ((data[offset + 1] & 0x80) == 0))
	{
		offset += 2;
	}
	else
	{
		offset += 3;
	}
	if (((data[offset] & 0x07) == 2) && ((data[offset + 1] & 0x80) == 0))
	{
		len = data[offset + 1];
		offset += 2;
	}
	else
	{
		len = data[offset + 1] << 8 | data[offset + 2];
		offset += 3;
	}
	*outOffset = offset;

	return len;
}


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
__BOATSTATIC BOAT_RESULT hlfabricDiscoverySignatureHeaderPacked(const BoatHlfabricTx *tx_ptr,
															   BUINT8 *txIdBin,
															   BoatFieldVariable *output_ptr)
{
	Common__SignatureHeader signatureHeader = COMMON__SIGNATURE_HEADER__INIT;
	Msp__SerializedIdentity serializedIdentity = MSP__SERIALIZED_IDENTITY__INIT;
	BUINT8 *serializedIdentityBuffer = NULL;
	BUINT32 packedLength;
	BoatFieldVariable txIdRawMaterial = {NULL, 0};

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	/* step-0: param in check */
	if (tx_ptr->wallet_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* step-1: signatureHeader packed */
	/* -creator */
	/* --------> organization name */
	serializedIdentity.mspid = (BCHAR *)tx_ptr->var.orgName;
	/* --------> certificate */
	serializedIdentity.has_id_bytes = true;
	serializedIdentity.id_bytes.len = tx_ptr->wallet_ptr->account_info.cert.field_len;
	serializedIdentity.id_bytes.data = tx_ptr->wallet_ptr->account_info.cert.field_ptr;
	/* --------> creator packed */
	packedLength = msp__serialized_identity__get_packed_size(&serializedIdentity);
	serializedIdentityBuffer = BoatMalloc(packedLength);
	if (NULL == serializedIdentityBuffer)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate serializedIdentityBuffer.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricSignatureHeaderPacked_exception);
	}
	msp__serialized_identity__pack(&serializedIdentity, serializedIdentityBuffer);
	/* -------->creator value assignment */
	signatureHeader.has_creator = true;
	signatureHeader.creator.len = packedLength;
	signatureHeader.creator.data = serializedIdentityBuffer;
	/* -nonce */
	signatureHeader.has_nonce = true;
	signatureHeader.nonce.len = tx_ptr->var.nonce.field_len;
	signatureHeader.nonce.data = ((BoatHlfabricTx *)tx_ptr)->var.nonce.field;

	/* generate txID */
	txIdRawMaterial.field_len = signatureHeader.nonce.len + signatureHeader.creator.len;
	txIdRawMaterial.field_ptr = BoatMalloc(txIdRawMaterial.field_len);
	if (NULL == txIdRawMaterial.field_ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate txIdRawMaterial buffer.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricSignatureHeaderPacked_exception);
	}

	memcpy(txIdRawMaterial.field_ptr, signatureHeader.nonce.data, signatureHeader.nonce.len);
	memcpy(txIdRawMaterial.field_ptr + signatureHeader.nonce.len, signatureHeader.creator.data, signatureHeader.creator.len);
	result = BoatHash(BOAT_HASH_SHA256, txIdRawMaterial.field_ptr, txIdRawMaterial.field_len, txIdBin, NULL, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		boat_throw(result, hlfabricSignatureHeaderPacked_exception);
	};
	/* pack the signatureHeader */
	// if (tx_ptr->var.type == HLFABRIC_TYPE_DISCOVER)
	// {
		packedLength = common__signature_header__get_packed_size(&signatureHeader);
		output_ptr->field_ptr = BoatMalloc(packedLength + 3);
		output_ptr->field_len = packedLength + 3;
		common__signature_header__pack(&signatureHeader, output_ptr->field_ptr + 3);
		output_ptr->field_ptr[0] = 0x0A;
		output_ptr->field_ptr[1] = (packedLength & 0xFF) | 0x80;
		output_ptr->field_ptr[2] = packedLength >> 7;
	// }
	// else
	// {
	// 	packedLength = common__signature_header__get_packed_size(&signatureHeader);
	// 	output_ptr->field_ptr = BoatMalloc(packedLength);
	// 	output_ptr->field_len = packedLength;
	// 	common__signature_header__pack(&signatureHeader, output_ptr->field_ptr);
	// }

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
 * @brief peer discover payload data packed
 * 
 * @details
 *   proposal payload is consist of query content field. query content field is 
 *   made up configuration information to be queried,  e.g. 
 *   "DISCOVERY__QUERY__QUERY_CC_QUERY ","DISCOVERY__QUERY__QUERY_CONFIG_QUERY"
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param[out] output_ptr 
*   A structure pointer to store nodes info, e.g. "hostname","port","TLScert".
 *   In internal of this function, the memory of store serialize data has been alloced,
 *   caller SHOULD NOT alloc memory for this pointer again.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code. 
 ******************************************************************************/


__BOATSTATIC BOAT_RESULT hlfabricDiscoveryPayloadDataPacked(BoatHlfabricTx *tx_ptr,
														   BoatFieldVariable *output_ptr)
{

	// ProtobufCBinaryData argsTmp[BOAT_HLFABRIC_ARGS_MAX_NUM];
	BUINT8 *chaincodeInvocationSpecBuffer = NULL;
	BUINT16 packedLength;
	Protos__ChaincodeInput message = PROTOS__CHAINCODE_INPUT__INIT;
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT16 resLen = 0, len = 0;
	BUINT16 offset = 0;
	// boat_try_declare;

	len = strlen(tx_ptr->var.chaincodeId.name);
	chaincodeInvocationSpecBuffer = BoatMalloc(len + 6);
	chaincodeInvocationSpecBuffer[0] = 0x0A;
	chaincodeInvocationSpecBuffer[1] = len + 4;
	chaincodeInvocationSpecBuffer[2] = 0x0A;
	chaincodeInvocationSpecBuffer[3] = len + 2;
	chaincodeInvocationSpecBuffer[4] = 0x0A;
	chaincodeInvocationSpecBuffer[5] = len;
	memcpy(chaincodeInvocationSpecBuffer + 6, tx_ptr->var.chaincodeId.name, len);
	len += 6;

	message.n_args = 1;
	message.args = BoatMalloc((message.n_args) * sizeof(ProtobufCBinaryData));

	message.args[0].data = (BUINT8*)tx_ptr->var.channelId;
	message.args[0].len = strlen(tx_ptr->var.channelId);

	packedLength = protos__chaincode_input__get_packed_size(&message);
	BoatLog(BOAT_LOG_CRITICAL, "packedLength = %d ", packedLength);
	BUINT8 *configQueryData = NULL;
	BUINT8 *peerQueryData = NULL;
	BUINT8 *ccQueryData = NULL;
	// if (DISCOVERY_CONFIG_QUERY == 1)
	// {
	// 	configQueryData = BoatMalloc(packedLength + 4);
	// 	resLen += packedLength + 4;
	// 	configQueryData[0] = 0x12;
	// 	configQueryData[1] = packedLength + 2;
	// 	protos__chaincode_input__pack(&message, configQueryData + 2);
	// 	configQueryData[2 + packedLength] = DISCOVERY__QUERY__QUERY_CONFIG_QUERY << 3 | 2;
	// 	configQueryData[2 + 1 + packedLength] = 0;
	// }
	if (DISCOVERY_PEER_QUERY == 1)
	{

		// peerQueryData = BoatMalloc(packedLength + 4);
		// resLen += packedLength + 4;
		// peerQueryData[0] = 0x12;
		// peerQueryData[1] = packedLength + 2;
		// protos__chaincode_input__pack(&message, peerQueryData + 2);
		// peerQueryData[2 + packedLength] = DISCOVERY__QUERY__QUERY_PEER_QUERY << 3 | 2;
		// peerQueryData[2 + 1 + packedLength] = 0;

		ccQueryData = BoatMalloc(packedLength + 4 + len);
		resLen += packedLength + 4 + len;
		ccQueryData[0] = 0x12;
		ccQueryData[1] = packedLength + 2 + len;
		protos__chaincode_input__pack(&message, ccQueryData + 2);
		ccQueryData[2 + packedLength] = DISCOVERY__QUERY__QUERY_CC_QUERY << 3 | 2;

		ccQueryData[2 + 1 + packedLength] = len;
		memcpy(ccQueryData + 2 + 2 + packedLength, chaincodeInvocationSpecBuffer, len);

		configQueryData = BoatMalloc(packedLength + 4);
		resLen += packedLength + 4;
		configQueryData[0] = 0x12;
		configQueryData[1] = packedLength + 2;
		protos__chaincode_input__pack(&message, configQueryData + 2);
		configQueryData[2 + packedLength] = DISCOVERY__QUERY__QUERY_CONFIG_QUERY << 3 | 2;
		configQueryData[2 + 1 + packedLength] = 0;
	}

	output_ptr->field_ptr = BoatMalloc(resLen);
	protos__chaincode_input__pack(&message, output_ptr->field_ptr + 2);

	// if (peerQueryData != NULL)
	// {
	// 	memcpy(output_ptr->field_ptr + offset, peerQueryData, packedLength + 4);
	// 	offset += packedLength + 4;
	// }

	if (ccQueryData != NULL)
	{
		memcpy(output_ptr->field_ptr + offset, ccQueryData, packedLength + 4 + len);
		offset += packedLength + 4 + len;
	}
	if (configQueryData != NULL)
	{
		memcpy(output_ptr->field_ptr + offset, configQueryData, packedLength + 4);
		offset += packedLength + 4;
	}
	output_ptr->field_len = resLen;


	/* free malloc */
	BoatFree(chaincodeInvocationSpecBuffer);
	if (configQueryData != NULL)
	{
		BoatFree(configQueryData);
	}
	if (peerQueryData != NULL)
	{
		BoatFree(peerQueryData);
	}
	if (ccQueryData != NULL)
	{
		BoatFree(ccQueryData);
	}

	//protos__chaincode_input__free_unpacked(&message,NULL);
	if(message.args != NULL){
		BoatFree(message.args);
	}

	return result;
}

/*!****************************************************************************
 * @brief peer discover payload packed
 * 
 * @details
 *   proposal payload is consist of  chaincode id field and 
 *    query content field.
 *   \n Language type indicate chaincode coding language, e.g. the chaincode is 
 *   coding by go, or java, or other support languages. chaincode id is made up
 *   path, name, and version. query content field is made up configuration 
 *   information to be queried,  e.g. "DISCOVERY__QUERY__QUERY_CC_QUERY "
 *   ,"DISCOVERY__QUERY__QUERY_CONFIG_QUERY"
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param[out] output_ptr 
*   A structure pointer to store nodes info, e.g. "hostname","port","TLScert".
 *   In internal of this function, the memory of store serialize data has been alloced,
 *   caller SHOULD NOT alloc memory for this pointer again.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code. 
 ******************************************************************************/

__BOATSTATIC BOAT_RESULT hlfabricDiscoveryPayloadPacked(BoatHlfabricTx *tx_ptr,
													   BoatFieldVariable *output_ptr)
{
	Common__Header header = COMMON__HEADER__INIT;
	Common__Payload payload = COMMON__PAYLOAD__INIT;
	BoatFieldVariable signatureHeaderPacked = {NULL, 0};
	// BoatFieldVariable channelHeaderPacked = {NULL, 0};
	BoatFieldVariable payloadDataPacked = {NULL, 0};
	BUINT8 txIdBin[32];
	BUINT32 packedLength;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	/* payload.header */
	payload.header = &header;
	/* ------>signature header */
	header.has_signature_header = true;
	result = hlfabricDiscoverySignatureHeaderPacked(tx_ptr, txIdBin, &signatureHeaderPacked);
	header.signature_header.len = signatureHeaderPacked.field_len;
	header.signature_header.data = signatureHeaderPacked.field_ptr;
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricSignatureHeaderPacked.");
		boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, hlfabricPayloadPacked_exception);
	}

	BoatLog_hexasciidump(BOAT_LOG_NORMAL, "signatureHeaderPacked result",
						 signatureHeaderPacked.field_ptr,
						 signatureHeaderPacked.field_len);


	/* payload.data */
	payload.has_data = true;
	result = hlfabricDiscoveryPayloadDataPacked(tx_ptr, &payloadDataPacked);
	payload.data.len = payloadDataPacked.field_len;
	payload.data.data = payloadDataPacked.field_ptr;


	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec hlfabricChannelHeaderPacked[data].");
		boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, hlfabricPayloadPacked_exception);
	}

	/* pack the payload */
	packedLength = signatureHeaderPacked.field_len + payloadDataPacked.field_len;

	output_ptr->field_len = packedLength;
	output_ptr->field_ptr = BoatMalloc(packedLength);
	memcpy(output_ptr->field_ptr, signatureHeaderPacked.field_ptr, signatureHeaderPacked.field_len);
	memcpy(output_ptr->field_ptr + signatureHeaderPacked.field_len, payloadDataPacked.field_ptr, payloadDataPacked.field_len);


	/* boat catch handle */
	boat_catch(hlfabricPayloadPacked_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	/* free malloc */
	BoatFree(header.signature_header.data);
	BoatFree(header.channel_header.data);
	BoatFree(payload.data.data);

	return result;
}
/*!****************************************************************************
 * @brief  peer discover transaction payload data packed
 *
 * @details
 *   The composition of transaction is shown as follow:
   @verbatim
   
                                   /signatureHeader             
   transaction -->transactionAction 
                                   \                       /chaincodeProposalPayload
                                    \chaincodeActionPayload
  								                           \chaincodeQueryAction
 * @endverbatim
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param[out] output_ptr 
 *   A structure pointer to store nodes info, e.g. "hostname","port","TLScert".
 *   In internal of this function, the memory of store serialize data has been alloced,
 *   caller SHOULD NOT alloc memory for this pointer again.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code. 
 ******************************************************************************/
BOAT_RESULT hlfabricProposalDiscoveryTransactionPacked(BoatHlfabricTx *tx_ptr)
{
	// Common__Envelope envelope = COMMON__ENVELOPE__INIT;
	BoatFieldVariable payloadPacked = {NULL, 0};
	BoatSignatureResult signatureResult;
	Discovery__SignedRequest message = DISCOVERY__SIGNED_REQUEST__INIT;
	BUINT8 grpcHeader[5];
	BUINT8 hash[32];
	BUINT32 packedLength;
	BUINT8 *packedData = NULL;
	int i;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	/* step-0: param in check */
	if ((tx_ptr == NULL) || (tx_ptr->wallet_ptr == NULL) ||
		(tx_ptr->wallet_ptr->http2Context_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter should not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	/* step-2: compute payload packed length */
	result = hlfabricDiscoveryPayloadPacked(tx_ptr, &payloadPacked);
	/* step-3: compute hash */
	result = BoatHash(BOAT_HASH_SHA256, payloadPacked.field_ptr,
					  payloadPacked.field_len, hash, NULL, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		boat_throw(result, hlfabricProposalTransactionPacked_exception);
	}

	/* step-4: signature */
	result = BoatSignature(tx_ptr->wallet_ptr->account_info.prikeyCtx,
						   hash, sizeof(hash), &signatureResult, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
		boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, hlfabricProposalTransactionPacked_exception);
	}

	if (!signatureResult.pkcs_format_used)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
		boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, hlfabricProposalTransactionPacked_exception);
	}

	message.has_payload = true;
	message.payload.data = payloadPacked.field_ptr;
	message.payload.len = payloadPacked.field_len;
	message.has_signature = signatureResult.pkcs_format_used;
	message.signature.data = signatureResult.pkcs_sign;
	message.signature.len = signatureResult.pkcs_sign_length;
	packedLength = discovery__signed_request__get_packed_size(&message);

	/* step-6: packed length assignment */
	tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len = packedLength + sizeof(grpcHeader);
	if (tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len > BOAT_HLFABRIC_HTTP2_SEND_BUF_MAX_LEN)
	{
		BoatLog(BOAT_LOG_CRITICAL, "packed length out of sendbuffer size limit.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hlfabricProposalTransactionPacked_exception);
	}

	/* step-7: packed data assignment */
	/* ---grpcHeader compute */
	grpcHeader[0] = 0x00; //uncompressed
	for ( i = 0; i < 4; i++)
	{
		grpcHeader[i + 1] = (packedLength >> (32 - 8 * (i + 1))) & 0xFF;
	}
	/* ---generate packed data */
	packedData = tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr;
	memcpy(packedData, grpcHeader, sizeof(grpcHeader));
	discovery__signed_request__pack(&message, &packedData[sizeof(grpcHeader)]);

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

/*!****************************************************************************
 * @brief Access to the specified node 
 * 
 * @param tx_ptr 
 *   Fabric transaction structure pointer
 *
 * @param nodeInfo 
*    The first element address of nodeinfo structure.
 *  
 * @param nodeMaxNum 
 *   Support maxium node number.
 *   \n For endorser node, this param is equeal with the macro #BOAT_HLFABRIC_ENDORSER_MAX_NUM,
 *   for orderer node, this param is equeal with the macro #BOAT_HLFABRIC_ORDERER_MAX_NUM.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if set successed, otherwise return a failed code.
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT BoatHlfabricDiscoveryExec(BoatHlfabricTx *tx_ptr,
												  BoatHlfabricNodesCfg nodeCfg)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT8 i, j, k;
	boat_try_declare;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	result = hlfabricProposalDiscoveryTransactionPacked(tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "[%s]:packed failed.", tx_ptr->var.args.args[0]);
		boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, BoatHlfabricTxProposal_exception);
	}
	for (i = 0; i < nodeCfg.endorserLayoutNum; i++)
	{
		for (j = 0; j < nodeCfg.layoutCfg[i].endorserGroupNum; j++)
		{
			for (k = 0; k < nodeCfg.layoutCfg[i].groupCfg[j].endorserNumber; k++)
			{
				tx_ptr->wallet_ptr->http2Context_ptr->nodeUrl = nodeCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl;
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)

				if (tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain != NULL)
				{
					if (tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr != NULL)
					{
						BoatFree(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr);
					}
					tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = 0;
					BoatFree(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain);
				}

				tx_ptr->wallet_ptr->http2Context_ptr->hostName = nodeCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName;
				tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain = BoatMalloc(sizeof(BoatFieldVariable));
				tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length + 1;
				tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr = BoatMalloc(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
				memset(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, 0x00, tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
				memcpy(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length);
				// tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr = nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content;
				// BoatLog(BOAT_LOG_CRITICAL, "hostname : %s ", tx_ptr->wallet_ptr->http2Context_ptr->hostName);
#endif
				tx_ptr->wallet_ptr->http2Context_ptr->type = tx_ptr->var.type;
				tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &tx_ptr->evaluateRes;
				// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "http2SubmitRequest  :",
				// 					 tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr,
				// 					 tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len);
				tx_ptr->wallet_ptr->http2Context_ptr->pathTmp = "/discovery.Discovery/Discover";
				result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
				if(result == BOAT_SUCCESS)
				{
					return result;
				}

			}
		}
	}

	//! boat catch handle
	boat_catch(BoatHlfabricTxProposal_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	return result;
}

BOAT_RESULT BoatHlfabricDiscoverySubmit(BoatHlfabricTx *tx_ptr, const BoatHlfabricNodesCfg endorserInfo_ptr)
{
	// BoatHlfabricNodeInfo urlTmp[2] = {{NULL, NULL}, {NULL, NULL}};
	BOAT_RESULT result = BOAT_SUCCESS;
	Discovery__Response *discoveryResponse = NULL;
	Msp__SerializedIdentity *msp_serializedIdentity = NULL;
	// Protos__ProposalResponsePayload *ResponsePayload = NULL;
	DiscoverRes discoverResult;
	BUINT32 len = 0, offset = 0;
	BCHAR *port;
	int i,j,k,l,m;
	// boat_try_declare;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	BoatLog(BOAT_LOG_NORMAL, "Submit will execute... [%d] " ,endorserInfo_ptr.endorserLayoutNum);
	DiscoveryResInit(&discoverResult);
	/* invoke-step1: submit proposal to endorer */
	tx_ptr->var.type = HLFABRIC_TYPE_DISCOVER;
	result = BoatHlfabricDiscoveryExec(tx_ptr, endorserInfo_ptr);
	
	if(tx_ptr->evaluateRes.httpResLen == 0){
		return BOAT_ERROR;
	}
	BoatLog_hexasciidump(BOAT_LOG_NORMAL, "invoke result",
						 tx_ptr->evaluateRes.http2Res,
						 tx_ptr->evaluateRes.httpResLen);
	// http2ResData = tx_ptr->endorserResponse.response[0].payload.field_ptr;
	// http2Reslen = tx_ptr->endorserResponse.response[0].payload.field_len;

	discoveryResponse = discovery__response__unpack(NULL, tx_ptr->evaluateRes.httpResLen - 5, tx_ptr->evaluateRes.http2Res + 5);
	if (tx_ptr->evaluateRes.http2Res != NULL)
	{
		BoatFree(tx_ptr->evaluateRes.http2Res);
	}
	tx_ptr->evaluateRes.httpResLen =0;
	if (discoveryResponse == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "[http2] discovery__response__unpack failed, maybe a invalid endorser.");
		return 0;
	}
		for ( i = 0; i < discoveryResponse->n_results; i++)
	{
		if(discoveryResponse->results[i]->result_case == DISCOVERY__QUERY_RESULT__RESULT_ERROR || discoveryResponse->results[i]->result_case == DISCOVERY__QUERY_RESULT__RESULT__NOT_SET)
		{
			BoatLog(BOAT_LOG_CRITICAL, "node discover err : %s ",discoveryResponse->results[i]->error->content);
			return BOAT_ERROR;
		}
	}

	Discovery__ConfigResult *mconfig_result;

	mconfig_result = discoveryResponse->results[0]->config_result;
	BUINT8 num = mconfig_result->n_msps;
	// discoverResult->Peership.orgNum = num;
	Discovery__ChaincodeQueryResult *cc_query_res;
	cc_query_res = discoveryResponse->results[0]->cc_query_res;
	num = cc_query_res->n_content;

	
	BUINT8 n_layouts, n_quantities_by_group;
	for ( i = 0; i < num; i++)
	{

		// BoatLog(BOAT_LOG_CRITICAL, "[http2] discover endorsers_by_groups key  : %s ",cc_query_res->content[i]->endorsers_by_groups[0]->key);
		n_layouts = cc_query_res->content[i]->n_layouts;
		discoverResult.cc_res.num = n_layouts;
		discoverResult.cc_res.layouts = BoatMalloc(n_layouts * sizeof(layoutInfo));
		for ( j = 0; j < n_layouts; j++)
		{
			n_quantities_by_group = cc_query_res->content[i]->layouts[j]->n_quantities_by_group;
			discoverResult.cc_res.layouts[j].num = n_quantities_by_group;
			discoverResult.cc_res.layouts[j].groups = BoatMalloc(n_quantities_by_group * sizeof(groupInfo));
			for ( k = 0; k < n_quantities_by_group; k++)
			{
				discoverResult.cc_res.layouts[j].groups[k].numEndorsers = 0;
				BoatLog(BOAT_LOG_CRITICAL, "[http2] discover layout key  : %s ", cc_query_res->content[i]->layouts[j]->quantities_by_group[k]->key);
				BoatLog(BOAT_LOG_CRITICAL, "[http2] discover layout value: %x ", cc_query_res->content[i]->layouts[j]->quantities_by_group[k]->value);
				if (cc_query_res->content[i]->layouts[j]->quantities_by_group[k]->has_value == true)
					discoverResult.cc_res.layouts[j].groups[k].value = cc_query_res->content[i]->layouts[j]->quantities_by_group[k]->value;
				discoverResult.cc_res.layouts[j].groups[k].key = BoatMalloc(strlen(cc_query_res->content[i]->layouts[j]->quantities_by_group[k]->key)+1);
				memset(discoverResult.cc_res.layouts[j].groups[k].key,0,strlen(cc_query_res->content[i]->layouts[j]->quantities_by_group[k]->key)+1);
				memcpy(discoverResult.cc_res.layouts[j].groups[k].key, cc_query_res->content[i]->layouts[j]->quantities_by_group[k]->key, strlen(cc_query_res->content[i]->layouts[j]->quantities_by_group[k]->key));
			}
		}

		BUINT8 n_endorsers = cc_query_res->content[i]->n_endorsers_by_groups;

		for ( j = 0; j < n_endorsers; j++)
		{
			BoatLog(BOAT_LOG_CRITICAL, "endorsers_by_groups[%d] key : %s ", j, cc_query_res->content[i]->endorsers_by_groups[j]->key);
			for ( m = 0; m < discoverResult.cc_res.num; m++)
			{
				for ( k = 0; k < discoverResult.cc_res.layouts[m].num; k++)
				{

					if (strlen(cc_query_res->content[i]->endorsers_by_groups[j]->key) == strlen(discoverResult.cc_res.layouts[m].groups[k].key) &&
						memcmp(cc_query_res->content[i]->endorsers_by_groups[j]->key, discoverResult.cc_res.layouts[m].groups[k].key, strlen(discoverResult.cc_res.layouts[0].groups[k].key)) == 0)
					{
						discoverResult.cc_res.layouts[m].groups[k].numEndorsers = cc_query_res->content[i]->endorsers_by_groups[j]->value->n_peers;
						BUINT8 n_peers = discoverResult.cc_res.layouts[m].groups[k].numEndorsers;

						discoverResult.cc_res.layouts[m].groups[k].endorsers = BoatMalloc(n_peers * sizeof(Endorsers));
						for ( l = 0; l < n_peers; l++)
						{
							msp_serializedIdentity = msp__serialized_identity__unpack(NULL, cc_query_res->content[i]->endorsers_by_groups[j]->value->peers[l]->identity.len, cc_query_res->content[i]->endorsers_by_groups[j]->value->peers[l]->identity.data);
							discoverResult.cc_res.layouts[m].groups[k].endorsers[l].MSPID = BoatMalloc(strlen(msp_serializedIdentity->mspid)+1);
							memset(discoverResult.cc_res.layouts[m].groups[k].endorsers[l].MSPID,0,strlen(msp_serializedIdentity->mspid)+1);
							memcpy(discoverResult.cc_res.layouts[m].groups[k].endorsers[l].MSPID, msp_serializedIdentity->mspid, strlen(msp_serializedIdentity->mspid));
							// BoatLog(BOAT_LOG_CRITICAL, " endorsers[%d].MSPID  : %s ", l, discoverResult.cc_res.layouts[m].groups[k].endorsers[l].MSPID);
							len = hlfabricDiscoveryGetURL(cc_query_res->content[i]->endorsers_by_groups[j]->value->peers[l]->membership_info->payload.data, cc_query_res->content[i]->endorsers_by_groups[j]->value->peers[l]->membership_info->payload.len, &offset);

							discoverResult.cc_res.layouts[m].groups[k].endorsers[l].Endpoint = BoatMalloc(len+1);
							memset(discoverResult.cc_res.layouts[m].groups[k].endorsers[l].Endpoint,0,len+1);
							memcpy(discoverResult.cc_res.layouts[m].groups[k].endorsers[l].Endpoint, cc_query_res->content[i]->endorsers_by_groups[j]->value->peers[l]->membership_info->payload.data + offset, len);
							msp__serialized_identity__free_unpacked(msp_serializedIdentity,NULL);
						}
					}
				}
			}
		}
	}
	Discovery__ConfigResult *config_result = discoveryResponse->results[1]->config_result;
	discoverResult.discoverConfig.discoverMsps.num = config_result->n_msps;
	discoverResult.discoverConfig.discoverMsps.discoverMspInfo = BoatMalloc(config_result->n_msps * sizeof(mspsInfo));
	for ( i = 0; i < config_result->n_msps; i++)
	{
		discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].name = BoatMalloc(strlen(config_result->msps[i]->key)+1);
		memset(discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].name,0,strlen(config_result->msps[i]->key)+1);
		memcpy(discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].name, config_result->msps[i]->key, strlen(config_result->msps[i]->key));
		if (config_result->msps[i]->value->n_tls_root_certs > 0)
		{
			discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].tlsCertLen = config_result->msps[i]->value->tls_root_certs[0].len;
			discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].tlsCert = BoatMalloc(config_result->msps[i]->value->tls_root_certs[0].len);
			//memset(discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].tlsCert,0,config_result->msps[i]->value->tls_root_certs[0].len+1);
			memcpy(discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].tlsCert, config_result->msps[i]->value->tls_root_certs[0].data, config_result->msps[i]->value->tls_root_certs[0].len);
		}
	}
	discoverResult.discoverConfig.discoverOrders.num = 0;
	 k = 0;
	for ( i = 0; i < config_result->n_orderers; i++)
	{
		/* code */
		discoverResult.discoverConfig.discoverOrders.num += config_result->orderers[i]->value->n_endpoint;
	}
	discoverResult.discoverConfig.discoverOrders.discoverOrderinfo = BoatMalloc(discoverResult.discoverConfig.discoverOrders.num * sizeof(orderInfo));
	for ( i = 0; i < config_result->n_orderers; i++)
	{
		/* code */
		for ( j = 0; j < config_result->orderers[i]->value->n_endpoint; j++)
		{
			/* code */
			discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[k].host = BoatMalloc(strlen(config_result->orderers[i]->value->endpoint[j]->host)+1);
			memset(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[k].host,0,strlen(config_result->orderers[i]->value->endpoint[j]->host)+1);
			memcpy(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[k].host, config_result->orderers[i]->value->endpoint[j]->host, strlen(config_result->orderers[i]->value->endpoint[j]->host));
			// discoverResult->discoverConfig.discoverOrders.discoverOrderinfo[k].port = config_result->orderers[i]->value->endpoint[j]->port;
			Utility_itoa(config_result->orderers[i]->value->endpoint[j]->port, discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[k].port, 10);
			discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[k].name = BoatMalloc(strlen(config_result->orderers[i]->key)+1);
			memset(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[k].name,0,strlen(config_result->orderers[i]->key)+1);
			memcpy(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[k++].name, config_result->orderers[i]->key, strlen(config_result->orderers[i]->key));
		}
	}

	for ( i = 0; i < tx_ptr->wallet_ptr->network_info.endorserLayoutNum; i++)
	{
		for ( j = 0; j < tx_ptr->wallet_ptr->network_info.layoutCfg[i].endorserGroupNum; j++)
		{
			for ( k = 0; k < tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorserNumber; k++)
			{
				if (tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].hostName != NULL)
				{
					BoatFree(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].hostName);
				}
				if (tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl != NULL)
				{
					BoatFree(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].hostName);
				}
			}
		}
	}
	tx_ptr->wallet_ptr->network_info.endorserLayoutNum = discoverResult.cc_res.num;
	tx_ptr->wallet_ptr->network_info.layoutCfg = BoatMalloc(tx_ptr->wallet_ptr->network_info.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	for ( i = 0; i < discoverResult.cc_res.num; i++)
	{

		tx_ptr->wallet_ptr->network_info.layoutCfg[i].endorserGroupNum = discoverResult.cc_res.layouts[i].num;
		tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg = BoatMalloc(discoverResult.cc_res.layouts[i].num * sizeof(BoatHlfabricNodeGroupCfg));
		for ( j = 0; j < discoverResult.cc_res.layouts[i].num; j++)
		{
			tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorserNumber = discoverResult.cc_res.layouts[i].groups[j].numEndorsers;
			tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser = BoatMalloc(discoverResult.cc_res.layouts[i].groups[j].numEndorsers * sizeof(BoatHlfabricNodeInfoCfg));
			tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].quantities = discoverResult.cc_res.layouts[i].groups[j].value;
			for ( k = 0; k < discoverResult.cc_res.layouts[i].groups[j].numEndorsers; k++)
			{
				port = strchr(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].Endpoint, ':');
				len = strlen(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].Endpoint) - strlen(port);
				tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].hostName = BoatMalloc(len+1);
				memset(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].hostName,0,len+1);
				memcpy(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].hostName, discoverResult.cc_res.layouts[i].groups[j].endorsers[k].Endpoint, len);
				len = strlen(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].Endpoint);
				tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl = BoatMalloc(len+1);
				memset(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl,0,len+1);
				// memcpy(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl, IP, strlen(IP));
				// memcpy(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl + strlen(IP), port, strlen(port));
				memcpy(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl, discoverResult.cc_res.layouts[i].groups[j].endorsers[k].Endpoint, len);

				for ( l = 0; l < discoverResult.discoverConfig.discoverMsps.num; l++)
				{
					if (strlen(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].MSPID) == strlen(discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].name) && memcmp(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].MSPID, discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].name, strlen(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].MSPID)) == 0)
					{
						tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length = discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].tlsCertLen;
						memcpy(tx_ptr->wallet_ptr->network_info.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].tlsCert, discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].tlsCertLen);
					}
				}
			}
		}
	}
	tx_ptr->wallet_ptr->network_info.orderCfg.endorserNumber = discoverResult.discoverConfig.discoverOrders.num;
	tx_ptr->wallet_ptr->network_info.orderCfg.endorser = BoatMalloc(discoverResult.discoverConfig.discoverOrders.num * sizeof(BoatHlfabricNodeInfoCfg));
	for ( i = 0; i < discoverResult.discoverConfig.discoverOrders.num; i++)
	{
		len = sizeof(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].port) + strlen(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].host) + 1;
		tx_ptr->wallet_ptr->network_info.orderCfg.endorser[i].nodeUrl = BoatMalloc(len+1);
		memset(tx_ptr->wallet_ptr->network_info.orderCfg.endorser[i].nodeUrl,0,len+1);
		offset = 0;
		memcpy(tx_ptr->wallet_ptr->network_info.orderCfg.endorser[i].nodeUrl + offset, discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].host, strlen(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].host));
		offset += strlen(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].host);
		// memcpy(tx_ptr->wallet_ptr->network_info.orderCfg.endorser[i].nodeUrl + offset, IP, strlen(IP));
		// offset += strlen(IP);
		tx_ptr->wallet_ptr->network_info.orderCfg.endorser[i].nodeUrl[offset++] = ':';
		memcpy(tx_ptr->wallet_ptr->network_info.orderCfg.endorser[i].nodeUrl + offset, discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].port, sizeof(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].port));

		for ( l = 0; l < discoverResult.discoverConfig.discoverMsps.num; l++)
		{
			if (strlen(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].name) == strlen(discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].name) && memcmp(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].name, discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].name, strlen(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].name)) == 0)
			{
				tx_ptr->wallet_ptr->network_info.orderCfg.tlsOrgCertContent.length = discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].tlsCertLen;
				memcpy(tx_ptr->wallet_ptr->network_info.orderCfg.tlsOrgCertContent.content, discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].tlsCert, discoverResult.discoverConfig.discoverMsps.discoverMspInfo[l].tlsCertLen);
			}
		}
	}
	tx_ptr->endorserResponse.responseCount = 0;
	DiscoveryResFree(discoverResult);
	discovery__response__free_unpacked(discoveryResponse,NULL);
	return result;
}

void DiscoveryResInit(DiscoverRes *discoverResult)
{
	discoverResult->cc_res.num = 0;
	discoverResult->cc_res.layouts = NULL;
	discoverResult->discoverConfig.discoverMsps.num = 0;
	discoverResult->discoverConfig.discoverMsps.discoverMspInfo = NULL;
	discoverResult->discoverConfig.discoverOrders.num = 0;
	discoverResult->discoverConfig.discoverOrders.discoverOrderinfo = NULL;
}

void DiscoveryResFree(DiscoverRes discoverResult)
{
	int i,j;
	for ( i = 0; i < discoverResult.cc_res.num; i++)
	{
		for ( j = 0; j < discoverResult.cc_res.layouts[i].num; j++)
		{
			if (discoverResult.cc_res.layouts[i].groups[j].key != NULL)
			{
				BoatFree(discoverResult.cc_res.layouts[i].groups[j].key);
			}
			for (size_t k = 0; k < discoverResult.cc_res.layouts[i].groups[j].numEndorsers; k++)
			{
				/* code */
				BoatFree(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].Endpoint);
				if(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].MSPID != NULL){
					BoatFree(discoverResult.cc_res.layouts[i].groups[j].endorsers[k].MSPID);
					discoverResult.cc_res.layouts[i].groups[j].endorsers[k].MSPID = NULL;
				}
			}
			
			if(discoverResult.cc_res.layouts[i].groups[j].endorsers != NULL){
				BoatFree(discoverResult.cc_res.layouts[i].groups[j].endorsers);
			}

			
			
		}
		if(discoverResult.cc_res.layouts[i].groups != NULL){
			BoatFree(discoverResult.cc_res.layouts[i].groups);
			discoverResult.cc_res.layouts[i].groups = NULL;
		}
	}
	if(discoverResult.cc_res.layouts != NULL){
			BoatFree(discoverResult.cc_res.layouts);
			discoverResult.cc_res.layouts = NULL;
	}
	for ( i = 0; i < discoverResult.discoverConfig.discoverMsps.num; i++)
	{
		if (discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].name != NULL)
		{
			BoatFree(discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].name);
		}
		if (discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].tlsCert != NULL)
		{
			BoatFree(discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].tlsCert);
		}

	}
	if(discoverResult.discoverConfig.discoverMsps.discoverMspInfo != NULL){
			BoatFree(discoverResult.discoverConfig.discoverMsps.discoverMspInfo);
			discoverResult.discoverConfig.discoverMsps.discoverMspInfo = NULL;
		}

	for ( i = 0; i < discoverResult.discoverConfig.discoverOrders.num; i++)
	{
		if (discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].host != NULL)
		{
			BoatFree(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].host);
		}
		if (discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].name != NULL)
		{
			BoatFree(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].name);
		}
	}
	if(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo != NULL){
		BoatFree(discoverResult.discoverConfig.discoverOrders.discoverOrderinfo);
		discoverResult.discoverConfig.discoverOrders.discoverOrderinfo = NULL;
	}
}

#endif
