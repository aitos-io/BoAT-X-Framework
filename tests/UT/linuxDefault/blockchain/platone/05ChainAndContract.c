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
#include "tcase_platone.h"
#include "my_contract.cpp.abi.h"

#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_PLATONE_CHAIN_ID       1

#define TEST_RECIPIENT_ADDRESS      "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45"

__BOATSTATIC BoatPlatoneWallet *platoneOnetimeWalletPrepare()
{
    BOAT_RESULT index;
    BoatKeypairPriKeyCtx_config keypair_config;
    BUINT8 keypair_index;
    BoatPlatoneNetworkConfig network_config;
    BUINT8 network_index;

    BoatPlatoneWallet *wallet_p = NULL;

        
    if (TEST_KEY_TYPE == "BOAT_WALLET_PRIKEY_FORMAT_NATIVE")
    {
        BoatLog(BOAT_LOG_CRITICAL, "g_platone_private_key_buf: %s", g_platone_private_key_buf);
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(g_binFormatKey, 32, g_platone_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = g_binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    }
    else
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)g_platone_private_key_buf;
	    keypair_config.prikey_content.field_len = strlen(g_platone_private_key_buf) + 1;
    }
	keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

	network_config.chain_id             = TEST_PLATONE_CHAIN_ID;
    network_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    memset(network_config.node_url_str,0U,BOAT_PLATONE_NODE_URL_MAX_LEN);
    strncpy(network_config.node_url_str, TEST_PLATONE_NODE_URL, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    network_index = BoATPlatoneNetworkCreate(&network_config,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(network_index, 0);

    wallet_p = BoatPlatoneWalletInit(keypair_index,network_index);
    ck_assert(wallet_p != NULL);

    return wallet_p;
}

START_TEST(test_005ChainAndContract_0001WASMsetNameSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet = NULL;
    BoatPlatoneTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet, &tx_ctx, BOAT_TRUE, NULL,
						TEST_GAS_LIMIT, (BCHAR *)TEST_RECIPIENT_ADDRESS,
                        BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Call Contract */
    result_str = my_contract_cpp_abi_setName(&tx_ctx,"aitos");
    ck_assert_ptr_ne(result_str, NULL);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0002WASMgetNameSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet = NULL;
    BoatPlatoneTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet, &tx_ctx, BOAT_TRUE, NULL,
						TEST_GAS_LIMIT, (BCHAR *)TEST_RECIPIENT_ADDRESS,
                        BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Call Contract */
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    ck_assert_ptr_ne(result_str, NULL);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0003WASMsetNameNulltxctxFailure) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet = NULL;
    BoatPlatoneTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet, &tx_ctx, BOAT_TRUE, NULL,
						TEST_GAS_LIMIT, (BCHAR *)TEST_RECIPIENT_ADDRESS,
                        BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Call Contract */
    result_str = my_contract_cpp_abi_setName(NULL,"aitos");
    ck_assert_ptr_eq(result_str,NULL);

    result_str = my_contract_cpp_abi_setName(NULL, NULL);
    ck_assert_ptr_eq(result_str,NULL);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0004WASMsetNameNullmsgFailures) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet = NULL;
    BoatPlatoneTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet, &tx_ctx, BOAT_TRUE, NULL,
						TEST_GAS_LIMIT, (BCHAR *)TEST_RECIPIENT_ADDRESS,
                        BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Call Contract */
    result_str = my_contract_cpp_abi_setName(&tx_ctx, NULL);
    ck_assert_ptr_eq(result_str,NULL);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0005GetBalanceSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet = NULL;
    BoatPlatoneTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Get Balance */
    result_str = BoatPlatoneWalletGetBalance(wallet,TEST_RECIPIENT_ADDRESS);
    ck_assert_ptr_ne(result_str, NULL);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0006GetBalanceSuccessNullAddress) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet = NULL;
    BoatPlatoneTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Get Balance */
    result_str = BoatPlatoneWalletGetBalance(wallet, NULL);
    ck_assert_ptr_ne(result_str, NULL);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0007GetBalanceFailureNullWallet)
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet = NULL;
    BoatPlatoneTx tx_ctx;

    BCHAR *result_str;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Get Balance */
    result_str = BoatPlatoneWalletGetBalance(NULL, NULL);
    ck_assert_ptr_eq(result_str, NULL);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ChainAndContract_0008TransferSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet = NULL;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet, &tx_ctx, BOAT_TRUE, NULL,
						TEST_GAS_LIMIT, (BCHAR *)TEST_RECIPIENT_ADDRESS,
                        BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Transfer */
    ret = BoatPlatoneTransfer(&tx_ctx,"0x0");
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatPlatoneWalletDeInit(wallet);

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

    /* Set the time out for test case */
    tcase_set_timeout(tc_chainAndContract_api,50); 
        
    /* Test cases are added to the test set */
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0001WASMsetNameSuccess);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0002WASMgetNameSuccess);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0003WASMsetNameNulltxctxFailure);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0004WASMsetNameNullmsgFailures);
    
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0005GetBalanceSuccess);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0006GetBalanceSuccessNullAddress);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0007GetBalanceFailureNullWallet);
    tcase_add_test(tc_chainAndContract_api, test_005ChainAndContract_0008TransferSuccess);

    return s_chainAndContract;
}

