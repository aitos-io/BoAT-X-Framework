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
#include <time.h>

#define USE_LOAD_PERSIST_WALLET
//#define USE_CREATE_PERSIST_WALLET
const BCHAR* chainmaker_user_key = "-----BEGIN EC PRIVATE KEY-----\n"
								   "MHcCAQEEIGAsHq9vdQSPywbdy0vBEzWexkMwjtO0jjn7aiSoPZZKoAoGCCqGSM49\n"
								   "AwEHoUQDQgAE5lSVTF90J60bc8U2MNyYzmS1f7Ez1jOUXNXrakbHU6ZRWmZpAswb\n"
								   "mhWkOfpEWBoegtGdPgIeIJX8z8KT7RL1YQ==\n"
								   "-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_user_cert = "-----BEGIN CERTIFICATE-----\n"
									"MIICaDCCAg6gAwIBAgIDCK2ZMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
									"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
									"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
									"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIxMTEwNzE1MzE0NloXDTI2\n"
									"MTEwNjE1MzE0NlowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
									"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
									"MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
									"LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE5lSVTF90\n"
									"J60bc8U2MNyYzmS1f7Ez1jOUXNXrakbHU6ZRWmZpAswbmhWkOfpEWBoegtGdPgIe\n"
									"IJX8z8KT7RL1YaNaMFgwKQYDVR0OBCIEIP4DgiyLenX5yCWNDAby3VDxjQ1Cha/R\n"
									"f8oU/WORTOagMCsGA1UdIwQkMCKAIKHQDgYZIdDzDcmdr5v7yrR52mzecWHmdaWm\n"
									"UVerWnaVMAoGCCqGSM49BAMCA0gAMEUCIEneyi79s4M5VjZ6gw4jxHhs8M5h3QAn\n"
									"PndVTWzaoaVHAiEAvVmqpu11y7jYpgflziCtU80ujlLs8XHDqSet+l+5oiw=\n"
									"-----END CERTIFICATE-----\n";

const BCHAR* chainmaker_tls_ca_cert =   "-----BEGIN CERTIFICATE-----\n"
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
BCHAR *contract_name         = "fact";

//method query invoke
BCHAR *invoke_metod          = "save";
BCHAR *query_metod           = "find_by_file_hash"; 

//key value para
BCHAR *key1 = "time";
BCHAR *key2 = "file_hash";
BCHAR *key3 = "file_name";


#define TIME_LEN 20
#define FILE_HASH_LEN 32
BCHAR str_value[90] = {"file_"};

BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
BoatHlchainmakerWalletConfig wallet_config = {0};

void  get_file_hash_str(char** file_hash_str)
{
	char* file_hash = BoatMalloc(FILE_HASH_LEN);
	//randrom generate
	BoatFieldMax16B  random_data;
	random_data.field_len = 16;
	BoatRandom(random_data.field, random_data.field_len, NULL);
	array_to_str(random_data.field ,file_hash, 16);
	*file_hash_str = file_hash;
}

void get_time_string(char** time_str)
{
	time_t rawtime;
	struct tm *info;
	char* time_buf = NULL;

	time(&rawtime);
	info = localtime(&rawtime);
	time_buf = BoatMalloc(TIME_LEN);
	strftime(time_buf, TIME_LEN, "%Y-%m-%d %H:%M:%S", info);
	*time_str = time_buf;	
} 

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

BoatTransactionPara get_invoke_para(char* time_str, char* file_hash_str)
{
	BoatTransactionPara transaction_para;
	transaction_para.n_parameters = 3;
	transaction_para.parameters = BoatMalloc(transaction_para.n_parameters * sizeof(BoatKeyValuePair));

	get_time_string(&time_str);
	get_file_hash_str(&file_hash_str);

	transaction_para.parameters[0].key   = key1;
	transaction_para.parameters[0].value = time_str;
	transaction_para.parameters[1].key   = key2;
	transaction_para.parameters[1].value = file_hash_str;;
	transaction_para.parameters[2].key   = key3;
	transaction_para.parameters[2].value = strcat(str_value, time_str);

	return transaction_para;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT           result  = BOAT_SUCCESS;
	BoatHlchainmakerTx    tx_ptr;
	BoatTransactionPara   inovke_transaction_para;
	BoatTransactionPara   query_transaction_para;
	char* time_str;
	char* file_hash_str;

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();

	/* step-2: prepare wallet */
	result = chainmakerWalletPrepare();
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	/* step-3: Chainmaker transaction structure initialization */
	result = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, chain_id, org_id, contract_name, &tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	inovke_transaction_para = get_invoke_para(time_str, file_hash_str);
	tx_ptr.wallet_ptr->node_info.node_url  = chainmaker_node_url;
	tx_ptr.wallet_ptr->node_info.host_name = chainmaker_host_name;
	/* step-4: set transaction 'invoke' command */
	result = BoatHlchainmakerContractClaimInvoke(&tx_ptr, &inovke_transaction_para, invoke_metod); 
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	/* step-5: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	query_transaction_para.n_parameters = 1;
	query_transaction_para.parameters = BoatMalloc(sizeof(BoatKeyValuePair));
	query_transaction_para.parameters->key   = key2;
	query_transaction_para.parameters->value = file_hash_str;

	result = BoatHlchainmakerContractClaimQuery(&tx_ptr, &query_transaction_para, query_metod);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	/* step-6: chainmaker transaction structure Deinitialization */
	BoatHlchainmakerTxDeInit(&tx_ptr);
	
	/* step-7: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    if (inovke_transaction_para.parameters != NULL)
    {
    	BoatFree(inovke_transaction_para.parameters);
    }

    if (query_transaction_para.parameters != NULL)
    {
    	BoatFree(query_transaction_para.parameters);
    }

    if (time_str != NULL) 
    {
    	BoatFree(time_str);
    }

    if (file_hash_str != NULL) 
    {
    	BoatFree(file_hash_str);
    }


    return 0;
}

