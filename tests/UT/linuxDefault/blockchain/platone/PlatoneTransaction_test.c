#include "tcase_platone.h"

#define EXCEED_STR_MAX_LEN          4097
#define TEST_PLATONE_CHAIN_ID       1
#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_IS_SYNC_TX             BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS      "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45"

START_TEST(test_005Transaction_0001GetSuccess) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;
    BCHAR *cur_balance_wei = NULL;
    BoatFieldVariable parse_result = {NULL, 0};

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, TEST_GAS_PRICE,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    cur_balance_wei = BoatPlatoneWalletGetBalance(g_platone_wallet_ptr, TEST_RECIPIENT_ADDRESS);
	result          = BoatPlatoneParseRpcResponseStringResult(cur_balance_wei, &parse_result);

    ck_assert_int_eq(result, BOAT_SUCCESS);
    ck_assert_ptr_ne(parse_result.field_ptr, NULL);

    BoatFree(parse_result.field_ptr);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0002GetWalletDefaultAddressSuccess) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;
    BCHAR *cur_balance_wei = NULL;
    BoatFieldVariable parse_result = {NULL, 0};

    BoatIotSdkInit();

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);


    cur_balance_wei = BoatPlatoneWalletGetBalance(g_platone_wallet_ptr, NULL);
	result          = BoatPlatoneParseRpcResponseStringResult(cur_balance_wei, &parse_result);

    ck_assert_ptr_ne(parse_result.field_ptr, NULL);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BoatFree(parse_result.field_ptr);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0003GetFailureNullWallet) 
{
    BCHAR *cur_balance_wei = NULL;

    BoatIotSdkInit();

    cur_balance_wei = BoatPlatoneWalletGetBalance(NULL, NULL);

    ck_assert_ptr_eq(cur_balance_wei, NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0004TransferSuccess) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatPlatoneTransfer(&tx_ctx, "0x0");
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0005TransferFailureNullParam) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatPlatoneTransfer(&tx_ctx, NULL);
    ck_assert_int_eq(result, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0006BoatPlatoneParseRpcResponseResultFail)
{
    BOAT_RESULT result;
    result = BoatPlatoneParseRpcResponseResult(NULL,NULL,NULL);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_005Transaction_0007TransferSuccessWithoutSync) 
{
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();    

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, BOAT_FALSE, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatPlatoneTransfer(&tx_ctx, "0x0");
    ck_assert_int_eq(result, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0008ParseJsonFailureNullParam)
{
    BOAT_RESULT result;
    BCHAR *json_string= "111";
    BCHAR *child_name = "";
    BoatFieldVariable *result_out = {NULL, 0};

    result = platone_parse_json_result(NULL, child_name, result_out);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    result = platone_parse_json_result(json_string, NULL, result_out);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    result = platone_parse_json_result(json_string, child_name, NULL);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);        
}
END_TEST

START_TEST(test_005Transaction_0009ObtainResultObjectFailure)
{
    BOAT_RESULT result;
    BCHAR *json_string= "111";
    BCHAR *child_name = "";
    BoatFieldVariable *result_out = {"1", 1};

    result = platone_parse_json_result(json_string, child_name, result_out);
    ck_assert(result = BOAT_ERROR_COMMON_INVALID_ARGUMENT);
   
}
END_TEST

START_TEST(test_005Transaction_0010WASMsetNameSuccess) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert(result == BOAT_SUCCESS);

    result_str = my_contract_cpp_abi_setName(&tx_ctx,"aitos");

    ck_assert_ptr_ne(result_str,NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0011WASMgetNameSuccess) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert(result == BOAT_SUCCESS); 

    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    
    ck_assert_ptr_ne(result_str,NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0012WASMsetNameNulltxctxFailure) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);


    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert(result == BOAT_SUCCESS);

    result_str = my_contract_cpp_abi_setName(NULL,"aitos");

    ck_assert_ptr_eq(result_str,NULL);

    result_str = my_contract_cpp_abi_setName(NULL, NULL);

    ck_assert_ptr_eq(result_str,NULL);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_005Transaction_0013WASMsetNameNullmsgFailure) 
{
    BCHAR *result_str;
    BOAT_RESULT result;
    BoatPlatoneTx tx_ctx;

    BoatIotSdkInit();

    BoatPlatoneWallet *g_platone_wallet_ptr = platone_get_wallet_ptr();
    ck_assert_int_ne(g_platone_wallet_ptr, NULL);

    result = BoatPlatoneTxInit(g_platone_wallet_ptr, &tx_ctx, TEST_IS_SYNC_TX, NULL,
                           TEST_GAS_LIMIT,
                           (BCHAR *)TEST_RECIPIENT_ADDRESS, BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);
    ck_assert(result == BOAT_SUCCESS);

    result_str = my_contract_cpp_abi_setName(&tx_ctx, NULL);

    ck_assert_ptr_eq(result_str,NULL);

    BoatIotSdkDeInit();
}
END_TEST

Suite *make_platoneTransactionTest_suite(void)
{
    /* Create Suite */
    Suite *s_transaction = suite_create("transaction");


    /* Create test cases */
    TCase *tc_transaction_api = tcase_create("transaction_api");

    tcase_set_timeout(tc_transaction_api,50);       

    /* Add a test case to the Suite */
    suite_add_tcase(s_transaction, tc_transaction_api);      
 
    /* Test cases are added to the test set */
    tcase_add_test(tc_transaction_api, test_005Transaction_0001GetSuccess); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0002GetWalletDefaultAddressSuccess); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0003GetFailureNullWallet); 

    tcase_add_test(tc_transaction_api, test_005Transaction_0004TransferSuccess); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0005TransferFailureNullParam); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0006BoatPlatoneParseRpcResponseResultFail); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0007TransferSuccessWithoutSync); 
    
    tcase_add_test(tc_transaction_api, test_005Transaction_0008ParseJsonFailureNullParam); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0009ObtainResultObjectFailure); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0010WASMsetNameSuccess); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0011WASMgetNameSuccess); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0012WASMsetNameNulltxctxFailure); 
    tcase_add_test(tc_transaction_api, test_005Transaction_0013WASMsetNameNullmsgFailure); 
    return s_transaction;
}

