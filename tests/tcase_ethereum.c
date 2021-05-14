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

/* This file used to test ethereum releated interface 

*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "boattypes.h"
#include "boatutility.h"
#include "protocolapi/api_ethereum.h"
#include "tcase_common.h"
#include "check.h"


static BoatEthWallet *g_test_wallet_ptr;
static BoatEthTx g_test_tx_ptr;
static void Test_Protocol_EthApi_Init(void)
{
	BoatEthWalletConfig wallet_config;
	/* wallet_config value assignment */
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;

    strncpy( wallet_config.node_url_str, 
			 TEST_ETH_NODE_URL, 
			 BOAT_ETH_NODE_URL_MAX_LEN - 1 );
	g_test_wallet_ptr = BoatEthWalletInit(&wallet_config, sizeof(BoatEthWalletConfig));

	BoatEthTxInit(g_test_wallet_ptr,&g_test_tx_ptr,BOAT_TRUE,NULL,TEST_ETH_GASPRICE,TEST_ETH_WALLET_ADDR_1);
}

static void Test_Protocol_EthApi_DeInit(void)
{
	BoatEthWalletDeInit(g_test_wallet_ptr);
}

START_TEST(test_Api_WalletInit){
	BoatEthWalletConfig wallet_config;

	/* wallet_config value assignment */
	wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
	wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy( wallet_config.node_url_str, TEST_ETH_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1 );

	//case 1:
	BoatEthWallet *wallet_ptr_1;
	wallet_ptr_1 = BoatEthWalletInit(&wallet_config, sizeof(BoatEthWalletConfig));
	ck_assert(wallet_ptr_1 != NULL);

	ck_assert(wallet_ptr_1->web3intf_context_ptr != NULL);
	ck_assert_str_eq(wallet_ptr_1->network_info.node_url_ptr, TEST_ETH_NODE_URL);
	ck_assert(wallet_ptr_1->network_info.chain_id == 1);
	ck_assert(wallet_ptr_1->network_info.eip155_compatibility == BOAT_FALSE);
}
END_TEST

START_TEST(test_Api_SetNodeUrl){

	//Node url	
	//case 1:
	memset(g_test_wallet_ptr->network_info.node_url_ptr,0,strlen(g_test_wallet_ptr->network_info.node_url_ptr));
	ck_assert(BoatEthWalletSetNodeUrl(g_test_wallet_ptr, TEST_ETH_NODE_URL) == BOAT_SUCCESS);
	ck_assert_str_eq(g_test_wallet_ptr->network_info.node_url_ptr,TEST_ETH_NODE_URL);

	//case 2:
	ck_assert(BoatEthWalletSetNodeUrl(NULL, TEST_ETH_NODE_URL) == BOAT_ERROR_INVALID_ARGUMENT);

	//case 3:
	ck_assert(BoatEthWalletSetNodeUrl(g_test_wallet_ptr, NULL) == BOAT_ERROR_INVALID_ARGUMENT);

	//case 4:
	ck_assert(BoatEthWalletSetNodeUrl(g_test_wallet_ptr, TEST_ETH_NODE_URL_1) == BOAT_ERROR_INVALID_ARGUMENT);

	//case 5:
	g_test_wallet_ptr->network_info.node_url_ptr = BoatMalloc(strlen(TEST_ETH_NODE_URL)+1);
	ck_assert(BoatEthWalletSetNodeUrl(g_test_wallet_ptr, TEST_ETH_NODE_URL) == BOAT_SUCCESS);
	ck_assert_str_eq(g_test_wallet_ptr->network_info.node_url_ptr,TEST_ETH_NODE_URL);
}
END_TEST

