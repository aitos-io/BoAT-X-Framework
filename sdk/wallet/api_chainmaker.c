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
api_chainmaker.c defines the Ethereum wallet API for BoAT IoT SDK.
*/

/* self-header include */
#include "boatinternal.h"


BOAT_RESULT BoatHlchainmakerWalletSetRootCaInfo(BoatHlchainmakerWallet *wallet_ptr,
											const BoatHlchainmakerCertInfoCfg *rootCaContent,
											BUINT32 rootCaNumber)
{
	BUINT16 i = 0;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	if (rootCaContent == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	if ((rootCaNumber == 0) || (rootCaNumber > BOAT_CHAINMAKER_ROOTCA_MAX_NUM))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter rootCaNumber out of limit.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	/* initialization */
	for (i = 0; i < BOAT_CHAINMAKER_ROOTCA_MAX_NUM; i++)
	{
		wallet_ptr->tls_rootCA_info[i].field_len = 0;
		wallet_ptr->tls_rootCA_info[i].field_ptr = NULL;
	}

	/* assignment */
	for (i = 0; i < rootCaNumber; i++)
	{
		/* get rootCA file size */
		wallet_ptr->tls_rootCA_info[i].field_len = (rootCaContent + i)->length;

		wallet_ptr->tls_rootCA_info[i].field_ptr = BoatMalloc(wallet_ptr->tls_rootCA_info[i].field_len);
		if (wallet_ptr->tls_rootCA_info[i].field_ptr == NULL)
		{
			BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
			boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatChainmakerWalletSetRootCaInfo_exception);
		}
		memset(wallet_ptr->tls_rootCA_info[i].field_ptr, 0, wallet_ptr->tls_rootCA_info[i].field_len);
		memcpy(wallet_ptr->tls_rootCA_info[i].field_ptr, (rootCaContent + i)->content, wallet_ptr->tls_rootCA_info[i].field_len);
	}

	/* boat catch handle */
	boat_catch(BoatChainmakerWalletSetRootCaInfo_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		/* free malloc param Deinit */
		for (i = 0; i < rootCaNumber; i++)
		{
			if (wallet_ptr->tls_rootCA_info[i].field_ptr != NULL)
			{
				BoatFree(wallet_ptr->tls_rootCA_info[i].field_ptr);
				wallet_ptr->tls_rootCA_info[i].field_len = 0;
			}
		}
	}

	return result;
}


BOAT_RESULT BoatHlchainmakerWalletSetUserClientInfo(BoatHlchainmakerWallet *wallet_ptr,
											 const BoatWalletPriKeyCtx_config prikeyCtx_config,
											 const BoatHlchainmakerCertInfoCfg certContent)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	if (wallet_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	/* initialization */
	memset(&wallet_ptr->user_client_info.prikeyCtx, 0, sizeof(BoatWalletPriKeyCtx));
	wallet_ptr->user_client_info.cert.field_ptr = NULL;
	wallet_ptr->user_client_info.cert.field_len = 0;

	/* prikey context assignment */
	if (prikeyCtx_config.prikey_content.field_ptr != NULL)
	{
		if (BOAT_SUCCESS != BoatPort_keyCreate(&prikeyCtx_config, &wallet_ptr->user_client_info.prikeyCtx))
		{
			BoatLog(BOAT_LOG_CRITICAL, "Failed to exec BoatPort_keyCreate.");
			return BOAT_ERROR_INVALID_ARGUMENT;
		}
	}

	/* cert assignment */
	wallet_ptr->user_client_info.cert.field_ptr = BoatMalloc(certContent.length);
	if (wallet_ptr->user_client_info.cert.field_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatChainmakerWalletSetAccountInfo_exception);
	}
	memcpy(wallet_ptr->user_client_info.cert.field_ptr, certContent.content, certContent.length);
	wallet_ptr->user_client_info.cert.field_len = certContent.length;

	/* boat catch handle */
	boat_catch(BoatChainmakerWalletSetAccountInfo_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		/* free malloc param Deinit */
		BoatFree(wallet_ptr->user_client_info.cert.field_ptr);
		wallet_ptr->user_client_info.cert.field_len = 0;
	}

	return result;
}

