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
#include "tcase_ethereum.h"

static BoatEthWallet* g_ethereum_wallet_ptr;
static BoatEthWalletConfig wallet_config = {0};
#define USE_ONETIME_WALLET

static BOAT_RESULT ethereumWalletPrepare(void)
{
    BOAT_RESULT index;

    //set user private key context
	wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.prikeyCtx_config.prikey_type	  = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
	wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)ethereum_pkcs_key_buf;
	wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(ethereum_pkcs_key_buf) + 1;

	wallet_config.chain_id             = TEST_ETHEREUM_CHAIN_ID;
    wallet_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    strncpy(wallet_config.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    // create wallet
#if defined(USE_ONETIME_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig));
#elif defined(USE_CREATE_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum.cfg", &wallet_config, sizeof(BoatEthWalletConfig));
#elif defined(USE_LOAD_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum.cfg", NULL, sizeof(BoatEthWalletConfig));
#else
    return BOAT_ERROR;
#endif
    if (index == BOAT_ERROR)
    {
        return BOAT_ERROR;
    }
    
    g_ethereum_wallet_ptr = BoatGetWalletByIndex(index);
    if (g_ethereum_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

static BOAT_RESULT string_eq_check(BUINT8 *string_a, BUINT8 *string_b, BUINT32 string_len_a, BUINT32 string_len_b)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	
	if(string_len_a != string_len_b)
	{
		return BOAT_ERROR;
	}
	
	int count = 0;
	while (count < string_len_a)
	{
		if(string_a[count] != string_b[count])
		{
			return BOAT_ERROR;
		}
		count++;
	}
	return result;
}

static BOAT_RESULT param_init_check(BoatEthTx* tx_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if (tx_ptr == NULL)
    {
        return BOAT_ERROR;
    }
	
	BoatFieldMax32B BIN_GAS_PRICE;
	if (TEST_GAS_PRICE != NULL)
    {
        BIN_GAS_PRICE.field_len = UtilityHexToBin(BIN_GAS_PRICE.field, 32, TEST_GAS_PRICE, 
											      TRIMBIN_LEFTTRIM, BOAT_TRUE);
		result = string_eq_check(tx_ptr->rawtx_fields.gasprice.field, BIN_GAS_PRICE.field, 
								 tx_ptr->rawtx_fields.gasprice.field_len, BIN_GAS_PRICE.field_len);
    	if (result != 0) 
    	{
        	return BOAT_ERROR;
    	}
    }
	else
	{
		
	}
	
	BoatFieldMax32B BIN_GAS_LIMIT;
	BIN_GAS_LIMIT.field_len = UtilityHexToBin(BIN_GAS_LIMIT.field, 32, TEST_GAS_LIMIT,
					                          TRIMBIN_LEFTTRIM, BOAT_TRUE);
	result = string_eq_check(tx_ptr->rawtx_fields.gaslimit.field, BIN_GAS_LIMIT.field, 
							 tx_ptr->rawtx_fields.gaslimit.field_len, BIN_GAS_LIMIT.field_len);
	
	BUINT8 BIN_RECIPIENT_ADDRESS[BOAT_ETH_ADDRESS_SIZE];
	BUINT32 CONVERTED_LEN;
	CONVERTED_LEN = UtilityHexToBin(BIN_RECIPIENT_ADDRESS, BOAT_ETH_ADDRESS_SIZE, TEST_RECIPIENT_ADDRESS,
		                            TRIMBIN_TRIM_NO, BOAT_TRUE);
	result = string_eq_check(tx_ptr->rawtx_fields.recipient, BIN_RECIPIENT_ADDRESS,
							 BOAT_ETH_ADDRESS_SIZE, CONVERTED_LEN);
	
    if (tx_ptr->wallet_ptr->network_info.chain_id != TEST_ETHEREUM_CHAIN_ID) 
    {
        return BOAT_ERROR;
    }

    if (tx_ptr->wallet_ptr->network_info.eip155_compatibility != TEST_EIP155_COMPATIBILITY) 
    {
        return BOAT_ERROR;
    }

    if (tx_ptr->is_sync_tx != TEST_IS_SYNC_TX) 
    {
        return BOAT_ERROR;
    }

    return result;
}

START_TEST(test_004ParametersInit_0001TxInitSuccess)
{
    BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_SUCCESS);
	ck_assert(param_init_check(&tx_ptr) == BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0002TxInitFailureNullParam)
{
    BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatEthTxInit(NULL, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);

	rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, NULL, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
				   
	rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, NULL);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
				   
	rtnVal = BoatEthTxInit(NULL, NULL, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
					   
	rtnVal = BoatEthTxInit(NULL, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, NULL);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, NULL, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, NULL);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
						   
    rtnVal = BoatEthTxInit(NULL, NULL, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, NULL);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0003TxInitSuccessNullGasPrice)
{
	BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
	rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, NULL, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);	
    ck_assert(rtnVal == BOAT_SUCCESS);
}
END_TEST

