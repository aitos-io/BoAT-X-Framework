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

const BCHAR* chainmaker_client_sign_prikey = 
    "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEILgtE1C5l2Tx5hnYWm7MrQlq4eOcxRuCbOtatp0wEk81oAoGCCqGSM49\n"
    "AwEHoUQDQgAEQ6dROujZYsFMX3pqnMPenjWelq5J1BbO9LnucX5f45RgKXcrpOuT\n"
    "pL8Mt3k6aWzgjLldevbDxizSnbr3gzfDXw==\n"
    "-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_client_sign_cert = 
    "-----BEGIN CERTIFICATE-----\n"
    "MIICeTCCAh6gAwIBAgIDAogrMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
    "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
    "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
    "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMTEyNTA2MjUzMVoXDTI3\n"
    "MTEyNDA2MjUzMVowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
    "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
    "MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
    "LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEQ6dROujZ\n"
    "YsFMX3pqnMPenjWelq5J1BbO9LnucX5f45RgKXcrpOuTpL8Mt3k6aWzgjLldevbD\n"
    "xizSnbr3gzfDX6NqMGgwDgYDVR0PAQH/BAQDAgbAMCkGA1UdDgQiBCCufTSDLWhz\n"
    "/z1VO5owaUrOZorjUQzufjHlEpHYNqgZUTArBgNVHSMEJDAigCD/CRels6efRbYk\n"
    "+8teT20O+VdtmeN+3vpIgXbKLarTZzAKBggqhkjOPQQDAgNJADBGAiEA5VwXscWv\n"
    "V330I/TGBFSyJ1pEhfGDzkUCDH5JgdGuL48CIQCIfqN95GPu2KUfKUMPhVAX+20o\n"
    "PoHSDSh/PzoPpZLdHg==\n"
    "-----END CERTIFICATE-----\n";


