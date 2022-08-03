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

/*!@brief chainmaker wallet API for BoAT IoT SDK

@file
api_chainmaker.c defines the chainmaker wallet API for BoAT IoT SDK.
*/

/* self-header include */
#include "boatinternal.h"
#include "boatiotsdk.h"
#include "http2intf.h"
#include "common/result.pb-c.h"
#include "common/transaction.pb-c.h"
#include "boatchainmaker/boatchainmaker.h"

#define BOAT_RETRY_CNT 10
#define BOAT_TXID_LEN 64

static BOAT_RESULT BoatChainmakerWalletSetOrgId(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *org_id_ptr);
static BOAT_RESULT BoatChainmakerWalletSetChainId(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *chain_id_ptr);
static BOAT_RESULT BoatChainmakerWalletSetHostName(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *host_name_ptr);
static BOAT_RESULT BoatChainmakerWalletSetNodeUrl(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *node_url_ptr);



BUINT8 get_fibon_data(BUINT8 n) 
{
	if ((n == 1) || (n == 2) || (n == 0)) 
	{
		return 1;
	}
	else 
	{
		return get_fibon_data(n - 1) + get_fibon_data(n - 2);
	}
}

BOAT_RESULT array_to_str(BUINT8* array, BCHAR* str, char lenth)
{
    char value_up;
    char value_down;
    int i = 0;
    int n = 0;

    if ((array == NULL) || (str == NULL))
    {
    	return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    for (i = 0; i < lenth; i++)
    {
    	//up data 
        value_up = (array[i] & 0xf0) >> 4;
        if ((value_up >= 0) && (value_up <= 9)) 
        {
            str[n++] = value_up + 0x30;
        }
        else if ((value_up >= 0xA) && (value_up <= 0xF)) 
        {
            str[n++] = value_up + 0x57;
        }

        //low data
        value_down = array[i] & 0x0f;
        if ((value_down >= 0) && (value_down <= 9)) 
        {  
            str[n++] = value_down + 0x30;
        }
        else if ((value_down >= 0xA) && (value_down <= 0xF)) 
        {
            str[n++] = value_down + 0x57;
        }
    }
    return BOAT_SUCCESS;
}


BOAT_RESULT get_tx_id(BCHAR* tx_id_ptr)
{
	//32 byte randrom generate
	BoatFieldMax32B  random_data;
	random_data.field_len = 32;
	BOAT_RESULT result = BOAT_SUCCESS;

	result = BoatRandom(random_data.field, random_data.field_len, NULL);
	if (result != BOAT_SUCCESS) 
	{
		return BOAT_ERROR;
	}

	result = array_to_str(random_data.field, tx_id_ptr, random_data.field_len);
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
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* initialization */
	memset(&wallet_ptr->user_cert_prikey_info.prikeyCtx, 0, sizeof(BoatWalletPriKeyCtx));
	
	/* prikey context assignment */
	if (prikeyCtx_config.prikey_content.field_ptr != NULL) 
	{
		if (BOAT_SUCCESS != BoatPort_keyCreate(&prikeyCtx_config, &wallet_ptr->user_cert_prikey_info.prikeyCtx)) 
		{
			BoatLog(BOAT_LOG_CRITICAL, "Failed to exec BoatPort_keyCreate.");
			return BOAT_ERROR_WALLET_KEY_CREAT_FAIL;
		}
	}

	/* cert assignment */
	wallet_ptr->user_cert_prikey_info.cert.field_ptr = BoatMalloc(certContent.length);
	if (wallet_ptr->user_cert_prikey_info.cert.field_ptr == NULL) 
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatChainmakerWalletSetAccountInfo_exception);
	}

	if (certContent.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
	{
		BoatLog(BOAT_LOG_CRITICAL, "certContent length invalid");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatChainmakerWalletSetAccountInfo_exception);
	}
	memcpy(wallet_ptr->user_cert_prikey_info.cert.field_ptr, certContent.content, certContent.length);
	wallet_ptr->user_cert_prikey_info.cert.field_len = certContent.length;

	/* boat catch handle */
	boat_catch(BoatChainmakerWalletSetAccountInfo_exception) 
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		// /* free malloc param Deinit */
		if (wallet_ptr->user_cert_prikey_info.cert.field_ptr != NULL)
		{
			BoatFree(wallet_ptr->user_cert_prikey_info.cert.field_ptr);
			wallet_ptr->user_cert_prikey_info.cert.field_ptr = NULL;
		}
		
		wallet_ptr->user_cert_prikey_info.cert.field_len = 0;
	}
	return result;
}

