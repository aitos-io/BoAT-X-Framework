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
#include "protocolapi/api_ethereum.h"
#include "tcase_common.h"
#include "check.h"




START_TEST(test_BoatWalletCreate_1) {
    BoatEthWalletConfig wallet_config = {0};
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. prepare test conditions*/
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);
    /* 2. execute unit test */
    rtnVal = BoatWalletCreate( BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig) );
    /* 3. verify test result */
    /* 3-1. verify the return value(if have) */
    ck_assert_int_eq(rtnVal, 0);
    /* 3-2. verify the output parameter(if have) */
    //none
    /* 3-3. verify the global variables that be affected(if have) */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == false);

    /* if BoatWalletCreate be invoked again, the return value should growth to 1.
       note: in a unit test, if a faild condition be detected, it will break and the following
       test result can't be verified. */
    rtnVal = BoatWalletCreate( BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig) );
    /* 3. verify test result */
    /* 3-1. verify the return value(if have) */
    ck_assert_int_eq(rtnVal, 1);
    /* 3-2. verify the output parameter(if have) */
    //none
    /* 3-3. verify the global variables that be affected(if have) */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
}
END_TEST

START_TEST(test_BoatWalletCreate_2) {
    BoatEthWalletConfig wallet_config = {0};
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. prepare test conditions*/
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);
    /* 2. execute unit test */
    rtnVal = BoatWalletCreate( BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig) );
    /* 3. verify test result */
    /* 3-1. verify the return value(if have) */
    ck_assert_int_eq(rtnVal, 0);
    /* 3-2. verify the output parameter(if have) */
    //none
    /* 3-3. verify the global variables that be affected(if have) */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == false);
}
END_TEST


Suite * make_wallet_suite(void) {
    /* Create Suite */
    Suite *s_wallet = suite_create("wallet");
    /* Create test cases */
    TCase *tc_wallet_api = tcase_create("wallet_api");
    /* Add a test case to the Suite */
    suite_add_tcase(s_wallet, tc_wallet_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_wallet_api, test_BoatWalletCreate_1); 
    tcase_add_test(tc_wallet_api, test_BoatWalletCreate_2);
    
    return s_wallet;
}

