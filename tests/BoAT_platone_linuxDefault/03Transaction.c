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

#define EXCEED_STR_MAX_LEN          4097
#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_PLATONE_CHAIN_ID       1
#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_IS_SYNC_TX             BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS      "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45"

extern BoatPlatoneWalletConfig get_platone_wallet_settings();

START_TEST(test_006GetBalance_0001GetSuccess) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;
    BCHAR *cur_balance_wei = NULL;
    BoatFieldVariable parse_result = {NULL, 0};

    BoatIotSdkInit();

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, TEST_GAS_PRICE,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    cur_balance_wei = BoatPlatoneWalletGetBalance(g_platone_wallet_ptr, TEST_RECIPIENT_ADDRESS);
	result          = BoatPlatoneParseRpcResponseStringResult(cur_balance_wei, &parse_result);

    ck_assert_int_eq(result, BOAT_SUCCESS);
    ck_assert_ptr_ne(parse_result.field_ptr, NULL);

    BoatFree(parse_result.field_ptr);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_006GetBalance_0002GetWalletDefaultAddressSuccess) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;
    BCHAR *cur_balance_wei = NULL;
    BoatFieldVariable parse_result = {NULL, 0};

    BoatIotSdkInit();

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);


    cur_balance_wei = BoatPlatoneWalletGetBalance(g_platone_wallet_ptr, NULL);
	result          = BoatPlatoneParseRpcResponseStringResult(cur_balance_wei, &parse_result);

    ck_assert_ptr_ne(parse_result.field_ptr, NULL);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BoatFree(parse_result.field_ptr);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_006GetBalance_0003GetFailureNullWallet) 
{
    BCHAR *cur_balance_wei = NULL;

    BoatIotSdkInit();

    cur_balance_wei = BoatPlatoneWalletGetBalance(NULL, NULL);

    ck_assert_ptr_eq(cur_balance_wei, NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007Transfer_0001TransferSuccess) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatPlatoneTransfer(&tx_ctx, "0x0");
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007Transfer_0002TransferFailureNullParam) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatPlatoneTransfer(&tx_ctx, NULL);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007Transfer_0003BoatPlatoneParseRpcResponseResultFail)
{
    BOAT_RESULT result;
    result = BoatPlatoneParseRpcResponseResult(NULL,NULL,NULL);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_007Transfer_0004TransferSuccessWithoutSync) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();    

    result = platoneWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, BOAT_FALSE, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatPlatoneTransfer(&tx_ctx, "0x0");
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParseJson_0001ParseJsonFailureNullParam)
{
    BOAT_RESULT result;
    BCHAR *json_string= "111";
    BCHAR *child_name = "";
    BoatFieldVariable *result_out = {NULL, 0};

    result = platone_parse_json_result(NULL, child_name, result_out);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    result = platone_parse_json_result(json_string, NULL, result_out);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    result = platone_parse_json_result(json_string, child_name, NULL);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);        
}
END_TEST

START_TEST(test_008ParseJson_0002ObtainResultObjectFailure)
{
    BOAT_RESULT result;
    BCHAR *json_string= "111";
    BCHAR *child_name = "";
    BoatFieldVariable *result_out = {"1", 1};

    result = platone_parse_json_result(json_string, child_name, result_out);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);
   
}
END_TEST

Suite *make_transactions_suite(void)
{
    /* Create Suite */
    Suite *s_transaction = suite_create("transaction");


    /* Create test cases */
    TCase *tc_transaction_api = tcase_create("transaction_api");

    tcase_set_timeout(tc_transaction_api,50);       

    /* Add a test case to the Suite */
    suite_add_tcase(s_transaction, tc_transaction_api);      
 
    /* Test cases are added to the test set */
    tcase_add_test(tc_transaction_api, test_006GetBalance_0001GetSuccess); 
    tcase_add_test(tc_transaction_api, test_006GetBalance_0002GetWalletDefaultAddressSuccess); 
    tcase_add_test(tc_transaction_api, test_006GetBalance_0003GetFailureNullWallet); 

    tcase_add_test(tc_transaction_api, test_007Transfer_0001TransferSuccess); 
    tcase_add_test(tc_transaction_api, test_007Transfer_0002TransferFailureNullParam); 
    tcase_add_test(tc_transaction_api, test_007Transfer_0003BoatPlatoneParseRpcResponseResultFail); 
    tcase_add_test(tc_transaction_api, test_007Transfer_0004TransferSuccessWithoutSync); 
    
    tcase_add_test(tc_transaction_api, test_008ParseJson_0001ParseJsonFailureNullParam); 
    tcase_add_test(tc_transaction_api, test_008ParseJson_0002ObtainResultObjectFailure); 
    
    return s_transaction;
}