#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    const BCHAR* chainmaker_ca_tls_cert = 
    "-----BEGIN CERTIFICATE-----\n"
    "MIICnjCCAkOgAwIBAgICK7swCgYIKoZIzj0EAwIwgYoxCzAJBgNVBAYTAkNOMRAw\n"
    "DgYDVQQIEwdCZWlqaW5nMRAwDgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1v\n"
    "cmcxLmNoYWlubWFrZXIub3JnMRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMT\n"
    "GWNhLnd4LW9yZzEuY2hhaW5tYWtlci5vcmcwHhcNMjIxMTI1MDYyNTMxWhcNMzIx\n"
    "MTIyMDYyNTMxWjCBijELMAkGA1UEBhMCQ04xEDAOBgNVBAgTB0JlaWppbmcxEDAO\n"
    "BgNVBAcTB0JlaWppbmcxHzAdBgNVBAoTFnd4LW9yZzEuY2hhaW5tYWtlci5vcmcx\n"
    "EjAQBgNVBAsTCXJvb3QtY2VydDEiMCAGA1UEAxMZY2Eud3gtb3JnMS5jaGFpbm1h\n"
    "a2VyLm9yZzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABMbHVgzI2LlAMNsPCnZO\n"
    "IUfYpkqRNDwjFalPs3gUQndwUYWCl1BiguaWed9fHorsfnc+GwTGjcIwE7iO/nVu\n"
    "1najgZYwgZMwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wKQYDVR0O\n"
    "BCIEIP8JF6Wzp59FtiT7y15PbQ75V22Z437e+kiBdsotqtNnMEUGA1UdEQQ+MDyC\n"
    "DmNoYWlubWFrZXIub3Jngglsb2NhbGhvc3SCGWNhLnd4LW9yZzEuY2hhaW5tYWtl\n"
    "ci5vcmeHBH8AAAEwCgYIKoZIzj0EAwIDSQAwRgIhAKN3jHuuMUsfY9UG6cyrRLtN\n"
    "MtqVL1KAr+h/KBxwCRWcAiEAx3uFKTj/RCNqJVL35ULz5pWcIaK/0TWRuq6sisXd\n"
    "wVk=\n"
    "-----END CERTIFICATE-----\n";

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        const BCHAR* chainmaker_clinet_tls_prikey = 
        "-----BEGIN EC PRIVATE KEY-----\n"
        "MHcCAQEEINgke7SKh+psxDsBD3esD8zCpJhwIU7m6IPDp60FPlDAoAoGCCqGSM49\n"
        "AwEHoUQDQgAEGycJFX0GC8e4Hek9CBv53lb8sjEziy61P/mg/46Ut7I0awfHSpfF\n"
        "xvysCMEE1XEdXCT6JjebjMfffNCsVRyWxQ==\n"
        "-----END EC PRIVATE KEY-----\n";

        const BCHAR* chainmaker_client_tls_cert = 
        "-----BEGIN CERTIFICATE-----\n"
        "MIICjjCCAjWgAwIBAgICdcAwCgYIKoZIzj0EAwIwgYoxCzAJBgNVBAYTAkNOMRAw\n"
        "DgYDVQQIEwdCZWlqaW5nMRAwDgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1v\n"
        "cmcxLmNoYWlubWFrZXIub3JnMRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMT\n"
        "GWNhLnd4LW9yZzEuY2hhaW5tYWtlci5vcmcwHhcNMjIxMTI1MDYyNTMxWhcNMjcx\n"
        "MTI0MDYyNTMxWjCBlDELMAkGA1UEBhMCQ04xEDAOBgNVBAgTB0JlaWppbmcxEDAO\n"
        "BgNVBAcTB0JlaWppbmcxHzAdBgNVBAoTFnd4LW9yZzEuY2hhaW5tYWtlci5vcmcx\n"
        "EzARBgNVBAsTCkNoYWluTWFrZXIxKzApBgNVBAMTImNsaWVudDEudGxzLnd4LW9y\n"
        "ZzEuY2hhaW5tYWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQbJwkV\n"
        "fQYLx7gd6T0IG/neVvyyMTOLLrU/+aD/jpS3sjRrB8dKl8XG/KwIwQTVcR1cJPom\n"
        "N5uMx9980KxVHJbFo38wfTAOBgNVHQ8BAf8EBAMCA/gwEwYDVR0lBAwwCgYIKwYB\n"
        "BQUHAwIwKQYDVR0OBCIEIDA7/j4z/SKzrbgF2MPSKdaxXoWGfObDZ4+22COqrszb\n"
        "MCsGA1UdIwQkMCKAIP8JF6Wzp59FtiT7y15PbQ75V22Z437e+kiBdsotqtNnMAoG\n"
        "CCqGSM49BAMCA0cAMEQCIDS4InAM5Oa6GB9lzL/K4c1/2F2vnf9k3rU7HQ3W2xAQ\n"
        "AiAX4PjazfcIqrDPztfHqxxcXRF2tMAqkMBJapZgWlQZrA==\n"
        "-----END CERTIFICATE-----\n";
    #endif
#endif

BCHAR *chainmaker_node_url   = "127.0.0.1:12301";
BCHAR *chainmaker_host_name  = "chainmaker.org";
BCHAR *chainmaker_chain_id   = "chain1";
BCHAR *chainmaker_org_id     = "wx-org1.chainmaker.org";

