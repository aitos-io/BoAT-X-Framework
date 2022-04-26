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
#include "TestABIContract.h"

#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_ETHEREUM_CHAIN_ID      5777
#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_IS_SYNC_TX             BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS      "0xde4c806b372Df8857C97cF36A08D528bB8E261Bd"
#define TEST_CONTRACT_ADDRESS       "0x"


extern BoatEthWalletConfig get_ethereum_wallet_settings();
/*BoatEthWalletConfig get_ethereum_wallet_settings()
{
    //set user private key context
        
    if (TEST_KEY_TYPE == BOAT_WALLET_PRIKEY_FORMAT_NATIVE)
    {
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(binFormatKey, 32, ethereum_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    }
    else
    {
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)ethereum_private_key_buf;
	    wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(ethereum_private_key_buf) + 1;
    }
	wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.prikeyCtx_config.prikey_type	  = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;


	wallet_config.chain_id             = TEST_ETHEREUM_CHAIN_ID;
    wallet_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    strncpy(wallet_config.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);
	
    return wallet_config;
}*/

START_TEST(test_001CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr = NULL;
    BoatEthWalletConfig wallet = get_ethereum_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet, sizeof(BoatEthWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

     /* 1. execute unit test */
	rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet, sizeof(BoatEthWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);

    g_ethereum_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_ethereum_wallet_ptr != NULL);
    //ck_assert(check_ethereum_wallet(g_ethereum_wallet_ptr) == BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001CreateWallet_0003CreatePersistWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr = NULL;

    BoatIotSdkInit();
    BoatEthWalletConfig wallet = get_ethereum_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum", &wallet, sizeof(BoatEthWalletConfig));

    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(access("ethereum", F_OK), 0);

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum", &wallet, sizeof(BoatEthWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);

    g_ethereum_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_ethereum_wallet_ptr != NULL);
    //ck_assert(check_ethereum_wallet(g_ethereum_wallet_ptr) == BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001CreateWallet_0004CreateLoadWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr = NULL;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum", NULL, sizeof(BoatEthWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);

    g_ethereum_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_ethereum_wallet_ptr != NULL);
    //ck_assert(check_ethereum_wallet(g_ethereum_wallet_ptr) == BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002InitWallet_0001SetEIP155CompSuccess)
{
    BSINT32 rtnVal;
    BoatEthWallet *wallet_ptr = BoatMalloc(sizeof(BoatEthWallet));
    BoatEthWalletConfig wallet = get_ethereum_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatEthWalletSetEIP155Comp(wallet_ptr, wallet.eip155_compatibility);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.eip155_compatibility == wallet.eip155_compatibility);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002InitWallet_0003SetChainIdSuccess)
{
    BSINT32 rtnVal;
    BoatEthWallet *wallet_ptr = BoatMalloc(sizeof(BoatEthWallet));
    BoatEthWalletConfig wallet = get_ethereum_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatEthWalletSetChainId(wallet_ptr, wallet.chain_id);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.chain_id == wallet.chain_id);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002InitWallet_0005SetNodeUrlSuccess)
{
    BSINT32 rtnVal;
    BoatEthWallet *wallet_ptr = BoatMalloc(sizeof(BoatEthWallet));
    BoatEthWalletConfig wallet = get_ethereum_wallet_settings();
    
    /* 1. execute unit test */
    wallet_ptr->network_info.node_url_ptr = NULL;
    rtnVal = BoatEthWalletSetNodeUrl(wallet_ptr, wallet.node_url_str);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 2-2. verify the global variables that be affected */
    ck_assert_str_eq(wallet_ptr->network_info.node_url_ptr, wallet.node_url_str);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002InitWallet_0012InitEthWalletSuccess)
{
    BoatEthWallet *rtnVal;
    BoatEthWalletConfig walletConfig = get_ethereum_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatEthWalletInit(&walletConfig, sizeof(BoatEthWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_ptr_ne(rtnVal,NULL);

    /* 2-2. verify the global variables that be affected */ 
}
END_TEST

START_TEST(test_008Contract_0001CallNoneInputContractFuction)
{
    BoatEthTx tx_ctx;
    BoatEthWallet *rtnVal;
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatEthWalletConfig walletConfig = get_ethereum_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatEthWalletInit(&walletConfig, sizeof(BoatEthWalletConfig));
    ck_assert_ptr_ne(rtnVal, NULL);

    result = BoatEthTxInit(rtnVal, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   (BCHAR *)TEST_CONTRACT_ADDRESS);
    ck_assert_int_eq(result, BOAT_SUCCESS);                   

    result_str = TestABIContract_count(&tx_ctx);
    ck_assert_ptr_ne(rtnVal, NULL);
}
END_TEST

START_TEST(test_008Contract_0002SetAndGetBoolContractFuction)
{
    BoatEthTx tx_ctx;
    BoatEthWallet *rtnVal;
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};
    BoatEthWalletConfig walletConfig = get_ethereum_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatEthWalletInit(&walletConfig, sizeof(BoatEthWalletConfig));
    ck_assert_ptr_ne(rtnVal, NULL);

    result = BoatEthTxInit(rtnVal, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   (BCHAR *)TEST_CONTRACT_ADDRESS);
    ck_assert_int_eq(result, BOAT_SUCCESS);                   

    result_str = TestABIContract_setBool(&tx_ctx, BOAT_TRUE);
    ck_assert_ptr_ne(rtnVal, NULL);

    result_str = TestABIContract_getBool(&tx_ctx);
    ck_assert_ptr_ne(rtnVal, NULL);
    result = BoatEthParseRpcResponseStringResult(result_str, &parse_result);
    ck_assert_int_eq(result, BOAT_SUCCESS);   

    BoatLog(BOAT_LOG_NORMAL, "readListByIndex returns: %s", parse_result.field_ptr);
}
END_TEST

START_TEST(test_008Contract_0003SetAndGetUintContractFuction)
{
    BoatEthTx tx_ctx;
    BoatEthWallet *rtnVal;
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};
    BoatEthWalletConfig walletConfig = get_ethereum_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatEthWalletInit(&walletConfig, sizeof(BoatEthWalletConfig));
    ck_assert_ptr_ne(rtnVal, NULL);

    result = BoatEthTxInit(rtnVal, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   (BCHAR *)TEST_CONTRACT_ADDRESS);
    ck_assert_int_eq(result, BOAT_SUCCESS);                   

    result_str = TestABIContract_setUint32(&tx_ctx, 0x12345);
    ck_assert_ptr_ne(rtnVal, NULL);

    result_str = TestABIContract_getUint32(&tx_ctx);
    ck_assert_ptr_ne(rtnVal, NULL);
    result = BoatEthParseRpcResponseStringResult(result_str, &parse_result);
    ck_assert_int_eq(result, BOAT_SUCCESS);   

    BoatLog(BOAT_LOG_NORMAL, "readListByIndex returns: %s", parse_result.field_ptr);
}
END_TEST

