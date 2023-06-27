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
#include "TestPython.h"

#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_ETHEREUM_CHAIN_ID      5777

#define TEST_CONTRACT_ADDRESS       "0x50D0501A86332245c9B4Bdece773A6388c1b69Aa"
#define TEST_RECIPIENT_ADDRESS      "0x8c713E2F106a00F6d657BF00B3eF1eaCcffCbA4C"

__BOATSTATIC BoatEthWallet *ethereumOnetimeWalletPrepare()
{
    BOAT_RESULT index;
    BoatKeypairPriKeyCtx_config keypair_config;
    BUINT8 keypair_index;
    BoatEthNetworkConfig network_config;
    BUINT8 network_index;
    BoatEthWallet *wallet_p;

        
    if (TEST_KEY_TYPE == "BOAT_WALLET_PRIKEY_FORMAT_NATIVE")
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(g_binFormatKey, 32, g_ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = g_binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    }
    else
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)g_ethereum_private_key_buf;
	    keypair_config.prikey_content.field_len = strlen(g_ethereum_private_key_buf) + 1;
    }
	keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config, NULL, BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

	network_config.chain_id             = TEST_ETHEREUM_CHAIN_ID;
    network_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    memset(network_config.node_url_str,0U,BOAT_ETH_NODE_URL_MAX_LEN);
    strncpy(network_config.node_url_str, "http://127.0.0.1:7545", BOAT_ETH_NODE_URL_MAX_LEN - 1);

    network_index = BoATEthNetworkCreate(&network_config,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(network_index, 0);

    wallet_p = BoatEthWalletInit(keypair_index, network_index);
    ck_assert(wallet_p != NULL);

    return wallet_p;
}

START_TEST(test_005ChainAndContract_0001SetBytesSuccess) 
{
    BOAT_RESULT ret;

    BoatEthWallet *wallet;
    BoatEthTx tx_ctx;

    BCHAR *result_str;
    BoatFieldVariable bs[] = {{"123",3},{"456",4},{"7",1},{"89",3},{"012",2},{"345",4}};
    BUINT32 i;

    BoatIotSdkInit();

    wallet = ethereumOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatEthTxInit(wallet, &tx_ctx, BOAT_TRUE, NULL,
						TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Call Contract */
    result_str = TestPython_testOneBytesArray(&tx_ctx, bs, 6);
    ck_assert_ptr_ne(result_str, NULL);

    BoatEthWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005ChainAndContract_0002SetTwoBytesArraySuccess) 
{
    BOAT_RESULT ret;

    BoatEthWallet *wallet;
    BoatEthTx tx_ctx;

    BCHAR *result_str;
    BoatFieldVariable ba1[] = {{"123",3},{"456",4},{"7",1}};
    BoatFieldVariable ba2[] = {{"5",1},{"10",2},{"15",2},{"20",2},{"25",2}};
    BUINT32 i;

    BoatIotSdkInit();

    wallet = ethereumOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatEthTxInit(wallet, &tx_ctx,BOAT_TRUE, NULL,
						TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Call Contract */
    result_str = TestPython_testTwoBytesArrays(&tx_ctx, ba1, 3, ba2, 5);
    ck_assert_ptr_ne(result_str, NULL);

    BoatEthWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005ChainAndContract_0003SetTwoBytesArraysAndTwoNonFixedSuccess) 
{
    BOAT_RESULT ret;

    BoatEthWallet *wallet;
    BoatEthTx tx_ctx;

    BCHAR *result_str;
    BCHAR *teststring1 = "Hello";
    BCHAR *teststring2 = ",World";
    BoatFieldVariable ba1[] = {{"AbcdE", 5},{"qae", 4},{"Mti64$#", 6}};
    BoatFieldVariable ba2[] = {{"hy45jws4jw", 3},{"3qh553", 4},{"3246754", 5},{"12345", 5},{"31351", 4}};
    BUINT32 i;

    BoatIotSdkInit();

    wallet = ethereumOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatEthTxInit(wallet, &tx_ctx,BOAT_TRUE, NULL,
						TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Call Contract */
    result_str = TestPython_testTwoBytesArraysAndTwoNonFixed(&tx_ctx, teststring1, ba1, 3, teststring2, ba2, 5);
    ck_assert_ptr_ne(result_str, NULL);

    BoatEthWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0004GetBalanceSuccess) 
{
    BOAT_RESULT ret;

    BoatEthWallet *wallet = NULL;
    BoatEthTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = ethereumOnetimeWalletPrepare();

    /* Get Balance */
    result_str = BoatEthWalletGetBalance(wallet,TEST_RECIPIENT_ADDRESS);
    ck_assert_ptr_ne(result_str, NULL);

    BoatEthWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0005GetBalanceSuccessNullAddress) 
{
    BOAT_RESULT ret;

    BoatEthWallet *wallet = NULL;
    BoatEthTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = ethereumOnetimeWalletPrepare();

    /* Get Balance */
    result_str = BoatEthWalletGetBalance(wallet,NULL);
    ck_assert_ptr_ne(result_str, NULL);

    BoatEthWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0006GetBalanceFailureNullWallet) 
{
    BOAT_RESULT ret;

    BoatEthWallet *wallet = NULL;
    BoatEthTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    /* Get Balance */
    result_str = BoatEthWalletGetBalance(NULL,NULL);
    ck_assert_ptr_eq(result_str, NULL);

    BoatEthWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0007TransferSuccess) 
{
    BOAT_RESULT ret;

    BoatEthWallet *wallet = NULL;
    BoatEthTx tx_ctx;

    BoatIotSdkInit();

    wallet = ethereumOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatEthTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Transfer */
    ret = BoatEthTransfer(&tx_ctx,"0x0");
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatEthWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


Suite *make_chainAndContract_suite(void) 
{
    /* Create Suite */
    Suite *s_chainAndContract = suite_create("chainAndContract");

    /* Create test cases */
    TCase *tc_chainAndContract_api = tcase_create("chainAndContract_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_chainAndContract, tc_chainAndContract_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0001SetBytesSuccess);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0002SetTwoBytesArraySuccess);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0003SetTwoBytesArraysAndTwoNonFixedSuccess);
    
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0004GetBalanceSuccess);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0005GetBalanceSuccessNullAddress);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0006GetBalanceFailureNullWallet);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0007TransferSuccess);

    return s_chainAndContract;
}

