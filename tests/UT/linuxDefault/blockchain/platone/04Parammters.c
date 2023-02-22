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

#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_PLATONE_CHAIN_ID       1

#define TEST_RECIPIENT_ADDRESS      "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45"

__BOATSTATIC BoatPlatoneWallet *platoneOnetimeWalletPrepare()
{
    BOAT_RESULT index;
    BoatKeypairPriKeyCtx_config keypair_config;
    BUINT8 keypair_index;
    BoatPlatoneNetworkConfig network_config;
    BUINT8 network_index;
    BoatPlatoneWallet *wallet = BoatMalloc(sizeof(BoatPlatoneWallet));
        
    if (TEST_KEY_TYPE == "BOAT_WALLET_PRIKEY_FORMAT_NATIVE")
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(g_binFormatKey, 32, g_platone_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = g_binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    }
    else
    {
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)g_platone_private_key_buf;
	    keypair_config.prikey_content.field_len = strlen(g_platone_private_key_buf) + 1;
    }
	keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

	network_config.chain_id             = TEST_PLATONE_CHAIN_ID;
    network_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    memset(network_config.node_url_str,0U,BOAT_PLATONE_NODE_URL_MAX_LEN);
    strncpy(network_config.node_url_str, TEST_PLATONE_NODE_URL, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    network_index = BoATPlatoneNetworkCreate(&network_config,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(network_index, 0);

    wallet = BoatPlatoneWalletInit(keypair_index,network_index);
    ck_assert(wallet != NULL);

    return wallet;
}

START_TEST(test_004Parameters_0001TxInitSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS, 
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0002TxInitFailureNullParam) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(NULL,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    ret = BoatPlatoneTxInit(wallet,NULL,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0003TxInitSuccessNullGasPrice) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, NULL,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0004TxInitFailureErrorGasPriceHexFormat) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, "0x4A81LLJK",
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_004Parameters_0005TxInitSuccessGasPriceHexNonOx) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, "0x4A817C800",
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_004Parameters_0006TxInitFailureGasLimitErrorHexFormat) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   "0x6691JJ",(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0007TxInitSuccessGasLimitHexNonOx) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   "0x6691B7",(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_004Parameters_0008TxInitFailureRecipientErrorHexFormat) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)"0xde4c806b372Df8857C97cF36A08D528bB8E261JJ",
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0009TxInitFailureRecipientLongLength) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)"0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0010GetNonceFromNetworkSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatPlatoneTxSetNonce(&tx_ctx,BOAT_PLATONE_NONCE_AUTO);
    ck_assert_int_eq(ret, BOAT_SUCCESS);


    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0011SetNonceSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatPlatoneTxSetNonce(&tx_ctx, 161);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatFieldMax32B NONCE;
    memset(NONCE.field, 0, 32);
	NONCE.field_len = UtilityHexToBin(NONCE.field, 32, "0xA1",
	 				                  TRIMBIN_LEFTTRIM, BOAT_TRUE);
	ck_assert_str_eq(tx_ctx.rawtx_fields.nonce.field, NONCE.field);


    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0012SetNonceFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Nonce*/
    ret = BoatPlatoneTxSetNonce(NULL,"0xA1");
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0013SetValueSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldMax32B value;
    value.field_len = UtilityHexToBin(value.field, 32, "0x2386F26FC10000",
									  TRIMBIN_LEFTTRIM, BOAT_TRUE);

    ret = BoatPlatoneTxSetValue(&tx_ctx,&value);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ck_assert_str_eq(tx_ctx.rawtx_fields.value.field, value.field);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_004Parameters_0014SetValueFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldMax32B value;
    value.field_len = UtilityHexToBin(value.field, 32, "0x2386F26FC10000",
									  TRIMBIN_LEFTTRIM, BOAT_TRUE);

    ret = BoatPlatoneTxSetValue(NULL,&value);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0015SetValueSuccessNullvalue) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/

    ret = BoatPlatoneTxSetValue(&tx_ctx,NULL);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(tx_ctx.rawtx_fields.value.field_len, 0);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0016SetDataSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldVariable data;
    data.field_ptr = "hello";
    data.field_len = strlen("hello");
    ret = BoatPlatoneTxSetData(&tx_ctx,&data);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_str_eq(tx_ctx.rawtx_fields.data.field_ptr, data.field_ptr);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0017SetDataFailureNullTx) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    BoatFieldVariable data;
    data.field_ptr = "hello";
    data.field_len = strlen("hello");
    ret = BoatPlatoneTxSetData(NULL,&data);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0018SetDataSuccessNullData) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Init TX */
    ret = BoatPlatoneTxInit(wallet,&tx_ctx,BOAT_TRUE, TEST_GAS_PRICE,
							   TEST_GAS_LIMIT,(BCHAR *)TEST_RECIPIENT_ADDRESS,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* Set Value*/
    ret = BoatPlatoneTxSetData(&tx_ctx,NULL);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(tx_ctx.rawtx_fields.data.field_len, 0);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_004Parameters_0019ChangeNodeUrlSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Change url*/
    ret = BoatPlatoneWalletChangeNodeUrl(wallet,"www.123.com");
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_str_eq(wallet->network_info.node_url_str,"www.123.com");

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0020ChangeNodeUrlFailureNullParameters) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Change url*/
    ret = BoatPlatoneWalletChangeNodeUrl(wallet,NULL);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    ret = BoatPlatoneWalletChangeNodeUrl(NULL,"www.123.com");
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0021ChangeChainIDSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Change chainID*/
    ret = BoatPlatoneWalletChangeChainID(wallet,111);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert_int_eq(wallet->network_info.chain_id, 111);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0022ChangeChainIDFailureNullParameters) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Change chainID*/
    ret = BoatPlatoneWalletChangeChainID(NULL,111);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0023ChangeEip155CompSuccess) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Change EIP155 comp*/
    ret = BoatPlatoneWalletChangeEIP155Comp(wallet,BOAT_TRUE);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ck_assert(wallet->network_info.eip155_compatibility == BOAT_TRUE);

    BoatPlatoneWalletDeInit(wallet);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004Parameters_0024ChangeEip155CompFailureNullParameters) 
{
    BOAT_RESULT ret;

    BoatPlatoneWallet *wallet;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    wallet = platoneOnetimeWalletPrepare();

    /* Change EIP155 comp*/
    ret = BoatPlatoneWalletChangeEIP155Comp(NULL,BOAT_TRUE);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatPlatoneWalletDeInit(wallet);

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
    tcase_add_test(tc_parameters_api, test_004Parameters_0001TxInitSuccess);
    tcase_add_test(tc_parameters_api, test_004Parameters_0002TxInitFailureNullParam);
    tcase_add_test(tc_parameters_api, test_004Parameters_0003TxInitSuccessNullGasPrice);
    tcase_add_test(tc_parameters_api, test_004Parameters_0004TxInitFailureErrorGasPriceHexFormat);
    tcase_add_test(tc_parameters_api, test_004Parameters_0005TxInitSuccessGasPriceHexNonOx);
    tcase_add_test(tc_parameters_api, test_004Parameters_0006TxInitFailureGasLimitErrorHexFormat);
    tcase_add_test(tc_parameters_api, test_004Parameters_0007TxInitSuccessGasLimitHexNonOx);
    tcase_add_test(tc_parameters_api, test_004Parameters_0008TxInitFailureRecipientErrorHexFormat);
    tcase_add_test(tc_parameters_api, test_004Parameters_0009TxInitFailureRecipientLongLength);

    tcase_add_test(tc_parameters_api, test_004Parameters_0010GetNonceFromNetworkSuccess);
    tcase_add_test(tc_parameters_api, test_004Parameters_0011SetNonceSuccess);
    tcase_add_test(tc_parameters_api, test_004Parameters_0012SetNonceFailureNullTx);
    tcase_add_test(tc_parameters_api, test_004Parameters_0013SetValueSuccess);
    tcase_add_test(tc_parameters_api, test_004Parameters_0014SetValueFailureNullTx);
    tcase_add_test(tc_parameters_api, test_004Parameters_0015SetValueSuccessNullvalue);
    tcase_add_test(tc_parameters_api, test_004Parameters_0016SetDataSuccess);
    tcase_add_test(tc_parameters_api, test_004Parameters_0017SetDataFailureNullTx);
    tcase_add_test(tc_parameters_api, test_004Parameters_0018SetDataSuccessNullData);

    tcase_add_test(tc_parameters_api, test_004Parameters_0019ChangeNodeUrlSuccess);
    tcase_add_test(tc_parameters_api, test_004Parameters_0020ChangeNodeUrlFailureNullParameters);
    tcase_add_test(tc_parameters_api, test_004Parameters_0021ChangeChainIDSuccess);
    tcase_add_test(tc_parameters_api, test_004Parameters_0022ChangeChainIDFailureNullParameters);
    tcase_add_test(tc_parameters_api, test_004Parameters_0023ChangeEip155CompSuccess);
    tcase_add_test(tc_parameters_api, test_004Parameters_0024ChangeEip155CompFailureNullParameters);

    return s_parameters;
}
