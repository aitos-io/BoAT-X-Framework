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

#include "boatiotsdk.h"
#include "my_contract.cpp.abi.h"
#include "testcommon.h"
//BoatDisplayTestResult(is_pass, case_name_str)


//Gas price
#define TEST_PONE_GASPRICE_0	NULL
#define TEST_PONE_GASPRICE_1 	"0x000333"
//Gas limit
#define TEST_PONE_GASLIMIT_0	"0x333333"
#define TEST_PONE_GASLIMIT_1	"0x666666"
#define TEST_PONE_GASLIMIT_2	"0x111111"

//Contract Address
#define TEST_PONE_CONTRACT_ADDR_0 "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45"
#define TEST_PONE_CONTRACT_ADDR_1 "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e46"
//Private key
#define TEST_PONE_PRIVATE_KEY_0 "0xe55464c12b9e034ab00f7dddeb01874edcf514b3cd77a9ad0ad8796b4d3b1fdb"
#define TEST_PONE_PRIVATE_KEY_1 "0xe55464c12b9e034ab00f7dddeb01874edcf514b3cd77a9ad0ad8796b4d3b1fdf"
//Node URL
#define TEST_PONE_NODE_URL_0 "http://116.236.47.90:7545"
#define TEST_PONE_NODE_URL_1 "HTTP://127.1.1.1:7545"
//Wallet name
#define TEST_PONE_WALLET_NAME_0 "./tests/platone.key"
#define TEST_PONE_WALLET_NAME_1 "./tests/platone/platone.key"

#define TEST_PONE_PROTOCOL_0 BOAT_PROTOCOL_PLATONE
#define TEST_PONE_PROTOCOL_1 BOAT_PROTOCOL_ETHEREUM


BoatPlatoneWallet *g_case_15_platone_wallet_ptr;


