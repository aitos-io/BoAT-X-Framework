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
#include <time.h>

const BCHAR *key1 = "time";
const BCHAR *key2 = "file_hash";
const BCHAR *key3 = "file_name";

#define TIME_LEN 80
BCHAR str_value[90] = {"file_"};
BCHAR time_buf[TIME_LEN]  = {0};
BCHAR file_hash[33] = {0};



void get_time_string(char* time_buf)
{
	time_t rawtime;
	struct tm *info;

	time(&rawtime);
	info = localtime(&rawtime);
	strftime(time_buf, TIME_LEN, "%Y-%m-%d %H:%M:%S", info);	
	return;
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

	printf("gooooooooooooo\n");
	/* prikey context assignment */
	if (prikeyCtx_config.prikey_content.field_ptr != NULL)
	{
		printf("gccccccccccccccc\n");
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


    wallet_ptr->node_info.org_Id    = config_ptr->node_cfg.org_Id;
	wallet_ptr->node_info.chain_Id  = config_ptr->node_cfg.chain_Id;
	wallet_ptr->node_info.host_name = config_ptr->node_cfg.host_name;
	wallet_ptr->node_info.node_url  = config_ptr->node_cfg.node_url;
	wallet_ptr->node_info.claim_contract_name = config_ptr->node_cfg.claim_contract_name;
	/* assignment */
	wallet_ptr->node_info.org_tls_ca_cert.length = config_ptr->node_cfg.org_tls_ca_cert.length;
	memset(wallet_ptr->node_info.org_tls_ca_cert.content, 0, BOAT_CHAINMAKER_CERT_MAX_LEN);
	memcpy(wallet_ptr->node_info.org_tls_ca_cert.content, config_ptr->node_cfg.org_tls_ca_cert.content, wallet_ptr->node_info.org_tls_ca_cert.length);

	/* account_info assignment */
	result = BoatHlchainmakerWalletSetUserClientInfo(wallet_ptr, config_ptr->user_prikey_config, config_ptr->user_cert_content);

	/* http2Context_ptr assignment */
	wallet_ptr->http2Context_ptr = http2Init();
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to set accountInfo|TlsUInfo|networkInfo.");
		return NULL;
	}

	printf("liuzhenhe1 = %s\n", wallet_ptr->node_info.org_Id);
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
	BOAT_RESULT result = BOAT_SUCCESS;

	boat_try_declare;

	if ((tx_ptr == NULL) || (wallet_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments 'tx_ptr' or 'wallet_ptr' cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
   
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
	BoatHlchainmakerResponse *parsePtr = NULL;
	Common__TxResponse *tx_reponse = NULL;

	boat_try_declare;

	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	//randrom generate
	BoatFieldMax16B  random_data;
	random_data.field_len = 16;
	BoatRandom(random_data.field, random_data.field_len, NULL);

	get_time_string(time_buf);
	array_to_str(random_data.field ,file_hash, 16);
	

	BoatTransactionPara transaction_para;
	transaction_para.n_parameters = 3;
	transaction_para.parameters = BoatMalloc(transaction_para.n_parameters * sizeof(BoatKeyValuePair));

	transaction_para.parameters[0].key   = key1;
	
	transaction_para.parameters[0].value = time_buf;

	transaction_para.parameters[1].key   = key2;
	transaction_para.parameters[1].value = file_hash;

	transaction_para.parameters[2].key   = key3;
	transaction_para.parameters[2].value = strcat(str_value, time_buf);

	int i = 0;

	for (i = 0; i < 3; i++)
	{
		printf("ghgggkhn = %s\n", transaction_para.parameters[i].key);
		printf("ghgggkhn = %s\n", transaction_para.parameters[i].value);
	}

	result = hlchainmakerTransactionPacked(tx_ptr, method, &transaction_para, tx_type);
	if (result != BOAT_SUCCESS)
	{
		return result;
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
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = node_cfg.org_tls_ca_cert.length + 1;
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr = BoatMalloc(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
	memset(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, 0x00, tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
	memcpy(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, node_cfg.org_tls_ca_cert.content, node_cfg.org_tls_ca_cert.length);

	tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &tx_ptr->tx_reponse;
	printf("BoatHlchainmakerTxExec 222222222222222\n");
	parsePtr = tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr;
	tx_ptr->wallet_ptr->http2Context_ptr->chainType = 2;
	result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);

	printf("BoatHlchainmakerTxExec 3333333333333333333\n");
	tx_reponse = common__tx_response__unpack(NULL, parsePtr->httpResLen, parsePtr->http2Res);
	if(parsePtr->httpResLen != 0)
	{
		BoatFree(parsePtr->http2Res);
	}
	
	parsePtr->httpResLen = 0;
	if (tx_reponse != NULL) 
	{
		BoatLog(BOAT_LOG_NORMAL, "[http2]endorser respond received.");
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
	result = BoatHlchainmakerTxExec(tx_ptr->wallet_ptr->node_info, tx_ptr,method, tx_type);
	if (result != BOAT_SUCCESS)
	{
		return BOAT_ERROR;
	}

	return result;
}




							   
