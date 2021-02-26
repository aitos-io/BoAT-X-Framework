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

#include "boatinternal.h"
#include "StoreRead.h"

#include "testcommon.h"

//Gas price
#define TEST_ETH_GASPRICE_0	NULL
#define TEST_ETH_GASPRICE_1 "0x000333"
//Gas limit
#define TEST_ETH_GASLIMIT_0	"0x333333"
#define TEST_ETH_GASLIMIT_1	"0x000001"
#define TEST_ETH_GASLIMIT_2	"0xffffff"
//Contract Address
#define TEST_ETH_CONTRACT_ADDR_0 "0x6ecd4263d0c2e3f16279e6bf39276784bd06d3d5"
#define TEST_ETH_CONTRACT_ADDR_1 "0x6ecd4263d0c2e3f16279e6bf39276784bd06d3d4"
//Recipient Address
#define TEST_ETH_RECIPIENT_ADDR_0 "0xEF5D6d9D037Faa6493637E26A2B5d604f7722779"
#define TEST_ETH_RECIPIENT_ADDR_1 "0xEF5D6d9D037Faa6493637E26A2B5d604f7722778"
//Private key
#define TEST_ETH_PRIVATE_KEY_0 "0x88c40e712a2af0ac032aa17ba07963aa2c2f732ef2a88188930aa4e33151d3c9"
#define TEST_ETH_PRIVATE_KEY_1 "0x88c40e712a2af0ac032aa17ba07963aa2c2f732ef2a88188930aa4e33151d3c8"
//Node URL
#define TEST_ETH_NODE_URL_0 "HTTP://127.0.0.1:7545"
#define TEST_ETH_NODE_URL_1 "HTTP://127.1.1.1:7545"
//Wallet name
#define TEST_ETH_WALLET_NAME_0 "./tests/eth.key"
#define TEST_ETH_WALLET_NAME_1 "./test/eth/eth.key"
//Transfer value
#define TEST_ETH_TRANSFER_ETH_0 "0x2386F26FC10000"      // 0.01ETH or 1e16 wei, value
#define TEST_ETH_TRANSFER_ETH_1 "0xDE0B6B3A7640000"     // 1ETH or 1e18 wei, value
#define TEST_ETH_TRANSFER_ETH_2 "0x29A2241AF62C0000"    // 3ETH or 3e18 wei, value

#define TEST_ETH_PROTOCOL_0 BOAT_PROTOCOL_ETHEREUM
#define TEST_ETH_PROTOCOL_1 BOAT_PROTOCOL_PLATONE

BoatEthWallet *g_case_10_ethereum_wallet_ptr;

/*****************************************************************************************************************************************************************/

/*
Test case 0:
Test purpose:Test cases for proper operation,serve as templates for other test cases.
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumPreCondition_0(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);

    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, TEST_ETH_WALLET_NAME_0, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }    
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthereumTransfer_0(BoatEthWallet *wallet_ptr)
{
    BoatEthTx tx_ctx;    
    // Set Recipient Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_RECIPIENT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTransfer(&tx_ctx, TEST_ETH_TRANSFER_ETH_0),"BoatEthTransfer");

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_Call_ReadStore_0(BoatEthWallet *wallet_ptr)
{
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;

    BoatFieldVariable result;
    BUINT8 field_buf[128];
    result.field_ptr = field_buf;
    BCHAR * json_string;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (json_string = StoreRead_saveList(&tx_ctx, (BUINT8 *)"Hello world")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", json_string);
    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result.field_ptr);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),result.field_ptr,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result.field_ptr);
    UtilityHex2Bin(result_buff,sizeof(result_buff),(const char *)result.field_ptr,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"Hello world",strlen("Hello world")), "readListByIndex-memcmp" );
    
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_0(BSINT8 *wallet_num)
{

    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );

    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    printf("+++++++++++balance_wei.field_ptr:%s, balance_bef_a=%lf\n",balance_wei.field_ptr,balance_bef_a);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred " );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "ReadStore_0" );   
    
    return BOAT_SUCCESS;
}

/*
Test case 1:no private key
Test purpose:Test cases where the private key is not set when an Ethereum transaction and invoking contracts.
Test target results:The return value of BoateWalletGetBalance () is "0x0".
*/
BOAT_RESULT Case_10_EthereumPreCondition_1(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;
    /* 1:  The private key is not populated */
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);

    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, TEST_ETH_WALLET_NAME_0, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_1(BSINT8 *wallet_num)
{
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_1(wallet_num), "[Case_10_EthFunTest_1]:PreCondition_1" );
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    printf("+++++++++++json_string:%s\n",json_string);
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( (balance_bef_a == (double)0),"Case_10_EthereumTransfer A balance" );
    return BOAT_SUCCESS; 
}

