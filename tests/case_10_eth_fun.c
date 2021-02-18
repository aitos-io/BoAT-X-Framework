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


#define TEST_ETH_GASPRICE_0	"0x111111"
#define TEST_ETH_GASPRICE_1 "0x222222"

#define TEST_ETH_GASLIMIT_0	"0x111111"
#define TEST_ETH_GASLIMIT_1	"0x222222"
#define TEST_ETH_GASLIMIT_2	"0x333333"

//Contract Address
#define TEST_ETH_CONTRACT_ADDR_0 "0x1234567812345678123456781234567812345678"
#define TEST_ETH_CONTRACT_ADDR_1 "0x1234567812345678123456781234567812341234"
//Recipient Address
#define TEST_ETH_RECIPIENT_ADDR_0 "0x1234123412341234123412341234123412341234"
#define TEST_ETH_RECIPIENT_ADDR_1 "0x1234123412341234123412341234123412345678"

#define TEST_ETH_PRIVATE_KEY_0 "0x1234567812345678123456781234567812345678123456781234567812345678"
#define TEST_ETH_PRIVATE_KEY_1 "0x1234123412341234123412341234123412341234123412341234123412341234"

#define TEST_ETH_NODE_URL_0 "HTTP://127.0.0.1:7545"
#define TEST_ETH_NODE_URL_1 "HTTP://127.1.1.1:7545"

#define TEST_ETH_WALLET_NAME_0 "./tests/eth.key"
#define TEST_ETH_WALLET_NAME_1 "./tests/eth/eth.key"

#define TEST_ETH_PROTOCOL_0 BOAT_PROTOCOL_ETHEREUM
#define TEST_ETH_PROTOCOL_1 BOAT_PROTOCOL_PLATONE

#define TEST_ETH_TRANSFER_ETH_0 "0x2386F26FC10000"      // 0.01ETH or 1e16 wei, value
#define TEST_ETH_TRANSFER_ETH_1 "0xDE0B6B3A7640000"     // 1ETH or 1e18 wei, value
#define TEST_ETH_TRANSFER_ETH_2 "0x29A2241AF62C0000"    // 3ETH or 3e18 wei, value

BoatEthWallet *g_case_10_ethereum_wallet_ptr;

/*****************************************************************************************************************************************************************/

//test case 0:
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
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (result_str = StoreRead_saveList(&tx_ctx, (BUINT8 *)"Hello world")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", result_str);
    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),result_str,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result_str);

    UtilityHex2Bin(result_buff,sizeof(result_buff),result_str,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"Hello world",strlen("Hello world")), "readListByIndex-memcmp" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_0(BSINT8 *wallet_num)
{
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred " );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "ReadStore_0" );   
    return BOAT_SUCCESS;
}


//test case 1: no private key
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
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_1(wallet_num), "[Case_10_EthFunTest_1]:PreCondition_1" );
    return BOAT_SUCCESS; 
}


//test case 2: other private key
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
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_2(wallet_num), "[Case_10_EthFunTest_2]:PreCondition_2" );
    //BCHAR * balance_wei;
    //BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    return BOAT_SUCCESS; 
}

//test case 3:chain_id = 0
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_3(wallet_num), "[Case_10_EthFunTest_3]:PreCondition_3" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );   
    return BOAT_SUCCESS; 
}

//test case 4:URL = TEST_ETH_NODE_URL_1
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

//test case 5:eip155=1
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_5(wallet_num), "[Case_10_EthFunTest_5]:PreCondition_5" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );
    return BOAT_SUCCESS; 
}

//test case 6:load wallet
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_6(wallet_num), "[Case_10_EthFunTest_6]:PreCondition_6" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), " Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred " );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0))," A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0))," B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );
    return BOAT_SUCCESS; 
}

//test case 7: one-time wallet
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_7(wallet_num), "[Case_10_EthFunTest_7]:PreCondition_7" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );  
    return BOAT_SUCCESS; 
}

