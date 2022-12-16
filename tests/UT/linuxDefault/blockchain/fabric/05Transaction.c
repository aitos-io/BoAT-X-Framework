/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-04 11:28:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-13 14:37:50
 */
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
#include "tcase_fabric.h"

  
START_TEST(test_004ParmsInit_0001TXInit_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0001TxInvoke_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0002TxInvoke_Failure_Txptr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(NULL); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0003TxInvoke_Failure_Args_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
   
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0004TxInvoke_Failure_Args_Lack1) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0005TxInvoke_Failure_Args_Lack2) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
   
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a",  NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0006TxInvoke_Failure_Args_Lack3) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
   
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke",  NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0007TxInvoke_Failure_Args_ADD1) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10","c", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0008TxQuery_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
 
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0009TxQuery_Failure_Txptr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(NULL); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0010TxQuery_Failure_Args_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, NULL, NULL, NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0011TxQuery_Failure_Method_ERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
   
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query_test", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0012TxQuery_Failure_arg2_ERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
   
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "c", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0013TxQuery_Failure_arg2_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", NULL, NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0014TxQuery_Failure_args_ADD1) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a","b", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0015DeInit_Txptr_NULL) 
{
    BoatIotSdkInit();
    BoatHlfabricTxDeInit(NULL);
    BoatIotSdkDeInit();
}
END_TEST

Suite *make_fabricTransactionTest_suite(void)
{
    /* Create Suite */
    Suite *s_transaction = suite_create("transaction");

    /* Create test cases */
    TCase *tc_transaction_api = tcase_create("transaction_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_transaction, tc_transaction_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_transaction_api, test_005Transaction_0001TxInvoke_Success); 
 	tcase_add_test(tc_transaction_api, test_005Transaction_0002TxInvoke_Failure_Txptr_NULL); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0003TxInvoke_Failure_Args_NULL);
    tcase_add_test(tc_transaction_api, test_005Transaction_0004TxInvoke_Failure_Args_Lack1);
    tcase_add_test(tc_transaction_api, test_005Transaction_0005TxInvoke_Failure_Args_Lack2); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0006TxInvoke_Failure_Args_Lack3);
    tcase_add_test(tc_transaction_api, test_005Transaction_0007TxInvoke_Failure_Args_ADD1);

    tcase_add_test(tc_transaction_api, test_005Transaction_0008TxQuery_Success);
    tcase_add_test(tc_transaction_api, test_005Transaction_0009TxQuery_Failure_Txptr_NULL);
    tcase_add_test(tc_transaction_api, test_005Transaction_0010TxQuery_Failure_Args_NULL);
    tcase_add_test(tc_transaction_api, test_005Transaction_0011TxQuery_Failure_Method_ERR);
    tcase_add_test(tc_transaction_api, test_005Transaction_0012TxQuery_Failure_arg2_ERR);
    tcase_add_test(tc_transaction_api, test_005Transaction_0013TxQuery_Failure_arg2_NULL);
    tcase_add_test(tc_transaction_api, test_005Transaction_0014TxQuery_Failure_args_ADD1);
    tcase_add_test(tc_transaction_api, test_005Transaction_0015DeInit_Txptr_NULL);
    
    return s_transaction;
}