/*
Test case 2: other private key
Test purpose:Test case for setting up other private keys when an Ethereum transaction and invoking contracts ,
Test target results:BoateThWalletGetBalance () returns the balance of the other private key accounts.
*/
BOAT_RESULT Case_10_EthereumPreCondition_2(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEY_1,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);

    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, TEST_ETH_WALLET_NAME_0, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_2(BSINT8 *wallet_num)
{
    BCHAR * json_string;
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_2(wallet_num), "[Case_10_EthFunTest_2]:PreCondition_2" );

    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    //todo
    printf("++++++++++++++++++++++ json_string:%s\n",json_string);
    return BOAT_SUCCESS; 
}

/*
Test case 3: chain_id = 0
Test purpose:Test cases where the chain ID is not set when an Ethereum transaction and contract is invoked,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumPreCondition_3(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    wallet_config.chain_id = 0;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);
   
    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, TEST_ETH_WALLET_NAME_0, &wallet_config, sizeof(BoatEthWalletConfig)); 
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;            
}

BOAT_RESULT Case_10_EthFunTest_3(BSINT8 *wallet_num)
{
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_3(wallet_num), "[Case_10_EthFunTest_3]:PreCondition_3" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );

    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );   
    
    return BOAT_SUCCESS; 
}

/*
Test case 4: URL = TEST_ETH_NODE_URL_1
Test purpose:Test case for setting other Node URLs on Ethereum transactions and calling contracts,
Test target results:The return value of BoateWalletGetBalance () is null.
*/
BOAT_RESULT Case_10_EthereumPreCondition_4(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr; 
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_1, BOAT_NODE_URL_MAX_LEN-1);
    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, TEST_ETH_WALLET_NAME_0, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_4(BSINT8 *wallet_num)
{   

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_4(wallet_num), "[Case_10_EthFunTest_4]:PreCondition_4" );
    //Get balance
    BoatDisplayTestResult( NULL == BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL), "BoatEthWalletGetBalance" );
    return BOAT_SUCCESS;        
}

