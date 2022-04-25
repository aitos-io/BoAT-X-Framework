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
    

    return s_transaction;
}