START_TEST(test_Api_SetEIP155Comp) {

	//Eip155
	//case 1:
	g_test_wallet_ptr->network_info.eip155_compatibility = BOAT_TRUE;
	ck_assert(BoatEthWalletSetEIP155Comp(g_test_wallet_ptr, BOAT_FALSE ) == BOAT_SUCCESS);
	ck_assert(g_test_wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);
	//case 2:
	ck_assert(BoatEthWalletSetNodeUrl(NULL, BOAT_FALSE) == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_Api_SetChainId) {

	//Chain id 
	//case 1:
	g_test_wallet_ptr->network_info.chain_id = 0xFF;
	ck_assert(BoatEthWalletSetChainId(g_test_wallet_ptr, 0 ) == BOAT_SUCCESS);
	ck_assert(g_test_wallet_ptr->network_info.chain_id == BOAT_FALSE);

	//case 2:
	ck_assert(BoatEthWalletSetChainId(NULL, BOAT_FALSE) == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_Api_GetBalance) {

	//Get balance 
	//case 1:
	BCHAR *wallet_balance_ptr;
	BoatFieldVariable result_ptr;
	result_ptr.field_len = 128;
	result_ptr.field_ptr = BoatMalloc(result_ptr.field_len);
	ck_assert((wallet_balance_ptr = BoatEthWalletGetBalance(g_test_wallet_ptr, TEST_ETH_WALLET_ADDR_0)) != NULL);
	ck_assert(BoatEthPraseRpcResponseResult( wallet_balance_ptr,"result",&result_ptr) == BOAT_SUCCESS);
	//! @todo 
	//ck_assert_str_eq(result_ptr.field_ptr, TEST_ETH_WALLET_BALANCE);
	BoatFree(result_ptr.field_ptr);

	//case 2:
	ck_assert(BoatEthWalletSetChainId(NULL, BOAT_FALSE) == BOAT_ERROR_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_Api_TxInit) {

	//tx init
	//case 1:
	BoatEthTx tx_ptr;					  
	ck_assert(BoatEthTxInit(g_test_wallet_ptr,&tx_ptr,BOAT_TRUE,NULL,TEST_ETH_GASPRICE,TEST_ETH_WALLET_ADDR_1) == BOAT_SUCCESS);

	ck_assert_str_eq(tx_ptr.wallet_ptr->network_info.node_url_ptr ,TEST_ETH_NODE_URL);
	ck_assert(tx_ptr.wallet_ptr->network_info.chain_id == 1);
	ck_assert(tx_ptr.wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);

	ck_assert_str_eq(tx_ptr.rawtx_fields.gasprice.field,TEST_ETH_GASPRICE);
	ck_assert_str_eq(tx_ptr.rawtx_fields.recipient,TEST_ETH_WALLET_ADDR_1);

	//case 2:
	ck_assert(BoatEthTxInit(g_test_wallet_ptr,&tx_ptr,BOAT_TRUE,NULL,NULL,TEST_ETH_WALLET_ADDR_1) == BOAT_SUCCESS);
	ck_assert_str_eq(tx_ptr.wallet_ptr->network_info.node_url_ptr ,TEST_ETH_NODE_URL);
	ck_assert(tx_ptr.wallet_ptr->network_info.chain_id == 1);
	ck_assert(tx_ptr.wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);
	printf("++++++++++++tx_ptr.rawtx_fields.gasprice.field: %s\n",tx_ptr.rawtx_fields.gasprice.field);
	//ck_assert_str_eq(tx_ptr.rawtx_fields.gasprice.field,TEST_ETH_GASPRICE);
	ck_assert_str_eq(tx_ptr.rawtx_fields.recipient,TEST_ETH_WALLET_ADDR_1);
}
END_TEST

START_TEST(test_Api_SetNonce) {

	//Set Nonce
	//case 1:
	BUINT64 nonce=0x12;
	ck_assert(BoatEthTxSetNonce(&g_test_tx_ptr,nonce) == BOAT_SUCCESS);
	ck_assert_str_eq(g_test_tx_ptr.rawtx_fields.nonce.field,&nonce);


	//case 2:
	ck_assert(BoatEthTxSetNonce(NULL, BOAT_FALSE) == BOAT_ERROR_INVALID_ARGUMENT);

	//case 3:
	//ck_assert(BoatEthTxSetNonce(&g_test_tx_ptr, NULL) == BOAT_ETH_NONCE_AUTO);

}
END_TEST

START_TEST(test_Api_SetGasprice) {
	//Set Gasprice
	//case 1:
	BoatFieldMax32B gas_price_ptr;
	ck_assert(UtilityHexToBin(gas_price_ptr.field,32,TEST_ETH_GASPRICE,TRIMBIN_LEFTTRIM,BOAT_TRUE) != 0);
	ck_assert(BoatEthTxSetGasPrice(&g_test_tx_ptr,gas_price_ptr.field) == BOAT_SUCCESS);
	ck_assert_str_eq(g_test_tx_ptr.rawtx_fields.gasprice.field,gas_price_ptr.field);

	//case 2:
	ck_assert(BoatEthTxSetGasPrice(NULL,gas_price_ptr.field) == BOAT_ERROR_INVALID_ARGUMENT);

	//case 3:
	ck_assert(BoatEthTxSetGasPrice(&g_test_tx_ptr,NULL) == BOAT_SUCCESS);

}
END_TEST

START_TEST(test_Api_SetGaslimit) {
	//Set GasLimit
	//case 1:
	BoatFieldMax32B gas_limit_ptr;
	ck_assert(UtilityHexToBin(gas_limit_ptr.field,32,TEST_ETH_GASLIMIT,TRIMBIN_LEFTTRIM,BOAT_TRUE) != 0);
	ck_assert(BoatEthTxSetGasLimit(&g_test_tx_ptr,gas_limit_ptr.field) == BOAT_SUCCESS);
	ck_assert_str_eq(g_test_tx_ptr.rawtx_fields.gaslimit.field,gas_limit_ptr.field);
	//case 2:
	ck_assert(BoatEthTxSetGasLimit(NULL,gas_limit_ptr.field) == BOAT_ERROR_INVALID_ARGUMENT);

	//case 3:
	ck_assert(BoatEthTxSetGasLimit(&g_test_tx_ptr,NULL) == BOAT_SUCCESS);

}
END_TEST

START_TEST(test_Api_SetRecipientAddr) {
	//Set GasLimit
	//case 1:
	BUINT8 recipient_address[BOAT_ETH_ADDRESS_SIZE];
	ck_assert(UtilityHexToBin(recipient_address,20,TEST_ETH_RECIPIENT_ADDR,TRIMBIN_TRIM_NO,BOAT_TRUE) != 0);
	ck_assert(BoatEthTxSetRecipient(&g_test_tx_ptr,recipient_address) == BOAT_SUCCESS);
	ck_assert_str_eq(g_test_tx_ptr.rawtx_fields.recipient,recipient_address);

	//case 2:
	ck_assert(BoatEthTxSetRecipient(NULL,recipient_address) == BOAT_ERROR_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_Api_SetValue) {
	//Set value
	//case 1:
	BoatFieldMax32B value_ptr;
	ck_assert(UtilityHexToBin(value_ptr.field,32,TEST_ETH_VALUE,TRIMBIN_LEFTTRIM,BOAT_TRUE) != 0);
	ck_assert(BoatEthTxSetValue(&g_test_tx_ptr,value_ptr.field) == BOAT_SUCCESS);
	ck_assert_str_eq(g_test_tx_ptr.rawtx_fields.value.field,value_ptr.field);

	//case 2:
	ck_assert(BoatEthTxSetValue(NULL,value_ptr.field) == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_Api_SetData) {
	//Set Data
	//case 1:
	ck_assert(BoatEthTxSetData(&g_test_tx_ptr,NULL) == BOAT_SUCCESS);

	//case 2:
	ck_assert(BoatEthTxSetData(NULL,NULL) == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_Api_TxSend) {
	//Tx Send
	//case 1:
	ck_assert(BoatEthTxSend(&g_test_tx_ptr) == BOAT_SUCCESS);

	//case 2:
	ck_assert(BoatEthTxSend(NULL) == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST


/*
BCHAR * BoatEthCallContractFunc( BoatEthTx *tx_ptr, BCHAR *func_proto_str,
		 BUINT8 *func_param_ptr, BUINT32 func_param_len )
*/

START_TEST(test_Api_Transfer) {
	//Tx Send
	//case 1:
	ck_assert(BoatEthTransfer(&g_test_tx_ptr,TEST_ETH_VALUE) == BOAT_SUCCESS);

	//case 2:
	ck_assert(BoatEthTransfer(NULL,TEST_ETH_VALUE) == BOAT_ERROR_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_Api_GetTransactionReceipt) {
	//Tx Send
	//case 1:
	ck_assert(BoatEthGetTransactionReceipt(&g_test_tx_ptr) == BOAT_SUCCESS);

	//case 2:
	ck_assert(BoatEthGetTransactionReceipt(NULL) == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST


Suite * make_ethereum_suite(void) {
	/* Create Suite */
    Suite *s_ethereum = suite_create("ethereum");
	/* Create test cases */
    TCase *tc_eth_pub_api = tcase_create("ethereum_public_api");
	TCase *tc_eth_pri_api = tcase_create("ethereum_private_api");
	/* Add test case to the Suite */
    suite_add_tcase(s_ethereum, tc_eth_pub_api);
	suite_add_tcase(s_ethereum, tc_eth_pri_api); 
	/* Add unit test to test case.
	   note: For clarity, it is recommended that unit tests of different attributes 
	   be added to different test cases*/
    tcase_add_test(tc_eth_pub_api, test_Api_WalletInit);  
	/* if a unit test need run multiple times, use function tcase_add_loop_test to add unit-test,
	   and the results of each test will be recorded */
	tcase_add_loop_test(tc_eth_pri_api, test_Api_WalletInit, 0, 10);
	//tcase_add_test(tc_eth_pub_api, test_Api_SetNodeUrl); 
	//tcase_add_test(tc_eth_pub_api, test_Api_SetGasprice);
	//tcase_add_test(tc_eth_pub_api, test_Api_SetGaslimit);
	//tcase_add_test(tc_eth_pub_api, test_Api_SetRecipientAddr); 
	//tcase_add_test(tc_eth_pub_api, test_Api_SetValue); 
	//tcase_add_test(tc_eth_pub_api, test_Api_SetData); 
	//tcase_add_test(tc_eth_pub_api, test_Api_TxSend);
	//tcase_add_test(tc_eth_pub_api, test_Api_Transfer);
	//tcase_add_test(tc_eth_pub_api, test_Api_GetTransactionReceipt);

    return s_ethereum;
}



