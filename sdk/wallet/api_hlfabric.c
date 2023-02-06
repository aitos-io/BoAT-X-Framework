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

/*!@brief fabric wallet API for BoAT IoT SDK

@file
api_hlfabric.c defines the fabric wallet API for BoAT IoT SDK.
*/

/* self-header include */
#include "boatinternal.h"
#if PROTOCOL_USE_HLFABRIC == 1
#include "boatprotocols.h"
#include "http2intf.h"
/* protos include */
#include "peer/proposal_response.pb-c.h"
#include "orderer/cluster.pb-c.h"
#include "peer/proposal.pb-c.h"

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
__BOATSTATIC BOAT_RESULT BoatHlfabricTxExec(BoatHlfabricTx *tx_ptr,
											BoatHlfabricNodesCfg nodeCfg, BoatHlfabricFunType funType)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHlfabricEndorserResponse *parsePtr = NULL;
	Http2Response fabricHttp2res = {0, NULL};
	Protos__ProposalResponse *proposalResponse = NULL;
	Orderer__SubmitResponse *submitResponse = NULL;
	Protos__ProposalResponsePayload *proposalResPayload = NULL;
	Protos__ChaincodeAction *chaincodeEvent = NULL;
	BUINT8 valid_node_quantities;
	boat_try_declare;
	// DiscoverRes discooverRes = tx_ptr->discooverRes;
	int i, j, k;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	if (tx_ptr->evaluateRes.httpResLen != 0)
	{
		if (tx_ptr->evaluateRes.http2Res != NULL)
		{
			BoatFree(tx_ptr->evaluateRes.http2Res);
		}
		tx_ptr->evaluateRes.httpResLen = 0;
	}

	result = hlfabricProposalTransactionPacked(tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "[%s]:packed failed.", tx_ptr->var.args.args[0]);
		boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, BoatHlfabricTxProposal_exception);
	}
