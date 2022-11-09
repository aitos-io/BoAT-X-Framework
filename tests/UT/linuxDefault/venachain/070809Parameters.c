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
#include "tcase_venachain.h"

#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_VENACHAIN_CHAIN_ID      300

#define TEST_RECIPIENT_ADDRESS      "0x3e3bd84cf33796cb55cc713d5134597eb809fcc3"

__BOATSTATIC BOAT_RESULT venachainOnetimeWalletPrepare(BoatVenachainWallet *wallet_p)
{
    BOAT_RESULT index;
    BoatKeypairPriKeyCtx_config keypair_config;
    BUINT8 keypair_index;
    BoatVenachainNetworkConfig network_config;
    BUINT8 network_index;

        
    if (TEST_KEY_TYPE == "BOAT_WALLET_PRIKEY_FORMAT_NATIVE")
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = g_binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    }
    else
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)g_venachain_private_key_buf;
	    keypair_config.prikey_content.field_len = strlen(g_venachain_private_key_buf) + 1;
    }
	keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

	network_config.chain_id             = TEST_VENACHAIN_CHAIN_ID;
    network_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    memset(network_config.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(network_config.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    network_index = BoatVenachainNetworkCreate(&network_config,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(network_index, 0);

    wallet_p = BoatVenachainWalletInit(keypair_index,network_index);
    ck_assert(wallet_p != NULL);

    return BOAT_SUCCESS;
}


START_TEST(test_007ParametersInit_0001TxInitSuccess) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0002TxInitFailureNullParam) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(NULL,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    ret = BoatVenachainTxInit(&wallet,NULL,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0003TxInitSuccessNullGasPrice) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, NULL,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0004TxInitFailureErrorGasPriceHexFormat) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, "0x4A81LLJK",
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_007ParametersInit_0005TxInitSuccessGasPriceHexNonOx) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, "4A817C800",
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_007ParametersInit_0006TxInitFailureGasLimitErrorHexFormat) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   "0x6691JJ",TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0007TxInitSuccessGasLimitHexNonOx) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   "6691B7",TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_007ParametersInit_0008TxInitFailureRecipientErrorHexFormat) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,"0xde4c806b372Df8857C97cF36A08D528bB8E261JJ");
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_007ParametersInit_0009TxInitFailureRecipientLongLength) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,"0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0001GetNonceFromNetworkSuccess) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatVenachainTxSetNonce(&tx_ctx,BOAT_VENACHAIN_NONCE_AUTO);
    ck_assert_int_eq(ret, BOAT_SUCCESS);


    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0002SetNonceSuccess) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatVenachainTxSetNonce(&tx_ctx,"0xA1");
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatFieldMax32B NONCE;
    memset(NONCE.field, 0, 32);
	NONCE.field_len = UtilityHexToBin(NONCE.field, 32, "0xA1",
	 				                  TRIMBIN_LEFTTRIM, BOAT_TRUE);
	ck_assert_str_eq(tx_ctx.rawtx_fields.nonce.field, NONCE.field);


    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0003SetNonceFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatVenachainTxSetNonce(NULL,"0xA1");
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0004SetValueSuccess) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldMax32B value;
    value.field_len = UtilityHexToBin(value.field, 32, "0x2386F26FC10000",
									  TRIMBIN_LEFTTRIM, BOAT_TRUE);

    ret = BoatVenachainTxSetValue(&tx_ctx,&value);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ck_assert_str_eq(tx_ctx.rawtx_fields.value.field, value.field);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_008ParametersSet_0005SetValueFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldMax32B value;
    value.field_len = UtilityHexToBin(value.field, 32, "0x2386F26FC10000",
									  TRIMBIN_LEFTTRIM, BOAT_TRUE);

    ret = BoatVenachainTxSetValue(NULL,&value);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0006SetValueSuccessNullvalue) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/

    ret = BoatVenachainTxSetValue(&tx_ctx,NULL);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(tx_ctx.rawtx_fields.value.field_len, 0);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0007SetDataSuccess) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldVariable data;
    data.field_ptr = "hello";
    data.field_len = strlen("hello");
    ret = BoatVenachainTxSetData(&tx_ctx,&data);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_str_eq(tx_ctx.rawtx_fields.data.field_ptr, data.field_ptr);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0008SetDataFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldVariable data;
    data.field_ptr = "hello";
    data.field_len = strlen("hello");
    ret = BoatVenachainTxSetData(NULL,&data);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_008ParametersSet_0009SetDataSuccessNullData) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Init TX */
    ret = BoatVenachainTxInit(&wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    ret = BoatVenachainTxSetData(&tx_ctx,NULL);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(tx_ctx.rawtx_fields.data.field_len, 0);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_009ParametersChange_0001ChangeNodeUrlSuccess) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Change url*/
    ret = BoatVenachainWalletChangeNodeUrl(&wallet,"www.123.com");
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_str_eq(wallet.network_info.node_url_str,"www.123.com");

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009ParametersChange_0002ChangeNodeUrlFailureNullParameters) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Change url*/
    ret = BoatVenachainWalletChangeNodeUrl(&wallet,NULL);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    ret = BoatVenachainWalletChangeNodeUrl(NULL,"www.123.com");
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009ParametersChange_0003ChangeChainIDSuccess) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Change chainID*/
    ret = BoatVenachainWalletChangeChainID(&wallet,111);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(wallet.network_info.chain_id, 111);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009ParametersChange_0004ChangeChainIDFailureNullParameters) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Change chainID*/
    ret = BoatVenachainWalletChangeChainID(NULL,111);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009ParametersChange_0005ChangeEip155CompSuccess) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Change EIP155 comp*/
    ret = BoatVenachainWalletChangeEIP155Comp(&wallet,BOAT_TRUE);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert(wallet.network_info.eip155_compatibility == BOAT_TRUE);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_009ParametersChange_0006ChangeEip155CompFailureNullParameters) 
{
    BOAT_RESULT ret;

    BoatVenachainWallet wallet;
    BoatVenachainTx tx_ctx;

    BoatIotSdkInit();

    venachainOnetimeWalletPrepare(&wallet);

    /* Change EIP155 comp*/
    ret = BoatVenachainWalletChangeEIP155Comp(NULL,BOAT_TRUE);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatVenachainWalletDeInit(&wallet);

    BoatIotSdkDeInit();
}
END_TEST

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
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0005TxInitSuccessGasPriceHexNonOx);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0006TxInitFailureGasLimitErrorHexFormat);
    tcase_add_test(tc_parameters_api, test_007ParametersInit_0007TxInitSuccessGasLimitHexNonOx);
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

    tcase_add_test(tc_parameters_api, test_009ParametersChange_0001ChangeNodeUrlSuccess);
    tcase_add_test(tc_parameters_api, test_009ParametersChange_0002ChangeNodeUrlFailureNullParameters);
    tcase_add_test(tc_parameters_api, test_009ParametersChange_0003ChangeChainIDSuccess);
    tcase_add_test(tc_parameters_api, test_009ParametersChange_0004ChangeChainIDFailureNullParameters);
    tcase_add_test(tc_parameters_api, test_009ParametersChange_0005ChangeEip155CompSuccess);
    tcase_add_test(tc_parameters_api, test_009ParametersChange_0006ChangeEip155CompFailureNullParameters);

    return s_parameters;
}

