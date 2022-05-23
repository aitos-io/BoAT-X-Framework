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
#define EXCEED_STR_MAX_LEN 4097
extern BoatIotSdkContext g_boat_iot_sdk_context;

BoatHlfabricWalletConfig get_fabric_wallet_settings();

START_TEST(test_002Transaction_0001TXInit_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0002TxInit_tx_ptr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(NULL, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0003TxInit_Wallet_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, NULL, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0004TxInit_chainCodeIDName_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, NULL, NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0005TxInit_channelID_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, NULL, "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST


START_TEST(test_002Transaction_0006TxInit_orgName_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0007SetNetworkInfoSuccess) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST


START_TEST(test_002Transaction_0008TxSetNetworkInfo_Wallet_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(NULL, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0009TxSetNetworkInfo_NodesCfg_layoutNumIs0) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    wallet_config.nodesCfg.endorserLayoutNum = 0;
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0010TxSetNetworkInfo_NodesCfg_orderCfg_NumIs0) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    wallet_config.nodesCfg.orderCfg.endorserNumber = 0;
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0011TxSetTimestamp_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0012TxSetTimestamp_TxPtr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(NULL, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0013TxSetArgs_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0014TxSetArgs_Txptr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(NULL, "invoke", "a", "b", "10", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0015TxSetArgs_argsMoreThanLimit) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", "c","d","e","f","c","d","e","f");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_OUT_OF_MEMORY);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0016TxInvoke_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0017TxInvokeFail_Txptr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0018TxInvokeFail_Args_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0019TxInvokeFail_Args_Lack1) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0020TxInvokeFail_Args_Lack2) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0021TxInvokeFail_Args_Lack3) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0022TxInvokeFail_Args_ADD1) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0023TxQuery_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0024TxQueryFail_Txptr_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0025TxQueryFail_Args_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0026TxQueryFail_Method_ERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0027TxQueryFail_arg2_ERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0028TxQueryFail_arg2_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_002Transaction_0029TxQueryFail_args_ADD1) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);

    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
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
    fabricWalletConfigFree(wallet_config);
}
END_TEST


Suite *make_transaction_suite(void) 
{
    /* Create Suite */
    Suite *s_transaction = suite_create("transaction");

    /* Create test cases */
    TCase *tc_transaction_api = tcase_create("transaction_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_transaction, tc_transaction_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_transaction_api, test_002Transaction_0001TXInit_Success);
    tcase_add_test(tc_transaction_api, test_002Transaction_0002TxInit_tx_ptr_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0003TxInit_Wallet_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0004TxInit_chainCodeIDName_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0005TxInit_channelID_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0006TxInit_orgName_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0007SetNetworkInfoSuccess);
    tcase_add_test(tc_transaction_api, test_002Transaction_0008TxSetNetworkInfo_Wallet_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0009TxSetNetworkInfo_NodesCfg_layoutNumIs0);
    tcase_add_test(tc_transaction_api, test_002Transaction_0010TxSetNetworkInfo_NodesCfg_orderCfg_NumIs0);
    tcase_add_test(tc_transaction_api, test_002Transaction_0011TxSetTimestamp_Success);
    tcase_add_test(tc_transaction_api, test_002Transaction_0012TxSetTimestamp_TxPtr_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0013TxSetArgs_Success);
    tcase_add_test(tc_transaction_api, test_002Transaction_0014TxSetArgs_Txptr_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0015TxSetArgs_argsMoreThanLimit);
    tcase_add_test(tc_transaction_api, test_002Transaction_0016TxInvoke_Success);
    tcase_add_test(tc_transaction_api, test_002Transaction_0017TxInvokeFail_Txptr_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0018TxInvokeFail_Args_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0019TxInvokeFail_Args_Lack1);
    tcase_add_test(tc_transaction_api, test_002Transaction_0020TxInvokeFail_Args_Lack2);
    tcase_add_test(tc_transaction_api, test_002Transaction_0021TxInvokeFail_Args_Lack3);
    tcase_add_test(tc_transaction_api, test_002Transaction_0022TxInvokeFail_Args_ADD1);
    tcase_add_test(tc_transaction_api, test_002Transaction_0023TxQuery_Success);
    tcase_add_test(tc_transaction_api, test_002Transaction_0024TxQueryFail_Txptr_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0025TxQueryFail_Args_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0026TxQueryFail_Method_ERR);
    tcase_add_test(tc_transaction_api, test_002Transaction_0027TxQueryFail_arg2_ERR);
    tcase_add_test(tc_transaction_api, test_002Transaction_0028TxQueryFail_arg2_NULL);
    tcase_add_test(tc_transaction_api, test_002Transaction_0029TxQueryFail_args_ADD1);

    return s_transaction;
}

