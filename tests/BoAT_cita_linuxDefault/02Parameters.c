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
#define TEST_CITA_CHAIN_ID          1
#define TEST_IS_SYNC_TX             BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS      "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45"
static const BCHAR *demoChainIdStr       = "0x1";
static const BUINT64 quota_limit_value   = 10000000;

BoatCitaWallet *g_cita_wallet_ptr;
BoatCitaWalletConfig wallet_config = {0};

BOAT_RESULT citaWalletPrepare()
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
    index = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet_config, sizeof(BoatCitaWalletConfig));

    if (index == BOAT_ERROR)
    {
        return BOAT_ERROR;
    }
    
    g_cita_wallet_ptr = BoatGetWalletByIndex(index);
    if (g_cita_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

START_TEST(test_004ParametersInit_0001TxInitSuccess)
{
    BOAT_RESULT rtnVal;
    BoatCitaTx tx_ptr = {0};
    BoatIotSdkInit();

    rtnVal = citaWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_004ParametersInit_0002TxInitFailureNullParam)
{
    BSINT32 rtnVal;
    BoatCitaTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = citaWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatCitaTxInit(NULL, &tx_ptr, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    rtnVal = BoatCitaTxInit(g_cita_wallet_ptr, NULL, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
                   
    rtnVal = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, NULL, quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    rtnVal = BoatCitaTxInit(NULL, NULL, TEST_IS_SYNC_TX, NULL, quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST


START_TEST(test_004ParametersInit_0003TxInitFailureRecipientErrorHexFormat)
{
    BSINT32 rtnVal;
    BoatCitaTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = citaWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, "0xABCDG", quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_004ParametersInit_0004TxInitFailureRecipientLongLength)
{
    BSINT32 rtnVal;
    BoatCitaTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = citaWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, "0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_005ParametersGet_0001GetBlockerSuccess)
{
    BOAT_RESULT result;
    BoatCitaTx tx_ptr;
    BUINT64 valid_until_block_value;
    BoatIotSdkInit();

    result = citaWalletPrepare();
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatCitaGetBlockNumber(&tx_ptr, &valid_until_block_value);
    ck_assert_int_eq(result, BOAT_SUCCESS);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_005ParametersGet_0002GetBlockerFailureNullTx)
{
    BOAT_RESULT result;
    BoatCitaTx tx_ptr;
    BUINT64 valid_until_block_value;
    BoatIotSdkInit();

    result = BoatCitaGetBlockNumber(NULL, &valid_until_block_value);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    result = BoatCitaGetBlockNumber(&tx_ptr, NULL);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_005ParametersSet_0003SetValueSuccess)
{
    BSINT32 rtnVal;
    BoatCitaTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = citaWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    BoatFieldMax32B value;
    value.field_len = UtilityHexToBin(value.field, 32, "0x2386F26FC10000",
                                      TRIMBIN_LEFTTRIM, BOAT_TRUE);
    rtnVal = BoatCitaTxSetValue(&tx_ptr, &value);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    ck_assert_str_eq(tx_ptr.rawtx_fields.value.field, value.field);
    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_005ParametersSet_0004SetValueFailureNullTx)
{
    BSINT32 rtnVal;
    BoatCitaTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = citaWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    ck_assert_int_eq(strlen(TEST_RECIPIENT_ADDRESS), 42);

    rtnVal = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    BoatFieldMax32B value;
    value.field_len = UtilityHexToBin(value.field, 32, "0x2386F26FC10000",
                                      TRIMBIN_LEFTTRIM, BOAT_TRUE);
    rtnVal = BoatCitaTxSetValue(NULL, &value);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

START_TEST(test_005ParametersSet_0005SetValueSuccessNullvalue)
{
    BSINT32 rtnVal;
    BoatCitaTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = citaWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatCitaTxInit(g_cita_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_RECIPIENT_ADDRESS, quota_limit_value);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatCitaTxSetValue(&tx_ptr, NULL);
    ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert_int_eq(tx_ptr.rawtx_fields.value.field_len, 32);

    BoatIotSdkDeInit();
    BoatSleep(1);
}
END_TEST

Suite *make_parameters_suite(void)
{
    /* Create Suite */
    Suite *s_param = suite_create("param");

    /* Create test cases */
    TCase *tc_param_api = tcase_create("param_api");

    tcase_set_timeout(tc_param_api,50);       

    /* Add a test case to the Suite */
    suite_add_tcase(s_param, tc_param_api);      
 
    /* Test cases are added to the test set */
    tcase_add_test(tc_param_api, test_004ParametersInit_0001TxInitSuccess); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0002TxInitFailureNullParam); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0003TxInitFailureRecipientErrorHexFormat); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0004TxInitFailureRecipientLongLength); 

    tcase_add_test(tc_param_api, test_005ParametersGet_0001GetBlockerSuccess); 
    tcase_add_test(tc_param_api, test_005ParametersGet_0002GetBlockerFailureNullTx); 
    tcase_add_test(tc_param_api, test_005ParametersSet_0003SetValueSuccess); 
    tcase_add_test(tc_param_api, test_005ParametersSet_0004SetValueFailureNullTx); 
    tcase_add_test(tc_param_api, test_005ParametersSet_0005SetValueSuccessNullvalue); 

    return s_param;
}