START_TEST(test_004ParametersInit_0004TxInitFailureErrorGasPriceHexFormat)
{
    BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
	rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, "0x123G", 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0005TxInitSuccessGasPriceHexNullOx)
{
	BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
	rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, "A", 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);	
    ck_assert(rtnVal == BOAT_SUCCESS);
	ck_assert(param_init_check(&tx_ptr) == BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0006TxInitFailureGasLimitErrorHexFormat)
{
	BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   "0x123G", TEST_RECIPIENT_ADDRESS);
	ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0007TxInitSuccessGasLimitHexNullOx)
{
    BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   "333333", TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_SUCCESS);
	ck_assert(param_init_check(&tx_ptr) == BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_004ParametersInit_0008TxInitFailureRecipientErrorHexFormat)
{
    BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, "0xABCDG");
	ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_005ParametersSet_0001GetNonceFromNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
	
    rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_SUCCESS);

	rtnVal = BoatEthTxSetNonce(&tx_ptr, BOAT_ETH_NONCE_AUTO);	
    ck_assert(rtnVal == BOAT_SUCCESS);
    BoatIotSdkDeInit();
  
}
END_TEST

START_TEST(test_005ParametersSet_0002SetNonceSuccess)
{
	BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

	rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_SUCCESS);
	
	rtnVal = BoatEthTxSetNonce(&tx_ptr, 0xA1);	
	BoatFieldMax32B NONCE;
	NONCE.field_len = UtilityHexToBin(NONCE.field, 32, "0xA1",
					                  TRIMBIN_LEFTTRIM, BOAT_TRUE);
    ck_assert(rtnVal == BOAT_SUCCESS);
	rtnVal = string_eq_check(tx_ptr.rawtx_fields.nonce.field, NONCE.field, 
					tx_ptr.rawtx_fields.nonce.field_len, NONCE.field_len);
    ck_assert(rtnVal == BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005ParametersSet_0003SetNonceFailureNullTx)
{
	BSINT32 rtnVal;
    BoatEthTx tx_ptr;
    rtnVal = ethereumWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

	rtnVal = BoatEthTxInit(g_ethereum_wallet_ptr, &tx_ptr, TEST_IS_SYNC_TX, TEST_GAS_PRICE, 
		                   TEST_GAS_LIMIT, TEST_RECIPIENT_ADDRESS);
    ck_assert(rtnVal == BOAT_SUCCESS);
	
	rtnVal = BoatEthTxSetNonce(NULL, 0xA1);	
	ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
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
    tcase_add_test(tc_param_api, test_004ParametersInit_0008TxInitFailureRecipientErrorHexFormat); 
    tcase_add_test(tc_param_api, test_005ParametersSet_0001GetNonceFromNetworkSuccess);  
    tcase_add_test(tc_param_api, test_005ParametersSet_0002SetNonceSuccess);  
	tcase_add_test(tc_param_api, test_005ParametersSet_0003SetNonceFailureNullTx);
	
    return s_param;
}