BoatHlchainmakerWallet *BoatHlchainmakerWalletInit(const BoatHlchainmakerWalletConfig *config_ptr)
{
	BoatHlchainmakerWallet *wallet_ptr = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	if (config_ptr == NULL) 
	{
		BoatLog(BOAT_LOG_CRITICAL, "config_ptr cannot be NULL");
		return NULL;
	}

	/* allocate wallet memory */
	wallet_ptr = BoatMalloc(sizeof(BoatHlchainmakerWallet));
	if (wallet_ptr == NULL) 
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to malloc wallet memory.");
		return NULL;
	}

	/* initialization */
	wallet_ptr->user_cert_prikey_info.cert.field_ptr  = NULL;
	wallet_ptr->user_cert_prikey_info.cert.field_len  = 0;	
	wallet_ptr->tls_ca_cert_info.field_ptr     = NULL;
	wallet_ptr->tls_ca_cert_info.field_len     = 0;
	wallet_ptr->http2Context_ptr               = NULL;
	wallet_ptr->node_info.node_url_info        = NULL;
	wallet_ptr->node_info.host_name_info       = NULL;   
	wallet_ptr->node_info.chain_id_info        = NULL;
	wallet_ptr->node_info.org_id_info          = NULL;

	result = BoatChainmakerWalletSetNodeUrl(wallet_ptr, config_ptr->node_url_cfg);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerWalletSetNodeUrl failed");
		boat_throw(result, BoatHlchainmakerWalletInitException);
	}

	result = BoatChainmakerWalletSetHostName(wallet_ptr, config_ptr->host_name_cfg);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerWalletSetHostName failed");
		boat_throw(result, BoatHlchainmakerWalletInitException);
	}

	result = BoatChainmakerWalletSetChainId(wallet_ptr, config_ptr->chain_id_cfg);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerWalletSetChainId failed");
		boat_throw(result, BoatHlchainmakerWalletInitException);
	}

	result = BoatChainmakerWalletSetOrgId(wallet_ptr, config_ptr->org_id_cfg);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerWalletSetOrgId failed");
		boat_throw(result, BoatHlchainmakerWalletInitException);
	}

	/* assignment */
#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
	wallet_ptr->tls_ca_cert_info.field_len = config_ptr->tls_ca_cert_cfg.length;

	wallet_ptr->tls_ca_cert_info.field_ptr = BoatMalloc(wallet_ptr->tls_ca_cert_info.field_len);
	if (wallet_ptr->tls_ca_cert_info.field_ptr == NULL) 
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to malloc tls_ca_cert_info memory.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlchainmakerWalletInitException);
	}

	memcpy(wallet_ptr->tls_ca_cert_info.field_ptr, config_ptr->tls_ca_cert_cfg.content, wallet_ptr->tls_ca_cert_info.field_len);
#endif

	/* account_info assignment */
	result = BoatHlchainmakerWalletSetUserClientInfo(wallet_ptr, config_ptr->user_prikey_cfg, config_ptr->user_cert_cfg);
	if (result != BOAT_SUCCESS) 
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to set accountInfo|TlsUInfo|networkInfo.");
		boat_throw(result, BoatHlchainmakerWalletInitException);
	}

 	/* http2Context_ptr assignment */
 	wallet_ptr->http2Context_ptr = http2Init();

	boat_catch(BoatHlchainmakerWalletInitException)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        BoatHlchainmakerWalletDeInit(wallet_ptr);
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

	if (wallet_ptr->user_cert_prikey_info.cert.field_ptr != NULL) 
	{
		BoatFree(wallet_ptr->user_cert_prikey_info.cert.field_ptr);
		wallet_ptr->user_cert_prikey_info.cert.field_ptr = NULL;
	}

	
