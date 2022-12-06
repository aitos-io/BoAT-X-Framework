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
#include "tcase_cita.h"
#include "SimpleStorage.h"           //wasm contract 

#define TEST_IS_SYNC_TX             BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS      "0x776daaa7443599c2ef54fc41b1d26b98d916fead"
static const BUINT64 quota_limit_value   = 10000000;

extern BoatCitaWalletConfig get_cita_wallet_settings();

START_TEST(test_006CallMycontract_0001SolStorageSetSuccess) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatCitaTx tx_ctx;

    BoatIotSdkInit();

    result = citaWalletPrepare();
    ck_assert_int_eq(result, BOAT_SUCCESS);    

    result = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BUINT8 set_data[32] = {0};
    set_data[0]= 7;
    result_str = SimpleStorage_set(&tx_ctx, set_data);
    ck_assert_ptr_ne(result_str, NULL);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_006CallMycontract_0002SolStorageGetSuccess) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatCitaTx tx_ctx;

    BoatIotSdkInit();

    result = citaWalletPrepare();
    ck_assert_int_eq(result, BOAT_SUCCESS);    

    result = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result_str = SimpleStorage_get(&tx_ctx);
    ck_assert_ptr_ne(result_str,NULL);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_006CallMycontract_0003SolStorageSetNulltxctxFailure) 
{
     BCHAR *result_str;
    BOAT_RESULT result;
    BoatCitaTx tx_ctx;

    BoatIotSdkInit();

    result = citaWalletPrepare();
    ck_assert_int_eq(result, BOAT_SUCCESS);    

    result = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BUINT8 set_data[32] = {0};
    set_data[0]= 7;
    result_str = SimpleStorage_set(NULL, set_data);
    ck_assert_ptr_eq(result_str, NULL);

    result_str = SimpleStorage_get(NULL);
    ck_assert_ptr_eq(result_str,NULL);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_006CallMycontract_0004BoatCitaParseRpcResponseResultFail)
{
    BOAT_RESULT result;
    result = BoatCitaParseRpcResponseResult(NULL,NULL,NULL);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatSleep(1);
}
END_TEST

START_TEST(test_006CallMycontract_0005ParseJsonFailureNullParam)
{
    BOAT_RESULT result;
    BCHAR *json_string= "111";
    BCHAR *child_name = "";
    BoatFieldVariable *result_out = {NULL, 0};

    result = cita_parse_json_result(NULL, child_name, result_out);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    result = cita_parse_json_result(json_string, NULL, result_out);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    result = cita_parse_json_result(json_string, child_name, NULL);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);  
    BoatSleep(1);      
}
END_TEST

START_TEST(test_006CallMycontract_0006ObtainResultObjectFailure)
{
    BOAT_RESULT result;
    BCHAR *json_string= "111";
    BCHAR *child_name = "";
    BoatFieldVariable *result_out = {"1", 1};

    result = cita_parse_json_result(json_string, child_name, result_out);
    ck_assert_int_eq(result, BOAT_ERROR_WEB3_JSON_GETOBJ_FAIL);
    BoatSleep(1);
   
}
END_TEST

Suite *make_contract_suite(void)
{
    // /* Create Suite */
    Suite *s_contract = suite_create("contract");

    /* Create test cases */
    TCase *tc_contract_api = tcase_create("contract_api");\

    /* Add a test case to the Suite */
    suite_add_tcase(s_contract, tc_contract_api);
    /* Set the time out for test case */
    tcase_set_timeout(tc_contract_api, 50);            
 
    /* Test cases are added to the test set */
    tcase_add_test(tc_contract_api, test_006CallMycontract_0001SolStorageSetSuccess); 
    tcase_add_test(tc_contract_api, test_006CallMycontract_0002SolStorageGetSuccess); 
    tcase_add_test(tc_contract_api, test_006CallMycontract_0003SolStorageSetNulltxctxFailure); 
    tcase_add_test(tc_contract_api, test_006CallMycontract_0004BoatCitaParseRpcResponseResultFail); 
    tcase_add_test(tc_contract_api, test_006CallMycontract_0005ParseJsonFailureNullParam); 
    tcase_add_test(tc_contract_api, test_006CallMycontract_0006ObtainResultObjectFailure); 
    return s_contract;
}
