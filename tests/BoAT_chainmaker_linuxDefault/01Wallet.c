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

BOAT_RESULT check_chainmaker_wallet(BoatHlchainmakerWallet *wallet_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    result = strncmp(wallet_ptr->node_info.node_url_info, TEST_CHAINMAKER_NODE_URL, strlen(TEST_CHAINMAKER_NODE_URL));

    if (result != 0)
    {
        return result;
    }

    result = strncmp(wallet_ptr->user_cert_info.prikeyCtx.extra_data.value, chainmaker_sign_key_buf, strlen(chainmaker_sign_key_buf));
    if (result != 0) 
    {
        return result;
    }

    result = strncmp(wallet_ptr->user_cert_info.cert.field_ptr, chainmaker_sign_cert_buf, strlen(chainmaker_sign_cert_buf));
    if (result != 0) 
    {
        return result;
    }

    return BOAT_SUCCESS;
}

BoatHlchainmakerWalletConfig get_chainmaker_wallet_settings()
{
    BoatHlchainmakerWalletConfig wallet_config = {0};
    //set user private key context
    wallet_config.user_prikey_cfg.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.user_prikey_cfg.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
    wallet_config.user_prikey_cfg.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;

    wallet_config.user_prikey_cfg.prikey_content.field_ptr = (BUINT8 *)chainmaker_sign_key_buf;
    wallet_config.user_prikey_cfg.prikey_content.field_len = strlen(chainmaker_sign_key_buf) + 1; 

    //set user cert context
    wallet_config.user_cert_cfg.length = strlen(chainmaker_sign_cert_buf);
    memcpy(wallet_config.user_cert_cfg.content, chainmaker_sign_cert_buf, wallet_config.user_cert_cfg.length);
    //tls ca cert
    wallet_config.tls_ca_cert_cfg.length = strlen(chainmaker_ca_cert_buf) + 1;
    memcpy(wallet_config.tls_ca_cert_cfg.content, chainmaker_ca_cert_buf, wallet_config.tls_ca_cert_cfg.length);

    strncpy(wallet_config.node_url_cfg,  TEST_CHAINMAKER_NODE_URL,  strlen(TEST_CHAINMAKER_NODE_URL));
    strncpy(wallet_config.host_name_cfg, TEST_CHAINMAKER_HOST_NAME, strlen(TEST_CHAINMAKER_HOST_NAME));
    strncpy(wallet_config.chain_id_cfg,  TEST_CHAINMAKER_CHAIN_ID,  strlen(TEST_CHAINMAKER_CHAIN_ID));
    strncpy(wallet_config.org_id_cfg,    TEST_CHAINMAKER_ORG_ID,    strlen(TEST_CHAINMAKER_ORG_ID));

    return wallet_config;
}

START_TEST(test_001CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
    BoatHlchainmakerWalletConfig wallet_config = get_chainmaker_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

     /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);

    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);
}
END_TEST

START_TEST(test_001CreateWallet_0002CreateOneTimeWalletFailureNullConfig) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, NULL, sizeof(BoatHlchainmakerWalletConfig));
    
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
    BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;

    BoatIotSdkInit();
    BoatHlchainmakerWalletConfig wallet_config = get_chainmaker_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker", &wallet_config, sizeof(BoatHlchainmakerWalletConfig));

    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(access("chainmaker", F_OK), 0);

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker", &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);

    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);
}
END_TEST

START_TEST(test_001CreateWallet_0004CreateLoadWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker", NULL, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);

    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);
}
END_TEST

START_TEST(test_001CreateWallet_0005CreateLoadWalletFailureNoExist) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker_no_exist", NULL, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_PERSISTER_READ_FAIL);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
}
END_TEST

START_TEST(test_001CreateWallet_0006_CreateOneTimeWalletFailureShortSize) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
    BoatHlchainmakerWalletConfig wallet_config = get_chainmaker_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig) - 10);
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_OUT_OF_MEMORY);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
}
END_TEST

START_TEST(test_001CreateWallet_0007_CreateOneTimeWalletSucessLongSize) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
    BoatHlchainmakerWalletConfig wallet_config = get_chainmaker_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig) + 10);
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
}
END_TEST

START_TEST(test_001CreateWallet_0008_CreateOneTimeWalletFailureUrlFormatError) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
    BoatHlchainmakerWalletConfig wallet_config = get_chainmaker_wallet_settings();
    memset(wallet_config.node_url_cfg, 0, BAOT_CHAINMAKER_URL_HOSTNAME_LEN);
    strncpy(wallet_config.node_url_cfg,  "192",  4);

    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
}
END_TEST

START_TEST(test_001CreateWallet_0009_CreateOneTimeWalletFailureLongNum) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
    BoatHlchainmakerWalletConfig wallet_config = get_chainmaker_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));

    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);

     /* 2. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    ck_assert_int_eq(rtnVal, 1);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);

    /* 3. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    ck_assert_int_eq(rtnVal, 2);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);

     /* 4. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    ck_assert_int_eq(rtnVal, 3);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);

     /* 5. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    ck_assert_int_eq(rtnVal, 4);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);

     /* 6. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    ck_assert_int_eq(rtnVal, 5);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    ck_assert(check_chainmaker_wallet(g_chaninmaker_wallet_ptr) == BOAT_SUCCESS);

     /* 7. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_ERROR_WALLET_NUM_EXCEED);
}
END_TEST


START_TEST(test_001CreateWallet_0010_CreatePersisWalletFailureLongName) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerWallet *g_chaninmaker_wallet_ptr;
    char chainmaker_name[4097];

    for (int i = 0; i < 4097; i++)
    {
        chainmaker_name[i] = 'a';
    }

    BoatIotSdkInit();
    BoatHlchainmakerWalletConfig wallet_config = get_chainmaker_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, chainmaker_name, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));

    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_PERSISTER_STORE_FAIL);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
 
}
END_TEST

START_TEST(test_002DeleteWallet_0001DeleteWalletFailureNullFleName) 
{
    BoatWalletDelete(NULL);
    ck_assert_int_eq(access("chainmaker", F_OK), 0);
}
END_TEST

START_TEST(test_002DeleteWallet_0002DeleteWalletFailureNoExistingFile) 
{
    BoatWalletDelete("chainmaker_no_exist");
    ck_assert_int_eq(access("chainmaker", F_OK), 0);
}
END_TEST

START_TEST(test_002DeleteWallet_0003DeleteWalletSucessExistingFile) 
{
    BoatWalletDelete("chainmaker");
    ck_assert_int_eq(access("chainmaker", F_OK), -1);
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
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0006_CreateOneTimeWalletFailureShortSize);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0007_CreateOneTimeWalletSucessLongSize);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0008_CreateOneTimeWalletFailureUrlFormatError);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0009_CreateOneTimeWalletFailureLongNum);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0010_CreatePersisWalletFailureLongName);
    tcase_add_test(tc_wallet_api, test_002DeleteWallet_0001DeleteWalletFailureNullFleName);
    tcase_add_test(tc_wallet_api, test_002DeleteWallet_0002DeleteWalletFailureNoExistingFile);
    tcase_add_test(tc_wallet_api, test_002DeleteWallet_0003DeleteWalletSucessExistingFile);

    return s_wallet;
}