BoatChainmakerWallet *g_chaninmaker_wallet_ptr;
BoatChainmakerWallet  wallet_config = {0};
BUINT8 keypair_index = 0;
BUINT8 network_index = 0;

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
    BoatLog(BOAT_LOG_NORMAL, "create keypair persist");
    result = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif

    if (result < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypair_index = result;
    
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT chainmaker_create_network(void)
{
    BOAT_RESULT index;
    BoatChainmakerNetworkData networkConfig = {0};

    //set user cert context
    networkConfig.client_member_type              = BOAT_MEMBER_TYPE_CERT;
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert);
    if (networkConfig.client_sign_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert);

    networkConfig.node_url  = BoatMalloc(strlen(chainmaker_node_url) + 1);
    networkConfig.host_name = BoatMalloc(strlen(chainmaker_host_name) + 1);
    networkConfig.chain_id  = BoatMalloc(strlen(chainmaker_chain_id) + 1);
    networkConfig.org_id    = BoatMalloc(strlen(chainmaker_org_id) + 1);

    strcpy(networkConfig.node_url,  chainmaker_node_url);
    strcpy(networkConfig.host_name, chainmaker_host_name);
    strcpy(networkConfig.chain_id,  chainmaker_chain_id);
    strcpy(networkConfig.org_id,    chainmaker_org_id);
   
#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1) 
     networkConfig.ca_tls_cert_content.length = strlen(chainmaker_ca_tls_cert);
    if (networkConfig.ca_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig.ca_tls_cert_content.content, chainmaker_ca_tls_cert);

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        //set tls cert context
        networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert);
        if (networkConfig.client_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
        {
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        strcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert);

        networkConfig.client_tls_privkey_data.value_len = strlen(chainmaker_clinet_tls_prikey);
        if (networkConfig.client_tls_privkey_data.value_len  > BOAT_CHAINMAKER_PRIKEY_MAX_LEN)
        {
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        strcpy((BCHAR *)networkConfig.client_tls_privkey_data.value, chainmaker_clinet_tls_prikey);
    #endif
#endif 

#if defined(USE_ONETIME_WALLET)
    index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);

#elif defined(USE_CREATE_PERSIST_WALLET)
    BoatLog(BOAT_LOG_NORMAL, "startting create persist wallet");
    index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);

#else
    return BOAT_ERROR;
#endif
    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    network_index = index;

    if (networkConfig.node_url != NULL)
    {
        BoatFree(networkConfig.node_url);
    }

    if (networkConfig.host_name != NULL)
    {
        BoatFree(networkConfig.host_name);
    }

    if (networkConfig.chain_id != NULL)
    {
        BoatFree(networkConfig.chain_id);
    }

    if (networkConfig.org_id != NULL)
    {
        BoatFree(networkConfig.org_id);
    }

    return index;
}

int main(int argc, char *argv[])
{
    BOAT_RESULT           result  = BOAT_SUCCESS;
    BoatChainmakerTx      tx_ptr;
    BoatResponseData      response_data;
    boat_try_declare;

    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare keypair and network*/
    result = chainmaker_create_keypair();
    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "chainmaker create keypair failed.");
        boat_throw(result, chainmaker_demo_catch);;
    }

    result = chainmaker_create_network();
    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "chainmaker create network failed.");
        boat_throw(result, chainmaker_demo_catch);;
    }
    BoatLog(BOAT_LOG_CRITICAL, "wallet init keypair_index =%d, network_index = %d",keypair_index, network_index);

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    if (g_chaninmaker_wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerWalletInit failed.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, chainmaker_demo_catch);
    }

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, &tx_ptr, 0);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerTxInit failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-4: add invoke parameters */
    result = BoatChainmakerAddTxParam(&tx_ptr, 6, "time", "1669200547", "file_hash", "a3f485ea14d946168d2a448adea8d47123", "file_name", "file_name", NULL);
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
        BoatChainmakerResponseFree(&response_data);
        boat_throw(result, chainmaker_demo_catch);
    }
    BoatLog(BOAT_LOG_CRITICAL, "response code = %d, message = %s", response_data.code, response_data.message);
    BoatChainmakerResponseFree(&response_data);

    // /* step-6: add query parameters*/
    BoatSleep(2);
    result = BoatChainmakerAddTxParam(&tx_ptr, 2, "file_hash", "a3f485ea14d946168d2a448adea8d47123", NULL);
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
        BoatChainmakerResponseFree(&response_data);
        boat_throw(result, chainmaker_demo_catch);
    }
    BoatLog(BOAT_LOG_CRITICAL, "response code = %d,  message = %s,  contract_result = %s, tx_id = %s", 
            response_data.code, response_data.message, response_data.contract_result.payload.field_ptr, response_data.tx_id);
     BoatChainmakerResponseFree(&response_data);

    boat_catch(chainmaker_demo_catch)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
    }   

    if (g_chaninmaker_wallet_ptr != NULL)
    {
        BoatChainmakerWalletDeInit(g_chaninmaker_wallet_ptr);
    }

    /* step-8: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}