#if (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	// if (((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr != NULL)
	// {
	// 	BoatFree(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr);
	// }
	((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_len = tx_ptr->wallet_ptr->network_info.accountClientTlsPrikey.value_len + 1;
	// ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr = BoatMalloc(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_len);
	// strcpy((BCHAR *)((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr, (BCHAR *)tx_ptr->wallet_ptr->network_info.accountClientTlsPrikey.value);
	((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr = tx_ptr->wallet_ptr->network_info.accountClientTlsPrikey.value;
	// if (((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr != NULL)
	// {
	// 	BoatFree(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr);
	// }
	((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_len = tx_ptr->wallet_ptr->network_info.accountClientTlsCert.length + 1;
	// ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr = BoatMalloc(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_len);
	// strcpy((BCHAR *)((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr, tx_ptr->wallet_ptr->network_info.accountClientTlsCert.content);
	((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr = (BUINT8 *)tx_ptr->wallet_ptr->network_info.accountClientTlsCert.content;
#endif

	if (tx_ptr->var.type == HLFABRIC_TYPE_PROPOSAL)
	{
		for (i = 0; i < nodeCfg.endorserLayoutNum; i++)
		{
			for (j = 0; j < nodeCfg.layoutCfg[i].endorserGroupNum; j++)
			{
				valid_node_quantities = 0;
				for (k = 0; k < nodeCfg.layoutCfg[i].groupCfg[j].endorserNumber; k++)
				{
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->nodeUrl = nodeCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl;
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)

					// // if (((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain != NULL)
					// {
					// 	if (((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr != NULL)
					// 	{
					// 		BoatFree(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr);
					// 	}
					// 	((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len = 0;
					// 	// BoatFree(((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain);
					// }

					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->hostName = nodeCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName;
					// ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain = BoatMalloc(sizeof(BoatFieldVariable));
					// if(((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain == NULL){
					// 	BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate tlsCAchain buffer.");
					// 	boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlfabricTxProposal_exception);
					// }
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len = nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length + 1;
					// ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr = BoatMalloc(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len);
					// if (NULL == ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr)
					// {
					// 	BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate field_ptr buffer.");
					// 	boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlfabricTxProposal_exception);
					// }
					// memset(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr, 0x00, ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len);
					// memcpy(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr, nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length);
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr = (BUINT8 *)nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content;

#endif

					// if (((nodeInfo + i)->hostName != NULL) && (strlen((nodeInfo + i)->hostName) > 0))

					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))
						->type = tx_ptr->var.type;

					// tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &tx_ptr->endorserResponse;
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->parseDataPtr = &(fabricHttp2res);

					// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "http2SubmitRequest  send :",
					// 			 ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->sendBuf.field_ptr,
					// 			 ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->sendBuf.field_len);
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->pathTmp = "/protos.Endorser/ProcessProposal";
					result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
					if (result != BOAT_SUCCESS)
					{
						continue;
					}
					BoatLog(BOAT_LOG_CRITICAL, "http2SubmitRequest ok \n ");
					// BoatLog(BOAT_LOG_CRITICAL, "http2SubmitRequest fabricHttp2res.httpResLen = %d \n ",fabricHttp2res.httpResLen);
					if (fabricHttp2res.httpResLen < 5)
					{
						result = BOAT_ERROR;
						continue;
					}
					proposalResponse = protos__proposal_response__unpack(NULL, fabricHttp2res.httpResLen - 5, fabricHttp2res.http2Res + 5);
					if (fabricHttp2res.httpResLen != 0)
					{
						BoatFree(fabricHttp2res.http2Res);
						fabricHttp2res.httpResLen = 0;
					}
					parsePtr = &(tx_ptr->endorserResponse);
					if ((proposalResponse != NULL) && (proposalResponse->endorsement != NULL))
					{
						BoatLog(BOAT_LOG_NORMAL, "[http2]endorser respond received.");
						parsePtr->response[parsePtr->responseCount].contentPtr = proposalResponse;
						parsePtr->response[parsePtr->responseCount].responseType = HLFABRIC_TYPE_PROPOSAL;
						parsePtr->response[parsePtr->responseCount].payload.field_ptr = proposalResponse->payload.data;
						parsePtr->response[parsePtr->responseCount].payload.field_len = proposalResponse->payload.len;
						parsePtr->response[parsePtr->responseCount].endorser.field_ptr = proposalResponse->endorsement->endorser.data;
						parsePtr->response[parsePtr->responseCount].endorser.field_len = proposalResponse->endorsement->endorser.len;
						parsePtr->response[parsePtr->responseCount].signature.field_ptr = proposalResponse->endorsement->signature.data;
						parsePtr->response[parsePtr->responseCount].signature.field_len = proposalResponse->endorsement->signature.len;
						parsePtr->responseCount++;
					}
					else
					{
						continue;
					}
					if (funType == HLFABRIC_FUN_EVALUATE)
					{
						if (proposalResponse->payload.len != 0)
						{
							proposalResPayload = protos__proposal_response_payload__unpack(NULL, proposalResponse->payload.len, proposalResponse->payload.data);
							if (proposalResPayload == NULL || proposalResPayload->has_extension == BOAT_FALSE)
							{
								BoatLog(BOAT_LOG_CRITICAL, "proposalResPayload failed.");
								continue;
							}
							chaincodeEvent = protos__chaincode_action__unpack(NULL, proposalResPayload->extension.len, proposalResPayload->extension.data);
							protos__proposal_response_payload__free_unpacked(proposalResPayload, NULL);
							if (chaincodeEvent == NULL || chaincodeEvent->response->status != COMMON__STATUS__SUCCESS)
							{
								BoatLog(BOAT_LOG_CRITICAL, "protos__chaincode_action__unpack failed.");
								continue;
							}
							tx_ptr->evaluateRes.httpResLen = chaincodeEvent->response->payload.len;
							tx_ptr->evaluateRes.http2Res = BoatMalloc(tx_ptr->evaluateRes.httpResLen);
							if (NULL == tx_ptr->evaluateRes.http2Res)
							{
								BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate http2Res buffer.");
								boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlfabricTxProposal_exception);
							}
							memcpy(tx_ptr->evaluateRes.http2Res, chaincodeEvent->response->payload.data, chaincodeEvent->response->payload.len);
							protos__chaincode_action__free_unpacked(chaincodeEvent, NULL);
						}
						return result;
					}
					valid_node_quantities++;
					if (valid_node_quantities >= nodeCfg.layoutCfg[i].groupCfg[j].quantities)
					{
						if (nodeCfg.layoutCfg[i].endorserGroupNum == (j + 1))
						{
							return BOAT_SUCCESS;
						}
						break;
					}
				}
				if (valid_node_quantities < nodeCfg.layoutCfg[i].groupCfg[j].quantities)
				{
					BoatLog(BOAT_LOG_CRITICAL, "http2SubmitRequest failed ");
					boat_throw(BOAT_ERROR, BoatHlfabricTxProposal_exception);
				}
			}
		}
	}
	else
	{
		for (i = 0; i < nodeCfg.orderCfg.endorserNumber; i++)
		{
			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->nodeUrl = nodeCfg.orderCfg.endorser[i].nodeUrl;

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)

			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->hostName = nodeCfg.orderCfg.endorser[i].hostName;
			// ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain = nodeCfg.orderCfg.tlsOrgCertContent.content;
			// if (((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain != NULL)
			// {
			// 	if (((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr != NULL)
			// 	{
			// 		BoatFree(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr);
			// 	}
			// 	((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len = 0;
			// 	// BoatFree(((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain);
			// }
			// ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain = BoatMalloc(sizeof(BoatFieldVariable));
			// if(NULL == ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain){
			// 	BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate tlsCAchain buffer.");
			// 	boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlfabricTxProposal_exception);
			// }
			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len = nodeCfg.orderCfg.tlsOrgCertContent.length + 1;
			// ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr = BoatMalloc(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len);
			// if (NULL == ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr)
			// {
			// 	BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate field_ptr buffer.");
			// 	boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlfabricTxProposal_exception);
			// }
			// memset(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr, 0x00, ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len);
			// memcpy(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr, nodeCfg.orderCfg.tlsOrgCertContent.content, nodeCfg.orderCfg.tlsOrgCertContent.length);
			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr = (BUINT8 *)nodeCfg.orderCfg.tlsOrgCertContent.content;
#endif

			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))
				->type = tx_ptr->var.type;

			// ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->parseDataPtr = &tx_ptr->endorserResponse;

			// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "http2SubmitRequest 111 send :",
			// 					 ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->sendBuf.field_ptr,
			// 					 ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->sendBuf.field_len);

			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->parseDataPtr = &(fabricHttp2res);

			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->pathTmp = "/orderer.AtomicBroadcast/Broadcast";
			result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "[http2]http2SubmitRequest failed.");
				continue;
			}
			if (fabricHttp2res.httpResLen < 5)
			{
				BoatLog(BOAT_LOG_CRITICAL, "[http2]http2SubmitRequest failed.");
				continue;
			}
			parsePtr = &(tx_ptr->endorserResponse);
			submitResponse = orderer__submit_response__unpack(NULL, fabricHttp2res.httpResLen - 5, fabricHttp2res.http2Res + 5);
			BoatFree(fabricHttp2res.http2Res);
			if (submitResponse != NULL && submitResponse->status == COMMON__STATUS__SUCCESS)
			{
				BoatLog(BOAT_LOG_NORMAL, "[http2]orderer respond received.%d", submitResponse->status);
				parsePtr->response[parsePtr->responseCount].contentPtr = submitResponse;
				parsePtr->response[parsePtr->responseCount].responseType = HLFABRIC_TYPE_TRANSACTION;
				parsePtr->responseCount++;
				result = BOAT_SUCCESS;
				break;
			}
			else
			{
				BoatLog(BOAT_LOG_CRITICAL, "[http2]orderer respond unpacked failed.");
				continue;
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

/**
 * @description:
 * 	This function init fabric wallet by keypair index and network index;
 * @param {BUINT8} keypairIndex
 * @param {BUINT8} networkIndex
 * @return {*}
 *  This function returns fabricwallet if successfully executed.
 *  Otherwise it returns NULL.
 * @author: aitos
 */
BoatHlfabricWallet *BoatHlfabricWalletInit(BUINT8 keypairIndex, BUINT8 networkIndex)
{
	BoatHlfabricWallet *wallet_ptr = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;
	// BUINT8 i = 0;

	/* allocate wallet memory */
	wallet_ptr = BoatMalloc(sizeof(BoatHlfabricWallet));
	if (wallet_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to malloc wallet memory.");
		return NULL;
	}
	result = BoATKeypair_GetKeypairByIndex(&(wallet_ptr->account_info.prikeyCtx), keypairIndex);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "get keypair by index fail");
		return NULL;
	}
	result = BoATHlfabric_GetNetworkByIndex(&(wallet_ptr->network_info), networkIndex);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "get network by index fail");
		return NULL;
	}
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
	wallet_ptr->tlsCAchain.ca.field_len = 0;
	wallet_ptr->tlsCAchain.ca.field_ptr = NULL;