/*
Test case 5: eip155=1
Test purpose:To set up test cases that comply with the EIP155 rule when an Ethereum transaction and invoking contracts,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumPreCondition_5(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr; 
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 1;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);

    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, TEST_ETH_WALLET_NAME_0, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_5(BSINT8 *wallet_num)
{
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_5(wallet_num), "[Case_10_EthFunTest_5]:PreCondition_5" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );

    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );
    
    return BOAT_SUCCESS; 
}

//test case 6:load wallet
/*
Test case 6: load wallet
Test purpose:Test the load wallet to send Ethereum transactions and invoke contracts,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumPreCondition_6(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr; 
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);

    // Load an existed wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, TEST_ETH_WALLET_NAME_0, NULL, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr; 
    *wallet_num =index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_6(BSINT8 *wallet_num)
{  
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_6(wallet_num), "[Case_10_EthFunTest_6]:PreCondition_6" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), " Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );

    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred " );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0))," A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0))," B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );
    
    return BOAT_SUCCESS; 
}

//test case 7: one-time wallet
/*
Test case 7: one-time wallet
Test purpose:Test one-time wallets sending Ethereum transactions and invoking contracts,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumPreCondition_7(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr; 

    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);

    // Test one-time wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, NULL, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr; 
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_7(BSINT8 *wallet_num)
{  
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_7(wallet_num), "[Case_10_EthFunTest_7]:PreCondition_7" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );

    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );  
    
    return BOAT_SUCCESS; 
}

//test case 8: wallet name path error
/*
Test case 8: wallet name path error
Test purpose:Test the impact of path errors on sending Ethereum transactions and invoking contracts when storing wallets,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumPreCondition_8(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEY_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URL_0, BOAT_NODE_URL_MAX_LEN-1);

    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(TEST_ETH_PROTOCOL_0, TEST_ETH_WALLET_NAME_1, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_10_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_8(BSINT8 *wallet_num)
{

    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_8(wallet_num), "[Case_10_EthFunTest_8]:PreCondition_8" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    
    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );  
    
    return BOAT_SUCCESS;     
}

//test case 9: is_sync_tx=BOAT_FALSE
/*
Test case 9: is_sync_tx=BOAT_FALSE
Test purpose:The tests performed Ethereum transactions and invoking contracts several times in a short time,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumTransfer_9(BoatEthWallet *wallet_ptr)
{
    
    BoatEthTx tx_ctx;    
    // Set Recipient Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_RECIPIENT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTransfer(&tx_ctx, TEST_ETH_TRANSFER_ETH_0),"BoatEthTransfer");
    return BOAT_SUCCESS; 
}

BOAT_RESULT Case_10_EthFunTest_9(BSINT8 *wallet_num)
{
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_9]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    
    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    
    return BOAT_SUCCESS; 
}


//test case 10: TEST_ETH_GASPRICE_1
/*
Test case 10: TEST_ETH_GASPRICE_1
Test purpose:Test the impact of setting a smaller GasPrice on sending Ethereum transactions and invoking contracts,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumTransfer_10(BoatEthWallet *wallet_ptr)
{
    
    BoatEthTx tx_ctx;    
    // Set Recipient Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_1,TEST_ETH_GASLIMIT_0,TEST_ETH_RECIPIENT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTransfer(&tx_ctx, TEST_ETH_TRANSFER_ETH_0),"BoatEthTransfer");
    return BOAT_SUCCESS; 
}

BOAT_RESULT Case_10_EthFunTest_10(BSINT8 *wallet_num)
{
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_10]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    
    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );    
    
    return BOAT_SUCCESS; 
}


//test case 11: TEST_ETH_GASLIMIT_1
/*
Test case 11: TEST_ETH_GASLIMIT_1
Test purpose:Test the impact of setting a smaller GasLimit on sending Ethereum transactions and invoking contracts,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumTransfer_11(BoatEthWallet *wallet_ptr)
{
    
    BoatEthTx tx_ctx;    
    // Set Recipient Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_1,TEST_ETH_RECIPIENT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTransfer(&tx_ctx, TEST_ETH_TRANSFER_ETH_0),"BoatEthTransfer");
    return BOAT_SUCCESS; 
}

BOAT_RESULT Case_10_EthFunTest_11(BSINT8 *wallet_num)
{  
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_11]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    
    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    
    return BOAT_SUCCESS; 
}



//test case 12: TEST_ETH_RECIPIENT_ADDR_1
/*
Test case 12: TEST_ETH_RECIPIENT_ADDR_1
Test purpose:Test the effect of setting other receiving addresses on sending Ethereum transactions and invoking contracts,
Test target results:The transfer failed. The balance of the receiving account remains unchanged
*/
BOAT_RESULT Case_10_EthereumTransfer_12(BoatEthWallet *wallet_ptr)
{
    
    BoatEthTx tx_ctx;    
    // Set Recipient Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_RECIPIENT_ADDR_1),"BoatEthTxInit");

    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTransfer(&tx_ctx, TEST_ETH_TRANSFER_ETH_0),"BoatEthTransfer");
    return BOAT_SUCCESS; 
}

BOAT_RESULT Case_10_EthFunTest_12(BSINT8 *wallet_num)
{ 
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_12]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_1)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_1)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    
    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) == 0.0001),"Case_10_EthereumTransfer B balance" );
    
    return BOAT_SUCCESS; 
}


