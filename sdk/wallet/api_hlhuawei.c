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
api_hlhuawei.c defines the Ethereum wallet API for BoAT IoT SDK.
*/

/* self-header include */
#include "boatinternal.h"
#if PROTOCOL_USE_HLHUAWEI == 1
#include "boatprotocols.h"
#include "http2intf.h"
/* protos include */
#include "peer/proposal_response.pb-c.h"
#include "orderer/cluster.pb-c.h"
#include "common/transaction.pb-c-huawei.h"

/*!****************************************************************************
 * @brief Access to the specified node 
 * 
 * @param tx_ptr 
 *   huawei chain transaction structure pointer
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
__BOATSTATIC BOAT_RESULT BoatHlhuaweiTxExec(BoatHlfabricTx *tx_ptr,
											BoatHlfabricNodesCfg nodeCfg, BoatHlfabricFunType funType)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHlfabricEndorserResponse *parsePtr = NULL;
	Common__Transaction *proposalResponse = NULL;
	Common__Response *resData = NULL;
	Common__TxPayload *resPayload = NULL;
	Common__CommonTxData *commondTxData = NULL;

	// Orderer__SubmitResponse *submitResponse = NULL;
	BUINT8 valid_node_quantities;
	BUINT32 datalen = 0;
	boat_try_declare;
	// DiscoverRes discooverRes = tx_ptr->discooverRes;
	int i, j, k;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	result = hlhuaweiProposalTransactionPacked(tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "[%s]:packed failed.", tx_ptr->var.args.args[0]);
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricTxProposal_exception);
	}
	if (tx_ptr->var.type == HLFABRIC_TYPE_PROPOSAL)
	{
		for (i = 0; i < nodeCfg.endorserLayoutNum; i++)
		{
			for (j = 0; j < nodeCfg.layoutCfg[i].endorserGroupNum; j++)
			{
				valid_node_quantities = 0;
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
// 			BoatLog_hexasciidump(BOAT_LOG_NORMAL, "tlsCAchain  :",
// 			 tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr,
// 			 tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
#endif
					tx_ptr->wallet_ptr->http2Context_ptr->type = tx_ptr->var.type;

					tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &tx_ptr->endorserResponse;

					parsePtr = tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr;

					tx_ptr->wallet_ptr->http2Context_ptr->chainType = HLCHAIN_TYPE_HUAWEI;
					result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
					if (result != BOAT_SUCCESS)
					{
						continue;
					}

					proposalResponse = common__transaction__unpack(NULL, parsePtr->httpResLen - 5, parsePtr->http2Res + 5);
					if (parsePtr->httpResLen != 0)
					{
						BoatFree(parsePtr->http2Res);
					}
					parsePtr->httpResLen = 0;

					if ((proposalResponse != NULL) && (proposalResponse->n_approvals != 0) && (proposalResponse->approvals != NULL))
					{

						resData = common__response__unpack(NULL, proposalResponse->payload.len, proposalResponse->payload.data);
						if (resData->status != COMMON__STATUS__HUAWEI__SUCCESS)
						{
							continue;
						}

						if ((resData->payload.data[1] & 0x80) == 0x80)
						{
							datalen = (resData->payload.data[1] & 0x7F) + (resData->payload.data[2] << 7);
							resPayload = common__tx_payload__unpack(NULL, datalen, resData->payload.data + 3);
						}
						else
						{
							datalen = resData->payload.data[1];
							resPayload = common__tx_payload__unpack(NULL, datalen, resData->payload.data + 2);
						}
						commondTxData = common__common_tx_data__unpack(NULL, resPayload->data.len, resPayload->data.data);
						if (commondTxData->response->status != COMMON__STATUS__HUAWEI__SUCCESS)
						{
							continue;
						}

						parsePtr->response[parsePtr->responseCount].contentPtr = proposalResponse;
						parsePtr->response[parsePtr->responseCount].responseType = HLFABRIC_TYPE_PROPOSAL;
						parsePtr->response[parsePtr->responseCount].payload.field_len = resData->payload.len;
						parsePtr->response[parsePtr->responseCount].payload.field_ptr = BoatMalloc(resData->payload.len);
						memcpy(parsePtr->response[parsePtr->responseCount].payload.field_ptr,resData->payload.data,resData->payload.len);
						parsePtr->httpResLen = commondTxData->response->payload.len;
						parsePtr->http2Res = BoatMalloc(parsePtr->httpResLen);
						memcpy(parsePtr->http2Res,commondTxData->response->payload.data,commondTxData->response->payload.len);
						// parsePtr->http2Res =  commondTxData->response->payload.data;
						parsePtr->responseCount++;
						common__common_tx_data__free_unpacked(commondTxData,NULL);
						common__response__free_unpacked(resData,NULL);
					}
					else
					{
						continue;
					}
					if (funType == HLFABRIC_FUN_EVALUATE)
					{
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
			tx_ptr->wallet_ptr->http2Context_ptr->nodeUrl = nodeCfg.orderCfg.endorser[i].nodeUrl;

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)

			tx_ptr->wallet_ptr->http2Context_ptr->hostName = nodeCfg.orderCfg.endorser[i].hostName;
			// tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain = nodeCfg.orderCfg.tlsOrgCertContent.content;
			if (tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain != NULL)
			{
				if (tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr != NULL)
				{
					BoatFree(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr);
				}
				tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = 0;
				BoatFree(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain);
			}
			tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain = BoatMalloc(sizeof(BoatFieldVariable));
			tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = nodeCfg.orderCfg.tlsOrgCertContent.length + 1;
			tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr = BoatMalloc(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
			memset(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, 0x00, tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
			memcpy(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, nodeCfg.orderCfg.tlsOrgCertContent.content, nodeCfg.orderCfg.tlsOrgCertContent.length);

#endif

			tx_ptr->wallet_ptr->http2Context_ptr->type = tx_ptr->var.type;

			tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &tx_ptr->endorserResponse;

			// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "http2SubmitRequest 111 send :",
			// 					 tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr,
			// 					 tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len);
			parsePtr = tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr;
			tx_ptr->wallet_ptr->http2Context_ptr->chainType = HLCHAIN_TYPE_HUAWEI;
			result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "[http2]http2SubmitRequest failed.");
				continue;
			}

			resData = common__response__unpack(NULL, parsePtr->httpResLen - 7, parsePtr->http2Res + 7);
			BoatLog(BOAT_LOG_NORMAL, "[http2]common__response__unpack respond status : %d .", resData->status);
			if (resData->status == COMMON__STATUS__HUAWEI__SUCCESS)
			{
				result = BOAT_SUCCESS;
				break;
			}
			else
			{
				BoatLog(BOAT_LOG_CRITICAL, "[http2]orderer respond unpacked failed.");
				result = BOAT_ERROR;
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

BOAT_RESULT BoatHlhuaweiWalletSetAccountInfo(BoatHlfabricWallet *wallet_ptr,
											 const BoatWalletPriKeyCtx_config prikeyCtx_config,
											 const BoatHlfabricCertInfoCfg certContent)
{
	return BoatHlfabricWalletSetAccountInfo(wallet_ptr, prikeyCtx_config, certContent);
}

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
BOAT_RESULT BoatHlfabricWalletSetTlsClientInfo(BoatHlfabricWallet *wallet_ptr,
											   const BoatWalletPriKeyCtx_config prikeyCtx_config,
											   const BoatHlfabricCertInfoCfg certContent)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	if (wallet_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	/* initialization */
	memset(&wallet_ptr->tlsClinet_info.prikeyCtx, 0, sizeof(BoatWalletPriKeyCtx));
	wallet_ptr->tlsClinet_info.cert.field_ptr = NULL;
	wallet_ptr->tlsClinet_info.cert.field_len = 0;

	/* prikey context assignment */
	memcpy(&wallet_ptr->tlsClinet_info.prikeyCtx,
		   &prikeyCtx_config.private_KeyCtx, sizeof(BoatWalletPriKeyCtx));

	/* cert assignment */
	wallet_ptr->tlsClinet_info.cert.field_ptr = BoatMalloc(certContent.length);
	if (wallet_ptr->tlsClinet_info.cert.field_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetTlsInfo_exception);
	}
	memcpy(wallet_ptr->tlsClinet_info.cert.field_ptr, certContent.content, certContent.length);
	wallet_ptr->tlsClinet_info.cert.field_len = certContent.length;

	/* boat catch handle */
	boat_catch(BoatHlfabricWalletSetTlsInfo_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		/* free malloc param Deinit */
		BoatFree(wallet_ptr->tlsClinet_info.cert.field_ptr);
		wallet_ptr->tlsClinet_info.cert.field_len = 0;
	}

	return result;
}
#endif

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
BOAT_RESULT BoatHlhuaweiWalletSetRootCaInfo(BoatHlfabricWallet *wallet_ptr,
											const BoatHlfabricCertInfoCfg *rootCaContent,
											BUINT32 rootCaNumber)
{
	return BoatHlfabricWalletSetRootCaInfo(wallet_ptr, rootCaContent, rootCaNumber);
}
#endif