//test case 8: wallet name path error
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

    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_8(wallet_num), "[Case_10_EthFunTest_8]:PreCondition_8" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" );  
    return BOAT_SUCCESS;     
}

//test case 9: is_sync_tx=BOAT_FALSE
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_9]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    return BOAT_SUCCESS; 
}


//test case 10: TEST_ETH_GASPRICE_1
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_10]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );    

    return BOAT_SUCCESS; 
}


//test case 11: TEST_ETH_GASLIMIT_1
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_11]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    return BOAT_SUCCESS; 
}



//test case 12: TEST_ETH_RECIPIENT_ADDR_1
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_12]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_0(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_0)),"Case_10_EthereumTransfer B balance" );
    return BOAT_SUCCESS; 
}


//test case 13: TEST_ETH_TRANSFER_ETH_1
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
    BCHAR * balance_wei;
    //A account
    double balance_bef_a=0.0;
    double balance_aft_a=0.0;
    //B account
    double balance_bef_b=0.0;
    double balance_aft_b=0.0;

    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_13]:PreCondition_0" );
    //Balance of Account A before transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "Account A before the transfer" );
    balance_bef_a = UtilityWeiStrToEthDouble(balance_wei);
    printf("balance_bef_a:%lf\n",balance_bef_a);
    //Balance of Account B before transfer 
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "Account B before the transfer" );
    balance_bef_b = UtilityWeiStrToEthDouble(balance_wei);
    //Account A transfers 0.01ETH to Account B
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumTransfer_13(g_case_10_ethereum_wallet_ptr), "Case_10_EthereumTransfer_0" );
    //Balance of account A after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, NULL)), "After account A is transferred" );
    balance_aft_a = UtilityWeiStrToEthDouble(balance_wei);
    printf("balance_aft_a:%lf\n",balance_aft_a);
    //Balance of account B after transfer
    BoatDisplayTestResult( NULL != (balance_wei = BoatEthWalletGetBalance(g_case_10_ethereum_wallet_ptr, TEST_ETH_RECIPIENT_ADDR_0)), "After account B is transferred" );
    balance_aft_b = UtilityWeiStrToEthDouble(balance_wei);
    //Calculate the difference before and after the transfer
    BoatDisplayTestResult( ((balance_bef_a - balance_aft_a + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_1)),"Case_10_EthereumTransfer A balance" );
    BoatDisplayTestResult( ((balance_aft_b - balance_bef_b + 0.0001) >= UtilityWeiStrToEthDouble(TEST_ETH_TRANSFER_ETH_1)),"Case_10_EthereumTransfer B balance" );
    
    return BOAT_SUCCESS;
}


//test case 14: newevent = 64
BOAT_RESULT Case_10_Call_ReadStore_14(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (result_str = StoreRead_saveList(&tx_ctx, (BUINT8 *)"64")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", result_str);
    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),result_str,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result_str);

    UtilityHex2Bin(result_buff,sizeof(result_buff),result_str,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"64",strlen("64")), "readListByIndex-memcmp" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_14(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_14]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_14(g_case_10_ethereum_wallet_ptr), "[Case_10_EthFunTest_14]:Case_10_Call_ReadStore_14" );  
    return BOAT_SUCCESS; 
}


//test case 15: is_sync_tx = BOAT_FALSE
BOAT_RESULT Case_10_Call_ReadStore_15(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (result_str = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", result_str);
    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),result_str,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result_str);

    UtilityHex2Bin(result_buff,sizeof(result_buff),result_str,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"Hello world",strlen("Hello world")), "readListByIndex-memcmp" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_15(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_15]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" ); 
    return BOAT_SUCCESS;  
}

//test case 16: TEST_ETH_GASPRICE_1
BOAT_RESULT Case_10_Call_ReadStore_16(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_1,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (result_str = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", result_str);
    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),result_str,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result_str);

    UtilityHex2Bin(result_buff,sizeof(result_buff),result_str,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"Hello world",strlen("Hello world")), "readListByIndex-memcmp" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_16(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_16]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" ); 
    return BOAT_SUCCESS;  
}

