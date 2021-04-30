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

#if 0 //! @todo Adjust it at a later time

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "boattypes.h"
#include "boatutility.h"
#include "protocolapi/api_ethereum.h"
#include "check.h"


#define TEST_ETH_PRIKEYID		"/home/c/work_space/boat_sdk_Template/case_boat1.0/BOAT-X-Framework2.0/demo/demo_key/ethereum_client.key"
#define TEST_ETH_NODE_URL		"HTTP://127.0.0.1:7545"

#define TEST_ETH_NODE_URL_1		"HTTP//127.0.0.17545"

#define TEST_ETH_WALLET_ADDR_0	"0xEF5D6d9D037Faa6493637E26A2B5d604f7722779"
#define TEST_ETH_WALLET_ADDR_1	"0xEF5D6d9D037Faa6493637E26A2B5d604f7722779"

#define TEST_ETH_RECIPIENT_ADDR	"0xEF5D6d9D037Faa6493637E26A2B5d604f7722779"

//! @todo The balance provides the actual value on the chain
#define TEST_ETH_WALLET_BALANCE	"0xDE0B6B3A7640000"  

#define TEST_ETH_GASPRICE 		"0x333333"
#define TEST_ETH_GASLIMIT 		"0x333333"
#define TEST_ETH_VALUE			"0xDE0B6B3A7640000"


static BoatEthWallet *g_test_wallet_ptr;
static BoatEthTx g_test_tx_ptr;
static void Test_Protocol_EthApi_Init(void)
{
	BoatEthWalletConfig wallet_config;
	/* wallet_config value assignment */
	//strncpy( (char*)wallet_config.prikeyId, TEST_ETH_PRIKEYID, BOAT_KEYID_MAX_LEN - 1 );
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;

    strncpy( wallet_config.node_url_str, 
			 TEST_ETH_NODE_URL, 
			 BOAT_NODE_URL_MAX_LEN - 1 );
	g_test_wallet_ptr = BoatEthWalletInit(&wallet_config, sizeof(BoatEthWalletConfig));

	BoatEthTxInit(g_test_wallet_ptr,&g_test_tx_ptr,BOAT_TRUE,NULL,TEST_ETH_GASPRICE,TEST_ETH_WALLET_ADDR_1);

}

static void Test_Protocol_EthApi_DeInit(void)
{
	BoatEthWalletDeInit(g_test_wallet_ptr);
}

START_TEST(test_Api_WalletInit) {
	BoatEthWalletConfig wallet_config;
	/* wallet_config value assignment */
	//strncpy( (char*)wallet_config.prikeyId,  TEST_ETH_PRIKEYID,  BOAT_KEYID_MAX_LEN - 1 );
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;

    strncpy( wallet_config.node_url_str, 
			 TEST_ETH_NODE_URL, 
			 BOAT_NODE_URL_MAX_LEN - 1 );

	//case 1:		 
	BoatEthWallet *wallet_ptr_1;
	ck_assert((wallet_ptr_1 = BoatEthWalletInit(&wallet_config, sizeof(BoatEthWalletConfig))) != NULL);

	ck_assert(wallet_ptr_1->web3intf_context_ptr != NULL);
	//ck_assert_str_eq(wallet_ptr_1->account_info.prikeyId , TEST_ETH_PRIKEYID);
	ck_assert_str_eq(wallet_ptr_1->network_info.node_url_ptr, TEST_ETH_NODE_URL);
	ck_assert(wallet_ptr_1->network_info.chain_id == 1);
	ck_assert(wallet_ptr_1->network_info.eip155_compatibility == BOAT_FALSE);

	BoatEthWalletDeInit(wallet_ptr_1);

	//case 2:
	BoatEthWallet *wallet_ptr_2;
	ck_assert((wallet_ptr_2 = BoatEthWalletInit(NULL, sizeof(BoatEthWalletConfig))) == NULL);
	BoatEthWalletDeInit(wallet_ptr_2);
}
END_TEST

START_TEST(test_Api_SetNodeUrl) {

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
	ck_assert_str_eq(result_ptr.field_ptr,TEST_ETH_WALLET_BALANCE);
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
	//ck_assert_str_eq(tx_ptr.wallet_ptr->account_info.prikeyId ,TEST_ETH_PRIKEYID);
	ck_assert(tx_ptr.wallet_ptr->network_info.chain_id == 1);
	ck_assert(tx_ptr.wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);

	ck_assert_str_eq(tx_ptr.rawtx_fields.gasprice.field,TEST_ETH_GASPRICE);
	ck_assert_str_eq(tx_ptr.rawtx_fields.recipient,TEST_ETH_WALLET_ADDR_1);

	//case 2:
	ck_assert(BoatEthTxInit(g_test_wallet_ptr,&tx_ptr,BOAT_TRUE,NULL,NULL,TEST_ETH_WALLET_ADDR_1) == BOAT_SUCCESS);
	ck_assert_str_eq(tx_ptr.wallet_ptr->network_info.node_url_ptr ,TEST_ETH_NODE_URL);
	//ck_assert_str_eq(tx_ptr.wallet_ptr->account_info.prikeyId ,TEST_ETH_PRIKEYID);
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
	ck_assert(BoatEthTxSetNonce(&g_test_tx_ptr, NULL) == BOAT_ETH_NONCE_AUTO);

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

Suite * make_eth_suite(void) {
    Suite *s_ethereum = suite_create("ethereum");       // Create Suite
    TCase *tc_eth_api = tcase_create("eth_api");  // Create test cases
    suite_add_tcase(s_ethereum, tc_eth_api);           // Add a test case to the Suite
    tcase_add_test(tc_eth_api, test_Api_WalletInit);     // Test cases are added to the test set
	tcase_add_test(tc_eth_api, test_Api_SetNodeUrl); 
	tcase_add_test(tc_eth_api, test_Api_SetGasprice);
	tcase_add_test(tc_eth_api, test_Api_SetGaslimit);
	tcase_add_test(tc_eth_api, test_Api_SetRecipientAddr); 
	tcase_add_test(tc_eth_api, test_Api_SetValue); 
	tcase_add_test(tc_eth_api, test_Api_SetData); 
	tcase_add_test(tc_eth_api, test_Api_TxSend);
	tcase_add_test(tc_eth_api, test_Api_Transfer);
	tcase_add_test(tc_eth_api, test_Api_GetTransactionReceipt);


    //tcase_add_test(tc_eth_api, test_add);    
    return s_ethereum;
}

void Test_Protocol_Init(void)
{
	//eth
	Test_Protocol_EthApi_Init();
}

void Test_Protocol_DeInit(void)
{
	//eth
	Test_Protocol_EthApi_DeInit();
}

int main(int argc, char *argv[])
{
	Test_Protocol_Init();

    SRunner *sr;
    sr = srunner_create(make_eth_suite()); // Add Suite to SRunner
    srunner_run_all(sr, CK_NORMAL);
    int n = srunner_ntests_failed(sr);         // Run all the test cases
	Test_Protocol_DeInit();
    srunner_free(sr);

	return 0;
}
#else
int main(int argc, char *argv[])
{
	return 0;
}

#endif