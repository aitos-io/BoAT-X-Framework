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
#include "tcase_chainmaker.h"

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

const BUINT8 pubkey_256R1[] = {0x43, 0xA7, 0x51, 0x3A, 0xE8, 0xD9, 0x62, 0xC1, 0x4C, 0x5F, 0x7A, 0x6A, 0x9C, 0xC3, 0xDE, 0x9E, 0x35, 0x9E, 0x96, 0xAE, 0x49, 0xD4, 0x16, 0xCE, 0xF4, 0xB9, 0xEE, 0x71, 0x7E, 0x5F, 0xE3, 0x94, 0x60, 0x29, 0x77, 0x2B, 0xA4, 0xEB, 0x93, 0xA4, 0xBF, 0x0C, 0xB7, 0x79, 0x3A, 0x69, 0x6C, 0xE0, 0x8C, 0xB9, 0x5D, 0x7A, 0xF6, 0xC3, 0xC6, 0x2C, 0xD2, 0x9D, 0xBA, 0xF7, 0x83, 0x37, 0xC3, 0x5F};
BUINT8 binFormatKey[32] = {0};

BOAT_RESULT test_chainmaker_create_keypair(BCHAR *keypairName, BBOOL is_onetime, BUINT8 *keypair_index_ptr)
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]                    = {0};
    BSINT32 index;

    (void)binFormatKey; //avoid warning

    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    /* create ethereum keypair */
    if (is_onetime)
    {
        index = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_RAM);
    }

    else
    {
        index = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_FLASH);
    }

    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *keypair_index_ptr = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT test_chainmaker_create_network(BBOOL is_onetime, BUINT8 *network_index_ptr)
{
    BSINT32 index;
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
    /* create ethereum keypair */
    if (is_onetime)
    {
        index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);
    }

    else
    {
         index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    }

    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *network_index_ptr = index;

    return BOAT_SUCCESS;
}

START_TEST(test_003CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BUINT8 keypair_index;
    BUINT8 network_index;
    BoatIotSdkInit();

    /* 1. execute unit test */
    rtnVal = test_chainmaker_create_keypair("keypairOnetime", BOAT_TRUE, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 0);
 
    rtnVal = test_chainmaker_create_network(BOAT_TRUE, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 0);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);

    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.keypair_index, 0);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);
    // /* check public key in the struct */
    ck_assert_int_eq(memcmp(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.pubkey_content, pubkey_256R1, sizeof(pubkey_256R1)), 0);

    /* check the result of getting network data */
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->network_info.networkIndex, 0);
     /* check client_member_type in the struct , must be same to the configuration */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->network_info.client_member_type, BOAT_MEMBER_TYPE_CERT);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.chain_id, chainmaker_chain_id);
    /* check hostname in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.host_name, chainmaker_host_name);
    /* check orgID in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.org_id, chainmaker_org_id);
    /* check url in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.node_url, chainmaker_node_url);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.ca_tls_cert_content.content, chainmaker_ca_tls_cert);

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.client_tls_privkey_data.value, chainmaker_clinet_tls_prikey);
        ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.client_tls_cert_content.content, chainmaker_client_tls_cert);  
    #endif
#endif

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0002CreateOneTimeWalletFailureWithoutKeypair) 
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;

    rtnVal = test_chainmaker_create_network(BOAT_TRUE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 0);
   
    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0003CreateOneTimeWalletFailureWithoutNetwork) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;

    BoatIotSdkInit();

    rtnVal = test_chainmaker_create_keypair("keypairOnetime", BOAT_TRUE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 0);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0004CreatePersistWalletSuccess) 
{
   
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = test_chainmaker_create_keypair("keypairPersist", BOAT_FALSE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 1);

    rtnVal = test_chainmaker_create_network(BOAT_FALSE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 1);
   
    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);

    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.keypair_index, 1);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.keypair_name, "keypairPersist"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);
    // /* check public key in the struct */
    ck_assert_int_eq(memcmp(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.pubkey_content, pubkey_256R1, sizeof(pubkey_256R1)), 0);

    /* check the result of getting network data */
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->network_info.networkIndex, 1);
     /* check client_member_type in the struct , must be same to the configuration */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->network_info.client_member_type, BOAT_MEMBER_TYPE_CERT);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.chain_id, chainmaker_chain_id);
    /* check hostname in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.host_name, chainmaker_host_name);
    /* check orgID in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.org_id, chainmaker_org_id);
    /* check url in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.node_url, chainmaker_node_url);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.ca_tls_cert_content.content, chainmaker_ca_tls_cert);

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.client_tls_privkey_data.value, chainmaker_clinet_tls_prikey);
        ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.client_tls_cert_content.content, chainmaker_client_tls_cert);  
    #endif
#endif

    BoATIotKeypairDelete(1);
    BoATChainmakerNetworkDelete(1);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0002CreatePersistWalletFailureWithoutKeypair) 
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;

    rtnVal = test_chainmaker_create_network(BOAT_FALSE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 1);
   
    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);

    BoATChainmakerNetworkDelete(1);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0003CreatePersistWalletFailureWithoutNetwork) 
{
    BSINT32 rtnVal;
    BoatIotSdkInit();

    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;

    rtnVal = test_chainmaker_create_keypair("keypairPersist", BOAT_FALSE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 1);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);

    BoATIotKeypairDelete(1);
    BoatIotSdkDeInit();
}
END_TEST


Suite *make_wallet_suite(void) 
{
    /* Create Suite */
    Suite *s_wallet = suite_create("wallet");

    /* Create test cases */
    TCase *tc_wallet_api = tcase_create("wallet_api");
    tcase_set_timeout(tc_wallet_api, 5);
    /* Add a test case to the Suite */
    suite_add_tcase(s_wallet, tc_wallet_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0001CreateOneTimeWalletSuccess);  
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0002CreateOneTimeWalletFailureWithoutKeypair); 
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0003CreateOneTimeWalletFailureWithoutNetwork);

    tcase_add_test(tc_wallet_api, test_003CreateWallet_0004CreatePersistWalletSuccess);
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0002CreatePersistWalletFailureWithoutKeypair); 
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0003CreatePersistWalletFailureWithoutNetwork);
    return s_wallet;
}

