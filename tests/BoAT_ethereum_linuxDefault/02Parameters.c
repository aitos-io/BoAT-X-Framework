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

static BOAT_RESULT param_check(BoatEthTx* tx_ptr)
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
		if(tx_ptr->rawtx_fields.gasprice.field_len != BIN_GAS_PRICE.field_len)
		{
			return BOAT_ERROR;
		}
		for(int i = 0; i < BIN_GAS_PRICE.field_len; i++)
		{
			if(tx_ptr->rawtx_fields.gasprice.field[i] != BIN_GAS_PRICE.field[i])
			{
				return BOAT_ERROR;
			}
		}
    }
	else
	{
		
	}
	
	BoatFieldMax32B BIN_GAS_LIMIT;
	BIN_GAS_LIMIT.field_len = UtilityHexToBin(BIN_GAS_LIMIT.field, 32, TEST_GAS_LIMIT,
					                          TRIMBIN_LEFTTRIM, BOAT_TRUE);
	if(tx_ptr->rawtx_fields.gaslimit.field_len != BIN_GAS_LIMIT.field_len)
	{
		return BOAT_ERROR;
	}
	for(int i = 0; i < BIN_GAS_LIMIT.field_len; i++)
	{
		if(tx_ptr->rawtx_fields.gaslimit.field[i] != BIN_GAS_LIMIT.field[i])
		{
			return BOAT_ERROR;
		}
	}
	
	BUINT8 BIN_RECIPIENT_ADDRESS[BOAT_ETH_ADDRESS_SIZE];
	BUINT32 CONVERTED_LEN;
	CONVERTED_LEN = UtilityHexToBin(BIN_RECIPIENT_ADDRESS, BOAT_ETH_ADDRESS_SIZE, TEST_RECIPIENT_ADDRESS,
		                            TRIMBIN_TRIM_NO, BOAT_TRUE);
	for(int i = 0; i < CONVERTED_LEN; i++)
	{
		if(tx_ptr->rawtx_fields.recipient[i] != BIN_RECIPIENT_ADDRESS[i])
		{
			return BOAT_ERROR;
		}
	}
	
    result = (tx_ptr->wallet_ptr->network_info.chain_id == TEST_ETHEREUM_CHAIN_ID) ? 0 : -1;
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

	result = (tx_ptr->wallet_ptr->network_info.eip155_compatibility == TEST_EIP155_COMPATIBILITY) ? 0 : -1;
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

	result = (tx_ptr->is_sync_tx == TEST_IS_SYNC_TX) ? 0 : -1;
    if (result != 0) 
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
	ck_assert(param_check(&tx_ptr) == BOAT_SUCCESS);
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
 
    return s_param;
}



