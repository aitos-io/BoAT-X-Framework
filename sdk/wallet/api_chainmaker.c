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


//set signClient prikey and cert
BOAT_RESULT BoatChainmakerWalletSetSignClientInfo(BoatChainmakerWallet *wallet_ptr,
											 const BoatWalletPriKeyCtx_config prikeyCtx_config,
											 const BoatChainmakerCertInfoCfg certContent)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	if (wallet_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	/* initialization */
	memset(&wallet_ptr->signClient_info.prikeyCtx, 0, sizeof(BoatWalletPriKeyCtx));
	wallet_ptr->signClient_info.cert.field_ptr = NULL;
	wallet_ptr->signClient_info.cert.field_len = 0;

	/* prikey context assignment */
	if (prikeyCtx_config.prikey_content.field_ptr != NULL)
	{
		if (BOAT_SUCCESS != BoatPort_keyCreate(&prikeyCtx_config, &wallet_ptr->signClient_info.prikeyCtx))
		{
			BoatLog(BOAT_LOG_CRITICAL, "Failed to exec BoatPort_keyCreate.");
			return BOAT_ERROR_INVALID_ARGUMENT;
		}
	}

	/* cert assignment */
	wallet_ptr->signClient_info.cert.field_ptr = BoatMalloc(certContent.length);
	if (wallet_ptr->signClient_info.cert.field_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHChainmakerWalletSetAccountInfo_exception);
	}
	memcpy(wallet_ptr->signClient_info.cert.field_ptr, certContent.content, certContent.length);
	wallet_ptr->signClient_info.cert.field_len = certContent.length;

	/* boat catch handle */
	boat_catch(BoatHChainmakerWalletSetAccountInfo_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		/* free malloc param Deinit */
		BoatFree(wallet_ptr->signClient_info.cert.field_ptr);
		wallet_ptr->signClient_info.cert.field_len = 0;
	}

	return result;
}


BOAT_RESULT BoatChainmakerWalletSetRootCaInfo(BoatChainmakerWallet *wallet_ptr,
											const BoatChainmakerCertInfoCfg *rootCaContent,
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
	for (i = 0; i < BOAT_HLFABRIC_ROOTCA_MAX_NUM; i++)
	{
		wallet_ptr->tlsRootCA_info.ca[i].field_len = 0;
		wallet_ptr->tlsRootCA_info.ca[i].field_ptr = NULL;
	}

	/* assignment */
	for (i = 0; i < rootCaNumber; i++)
	{
		/* get rootCA file size */
		wallet_ptr->tlsRootCA_info.ca[i].field_len = (rootCaContent + i)->length;

		wallet_ptr->tlsRootCA_info.ca[i].field_ptr = BoatMalloc(wallet_ptr->tlsRootCA_info.ca[i].field_len);
		if (wallet_ptr->tlsRootCA_info.ca[i].field_ptr == NULL)
		{
			BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
			boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatChainmakerWalletSetRootCaInfo_exception);
		}
		memset(wallet_ptr->tlsRootCA_info.ca[i].field_ptr, 0, wallet_ptr->tlsRootCA_info.ca[i].field_len);
		memcpy(wallet_ptr->tlsRootCA_info.ca[i].field_ptr, (rootCaContent + i)->content, wallet_ptr->tlsRootCA_info.ca[i].field_len);
	}

	/* boat catch handle */
	boat_catch(BoatChainmakerWalletSetRootCaInfo_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		/* free malloc param Deinit */
		for (i = 0; i < rootCaNumber; i++)
		{
			if (wallet_ptr->tlsRootCA_info.ca[i].field_ptr != NULL)
			{
				BoatFree(wallet_ptr->tlsRootCA_info.ca[i].field_ptr);
				wallet_ptr->tlsRootCA_info.ca[i].field_len = 0;
			}
		}
	}

	return result;
}


BoatChainmakerWallet *BoatChainmakerWalletInit(const BoatChainmakerWalletConfig *config_ptr,
										   BUINT32 config_size)
{
	BoatChainmakerWallet *wallet_ptr = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT8 i = 0;

	if ((config_ptr == NULL) || (config_size == 0))
	{
		BoatLog(BOAT_LOG_CRITICAL, "config_ptr cannot be NULL or config_size cannot be zero.");
		return NULL;
	}

	/* allocate wallet memory */
	wallet_ptr = BoatMalloc(sizeof(BoatChainmakerWallet));
	if (wallet_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to malloc wallet memory.");
		return NULL;
	}
	if (sizeof(BoatChainmakerWalletConfig) != config_size)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Incorrect configuration size.");
		return NULL;
	}

	/* initialization */
	wallet_ptr->signClient_info.cert.field_ptr = NULL;
	wallet_ptr->signClient_info.cert.field_len = 0;

	for (i = 0; i < BOAT_CHAINMAKER_ROOTCA_MAX_NUM; i++)
	{
		wallet_ptr->tlsRootCA_info.ca[i].field_len = 0;
		wallet_ptr->tlsRootCA_info.ca[i].field_ptr = NULL;
	}

	wallet_ptr->http2Context_ptr = NULL;

	/* account_info assignment */
	result += BoatHlfabricWalletSetAccountInfo(wallet_ptr, config_ptr->user_pri_key_config,
											   config_ptr->user_cert_content);

	/* tlsRootCa_info assignment */
	BoatHlfabricWalletSetRootCaInfo(wallet_ptr, config_ptr->root_ca_cert, config_ptr->rootCaNumber);

	/* http2Context_ptr assignment */
	wallet_ptr->http2Context_ptr = http2Init();

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to set accountInfo|TlsUInfo|networkInfo.");
		return NULL;
	}

	return wallet_ptr;
}

											

void BoatCHainmakerTxRequestDeInit(BoatChainmkaerTxRequest *tx_ptr)
{
	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Tx argument cannot be nULL.");
		return;
	}
	
	BoatFree();
}


//cacl hash
BOAT_RESULT BoaTChainmakerCalcUnsignedTxRequestBytes(BoatChainmkaerTxRequest *tx_ptr) {
	BOAT_RESULT result = BOAT_SUCCESS;
	return result;	
}

//sign 
BOAT_RESULT BoaTChainmakerSignTx(char *privateKey, char*cert, char* hash_data)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	return result;
}


BOAT_RESULT BoatChainmakerGenerateTxRequest(BoatChainmkaerTxRequest *tx_ptr, BUINT32             txType, BoatChainmakerTransactPayload* payload)						 
{
	BOAT_RESULT result = BOAT_SUCCESS;

	return result;
}

BOAT_RESULT BoatChainmakerTxSetTimestamp(BUINT64 *time_stamp)
{
	if (time_stamp == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

#if defined(__unix__) || defined(__unix) || defined(unix)
	struct timespec txTimestamp;
	clock_gettime(CLOCK_REALTIME, &txTimestamp);
	*time_stamp = txTimestamp.tv_sec;
	
#else
	long int timesec = 0;
	time(&timesec);
	*time_stamp = timesec;
#endif

	return BOAT_SUCCESS;
}




