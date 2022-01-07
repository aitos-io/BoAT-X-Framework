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

static BoatHlchainmakerWallet* g_chaninmaker_wallet_ptr;
static BoatHlchainmakerWalletConfig wallet_config = {0};
#define USE_ONETIME_WALLET

static BOAT_RESULT ChainmakerWalletPrepare(void)
{
    BOAT_RESULT index;

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


    // create wallet
#if defined(USE_ONETIME_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
#elif defined(USE_CREATE_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker.cfg", &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
#elif defined(USE_LOAD_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker.cfg", NULL, sizeof(BoatHlchainmakerWalletConfig));
#else
    return BOAT_ERROR;
#endif
    if (index == BOAT_ERROR)
    {
        return BOAT_ERROR;
    }
    
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(index);
    if (g_chaninmaker_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

static BOAT_RESULT test_contrct_invoke_prepara(BoatHlchainmakerTx  *tx_ptr)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare wallet */
    result = ChainmakerWalletPrepare();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test ChainmakerWalletPrepare() failed.");
        return -1;
    }

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test BoatHlChainmakerTxInit() failed.");
        return -1;
    }

    result = BoatHlchainmakerAddTxParam(tx_ptr, 6, "time", "6543235", "file_hash", "ab3456df5799b87c77e7f85", "file_name", "name005");
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test BoatHlchainmakerAddTxParam() failed.");
        return -1;
    }
    return result;
}

static BOAT_RESULT test_contrct_query_prepara(BoatHlchainmakerTx  *tx_ptr)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare wallet */
    result = ChainmakerWalletPrepare();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test ChainmakerWalletPrepare() failed.");
        return -1;
    }

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test BoatHlChainmakerTxInit() failed.");
        return -1;
    }

    result = BoatHlchainmakerAddTxParam(tx_ptr, 2, "file_hash", "ab3456df5799b87c77e7f85");
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "test BoatHlchainmakerAddTxParam() failed.");
        return -1;
    }
    return result;
}
    

START_TEST(test_003Contract_0001InvokeFailureTxNull) 
{
    BOAT_RESULT        result;
    BoatInvokeResponse  invoke_response;

    result = BoatHlchainmakerContractInvoke(NULL, "save", "fact", true, &invoke_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_003Contract_0002InvokeFailureMethodNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatInvokeResponse  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractInvoke(&tx_ptr, NULL, "fact", true, &invoke_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_003Contract_0003InvokeFailureContractNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatInvokeResponse  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractInvoke(&tx_ptr, "save", NULL, true, &invoke_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_003Contract_0004InvokeFailureContractNoExist) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatInvokeResponse  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractInvoke(&tx_ptr, "save", "test", true, &invoke_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {
        ck_assert(invoke_response.code == BOAT_SUCCESS);
        ck_assert(invoke_response.gas_used == 0);
    }
}
END_TEST

START_TEST(test_003Contract_0005InvokeFailureresponseNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractInvoke(&tx_ptr, "save", "test", true, NULL); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_003Contract_0006InvokeSucessSyncOn) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatInvokeResponse  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractInvoke(&tx_ptr, "save", "fact", true, &invoke_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {
        ck_assert(invoke_response.code == BOAT_SUCCESS);
        ck_assert(invoke_response.gas_used != 0);
    }
}
END_TEST

START_TEST(test_003Contract_0007InvokeSucessSyncOff) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatInvokeResponse  invoke_response;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractInvoke(&tx_ptr, "save", "fact", false, &invoke_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {
        ck_assert(invoke_response.code == BOAT_SUCCESS);
        ck_assert(invoke_response.gas_used == 0);
    }
}
END_TEST

START_TEST(test_003Contract_0008QueryFailureTxNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatQueryResponse  query_response;

    result = BoatHlchainmakerContractQuery(NULL, "save", "fact", &query_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_003Contract_0009QueryFailureMethodNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatQueryResponse  query_response;

    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractQuery(&tx_ptr, NULL, "fact", &query_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST


START_TEST(test_003Contract_00010QueryFailureContractNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatQueryResponse   query_response;

    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractQuery(&tx_ptr, "save", NULL, &query_response); ;
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_003Contract_00011QueryFailureContractNoExist) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatQueryResponse  query_response;

    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractQuery(&tx_ptr, "save", "test", &query_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {
        ck_assert(query_response.code != BOAT_SUCCESS);
        ck_assert(query_response.gas_used == 0);
    }
}
END_TEST

START_TEST(test_003Contract_00012QueryFailureResponseNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;


    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractQuery(&tx_ptr, "save", "fact", NULL); 
    ck_assert(result == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_003Contract_00013QuerySucess) 
{
   BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatQueryResponse  query_response;

    result = test_contrct_query_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractQuery(&tx_ptr, "save", "fact", &query_response); ;
    ck_assert(result == BOAT_SUCCESS);
    if (result == BOAT_SUCCESS)
    {
        ck_assert(query_response.code == BOAT_SUCCESS);
        ck_assert(query_response.gas_used != 0);
    }
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
    /* Test cases are added to the test set */
    tcase_add_test(tc_contract_api, test_003Contract_0001InvokeFailureTxNull); 
    tcase_add_test(tc_contract_api, test_003Contract_0002InvokeFailureMethodNull);  
    tcase_add_test(tc_contract_api, test_003Contract_0003InvokeFailureContractNull);  
    tcase_add_test(tc_contract_api, test_003Contract_0004InvokeFailureContractNoExist);  
    tcase_add_test(tc_contract_api, test_003Contract_0005InvokeFailureresponseNull);  
    tcase_add_test(tc_contract_api, test_003Contract_0006InvokeSucessSyncOn); 
    tcase_add_test(tc_contract_api, test_003Contract_0007InvokeSucessSyncOff); 

    tcase_add_test(tc_contract_api, test_003Contract_0008QueryFailureTxNull); 
    tcase_add_test(tc_contract_api, test_003Contract_0009QueryFailureMethodNull); 
    tcase_add_test(tc_contract_api, test_003Contract_00010QueryFailureContractNull); 
    tcase_add_test(tc_contract_api, test_003Contract_00011QueryFailureContractNoExist);
    tcase_add_test(tc_contract_api, test_003Contract_00012QueryFailureResponseNull);
    tcase_add_test(tc_contract_api, test_003Contract_00013QuerySucess);
 
    return s_contract;
}

