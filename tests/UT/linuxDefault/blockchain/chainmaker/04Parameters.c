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

BOAT_RESULT test_chainmaker_wallet_prepare(BoatChainmakerWallet** g_chaninmaker_wallet_ptr)
{
     /* 1. execute unit test */
    BUINT8 keypair_index;
    BUINT8 network_index;
    BOAT_RESULT result;

    result = test_chainmaker_create_keypair("keypairOnetime", BOAT_TRUE, &keypair_index);

    if (result != BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }
    result = test_chainmaker_create_network(BOAT_TRUE, &network_index);
    if (result != BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }

    *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    if (g_chaninmaker_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }
    return BOAT_SUCCESS;
}

START_TEST(test_004ParametersInit_0001TxinitSuccess) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx tx_ptr;
    BoatChainmakerWallet* g_chaninmaker_wallet_ptr;
    BoatIotSdkInit();
    rtnVal = test_chainmaker_wallet_prepare(&g_chaninmaker_wallet_ptr);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    ck_assert(g_chaninmaker_wallet_ptr != NULL);
    
    rtnVal = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, &tx_ptr, 100);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    ck_assert_ptr_eq(tx_ptr.wallet_ptr, g_chaninmaker_wallet_ptr);

    ck_assert_int_eq(tx_ptr.gas_limit, 100);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0002TxinitxFailureNullpara) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx    tx_ptr;
    BoatChainmakerWallet* g_chaninmaker_wallet_ptr;
    rtnVal = test_chainmaker_wallet_prepare(&g_chaninmaker_wallet_ptr);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatChainmakerTxInit(NULL, &tx_ptr, 100);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    rtnVal = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, NULL, 100);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    rtnVal = BoatChainmakerTxInit(NULL, NULL, 100);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_004ParametersInit_0003AddTxParamSuccess) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx tx_ptr;

    rtnVal = BoatChainmakerAddTxParam(&tx_ptr, 6, "key1", "value1", "key2", "value2", 
                                                  "key3", "value3", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[0].key,             "key1"),   0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[0].value.field_ptr, "value1"), 0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[1].key,             "key2"),   0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[1].value.field_ptr, "value2"), 0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[2].key,             "key3"),   0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[2].value.field_ptr, "value3"), 0);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_004ParametersInit_0004AddTxParamFailureShortParams) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx tx_ptr;

    rtnVal = BoatChainmakerAddTxParam(&tx_ptr, 6, "key1", "value1", "key2", "value2", "key3", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0005AddTxParamFailureLongParams) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx tx_ptr;

    rtnVal = BoatChainmakerAddTxParam(&tx_ptr, 12, "key1", "value1", "key2", "value2", "key3", "value3", 
                                                     "key4", "value4", "key5", "value5", "key6", "value6", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0006AddTxParamFailureOddParams) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx tx_ptr;

    rtnVal = BoatChainmakerAddTxParam(&tx_ptr, 9, "key1", "value1", "key2", "value2", "key3", "value3", 
                                                     "key4", "value4", "key5", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0007AddTxParamSucessNumberNULLParams) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx    tx_ptr;

    rtnVal = BoatChainmakerAddTxParam(&tx_ptr, 0, NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    ck_assert_int_eq(tx_ptr.trans_para.n_parameters, 0);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0008AddTxParamFailureTxNULLParams) 
{
    BSINT32 rtnVal;

    rtnVal = BoatChainmakerAddTxParam(NULL, 6, "key1", "vlaue1", "key2", "vlaue2", "key3", "value3", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0009AddTxParamSucessTxLessMaxEvenParams) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx    tx_ptr;

    rtnVal = BoatChainmakerAddTxParam(&tx_ptr, 8, "key1", "value1", "key2", "value2", "key3", "value3", 
                                                    "key4", "value4",  NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[0].key,             "key1"),   0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[0].value.field_ptr, "value1"), 0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[1].key,             "key2"),   0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[1].value.field_ptr, "value2"), 0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[2].key,             "key3"),   0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[2].value.field_ptr, "value3"), 0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[3].key,             "key4"),   0);
    ck_assert_int_eq(strcmp(tx_ptr.trans_para.parameters[3].value.field_ptr, "value4"), 0);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0010AddTxParamFailureMoreMaxOddParams) 
{
    BSINT32 rtnVal;
    BoatChainmakerTx    tx_ptr;

    rtnVal = BoatChainmakerAddTxParam(&tx_ptr, 11, "key1", "value1", "key2", "value2", "key3", "value3", 
                                                     "key4", "value4", "key5", "value5", "key6", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST


Suite *make_params_suite(void) 
{
    /* Create Suite */
    Suite *s_param = suite_create("param");

    /* Create test cases */
    TCase *tc_param_api = tcase_create("param_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_param, tc_param_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_param_api, test_004ParametersInit_0001TxinitSuccess);  
    tcase_add_test(tc_param_api, test_004ParametersInit_0002TxinitxFailureNullpara);  
    tcase_add_test(tc_param_api, test_004ParametersInit_0003AddTxParamSuccess);  
    tcase_add_test(tc_param_api, test_004ParametersInit_0004AddTxParamFailureShortParams);  
    tcase_add_test(tc_param_api, test_004ParametersInit_0005AddTxParamFailureLongParams); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0006AddTxParamFailureOddParams);   
    tcase_add_test(tc_param_api, test_004ParametersInit_0007AddTxParamSucessNumberNULLParams);  
    tcase_add_test(tc_param_api, test_004ParametersInit_0008AddTxParamFailureTxNULLParams); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0009AddTxParamSucessTxLessMaxEvenParams);  
    tcase_add_test(tc_param_api, test_004ParametersInit_0010AddTxParamFailureMoreMaxOddParams);  

    return s_param;
}

