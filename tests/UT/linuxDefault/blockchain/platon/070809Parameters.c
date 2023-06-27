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

#define TEST_GAS_LIMIT              "0x33333"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_PLATON_CHAIN_ID      210309

#define TEST_RECIPIENT_ADDRESS      "lat159js9hw63x2y8m65mhgprm3eu9f4c7xmv3lym4"

/**
 * PlatON test network human-readable part
 */
static const BCHAR *hrp = "lat";

__BOATSTATIC BoatPlatONWallet * platonOnetimeWalletPrepare()
{
    BOAT_RESULT index;
    BoatKeypairPriKeyCtx_config keypair_config;
    BUINT8 keypair_index;
    BoatPlatONNetworkConfig network_config;
    BUINT8 network_index;
    BoatPlatONWallet *wallet_p = NULL;
    
        
    if (TEST_KEY_TYPE == "BOAT_WALLET_PRIKEY_FORMAT_NATIVE")
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(g_binFormatKey, 32, g_platon_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = g_binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    }
    else
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)g_platon_private_key_buf;
	    keypair_config.prikey_content.field_len = strlen(g_platon_private_key_buf) + 1;
    }
	keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

	network_config.chain_id             = TEST_PLATON_CHAIN_ID;
    network_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    memset(network_config.node_url_str,0U,BOAT_PLATON_NODE_URL_MAX_LEN);
    strncpy(network_config.node_url_str, TEST_PLATON_NODE_URL, BOAT_PLATON_NODE_URL_MAX_LEN - 1);

    network_index = BoATPlatONNetworkCreate(&network_config,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(network_index, 0);

    wallet_p = BoatPlatONWalletInit(keypair_index,network_index);
    ck_assert(wallet_p != NULL);

    return wallet_p;
}


START_TEST(test_007ParametersInit_0001TxInitSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0002TxInitFailureNullParam) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(NULL,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    ret = BoatPlatONTxInit(wallet,NULL,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0003TxInitSuccessNullGasPrice) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet =  NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, NULL,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0004TxInitFailureErrorGasPriceHexFormat) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, "0x4A81LLJK",
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_007ParametersInit_0005TxInitFailureGasPriceHexNonOx) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, "4A817C800",
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_007ParametersInit_0006TxInitFailureGasLimitErrorHexFormat) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   "0x6691JJ",TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0007TxInitFailureGasLimitHexNonOx) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   "6691B7",TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_007ParametersInit_0008TxInitFailureRecipientErrorHexFormat) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,"0xde4c806b372Df8857C97cF36A08D528bB8E261JJ",hrp);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0009TxInitFailureRecipientLongLength) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,"0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",hrp);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0001GetNonceFromNetworkSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatPlatONTxSetNonce(&tx_ctx,BOAT_PLATON_NONCE_AUTO);
    ck_assert_int_eq(ret, BOAT_SUCCESS);


    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0002SetNonceSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare(wallet);

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatPlatONTxSetNonce(&tx_ctx,0xA1);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatFieldMax32B NONCE;
    memset(NONCE.field, 0, 32);
	NONCE.field_len = UtilityHexToBin(NONCE.field, 32, "0xA1",
	 				                  TRIMBIN_LEFTTRIM, BOAT_TRUE);

	ck_assert(0 == memcmp(tx_ctx.rawtx_fields.nonce.field, NONCE.field,tx_ctx.rawtx_fields.nonce.field_len));


    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0003SetNonceFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatPlatONTxSetNonce(NULL,"0xA1");
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0004SetValueSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldMax32B value;
    value.field_len = UtilityHexToBin(value.field, 32, "0x2386F26FC10000",
									  TRIMBIN_LEFTTRIM, BOAT_TRUE);

    ret = BoatPlatONTxSetValue(&tx_ctx,&value);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ck_assert_str_eq(tx_ctx.rawtx_fields.value.field, value.field);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_008ParametersSet_0005SetValueFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldMax32B value;
    value.field_len = UtilityHexToBin(value.field, 32, "0x2386F26FC10000",
									  TRIMBIN_LEFTTRIM, BOAT_TRUE);

    ret = BoatPlatONTxSetValue(NULL,&value);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0006SetValueSuccessNullvalue) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/

    ret = BoatPlatONTxSetValue(&tx_ctx,NULL);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(tx_ctx.rawtx_fields.value.field_len, 0);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0007SetDataSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldVariable data;
    data.field_ptr = "hello";
    data.field_len = strlen("hello");
    ret = BoatPlatONTxSetData(&tx_ctx,&data);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_str_eq(tx_ctx.rawtx_fields.data.field_ptr, data.field_ptr);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0008SetDataFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldVariable data;
    data.field_ptr = "hello";
    data.field_len = strlen("hello");
    ret = BoatPlatONTxSetData(NULL,&data);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0009SetDataSuccessNullData) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatONTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS,hrp);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    ret = BoatPlatONTxSetData(&tx_ctx,NULL);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(tx_ctx.rawtx_fields.data.field_len, 0);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

