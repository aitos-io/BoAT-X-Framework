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

const BCHAR * chainmaker_user_key = "-----BEGIN EC PRIVATE KEY-----\n"
									"MHcCAQEEIMWFr8I0WTT4WZKlToYIA/009/76L92EdseVcdTkCIxcoAoGCCqGSM49\n"
									"AwEHoUQDQgAEn+VS1wxvWZ7KexzxBaqMFbkAShsYN5jNvl0tK8U4ZQE2vazs5h6V\n"
									"0QnHI6cZNYfitnm3q6ua3UDcK5Vmi6w2SQ==\n"
									"-----END EC PRIVATE KEY-----";

const BCHAR *chainmaker_user_cert = "-----BEGIN CERTIFICATE-----\n"
									"MIICZjCCAg2gAwIBAgIDBn+vMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
									"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
									"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
									"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIxMTAxMzExMDY0MVoXDTI2\n"
									"MTAxMjExMDY0MVowgZAxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
									"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
									"MQ8wDQYDVQQLEwZjbGllbnQxKzApBgNVBAMTImNsaWVudDEudGxzLnd4LW9yZzEu\n"
									"Y2hhaW5tYWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASf5VLXDG9Z\n"
									"nsp7HPEFqowVuQBKGxg3mM2+XS0rxThlATa9rOzmHpXRCccjpxk1h+K2eberq5rd\n"
									"QNwrlWaLrDZJo1owWDApBgNVHQ4EIgQgTx52MwE6udu+5+cWWcDTyX8tr/CvbZ+2\n"
									"8gAbU2jBIb8wKwYDVR0jBCQwIoAgIpsb3IHzZkTPGWvYWDYBnEhKUdSl0xXwXf+N\n"
									"yJ3VNMEwCgYIKoZIzj0EAwIDRwAwRAIgdY3hnAn42SQhsTUG9bwvfnbIObfModMT\n"
									"Jb84nb/3IDQCIBl7allce0cJ342OGWnoaD4l9hsd43mD0erI9fM/H2yv\n"
									"-----END CERTIFICATE-----\n";

const BCHAR * chainmaker_root_ca_cert = "-----BEGIN CERTIFICATE-----\n"
									    "MIICnjCCAkSgAwIBAgIDAItWMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
										"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
										"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
										"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIxMTAxMzExMDY0MVoXDTMx\n"
										"MTAxMTExMDY0MVowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
										"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
										"MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
										"YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQQWQ4YdSBc+BCf1bfr\n"
										"0pEvffC8XLcaKlu+ltjsd56MKTz8SI4haQGnXeXoOnkqMtxV3bjeGkJEllo8r7Up\n"
										"e6ILo4GWMIGTMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
										"DgQiBCAimxvcgfNmRM8Za9hYNgGcSEpR1KXTFfBd/43IndU0wTBFBgNVHREEPjA8\n"
										"gg5jaGFpbm1ha2VyLm9yZ4IJbG9jYWxob3N0ghljYS53eC1vcmcxLmNoYWlubWFr\n"
										"ZXIub3JnhwR/AAABMAoGCCqGSM49BAMCA0gAMEUCIQC0i6lAHBpgiE1W2pW35zdw\n"
										"XMGyayGkiOGTgKOZ0W+aSQIgcT0DiYqQVcop/oKMlaV+FTagEcNw0+sjts36uSQA\n"
										"PT4=\n"
										"-----END CERTIFICATE-----\n";


const BCHAR *chain_Id              = "chain1";
const BCHAR *org_Id                = "wx-org1.chainmaker.org";
const BCHAR *claim_contract_name   = "claim001";
const BCHAR *chainmaker_node_url   = "127.0.0.1:12301";
const BCHAR *chainmaker_host_name  = "chainmaker.org";
const BCHAR *invoke_metod          = "save";
const BCHAR *query_metod           = "find_by_file_hash";    

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
	wallet_config.user_cert_content.length = strlen(chainmaker_user_cert) + 1;
	memcpy(wallet_config.user_cert_content.content, chainmaker_user_cert, wallet_config.user_cert_content.length);

	//root ca pool
	if (strlen(chainmaker_root_ca_cert) < BOAT_CHAINMAKER_CERT_MAX_LEN)
	{
		memcpy(wallet_config.root_ca_cert[0], chainmaker_root_ca_cert, strlen(chainmaker_root_ca_cert));	
	}
	
	wallet_config.root_ca_number = 1;
	wallet_config.node_info.org_Id    = org_Id;
	wallet_config.node_info.chain_Id  = chain_Id;
	wallet_config.node_info.node_url  = chainmaker_node_url;
	wallet_config.node_info.host_name = chainmaker_host_name;
	wallet_config.node_info.claim_contract_name  = claim_contract_name;
	wallet_config.node_info.tls_root_cert.length = strlen(chainmaker_root_ca_cert);
	memcpy(wallet_config.node_info.tls_root_cert.content, chainmaker_root_ca_cert, wallet_config.node_info.tls_root_cert.length);
			
	/* create chainmaker wallet */
	index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
	if (index == BOAT_ERROR)
	{
		return BOAT_ERROR;
	}
	
	g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(index);
	return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT           result  = BOAT_SUCCESS;
	BoatHlchainmakerTx    tx_ptr;
	
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();

	/* step-2: prepare wallet */
	result = chainmakerWalletPrepare();
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	/* step-3: Chainmaker transaction structure initialization */
	result = BoatHlChainmakerTxInit(&tx_ptr, g_chaninmaker_wallet_ptr);
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	/* step-4: set transaction 'invoke' command */
	result += BoatHlchainmakerContractClaimInvoke(&tx_ptr, "save"); 
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}

	/* step-5: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	result = BoatHlchainmakerContractClaimQuery(&tx_ptr, "find_by_file_hash");
	if (result != BOAT_SUCCESS)
	{
		return -1;
	}
	
	/* step-6: chainmaker transaction structure Deinitialization */
	BoatHlchainmakerTxDeInit(&tx_ptr);
	
	/* step-7: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}

