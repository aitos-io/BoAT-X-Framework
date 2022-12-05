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
#include "tcase_cita.h"

#define EXCEED_STR_MAX_LEN          4097
#define TEST_IS_SYNC_TX             BOAT_TRUE
static const BCHAR *demoChainIdStr       = "0x1";


BOAT_RESULT check_cita_wallet(BoatCitaWallet *wallet_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    result = strncmp(wallet_ptr->account_info.prikeyCtx.extra_data.value, cita_private_key_buf, strlen(cita_private_key_buf));
    if (result != 0) 
    {
        return result;
    }
    
    result = strncmp(wallet_ptr->network_info.node_url_ptr, TEST_CITA_NODE_URL, strlen(TEST_CITA_NODE_URL));
    if (result != 0)
    {
        return result;
    }

    return BOAT_SUCCESS;
}

BoatCitaWalletConfig get_cita_wallet_settings()
{
    //set user private key context
    BSINT32 index;
    BoatCitaWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]            = {0};
    BUINT8 binChainId[32]              = {0};

    if (TEST_KEY_TYPE == BOAT_WALLET_PRIKEY_FORMAT_NATIVE)
    {
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(binFormatKey, 32, cita_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    }
    else
    {
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)cita_private_key_buf;
        wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(cita_private_key_buf) + 1;
    }

    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;

    wallet_config.chain_id.value_len = UtilityHexToBin(binChainId, 32, demoChainIdStr, TRIMBIN_TRIM_NO, BOAT_FALSE);
    memcpy(wallet_config.chain_id.value, binChainId, 32);
    wallet_config.version = 2;

    strncpy(wallet_config.node_url_str, TEST_CITA_NODE_URL, BOAT_CITA_NODE_URL_MAX_LEN - 1);
    /* create cita wallet */
    return wallet_config;
}

START_TEST(test_001CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatCitaWallet *g_cita_wallet_ptr = NULL;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

     /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);

    g_cita_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_cita_wallet_ptr != NULL);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0002CreateOneTimeWalletFailureNullConfig) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, NULL, sizeof(BoatCitaWalletConfig));
    
    /* 3. verify test result */
    /* 3-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    /* 3-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0003CreatePersistWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatCitaWallet *g_cita_wallet_ptr = NULL;

    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, "cita", &wallet, sizeof(BoatCitaWalletConfig));

    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(access("cita", F_OK), 0);

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, "cita", &wallet, sizeof(BoatCitaWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 1);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);

    g_cita_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_cita_wallet_ptr != NULL);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0004CreateLoadWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatCitaWallet *g_cita_wallet_ptr = NULL;
    BoatIotSdkInit();    
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, "cita", NULL, sizeof(BoatCitaWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);

    g_cita_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_cita_wallet_ptr != NULL);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0005CreateLoadWalletFailureNoExist) 
{
    BSINT32 rtnVal;
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, "cita_no_exist", NULL, sizeof(BoatCitaWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_PERSISTER_READ_FAIL);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0006CreateOneTimeWalletFailureShortSize) 
{
    BSINT32 rtnVal;
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig) - 10);
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0007CreateOneTimeWalletSuccessLongSize) 
{
    BSINT32 rtnVal;
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig) + 10);
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0008CreateOneTimeWalletFailureProtocolUnknown)
{
    BSINT32 rtnVal;
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_UNKNOWN, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0009CreateWalletWithInternalGeneration)
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0010CreateSixWallet)
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 0);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 1);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 2);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 3);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 4);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 5);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0011CreateSevenWallet)
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 0);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 1);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 2);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 3);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 4);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 5);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_ERROR_WALLET_NUM_EXCEED);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0012CreateOnetimeWalletWithLoadExistedWallet)
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.load_existed_wallet = BOAT_TRUE;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0013UnloadWalletSuccess)
{
    BSINT32 rtnVal;
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);

    BoatWalletUnload(0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0014UnloadInexistentWallet)
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);

    BoatWalletUnload(1);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0015CreateSixWalletUnloadOneSuccess)
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 0);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 1);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 2);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 3);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 4);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 5);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);

    BoatWalletUnload(1);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == false);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0016CreateSixWalletUnloadOneCreateOne)
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 0);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 1);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 2);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 3);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 4);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 5);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);

    BoatWalletUnload(3);
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 3);

    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0017CreateSixWalletUnloadTwoCreateOne)
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;
    wallet.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 0);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 1);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 2);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 3);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 4);

    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 5);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);

    BoatWalletUnload(2);
    BoatWalletUnload(4);
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet, sizeof(BoatCitaWalletConfig));
    ck_assert_int_eq(rtnVal, 2);

    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == false);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_001CreateWallet_0018DeletePersistWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatCitaWallet *g_cita_wallet_ptr = NULL;

    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, "cita", &wallet, sizeof(BoatCitaWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    rtnVal = BoatWalletDelete("cita");

    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, "cita", NULL, sizeof(BoatCitaWalletConfig));
    
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_PERSISTER_READ_FAIL);

    /* 2-2. verify the global variables that be affected */
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == false);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0001SetChainIdSuccess)
{
    BSINT32 rtnVal;
    BoatCitaWallet *wallet_ptr = BoatMalloc(sizeof(BoatCitaWallet));
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();

    ck_assert_ptr_ne(wallet_ptr, NULL);
    
    /* 1. execute unit test */
    rtnVal = BoatCitaWalletSetChainId(wallet_ptr, &wallet.chain_id);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 2-2. verify the global variables that be affected */

    ck_assert_int_eq(wallet_ptr->network_info.chain_id_buf[BOAT_CITA_CHAIN_ID_V1_SIZE -1], 0x1);
    BoatFree(wallet_ptr); 
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0002SetChainIdFailureNullParam)
{
    BSINT32 rtnVal;
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatCitaWalletSetChainId(NULL, &wallet.chain_id);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0003SetNodeUrlSuccess)
{
    BSINT32 rtnVal;
    BoatCitaWallet *wallet_ptr = BoatMalloc(sizeof(BoatCitaWallet));
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    
    /* 1. execute unit test */
    wallet_ptr->network_info.node_url_ptr = NULL;
    rtnVal = BoatCitaWalletSetNodeUrl(wallet_ptr, wallet.node_url_str);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* 2-2. verify the global variables that be affected */
    ck_assert_str_eq(wallet_ptr->network_info.node_url_ptr, wallet.node_url_str);
    BoatCitaWalletDeInit(wallet_ptr);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002nitWallet_0004SetNodeUrlFailureNullParam)
{
    BSINT32 rtnVal;
    BoatCitaWallet *wallet_ptr = BoatMalloc(sizeof(BoatCitaWallet));
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    
    /* 1. execute unit test */
    wallet_ptr->network_info.node_url_ptr = NULL;
    rtnVal = BoatCitaWalletSetNodeUrl(NULL, wallet.node_url_str);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.node_url_ptr == NULL);

    /* 1. execute unit test */
    wallet_ptr->network_info.node_url_ptr = NULL;
    rtnVal = BoatCitaWalletSetNodeUrl(wallet_ptr, NULL);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.node_url_ptr == NULL);

    /* 1. execute unit test */
    wallet_ptr->network_info.node_url_ptr = NULL;
    rtnVal = BoatCitaWalletSetNodeUrl(NULL, NULL);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.node_url_ptr == NULL);
    BoatFree(wallet_ptr); 
    BoatSleep(1);
}
END_TEST