BoatHlchainmakerWallet *BoatHlchainmakerWalletInit(const BoatHlchainmakerWalletConfig *config_ptr,
										   BUINT32 config_size)
{
	BoatHlchainmakerWallet *wallet_ptr = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT8 i = 0;

	if ((config_ptr == NULL) || (config_size == 0))
	{
		BoatLog(BOAT_LOG_CRITICAL, "config_ptr cannot be NULL or config_size cannot be zero.");
		return NULL;
	}

	/* allocate wallet memory */
	wallet_ptr = BoatMalloc(sizeof(BoatHlchainmakerWallet));
	if (wallet_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to malloc wallet memory.");
		return NULL;
	}
	if (sizeof(BoatHlChainmakerWalletConfig) != config_size)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Incorrect configuration size.");
		return NULL;
	}

	/* initialization */
	wallet_ptr->user_client_info.cert.field_ptr = NULL;
	wallet_ptr->user_client_info.cert.field_len = 0;
	wallet_ptr->tls_client_info.cert.field_ptr = NULL;
	wallet_ptr->tls_client_info.cert.field_len = 0;

	for (i = 0; i < BOAT_CHAINMAKER_ROOTCA_MAX_NUM; i++)
	{
		wallet_ptr->tls_rootCA_info[i].field_len = 0;
		wallet_ptr->tls_rootCA_info[i].field_ptr = NULL;
	}

	wallet_ptr->http2Context_ptr = NULL;

	/* account_info assignment */
	result += BoatHlchainmakerWalletSetUserClientInfo(wallet_ptr, config_ptr->user_prikey_config,
											   config_ptr->user_cert_content);

	/* tlsRootCa_info assignment */
	BoatHlchainmakerWalletSetRootCaInfo(wallet_ptr, config_ptr->root_ca_cert, config_ptr->root_ca_number);

	/* http2Context_ptr assignment */
	wallet_ptr->http2Context_ptr = http2Init();

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to set accountInfo|TlsUInfo|networkInfo.");
		return NULL;
	}

	return wallet_ptr;
}

void BoatHlchainmakerWalletDeInit(BoatHlchainmakerWallet *wallet_ptr)
{
	BUINT16 i, j, k;

	if (NULL == wallet_ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr needn't DeInit: wallet_ptr is NULL.");
		return;
	}

	/* account_info DeInit */
	BoatFree(wallet_ptr->user_client_info.cert.field_ptr);
	wallet_ptr->user_client_info.cert.field_len = 0;

	/* tlsClinet_info DeInit */
#if 0
	BoatFree(wallet_ptr->tls_rootCA_info.field_ptr);
	wallet_ptr->tls_rootCA_info.field_len = 0;
	
	for (i = 0; i < BOAT_HLFABRIC_ROOTCA_MAX_NUM; i++)
	{
		BoatFree(wallet_ptr->tlsCAchain.ca[i].field_ptr);
		wallet_ptr->tlsCAchain.ca[i].field_len = 0;
	}
#endif

	if (wallet_ptr->node_info.host_name != NULL)
	{
		BoatFree(wallet_ptr->node_info.host_name);
	}
	if (wallet_ptr->node_info.node_url != NULL)
	{
		BoatFree(wallet_ptr->node_info.node_url);
	}
	/* http2Context DeInit */
	http2DeInit(wallet_ptr->http2Context_ptr);

	/* wallet_ptr DeInit */
	BoatFree(wallet_ptr);

	/* set NULL after free completed */
	wallet_ptr = NULL;
}

