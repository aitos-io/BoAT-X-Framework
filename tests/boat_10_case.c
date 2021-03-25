#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "boattypes.h"

#include "protocolapi/api_ethereum.h"
#include "boatutility.h"
#include "check.h"




#define TEST_ETH_PRIKEYID		"/mnt/sharework/boatiotsdk_fabric/demo/demo_key/ethereum_client.key"
#define TEST_ETH_NODE_URL		"HTTP://127.0.0.1:7545"

#define TEST_ETH_WALLET_ADDR_0	"0xEF5D6d9D037Faa6493637E26A2B5d604f7722779"
#define TEST_ETH_WALLET_ADDR_1	"0xEF5D6d9D037Faa6493637E26A2B5d604f7722779"

#define TEST_ETH_RECIPIENT_ADDR	"0xEF5D6d9D037Faa6493637E26A2B5d604f7722779"

//! @todo 
#define TEST_ETH_WALLET_BALANCE	"0xEF5D6d9D037F"  

#define TEST_ETH_GASPRICE 		"0x333333"
#define TEST_ETH_GASLIMIT 		"0x333333"
#define TEST_ETH_VALUE			"0x333333"


START_TEST(test_Protocol_EthApi) {
	BoatEthWalletConfig wallet_config;
		/* wallet_config value assignment */
	strncpy( (char*)wallet_config.prikeyId, 
			 TEST_ETH_PRIKEYID, 
			 BOAT_KEYID_MAX_LEN - 1 );

    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;

    strncpy( wallet_config.node_url_str, 
			 TEST_ETH_NODE_URL, 
			 BOAT_NODE_URL_MAX_LEN - 1 );
			 
	BoatEthWallet *wallet_ptr;
	ck_assert((wallet_ptr = BoatEthWalletInit(&wallet_config, sizeof(BoatEthWalletConfig))) != NULL);

	ck_assert(wallet_ptr->web3intf_context_ptr != NULL);
	ck_assert_str_eq(wallet_ptr->account_info.prikeyId , TEST_ETH_PRIKEYID);
	ck_assert_str_eq(wallet_ptr->network_info.node_url_ptr, TEST_ETH_NODE_URL);
	ck_assert(wallet_ptr->network_info.chain_id == 1);
	ck_assert(wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);







	//Node url	
	memset(wallet_ptr->network_info.node_url_ptr,0,strlen(wallet_ptr->network_info.node_url_ptr));
	ck_assert(BoatEthWalletSetNodeUrl(wallet_ptr, TEST_ETH_NODE_URL) == BOAT_SUCCESS);
	ck_assert_str_eq(wallet_ptr->network_info.node_url_ptr,TEST_ETH_NODE_URL);







	//Eip155
	wallet_ptr->network_info.eip155_compatibility = BOAT_TRUE;
	ck_assert(BoatEthWalletSetEIP155Comp(wallet_ptr, BOAT_FALSE ) == BOAT_SUCCESS);
	ck_assert(wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);
	//Chain id 
	wallet_ptr->network_info.chain_id = 0xFF;
	ck_assert(BoatEthWalletSetChainId(wallet_ptr, 0 ) == BOAT_SUCCESS);
	ck_assert(wallet_ptr->network_info.chain_id == BOAT_FALSE);

	//Get balance
	BCHAR *wallet_balance_ptr;
	BoatFieldVariable result_ptr;
	result_ptr.field_len = 128;
	result_ptr.field_ptr = BoatMalloc(result_ptr.field_len);
	ck_assert((wallet_balance_ptr = BoatEthWalletGetBalance(wallet_ptr, TEST_ETH_WALLET_ADDR_0)) != NULL);
	ck_assert(BoatEthPraseRpcResponseResult( wallet_balance_ptr,"result",&result_ptr) == BOAT_SUCCESS);
	//! @todo 
	ck_assert_str_eq(result_ptr.field_ptr,TEST_ETH_WALLET_BALANCE);
	BoatFree(result_ptr.field_ptr);
	//tx init
	BoatEthTx tx_ptr;					  
	ck_assert(BoatEthTxInit(wallet_ptr,&tx_ptr,BOAT_TRUE,NULL,TEST_ETH_GASPRICE,TEST_ETH_WALLET_ADDR_1) == BOAT_SUCCESS);

	ck_assert_str_eq(tx_ptr.wallet_ptr->network_info.node_url_ptr ,TEST_ETH_NODE_URL);
	ck_assert_str_eq(tx_ptr.wallet_ptr->account_info.prikeyId ,TEST_ETH_PRIKEYID);
	ck_assert(tx_ptr.wallet_ptr->network_info.chain_id == 1);
	ck_assert(tx_ptr.wallet_ptr->network_info.eip155_compatibility == BOAT_FALSE);

	ck_assert_str_eq(tx_ptr.rawtx_fields.gasprice.field,TEST_ETH_GASPRICE);
	ck_assert_str_eq(tx_ptr.rawtx_fields.recipient,TEST_ETH_WALLET_ADDR_1);

	//Set Nonce
	BUINT64 nonce=0x12;
	ck_assert(BoatEthTxSetNonce(&tx_ptr,nonce) == BOAT_SUCCESS);
	ck_assert_str_eq(tx_ptr.rawtx_fields.nonce.field,&nonce);

	//Set Gasprice
	BoatFieldMax32B gas_price_ptr;
	ck_assert(UtilityHex2Bin(gas_price_ptr.field,32,TEST_ETH_GASPRICE,TRIMBIN_LEFTTRIM,BOAT_TRUE) != 0);
	ck_assert(BoatEthTxSetGasPrice(&tx_ptr,gas_price_ptr.field) == BOAT_SUCCESS);
	ck_assert_str_eq(tx_ptr.rawtx_fields.gasprice.field,gas_price_ptr.field);

	//Set Gasprice
	//BOAT_RESULT BoatEthTxSetGasLimit(BoatEthTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
	BoatFieldMax32B gas_limit_ptr;
	ck_assert(UtilityHex2Bin(gas_limit_ptr.field,32,TEST_ETH_GASLIMIT,TRIMBIN_LEFTTRIM,BOAT_TRUE) != 0);
	ck_assert(BoatEthTxSetGasPrice(&tx_ptr,gas_limit_ptr.field) == BOAT_SUCCESS);
	ck_assert_str_eq(tx_ptr.rawtx_fields.gaslimit.field,gas_limit_ptr.field);

	BUINT8 recipient_address[BOAT_ETH_ADDRESS_SIZE];
	ck_assert(UtilityHex2Bin(recipient_address,20,TEST_ETH_RECIPIENT_ADDR,TRIMBIN_TRIM_NO,BOAT_TRUE) != 0);
	ck_assert(BoatEthTxSetRecipient(&tx_ptr,recipient_address) == BOAT_SUCCESS);
	ck_assert_str_eq(tx_ptr.rawtx_fields.recipient,recipient_address);

	BoatFieldMax32B value_ptr;
	ck_assert(UtilityHex2Bin(value_ptr.field,32,TEST_ETH_VALUE,TRIMBIN_LEFTTRIM,BOAT_TRUE) != 0);
	ck_assert(BoatEthTxSetGasPrice(&tx_ptr,value_ptr.field) == BOAT_SUCCESS);
	ck_assert_str_eq(tx_ptr.rawtx_fields.value.field,value_ptr.field);

/*
BOAT_RESULT BoatEthTxSetData(BoatEthTx *tx_ptr, BoatFieldVariable *data_ptr)

BOAT_RESULT BoatEthTxSend(BoatEthTx *tx_ptr)
BCHAR * BoatEthCallContractFunc( BoatEthTx *tx_ptr, BCHAR *func_proto_str,
								 BUINT8 *func_param_ptr, BUINT32 func_param_len )
BOAT_RESULT BoatEthTransfer(BoatEthTx *tx_ptr, BCHAR * value_hex_str)
BOAT_RESULT BoatEthGetTransactionReceipt(BoatEthTx *tx_ptr)
*/
	BoatEthWalletDeInit(wallet_ptr);
}