#endif

	wallet_ptr->http2Context_ptr = NULL;
	/* network_info assignment */
	// result += BoatHlfabricWalletSetNetworkInfo(wallet_ptr, config_ptr->nodesCfg);
	/* http2Context_ptr assignment */
	wallet_ptr->http2Context_ptr = http2Init();

	if (NULL == wallet_ptr->http2Context_ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to init http2Context_ptr");
		return NULL;
	}

	return wallet_ptr;
}

void BoatHlfabricWalletDeInit(BoatHlfabricWallet *wallet_ptr)
{
	// BUINT16 i, j, k;

	if (NULL == wallet_ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr needn't DeInit: wallet_ptr is NULL.");
		return;
	}

	/* account_info DeInit */
	if (wallet_ptr->account_info.prikeyCtx.keypair_name != NULL)
	{
		BoatFree(wallet_ptr->account_info.prikeyCtx.keypair_name);
		wallet_ptr->account_info.prikeyCtx.keypair_name = NULL;
	}

	/* tlsClinet_info DeInit */
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
	// for c99, free(NULL) will return directly, so here
	// use BOAT_HLFABRIC_ROOTCA_MAX_NUM as cyclic maximum is acceptable.
	if (wallet_ptr->tlsCAchain.ca.field_ptr != NULL)
	{
		BoatFree(wallet_ptr->tlsCAchain.ca.field_ptr);
	}
	wallet_ptr->tlsCAchain.ca.field_len = 0;
#endif /* #if (BOAT_HLFABRIC_TLS_SUPPORT == 1) */

	/* network_info DeInit */

	BoATHlfabric_FreeNetworkData(wallet_ptr->network_info);

	/* http2Context DeInit */
	http2DeInit(wallet_ptr->http2Context_ptr);

	/* wallet_ptr DeInit */
	BoatFree(wallet_ptr);

	/* set NULL after free completed */
	wallet_ptr = NULL;
}