//case 0:
/*
Test case 0:
Test purpose:Test cases for proper operation,serve as templates for other test cases
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_PlatonePreCondition_0(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_PONE_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(TEST_PONE_PROTOCOL_0, TEST_PONE_WALLET_NAME_0, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);

    g_case_15_platone_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    *wallet_num = index;
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_15_Call_my_contract_0(BoatPlatoneWallet *wallet_ptr)
{
    BoatPlatoneTx tx_ctx;
    BOAT_RESULT result;

    BCHAR *json_string;
    BoatFieldVariable result_str;
    BUINT8 field_buf[128];
    result_str.field_ptr = field_buf;
    // Set Contract Address
    BoatDisplayTestResult (BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,
                                                            &tx_ctx,
                                                            BOAT_TRUE,
                                                            NULL,
                                                            TEST_PONE_GASLIMIT_0,
                                                            TEST_PONE_CONTRACT_ADDR_0,
                                                            BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR),"BoatPlatoneTxInit");

    BoatDisplayTestResult(NULL != (json_string = my_contract_cpp_abi_setName(&tx_ctx, "Hello World")),"my_contract_cpp_abi_setName");
    BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", json_string);

    
    BoatDisplayTestResult(NULL != (json_string = my_contract_cpp_abi_getName(&tx_ctx)),"my_contract_cpp_abi_getName");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatonePraseRpcResponseResult( json_string, "result", &result_str ), "BoatPlatonePraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str.field_ptr);

    BoatDisplayTestResult( 0 == memcmp(result_str.field_ptr,"0x48656c6c6f20576f726c64",strlen("0x48656c6c6f20576f726c64")), "my_contract_cpp_abi_getName" );
 
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_0(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_0(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

/*
Test case 1:load wallet
Test purpose:Test the load wallet to invoke PlatNOE contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_PlatonePreCondition_1(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWallet *wallet_ptr;

    index = BoatWalletCreate(TEST_PONE_PROTOCOL_0, TEST_PONE_WALLET_NAME_0, NULL, sizeof(BoatPlatoneWalletConfig));   
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }     
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_15_platone_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_15_PlatoneFunTest_1(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_1(wallet_num), "Case_15_PlatonePreCondition_1" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}


//case 2:no private_key
/*
Test case 2: no private_key
Test purpose:Test cases where the private key is not set when invoking PlatNOE contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_PlatonePreCondition_2(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;        

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(TEST_PONE_PROTOCOL_0, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));   
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");      
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_15_platone_wallet_ptr = wallet_ptr;
    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_2(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_2(wallet_num), "Case_15_PlatonePreCondition_2" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

/*
Test case 3: other private key
Test purpose:Test case for setting up other private keys when invoking PlatNOE contracts ,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_PlatonePreCondition_3(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_PONE_PRIVATE_KEY_1,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(TEST_PONE_PROTOCOL_0, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_15_platone_wallet_ptr = wallet_ptr;
    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    *wallet_num = index;
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_15_PlatoneFunTest_3(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_3(wallet_num), "Case_15_PlatonePreCondition_3" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

//case 4:chain id
/*
Test case 4: chain_id = 0
Test purpose:Test cases where the chain ID is not set when invoking PlatNOE contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_PlatonePreCondition_4(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_PONE_PRIVATE_KEY_1,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    
    wallet_config.chain_id = 0;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(TEST_PONE_PROTOCOL_0, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_15_platone_wallet_ptr = wallet_ptr;
    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    *wallet_num = index;
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_15_PlatoneFunTest_4(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_4(wallet_num), "Case_15_PlatonePreCondition_4" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}


//case 5:EIP155
/*
Test case 5: eip155=1
Test purpose:To set up test cases that comply with the EIP155 rule when invoking PlatNOE contracts,
Test target results:The calling contract were failed.
*/
BOAT_RESULT Case_15_PlatonePreCondition_5(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_PONE_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 1;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(TEST_PONE_PROTOCOL_0, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);

    g_case_15_platone_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    *wallet_num = index;
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_15_PlatoneFunTest_5(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_5(wallet_num), "Case_15_PlatonePreCondition_5" );

    BoatDisplayTestResult( BOAT_SUCCESS != Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

//case 6:url
/*
Test case 6: URL = TEST_ETH_NODE_URL_1
Test purpose:Test case for setting other Node URLs when invoking PlatNOE contracts,
Test target results:The calling contract were failed.
*/
BOAT_RESULT Case_15_PlatonePreCondition_6(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_PONE_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_1, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(TEST_PONE_PROTOCOL_0, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);

    g_case_15_platone_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    *wallet_num = index;
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_15_PlatoneFunTest_6(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_6(wallet_num), "Case_15_PlatonePreCondition_6" );

    BoatDisplayTestResult( BOAT_SUCCESS != Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

//case 7:wallet_type
/*
Test case 7: load wallet
Test purpose:Test the load wallet to invoke PlatNOE contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_PlatonePreCondition_7(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_PONE_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(TEST_PONE_PROTOCOL_1, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_15_platone_wallet_ptr = wallet_ptr;
    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    *wallet_num = index;
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_15_PlatoneFunTest_7(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_7(wallet_num), "Case_15_PlatonePreCondition_7" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

//case 8:one time
/*
Test case 8: one-time wallet
Test purpose:Test one-time wallets to invoke PlatNOE contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_PlatonePreCondition_8(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_PONE_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(TEST_PONE_PROTOCOL_0, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);

    g_case_15_platone_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    *wallet_num = index;
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_15_PlatoneFunTest_8(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_8(wallet_num), "Case_15_PlatonePreCondition_8" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}



//case 9:is_sync_tx
/*
Test case 9: is_sync_tx=BOAT_TRUE
Test purpose:Test the effect of the transaction state parameter on the invoking PlatNOE contract,
Test target results:The calling contract were failed.
*/
BOAT_RESULT Case_15_Call_my_contract_9(BoatPlatoneWallet *wallet_ptr)
{
    BoatPlatoneTx tx_ctx;

    BCHAR *json_string;
    BoatFieldVariable result_str;
    BUINT8 field_buf[128];
    result_str.field_ptr = field_buf;    
    // Set Contract Address
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,NULL,TEST_PONE_GASLIMIT_0,TEST_PONE_CONTRACT_ADDR_0,BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit" );

    BoatDisplayTestResult( NULL == ( json_string = my_contract_cpp_abi_setName(NULL, "Hello World")), "my_contract_cpp_abi_setName" );   
    BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", json_string);

    BoatDisplayTestResult( NULL != ( json_string = my_contract_cpp_abi_getName(&tx_ctx)), "my_contract_cpp_abi_getName" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatonePraseRpcResponseResult( json_string, "result", &result_str ), "BoatPlatonePraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str.field_ptr);

    BoatDisplayTestResult( 0 == memcmp(result_str.field_ptr,"0x48656c6c6f20576f726c64",strlen("0x48656c6c6f20576f726c64")), "my_contract_cpp_abi_getName" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_9(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_0(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_9(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_9" );
    return BOAT_SUCCESS;
}

//case 10:TEST_PONE_GASPRICE_1
/*
Test case 10: TEST_PONE_GASPRICE_1
Test purpose:Test the impact of setting a smaller GasPrice on the invoking PlatNOE contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_Call_my_contract_10(BoatPlatoneWallet *wallet_ptr)
{
    BoatPlatoneTx tx_ctx;

    BCHAR *json_string;
    BoatFieldVariable result_str;
    BUINT8 field_buf[128];
    result_str.field_ptr = field_buf;        
    // Set Contract Address
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_PONE_GASPRICE_1,TEST_PONE_GASLIMIT_0,TEST_PONE_CONTRACT_ADDR_0,BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit" );

    BoatDisplayTestResult( NULL == ( json_string = my_contract_cpp_abi_setName(NULL, "Hello World")), "my_contract_cpp_abi_setName" );   
    BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", json_string);


    BoatDisplayTestResult( NULL != ( json_string = my_contract_cpp_abi_getName(&tx_ctx)), "my_contract_cpp_abi_getName" );   
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatonePraseRpcResponseResult( json_string, "result", &result_str ), "BoatPlatonePraseRpcResponseResult" );
    
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str.field_ptr);

    BoatDisplayTestResult( 0 == memcmp(result_str.field_ptr,"0x48656c6c6f20576f726c64",strlen("0x48656c6c6f20576f726c64")), "my_contract_cpp_abi_getName" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_10(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_0(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_10(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_10" );
    return BOAT_SUCCESS;
}

//case 11:TEST_PONE_GASLIMIT_1
/*
Test case 11: TEST_PONE_GASLIMIT_1
Test purpose:Test the impact of setting a smaller GasLimit on the invoking PlatNOE contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_Call_my_contract_11(BoatPlatoneWallet *wallet_ptr)
{

    BoatPlatoneTx tx_ctx;

    BCHAR *json_string;
    BoatFieldVariable result_str;
    BUINT8 field_buf[128];
    result_str.field_ptr = field_buf;          
    // Set Contract Address
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_PONE_GASPRICE_0,TEST_PONE_GASLIMIT_1,TEST_PONE_CONTRACT_ADDR_0,BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit" );

    BoatDisplayTestResult( NULL == ( json_string = my_contract_cpp_abi_setName(NULL, "Hello World")), "my_contract_cpp_abi_setName" );   
    BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", json_string);


    BoatDisplayTestResult( NULL != ( json_string = my_contract_cpp_abi_getName(&tx_ctx)), "my_contract_cpp_abi_getName" );   
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatonePraseRpcResponseResult( json_string, "result", &result_str ), "BoatPlatonePraseRpcResponseResult" );
    
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str.field_ptr);

    BoatDisplayTestResult( 0 == memcmp(result_str.field_ptr,"0x48656c6c6f20576f726c64",strlen("0x48656c6c6f20576f726c64")), "my_contract_cpp_abi_getName" );

    return BOAT_SUCCESS;
}


BOAT_RESULT Case_15_PlatoneFunTest_11(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_1(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_11(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_11" );
    return BOAT_SUCCESS;
}

//case 11:TEST_PONE_CONTRACT_ADDR_1
/*
Test case 11: TEST_PONE_CONTRACT_ADDR_1
Test purpose:Test the effect of setting other contract addresses on the invoking PlatNOE contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_15_Call_my_contract_12(BoatPlatoneWallet *wallet_ptr)
{
    BoatPlatoneTx tx_ctx;
    BCHAR *json_string;
    // Set Contract Address
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_PONE_GASPRICE_0,TEST_PONE_GASLIMIT_0,TEST_PONE_CONTRACT_ADDR_1,BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit" );

    BoatDisplayTestResult( NULL == ( json_string = my_contract_cpp_abi_setName(NULL, "Hello World")), "my_contract_cpp_abi_setName" );   

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_12(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_0(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_12(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_12" );
    return BOAT_SUCCESS;
}



BOAT_RESULT Case_15_PlatONEMain(void)
{
    BOAT_RESULT case_result = BOAT_SUCCESS;
    BSINT8 wallet_num=0;
    case_result += Case_15_PlatoneFunTest_0(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_1(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_2(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_3(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_4(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_5(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_6(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_7(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_8(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_9(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_10(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_11(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_15_PlatoneFunTest_12(&wallet_num);
    BoatWalletUnload(wallet_num);

    BoatLog(BOAT_LOG_NORMAL, "case_result: %d.", case_result);
    //Delete wallet
    BoatWalletDelete(TEST_PONE_WALLET_NAME_0);
    
    return case_result;
}

