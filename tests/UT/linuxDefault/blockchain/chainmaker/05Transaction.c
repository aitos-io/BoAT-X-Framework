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
#include "tcase_chainmaker.h"

static BoatChainmakerWallet* g_chaninmaker_wallet_ptr;

void test_contrct_invoke_prepara(BoatChainmakerTx *tx_ptr)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare wallet */
    result = test_chainmaker_wallet_prepare(&g_chaninmaker_wallet_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, tx_ptr, 0);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerAddTxParam(tx_ptr, 6, "time", "6543235", "file_hash", "ab3456df5799b87c77e7f85", "file_name", "name005");
    ck_assert_int_eq(result, BOAT_SUCCESS);
}

void test_contrct_query_prepara(BoatChainmakerTx  *tx_ptr)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare wallet */
    result = test_chainmaker_wallet_prepare(&g_chaninmaker_wallet_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, tx_ptr, 0);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerAddTxParam(tx_ptr, 2, "file_hash", "ab3456df5799b87c77e7f85");
    ck_assert_int_eq(result, BOAT_SUCCESS);
}
    

START_TEST(test_005RunTransaction_0001InvokeFailureTxNull) 
{
    BOAT_RESULT        result;
    BoatResponseData  invoke_response;

    result = BoatChainmakerContractInvoke(NULL, "save", "fact", true, &invoke_response);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0002InvokeFailureMethodNull) 
{
    BOAT_RESULT       result;
    BoatChainmakerTx  tx_ptr;
    BoatResponseData  invoke_response;

    test_contrct_invoke_prepara(&tx_ptr);
    result = BoatChainmakerContractInvoke(&tx_ptr, NULL, "fact", true, &invoke_response);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0003InvokeFailureContractNull) 
{
    BOAT_RESULT       result;
    BoatChainmakerTx  tx_ptr;
    BoatResponseData  invoke_response;

    test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractInvoke(&tx_ptr, "save", NULL, true, &invoke_response);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0004InvokeFailureContractNoExist) 
{
    BOAT_RESULT       result;
    BoatChainmakerTx  tx_ptr;
    BoatResponseData  invoke_response;

    test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "test", true, &invoke_response);
    ck_assert_int_eq(result, BOAT_SUCCESS);
    ck_assert_int_eq(invoke_response.code, BOAT_SUCCESS);
    ck_assert_str_eq(invoke_response.message, "SUCCESS");
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0005InvokeFailureResponseNull) 
{
    BOAT_RESULT       result;
    BoatChainmakerTx tx_ptr;
    
    test_contrct_invoke_prepara(&tx_ptr);

    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "test", true, NULL);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0006InvokeSucessSyncOn) 
{
    BOAT_RESULT       result;
    BoatChainmakerTx  tx_ptr;
    BoatResponseData  invoke_response;

    BoatIotSdkInit();
    test_contrct_invoke_prepara(&tx_ptr);

    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "fact", true, &invoke_response);
    ck_assert_int_eq(result, BOAT_SUCCESS);
    
    ck_assert_int_eq(invoke_response.code, BOAT_SUCCESS);
    ck_assert_str_eq(invoke_response.message, "SUCCESS");

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0007InvokeSucessSyncOff) 
{
    BOAT_RESULT       result;
    BoatChainmakerTx  tx_ptr;
    BoatResponseData  invoke_response;

    test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BoatSleep(3);
    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "fact", false, &invoke_response);

    ck_assert_int_eq(result, BOAT_SUCCESS);
    ck_assert_int_eq(invoke_response.code, BOAT_SUCCESS);
    ck_assert_str_eq(invoke_response.message, "OK");
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0008InvokeFailureInvalidUrl) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  invoke_response;
    char url_buf[] = {"127.0.0.1:12310"};
    BoatIotSdkInit();

    test_contrct_invoke_prepara(&tx_ptr);
    strcpy(tx_ptr.wallet_ptr->network_info.node_url, url_buf);
    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "fact", true, &invoke_response); ;
    ck_assert_int_eq(result, BOAT_ERROR_HTTP2_TLS_INIT_FAIL);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005RunTransaction_0009QueryFailureTxNull) 
{
    BOAT_RESULT        result;
    BoatResponseData  query_response;

    result = BoatChainmakerContractQuery(NULL, "save", "fact", &query_response); ;
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0010QueryFailureMethodNull) 
{
    BOAT_RESULT       result;
    BoatChainmakerTx  tx_ptr;
    BoatResponseData  query_response;

    test_contrct_query_prepara(&tx_ptr);

    result = BoatChainmakerContractQuery(&tx_ptr, NULL, "fact", &query_response); ;
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005RunTransaction_0011QueryFailureContractNull) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx   tx_ptr;
    BoatResponseData   query_response;

    test_contrct_query_prepara(&tx_ptr);

    result = BoatChainmakerContractQuery(&tx_ptr, "save", NULL, &query_response); ;
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0012QueryFailureContractNoExist) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  query_response;
    
    test_contrct_query_prepara(&tx_ptr);
    result = BoatChainmakerContractQuery(&tx_ptr, "save", "test", &query_response); ;
    ck_assert_int_eq(result, BOAT_SUCCESS);
    ck_assert_int_eq(query_response.code, INVALIDCONTRACTPARAMETERCONTRACTNAME);
    BoatIotSdkDeInit();
 
}
END_TEST

START_TEST(test_005RunTransaction_0013QueryFailureResponseNull) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;

    test_contrct_query_prepara(&tx_ptr);
    result = BoatChainmakerContractQuery(&tx_ptr, "save", "fact", NULL); 
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005RunTransaction_0014QuerySucess) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  query_response;

    test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractQuery(&tx_ptr, "find_by_file_hash", "fact", &query_response); ;
    ck_assert_int_eq(result, BOAT_SUCCESS);

    ck_assert_int_eq(query_response.code, BOAT_SUCCESS);
    ck_assert_str_eq(query_response.message, "SUCCESS");  
    BoatIotSdkDeInit();
}
END_TEST

Suite *make_transaction_suite(void) 
{
    /* Create Suite */
    Suite *s_transaction = suite_create("transaction");

    /* Create test cases */
    TCase *tc_transaction_api = tcase_create("transaction_api");
    tcase_set_timeout(tc_transaction_api, 100.0);
    /* Add a test case to the Suite */
    suite_add_tcase(s_transaction, tc_transaction_api);       
    // /* Test cases are added to the test set */
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0001InvokeFailureTxNull); 
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0002InvokeFailureMethodNull);  
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0003InvokeFailureContractNull);  
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0004InvokeFailureContractNoExist);  
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0005InvokeFailureResponseNull);  
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0006InvokeSucessSyncOn); 
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0007InvokeSucessSyncOff); 
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0008InvokeFailureInvalidUrl); 

    tcase_add_test(tc_transaction_api, test_005RunTransaction_0009QueryFailureTxNull); 
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0010QueryFailureMethodNull); 
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0011QueryFailureContractNull); 
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0012QueryFailureContractNoExist);
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0013QueryFailureResponseNull);
    tcase_add_test(tc_transaction_api, test_005RunTransaction_0014QuerySucess);
 
    return s_transaction;
}