BOAT_RESULT BoatHlfabricTxInit(BoatHlfabricTx *tx_ptr,
							   const BoatHlfabricWallet *wallet_ptr,
							   const BCHAR *chaincodeId_path_str,
							   const BCHAR *chaincodeId_name_str,
							   const BCHAR *chaincodeId_version_str,
							   const BCHAR *channelId_str,
							   const BCHAR *orgName_str)
{
	BUINT32 stringLen;
	BCHAR *paramSrcList[5];
	BCHAR **paramDstList[5];
	BUINT16 i = 0;
	BOAT_RESULT result = BOAT_SUCCESS;

	boat_try_declare;

	if ((tx_ptr == NULL) || (wallet_ptr == NULL) || (chaincodeId_name_str == NULL) || (channelId_str == NULL) || (orgName_str == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments 'tx_ptr' or 'wallet_ptr' cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* tx_ptr instance reset */
	/* ----->tx_ptr->wallet_ptr reset */
	tx_ptr->wallet_ptr = NULL;
	/* ----->tx_ptr->var reset */
	tx_ptr->var.type = HLFABRIC_TYPE_IDLE;
	tx_ptr->var.timestamp.sec = 0;
	tx_ptr->var.timestamp.nanos = 0;
	tx_ptr->var.nonce.field_len = 0;
	for (i = 0; i < 24; i++)
	{
		tx_ptr->var.nonce.field[i] = 0;
	}
	tx_ptr->var.chaincodeId.name = NULL;
	tx_ptr->var.chaincodeId.path = NULL;
	tx_ptr->var.chaincodeId.version = NULL;
	tx_ptr->var.args.nArgs = 0;
	tx_ptr->var.contract_name = NULL;
	tx_ptr->var.creator_id = NULL;
	for (i = 0; i < BOAT_HLFABRIC_ARGS_MAX_NUM; i++)
	{
		tx_ptr->var.args.args[i] = NULL;
	}
	tx_ptr->var.channelId = NULL;
	tx_ptr->var.orgName = NULL;
	/* ----->tx_ptr->endorserResponse reset */
	tx_ptr->endorserResponse.responseCount = 0;
	tx_ptr->evaluateRes.httpResLen = 0;
	for (i = 0; i < BOAT_HLFABRIC_ENDORSER_MAX_NUM; i++)
	{
		tx_ptr->endorserResponse.response[i].contentPtr = NULL;
		tx_ptr->endorserResponse.response[i].responseType = HLFABRIC_TYPE_IDLE;
		tx_ptr->endorserResponse.response[i].payload.field_len = 0;
		tx_ptr->endorserResponse.response[i].payload.field_ptr = NULL;
		tx_ptr->endorserResponse.response[i].endorser.field_len = 0;
		tx_ptr->endorserResponse.response[i].endorser.field_ptr = NULL;
		tx_ptr->endorserResponse.response[i].signature.field_len = 0;
		tx_ptr->endorserResponse.response[i].signature.field_ptr = NULL;
	}

	/* wallet assignment */
	tx_ptr->wallet_ptr = (BoatHlfabricWallet *)wallet_ptr;

	/* paramList assignment */
	paramSrcList[0] = (BCHAR *)chaincodeId_path_str;
	paramSrcList[1] = (BCHAR *)chaincodeId_name_str;
	paramSrcList[2] = (BCHAR *)chaincodeId_version_str;
	paramSrcList[3] = (BCHAR *)channelId_str;
	paramSrcList[4] = (BCHAR *)orgName_str;
	paramDstList[0] = &tx_ptr->var.chaincodeId.path;
	paramDstList[1] = &tx_ptr->var.chaincodeId.name;
	paramDstList[2] = &tx_ptr->var.chaincodeId.version;
	paramDstList[3] = &tx_ptr->var.channelId;
	paramDstList[4] = &tx_ptr->var.orgName;

	for (i = 0; i < sizeof(paramSrcList) / sizeof(paramSrcList[0]); i++)
	{
		if ((paramSrcList[i] != NULL) && ((stringLen = strlen(paramSrcList[i])) > 0))
		{
			*paramDstList[i] = BoatMalloc(stringLen + 1);
			if (*paramDstList[i] == NULL)
			{
				BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlfabricTxInit_exception);
			}
			memcpy(*paramDstList[i], paramSrcList[i], stringLen + 1);
		}
	}

	/* boat catch handle */
	boat_catch(BoatHlfabricTxInit_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		/* free malloc param Deinit */
		for (i = 0; i < sizeof(paramDstList) / sizeof(paramDstList[0]); i++)
		{
			BoatFree(paramDstList[i]);
		}
	}

	return result;
}

void BoatHlfabricTxDeInit(BoatHlfabricTx *tx_ptr)
{
	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Tx argument cannot be NULL.");
		return;
	}

	/* wallet_ptr DeInit */
	// DO NOTHING:wallet deinit will be implemented in BoatIotSdkDeInit(...)

	/* var DeInit */
	/* -----var.type */
	// DO NOTHING
	/* -----var.timestamp */
	// DO NOTHING
	/* -----var.nonce */
	// DO NOTHING
	/* -----var.chaincodeId */
	if (tx_ptr->var.chaincodeId.path != NULL)
	{
		BoatFree(tx_ptr->var.chaincodeId.path);
		tx_ptr->var.chaincodeId.path = NULL;
	}
	if (tx_ptr->var.chaincodeId.name != NULL)
	{
		BoatFree(tx_ptr->var.chaincodeId.name);
		tx_ptr->var.chaincodeId.name = NULL;
	}
	if (tx_ptr->var.chaincodeId.version != NULL)
	{
		BoatFree(tx_ptr->var.chaincodeId.version);
		tx_ptr->var.chaincodeId.version = NULL;
	}

	/* -----var.args */
	tx_ptr->var.args.nArgs = 0;
	for (int i = 0; i < BOAT_HLFABRIC_ARGS_MAX_NUM; i++)
	{
		tx_ptr->var.args.args[i] = NULL;
	}
	/* -----var.channelId */
	if (tx_ptr->var.channelId != NULL)
	{
		BoatFree(tx_ptr->var.channelId);
		tx_ptr->var.channelId = NULL;
	}
	if (tx_ptr->var.contract_name != NULL)
	{
		BoatFree(tx_ptr->var.contract_name);
		tx_ptr->var.contract_name = NULL;
	}
	if (tx_ptr->var.creator_id != NULL)
	{
		BoatFree(tx_ptr->var.creator_id);
		tx_ptr->var.creator_id = NULL;
	}
	if (tx_ptr->var.orgName != NULL)
	{
		BoatFree(tx_ptr->var.orgName);
		tx_ptr->var.orgName = NULL;
	}
	if (tx_ptr->evaluateRes.httpResLen != 0)
	{
		BoatFree(tx_ptr->evaluateRes.http2Res);
	}
	if (tx_ptr->wallet_ptr != NULL)
	{
		BoatHlfabricWalletDeInit(tx_ptr->wallet_ptr);
	}

	/* endorserResponse DeInit */
	// DO NOTHING:THIS FIELD HAD FREE AFTER EVERY TRANSCATION COMPLETED
}