#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
	/* tlsClinet_info DeInit */
    if (wallet_ptr->tls_ca_cert_info.field_ptr!= NULL)
	{
		BoatFree(wallet_ptr->tls_ca_cert_info.field_ptr);
		wallet_ptr->tls_ca_cert_info.field_ptr = NULL;
	}
	wallet_ptr->tls_ca_cert_info.field_len = 0;
#endif

	if (wallet_ptr->node_info.node_url_info != NULL)
	{
		BoatFree(wallet_ptr->node_info.node_url_info);
		wallet_ptr->node_info.node_url_info  = NULL;
	}

	if (wallet_ptr->node_info.host_name_info != NULL)
	{
		BoatFree(wallet_ptr->node_info.host_name_info);
		wallet_ptr->node_info.host_name_info = NULL;
	}

	if (wallet_ptr->node_info.chain_id_info != NULL)
	{
		BoatFree(wallet_ptr->node_info.chain_id_info);
		wallet_ptr->node_info.chain_id_info  = NULL;
	}

	if (wallet_ptr->node_info.org_id_info!= NULL)
	{
		BoatFree(wallet_ptr->node_info.org_id_info);
		wallet_ptr->node_info.org_id_info = NULL;
	}

	/* http2Context DeInit */
	http2DeInit(wallet_ptr->http2Context_ptr);

	/* wallet_ptr DeInit */
	if (wallet_ptr != NULL)
	{
		BoatFree(wallet_ptr);
		wallet_ptr = NULL;
	}

}

BOAT_RESULT BoatHlChainmakerTxInit(const BoatHlchainmakerWallet* wallet_ptr, BoatHlchainmakerTx* tx_ptr)
{
	BOAT_RESULT result = BOAT_SUCCESS;

	if ((tx_ptr == NULL) || (wallet_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlChainmakerTxInit paramters cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	tx_ptr->wallet_ptr = (BoatHlchainmakerWallet *)wallet_ptr;
	return result;
}

__BOATSTATIC BOAT_RESULT BoatHlchainmakerTxRequest(BoatHlchainmakerTx *tx_ptr, Common__TxResponse** tx_response)
{
	BOAT_RESULT result                  = BOAT_SUCCESS;
	Common__TxResponse* tx_response_ptr = NULL;
	Http2Response http2_response        = {0,NULL};

	if (tx_ptr == NULL) 
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	tx_ptr->wallet_ptr->http2Context_ptr->nodeUrl = tx_ptr->wallet_ptr->node_info.node_url_info;

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
	tx_ptr->wallet_ptr->http2Context_ptr->hostName                = tx_ptr->wallet_ptr->node_info.host_name_info;
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain              = BoatMalloc(sizeof(BoatFieldVariable));
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = tx_ptr->wallet_ptr->tls_ca_cert_info.field_len + 1;
	tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr = BoatMalloc(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
	memset(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, 0x00, tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len);
	memcpy(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr, tx_ptr->wallet_ptr->tls_ca_cert_info.field_ptr, tx_ptr->wallet_ptr->tls_ca_cert_info.field_len);
#endif
	tx_ptr->wallet_ptr->http2Context_ptr->pathTmp      = "/api.RpcNode/SendRequest";
	tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &http2_response;

	result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
	if (result == BOAT_SUCCESS)
	{
		tx_response_ptr = common__tx_response__unpack(NULL, http2_response.httpResLen - 5, http2_response.http2Res + 5);
		if (tx_response_ptr != NULL)
		{
			BoatLog(BOAT_LOG_NORMAL, "[http2] respond received.");
			*tx_response = tx_response_ptr;
		}
		else
		{
			BoatLog(BOAT_LOG_NORMAL, "[http2] respond NULL");
			*tx_response = NULL;
			result = BOAT_ERROR;
		}
	}

	if (http2_response.http2Res != NULL) 
	{
		BoatFree(http2_response.http2Res);
		http2_response.http2Res = NULL;
		http2_response.httpResLen = 0;
	}

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)		
	// clear last data
	if (tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain != NULL) 
	{
		if (tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr != NULL) 
		{
			BoatFree(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr);
			tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_ptr = NULL;
		}
		tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain[0].field_len = 0;
		BoatFree(tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain);
		tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain = NULL;
	}
#endif
	return result;

}

BOAT_RESULT BoatHlchainmakerAddTxParam(BoatHlchainmakerTx *tx_ptr, BUINT8 length, const BCHAR *arg, ...)						  
{
	va_list ap;
	
	BCHAR *args        = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;

	if ((tx_ptr == NULL) || (length > BOAT_HLCHAINMAKER_ARGS_MAX_NUM) || ((length & 0x01) == 1))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	tx_ptr->trans_para.n_parameters = 0;
	va_start(ap, arg);
	if (arg != NULL)
	{
		tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].key = (BCHAR *)arg;
	}

	for (int i = 1; i < length; i++)
	{
		args = va_arg(ap, BCHAR*);
		if (args == NULL)
		{
			BoatLog(BOAT_LOG_CRITICAL, "variable parameter cannot be NULL.");
			return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
		}
		if (i & 0x01) 
		{
			tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].value = (BCHAR *)args;
			tx_ptr->trans_para.n_parameters++;
		}
		else 
		{
			tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].key = (BCHAR *)args;
		}
	}

	va_end(ap);
	return result;
}
		