START_TEST(test_002InitWallet_0005SetNodeUrlFailureNodeUrlOutOfLimit)
{
    BSINT32 rtnVal;
    BoatCitaWallet *wallet_ptr = BoatMalloc(sizeof(BoatCitaWallet));
    char error_cita_node_url[EXCEED_STR_MAX_LEN];
    
    for (int i = 0; i < EXCEED_STR_MAX_LEN; i++)
    {
        error_cita_node_url[i] = ':';
    }

    /* 1. execute unit test */
    wallet_ptr->network_info.node_url_ptr = NULL;
    rtnVal = BoatCitaWalletSetNodeUrl(wallet_ptr, error_cita_node_url);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    /* 2-2. verify the global variables that be affected */
    ck_assert(wallet_ptr->network_info.node_url_ptr == NULL);
    BoatFree(wallet_ptr);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0006InitCitaWalletWithNullConfig)
{
    BoatCitaWallet *rtnVal;

    /* 1. execute unit test */
    rtnVal = BoatCitaWalletInit(NULL, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_ptr_eq(rtnVal,NULL);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0007InitCitaWalletWithSmallerSize)
{
    BoatCitaWallet *rtnVal;
    BoatCitaWalletConfig walletConfig = {0};

    /* 1. execute unit test */
    rtnVal = BoatCitaWalletInit(&walletConfig, sizeof(BoatCitaWalletConfig) - 1);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_ptr_eq(rtnVal,NULL);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0008InitCitaWalletWithBiggerSize)
{
    BoatCitaWallet *rtnVal;
    BoatCitaWalletConfig walletConfig = {0};

    /* 1. execute unit test */
    rtnVal = BoatCitaWalletInit(&walletConfig, sizeof(BoatCitaWalletConfig) + 1);
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_ptr_eq(rtnVal,NULL);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0009InitCitaWalletGenerationKey)
{
    BoatCitaWallet *rtnVal;
    BoatCitaWalletConfig walletConfig;

    walletConfig.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    walletConfig.prikeyCtx_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    walletConfig.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;

    /* 1. execute unit test */
    rtnVal = BoatCitaWalletInit(&walletConfig, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_ptr_ne(rtnVal,NULL);
    BoatCitaWalletDeInit(rtnVal);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0010InitCitaWalletWithWrongGenMode)
{
    BoatCitaWallet *rtnVal;
    BoatCitaWalletConfig walletConfig;

    walletConfig.prikeyCtx_config.prikey_genMode = 3;
    walletConfig.prikeyCtx_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    walletConfig.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    walletConfig.load_existed_wallet = BOAT_FALSE;

    /* 1. execute unit test */
    rtnVal = BoatCitaWalletInit(&walletConfig, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_ptr_eq(rtnVal,NULL);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0011InitCitaWalletWithWrongKeyFormat)
{
    BoatCitaWallet *rtnVal;
    BoatCitaWalletConfig walletConfig;

    walletConfig.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    walletConfig.prikeyCtx_config.prikey_format = 4;
    walletConfig.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    walletConfig.load_existed_wallet = BOAT_FALSE;

    /* 1. execute unit test */
    rtnVal = BoatCitaWalletInit(&walletConfig, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_ptr_eq(rtnVal, NULL);
    BoatSleep(1);
}
END_TEST

START_TEST(test_002InitWallet_0012InitCitaWalletWithWrongType)
{
    BoatCitaWallet *rtnVal;
    BoatCitaWalletConfig walletConfig;

    walletConfig.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    walletConfig.prikeyCtx_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    walletConfig.prikeyCtx_config.prikey_type = 3;
    walletConfig.load_existed_wallet = BOAT_FALSE;

    /* 1. execute unit test */
    rtnVal = BoatCitaWalletInit(&walletConfig, sizeof(BoatCitaWalletConfig));
    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_ptr_eq(rtnVal,NULL);
    BoatSleep(1);
}
END_TEST

START_TEST(test_003DeleteWallet_0001DeleteWalletFailureNullFleName) 
{
    BSINT32 rtnVal;

    BoatIotSdkInit();
    BoatCitaWalletConfig wallet = get_cita_wallet_settings();
    extern BoatIotSdkContext g_boat_iot_sdk_context;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_CITA, "cita", &wallet, sizeof(BoatCitaWalletConfig));

    /* 2. verify test result */
    /* 2-1. verify the return value */
    ck_assert_int_eq(rtnVal, 0);

    BoatWalletDelete(NULL);
    ck_assert_int_eq(access("cita", F_OK), 0);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_003DeleteWallet_0002DeleteWalletFailureNoExistingFile) 
{
    BoatWalletDelete("cita_no_exist");
    ck_assert_int_eq(access("cita", F_OK), 0);
    BoatSleep(1);
}
END_TEST

START_TEST(test_003DeleteWallet_0003DeleteWalletSucessExistingFile) 
{
    BoatWalletDelete("cita");
    ck_assert_int_eq(access("cita", F_OK), -1);
    BoatSleep(1);
}
END_TEST

Suite *make_wallet_suite(void) 
{
    /* Create Suite */
    Suite *s_wallet = suite_create("wallet");

    /* Create test cases */
    TCase *tc_wallet_api = tcase_create("wallet_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_wallet, tc_wallet_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0001CreateOneTimeWalletSuccess);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0002CreateOneTimeWalletFailureNullConfig);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0003CreatePersistWalletSuccess);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0004CreateLoadWalletSuccess);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0005CreateLoadWalletFailureNoExist);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0006CreateOneTimeWalletFailureShortSize);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0007CreateOneTimeWalletSuccessLongSize);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0008CreateOneTimeWalletFailureProtocolUnknown);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0009CreateWalletWithInternalGeneration);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0010CreateSixWallet);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0011CreateSevenWallet);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0012CreateOnetimeWalletWithLoadExistedWallet);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0013UnloadWalletSuccess);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0014UnloadInexistentWallet);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0015CreateSixWalletUnloadOneSuccess);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0016CreateSixWalletUnloadOneCreateOne);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0017CreateSixWalletUnloadTwoCreateOne);  
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0018DeletePersistWalletSuccess);  

    tcase_add_test(tc_wallet_api, test_002InitWallet_0001SetChainIdSuccess);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0002SetChainIdFailureNullParam);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0003SetNodeUrlSuccess);  
    tcase_add_test(tc_wallet_api, test_002nitWallet_0004SetNodeUrlFailureNullParam);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0005SetNodeUrlFailureNodeUrlOutOfLimit);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0006InitCitaWalletWithNullConfig);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0007InitCitaWalletWithSmallerSize);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0008InitCitaWalletWithBiggerSize);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0009InitCitaWalletGenerationKey);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0010InitCitaWalletWithWrongGenMode);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0011InitCitaWalletWithWrongKeyFormat);  
    tcase_add_test(tc_wallet_api, test_002InitWallet_0012InitCitaWalletWithWrongType);  

    tcase_add_test(tc_wallet_api, test_003DeleteWallet_0001DeleteWalletFailureNullFleName);  
    tcase_add_test(tc_wallet_api, test_003DeleteWallet_0002DeleteWalletFailureNoExistingFile);  
    tcase_add_test(tc_wallet_api, test_003DeleteWallet_0003DeleteWalletSucessExistingFile);  

    return s_wallet;
}