BOAT_RESULT BoatHlChainmakerTxInit(const BoatHlchainmakerWallet *wallet_ptr, BoatHlchainmakerTx *tx_ptr)
{
	BUINT32 stringLen;
	BUINT16 i = 0;
	BOAT_RESULT result = BOAT_SUCCESS;

	boat_try_declare;

	if ((tx_ptr == NULL) || (wallet_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments 'tx_ptr' or 'wallet_ptr' cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	tx_ptr->nod_info.org_Id    = wallet_ptr->node_info.org_Id;
	tx_ptr->nod_info.chain_Id  = wallet_ptr->node_info.chain_Id;
	tx_ptr->nod_info.host_name = wallet_ptr->node_info.host_name;
	tx_ptr->nod_info.node_url  = wallet_ptr->node_info.node_url;
	
	for (i = 0; i < BOAT_CHAINMAKER_ROOTCA_MAX_NUM; i++)
	{
		stringLen = wallet_ptr->tls_rootCA_info[i].field_len;
		memcpy(tx_ptr->wallet_ptr.tls_rootCA_info[i].field_ptr, wallet_ptr->tls_rootCA_info[i].field_ptr, stringLen);
	}

	return result;
}


void BoatHlchainmakerTxDeInit(BoatHlchainmakerTx *tx_ptr)
{
	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Tx argument cannot be nULL.");
		return;
	}
	/* -----var.chaincodeId */
	if(tx_ptr->tx_reponse.httpResLen != 0)
	{
		BoatFree(tx_ptr->tx_reponse.http2Res);
	}
}

__BOATSTATIC BOAT_RESULT BoatHlchainmakerTxExec(BoatHlchainmakerNode node_cfg, BoatHlchainmakerTx *tx_ptr, char* method, TxType tx_type)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHlfabricEndorserResponse *parsePtr = NULL;
	_Common__TxResponse *tx_reponse = NULL;

	boat_try_declare;
	int i, j, k;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	result = hlchainmakerTransactionPacked(tx_ptr, method, tx_type);
	if (result != BOAT_SUCCESS)
	{
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlchainmakerTxProposal_exception);
	}

	tx_ptr->wallet_ptr->http2Context_ptr->nodeUrl = node_cfg.node_url;

	// clear last data
	if (tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain != NULL)
	{
		if (tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr != NULL)
		{
			BoatFree(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr);
		}
		tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = 0;
		BoatFree(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain);
	}

	tx_ptr->wallet_ptr->http2Context_ptr->hostName = node_cfg.host_name;
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain = BoatMalloc(sizeof(BoatFieldVariable));
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = node_cfg.tls_root_cert.length + 1;
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr = BoatMalloc(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
	memset(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, 0x00, tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
	memcpy(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, node_cfg.tls_root_cert.content, node_cfg.tls_root_cert.length);

	tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &tx_ptr->tx_reponse;

	parsePtr = tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr;
	tx_ptr->wallet_ptr->http2Context_ptr->chainType = 2;
	result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
	if(result != BOAT_SUCCESS)
	{
		continue;
	}
	
	tx_reponse = common__tx_response__unpack(NULL, parsePtr->httpResLen, parsePtr->http2Res);
	if(parsePtr->httpResLen != 0)
	{
		BoatFree(parsePtr->http2Res);
	}
	
	parsePtr->httpResLen = 0;
	if ((tx_reponse != NULL) 
	{
		BoatLog(BOAT_LOG_NORMAL, "[http2]endorser respond received.")
	}
	return result;
}

BOAT_RESULT BoatHlchainmakerContractClaimInvoke(BoatHlchainmakerTx *tx_ptr, char* method)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	TxType tx_type = TxType_INVOKE_USER_CONTRACT;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	BoatLog(BOAT_LOG_NORMAL, "Submit will execute...");
	result = BoatHlchainmakerTxExec(tx_ptr->wallet_ptr->node_info, tx_ptr, method, tx_type);
	if (result != BOAT_SUCCESS)
	{
		return BOAT_ERROR;
	}
	return result;
}


BOAT_RESULT BoatHlchainmakerContractClaimQuery(BoatHlchainmakerTx *tx_ptr, char* method)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	TxType tx_type = TxType_INVOKE_USER_CONTRACT;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	BoatLog(BOAT_LOG_NORMAL, "Submit will execute...");
	result = BoatHlchainmakerTxExec(tx_ptr, tx_ptr->wallet_ptr->node_info, method, tx_type);
	if (result != BOAT_SUCCESS)
	{
		return BOAT_ERROR;
	}

	return result;
}




							   
