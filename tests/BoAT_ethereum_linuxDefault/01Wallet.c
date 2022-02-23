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
#include "tcase_common.h"

BOAT_RESULT check_ethereum_wallet(BoatEthWallet *wallet_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;
	
    result = strncmp(wallet_ptr->network_info.node_url_ptr, TEST_ETHEREUM_NODE_URL, strlen(TEST_ETHEREUM_NODE_URL));
    if (result != 0)
    {
        return result;
    }

    result = strncmp(wallet_ptr->account_info.prikeyCtx.extra_data.value, ethereum_pkcs_key_buf, strlen(ethereum_pkcs_key_buf));
    if (result != 0) 
    {
        return result;
    }

    return BOAT_SUCCESS;
}

BoatEthWalletConfig get_ethereum_wallet_settings()
{
    BoatEthWalletConfig wallet_config = {0};
	BUINT8 binFormatKey[32]           = {0};
    (void)binFormatKey; //avoid warning
    
    //set user private key context
	wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.prikeyCtx_config.prikey_type	  = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
	wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)ethereum_pkcs_key_buf;
	wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(ethereum_pkcs_key_buf) + 1;

	wallet_config.chain_id             = TEST_ETHEREUM_CHAIN_ID;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);
	
    return wallet_config;
}

START_TEST(test_001CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr;
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

     /* 1. execute unit test */
	rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);

    g_ethereum_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_ethereum_wallet_ptr != NULL);
    ck_assert(check_ethereum_wallet(g_ethereum_wallet_ptr) == BOAT_SUCCESS);
}
END_TEST

START_TEST(test_001CreateWallet_0002CreateOneTimeWalletFailureNullConfig) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, NULL, sizeof(BoatEthWalletConfig));
    
    /* 3. verify test result */
    /* 3-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    /* 3-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
}
END_TEST

START_TEST(test_001CreateWallet_0003CreatePersistWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr;

    BoatIotSdkInit();
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum", &wallet_config, sizeof(BoatEthWalletConfig));

    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(access("ethereum", F_OK), 0);

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum", &wallet_config, sizeof(BoatEthWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);

    g_ethereum_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_ethereum_wallet_ptr != NULL);
    ck_assert(check_ethereum_wallet(g_ethereum_wallet_ptr) == BOAT_SUCCESS);
}
END_TEST

START_TEST(test_001CreateWallet_0004CreateLoadWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr;
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
    ck_assert(check_ethereum_wallet(g_ethereum_wallet_ptr) == BOAT_SUCCESS);
}
END_TEST

START_TEST(test_001CreateWallet_0005CreateLoadWalletFailureNoExist) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum_no_exist", NULL, sizeof(BoatEthWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_PERSISTER_READ_FAIL);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == false);
}
END_TEST

START_TEST(test_001CreateWallet_0006CreateOneTimeWalletFailureShortSize) 
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr;
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig) - 10);
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
}
END_TEST

START_TEST(test_001CreateWallet_0007CreateOneTimeWalletSuccessLongSize) 
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr;
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig) + 10);
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
}
END_TEST

START_TEST(test_001CreateWallet_0008CreateOneTimeWalletFailureProtocolUnknown)
{
    BSINT32 rtnVal;
    BoatEthWallet *g_ethereum_wallet_ptr;
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_UNKNOWN, NULL, &wallet_config, sizeof(BoatEthWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
}
END_TEST

START_TEST(test_002InitWallet_0009SetEIP155CompSuccess)
{
    BSINT32 rtnVal;
    BoatEthWallet *wallet_ptr = BoatMalloc(sizeof(BoatEthWallet));
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatEthWalletSetEIP155Comp(wallet_ptr, wallet_config.eip155_compatibility);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.eip155_compatibility == wallet_config.eip155_compatibility);
    
}
END_TEST

START_TEST(test_002InitWallet_0010SetEIP155CompFailureNullParam)
{
    BSINT32 rtnVal;
    BoatEthWallet *wallet_ptr = BoatMalloc(sizeof(BoatEthWallet));
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatEthWalletSetEIP155Comp(NULL, wallet_config.eip155_compatibility);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.eip155_compatibility == NULL);

}
END_TEST

START_TEST(test_002InitWallet_0011SetChainIdSuccess)
{
    BSINT32 rtnVal;
    BoatEthWallet *wallet_ptr = BoatMalloc(sizeof(BoatEthWallet));
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatEthWalletSetChainId(wallet_ptr, wallet_config.chain_id);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.chain_id == wallet_config.chain_id);
    
}
END_TEST

START_TEST(test_002InitWallet_0012SetChainIdFailureNullParam)
{
    BSINT32 rtnVal;
    BoatEthWallet *wallet_ptr = BoatMalloc(sizeof(BoatEthWallet));
    BoatEthWalletConfig wallet_config = get_ethereum_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatEthWalletSetChainId(NULL, wallet_config.chain_id);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.chain_id == NULL);

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
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0001CreateOneTimeWalletSuccess);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0002CreateOneTimeWalletFailureNullConfig); 
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0003CreatePersistWalletSuccess);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0004CreateLoadWalletSuccess);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0005CreateLoadWalletFailureNoExist);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0006CreateOneTimeWalletFailureShortSize);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0007CreateOneTimeWalletSuccessLongSize);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0008CreateOneTimeWalletFailureProtocolUnknown);
    tcase_add_test(tc_wallet_api, test_002InitWallet_0009SetEIP155CompSuccess);
    tcase_add_test(tc_wallet_api, test_002InitWallet_0010SetEIP155CompFailureNullParam);
    tcase_add_test(tc_wallet_api, test_002InitWallet_0011SetChainIdSuccess);
    tcase_add_test(tc_wallet_api, test_002InitWallet_0012SetChainIdFailureNullParam);

    return s_wallet;
}

