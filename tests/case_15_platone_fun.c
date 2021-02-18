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



#define TEST_PONE_GASPRICE_0	"0x111111"
#define TEST_PONE_GASPRICE_1 	"0x222222"

#define TEST_PONE_GASLIMIT_0	"0x333333"
#define TEST_PONE_GASLIMIT_1	"0x222222"
#define TEST_PONE_GASLIMIT_2	"0x111111"

//Contract Address
#define TEST_PONE_CONTRACT_ADDR_0 "0x1234567812345678123456781234567812345678"
#define TEST_PONE_CONTRACT_ADDR_1 "0x1234567812345678123456781234567812345688"

#define TEST_PONE_PRIVATE_KEY_0 "0x1234567812345678123456781234567812345678123456781234567812345678"
#define TEST_PONE_PRIVATE_KEY_1 "0x1234567812345678123456781234567812345678123456781234567812345688"

#define TEST_PONE_NODE_URL_0 "HTTP://127.0.0.1:7545"
#define TEST_PONE_NODE_URL_1 "HTTP://127.1.1.1:7545"

#define TEST_PONE_WALLET_NAME_0 "./tests/platone.key"
#define TEST_PONE_WALLET_NAME_1 "./tests/platone/platone.key"

#define TEST_PONE_PROTOCOL_0 BOAT_PROTOCOL_PLATONE
#define TEST_PONE_PROTOCOL_1 BOAT_PROTOCOL_ETHEREUM


BoatPlatoneWallet *g_case_15_platone_wallet_ptr;


//case 0:
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
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    BOAT_RESULT result;
    
    // Set Contract Address
    result = BoatPlatoneTxInit(wallet_ptr,
                          &tx_ctx,
                          BOAT_TRUE,
                          NULL,
                          TEST_PONE_GASLIMIT_0,
                          TEST_PONE_CONTRACT_ADDR_0,
                          BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR;
    }
    

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
    if( result_str != NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);
    }
    else
    {
        return BOAT_ERROR;
    }
    
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if( result_str != NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);
        BoatDisplayTestResult( 0 == memcmp(result_str,"0x48656c6c6f576f726c64",strlen("0x48656c6c6f576f726c64")), "my_contract_cpp_abi_getName" );
    }
    else
    {
        return BOAT_ERROR;
    }
    
    
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_0(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_0(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

//case 1:load wallet
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

//case 3:other private_key
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

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

//case 6:url
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

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

//case 7:wallet_type
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
BOAT_RESULT Case_15_Call_my_contract_9(BoatPlatoneWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    
    // Set Contract Address
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ctx,BOAT_TRUE,NULL,TEST_PONE_GASLIMIT_0,TEST_PONE_CONTRACT_ADDR_0,BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit" );

    BoatDisplayTestResult( BOAT_SUCCESS == ( result_str = my_contract_cpp_abi_setName(NULL, "HelloWorld")), "my_contract_cpp_abi_setName" );   
    BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);


    BoatDisplayTestResult( BOAT_SUCCESS == ( result_str = my_contract_cpp_abi_getName(&tx_ctx)), "my_contract_cpp_abi_getName" );   
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);

    BoatDisplayTestResult( 0 == memcmp(result_str,"0x48656c6c6f576f726c64",strlen("0x48656c6c6f576f726c64")), "my_contract_cpp_abi_getName" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_9(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_0(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_9(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_9" );
    return BOAT_SUCCESS;
}

//case 10:TEST_PONE_GASPRICE_1
BOAT_RESULT Case_15_Call_my_contract_10(BoatPlatoneWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    
    // Set Contract Address
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_PONE_GASPRICE_1,TEST_PONE_GASLIMIT_0,TEST_PONE_CONTRACT_ADDR_0,BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit" );

    BoatDisplayTestResult( BOAT_SUCCESS == ( result_str = my_contract_cpp_abi_setName(NULL, "HelloWorld")), "my_contract_cpp_abi_setName" );   
    BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);


    BoatDisplayTestResult( BOAT_SUCCESS == ( result_str = my_contract_cpp_abi_getName(&tx_ctx)), "my_contract_cpp_abi_getName" );   
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);

    BoatDisplayTestResult( 0 == memcmp(result_str,"0x48656c6c6f576f726c64",strlen("0x48656c6c6f576f726c64")), "my_contract_cpp_abi_getName" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_10(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_0(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_10(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_10" );
    return BOAT_SUCCESS;
}

//case 11:TEST_PONE_GASLIMIT_1
BOAT_RESULT Case_15_Call_my_contract_11(BoatPlatoneWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    
    // Set Contract Address
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_PONE_GASPRICE_0,TEST_PONE_GASLIMIT_1,TEST_PONE_CONTRACT_ADDR_0,BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit" );

    BoatDisplayTestResult( BOAT_SUCCESS == ( result_str = my_contract_cpp_abi_setName(NULL, "HelloWorld")), "my_contract_cpp_abi_setName" );   
    BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);


    BoatDisplayTestResult( BOAT_SUCCESS == ( result_str = my_contract_cpp_abi_getName(&tx_ctx)), "my_contract_cpp_abi_getName" );   
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);

    BoatDisplayTestResult( 0 == memcmp(result_str,"0x48656c6c6f576f726c64",strlen("0x48656c6c6f576f726c64")), "my_contract_cpp_abi_getName" );

    return BOAT_SUCCESS;
}


BOAT_RESULT Case_15_PlatoneFunTest_11(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_1(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
    return BOAT_SUCCESS;
}

//case 11:TEST_PONE_CONTRACT_ADDR_1
BOAT_RESULT Case_15_Call_my_contract_12(BoatPlatoneWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    
    // Set Contract Address
    BoatDisplayTestResult( BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ctx,BOAT_FALSE,TEST_PONE_GASPRICE_0,TEST_PONE_GASLIMIT_0,TEST_PONE_CONTRACT_ADDR_1,BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit" );

    BoatDisplayTestResult( BOAT_SUCCESS == ( result_str = my_contract_cpp_abi_setName(NULL, "HelloWorld")), "my_contract_cpp_abi_setName" );   
    BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);


    BoatDisplayTestResult( BOAT_SUCCESS == ( result_str = my_contract_cpp_abi_getName(&tx_ctx)), "my_contract_cpp_abi_getName" );   
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);

    BoatDisplayTestResult( 0 == memcmp(result_str,"0x48656c6c6f576f726c64",strlen("0x48656c6c6f576f726c64")), "my_contract_cpp_abi_getName" );

    return BOAT_SUCCESS;
}

BOAT_RESULT Case_15_PlatoneFunTest_12(BSINT8 *wallet_num)
{
    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_PlatonePreCondition_1(wallet_num), "Case_15_PlatonePreCondition_0" );

    BoatDisplayTestResult( BOAT_SUCCESS == Case_15_Call_my_contract_0(g_case_15_platone_wallet_ptr), "Case_15_Call_my_contract_0" );
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

