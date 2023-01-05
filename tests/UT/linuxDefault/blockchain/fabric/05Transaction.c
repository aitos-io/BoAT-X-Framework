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
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, NULL, NULL);
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

START_TEST(test_005Transaction_0008TxInvoke_Failure_Args_nameErr) 
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
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "test", "a", "b", "10","c", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0009TxInvoke_Success_Args_timeZero) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    //time(&timesec);
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

START_TEST(test_005Transaction_0010TxInvoke_Failure_Args_chaincodeId_name_err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycctest", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0011TxInvoke_Failure_Args_channelId_err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychanneltest", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005Transaction_0012TxInvoke_Failure_Args_orgName_err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychanneltest", "Org1MSPtest");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0013TxInvoke_Failure_Walleturl_Err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    BoatFree(g_fabric_wallet_ptr->network_info.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl);
    g_fabric_wallet_ptr->network_info.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = NULL;

    BCHAR *fabric_demo_endorser_peer0Org1_url_err = "121.4.178.74";
    g_fabric_wallet_ptr->network_info.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url_err) + 1);
    strcpy(g_fabric_wallet_ptr->network_info.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org1_url_err);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
  
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSubmit(&tx_ptr);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0014TxInvoke_Success_WalletHostName_Err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    BoatFree(g_fabric_wallet_ptr->network_info.nodesCfg.orderCfg.endorser[0].hostName);
    g_fabric_wallet_ptr->network_info.nodesCfg.orderCfg.endorser[0].hostName = NULL;

    BCHAR *fabric_demo_order1_hostName_err = "peer0.org1.com111";
    g_fabric_wallet_ptr->network_info.nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric_demo_order1_hostName_err) + 1);
    strcpy(g_fabric_wallet_ptr->network_info.nodesCfg.orderCfg.endorser[0].hostName, fabric_demo_order1_hostName_err);

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



START_TEST(test_005Transaction_0015TxQuery_Success) 
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
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0016TxQuery_Failure_Txptr_NULL) 
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
    rtnVal = BoatHlfabricTxEvaluate(NULL); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0017TxQuery_Failure_Args_NULL) 
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
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0018TxQuery_Failure_Method_ERR) 
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
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0019TxQuery_Failure_arg2_ERR) 
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
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0020TxQuery_Failure_arg2_NULL) 
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
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0021TxQuery_Failure_args_ADD1) 
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
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0022TxQuery_Success_Args_timeZero) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
 
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, 0, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0023TxQuery_Failure_Args_chaincodeId_name_err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycctest", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
 
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005Transaction_0024TxQuery_Failure_Args_channelId_err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychanneltest", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
 
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0025TxQuery_Failure_Args_orgName_err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSPtest");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
 
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0026TxQuery_Failure_Walleturl_Err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    BoatFree(g_fabric_wallet_ptr->network_info.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl);
    g_fabric_wallet_ptr->network_info.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = NULL;

    BCHAR *fabric_demo_endorser_peer0Org1_url_err = "121.4.178.74";
    g_fabric_wallet_ptr->network_info.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url_err) + 1);
    strcpy(g_fabric_wallet_ptr->network_info.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org1_url_err);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
 
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0027TxQuery_Success_WalletHostName_Err) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    g_fabric_wallet_ptr = fabric_get_wallet_ptr();

    BoatFree(g_fabric_wallet_ptr->network_info.nodesCfg.orderCfg.endorser[0].hostName);
    g_fabric_wallet_ptr->network_info.nodesCfg.orderCfg.endorser[0].hostName = NULL;

    BCHAR *fabric_demo_order1_hostName_err = "peer0.org1.com111";
    g_fabric_wallet_ptr->network_info.nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric_demo_order1_hostName_err) + 1);
    strcpy(g_fabric_wallet_ptr->network_info.nodesCfg.orderCfg.endorser[0].hostName, fabric_demo_order1_hostName_err);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
 
    long int timesec = 0;
    time(&timesec);
    rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxEvaluate(&tx_ptr); 
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_005Transaction_0028DeInit_Txptr_NULL) 
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
    tcase_add_test(tc_transaction_api, test_005Transaction_0008TxInvoke_Failure_Args_nameErr);
    tcase_add_test(tc_transaction_api, test_005Transaction_0009TxInvoke_Success_Args_timeZero);
    tcase_add_test(tc_transaction_api, test_005Transaction_0010TxInvoke_Failure_Args_chaincodeId_name_err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0011TxInvoke_Failure_Args_channelId_err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0012TxInvoke_Failure_Args_orgName_err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0013TxInvoke_Failure_Walleturl_Err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0014TxInvoke_Success_WalletHostName_Err);

    tcase_add_test(tc_transaction_api, test_005Transaction_0015TxQuery_Success);
    tcase_add_test(tc_transaction_api, test_005Transaction_0016TxQuery_Failure_Txptr_NULL);
    tcase_add_test(tc_transaction_api, test_005Transaction_0017TxQuery_Failure_Args_NULL);
    tcase_add_test(tc_transaction_api, test_005Transaction_0018TxQuery_Failure_Method_ERR);
    tcase_add_test(tc_transaction_api, test_005Transaction_0019TxQuery_Failure_arg2_ERR);
    tcase_add_test(tc_transaction_api, test_005Transaction_0020TxQuery_Failure_arg2_NULL);
    tcase_add_test(tc_transaction_api, test_005Transaction_0021TxQuery_Failure_args_ADD1);
    tcase_add_test(tc_transaction_api, test_005Transaction_0022TxQuery_Success_Args_timeZero);
    tcase_add_test(tc_transaction_api, test_005Transaction_0023TxQuery_Failure_Args_chaincodeId_name_err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0024TxQuery_Failure_Args_channelId_err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0025TxQuery_Failure_Args_orgName_err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0026TxQuery_Failure_Walleturl_Err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0027TxQuery_Success_WalletHostName_Err);
    tcase_add_test(tc_transaction_api, test_005Transaction_0028DeInit_Txptr_NULL);
    
    return s_transaction;
}