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

static BOAT_RESULT chainmakerWalletPrepare()
{
     /* 1. execute unit test */
    BUINT8 keypair_index;
    BUINT8 network_index;
    BOAT_RESULT result;
    result = chainmaker_create_keypair(WALLET_ONEITEM, &keypair_index);

    if (result != BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }
    result = chainmaker_create_network(WALLET_ONEITEM, &network_index);
    if (result != BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    if (g_chaninmaker_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }
    return BOAT_SUCCESS;
}


static BOAT_RESULT test_contrct_invoke_prepara(BoatChainmakerTx  *tx_ptr)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare wallet */
    result = chainmakerWalletPrepare();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test ChainmakerWalletPrepare() failed.");
        return -1;
    }

    if (g_chaninmaker_wallet_ptr == NULL)
    {
         return -2;
    }

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test BoatChainmakerTxInit() failed.");
        return -1;
    }

    // result = BoatChainmakerAddTxParam(tx_ptr, 6, "time", "6543235", "file_hash", "ab3456df5799b87c77e7f85", "file_name", "name005");
    // if (result != BOAT_SUCCESS)
    // {
    //     BoatLog(BOAT_LOG_CRITICAL, "test BoatHlchainmakerAddTxParam() failed.");
    //     return -1;
    // }
    return result;
}

static BOAT_RESULT test_contrct_query_prepara(BoatChainmakerTx  *tx_ptr)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare wallet */
    result = chainmakerWalletPrepare();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test ChainmakerWalletPrepare() failed.");
        return -1;
    }

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test BoatChainmakerTxInit() failed.");
        return -1;
    }

    result = BoatChainmakerAddTxParam(tx_ptr, 2, "file_hash", "ab3456df5799b87c77e7f85");
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test BoatHlchainmakerAddTxParam() failed.");
        return -1;
    }
    return result;
}
    

START_TEST(test_005ContractInvoke_0001InvokeFailureTxNull) 
{
    BOAT_RESULT        result;
    BoatResponseData  invoke_response;

    result = BoatChainmakerContractInvoke(NULL, "save", "fact", true, &invoke_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractInvoke_0002InvokeFailureMethodNull) 
{
    BOAT_RESULT       result;
    BoatChainmakerTx  tx_ptr;
    BoatResponseData  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    // result = BoatChainmakerContractInvoke(&tx_ptr, NULL, "fact", true, &invoke_response); ;
    // ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    // BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractInvoke_0003InvokeFailureContractNull) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractInvoke(&tx_ptr, "save", NULL, true, &invoke_response); ;
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractInvoke_0004InvokeFailureContractNoExist) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    // result = BoatChainmakerContractInvoke(&tx_ptr, "save", "test", true, &invoke_response); ;
    // ck_assert(result == BOAT_SUCCESS);
    // if (result == BOAT_SUCCESS)
    // {
    //     ck_assert(invoke_response.code == BOAT_SUCCESS);
    // }
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractInvoke_0005InvokeFailureresponseNull) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "test", true, NULL); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractInvoke_0006InvokeSucessSyncOn) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  invoke_response;

    BoatIotSdkInit();
    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "fact", true, &invoke_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {
        ck_assert_int_eq(invoke_response.code, BOAT_SUCCESS);
        ck_assert_int_eq(invoke_response.gas_used, 0);
    }
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractInvoke_0007InvokeSucessSyncOff) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "fact", false, &invoke_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {

        ck_assert_int_eq(invoke_response.code, BOAT_SUCCESS);
        ck_assert_int_eq(invoke_response.gas_used ,0);
    }
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractInvoke_0008InvokeFailureInvalidUrl) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  invoke_response;
    char url_buf[] = {"127.0.0.1:12310"};
    BoatIotSdkInit();

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);
    strncpy(tx_ptr.wallet_ptr->network_info.node_url, url_buf, sizeof(url_buf));
    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "fact", true, &invoke_response); ;
    ck_assert_int_eq(result, BOAT_ERROR_HTTP2_CONNECT_FAIL);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005ContractQuery_0009QueryFailureTxNull) 
{
    BOAT_RESULT        result;
    BoatResponseData  query_response;

    result = BoatChainmakerContractQuery(NULL, "save", "fact", &query_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractQuery_0010QueryFailureMethodNull) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  query_response;

    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractQuery(&tx_ptr, NULL, "fact", &query_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005ContractQuery_0011QueryFailureContractNull) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData   query_response;

    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractQuery(&tx_ptr, "save", NULL, &query_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractQuery_0012QueryFailureContractNoExist) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  query_response;

    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractQuery(&tx_ptr, "save", "test", &query_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {
        ck_assert(query_response.code != BOAT_SUCCESS);
        ck_assert(query_response.gas_used == 0);
    }
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractQuery_0013QueryFailureResponseNull) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;


    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractQuery(&tx_ptr, "save", "fact", NULL); 
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ContractQuery_0014QuerySucess) 
{
    BOAT_RESULT        result;
    BoatChainmakerTx tx_ptr;
    BoatResponseData  query_response;

    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatChainmakerContractQuery(&tx_ptr, "save", "fact", &query_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {
        ck_assert(query_response.code == BOAT_SUCCESS);
        ck_assert(query_response.gas_used != 0);
    }
    BoatIotSdkDeInit();
}
END_TEST

Suite *make_contract_suite(void) 
{
    /* Create Suite */
    Suite *s_contract = suite_create("contract");

    /* Create test cases */
    TCase *tc_contract_api = tcase_create("contract_api");
    tcase_set_timeout(tc_contract_api, 20.0);
    /* Add a test case to the Suite */
    suite_add_tcase(s_contract, tc_contract_api);       
    // /* Test cases are added to the test set */
    tcase_add_test(tc_contract_api, test_005ContractInvoke_0001InvokeFailureTxNull); 
    tcase_add_test(tc_contract_api, test_005ContractInvoke_0002InvokeFailureMethodNull);  
    tcase_add_test(tc_contract_api, test_005ContractInvoke_0003InvokeFailureContractNull);  
    tcase_add_test(tc_contract_api, test_005ContractInvoke_0004InvokeFailureContractNoExist);  
    tcase_add_test(tc_contract_api, test_005ContractInvoke_0005InvokeFailureresponseNull);  
    tcase_add_test(tc_contract_api, test_005ContractInvoke_0006InvokeSucessSyncOn); 
    tcase_add_test(tc_contract_api, test_005ContractInvoke_0007InvokeSucessSyncOff); 
    tcase_add_test(tc_contract_api, test_005ContractInvoke_0008InvokeFailureInvalidUrl); 

    tcase_add_test(tc_contract_api, test_005ContractQuery_0009QueryFailureTxNull); 
    tcase_add_test(tc_contract_api, test_005ContractQuery_0010QueryFailureMethodNull); 
    tcase_add_test(tc_contract_api, test_005ContractQuery_0011QueryFailureContractNull); 
    tcase_add_test(tc_contract_api, test_005ContractQuery_0012QueryFailureContractNoExist);
    tcase_add_test(tc_contract_api, test_005ContractQuery_0013QueryFailureResponseNull);
    tcase_add_test(tc_contract_api, test_005ContractQuery_0014QuerySucess);
 
    return s_contract;
}

