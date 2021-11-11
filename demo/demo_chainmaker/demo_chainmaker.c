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
#include <time.h>

#define USE_ONETIME_WALLET
const BCHAR* chainmaker_user_key = "-----BEGIN EC PRIVATE KEY-----\n"
									"MHcCAQEEICVQ1L+mTE+Fg96tJHRyBSSJF/i0Na2EIdeO0I7MlIWKoAoGCCqGSM49\n"
									"AwEHoUQDQgAEnGbBhx8ZWXYQjgqUeMppwa3Af/bqKDzSLPnG7F2mfVtlCPSQEcrz\n"
									"hp1G2d1CZbGvOWilZmkOf1azKJ596NXAfg==\n"
									"-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_user_cert = "-----BEGIN CERTIFICATE-----\n"
									"MIICZzCCAg6gAwIBAgIDCQiPMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
									"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
									"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
									"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIxMTExMDAxMTUzMVoXDTI2\n"
									"MTEwOTAxMTUzMVowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
									"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
									"MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
									"LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEnGbBhx8Z\n"
									"WXYQjgqUeMppwa3Af/bqKDzSLPnG7F2mfVtlCPSQEcrzhp1G2d1CZbGvOWilZmkO\n"
									"f1azKJ596NXAfqNaMFgwKQYDVR0OBCIEINpgpwouzzNuvpVcqSJG2yEK/1Nsc+ee\n"
									"Hehc0CliS29kMCsGA1UdIwQkMCKAIBkZhvRKkDvOx6VhFkEggPTtjdMUnlIWesaS\n"
									"YmxKHzYpMAoGCCqGSM49BAMCA0cAMEQCIGLGlVFpj7vKin86DtSYbDJOfS9hjscJ\n"
									"+iM4rl+XUKyUAiAFZbcDM2bCvLALprWzi0kNQ6VkYP2BSGn/1niDcGZK1Q==\n"
									"-----END CERTIFICATE-----\n";


const BCHAR* chainmaker_tls_ca_cert = "-----BEGIN CERTIFICATE-----\n"
									  "MIICnTCCAkOgAwIBAgICafcwCgYIKoZIzj0EAwIwgYoxCzAJBgNVBAYTAkNOMRAw\n"
									  "DgYDVQQIEwdCZWlqaW5nMRAwDgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1v\n"
									  "cmcxLmNoYWlubWFrZXIub3JnMRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMT\n"
									  "GWNhLnd4LW9yZzEuY2hhaW5tYWtlci5vcmcwHhcNMjExMTA3MTUzMTQ2WhcNMzEx\n"
									  "MTA1MTUzMTQ2WjCBijELMAkGA1UEBhMCQ04xEDAOBgNVBAgTB0JlaWppbmcxEDAO\n"
									  "BgNVBAcTB0JlaWppbmcxHzAdBgNVBAoTFnd4LW9yZzEuY2hhaW5tYWtlci5vcmcx\n"
									  "EjAQBgNVBAsTCXJvb3QtY2VydDEiMCAGA1UEAxMZY2Eud3gtb3JnMS5jaGFpbm1h\n"
									  "a2VyLm9yZzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABMRD6zki7fZP1BiGj0g+\n"
									  "A94SbxIkDvntlRSO6HLqvJMG8jn4Ur4RwplvOkVz1jbD9WVRVPY7O91XiyU5ov+t\n"
									  "f0WjgZYwgZMwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wKQYDVR0O\n"
									  "BCIEIKHQDgYZIdDzDcmdr5v7yrR52mzecWHmdaWmUVerWnaVMEUGA1UdEQQ+MDyC\n"
									  "DmNoYWlubWFrZXIub3Jngglsb2NhbGhvc3SCGWNhLnd4LW9yZzEuY2hhaW5tYWtl\n"
									  "ci5vcmeHBH8AAAEwCgYIKoZIzj0EAwIDSAAwRQIgXeECA7tcPFd9BpkcSCGelV25\n"
									  "5fJDVQiRTGyg79MfxncCIQDTP3z3L84lrwe+YxppKyFS9ISSOA0EUc4o+/RdoGi8\n"
									  "aQ==\n"
									  "-----END CERTIFICATE-----\n";
//wallet para
BCHAR *chainmaker_node_url   = "127.0.0.1:12301";
BCHAR *chainmaker_host_name  = "chainmaker.org";

//client para
BCHAR *chain_id              = "chain1";
BCHAR *org_id                = "wx-org1.chainmaker.org";

BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
BoatHlchainmakerWalletConfig wallet_config = {0};


__BOATSTATIC BOAT_RESULT chainmakerWalletPrepare(void)
{
	BOAT_RESULT index;

	//set user private key context
	wallet_config.user_prikey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.user_prikey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.user_prikey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.user_prikey_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_user_key;
	wallet_config.user_prikey_config.prikey_content.field_len = strlen(chainmaker_user_key) + 1; 

	//set user cert context
	wallet_config.user_cert_content.length = strlen(chainmaker_user_cert);
	memcpy(wallet_config.user_cert_content.content, chainmaker_user_cert, wallet_config.user_cert_content.length);
	
	//set url and name
    wallet_config.node_cfg.node_url  = chainmaker_node_url;
    wallet_config.node_cfg.host_name = chainmaker_host_name;

	//tls ca cert
	wallet_config.node_cfg.org_tls_ca_cert.length = strlen(chainmaker_tls_ca_cert);
	memcpy(wallet_config.node_cfg.org_tls_ca_cert.content, chainmaker_tls_ca_cert, wallet_config.node_cfg.org_tls_ca_cert.length);

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
	if (index == BOAT_ERROR)
	{
		return BOAT_ERROR;
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
	BoatInvokeReponse     invoke_reponse;
	BoatQueryReponse      query_reponse;

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();

	/* step-2: prepare wallet */
	result = chainmakerWalletPrepare();
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	/* step-3: Chainmaker transaction structure initialization */
	result = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, chain_id, org_id, &tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	tx_ptr.wallet_ptr->node_info.node_url  = chainmaker_node_url;
	tx_ptr.wallet_ptr->node_info.host_name = chainmaker_host_name;

	BoatHlchainmakerAddTxParam(&tx_ptr, "time", "6543234", "file_hash", "ab3456df5799b87c77e7f88", "file_name", "name007", NULL);
	/* step-4: set transaction 'invoke' command */
	result = BoatHlchainmakerContractInvoke(&tx_ptr, "save","fact", true, &invoke_reponse); 
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}
	BoatLog( BOAT_LOG_CRITICAL, "reponse code = %d, message = %s, gas_used = %d\n", invoke_reponse.code, invoke_reponse.message, invoke_reponse.gas_used);


	/* step-5: wait seconds and 'query' the gas */
	BoatSleep(2);
	BoatHlchainmakerAddTxParam(&tx_ptr, "file_hash", "ab3456df5799b87c77e7f88");
	result = BoatHlchainmakerContractQuery(&tx_ptr, "find_by_file_hash","fact", &query_reponse);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}
	BoatLog( BOAT_LOG_CRITICAL, "reponse message = %s,  contract_result = %s, gas_used = %d\n", query_reponse.message, query_reponse.contract_result, query_reponse.gas_used);
	/* step-6: chainmaker transaction structure Deinitialization */
	BoatHlchainmakerTxDeInit(&tx_ptr);
	
	/* step-7: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}