BOAT_RESULT BoatHlhuaweiWalletSetNetworkInfo(BoatHlfabricWallet *wallet_ptr,
											 const BoatHlfabricNodesCfg endorserInfo_ptr)
{

	return BoatHlfabricWalletSetNetworkInfo(wallet_ptr, endorserInfo_ptr);
}

BoatHlfabricWallet *BoatHlhuaweiWalletInit(const BoatHlfabricWalletConfig *config_ptr,
										   BUINT32 config_size)
{
	return BoatHlfabricWalletInit(config_ptr, config_size);
}

void BoatHlhuaweiWalletDeInit(BoatHlfabricWallet *wallet_ptr)
{
	BoatHlfabricWalletDeInit(wallet_ptr);
}

BOAT_RESULT BoatHlhuaweiTxInit(BoatHlfabricTx *tx_ptr,
							   const BoatHlfabricWallet *wallet_ptr,
							   const BCHAR *chaincodeId_path_str,
							   const BCHAR *chaincodeId_name_str,
							   const BCHAR *chaincodeId_version_str,
							   const BCHAR *channelId_str,
							   const BCHAR *orgName_str,
							   const BCHAR *contract_name,
							   const BCHAR *creator_id)
{
	return BoatHlfabricTxInit(tx_ptr, wallet_ptr, chaincodeId_path_str, chaincodeId_name_str, chaincodeId_version_str, channelId_str, orgName_str, contract_name, creator_id);
}

