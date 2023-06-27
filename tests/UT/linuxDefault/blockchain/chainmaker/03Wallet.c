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
"MHcCAQEEICuhtWmvK4C3f1U48MMWflZTMJl4R74oRQ8MEsDGnF+doAoGCCqGSM49\n"
"AwEHoUQDQgAESwYKHefgAVJin1E8mSrBbsMpNKZybnsRA5HmTG6VQDp5rChGBvyl\n"
"5Gon43z1ff3cHaa1P42tmcnj2sS4lyCzvw==\n"
"-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_client_sign_cert = 
    "-----BEGIN CERTIFICATE-----\n"
"MIICeDCCAh6gAwIBAgIDCCc3MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIzMDEwNDA3MDMyOFoXDTI4\n"
"MDEwMzA3MDMyOFowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
"MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
"LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAESwYKHefg\n"
"AVJin1E8mSrBbsMpNKZybnsRA5HmTG6VQDp5rChGBvyl5Gon43z1ff3cHaa1P42t\n"
"mcnj2sS4lyCzv6NqMGgwDgYDVR0PAQH/BAQDAgbAMCkGA1UdDgQiBCDAkfh6k77j\n"
"VgRiyYoe54DL+7n7rua8a1DsLt685vaj2zArBgNVHSMEJDAigCDltbhMnD8zpJDE\n"
"cAfszaehAyyU8tPlO2MVLUk2sXG8YDAKBggqhkjOPQQDAgNIADBFAiEAhmyVTiVB\n"
"OcOkEcWdZKCO7KOkG9FUhqfvvDbXelYRFocCIC7qaf0Rg+ncjikoznItyNjpoB49\n"
"66RTrP5A5jquPLxN\n"
"-----END CERTIFICATE-----\n";


#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    const BCHAR* chainmaker_ca_tls_cert = 
   "-----BEGIN CERTIFICATE-----\n"
"MIICnjCCAkSgAwIBAgIDAiyYMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIzMDEwNDA3MDMyOFoXDTMz\n"
"MDEwMTA3MDMyOFowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
"MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
"YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAR3n1HznUS7Oh45zPaM\n"
"uA3HWXn9fnlHchtuy9J3ACQaPCUUSEGOONO4PyPs3L/jgtNprkUhgZsuHWCeZ2qS\n"
"X2gNo4GWMIGTMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
"DgQiBCDltbhMnD8zpJDEcAfszaehAyyU8tPlO2MVLUk2sXG8YDBFBgNVHREEPjA8\n"
"gg5jaGFpbm1ha2VyLm9yZ4IJbG9jYWxob3N0ghljYS53eC1vcmcxLmNoYWlubWFr\n"
"ZXIub3JnhwR/AAABMAoGCCqGSM49BAMCA0gAMEUCIGuw9A/NGU989gu7zbWqz+eV\n"
"hVTLMBspSjpfs1vG6jGrAiEA53b30lnIeNLKte03PXS4nkvNZ1wpOhO4fNiSh7r5\n"
"2rg=\n"
"-----END CERTIFICATE-----\n";


    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        const BCHAR* chainmaker_clinet_tls_prikey = 
           "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEIGwG4j7R07WQgVG+rsJpqRsx1W1szaPNDfG6xgElX6nYoAoGCCqGSM49\n"
    "AwEHoUQDQgAE0AtF7zzegqDfg+qg2Su1+16gJyCgZaaVljZMzzQ/VvxqtpcxaCR5\n"
    "7A4eG3wUR3h1G9VkTJA1ykajHsrk051HFw==\n"
    "-----END EC PRIVATE KEY-----\n";


            const BCHAR* chainmaker_client_tls_cert = 
            "-----BEGIN CERTIFICATE-----\n"
    "MIICkDCCAjagAwIBAgIDAssyMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
    "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
    "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
    "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIzMDEwNDA3MDMyOFoXDTI4\n"
    "MDEwMzA3MDMyOFowgZQxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
    "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
    "MRMwEQYDVQQLEwpDaGFpbk1ha2VyMSswKQYDVQQDEyJjbGllbnQxLnRscy53eC1v\n"
    "cmcxLmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE0AtF\n"
    "7zzegqDfg+qg2Su1+16gJyCgZaaVljZMzzQ/VvxqtpcxaCR57A4eG3wUR3h1G9Vk\n"
    "TJA1ykajHsrk051HF6N/MH0wDgYDVR0PAQH/BAQDAgP4MBMGA1UdJQQMMAoGCCsG\n"
    "AQUFBwMCMCkGA1UdDgQiBCCWH6Yex0Pz1WNnaBztiZ75JH8u2pAhWmEH5j3RNiIn\n"
    "EjArBgNVHSMEJDAigCDltbhMnD8zpJDEcAfszaehAyyU8tPlO2MVLUk2sXG8YDAK\n"
    "BggqhkjOPQQDAgNIADBFAiEA1Z8vZQqr2katSCW5idyNBTlxY7pSpcSI+ke5qaK/\n"
    "cUkCIGVDZPD2SASxDjXuuW5PXO5ftqCyrf5o9DAfoHXVgB3T\n"
    "-----END CERTIFICATE-----\n";
    #endif
#endif

BCHAR *chainmaker_host_name  = "chainmaker.org";
BCHAR *chainmaker_chain_id   = "chain1";
BCHAR *chainmaker_org_id     = "wx-org1.chainmaker.org";
const BUINT8 pubkey_256R1[] = {0x4B, 0x06, 0x0A, 0x1D, 0xE7, 0xE0, 0x01, 0x52, 0x62, 0x9F, 0x51, 0x3C, 0x99, 0x2A, 0xC1, 0x6E, 0xC3, 0x29, 0x34, 0xA6, 0x72, 0x6E, 0x7B, 0x11, 0x03, 0x91, 0xE6, 0x4C, 0x6E, 0x95, 0x40, 0x3A, 0x79, 0xAC, 0x28, 0x46, 0x06, 0xFC, 0xA5, 0xE4, 0x6A, 0x27, 0xE3, 0x7C, 0xF5, 0x7D, 0xFD, 0xDC, 0x1D, 0xA6, 0xB5, 0x3F, 0x8D, 0xAD, 0x99, 0xC9, 0xE3, 0xDA, 0xC4, 0xB8, 0x97, 0x20, 0xB3, 0xBF};
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
    networkConfig.node_url  = BoatMalloc(strlen(TEST_CHAINMAKER_NODE_URL) + 1);
    networkConfig.host_name = BoatMalloc(strlen(chainmaker_host_name) + 1);
    networkConfig.chain_id  = BoatMalloc(strlen(chainmaker_chain_id) + 1);
    networkConfig.org_id    = BoatMalloc(strlen(chainmaker_org_id) + 1);

    strcpy(networkConfig.node_url,  TEST_CHAINMAKER_NODE_URL);
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
        index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);
    }

    else
    {
         index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
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
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.node_url, TEST_CHAINMAKER_NODE_URL);

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
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.node_url, TEST_CHAINMAKER_NODE_URL);

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