BOAT_RESULT BoatHlchainmakerContractInvoke(BoatHlchainmakerTx *tx_ptr, char* method, char* contract_name, bool sync_result, BoatInvokeResponse *invoke_response)
{
	Common__TxResponse *tx_response              = NULL;
	Common__TransactionInfo* transactation_info = NULL;
	BCHAR* invoke_tx_id                        = NULL;
	BUINT8 sleep_second;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	TxType tx_type = TXTYPE_INVOKE_USER_CONTRACT; 
	if ((tx_ptr == NULL) || (method == NULL) || (contract_name == NULL) || (invoke_response == NULL))
    {
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, BoatHlchainmakerContractInvoke);
	}

	invoke_tx_id = BoatMalloc(BOAT_TXID_LEN + 1);
	memset(invoke_tx_id,0x00,BOAT_TXID_LEN + 1);
	result = get_tx_id(invoke_tx_id);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "get_tx_id failed");
		boat_throw(result, BoatHlchainmakerContractInvoke);
	}

	result = hlchainmakerTransactionPacked(tx_ptr, method, contract_name, tx_type, invoke_tx_id);
	if (result != BOAT_SUCCESS) 
    {
		BoatLog(BOAT_LOG_CRITICAL, "hlchainmakerTransactionPacked failed");
		boat_throw(result, BoatHlchainmakerContractInvoke);
	}

	result = BoatHlchainmakerTxRequest(tx_ptr, &tx_response);
	if (result != BOAT_SUCCESS) 
    {
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerTxRequest failed");
		boat_throw(result, BoatHlchainmakerContractInvoke);
	}

	invoke_response->gas_used = 0;
     invoke_response->code  = tx_response->code;
	if (strlen(tx_response->message) < BOAT_RESPONSE_MESSAGE_MAX_LEN) 
    {
		memset(invoke_response->message, 0, BOAT_RESPONSE_MESSAGE_MAX_LEN);
		memcpy(invoke_response->message, tx_response->message, strlen(tx_response->message));
	}

	if (tx_response != NULL)
	{
		common__tx_response__free_unpacked(tx_response, NULL);
	}

	if (sync_result) 
    {
		BoatHlchainmakerAddTxParam(tx_ptr, 2, "txId",invoke_tx_id);
		result = hlchainmakerTransactionPacked(tx_ptr, "GET_TX_BY_TX_ID", "SYSTEM_CONTRACT_QUERY", TxType_QUERY_SYSTEM_CONTRACT, invoke_tx_id);

		if (result == BOAT_SUCCESS) 
        {
			for (int i = 0; i < BOAT_RETRY_CNT; i++) 
			{
				sleep_second = get_fibon_data(i + 1);
				BoatSleep(sleep_second);

				result = BoatHlchainmakerTxRequest(tx_ptr, &tx_response);
				if (result != BOAT_SUCCESS) 
				{
					BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerTxRequest sync failed");
					boat_throw(result, BoatHlchainmakerContractInvoke);
				}

				if (tx_response->code == SUCCESS) 
                {
					transactation_info = common__transaction_info__unpack(NULL, tx_response->contract_result->result.len, tx_response->contract_result->result.data);
					invoke_response->gas_used = transactation_info->transaction->result->contract_result->gas_used;
					if (tx_response != NULL)
					{
						common__tx_response__free_unpacked(tx_response, NULL);
						tx_response = NULL;
					}
					if (transactation_info != NULL)
					{
						common__transaction_info__free_unpacked(transactation_info, NULL);
						transactation_info = NULL;
					}
			
					break;
				}
			}
		}
	}	

	/* boat catch handle */
	boat_catch(BoatHlchainmakerContractInvoke)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

	if (invoke_tx_id != NULL) 
	{
		BoatFree(invoke_tx_id);
		invoke_tx_id = NULL;
	}
	
	return result;
}							   

