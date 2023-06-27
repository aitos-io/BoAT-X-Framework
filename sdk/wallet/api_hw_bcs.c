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

/*!@brief hwbcs wallet API for BoAT IoT SDK

@file
api_hw_bcs.c defines the hwbcs wallet API for BoAT IoT SDK.
*/

/* self-header include */
#include "boatinternal.h"
#if PROTOCOL_USE_HWBCS == 1
#include "boatprotocols.h"
#include "http2intf.h"
/* protos include */
#include "peer/proposal_response.pb-c.h"
#include "orderer/cluster.pb-c.h"
#include "common/transaction.pb-c-hwbcs.h"

#include "boatutility.h"

BOAT_RESULT BoatHwbcsTxSetArgs(BoatHwbcsTx *tx_ptr,
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
		if (tx_ptr->var.args.nArgs >= BOAT_HWBCS_ARGS_MAX_NUM)
		{
			result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
			break;
		}
	}
	va_end(ap);

	return result;
}

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
 *   \n For endorser node, this param is equeal with the macro #BOAT_HWBCS_ENDORSER_MAX_NUM,
 *   for orderer node, this param is equeal with the macro #BOAT_HWBCS_ORDERER_MAX_NUM.
 *
 * @return
 *   Return \c BOAT_SUCCESS if set successed, otherwise return a failed code.
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT BoatHwbcsTxExec(BoatHwbcsTx *tx_ptr,
										 BoatHwbcsNodesCfg nodeCfg, BoatHwbcsFunType funType)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHwbcsEndorserResponse *parsePtr = NULL;
	Http2Response hwbcsHttp2res = {0, NULL};
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
	result = hwbcsProposalTransactionPacked(tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "[%s]:packed failed.", tx_ptr->var.args.args[0]);
		boat_throw(BOAT_ERROR_COMMON_PROTO_PACKET_FAIL, BoatHwbcsTxProposal_exception);
	}
	if (tx_ptr->var.type == HWBCS_TYPE_PROPOSAL)
	{
		for (i = 0; i < nodeCfg.endorserLayoutNum; i++)
		{
			for (j = 0; j < nodeCfg.layoutCfg[i].endorserGroupNum; j++)
			{
				valid_node_quantities = 0;
				for (k = 0; k < nodeCfg.layoutCfg[i].groupCfg[j].endorserNumber; k++)
				{
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->nodeUrl = nodeCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl;
#if (BOAT_HWBCS_TLS_SUPPORT == 1)

					// if (((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain != NULL)
					{
						if (((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr != NULL)
						{
							BoatFree(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr);
						}
						((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len = 0;
						// BoatFree(((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain);
					}

					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->hostName = nodeCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName;
					// ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain = BoatMalloc(sizeof(BoatFieldVariable));
					// if(NULL == ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain){
					// 	BoatLog(BOAT_LOG_CRITICAL, " Fail to malloc tlsCAchain memory");
					// 	boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsTxProposal_exception);
					// }
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len = nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length + 1;
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr = BoatMalloc(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len);
					if (NULL == ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr)
					{
						BoatLog(BOAT_LOG_CRITICAL, " Fail to malloc tlsCAchain field_ptr memory");
						boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsTxProposal_exception);
					}
					memset(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr, 0x00, ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len);
					memcpy(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr, nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, nodeCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length);
					// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "tlsCAchain  :",
					// 					 ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain[0].field_ptr,
					// 					 ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain[0].field_len);
#endif
					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->type = tx_ptr->var.type;

					// ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->parseDataPtr = &tx_ptr->endorserResponse;

					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->parseDataPtr = &(hwbcsHttp2res);

					((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->pathTmp = "/nodeservice.Contract/Invoke";
					result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
					if (result != BOAT_SUCCESS)
					{
						continue;
					}

					proposalResponse = common__transaction__unpack(NULL, hwbcsHttp2res.httpResLen - 5, hwbcsHttp2res.http2Res + 5);
					if (hwbcsHttp2res.httpResLen != 0)
					{
						BoatFree(hwbcsHttp2res.http2Res);
					}
					hwbcsHttp2res.httpResLen = 0;

					if ((proposalResponse != NULL) && (proposalResponse->n_approvals != 0) && (proposalResponse->approvals != NULL))
					{

						resData = common__response__unpack(NULL, proposalResponse->payload.len, proposalResponse->payload.data);
						if (resData->status != COMMON__STATUS__HWBCS__SUCCESS)
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
						if (commondTxData->response->status != COMMON__STATUS__HWBCS__SUCCESS)
						{
							continue;
						}

						parsePtr->response[parsePtr->responseCount].contentPtr = proposalResponse;
						parsePtr->response[parsePtr->responseCount].responseType = HWBCS_TYPE_PROPOSAL;
						parsePtr->response[parsePtr->responseCount].payload.field_len = resData->payload.len;
						parsePtr->response[parsePtr->responseCount].payload.field_ptr = BoatMalloc(resData->payload.len);
						if (NULL == parsePtr->response[parsePtr->responseCount].payload.field_ptr)
						{
							BoatLog(BOAT_LOG_CRITICAL, " Fail to malloc payload field_ptr memory");
							boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsTxProposal_exception);
						}
						memcpy(parsePtr->response[parsePtr->responseCount].payload.field_ptr, resData->payload.data, resData->payload.len);
						tx_ptr->evaluateRes.httpResLen = commondTxData->response->payload.len;
						tx_ptr->evaluateRes.http2Res = BoatMalloc(tx_ptr->evaluateRes.httpResLen);
						if (NULL == tx_ptr->evaluateRes.http2Res)
						{
							BoatLog(BOAT_LOG_CRITICAL, " Fail to malloc http2Res memory");
							boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsTxProposal_exception);
						}
						memcpy(tx_ptr->evaluateRes.http2Res, commondTxData->response->payload.data, commondTxData->response->payload.len);

						// parsePtr->http2res.httpResLen = commondTxData->response->payload.len;
						// parsePtr->http2res.http2Res = BoatMalloc(parsePtr->http2res.httpResLen);
						// memcpy(parsePtr->http2res.http2Res, commondTxData->response->payload.data, commondTxData->response->payload.len);
						// parsePtr->http2Res =  commondTxData->response->payload.data;
						parsePtr->responseCount++;
						common__common_tx_data__free_unpacked(commondTxData, NULL);
						common__response__free_unpacked(resData, NULL);
					}
					else
					{
						continue;
					}
					if (funType == HWBCS_FUN_EVALUATE)
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
					boat_throw(BOAT_ERROR, BoatHwbcsTxProposal_exception);
				}
			}
		}
	}
	else
	{
		for (i = 0; i < nodeCfg.orderCfg.endorserNumber; i++)
		{
			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->nodeUrl = nodeCfg.orderCfg.endorser[i].nodeUrl;

#if (BOAT_HWBCS_TLS_SUPPORT == 1)

			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->hostName = nodeCfg.orderCfg.endorser[i].hostName;
			// ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain = nodeCfg.orderCfg.tlsOrgCertContent.content;
			// if (((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain != NULL)
			{
				if (((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr != NULL)
				{
					BoatFree(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr);
				}
				((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len = 0;
				// BoatFree(((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain);
			}
			// ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain = BoatMalloc(sizeof(BoatFieldVariable));
			// if(NULL == ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain){
			// 	BoatLog(BOAT_LOG_CRITICAL, " Fail to malloc tlsCAchain memory");
			// 	boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsTxProposal_exception);
			// }
			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len = nodeCfg.orderCfg.tlsOrgCertContent.length + 1;
			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr = BoatMalloc(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len);
			if (NULL == ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr)
			{
				BoatLog(BOAT_LOG_CRITICAL, " Fail to malloc tlsCAchain field_ptr memory");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsTxProposal_exception);
			}
			memset(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr, 0x00, ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_len);
			memcpy(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCAchain.field_ptr, nodeCfg.orderCfg.tlsOrgCertContent.content, nodeCfg.orderCfg.tlsOrgCertContent.length);

#endif

			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->type = tx_ptr->var.type;

			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->parseDataPtr = &hwbcsHttp2res;

			// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "http2SubmitRequest 111 send :",
			// 					 ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->sendBuf.field_ptr,
			// 					 ((http2IntfContext*)(tx_ptr->wallet_ptr->http2Context_ptr))->sendBuf.field_len);
			parsePtr = ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->parseDataPtr;
			((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->pathTmp = "/nodeservice.TransactionSender/SendTransaction";
			result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "[http2]http2SubmitRequest failed.");
				continue;
			}

			resData = common__response__unpack(NULL, hwbcsHttp2res.httpResLen - 7, hwbcsHttp2res.http2Res + 7);
			BoatLog(BOAT_LOG_NORMAL, "[http2]common__response__unpack respond status : %d .", resData->status);
			if (hwbcsHttp2res.httpResLen != 0)
			{
				BoatFree(hwbcsHttp2res.http2Res);
			}
			hwbcsHttp2res.httpResLen = 0;
			if (resData->status == COMMON__STATUS__HWBCS__SUCCESS)
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
	boat_catch(BoatHwbcsTxProposal_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	return result;
}

// #if (BOAT_HWBCS_TLS_SUPPORT == 1) && (BOAT_HWBCS_TLS_IDENTIFY_CLIENT == 1)
// BOAT_RESULT BoatHwbcsWalletSetTlsClientInfo(BoatHwbcsWallet *wallet_ptr,
// 											const BoatWalletPriKeyCtx_config prikeyCtx_config,
// 											const BoatHwbcsCertInfoCfg certContent)
// {
// 	BOAT_RESULT result = BOAT_SUCCESS;
// 	boat_try_declare;

// 	if (wallet_ptr == NULL)
// 	{
// 		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
// 		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
// 	}

// 	/* initialization */
// 	memset(&wallet_ptr->tlsClinet_info.prikeyCtx, 0, sizeof(BoatWalletPriKeyCtx));
// 	wallet_ptr->tlsClinet_info.cert.field_ptr = NULL;
// 	wallet_ptr->tlsClinet_info.cert.field_len = 0;

// 	/* prikey context assignment */
// 	memcpy(&wallet_ptr->tlsClinet_info.prikeyCtx,
// 		   &prikeyCtx_config.private_KeyCtx, sizeof(BoatWalletPriKeyCtx));

// 	/* cert assignment */
// 	wallet_ptr->tlsClinet_info.cert.field_ptr = BoatMalloc(certContent.length);
// 	if (wallet_ptr->tlsClinet_info.cert.field_ptr == NULL)
// 	{
// 		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
// 		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsWalletSetTlsInfo_exception);
// 	}
// 	memcpy(wallet_ptr->tlsClinet_info.cert.field_ptr, certContent.content, certContent.length);
// 	wallet_ptr->tlsClinet_info.cert.field_len = certContent.length;

// 	/* boat catch handle */
// 	boat_catch(BoatHwbcsWalletSetTlsInfo_exception)
// 	{
// 		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
// 		result = boat_exception;
// 		/* free malloc param Deinit */
// 		BoatFree(wallet_ptr->tlsClinet_info.cert.field_ptr);
// 		wallet_ptr->tlsClinet_info.cert.field_len = 0;
// 	}

// 	return result;
// }
// #endif

/**
 * @description:
 * 	This function init hwbcs wallet by keypair index and network index;
 * @param {BUINT8} keypairIndex
 * @param {BUINT8} networkIndex
 * @return {*}
 *  This function returns hwbcswallet if successfully executed.
 *  Otherwise it returns NULL.
 * @author: aitos
 */
BoatHwbcsWallet *BoatHwbcsWalletInit(BUINT8 keypairIndex, BUINT8 networkIndex)
{
	BoatHwbcsWallet *wallet_ptr = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;
	// BUINT8 i = 0;

	/* allocate wallet memory */
	wallet_ptr = BoatMalloc(sizeof(BoatHwbcsWallet));
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
	result = BoATHwbcs_GetNetworkByIndex(&(wallet_ptr->network_info), networkIndex);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "get network by index fail");
		return NULL;
	}
#if (BOAT_HWBCS_TLS_SUPPORT == 1)
	wallet_ptr->tlsCAchain.ca.field_len = 0;
	wallet_ptr->tlsCAchain.ca.field_ptr = NULL;
#endif

	wallet_ptr->http2Context_ptr = NULL;
	/* network_info assignment */
	// result += BoatHwbcsWalletSetNetworkInfo(wallet_ptr, config_ptr->nodesCfg);
	/* http2Context_ptr assignment */
	wallet_ptr->http2Context_ptr = http2Init();

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to set accountInfo|TlsUInfo|networkInfo.");
		return NULL;
	}

	return wallet_ptr;
}

void BoatHwbcsWalletDeInit(BoatHwbcsWallet *wallet_ptr)
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
#if (BOAT_HWBCS_TLS_SUPPORT == 1)
	// for c99, free(NULL) will return directly, so here
	// use BOAT_HWBCS_ROOTCA_MAX_NUM as cyclic maximum is acceptable.
	if (wallet_ptr->tlsCAchain.ca.field_ptr != NULL)
	{
		BoatFree(wallet_ptr->tlsCAchain.ca.field_ptr);
	}
	wallet_ptr->tlsCAchain.ca.field_len = 0;
#endif /* #if (BOAT_HWBCS_TLS_SUPPORT == 1) */

	/* network_info DeInit */

	BoATHwbcs_FreeNetworkData(wallet_ptr->network_info);

	/* http2Context DeInit */
	http2DeInit(wallet_ptr->http2Context_ptr);

	/* wallet_ptr DeInit */
	BoatFree(wallet_ptr);

	/* set NULL after free completed */
	wallet_ptr = NULL;
}

BOAT_RESULT BoatHwbcsTxInit(BoatHwbcsTx *tx_ptr,
							const BoatHwbcsWallet *wallet_ptr,
							const BCHAR *chaincodeId_path_str,
							const BCHAR *chaincodeId_name_str,
							const BCHAR *chaincodeId_version_str,
							const BCHAR *channelId_str,
							const BCHAR *orgName_str,
							const BCHAR *contract_name)
{
	BUINT32 stringLen;
	BCHAR *paramSrcList[6];
	BCHAR **paramDstList[6];
	BUINT16 i = 0;
	BOAT_RESULT result = BOAT_SUCCESS;

	boat_try_declare;

	if ((tx_ptr == NULL) || (wallet_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments 'tx_ptr' or 'wallet_ptr' cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* tx_ptr instance reset */
	/* ----->tx_ptr->wallet_ptr reset */
	tx_ptr->wallet_ptr = NULL;
	/* ----->tx_ptr->var reset */
	tx_ptr->var.type = HWBCS_TYPE_IDLE;
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
	for (i = 0; i < BOAT_HWBCS_ARGS_MAX_NUM; i++)
	{
		tx_ptr->var.args.args[i] = NULL;
	}
	tx_ptr->var.channelId = NULL;
	tx_ptr->var.orgName = NULL;
	tx_ptr->evaluateRes.httpResLen = 0;
	tx_ptr->evaluateRes.http2Res = NULL;
	/* ----->tx_ptr->endorserResponse reset */
	tx_ptr->endorserResponse.responseCount = 0;
	for (i = 0; i < BOAT_HWBCS_ENDORSER_MAX_NUM; i++)
	{
		tx_ptr->endorserResponse.response[i].contentPtr = NULL;
		tx_ptr->endorserResponse.response[i].responseType = HWBCS_TYPE_IDLE;
		tx_ptr->endorserResponse.response[i].payload.field_len = 0;
		tx_ptr->endorserResponse.response[i].payload.field_ptr = NULL;
		tx_ptr->endorserResponse.response[i].endorser.field_len = 0;
		tx_ptr->endorserResponse.response[i].endorser.field_ptr = NULL;
		tx_ptr->endorserResponse.response[i].signature.field_len = 0;
		tx_ptr->endorserResponse.response[i].signature.field_ptr = NULL;
	}

	/* wallet assignment */
	tx_ptr->wallet_ptr = wallet_ptr;

	/* paramList assignment */
	paramSrcList[0] = (BCHAR *)chaincodeId_path_str;
	paramSrcList[1] = (BCHAR *)chaincodeId_name_str;
	paramSrcList[2] = (BCHAR *)chaincodeId_version_str;
	paramSrcList[3] = (BCHAR *)channelId_str;
	paramSrcList[4] = (BCHAR *)orgName_str;
	paramSrcList[5] = (BCHAR *)contract_name;
	paramDstList[0] = &tx_ptr->var.chaincodeId.path;
	paramDstList[1] = &tx_ptr->var.chaincodeId.name;
	paramDstList[2] = &tx_ptr->var.chaincodeId.version;
	paramDstList[3] = &tx_ptr->var.channelId;
	paramDstList[4] = &tx_ptr->var.orgName;
	paramDstList[5] = &tx_ptr->var.contract_name;

	for (i = 0; i < sizeof(paramSrcList) / sizeof(paramSrcList[0]); i++)
	{
		if ((paramSrcList[i] != NULL) && ((stringLen = strlen(paramSrcList[i])) > 0))
		{
			*paramDstList[i] = BoatMalloc(stringLen + 1);
			if (*paramDstList[i] == NULL)
			{
				BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsTxInit_exception);
			}
			memcpy(*paramDstList[i], paramSrcList[i], stringLen + 1);
		}
	}
	char value[64];
	size_t value_len;

	// value_len = boat_find_oid_value_in_name(name, "CN", value, sizeof(value));

	value_len = boat_find_subject_common_name(tx_ptr->wallet_ptr->network_info.accountCertContent.content, tx_ptr->wallet_ptr->network_info.accountCertContent.length, value, sizeof(value));

	if (value_len)
	{
		tx_ptr->var.creator_id = BoatMalloc(value_len + 1);
		if (NULL == tx_ptr->var.creator_id)
		{
			BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
			boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHwbcsTxInit_exception);
		}
		memcpy(tx_ptr->var.creator_id, value, value_len);
	}

	/* boat catch handle */
	boat_catch(BoatHwbcsTxInit_exception)
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

void BoatHwbcsTxDeInit(BoatHwbcsTx *tx_ptr)
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
	for (int i = 0; i < BOAT_HWBCS_ARGS_MAX_NUM; i++)
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
		BoatHwbcsWalletDeInit(tx_ptr->wallet_ptr);
	}

	/* endorserResponse DeInit */
	// DO NOTHING:THIS FIELD HAD FREE AFTER EVERY TRANSCATION COMPLETED
}

BOAT_RESULT BoatHwbcsTxSetTimestamp(BoatHwbcsTx *tx_ptr,
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

BOAT_RESULT BoatHwbcsTxEvaluate(BoatHwbcsTx *tx_ptr)
{
	// BoatHwbcsNodeInfo urlTmp[2]    = {{NULL,NULL}, {NULL,NULL}};
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
	tx_ptr->var.type = HWBCS_TYPE_PROPOSAL;
	// urlTmp[0] = tx_ptr->wallet_ptr->network_info.endorser[0];
	result = BoatHwbcsTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info.nodesCfg, HWBCS_FUN_EVALUATE);

	/* free the unpacked response data */
	for (int i = 0; i < tx_ptr->endorserResponse.responseCount; i++)
	{
		if (tx_ptr->endorserResponse.response[i].responseType == HWBCS_TYPE_PROPOSAL)
		{
			common__transaction__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
			if (tx_ptr->endorserResponse.response[i].payload.field_len != 0)
			{
				BoatFree(tx_ptr->endorserResponse.response[i].payload.field_ptr);
				tx_ptr->endorserResponse.response[i].payload.field_len = 0;
			}
		}
	}
	tx_ptr->endorserResponse.responseCount = 0;

	return result;
}

BOAT_RESULT BoatHwbcsTxSubmit(BoatHwbcsTx *tx_ptr)
{
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
	tx_ptr->var.type = HWBCS_TYPE_PROPOSAL;
	result = BoatHwbcsTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info.nodesCfg, HWBCS_FUN_SUBMIT);
	if (result != BOAT_SUCCESS)
	{
		return result;
	}
	BoatLog(BOAT_LOG_NORMAL, "Submit proposal OK ...");
	/* invoke-step2: submit transaction to orderer */
	tx_ptr->var.type = HWBCS_TYPE_TRANSACTION;
	result = BoatHwbcsTxExec(tx_ptr, tx_ptr->wallet_ptr->network_info.nodesCfg, HWBCS_FUN_SUBMIT);
	if (result != BOAT_SUCCESS)
	{
		return result;
	}
	/* free the unpacked response data */
	for (int i = 0; i < tx_ptr->endorserResponse.responseCount; i++)
	{
		if (tx_ptr->endorserResponse.response[i].responseType == HWBCS_TYPE_PROPOSAL)
		{
			common__transaction__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
		else
		{
			// orderer__submit_response__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
	}
	tx_ptr->endorserResponse.responseCount = 0;

	return result;
}

void hwbcsWalletConfigFree(BoatHwbcsNetworkConfig network_config)
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
#endif /* end of PROTOCOL_USE_HWBCS */
