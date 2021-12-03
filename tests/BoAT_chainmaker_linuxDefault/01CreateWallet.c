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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "boattypes.h"
#include "boatwallet.h"
#include "protocolapi/api_chainmaker.h"
#include "tcase_common.h"
#include "check.h"

static BoatHlchainmakerWalletConfig get_chainmaker_wallet_settings()
{
    BoatHlchainmakerWalletConfig wallet_config = {0};
    //set user private key context
    wallet_config.user_prikey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.user_prikey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
    wallet_config.user_prikey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
    wallet_config.user_prikey_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_user_key;
    wallet_config.user_prikey_config.prikey_content.field_len = strlen(chainmaker_user_key) + 1; 

    //set user cert context
    wallet_config.user_cert_content.length = strlen(chainmaker_user_cert);
    memcpy(wallet_config.user_cert_content.content, chainmaker_user_cert, wallet_config.user_cert_content.length);
    
    //set url and name
    strncpy(wallet_config.node_url_arry, test_chainmaker_node_url,   strlen(test_chainmaker_node_url));
    strncpy(wallet_config.host_name_arry, test_chainmaker_host_name, strlen(test_chainmaker_host_name));

    //tls ca cert
    wallet_config.org_tls_ca_cert.length = strlen(chainmaker_tls_ca_cert);
    memcpy(wallet_config.org_tls_ca_cert.content, chainmaker_tls_ca_cert, wallet_config.org_tls_ca_cert.length);
    return wallet_config;
}

START_TEST(test_01CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
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
}
END_TEST

START_TEST(test_01CreateWallet_0002CreateOneTimeWalletFailureNullConfig) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, NULL, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 3. verify test result */
    /* 3-1. verify the return value */
    ck_assert_int_eq(rtnVal, -1);

    /* 3-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
}
END_TEST

START_TEST(test_01CreateWallet_0003CreatePersistWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerWalletConfig wallet_config = get_chainmaker_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker", &wallet_config, sizeof(BoatHlchainmakerWalletConfig));

    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker", &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
}
END_TEST

START_TEST(test_01CreateWallet_0004CreateLoadWalletSuccess) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker", NULL, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
}
END_TEST

START_TEST(test_01CreateWallet_0005CreateLoadWalletFailureNoExist) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker_no_exit", NULL, sizeof(BoatHlchainmakerWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, -1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
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
    tcase_add_test(tc_wallet_api, test_01CreateWallet_0001CreateOneTimeWalletSuccess);  
    tcase_add_test(tc_wallet_api, test_01CreateWallet_0002CreateOneTimeWalletFailureNullConfig); 
    tcase_add_test(tc_wallet_api, test_01CreateWallet_0003CreatePersistWalletSuccess);
    tcase_add_test(tc_wallet_api, test_01CreateWallet_0004CreateLoadWalletSuccess);
    tcase_add_test(tc_wallet_api, test_01CreateWallet_0005CreateLoadWalletFailureNoExist);

    return s_wallet;
}