BOAT_RESULT BoatHlchainmakerContractQuery(BoatHlchainmakerTx *tx_ptr, char* method, char* contract_name, BoatQueryResponse *query_response)
{
	TxType tx_type                  = TXTYPE_QUERY_USER_CONTRACT;
	Common__TxResponse *tx_response = NULL;
	BCHAR* query_tx_id             = NULL;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	if ((tx_ptr == NULL) || (method == NULL) || (contract_name == NULL) || (query_response == NULL))  
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, BoatHlchainmakerContractQuery_exception);
	}
	BoatLog(BOAT_LOG_NORMAL, "Submit will execute...");
	query_tx_id = BoatMalloc(BOAT_TXID_LEN + 1);
	get_tx_id(query_tx_id);

	result = hlchainmakerTransactionPacked(tx_ptr, method, contract_name, tx_type, query_tx_id);
	if (result != BOAT_SUCCESS) 
    {
		BoatLog(BOAT_LOG_CRITICAL, "hlchainmakerTransactionPacked failed");
		boat_throw(result, BoatHlchainmakerContractQuery_exception);
	}
	
	result = BoatHlchainmakerTxRequest(tx_ptr, &tx_response);
	if (result != BOAT_SUCCESS) 
    {
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerTxRequest failed");
		boat_throw(result, BoatHlchainmakerContractQuery_exception);
	}

	if (tx_response == NULL) 
    {
		BoatLog(BOAT_LOG_CRITICAL, "tx_response is NULL");
		boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, BoatHlchainmakerContractQuery_exception);
	}

	query_response->gas_used = 0;
	query_response->code     = tx_response->code;
	memset(query_response->message, 0, BOAT_RESPONSE_MESSAGE_MAX_LEN);
	memset(query_response->contract_result, 0, BOAT_RESPONSE_CONTRACT_RESULT_MAX_LEN);

	if (query_response->code == BOAT_SUCCESS)
	{
		if (strlen(tx_response->message) > BOAT_RESPONSE_MESSAGE_MAX_LEN) 
		{
			BoatLog(BOAT_LOG_CRITICAL, "tx_response->message is too long");
			boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlchainmakerContractQuery_exception);
		}

		memcpy(query_response->message, tx_response->message, strlen(tx_response->message));
	
		if (tx_response->contract_result->code == BOAT_SUCCESS)
		{
			if (tx_response->contract_result->result.len != 0)
			{
				if (tx_response->contract_result->result.len > BOAT_RESPONSE_CONTRACT_RESULT_MAX_LEN) 
				{
					BoatLog(BOAT_LOG_CRITICAL, "tx_response->contract_result->result.datais too long");
					boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlchainmakerContractQuery_exception);
				}
				memcpy(query_response->contract_result, tx_response->contract_result->result.data, tx_response->contract_result->result.len);
			}
			query_response->gas_used = tx_response->contract_result->gas_used;
		}
	}

	/* boat catch handle */
	boat_catch(BoatHlchainmakerContractQuery_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

	if (query_tx_id != NULL)
	{
		BoatFree(query_tx_id);
		query_tx_id = NULL;
	}

	if (tx_response != NULL)
	{
		common__tx_response__free_unpacked(tx_response, NULL);
	}
	return result;
}	