Suite * make_add_suite(void) {
    Suite *seth_api = suite_create("eth_api");       // 建立Suite
    TCase *tc_math = tcase_create("ethereum");  // 建立测试用例�
    suite_add_tcase(s, tc_math);           // 将测试用例加到Suite�
    tcase_add_test(tc_math, test_sub);     // 测试用例加到测试集中
    tcase_add_test(tc_math, test_add);     // 测试用例加到测试集中
    return s;
}



int main(int argc, char *argv[])
{
	BCHAR    to_str[128];
	BUINT8   from_str[128];
	BUINT32  from_str_len;

	memset(to_str  , 0, sizeof(to_str)/sizeof(to_str[0]));
	memset(from_str, 0, sizeof(from_str)/sizeof(from_str[0]));
	from_str[0]  = 0x00;
    from_str[1]  = 0x00;
	from_str[2]  = 0x01;
	from_str[3]  = 0xAB;
	from_str_len = 4;

    SRunner *sr;
    sr = srunner_create(make_add_suite()); // 将Suite加入到SRunner
    srunner_run_all(sr, CK_NORMAL);
    int n = srunner_ntests_failed(sr);         // 运行所有测试用�
    srunner_free(sr);
 

	return 0;
}







int sub(int a, int b) {
    return a-b;
} 
int add(int a, int b) {
    return a+b;
}
 
START_TEST(test_sub) {
    ck_assert_msg(sub(7, 2) == 5, "error, 7 - 2 != 5"); 
    ck_assert_msg(sub(6, 2) == 3, "error, 6 - 2 != 4");
}
END_TEST
 
START_TEST(test_add) {
    ck_assert(add(7, 2) == 9);
    ck_assert(add(7, 5) == 12); 
}
END_TEST