//test case 17: TEST_ETH_GASLIMIT_1
BOAT_RESULT Case_10_Call_ReadStore_17(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_1,TEST_ETH_CONTRACT_ADDR_0),"BoatEthTxInit");

    BoatDisplayTestResult(NULL != (result_str = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", result_str);
    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListLength(&tx_ctx)),"StoreRead_readListLength");
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),result_str,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListByIndex(&tx_ctx, index)),"StoreRead_readListByIndex");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result_str);

    UtilityHex2Bin(result_buff,sizeof(result_buff),result_str,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 == memcmp(result_buff,"Hello world",strlen("Hello world")), "readListByIndex-memcmp" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_17(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_17]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_0(g_case_10_ethereum_wallet_ptr), "Case_10_Call_ReadStore_0" ); 
    return BOAT_SUCCESS;  
}

//test case 18: TEST_ETH_GASLIMIT_2
BOAT_RESULT Case_10_Call_ReadStore_18(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_2,TEST_ETH_CONTRACT_ADDR_0),"[ReadStore_18]:BoatEthTxInit");

    BoatDisplayTestResult(NULL != (result_str = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"[ReadStore_18]:StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", result_str);
    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListLength(&tx_ctx)),"[ReadStore_18]:StoreRead_readListLength");
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),result_str,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListByIndex(&tx_ctx, index)),"[ReadStore_18]:StoreRead_readListByIndex");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result_str);

    UtilityHex2Bin(result_buff,sizeof(result_buff),result_str,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 != memcmp(result_buff,"Hello world",strlen("Hello world")), "[ReadStore_18]:readListByIndex-memcmp" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_18(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_18]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_18(g_case_10_ethereum_wallet_ptr), "[Case_10_EthFunTest_18]:ReadStore_18" );  
    return BOAT_SUCCESS; 
}

//test case 19: TEST_ETH_CONTRACT_ADDR_1
BOAT_RESULT Case_10_Call_ReadStore_19(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT8  result_buff[127];
    BUINT32 index;
    BoatEthTx tx_ctx;
    // Set Contract Address
    BoatDisplayTestResult(BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,TEST_ETH_GASPRICE_0,TEST_ETH_GASLIMIT_0,TEST_ETH_CONTRACT_ADDR_1),"[ReadStore_19]:BoatEthTxInit");

    BoatDisplayTestResult(NULL != (result_str = StoreRead_saveList(&tx_ctx, (BUINT8 *)"hello world")),"[ReadStore_19]:StoreRead_saveList");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_saveList returns: %s", result_str);
    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListLength(&tx_ctx)),"[ReadStore_19]:StoreRead_readListLength");
    BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);

    UtilityHex2Bin((BUINT8*)index_u256_big,sizeof(index_u256_big),result_str,TRIMBIN_TRIM_NO,BOAT_FALSE);
    UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));            
    list_len = index_u256_big[0];
    index = list_len-1;

    BoatDisplayTestResult(NULL != (result_str = StoreRead_readListByIndex(&tx_ctx, index)),"[ReadStore_19]:StoreRead_readListByIndex");
    BoatLog(BOAT_LOG_NORMAL, "StoreRead_readListByIndex returns: %s\n", result_str);

    UtilityHex2Bin(result_buff,sizeof(result_buff),result_str,TRIMBIN_TRIM_NO, BOAT_FALSE);
    BoatDisplayTestResult( 0 != memcmp(result_buff,"Hello world",strlen("Hello world")), "[ReadStore_19]:readListByIndex-memcmp" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_10_EthFunTest_19(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_EthereumPreCondition_0(wallet_num), "[Case_10_EthFunTest_19]:PreCondition_0" );
    //Call the contract
    BoatDisplayTestResult( BOAT_SUCCESS == Case_10_Call_ReadStore_19(g_case_10_ethereum_wallet_ptr), "[Case_10_EthFunTest_19]:ReadStore_19" );  
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

