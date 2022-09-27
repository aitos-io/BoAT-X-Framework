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
#include "boatkeypair.h"
#include "protocolapi/api_chainmaker.h"

const BCHAR* chainmaker_client_sign_prikey = "-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEIOyaQAt/ksxq8VOlkJqvFyne60m8IjaRMCaDgmVACOF3oAoGCCqGSM49\n"
"AwEHoUQDQgAEvhmUZnXAdpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVW\n"
"CYCeeChl4OBno9aCLc5BvFk4tmKDdOp96g==\n"
"-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_clinet_tls_prikey = "-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEIOyaQAt/ksxq8VOlkJqvFyne60m8IjaRMCaDgmVACOF3oAoGCCqGSM49\n"
"AwEHoUQDQgAEvhmUZnXAdpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVW\n"
"CYCeeChl4OBno9aCLc5BvFk4tmKDdOp96g==\n"
"-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_client_sign_cert = "-----BEGIN CERTIFICATE-----\n"
"MIICdzCCAh6gAwIBAgIDC9o/MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMDExMTA4NDk0OFoXDTI3\n"
"MDExMDA4NDk0OFowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
"MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
"LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvhmUZnXA\n"
"dpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVWCYCeeChl4OBno9aCLc5B\n"
"vFk4tmKDdOp96qNqMGgwDgYDVR0PAQH/BAQDAgbAMCkGA1UdDgQiBCDfUK7TyHf+\n"
"FJl27KT6uoRgcDOmNgwmc5G0A2dhn5fQfTArBgNVHSMEJDAigCAMxN5G+OtJXgmo\n"
"0MEU1OcJTBuDkfakZV07kDTZI9TcpjAKBggqhkjOPQQDAgNHADBEAiBEmn2YPZ0r\n"
"zzPwilsgtOcoPc6PkOawUnWIU6d7iehfIQIgaqDwoWO6nwT0q0Tasr2+suqidv9V\n"
"WOhSqMzOl8zMQ0c=\n"
"-----END CERTIFICATE-----\n";

const BCHAR* chainmaker_client_tls_cert = "-----BEGIN CERTIFICATE-----\n"
"MIICnzCCAkSgAwIBAgIDBXs2MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMDExMTA4NDk0OFoXDTMy\n"
"MDEwOTA4NDk0OFowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
"MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
"YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASb/f+wD0tJVZ78bXBF\n"
"LU/tar8wOoJi8+5qFwifEUwe4WdTBZJeqcqxn2/pcErzXnzGfcHMY609CU8sW2Bb\n"
"zOyTo4GWMIGTMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
"DgQiBCAMxN5G+OtJXgmo0MEU1OcJTBuDkfakZV07kDTZI9TcpjBFBgNVHREEPjA8\n"
"gg5jaGFpbm1ha2VyLm9yZ4IJbG9jYWxob3N0ghljYS53eC1vcmcxLmNoYWlubWFr\n"
"ZXIub3JnhwR/AAABMAoGCCqGSM49BAMCA0kAMEYCIQCoXy5qAIGrfnxRop9pgy9A\n"
"XQdXmu1mlryNGVmfisnm2gIhAMjQP7CTwoJDeu8bs36i+7ha4bIHct+pmE5BpjYh\n"
"ln6O\n"
"-----END CERTIFICATE-----\n";

BCHAR *chainmaker_node_url   = "121.4.178.74:12301";
BCHAR *chainmaker_host_name  = "chainmaker.org";
BCHAR *chainmaker_chain_id   = "chain1";
BCHAR *chainmaker_org_id     = "wx-org1.chainmaker.org";

BoatChainmakerWallet *g_chaninmaker_wallet_ptr;
BoatChainmakerWallet  wallet_config = {0};
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;

#define USE_ONETIME_WALLET
__BOATSTATIC BOAT_RESULT chainmaker_create_keypair()
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BOAT_RESULT result                         = BOAT_SUCCESS;
    BUINT8 binFormatKey[32]                    = {0};

    (void)binFormatKey; //avoid warning

    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    /* create ethereum keypair */