START_TEST(test_008Contract_0004SetAndGetStringContractFuction)
{
    BoatEthTx tx_ctx;
    BoatEthWallet *rtnVal;
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};
    BCHAR s[] = "AbCdEfGhIjKlMn";
    BoatEthWalletConfig walletConfig = get_ethereum_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatEthWalletInit(&walletConfig, sizeof(BoatEthWalletConfig));
    ck_assert_ptr_ne(rtnVal, NULL);

    result = BoatEthTxInit(rtnVal, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   (BCHAR *)TEST_CONTRACT_ADDRESS);
    ck_assert_int_eq(result, BOAT_SUCCESS);               

    result_str = TestABIContract_setString(&tx_ctx, s);
    ck_assert_ptr_ne(rtnVal, NULL);

    result_str = TestABIContract_getString(&tx_ctx);
    ck_assert_ptr_ne(rtnVal, NULL);
    result = BoatEthParseRpcResponseStringResult(result_str, &parse_result);
    ck_assert_int_eq(result, BOAT_SUCCESS);   

    BoatLog(BOAT_LOG_NORMAL, "readListByIndex returns: %s", parse_result.field_ptr);
}
END_TEST

Suite *make_general_suite(void)
{
    /* Create Suite */
    Suite *s_general = suite_create("general");


    /* Create test cases */
    TCase *tc_general_api = tcase_create("general_api");


    /* Add a test case to the Suite */
    suite_add_tcase(s_general, tc_general_api);      
 
    /* Test cases are added to the test set */
    tcase_add_test(tc_general_api, test_001CreateWallet_0001CreateOneTimeWalletSuccess); 
    tcase_add_test(tc_general_api, test_001CreateWallet_0003CreatePersistWalletSuccess); 
    tcase_add_test(tc_general_api, test_001CreateWallet_0004CreateLoadWalletSuccess);

    tcase_add_test(tc_general_api, test_002InitWallet_0001SetEIP155CompSuccess); 
    tcase_add_test(tc_general_api, test_002InitWallet_0003SetChainIdSuccess); 
    tcase_add_test(tc_general_api, test_002InitWallet_0005SetNodeUrlSuccess); 
    tcase_add_test(tc_general_api, test_002InitWallet_0012InitEthWalletSuccess); 

    tcase_add_test(tc_general_api, test_008Contract_0001CallNoneInputContractFuction); 
    tcase_add_test(tc_general_api, test_008Contract_0002SetAndGetBoolContractFuction); 
    tcase_add_test(tc_general_api, test_008Contract_0003SetAndGetUintContractFuction); 
    tcase_add_test(tc_general_api, test_008Contract_0004SetAndGetStringContractFuction); 
    
    
    return s_general;
}