#if 0
START_TEST(test_009ParametersChange_0001ChangeNodeUrlSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Change url*/
    ret = BoatPlatONWalletChangeNodeUrl(wallet,"www.123.com");
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_str_eq(wallet->network_info.node_url_str,"www.123.com");

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009ParametersChange_0002ChangeNodeUrlFailureNullParameters) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Change url*/
    ret = BoatPlatONWalletChangeNodeUrl(wallet,NULL);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    ret = BoatPlatONWalletChangeNodeUrl(NULL,"www.123.com");
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009ParametersChange_0003ChangeChainIDSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Change chainID*/
    ret = BoatPlatONWalletChangeChainID(wallet,111);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(wallet->network_info.chain_id, 111);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009ParametersChange_0004ChangeChainIDFailureNullParameters) 
{
    BOAT_RESULT ret;

    BoatPlatONWallet *wallet = NULL;
    BoatPlatONTx tx_ctx;

    BoatIotSdkInit();

    wallet = platonOnetimeWalletPrepare();

    /* Change chainID*/
    ret = BoatPlatONWalletChangeChainID(NULL,111);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatONWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST
#endif

Suite *make_parameters_suite(void) 
{
    /* Create Suite */
    Suite *s_parameters = suite_create("parameters");

    /* Create test cases */
    TCase *tc_parameters_api = tcase_create("parameters_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_parameters, tc_parameters_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0001TxInitSuccess);

    tcase_add_test(tc_parameters_api, test_007ParametersInit_0002TxInitFailureNullParam);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0003TxInitSuccessNullGasPrice);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0004TxInitFailureErrorGasPriceHexFormat);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0005TxInitFailureGasPriceHexNonOx);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0006TxInitFailureGasLimitErrorHexFormat);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0007TxInitFailureGasLimitHexNonOx);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0008TxInitFailureRecipientErrorHexFormat);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0009TxInitFailureRecipientLongLength);

    tcase_add_test(tc_parameters_api, test_008ParametersSet_0001GetNonceFromNetworkSuccess);
    tcase_add_test(tc_parameters_api, test_008ParametersSet_0002SetNonceSuccess);
    tcase_add_test(tc_parameters_api, test_008ParametersSet_0003SetNonceFailureNullTx);
    tcase_add_test(tc_parameters_api, test_008ParametersSet_0004SetValueSuccess);
    tcase_add_test(tc_parameters_api, test_008ParametersSet_0005SetValueFailureNullTx);
    tcase_add_test(tc_parameters_api, test_008ParametersSet_0006SetValueSuccessNullvalue);
    tcase_add_test(tc_parameters_api, test_008ParametersSet_0007SetDataSuccess);
    tcase_add_test(tc_parameters_api, test_008ParametersSet_0008SetDataFailureNullTx);
    tcase_add_test(tc_parameters_api, test_008ParametersSet_0009SetDataSuccessNullData);

 //   tcase_add_test(tc_parameters_api, test_009ParametersChange_0001ChangeNodeUrlSuccess);
 //   tcase_add_test(tc_parameters_api, test_009ParametersChange_0002ChangeNodeUrlFailureNullParameters);
 //   tcase_add_test(tc_parameters_api, test_009ParametersChange_0003ChangeChainIDSuccess);
 //   tcase_add_test(tc_parameters_api, test_009ParametersChange_0004ChangeChainIDFailureNullParameters);

    return s_parameters;
}