#if defined(USE_ONETIME_WALLET)
    result = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
#endif

    if (result < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypairIndex = result;
    return BOAT_SUCCESS;
}


__BOATSTATIC BOAT_RESULT chainmaker_create_network(void)
{
    BOAT_RESULT index;
    BoatChainmakerNetworkData networkConfig;

    //set user cert context
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    if (networkConfig.client_sign_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert,  networkConfig.client_sign_cert_content.length);
    
    //set url and name
    if ((strlen(chainmaker_node_url) >= BAOT_CHAINMAKER_NODE_STR_LEN) || 
        (strlen(chainmaker_host_name) >= BAOT_CHAINMAKER_NODE_STR_LEN)||
        (strlen(chainmaker_chain_id) >= BAOT_CHAINMAKER_NODE_STR_LEN)||
        (strlen(chainmaker_org_id) >= BAOT_CHAINMAKER_NODE_STR_LEN))
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT ;
    }
    strncpy(networkConfig.node_url,  chainmaker_node_url,  strlen(chainmaker_node_url));
    strncpy(networkConfig.host_name, chainmaker_host_name, strlen(chainmaker_host_name));
    strncpy(networkConfig.chain_id,  chainmaker_chain_id,  strlen(chainmaker_chain_id));
    strncpy(networkConfig.org_id,    chainmaker_org_id,    strlen(chainmaker_org_id));

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    //set tls cert context
    networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert) + 1;
    if (networkConfig.client_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert,  networkConfig.client_tls_cert_content.length);
#endif

#if (BOAT_CHAINMAKR_TLS_IDENTIFY_CLIENT == 1 )
    networkConfig.client_tls_privkey_content.value_len = strlen(chainmaker_clinet_tls_prikey) + 1;
    if (networkConfig.client_tls_privkey_content.value_len  > BOAT_CHAINMAKER_PRIKEY_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig.client_tls_privkey_content.value, chainmaker_clinet_tls_prikey,  networkConfig.client_tls_privkey_content.value_len);
#endif

#if defined(USE_ONETIME_WALLET)
    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
#else
    return BOAT_ERROR;
#endif
    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    return index;
}




int main(int argc, char *argv[])
{
    BOAT_RESULT           result  = BOAT_SUCCESS;
    BoatChainmakerTx    tx_ptr;
    BoatResponseData      response_data;
    boat_try_declare;

    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare keypair and network*/
    result = chainmaker_create_keypair();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "chainmaker create keypair failed.");
        boat_throw(result, chainmaker_demo_catch);;
    }

     result = chainmaker_create_network();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "chainmaker create network failed.");
        boat_throw(result, chainmaker_demo_catch);;
    }

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, &tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerTxInit failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-4: add invoke parameters */
    result = BoatChainmakerAddTxParam(&tx_ptr, 6, "time", "6543235", "file_hash", "ab3456df5799b87c77e7f85", "file_name", "name005", NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerAddTxParam() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-5: execute invoke command */
    result = BoatChainmakerContractInvoke(&tx_ptr, "save","fact", true, &response_data); 
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerContractInvoke() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }
    BoatLog( BOAT_LOG_CRITICAL, "response code = %d, message = %s, gas_used = %d\n", response_data.code, response_data.message, response_data.gas_used);

    /* step-6: add query parameters*/
    BoatSleep(2);
    result = BoatChainmakerAddTxParam(&tx_ptr, 2, "file_hash", "ab3456df5799b87c77e7f85", NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerAddTxParam() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-7: execute query command*/
    result = BoatChainmakerContractQuery(&tx_ptr, "find_by_file_hash","fact", &response_data);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerContractQuery() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }
    BoatLog(BOAT_LOG_CRITICAL, "response code = %d,  message = %s,  contract_result = %s, gas_used = %d\n", 
            response_data.code, response_data.message, response_data.contract_result, response_data.gas_used);

    boat_catch(chainmaker_demo_catch)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
    }   

    /* step-6: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}

