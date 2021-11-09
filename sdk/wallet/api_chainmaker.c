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
#include "boatiotsdk.h"
#include "http2intf.h"
#include "common/result.pb-c.h"

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

BoatHlchainmakerWallet *BoatHlchainmakerWalletInit(const BoatHlchainmakerWalletConfig *config_ptr, BUINT32 config_size)
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
	if (sizeof(BoatHlchainmakerWalletConfig) != config_size)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Incorrect configuration size.");
		return NULL;
	}

	/* initialization */
	wallet_ptr->user_client_info.cert.field_ptr = NULL;
	wallet_ptr->user_client_info.cert.field_len = 0;	
	wallet_ptr->tls_client_info.cert.field_ptr = NULL;
	wallet_ptr->tls_client_info.cert.field_len = 0;

	wallet_ptr->node_info.org_tls_ca_cert.length = 0;
	wallet_ptr->http2Context_ptr = NULL;

	wallet_ptr->node_info.host_name = config_ptr->node_cfg.host_name;
	wallet_ptr->node_info.node_url  = config_ptr->node_cfg.node_url;

	/* assignment */
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
	wallet_ptr->node_info.org_tls_ca_cert.length = config_ptr->node_cfg.org_tls_ca_cert.length;
	memset(wallet_ptr->node_info.org_tls_ca_cert.content, 0, BOAT_CHAINMAKER_CERT_MAX_LEN);
	memcpy(wallet_ptr->node_info.org_tls_ca_cert.content, config_ptr->node_cfg.org_tls_ca_cert.content, wallet_ptr->node_info.org_tls_ca_cert.length);
#endif

	/* account_info assignment */
	result = BoatHlchainmakerWalletSetUserClientInfo(wallet_ptr, config_ptr->user_prikey_config, config_ptr->user_cert_content);
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
	if (NULL == wallet_ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr needn't DeInit: wallet_ptr is NULL.");
		return;
	}
	/* account_info DeInit */
	BoatFree(wallet_ptr->user_client_info.cert.field_ptr);
	wallet_ptr->user_client_info.cert.field_len = 0;
	
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
	/* tlsClinet_info DeInit */
	// BoatFree(wallet_ptr->tls_client_info.field_ptr);
	// wallet_ptr->tls.field_len = 0;
	
	// for (i = 0; i < BOAT_HLFABRIC_ROOTCA_MAX_NUM; i++)
	// {
	// 	BoatFree(wallet_ptr->tlsCAchain.ca[i].field_ptr);
	// 	wallet_ptr->tlsCAchain.ca[i].field_len = 0;
	// }
#endif

	wallet_ptr->node_info.host_name = NULL;
	wallet_ptr->node_info.node_url  = NULL;
	/* http2Context DeInit */
	http2DeInit(wallet_ptr->http2Context_ptr);

	/* wallet_ptr DeInit */
	BoatFree(wallet_ptr);

	/* set NULL after free completed */
	wallet_ptr = NULL;
}

BOAT_RESULT BoatHlChainmakerTxInit(const BoatHlchainmakerWallet* wallet_ptr,const BCHAR* chain_id, const BCHAR* org_id,
								   const BCHAR* contract_name, BoatHlchainmakerTx* tx_ptr)
{
	BUINT32 stringLen;
	BOAT_RESULT result = BOAT_SUCCESS;

	boat_try_declare;

	if ((tx_ptr == NULL) || (wallet_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments 'tx_ptr' or 'wallet_ptr' cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
    tx_ptr->tx_reponse.httpResLen = 0;

    tx_ptr->client_info.chain_id      = chain_id;
    tx_ptr->client_info.org_id        = org_id;
    tx_ptr->client_info.contract_name = contract_name;

    tx_ptr->wallet_ptr = (BoatHlchainmakerWallet *)wallet_ptr;
	stringLen = wallet_ptr->node_info.org_tls_ca_cert.length;
	memcpy(tx_ptr->wallet_ptr->node_info.org_tls_ca_cert.content, wallet_ptr->node_info.org_tls_ca_cert.content, stringLen);
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
	BoatHlchainmakerResponse *parsePtr      = NULL;
	Common__TxResponse *tx_reponse          = NULL;
	Common__ContractResult *contract_result = NULL;

	int i;
	boat_try_declare;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	result = hlchainmakerTransactionPacked(tx_ptr, method, tx_type);
	if (result != BOAT_SUCCESS)
	{
		return result;
	}

	tx_ptr->wallet_ptr->http2Context_ptr->nodeUrl = node_cfg.node_url;
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
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
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = node_cfg.org_tls_ca_cert.length + 1;
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr = BoatMalloc(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
	memset(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, 0x00, tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
	memcpy(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, node_cfg.org_tls_ca_cert.content, node_cfg.org_tls_ca_cert.length);
#endif
	
	tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &tx_ptr->tx_reponse;
	parsePtr = tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr;
	tx_ptr->wallet_ptr->http2Context_ptr->chainType = 2;

	result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
	tx_reponse = common__tx_response__unpack(NULL, parsePtr->httpResLen-5, parsePtr->http2Res+5);

	printf("htpp response start\n");

	for (i = 0; i < parsePtr->httpResLen-5; i++)
	{
		printf("%02x\n", (parsePtr->http2Res+5)[i]);
	}

	common__contract_result__free_unpacked(NULL, tx_reponse->contract_result);
	if (tx_reponse == NULL)
	{
		printf("tx_reponse.message NULL\n");
	}
	printf("tx_reponse.message =%s\n", tx_reponse->message);

	if(parsePtr->httpResLen != 0)
	{
		BoatFree(parsePtr->http2Res);
	}
	
	parsePtr->httpResLen = 0;
	if (tx_reponse != NULL) 
	{
		BoatLog(BOAT_LOG_NORMAL, "[http2] respond received.");
	}
	return result;
}

BOAT_RESULT BoatHlchainmaker(BoatHlchainmakerTx *tx_ptr, char* key_str, char* value_str)
{
	if ((tx_ptr == NULL) || (key_str == NULL) || ((value_str == NULL)))
	{
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	if ((tx_ptr->trans_para.n_parameters < 1) || (tx_ptr->trans_para.n_parameters > 10))
	{
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	tx_ptr->trans_para.n_parameters -= 1;
	tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].key   = key_str;
	tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].value = value_str;

	return BOAT_SUCCESS;
}

// BOAT_RESULT BoatHlchainmaker(BoatHlchainmakerTx *tx_ptr, char* key_str, char* value_str)
// {
// 	static bool first_malloc = true;
// 	if (first_malloc) 
// 	{
// 		tx_ptr->trans_para.parameters = BoatMalloc(tx_ptr->trans_para.n_parameters* sizeof(BoatKeyValuePair));
// 	}

// 	if ((tx_ptr->trans_para.parameters == NULL) || (tx_ptr->trans_para.n_parameters < 1))
// 	{	
// 		first_malloc = true;
// 		return BOAT_ERROR;
// 	}

// 	tx_ptr->trans_para.n_parameters -= 1;
	
// 	transaction_para.parameters[tx_ptr->trans_para.n_parameters].key   = key_str;
// 	transaction_para.parameters[tx_ptr->trans_para.n_parameters].value = value_str;
// 	if (tx_ptr->trans_para.n_parameters == 0)
// 	{
// 		first_malloc = true;
// 	}
// 	else
// 	{
// 		first_malloc = false;
// 	}
// 	return BOAT_SUCCESS;
// }


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
	TxType tx_type = TxType_QUERY_USER_CONTRACT;

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




							   