BOAT_RESULT BoatHlfabricTxSetTimestamp(BoatHlfabricTx *tx_ptr,
									   const BUINT64 sec,
									   const BUINT64 nanos)
{
	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	tx_ptr->var.timestamp.sec = sec;
	tx_ptr->var.timestamp.nanos = nanos;

	return BOAT_SUCCESS;
}

BOAT_RESULT BoatHlfabricTxSetArgs(BoatHlfabricTx *tx_ptr,
								  const BCHAR *arg1,
								  ...)
{
	va_list ap;
	BCHAR *args = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	tx_ptr->var.args.nArgs = 0;

	va_start(ap, arg1);

	tx_ptr->var.args.args[tx_ptr->var.args.nArgs] = (BCHAR *)arg1;
	tx_ptr->var.args.nArgs++;

	while (NULL != (args = va_arg(ap, BCHAR *)))
	{
		tx_ptr->var.args.args[tx_ptr->var.args.nArgs] = (BCHAR *)args;
		tx_ptr->var.args.nArgs++;
		if (tx_ptr->var.args.nArgs >= BOAT_HLFABRIC_ARGS_MAX_NUM)
		{
			result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
			break;
		}
	}
	va_end(ap);

	return result;
}

BOAT_RESULT BoatHlfabricTxEvaluate(BoatHlfabricTx *tx_ptr)
{
	// BoatHlfabricNodeInfo urlTmp[2]    = {{NULL,NULL}, {NULL,NULL}};
	BOAT_RESULT result = BOAT_SUCCESS;
	// boat_try_declare;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	if (tx_ptr->var.args.args[0] == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments args[0] cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	BoatLog(BOAT_LOG_NORMAL, "Evaluate will execute...");

	/* submit query */
	tx_ptr->var.type = HLFABRIC_TYPE_PROPOSAL;
	// urlTmp[0] = tx_ptr->wallet_ptr->network_info.endorser[0];
	result = BoatHlfabricTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info.nodesCfg, HLFABRIC_FUN_EVALUATE);
	// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "query result",
	// 					 tx_ptr->endorserResponse.response[0].payload.field_ptr,
	// 					 tx_ptr->endorserResponse.response[0].payload.field_len);

	/* free the unpacked response data */
	for (int i = 0; i < tx_ptr->endorserResponse.responseCount; i++)
	{
		if (tx_ptr->endorserResponse.response[i].responseType == HLFABRIC_TYPE_PROPOSAL)
		{
			protos__proposal_response__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
	}
	tx_ptr->endorserResponse.responseCount = 0;

	// /* boat catch handle */
	// boat_catch(BoatHlfabricTxEvaluate_exception)
	// {
	// 	BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	//  	result = boat_exception;
	// }

	return result;
}

