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
#include "tcase_ethereum.h"

BOAT_RESULT check_ethereum_wallet(BoatEthWallet *wallet,BUINT8 keypairIndex,BCHAR *keypairName,BoatKeypairPriKeyCtx_config *keypairConfig,
                                    BUINT8 networkIndex,BoatEthNetworkConfig *networkConfig)
{
    BOAT_RESULT ret = BOAT_ERROR;
    if(wallet == NULL)
    {
        return ret;
    }

    if((wallet->account_info.prikeyCtx.keypair_index != keypairIndex) || \
        (0 != strncmp(wallet->account_info.prikeyCtx.keypair_name,keypairName,strlen(keypairName))) || \
        (strlen(wallet->account_info.prikeyCtx.keypair_name) != strlen(keypairName)) || \
        (wallet->account_info.prikeyCtx.prikey_format != keypairConfig->prikey_format) || \
        (wallet->account_info.prikeyCtx.prikey_type != keypairConfig->prikey_type) || \
        (wallet->network_info.networkIndex != networkIndex) || \
        (wallet->network_info.chain_id != networkConfig->chain_id) || \
        (wallet->network_info.eip155_compatibility != networkConfig->eip155_compatibility) || \
        (0 != strncmp(wallet->network_info.node_url_str,networkConfig->node_url_str,strlen(networkConfig->node_url_str))) || \
        (strlen(wallet->network_info.node_url_str) != strlen(networkConfig->node_url_str)))
    {
        return ret;
    }

    return BOAT_SUCCESS;
}



START_TEST(test_003Wallet_0001InitWalletSuccessOneTimeKeypairOneTimeNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BSINT32 networkIndex;
    BoatEthNetworkConfig networkConfig;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

    /* 2. Create onetime network */
    networkConfig.chain_id             = 5777;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 3.Init onetime wallet */
    wallet_p = BoatEthWalletInit(keypair_index,networkIndex);
    //ret = check_ethereum_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert(wallet_p != NULL);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_003Wallet_0002InitWalletSuccessOneTimeKeypairPersistNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatEthNetworkConfig networkConfig;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

    /* 2. Create persist network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex,5);
    ck_assert_int_gt(networkIndex,0);

    /* 3.Init onetime wallet */
    wallet_p = BoatEthWalletInit(keypair_index,networkIndex);
    //ret = check_ethereum_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert(wallet_p != NULL);

    /* 4.Delete persist network*/
    ret = BoATEthNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0003InitWalletSuccessPersistKeypairOneTimeNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatEthNetworkConfig networkConfig;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create persist keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(keypair_index, 1);

    /* 2. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 3.Init onetime wallet */
    wallet_p = BoatEthWalletInit(keypair_index,networkIndex);
    //ret = check_ethereum_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert(wallet_p != NULL);

    /* 4.Delete persist keypair*/
    ret = BoATIotKeypairDelete(keypair_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0004InitWalletSuccessPersistKeypairPersistNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatEthNetworkConfig networkConfig;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create persist keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(keypair_index, 1);

    /* 2. Create persist network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(networkIndex, 1);

    /* 3.Init wallet */
    wallet_p = BoatEthWalletInit(keypair_index,networkIndex);
    //ret = check_ethereum_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert(wallet_p != NULL);

    /* 4.Delete persist keypair and network*/
    ret = BoATIotKeypairDelete(keypair_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = BoATEthNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_003Wallet_0003WalletFailureWrongKeypair) 
{
    BOAT_RESULT ret;

    BOAT_RESULT networkIndex;
    BoatEthNetworkConfig networkConfig;
    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();

    /* 1. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 2.Init wallet */
    wallet_p = BoatEthWalletInit(5,networkIndex);
    ck_assert(wallet_p == NULL);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_003Wallet_0006InitWalletFailureWrongNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);


    /* 3.Init wallet */
    wallet_p = BoatEthWalletInit(keypair_index,5);
    ck_assert(wallet_p == NULL);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_003Wallet_0007DeInitWalletSuccessOneTimeKeypairOneTimeNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatEthNetworkConfig networkConfig;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

    /* 2. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 3.Init onetime wallet */
    wallet_p = BoatEthWalletInit(keypair_index,networkIndex);
    //ret = check_ethereum_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert(wallet_p != NULL);


    /* 4.DeInit wallet*/
    BoatEthWalletDeInit(wallet_p);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0008DeInitWalletSuccessOneTimeKeypairPersistNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatEthNetworkConfig networkConfig;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

    /* 2. Create persist network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex,5);
    ck_assert_int_gt(networkIndex,0);

    /* 3.Init onetime wallet */
    wallet_p = BoatEthWalletInit(keypair_index,networkIndex);
    //ret = check_ethereum_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert(wallet_p != NULL);

    /* 4.Deinit wallet*/
    BoatEthWalletDeInit(wallet_p);

    /* 5.Delete persist network*/
    ret = BoATEthNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0009DeInitWalletSuccessPersistKeypairOneTimeNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatEthNetworkConfig networkConfig;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create persist keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(keypair_index, 1);

    /* 2. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 3.Init onetime wallet */
    wallet_p = BoatEthWalletInit(keypair_index,networkIndex);
    //ret = check_ethereum_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert(wallet_p != NULL);

    /* 4.Deinit wallet*/
    BoatEthWalletDeInit(wallet_p);

    /* 5.Delete persist keypair*/
    ret = BoATIotKeypairDelete(keypair_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0010DeInitWalletSuccessPersistKeypairPersistNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatEthNetworkConfig networkConfig;

    BoatEthWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create persist keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(keypair_index, 1);

    /* 2. Create persist network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    networkIndex = BoATEthNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(networkIndex, 1);

    /* 3.Init wallet */
    wallet_p = BoatEthWalletInit(keypair_index,networkIndex);
    //ret = check_ethereum_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert(wallet_p != NULL);

    /* 4.Deinit wallet*/
    BoatEthWalletDeInit(wallet_p);

    /* 5.Delete persist keypair and network*/
    ret = BoATIotKeypairDelete(keypair_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = BoATEthNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

Suite *make_wallet_suite(void) 
{
    /* Create Suite */
    Suite *s_wallet = suite_create("wallet");

    /* Create test cases */
    TCase *tc_wallet_api = tcase_create("wallet_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_wallet, tc_wallet_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_wallet_api, test_003Wallet_0001InitWalletSuccessOneTimeKeypairOneTimeNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0002InitWalletSuccessOneTimeKeypairPersistNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0003InitWalletSuccessPersistKeypairOneTimeNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0004InitWalletSuccessPersistKeypairPersistNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0003WalletFailureWrongKeypair);
    tcase_add_test(tc_wallet_api, test_003Wallet_0006InitWalletFailureWrongNetwork);

    tcase_add_test(tc_wallet_api, test_003Wallet_0007DeInitWalletSuccessOneTimeKeypairOneTimeNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0008DeInitWalletSuccessOneTimeKeypairPersistNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0009DeInitWalletSuccessPersistKeypairOneTimeNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0010DeInitWalletSuccessPersistKeypairPersistNetwork);

    return s_wallet;
}