void BoatHlhuaweiTxDeInit(BoatHlfabricTx *tx_ptr)
{
	BoatHlfabricTxDeInit(tx_ptr);
}

BOAT_RESULT BoatHlhuaweiTxSetTimestamp(BoatHlfabricTx *tx_ptr,
									   const BUINT64 sec,
									   const BUINT64 nanos)
{
	return BoatHlfabricTxSetTimestamp(tx_ptr, sec, nanos);
}

BOAT_RESULT BoatHlhuaweiTxEvaluate(BoatHlfabricTx *tx_ptr)
{
	// BoatHlfabricNodeInfo urlTmp[2]    = {{NULL,NULL}, {NULL,NULL}};
	BOAT_RESULT result = BOAT_SUCCESS;
	// boat_try_declare;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	if (tx_ptr->var.args.args[0] == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments args[0] cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	BoatLog(BOAT_LOG_NORMAL, "Evaluate will execute...");

	/* submit query */
	tx_ptr->var.type = HLFABRIC_TYPE_PROPOSAL;
	// urlTmp[0] = tx_ptr->wallet_ptr->network_info.endorser[0];
	result = BoatHlhuaweiTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info, HLFABRIC_FUN_EVALUATE);
	BoatLog_hexasciidump(BOAT_LOG_NORMAL, "query result",
						 tx_ptr->endorserResponse.http2Res,
						 tx_ptr->endorserResponse.httpResLen);

	/* free the unpacked response data */
	for (int i = 0; i < tx_ptr->endorserResponse.responseCount; i++)
	{
		if (tx_ptr->endorserResponse.response[i].responseType == HLFABRIC_TYPE_PROPOSAL)
		{
			common__transaction__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
			if(tx_ptr->endorserResponse.response[i].payload.field_len != 0)
			{
				BoatFree(tx_ptr->endorserResponse.response[i].payload.field_ptr);
				tx_ptr->endorserResponse.response[i].payload.field_len = 0;
			}
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

BOAT_RESULT BoatHlhuaweiTxSubmit(BoatHlfabricTx *tx_ptr)
{
	// BoatHlfabricNodeInfo urlTmp[2]    = {{NULL,NULL}, {NULL,NULL}};
	BOAT_RESULT result = BOAT_SUCCESS;
	// boat_try_declare;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	if (tx_ptr->var.args.args[0] == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments args[0] cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	BoatLog(BOAT_LOG_NORMAL, "Submit will execute...");

	/* invoke-step1: submit proposal to endorer */
	tx_ptr->var.type = HLFABRIC_TYPE_PROPOSAL;
	result = BoatHlhuaweiTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info, HLFABRIC_FUN_SUBMIT);
	if (result != BOAT_SUCCESS)
	{
		return BOAT_ERROR;
	}
	BoatLog(BOAT_LOG_NORMAL, "Submit proposal OK ...");
	/* invoke-step2: submit transaction to orderer */
	tx_ptr->var.type = HLFABRIC_TYPE_TRANSACTION;
	result = BoatHlhuaweiTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info, HLFABRIC_FUN_SUBMIT);
	if (result != BOAT_SUCCESS)
	{
		return BOAT_ERROR;
	}
	/* free the unpacked response data */
	for (int i = 0; i < tx_ptr->endorserResponse.responseCount; i++)
	{
		if (tx_ptr->endorserResponse.response[i].responseType == HLFABRIC_TYPE_PROPOSAL)
		{
			common__transaction__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
		else
		{
			// orderer__submit_response__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
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
#endif /* end of PROTOCOL_USE_HLFABRIC */
