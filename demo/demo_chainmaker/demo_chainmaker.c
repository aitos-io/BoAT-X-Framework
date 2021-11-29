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
									"MHcCAQEEIAGYWRJdL9VqEUGXYApXWxGfrwmUc07XqcAAuQA18G/6oAoGCCqGSM49\n"
									"AwEHoUQDQgAElZkz2g5CwCOr857LVVM4/zL3ftAkoVh/l+HvF03+HMczm946AX7/\n"
									"k71Bd9BJxz0asSCZ4HdQqTgQCu5pL1F//A==\n"
									"-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_user_cert =	"-----BEGIN CERTIFICATE-----\n"
									"MIICaDCCAg6gAwIBAgIDAIdOMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
									"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
									"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
									"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIxMTExNzAxNTAxOVoXDTI2\n"
									"MTExNjAxNTAxOVowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
									"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
									"MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
									"LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAElZkz2g5C\n"
									"wCOr857LVVM4/zL3ftAkoVh/l+HvF03+HMczm946AX7/k71Bd9BJxz0asSCZ4HdQ\n"
									"qTgQCu5pL1F//KNaMFgwKQYDVR0OBCIEIPY5hXPDLG+mONhC7QnIapV5DDP50jEU\n"
									"oxHnpn+BYl4+MCsGA1UdIwQkMCKAIKsQREhdNfQU/TkPwqoQe0mTuHDGCX0ayS+7\n"
									"C390a93xMAoGCCqGSM49BAMCA0gAMEUCIQDXccYdq3KaM9ilX9Jzs9SCNv69nmUd\n"
									"P0Lhsl/OcsFTYwIgSpgE5TcKh0JHg6dF1ILf81KxZWTyy6EhTQpd9lvSf+8=\n"
									"-----END CERTIFICATE-----\n";

const BCHAR* chainmaker_tls_ca_cert =   "-----BEGIN CERTIFICATE-----\n"
										"MIICnzCCAkSgAwIBAgIDBbvnMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
										"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
										"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
										"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIxMTExNzAxNTAxOVoXDTMx\n"
										"MTExNTAxNTAxOVowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
										"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
										"MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
										"YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATTrbDKTYGmoROnEIa0\n"
										"YuPQ37OYTChMmE02xS39jillRbkoALddX2V6i35TVvq8SWJO8ewDPEL9RrU2nkqN\n"
										"ALYBo4GWMIGTMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
										"DgQiBCCrEERIXTX0FP05D8KqEHtJk7hwxgl9Gskvuwt/dGvd8TBFBgNVHREEPjA8\n"
										"gg5jaGFpbm1ha2VyLm9yZ4IJbG9jYWxob3N0ghljYS53eC1vcmcxLmNoYWlubWFr\n"
										"ZXIub3JnhwR/AAABMAoGCCqGSM49BAMCA0kAMEYCIQDn8p4qX+LSR8ljZsIGvP2M\n"
										"S4zeal8ndiJb7nBcm+/osQIhAL3MNSBIETeizkfNHE7j1LUWaL83wGtzlfn+BxCN\n"
										"Fqdj\n"
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
	result = BoatHlchainmakerAddTxParam(&tx_ptr, 6, "time", "6543235", "file_hash", "ab3456df5799b87c77e7f85", "file_name", "name005");
   	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerAddTxParam() failed.");
		return -1;
	}

	/* step-4: set transaction 'invoke' command */
	result = BoatHlchainmakerContractInvoke(&tx_ptr, "save","fact", true, &invoke_reponse); 
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}
	printf("BoatHlchainmakerContractInvoke\n");
	BoatLog( BOAT_LOG_CRITICAL, "reponse code = %d, message = %s, gas_used = %d\n", invoke_reponse.code, invoke_reponse.message, invoke_reponse.gas_used);

	/* step-5: wait seconds and 'query' the gas */
	BoatSleep(2);
	result = BoatHlchainmakerAddTxParam(&tx_ptr, 2, "file_hash", "ab3456df5799b87c77e7f85");
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	result = BoatHlchainmakerContractQuery(&tx_ptr, "find_by_file_hash","fact", &query_reponse);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}
	BoatLog( BOAT_LOG_CRITICAL, "reponse code = %d, reponse message = %s,  contract_result = %s, gas_used = %d\n", 
			query_reponse.code, query_reponse.message, query_reponse.contract_result, query_reponse.gas_used);
	/* step-6: chainmaker transaction structure Deinitialization */
	BoatHlchainmakerTxDeInit(&tx_ptr);

	/* step-7: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}

