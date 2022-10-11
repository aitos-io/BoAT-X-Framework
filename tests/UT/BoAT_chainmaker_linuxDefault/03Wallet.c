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
#define EXCEED_STR_MAX_LEN 4097

BOAT_RESULT chainmaker_create_keypair(WALLET_TYPE wallet_type, BUINT8* keypair_index_ptr)
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
    if (wallet_type == WALLET_ONEITEM)
    {
        index = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    }

    else if (wallet_type == WALLET_PERSIST)
    {
        index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    }

    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *keypair_index_ptr = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT chainmaker_create_network(WALLET_TYPE wallet_type, BUINT8* network_index_ptr)
{
    BSINT32 index;
    BoatChainmakerNetworkData networkConfig = {0};

    //set user cert context
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    if (networkConfig.client_sign_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert,  networkConfig.client_sign_cert_content.length);

    strncpy(networkConfig.node_url,  TEST_CHAINMAKER_NODE_URL,  strlen(TEST_CHAINMAKER_NODE_URL));
    strncpy(networkConfig.host_name, TEST_CHAINMAKER_HOST_NAME, strlen(TEST_CHAINMAKER_HOST_NAME));
    strncpy(networkConfig.chain_id,  TEST_CHAINMAKER_CHAIN_ID,  strlen(TEST_CHAINMAKER_CHAIN_ID));
    strncpy(networkConfig.org_id,    TEST_CHAINMAKER_ORG_ID,    strlen(TEST_CHAINMAKER_ORG_ID));

    //set tls cert context
    networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert) + 1;
    if (networkConfig.client_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert,  networkConfig.client_tls_cert_content.length);

    /* create ethereum keypair */
    if (wallet_type == WALLET_ONEITEM)
    {
        index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);
    }

    else if (wallet_type == WALLET_PERSIST)
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
    rtnVal = chainmaker_create_keypair(WALLET_ONEITEM, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 0);
 
    rtnVal = chainmaker_create_network(WALLET_ONEITEM, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 0);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0002CreateOneTimeWalletFailureNullKeypair) 
{
    BSINT32 rtnVal;
    BUINT8 keypair_index;
    BUINT8 network_index;
    BoatIotSdkInit();

    rtnVal = chainmaker_create_network(WALLET_ONEITEM, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 0);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0003CreateOneTimeWalletFailureNullNetwork) 
{
    BSINT32 rtnVal;
    BUINT8 keypair_index;
    BUINT8 network_index;
    BoatIotSdkInit();

    rtnVal = chainmaker_create_network(WALLET_ONEITEM, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 0);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0004CreateOneTimeWalletFailureErrKeypairIndex) 
{
    BSINT32 rtnVal;
    BUINT8 keypair_index;
    BUINT8 network_index;
    BoatIotSdkInit();

    /* 1. execute unit test */
    rtnVal = chainmaker_create_keypair(WALLET_ONEITEM, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 0);
 
    rtnVal = chainmaker_create_network(WALLET_ONEITEM, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 0);

    keypair_index = 10;

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0005CreateOneTimeWalletFailureErrNetworkIndex) 
{
    BSINT32 rtnVal;
    BUINT8 keypair_index;
    BUINT8 network_index;
    BoatIotSdkInit();

    /* 1. execute unit test */
    rtnVal = chainmaker_create_keypair(WALLET_ONEITEM, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 0);
 
    rtnVal = chainmaker_create_network(WALLET_ONEITEM, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 0);

    network_index = 10;

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0006CreateFivePersistWalletSuccess) 
{
    BSINT32 rtnVal;
    BUINT8 keypair_index;
    BUINT8 network_index;
    BoatIotSdkInit();
    BoatChainmakerWallet *g_chaninmaker_wallet_ptr;

    /* Number 1 wallet sucess*/
    rtnVal = chainmaker_create_keypair(WALLET_PERSIST, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 1);
 
    rtnVal = chainmaker_create_network(WALLET_PERSIST, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 1);

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);

     /* Number 2 wallet sucess*/
    rtnVal = chainmaker_create_keypair(WALLET_PERSIST, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 2);
 
    rtnVal = chainmaker_create_network(WALLET_PERSIST, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 2);

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);

    /* Number 3 wallet sucess*/
    rtnVal = chainmaker_create_keypair(WALLET_PERSIST, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 3);
 
    rtnVal = chainmaker_create_network(WALLET_PERSIST, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 3);

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);

    /* Number 4 wallet sucess*/
    rtnVal = chainmaker_create_keypair(WALLET_PERSIST, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 4);
 
    rtnVal = chainmaker_create_network(WALLET_PERSIST, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 4);

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);

     /* Number 5 wallet sucess*/
    rtnVal = chainmaker_create_keypair(WALLET_PERSIST, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 5);
 
    rtnVal = chainmaker_create_network(WALLET_PERSIST, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 5);

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);
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
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0002CreateOneTimeWalletFailureNullKeypair); 
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0003CreateOneTimeWalletFailureNullNetwork);
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0004CreateOneTimeWalletFailureErrKeypairIndex);
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0005CreateOneTimeWalletFailureErrNetworkIndex);
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0006CreateFivePersistWalletSuccess);
    return s_wallet;
}