BOAT_RESULT BoatHlfabricTxSubmit(BoatHlfabricTx *tx_ptr)
{
	// BoatHlfabricNodeInfo urlTmp[2]    = {{NULL,NULL}, {NULL,NULL}};
	BOAT_RESULT result = BOAT_SUCCESS;
	// boat_try_declare;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	if (tx_ptr->var.args.args[0] == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments args[0] cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	BoatLog(BOAT_LOG_NORMAL, "Submit will execute...");

	/* invoke-step1: submit proposal to endorer */
	tx_ptr->var.type = HLFABRIC_TYPE_PROPOSAL;
	result = BoatHlfabricTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info.nodesCfg, HLFABRIC_FUN_SUBMIT);
	if (result != BOAT_SUCCESS)
	{
		return result;
	}

	/* invoke-step2: submit transaction to orderer */
	tx_ptr->var.type = HLFABRIC_TYPE_TRANSACTION;
	result = BoatHlfabricTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info.nodesCfg, HLFABRIC_FUN_SUBMIT);
	if (result != BOAT_SUCCESS)
	{
		return result;
	}
	/* free the unpacked response data */
	for (int i = 0; i < tx_ptr->endorserResponse.responseCount; i++)
	{
		if (tx_ptr->endorserResponse.response[i].responseType == HLFABRIC_TYPE_PROPOSAL)
		{
			protos__proposal_response__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
		else
		{
			orderer__submit_response__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
	}
	tx_ptr->endorserResponse.responseCount = 0;

	/* boat catch handle */
	// boat_catch(BoatHlfabricTxSubmit_exception)
	// {
	// 	BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	//  	result = boat_exception;
	// }

	return result;
}

void fabricWalletConfigFree(BoatHlfabricNetworkConfig network_config)
{
	for (size_t i = 0; i < network_config.nodesCfg.endorserLayoutNum; i++)
	{
		if (network_config.nodesCfg.layoutCfg != NULL)
		{
			for (size_t j = 0; j < network_config.nodesCfg.layoutCfg[i].endorserGroupNum; j++)
			{
				if (network_config.nodesCfg.layoutCfg[i].groupCfg != NULL)
				{
					for (size_t k = 0; k < network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber; k++)
					{
						if (network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser != NULL)
						{
							if (network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName != NULL)
							{
								BoatFree(network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName);
								network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName = NULL;
							}
							if (network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl != NULL)
							{
								BoatFree(network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl);
								network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl = NULL;
							}
						}
					}
					if (network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser != NULL)
					{
						BoatFree(network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser);
						network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorser = NULL;
					}
					network_config.nodesCfg.layoutCfg[i].groupCfg[j].quantities = 0;
					network_config.nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber = 0;
				}
			}
			if (network_config.nodesCfg.layoutCfg[i].groupCfg != NULL)
			{
				BoatFree(network_config.nodesCfg.layoutCfg[i].groupCfg);
			}
			network_config.nodesCfg.layoutCfg[i].endorserGroupNum = 0;
		}
	}
	if (network_config.nodesCfg.layoutCfg != NULL)
	{
		BoatFree(network_config.nodesCfg.layoutCfg);
		network_config.nodesCfg.layoutCfg = NULL;
	}
	for (size_t i = 0; i < network_config.nodesCfg.orderCfg.endorserNumber; i++)
	{
		if (network_config.nodesCfg.orderCfg.endorser != NULL)
		{
			if (network_config.nodesCfg.orderCfg.endorser[i].hostName != NULL)
			{
				BoatFree(network_config.nodesCfg.orderCfg.endorser[i].hostName);
				network_config.nodesCfg.orderCfg.endorser[i].hostName = NULL;
			}
			if (network_config.nodesCfg.orderCfg.endorser[i].nodeUrl != NULL)
			{
				BoatFree(network_config.nodesCfg.orderCfg.endorser[i].nodeUrl);
				network_config.nodesCfg.orderCfg.endorser[i].nodeUrl = NULL;
			}
		}
	}
	network_config.nodesCfg.orderCfg.endorserNumber = 0;
	if (network_config.nodesCfg.orderCfg.endorser != NULL)
	{
		BoatFree(network_config.nodesCfg.orderCfg.endorser);
		network_config.nodesCfg.orderCfg.endorser = NULL;
	}
}
#endif /* end of PROTOCOL_USE_HLFABRIC */