//test case 13: TEST_ETH_TRANSFER_ETH_1
/*
Test case 13: TEST_ETH_TRANSFER_ETH_1
Test purpose:Test the effect of setting other transfer value on sending Ethereum transactions and invoking contracts,
Test target results:Both the transaction and the calling contract were successful.
*/
BOAT_RESULT Case_10_EthereumTransfer_13(BoatEthWallet *wallet_ptr)
{
    
    BoatEthTx tx_ctx;    
    // Set Recipient Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_RECIPIENT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTransfer(&tx_ctx, TEST_ETH_TRANSFER_ETH_1),"[EthereumTransfer_13]:BoatEthTransfer");
    return BOAT_SUCCESS; 
}

BOAT_RESULT Case_10_EthFunTest_13(BSINT8 *wallet_num)
{
    BoatFieldVariable balance_wei;
    BUINT8 balance_w[32];
    balance_wei.field_ptr = balance_w;
    BCHAR * json_string;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_13]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    printf("balance_bef_a:%lf\n",balance_bef_a);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_bef_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_13(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );
    balance_aft_a = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    printf("balance_aft_a:%lf\n",balance_aft_a);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (json_string = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &balance_wei ), "BoatEthPraseRpcResponseResult" );

    balance_aft_b = UtilityWeiStrToEthDouble((const char *)balance_wei.field_ptr);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_1)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_1)),"Case_10_EthereumTransfer B balance" );
    
    return BOAT_SUCCESS;
}