BOAT_RESULT BoatChainmakerWalletSetNodeUrl(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *node_url_ptr)
{
    BOAT_RESULT result;
    
	if ((wallet_ptr == NULL) || (node_url_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr or node_url_ptr cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	if (strchr(node_url_ptr, ':') == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "node URL has a invalid format: %s.", node_url_ptr);
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// string length check
	if (BOAT_SUCCESS != UtilityStringLenCheck(node_url_ptr))
	{
		BoatLog(BOAT_LOG_CRITICAL, "node URL length out of limit: %s.", node_url_ptr);
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// Set Node URL

	if (wallet_ptr->node_info.node_url_info != NULL)
	{
		BoatFree(wallet_ptr->node_info.node_url_info);
		wallet_ptr->node_info.node_url_info = NULL;
	}

	// +1 for NULL Terminator
	wallet_ptr->node_info.node_url_info = BoatMalloc(strlen(node_url_ptr) + 1);
	if (wallet_ptr->node_info.node_url_info == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate memory for Node URL string.");
	 	result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	else
	{
		strcpy(wallet_ptr->node_info.node_url_info, node_url_ptr);
		result = BOAT_SUCCESS;
	}

    return result;
}


BOAT_RESULT BoatChainmakerWalletSetHostName(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *host_name_ptr)
{
    BOAT_RESULT result;
    
	if ((wallet_ptr == NULL) || (host_name_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr or host_name_ptr cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// string length check
	if (BOAT_SUCCESS != UtilityStringLenCheck(host_name_ptr))
	{
		BoatLog(BOAT_LOG_CRITICAL, "host name length out of limit: %s.", host_name_ptr);
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// Set Node URL

	if (wallet_ptr->node_info.host_name_info != NULL)
	{
		BoatFree(wallet_ptr->node_info.host_name_info);
		wallet_ptr->node_info.host_name_info = NULL;
	}

	// +1 for NULL Terminator
	wallet_ptr->node_info.host_name_info = BoatMalloc(strlen(host_name_ptr) + 1);
	if (wallet_ptr->node_info.host_name_info == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate memory for Host name string.");
	 	result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	else
	{
		strcpy(wallet_ptr->node_info.host_name_info, host_name_ptr);
		result = BOAT_SUCCESS;
	}

    return result;
}

BOAT_RESULT BoatChainmakerWalletSetChainId(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *chain_id_ptr)
{
    BOAT_RESULT result;
    
	if ((wallet_ptr == NULL) || (chain_id_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr or chain_id_ptr cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// string length check
	if (BOAT_SUCCESS != UtilityStringLenCheck(chain_id_ptr))
	{
		BoatLog(BOAT_LOG_CRITICAL, "chain id length out of limit: %s.", chain_id_ptr);
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// Set Node URL

	if (wallet_ptr->node_info.chain_id_info != NULL)
	{
		BoatFree(wallet_ptr->node_info.chain_id_info);
		wallet_ptr->node_info.chain_id_info = NULL;
	}

	// +1 for NULL Terminator
	wallet_ptr->node_info.chain_id_info = BoatMalloc(strlen(chain_id_ptr) + 1);
	if (wallet_ptr->node_info.chain_id_info == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate memory for chain id string.");
	 	result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	else
	{
		strcpy(wallet_ptr->node_info.chain_id_info, chain_id_ptr);
		result = BOAT_SUCCESS;
	}

    return result;
}

BOAT_RESULT BoatChainmakerWalletSetOrgId(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *org_id_ptr)
{
    BOAT_RESULT result;
    
	if ((wallet_ptr == NULL) || (org_id_ptr == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr or org_id_ptr cannot be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// string length check
	if (BOAT_SUCCESS != UtilityStringLenCheck(org_id_ptr))
	{
		BoatLog(BOAT_LOG_CRITICAL, "org id length out of limit: %s.", org_id_ptr);
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// Set Node URL

	if (wallet_ptr->node_info.org_id_info != NULL)
	{
		BoatFree(wallet_ptr->node_info.org_id_info);
		wallet_ptr->node_info.org_id_info = NULL;
	}

	// +1 for NULL Terminator
	wallet_ptr->node_info.org_id_info = BoatMalloc(strlen(org_id_ptr) + 1);
	if (wallet_ptr->node_info.org_id_info == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate memory for org id string.");
	 	result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	else
	{
		strcpy(wallet_ptr->node_info.org_id_info, org_id_ptr);
		result = BOAT_SUCCESS;
	}

    return result;
}