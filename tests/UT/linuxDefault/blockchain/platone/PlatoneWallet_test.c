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
#include "tcase_platone.h"

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
static BoatPlatoneWallet *g_platone_wallet_ptr;

/**
 * test node url
 */
const BCHAR *demoUrl = "http://116.236.47.90:7545";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";

/**
 * native demo key
 */
const BCHAR *native_demoKey = "0xf1395a1fc3f74f0c867b61292e28e0f6cc98a095535fd6bf04e4169ebc047e61";
const BUINT8 pubkey256K1[] = {0xC8, 0x00,0xAE, 0xD2, 0x61, 0x2C, 0xB2, 0xB6, 0x2F, 0x4C, 0xED, 0x0C, 0x8E, 0xD5, 0xDF, 0xE7, 0x49, 0xE5, 0x17, 0xC2, 0xCF, 0x1B, 0x31, 0x9C, 0x14, 0xC3, 0x1C, 0x2C, 0x44, 0xE6, 0xA2, 0x9D, 0xEA, 0x68, 0xC3, 0x84, 0x65, 0x02, 0x3E, 0x9C, 0x65, 0xD3, 0x64, 0x02, 0xDA, 0x5E, 0xE4, 0x24, 0xAC, 0x29, 0x55, 0xD9, 0x1A, 0x30, 0xA5, 0x0d, 0x98, 0xe0, 0x94, 0xb4, 0xC2, 0xE9, 0x25, 0x72};
BUINT8 binFormatKey[32] = {0};

/**
****************************************************************************************
* @brief:
*  This function check network data reading by index or list.
* @param[in] networkData
*  network data read from memory
* @param[in] networkCfg
*  network configuration to creat the network data
* @param[in] expectedIndex
*  index of created network
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT platone_createNetwork(BBOOL is_onetime, BUINT8* networkIndex)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatPlatoneNetworkConfig network_config = {0};

    network_config.chain_id             = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    /* create ciat wallet */
    if (is_onetime)
    {
        result = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    }
    else
    {
        result = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    }

    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *networkIndex = result;
    return BOAT_SUCCESS;
}

BOAT_RESULT platone_createKeypair_native(BCHAR * keypairName, BBOOL is_onetime, BUINT8* keypairIndex_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = binFormatKey;
    keypair_config.prikey_content.field_len = 32;

    /* create cita keypair */
    if (is_onetime)
    {
        result = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_RAM);
    }
    else
    {
        result = BoatKeypairCreate(&keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
    }

    if (result < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *keypairIndex_ptr = result;
    
    return BOAT_SUCCESS;
}


/**
****************************************************************************************
* @brief:
*  This function test case : create onetime keypair success
* @return
****************************************************************************************
*/
START_TEST(test_003CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = platone_createKeypair_native("keypairOnetime", BOAT_TRUE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 0);

    rtnVal = platone_createNetwork(BOAT_TRUE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 0);
   
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_ne(g_platone_wallet_ptr, NULL);

    // check keypair
    ck_assert_int_eq(g_platone_wallet_ptr->account_info.prikeyCtx.keypair_index, 0);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(g_platone_wallet_ptr->account_info.prikeyCtx.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(g_platone_wallet_ptr->account_info.prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(g_platone_wallet_ptr->account_info.prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);

    /* check public key in the struct */
    ck_assert_int_eq(memcmp(g_platone_wallet_ptr->account_info.prikeyCtx.pubkey_content, pubkey256K1, sizeof(pubkey256K1)), 0);

    // check network
    ck_assert_int_eq(g_platone_wallet_ptr->network_info.networkIndex, 0);
    /* check chainId in the struct , must be same to the second configuration */
    ck_assert_int_eq(g_platone_wallet_ptr->network_info.chain_id, 1);
    /* check eip155 flag */
    ck_assert(g_platone_wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);
    /* check url in the struct , must be same to the second configuration */
    ck_assert_int_eq(strcmp(g_platone_wallet_ptr->network_info.node_url_str, demoUrl), 0);

    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0002CreateOneTimeWalletFailureWithoutKeypair) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = platone_createNetwork(BOAT_TRUE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 0);
   
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_platone_wallet_ptr, NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0003CreateOneTimeWalletFailureWithoutNetwork) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = platone_createKeypair_native("keypairOnetime", BOAT_TRUE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 0);
   
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_platone_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0004CreatePersistWalletSuccess) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = platone_createKeypair_native("keypairOnetime", BOAT_FALSE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 1);

    rtnVal = platone_createNetwork(BOAT_FALSE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 1);
   
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_ne(g_platone_wallet_ptr, NULL);

    ck_assert_int_eq(g_platone_wallet_ptr->account_info.prikeyCtx.keypair_index, 1);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(g_platone_wallet_ptr->account_info.prikeyCtx.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(g_platone_wallet_ptr->account_info.prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(g_platone_wallet_ptr->account_info.prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    // /* check public key in the struct */
    ck_assert_int_eq(memcmp(g_platone_wallet_ptr->account_info.prikeyCtx.pubkey_content, pubkey256K1, sizeof(pubkey256K1)), 0);

    // check network
    ck_assert_int_eq(g_platone_wallet_ptr->network_info.networkIndex, 1);
    /* check chainId in the struct , must be same to the second configuration */
    ck_assert_int_eq(g_platone_wallet_ptr->network_info.chain_id, 1);
    /* check eip155 flag */
    ck_assert(g_platone_wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);
    /* check url in the struct , must be same to the second configuration */
    ck_assert_int_eq(strcmp(g_platone_wallet_ptr->network_info.node_url_str, demoUrl), 0);

    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATPlatoneNetworkDelete(1);
    BoATIotKeypairDelete(1);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0005CreatePersistWalletFailureWithoutKeypair) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = platone_createNetwork(BOAT_FALSE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 1);
   
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_platone_wallet_ptr, NULL);

    /* free configuration struct */
    BoATPlatoneNetworkDelete(1);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0006CreatePersistWalletFailureWithoutNetwork) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = platone_createKeypair_native("keypairOnetime", BOAT_FALSE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 1);
   
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_platone_wallet_ptr, NULL);
    BoATIotKeypairDelete(1);
    BoatIotSdkDeInit();
}
END_TEST

Suite *make_platoneWalletTest_suite(void)
{
    /* Create Suite */
    Suite *s_walletInit = suite_create("walletInit");

    /* Create test cases */
    TCase *tc_walletInit_api = tcase_create("walletInit_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_walletInit, tc_walletInit_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0001CreateOneTimeWalletSuccess);  
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0002CreateOneTimeWalletFailureWithoutKeypair); 
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0003CreateOneTimeWalletFailureWithoutNetwork);

    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0004CreatePersistWalletSuccess);
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0005CreatePersistWalletFailureWithoutKeypair); 
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0006CreatePersistWalletFailureWithoutNetwork);
    return s_walletInit;
}