//test case 14: newevent = 64
/*
Test case 14: newevent = 64
Test purpose:Test the effect of an integer type on the received parameter types that invoke an Ethereum contract,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_10_Call_ReadStore_14(BoatEthWallet *wallet_ptr)
{
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;

    BoatFieldVariable result;
    BUINT8 field_buf[128];
    result.field_ptr = field_buf;
    BCHAR * json_string;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (json_string = StoreRead_saveList(&tx_ctx, (BUINT8 *)"64")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", json_string);
    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result.field_ptr);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),(const char *)result.field_ptr,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );    
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result.field_ptr);

    UtilityHex2Bin(result_buff,sizeof(result_buff),(const char *)result.field_ptr,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"64",strlen("64")), "readListByIndex-memcmp" );
    
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_14(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_14]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_14(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_14" );  
    return BOAT_SUCCESS; 
}


//test case 15: is_sync_tx = BOAT_FALSE
/*
Test case 15: is_sync_tx = BOAT_FALSE
Test purpose:The tests invoking contracts several times in a short time,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_10_Call_ReadStore_15(BoatEthWallet *wallet_ptr)
{
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;

    BoatFieldVariable result;
    BUINT8 field_buf[128];
    result.field_ptr = field_buf;
    BCHAR * json_string;

    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (json_string = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", json_string);
    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );
    
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result.field_ptr);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),(const char *)result.field_ptr,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );    
    
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result.field_ptr);

    UtilityHex2Bin(result_buff,sizeof(result_buff),(const char *)result.field_ptr,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"hello world",strlen("hello world")), "readListByIndex-memcmp" );
    
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_15(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_15]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_15(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_15" ); 
    return BOAT_SUCCESS;  
}

//test case 16: TEST_ETH_GASPRICE_1
/*
Test case 16: TEST_ETH_GASPRICE_1
Test purpose:Test the impact of GasPrice on invoking Ethereum contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_10_Call_ReadStore_16(BoatEthWallet *wallet_ptr)
{
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;

    BoatFieldVariable result;
    BUINT8 field_buf[128];
    result.field_ptr = field_buf;
    BCHAR * json_string;

    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_1,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (json_string = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", json_string);
    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result.field_ptr);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),(const char *)result.field_ptr,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result.field_ptr);

    UtilityHex2Bin(result_buff,sizeof(result_buff),(const char *)result.field_ptr,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"hello world",strlen("hello world")), "readListByIndex-memcmp" );
    
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_16(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_16]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_16(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_16" ); 
    return BOAT_SUCCESS;  
}

//test case 17: TEST_ETH_GASLIMIT_1
/*
Test case 17: TEST_ETH_GASLIMIT_1
Test purpose:Test the impact of setting a smaller GasLimit on invoking Ethereum contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_10_Call_ReadStore_17(BoatEthWallet *wallet_ptr)
{

    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;
    BoatFieldVariable result;
    BUINT8 field_buf[128];
    result.field_ptr = field_buf;
    BCHAR * json_string;

    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_1,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL == (json_string = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"StoreRead_saveList");

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_17(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_17]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_17(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_17" ); 
    return BOAT_SUCCESS;  
}

//test case 18: TEST_ETH_GASLIMIT_2
/*
Test case 18: TEST_ETH_GASLIMIT_2
Test purpose:Test the impact of setting a larger GasLimit on invoking Ethereum contracts,
Test target results:The calling contract were successful.
*/
BOAT_RESULT Case_10_Call_ReadStore_18(BoatEthWallet *wallet_ptr)
{
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;

    BoatFieldVariable result;
    BUINT8 field_buf[128];
    result.field_ptr = field_buf;
    BCHAR * json_string;

    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_2,TEST_ETH_CONTRACT_ADDR_0),"[ReadStore_18]:BoatEthTxInit");

    BoatDisplayTestResult(NULL == (json_string = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"[ReadStore_18]:StoreRead_saveList");

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_18(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_18]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_18(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_18" );  
    return BOAT_SUCCESS; 
}

//test case 19: TEST_ETH_CONTRACT_ADDR_1
/*
Test case 19: TEST_ETH_CONTRACT_ADDR_1
Test purpose:Test the effect of setting other contract addresses on invoking Ethereum contracts
Test target results:StoreRead_ReadListByIndex () reads a different value than it writes.
*/
BOAT_RESULT Case_10_Call_ReadStore_19(BoatEthWallet *wallet_ptr)
{
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;

    BoatFieldVariable result;
    BUINT8 field_buf[128];
    result.field_ptr = field_buf;
    BCHAR * json_string;    
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_1),"[ReadStore_19]:BoatEthTxInit");

    BoatDisplayTestResult(NULL != (json_string = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"[ReadStore_19]:StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", json_string);
    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListLength(&tx_ctx)),"[ReadStore_19]:StoreRead_readListLength");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result.field_ptr);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),(const char *)result.field_ptr,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (json_string = StoreRead_readListByIndex(&tx_ctx, index)),"[ReadStore_19]:StoreRead_readListByIndex");
    BoatDisplayTestResult( BOAT_SUCCESS == BoatEthPraseRpcResponseResult( json_string, "result", &result ), "BoatEthPraseRpcResponseResult" );
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result.field_ptr);

    UtilityHex2Bin(result_buff,sizeof(result_buff),(const char *)result.field_ptr,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 != memcmp(result_buff,"hello world",strlen("hello world")), "[ReadStore_19]:readListByIndex-memcmp" );
    
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_19(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_19]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_19(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_19" );  
    return BOAT_SUCCESS; 
}


/*****************************************************************************************************************************************************************/


BOAT_RESULT Case_10_EthFunMain(void)
{
    BOAT_RESULT case_result = BOAT_SUCCESS;
    BSINT8 wallet_num;

    case_result += Case_10_EthFunTest_0(&wallet_num);
    BoatWalletUnload(wallet_num);
    //Condition
    case_result += Case_10_EthFunTest_1(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_2(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_3(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_4(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_5(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_6(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_7(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_8(&wallet_num);
    BoatWalletUnload(wallet_num);
    //Transfer
    case_result += Case_10_EthFunTest_9(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_10(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_11(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_12(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_13(&wallet_num);
    BoatWalletUnload(wallet_num);
    //Contract
    case_result += Case_10_EthFunTest_14(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_15(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_16(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_17(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_18(&wallet_num);
    BoatWalletUnload(wallet_num);

    case_result += Case_10_EthFunTest_19(&wallet_num);
    BoatWalletUnload(wallet_num);

    BoatLog(BOAT_LOG_NORMAL, "case_result:%d.\n", case_result);

    //Delete wallet
    BoatWalletDelete(TEST_ETH_WALLET_NAME_0);

    
    return case_result;
}

