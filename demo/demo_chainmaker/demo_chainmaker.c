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

#include "boatconfig.h"
#include "boatiotsdk.h"
#include "boatlog.h"

const BCHAR* chainmaker_user_key =  "------BEGIN EC PRIVATE KEY-----\n"
									"MHcCAQEEIObTIWiOPC014eokTlChY2zqolJvkjPsObydiDTBAg8boAoGCCqGSM49\n"
									"AwEHoUQDQgAETvjoYFEqRN/PediSM3Op6rTOWORSuSiv/hyoSqBfb5Os0+aNYfjM\n"
									"AylgQ8ug/EcXrnpCqijjwbeR4nFzoZf4RA==\n"
									"-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_user_cert =	"-----BEGIN CERTIFICATE-----\n"
"MIICijCCAi+gAwIBAgIDDlJ2MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMDQyMDAzMTUxOFoXDTI3\n"
"MDQxOTAzMTUxOFowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
"MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
"LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAETvjoYFEq\n"
"RN/PediSM3Op6rTOWORSuSiv/hyoSqBfb5Os0+aNYfjMAylgQ8ug/EcXrnpCqijj\n"
"wbeR4nFzoZf4RKN7MHkwDgYDVR0PAQH/BAQDAgGmMA8GA1UdJQQIMAYGBFUdJQAw\n"
"KQYDVR0OBCIEIAFwt17+e6KmNgVopl6ED5nNJwpZQ02fXNZ7bDZdfyBuMCsGA1Ud\n"
"IwQkMCKAIAmbbnImfl7WRFdfnVcN75avqpER441lOyLRE59F2GBGMAoGCCqGSM49\n"
"BAMCA0kAMEYCIQCu431XukTkg/pO91/hehPMmt/Z+ZuTwElw2ahvCTI7lwIhAO7A\n"
"i6DOlUowKa2AKpTULwWHED+9W/WPAMe6eCYXKKWx\n"
"-----END CERTIFICATE-----\n";



const BCHAR* chainmaker_tls_ca_cert =   "-----BEGIN CERTIFICATE-----\n"
										"MIICsDCCAlWgAwIBAgIDCVBkMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
										"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
										"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
										"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMDQyMDAzMTUxOFoXDTMy\n"
										"MDQxNzAzMTUxOFowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
										"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
										"MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
										"YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQ3IktxZ7XhTCaYt54a\n"
										"01oXyPoDwHPd1c6aqUyuu8QhAprPKkiOjeQ02nVaQ1/M4fgdHADdLgAxeU5iifAD\n"
										"pw04o4GnMIGkMA4GA1UdDwEB/wQEAwIBpjAPBgNVHSUECDAGBgRVHSUAMA8GA1Ud\n"
										"EwEB/wQFMAMBAf8wKQYDVR0OBCIEIAmbbnImfl7WRFdfnVcN75avqpER441lOyLR\n"
										"E59F2GBGMEUGA1UdEQQ+MDyCDmNoYWlubWFrZXIub3Jngglsb2NhbGhvc3SCGWNh\n"
										"Lnd4LW9yZzEuY2hhaW5tYWtlci5vcmeHBH8AAAEwCgYIKoZIzj0EAwIDSQAwRgIh\n"
										"AMx1WVoT0vZQEuc+383n0vwB/jXUYQhdNUzpsI8o0RUsAiEAl51FmBKPZQBoURz/\n"
										"Da0VTpq1XNH7wUU22CMqG+KkU+A=\n"
										"-----END CERTIFICATE-----\n";

//wallet para
BCHAR *chainmaker_node_url   = "127.0.0.1:12301";
BCHAR *chainmaker_host_name  = "chainmaker.org";
BCHAR *chainmaker_chain_id   = "chain1";
BCHAR *chainmaker_org_id     = "wx-org1.chainmaker.org";

BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
BoatHlchainmakerWalletConfig wallet_config = {0};

