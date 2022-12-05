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
#include "my_contract.cpp.abi.h"           //wasm contract 

#define EXCEED_STR_MAX_LEN          4097
#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_PLATONE_CHAIN_ID       1
#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_IS_SYNC_TX             BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS      "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45"

extern BoatPlatoneWalletConfig get_platone_wallet_settings();

START_TEST(test_009CallMycontract_0001WASMsetNameSuccess) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);    

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert(result == BOAT_SUCCESS);

    result_str = my_contract_cpp_abi_setName(&tx_ctx,"aitos");

    ck_assert_ptr_ne(result_str,NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009CallMycontract_0002WASMgetNameSuccess) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);  

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert(result == BOAT_SUCCESS); 

    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    
    ck_assert_ptr_ne(result_str,NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009CallMycontract_0003WASMsetNameNulltxctxFailure) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);    

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert(result == BOAT_SUCCESS);

    result_str = my_contract_cpp_abi_setName(NULL,"aitos");

    ck_assert_ptr_eq(result_str,NULL);

    result_str = my_contract_cpp_abi_setName(NULL, NULL);

    ck_assert_ptr_eq(result_str,NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009CallMycontract_0004WASMsetNameNullmsgFailure) 
{
    BoatLog(BOAT_LOG_VERBOSE, "test_009CallMycontract_0004WASMsetNameNullmsgFailure");
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);    

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert(result == BOAT_SUCCESS);

    result_str = my_contract_cpp_abi_setName(&tx_ctx, NULL);

    ck_assert_ptr_eq(result_str,NULL);

    BoatIotSdkDeInit();
}
END_TEST

Suite *make_contract_suite(void)
{
    /* Create Suite */
    Suite *s_contract = suite_create("contract");


    /* Create test cases */
    TCase *tc_contract_api = tcase_create("contract_api");


    /* Add a test case to the Suite */
    suite_add_tcase(s_contract, tc_contract_api);

    /* Set the time out for test case */
    tcase_set_timeout(tc_contract_api, 50);            
 
    /* Test cases are added to the test set */
    tcase_add_test(tc_contract_api, test_009CallMycontract_0001WASMsetNameSuccess); 
    tcase_add_test(tc_contract_api, test_009CallMycontract_0002WASMgetNameSuccess); 
    tcase_add_test(tc_contract_api, test_009CallMycontract_0003WASMsetNameNulltxctxFailure); 
        
    return s_contract;
}
