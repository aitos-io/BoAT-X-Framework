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

const BCHAR* chainmaker_user_key =  "-----BEGIN EC PRIVATE KEY-----\n"
									"MHcCAQEEIKra8nWWBSPS/AejMaW+yonu8d9dBoj+SPWDrJbmIh1HoAoGCCqGSM49\n"
									"AwEHoUQDQgAEMc5rXZAi7DJkvOdGAlEw/kOsLIhb6L5DXwSYltVYAndlKKubd7HO\n"
									"MrZBWP5+xAw/3eB6A8aBwqOWdx9Hv4vX+A==\n"
									"-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_user_cert =	"-----BEGIN CERTIFICATE-----\n"
									"MIICijCCAi+gAwIBAgIDDgl6MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
									"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
									"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
									"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIxMTIyODA4MDczNVoXDTI2\n"
									"MTIyNzA4MDczNVowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
									"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
									"MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
									"LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEMc5rXZAi\n"
									"7DJkvOdGAlEw/kOsLIhb6L5DXwSYltVYAndlKKubd7HOMrZBWP5+xAw/3eB6A8aB\n"
									"wqOWdx9Hv4vX+KN7MHkwDgYDVR0PAQH/BAQDAgGmMA8GA1UdJQQIMAYGBFUdJQAw\n"
									"KQYDVR0OBCIEIOaCMaVI//RuFz8f8kZkjB6MeBP/E7WtWF8S7TcpT0MEMCsGA1Ud\n"
									"IwQkMCKAIE2++CJNZObR1Gih2bf+ZpgA1sm5bvtr5G0mE1Ur2vaLMAoGCCqGSM49\n"
									"BAMCA0kAMEYCIQDTssD55Pt4BjkuSsJ13dqzIkeq6BQ9jT/FUeUxduUpdAIhAOOn\n"
									"20LbYVxt8FDAUYwToWUdJ6/AHmuB3flrsEYVzf9y\n"
									"-----END CERTIFICATE-----\n";


const BCHAR* chainmaker_tls_ca_cert =   "-----BEGIN CERTIFICATE-----\n"
										"MIICrzCCAlWgAwIBAgIDDokOMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
										"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
										"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
										"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIxMTIyODA4MDczNVoXDTMx\n"
										"MTIyNjA4MDczNVowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
										"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
										"MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
										"YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQGpOQlJ5dh0sLHX/tE\n"
										"k6PN3rEKUGCf00u4qsH/uRdQZbfIh0YGSKQWBegrCUS01RxpLlWgxfZAtUIOI4fi\n"
										"t16Po4GnMIGkMA4GA1UdDwEB/wQEAwIBpjAPBgNVHSUECDAGBgRVHSUAMA8GA1Ud\n"
										"EwEB/wQFMAMBAf8wKQYDVR0OBCIEIE2++CJNZObR1Gih2bf+ZpgA1sm5bvtr5G0m\n"
										"E1Ur2vaLMEUGA1UdEQQ+MDyCDmNoYWlubWFrZXIub3Jngglsb2NhbGhvc3SCGWNh\n"
										"Lnd4LW9yZzEuY2hhaW5tYWtlci5vcmeHBH8AAAEwCgYIKoZIzj0EAwIDSAAwRQIg\n"
										"CpL7BZ88AWei7Kx20u+U3QYiYfRugaVqz3FSVsKA/7wCIQC3VxwMdrsEKJexNGSL\n"
										"sNHNihrTET6DD5/3pPSpf9HL7g==\n"
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
	wallet_config.user_cert_cfg.length = strlen(chainmaker_user_cert);
	memcpy(wallet_config.user_cert_cfg.content, chainmaker_user_cert, wallet_config.user_cert_cfg.length);
	
	//set url and name
	if ((strlen(chainmaker_node_url) > BAOT_CHAINMAKER_URL_HOSTNAME_LEN) || 
		 (strlen(chainmaker_host_name) > BAOT_CHAINMAKER_URL_HOSTNAME_LEN)||
		 (strlen(chainmaker_chain_id) > BAOT_CHAINMAKER_URL_HOSTNAME_LEN)||
		 (strlen(chainmaker_org_id) > BAOT_CHAINMAKER_URL_HOSTNAME_LEN))
	{
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT ;
	}
	strncpy(wallet_config.node_url_cfg,  chainmaker_node_url,  strlen(chainmaker_node_url));
	strncpy(wallet_config.host_name_cfg, chainmaker_host_name, strlen(chainmaker_host_name));
	strncpy(wallet_config.chain_id_cfg,  chainmaker_chain_id,  strlen(chainmaker_chain_id));
	strncpy(wallet_config.org_id_cfg,    chainmaker_org_id,    strlen(chainmaker_org_id));

	//tls ca cert
	wallet_config.tls_ca_cert_cfg.length = strlen(chainmaker_tls_ca_cert) + 1;
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
	if (index < BOAT_SUCCESS)
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

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();

	/* step-2: prepare wallet */
	result = chainmakerWalletPrepare();
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	/* step-3: Chainmaker transaction structure initialization */
	result = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, &tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	result = BoatHlchainmakerAddTxParam(&tx_ptr, 6, "time", "6543235", "file_hash", "ab3456df5799b87c77e7f85", "file_name", "name005", NULL);
   	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerAddTxParam() failed.");
		return -1;
	}

	/* step-4: set transaction 'invoke' command */
	result = BoatHlchainmakerContractInvoke(&tx_ptr, "save","fact", true, &invoke_response); 
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	BoatLog( BOAT_LOG_CRITICAL, "response code = %d, message = %s, gas_used = %d\n", invoke_response.code, invoke_response.message, invoke_response.gas_used);

	/* step-5: wait seconds and 'query' the gas */
	BoatSleep(2);
	result = BoatHlchainmakerAddTxParam(&tx_ptr, 2, "file_hash", "ab3456df5799b87c77e7f85", NULL);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	result = BoatHlchainmakerContractQuery(&tx_ptr, "find_by_file_hash","fact", &query_response);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}
	BoatLog( BOAT_LOG_CRITICAL, "response code = %d,  message = %s,  contract_result = %s, gas_used = %d\n", 
			query_response.code, query_response.message, query_response.contract_result, query_response.gas_used);
	/* step-6: chainmaker transaction structure Deinitialization */
	BoatHlchainmakerTxDeInit(&tx_ptr);

	/* step-7: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}

