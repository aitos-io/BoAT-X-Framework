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

BoatHlfabricWallet *fabric_get_wallet_ptr()
{
	BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatHlfabricWallet *g_fabric_wallet_ptr;
    BoatHlfabricNetworkConfig networkConfig;
    BoatIotSdkInit();

	rtnVal = fabric_createKeypair("keypairOnetime", BOAT_TRUE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 0);

    rtnVal = fabric_createNetwork(BOAT_TRUE, &networkIndex, &networkConfig);
    fabricWalletConfigFree(networkConfig);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 0);
   
    g_fabric_wallet_ptr = BoatHlfabricWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_ne(g_fabric_wallet_ptr, NULL);
    return g_fabric_wallet_ptr;
}
  
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

START_TEST(test_004ParmsInit_0002TxInit_Failure_tx_ptr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(NULL, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    // BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0003TxInit_Failure_Wallet_NULL) 
{
   	BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, NULL, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    //BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0004TxInit_Failure_chainCodeIDName_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, NULL, NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    // BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0005TxInit_Failure_channelID_NULL) 
{
   	BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, NULL, "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    // BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_004ParmsInit_0006TxInit_Failure_orgName_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    // BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0007TxSetTimestamp_Success) 
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
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0008TxSetTimestamp_TxPtr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
 
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(NULL, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0009TxSetArgs_Success) 
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
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0010TxSetArgs_Txptr_NULL) 
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
    rtnVal = BoatHlfabricTxSetArgs(NULL, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0011TxSetArgs_argsMoreThanLimit) 
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
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", "c","d","e","f","c","d","e","f");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_OUT_OF_MEMORY);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0012TxSetArgs_Success_argsReachMaxLimit) 
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
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", "c","d","e","f","c","d", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0013TxSetArgs_Failure_argsMoreThanLimitOne) 
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
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", "c","d","e","f","c","d","e", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_OUT_OF_MEMORY);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0014TxSetArgs_Success_argsLessThanLimitOne) 
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
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", "c","d","e","f","c", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParmsInit_0015TxSetArgs_Success_argsNull) 
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
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, NULL, NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

Suite *make_fabricParamsTest_suite(void)
{
    /* Create Suite */
    Suite *s_paramsInit = suite_create("paramsInit");

    /* Create test cases */
    TCase *tc_paramsInit_api = tcase_create("paramsInit_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_paramsInit, tc_paramsInit_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0001TXInit_Success); 
 	tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0002TxInit_Failure_tx_ptr_NULL); 
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0003TxInit_Failure_Wallet_NULL);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0004TxInit_Failure_chainCodeIDName_NULL);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0005TxInit_Failure_channelID_NULL); 
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0006TxInit_Failure_orgName_NULL);

    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0007TxSetTimestamp_Success);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0008TxSetTimestamp_TxPtr_NULL);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0009TxSetArgs_Success);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0010TxSetArgs_Txptr_NULL);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0011TxSetArgs_argsMoreThanLimit);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0012TxSetArgs_Success_argsReachMaxLimit);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0013TxSetArgs_Failure_argsMoreThanLimitOne);
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0014TxSetArgs_Success_argsLessThanLimitOne);    
    tcase_add_test(tc_paramsInit_api, test_004ParmsInit_0015TxSetArgs_Success_argsNull);    
    return s_paramsInit;
}