__BOATSTATIC BOAT_RESULT chainmakerWalletPrepare(void)
{
	BOAT_RESULT index;

	//set user private key context
	wallet_config.user_prikey_cfg.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.user_prikey_cfg.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.user_prikey_cfg.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.user_prikey_cfg.prikey_content.field_ptr = (BUINT8 *)chainmaker_user_key;
	wallet_config.user_prikey_cfg.prikey_content.field_len = strlen(chainmaker_user_key) + 1; 

	//set user cert context
	wallet_config.user_cert_cfg.length = strlen(chainmaker_user_cert) + 1;
	if (wallet_config.user_cert_cfg.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
	{
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	memcpy(wallet_config.user_cert_cfg.content, chainmaker_user_cert, wallet_config.user_cert_cfg.length);
	
	//set url and name
	if ((strlen(chainmaker_node_url) >= BAOT_CHAINMAKER_NODE_STR_LEN) || 
		 (strlen(chainmaker_host_name) >= BAOT_CHAINMAKER_NODE_STR_LEN)||
		 (strlen(chainmaker_chain_id) >= BAOT_CHAINMAKER_NODE_STR_LEN)||
		 (strlen(chainmaker_org_id) >= BAOT_CHAINMAKER_NODE_STR_LEN))
	{
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT ;
	}
	strncpy(wallet_config.node_url_cfg,  chainmaker_node_url,  strlen(chainmaker_node_url));
	strncpy(wallet_config.host_name_cfg, chainmaker_host_name, strlen(chainmaker_host_name));
	strncpy(wallet_config.chain_id_cfg,  chainmaker_chain_id,  strlen(chainmaker_chain_id));
	strncpy(wallet_config.org_id_cfg,    chainmaker_org_id,    strlen(chainmaker_org_id));

	//tls ca cert
	wallet_config.tls_ca_cert_cfg.length = strlen(chainmaker_tls_ca_cert) + 1;
	if (wallet_config.tls_ca_cert_cfg.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
	{
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	memcpy(wallet_config.tls_ca_cert_cfg.content, chainmaker_tls_ca_cert, wallet_config.tls_ca_cert_cfg.length);

	// create wallet
#if defined(USE_ONETIME_WALLET)
	index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
#elif defined(USE_CREATE_PERSIST_WALLET)
	index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker.cfg", &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
#elif defined(USE_LOAD_PERSIST_WALLET)
	index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker.cfg", NULL, sizeof(BoatHlchainmakerWalletConfig));
#else
	return BOAT_ERROR;
#endif
	if (index < 0)
	{
		return BOAT_ERROR_WALLET_CREATE_FAIL;
	}
	
	g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(index);
	if (g_chaninmaker_wallet_ptr == NULL)
	{
		return BOAT_ERROR;
	}

	return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT           result  = BOAT_SUCCESS;
	BoatHlchainmakerTx    tx_ptr;
	BoatInvokeResponse     invoke_response;
	BoatQueryResponse      query_response;
	boat_try_declare;

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();

	/* step-2: prepare wallet */
	result = chainmakerWalletPrepare();

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "chainmakerWalletPrepare failed.");
		boat_throw(result, chainmaker_demo_catch);;
	}

	/* step-3: Chainmaker transaction structure initialization */
	result = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, &tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlChainmakerTxInit failed.");
		boat_throw(result, chainmaker_demo_catch);
	}

	/* step-4: add invoke parameters */
	result = BoatHlchainmakerAddTxParam(&tx_ptr, 6, "time", "6543235", "file_hash", "ab3456df5799b87c77e7f85", "file_name", "name005", NULL);
   	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerAddTxParam() failed.");
		boat_throw(result, chainmaker_demo_catch);
	}

	/* step-5: execute invoke command */
	result = BoatHlchainmakerContractInvoke(&tx_ptr, "save","fact", false, &invoke_response); 
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerContractInvoke() failed.");
		boat_throw(result, chainmaker_demo_catch);
	}
	BoatLog( BOAT_LOG_CRITICAL, "response code = %d, message = %s, gas_used = %d\n", invoke_response.code, invoke_response.message, invoke_response.gas_used);

	/* step-6: add query parameters*/
	BoatSleep(2);
	result = BoatHlchainmakerAddTxParam(&tx_ptr, 2, "file_hash", "ab3456df5799b87c77e7f85", NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerAddTxParam() failed.");
		boat_throw(result, chainmaker_demo_catch);
	}

	/* step-7: execute query command*/
	result = BoatHlchainmakerContractQuery(&tx_ptr, "find_by_file_hash","fact", &query_response);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerContractQuery() failed.");
		boat_throw(result, chainmaker_demo_catch);
	}
	BoatLog(BOAT_LOG_CRITICAL, "response code = %d,  message = %s,  contract_result = %s, gas_used = %d\n", 
			query_response.code, query_response.message, query_response.contract_result, query_response.gas_used);

	boat_catch(chainmaker_demo_catch)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
    }	

	/* step-6: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}

