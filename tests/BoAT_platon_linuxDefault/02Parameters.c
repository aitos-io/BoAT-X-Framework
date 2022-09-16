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
#include "tcase_platon.h"
#define EXCEED_STR_MAX_LEN 4097

#define TEST_EIP155_COMPATIBILITY   BOAT_TRUE
#define TEST_PLATON_CHAIN_ID        210309
#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_IS_SYNC_TX             BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS      "lat159js9hw63x2y8m65mhgprm3eu9f4c7xmv3lym4"

BoatPlatONWallet *g_platon_wallet_ptr;
BoatPlatONWalletConfig wallet_config = {0};

const BCHAR *hrp = "lat";

BOAT_RESULT platonWalletPrepare(void)
{
    BOAT_RESULT index;

    //set user private key context
    memset(&wallet_config, 0, sizeof(wallet_config));
        
    if (TEST_KEY_TYPE == BOAT_WALLET_PRIKEY_FORMAT_NATIVE)
    {
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(binFormatKey, 32, platon_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    }
    else
    {
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)platon_private_key_buf;
	    wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(platon_private_key_buf) + 1;
    }
	wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.prikeyCtx_config.prikey_type	  = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;

	wallet_config.chain_id             = TEST_PLATON_CHAIN_ID;
    wallet_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    strncpy(wallet_config.node_url_str, TEST_PLATON_CHAIN_ID, BOAT_PLATON_NODE_URL_MAX_LEN - 1);

    index = BoatWalletCreate(BOAT_PROTOCOL_PLATON, NULL, &wallet_config, sizeof(BoatPlatONWalletConfig));

    if (index == BOAT_ERROR)
    {
        return BOAT_ERROR;
    }
    
    g_platon_wallet_ptr = BoatGetWalletByIndex(index);
    if (g_platon_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

START_TEST(test_004ParametersInit_0001TxInitSuccess)
{
    BOAT_RESULT rtnVal;
    BoatPlatONTx tx_ptr = {0};

    BoatIotSdkInit();

    rtnVal = platonWalletPrepare();
    ck_assert(rtnVal == BOAT_SUCCESS);

    rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS, hrp);
    ck_assert(rtnVal == BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0002TxInitFailureNullParam)
{
    BSINT32 rtnVal;
    BoatPlatONTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = platonWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatPlatONTxInit(NULL, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);

	rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, NULL, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
				   
	rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, NULL, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
				   
	rtnVal = BoatPlatONTxInit(NULL, NULL, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
					   
	rtnVal = BoatPlatONTxInit(NULL, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, NULL, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, NULL, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, NULL, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
						   
    rtnVal = BoatPlatONTxInit(NULL, NULL, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, NULL, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0003TxInitSuccessNullGasPrice)
{
	BSINT32 rtnVal;
    BoatPlatONTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = platonWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

	rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, NULL, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS, hrp);	
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0004TxInitFailureErrorGasPriceHexFormat)
{
    BSINT32 rtnVal;
    BoatPlatONTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = platonWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
	rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, "0x123G", 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0005TxInitSuccessGasPriceHexNullOx)
{
	BSINT32 rtnVal;
    BoatPlatONTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = platonWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
	rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, "A", 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS, hrp);	
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0006TxInitFailureGasLimitErrorHexFormat)
{
	BSINT32 rtnVal;
    BoatPlatONTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = platonWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   "0x123G", TEST_RECIPIENT_ADDRESS, hrp);
	ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0007TxInitSuccessGasLimitHexNullOx)
{
    BSINT32 rtnVal;
    BoatPlatONTx tx_ptr;

    BoatIotSdkInit();

    rtnVal = platonWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatPlatONTxInit(g_platon_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   "333333", TEST_RECIPIENT_ADDRESS, hrp);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

Suite *make_parameters_suite(void)
{
    /* Create Suite */
    Suite *s_param = suite_create("param");


    /* Create test cases */
    TCase *tc_param_api = tcase_create("param_api");


    /* Add a test case to the Suite */
    suite_add_tcase(s_param, tc_param_api);      
 
    /* Test cases are added to the test set */
    tcase_add_test(tc_param_api, test_004ParametersInit_0001TxInitSuccess); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0002TxInitFailureNullParam); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0003TxInitSuccessNullGasPrice); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0004TxInitFailureErrorGasPriceHexFormat); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0005TxInitSuccessGasPriceHexNullOx); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0006TxInitFailureGasLimitErrorHexFormat); 
    tcase_add_test(tc_param_api, test_004ParametersInit_0007TxInitSuccessGasLimitHexNullOx); 

    return s_param;
}