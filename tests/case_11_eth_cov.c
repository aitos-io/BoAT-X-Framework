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
#include "boatethereum.h"
#include "web3intf.h"

#include "testcommon.h"

#define TEST_ETH_PRIVATE_KEYCOV_0 "0x1234567812345678123456781234567812345678123456781234567812345678"

#define TEST_ETH_NODE_URLCOV_0 "HTTP://127.0.0.1:7545"

#define TEST_ETH_WALLET_NAMECOV_0 "./tests/eth.key"



BoatEthWallet *g_case_30_ethereum_wallet_ptr;


BoatEthWallet *wallet_ptr;


BOAT_RESULT Case_EthereumPreCondition(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEYCOV_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URLCOV_0, BOAT_NODE_URL_MAX_LEN-1);

    // Test one-time wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    
    wallet_ptr = BoatGetWalletByIndex(index);

    g_case_30_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;        
    return BOAT_SUCCESS;

}

BOAT_RESULT Case_EthereumPreCondition_A(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;


    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEYCOV_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URLCOV_0, BOAT_NODE_URL_MAX_LEN-1);

    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, TEST_ETH_WALLET_NAMECOV_0, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_30_ethereum_wallet_ptr = wallet_ptr;
    *wallet_num = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT Case_EthereumPreCondition_B(BSINT8 *wallet_num)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_ETH_PRIVATE_KEYCOV_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_ETH_NODE_URLCOV_0, BOAT_NODE_URL_MAX_LEN-1);

    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, TEST_ETH_WALLET_NAMECOV_0, NULL, sizeof(BoatEthWalletConfig));    
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    
    wallet_ptr = BoatGetWalletByIndex(index);
    g_case_30_ethereum_wallet_ptr = wallet_ptr;    
    *wallet_num = index;
    return BOAT_SUCCESS;
}



BOAT_RESULT Case_utilitiesMain()
{
    BOAT_RESULT result=BOAT_SUCCESS;
    BUINT8 from_ptr_A1[10]={0,1,2,3,4,5,6,7,8,9};

    BoatDisplayCellTestResult(result, 0 == UtilityTrimBin(NULL,from_ptr_A1,10,TRIMBIN_TRIM_NO,BOAT_FALSE), "UtilityTrimBin");

    BUINT8 from_ptr_A2[10]={0};
    BUINT8 to_ptr[16];    
    BoatDisplayCellTestResult(result, 10 == UtilityTrimBin(to_ptr,from_ptr_A2,10,TRIMBIN_TRIM_NO,BOAT_FALSE), "UtilityTrimBin");


    BoatDisplayCellTestResult(result, 1 == UtilityTrimBin(to_ptr,from_ptr_A2,10,TRIMBIN_LEFTTRIM,BOAT_FALSE), "UtilityTrimBin");
    BCHAR to_ptr1[16];
    BoatDisplayCellTestResult(result, 3 == UtilityBin2Hex(to_ptr1,from_ptr_A2,10,BIN2HEX_LEFTTRIM_QUANTITY,BIN2HEX_PREFIX_0x_YES,BOAT_FALSE), "UtilityBin2Hex");

    BUINT8 from_ptr_A3[10]={0,1,2,3};
    BoatDisplayCellTestResult(result, 19 == UtilityBin2Hex(to_ptr1,from_ptr_A3,10,BIN2HEX_LEFTTRIM_QUANTITY,BIN2HEX_PREFIX_0x_YES,BOAT_FALSE), "UtilityBin2Hex");
    
    BUINT8 from_ptr_A4[10]={0x00,0x01,2,3};
    BoatDisplayCellTestResult(result, 19 == UtilityBin2Hex(to_ptr1,from_ptr_A4,10,BIN2HEX_LEFTTRIM_QUANTITY,BIN2HEX_PREFIX_0x_YES,BOAT_FALSE), "UtilityBin2Hex");
    
    BCHAR from_ptr_A5[10]="0x123456";
    BoatDisplayCellTestResult(result, 3 == UtilityHex2Bin(to_ptr,strlen(from_ptr_A5),from_ptr_A5,TRIMBIN_LEFTTRIM,BOAT_FALSE), "UtilityHex2Bin");

    BCHAR from_ptr_A6[10]="0x000000";
    BoatDisplayCellTestResult(result, 1 == UtilityHex2Bin(to_ptr,strlen(from_ptr_A6),from_ptr_A6,TRIMBIN_LEFTTRIM,BOAT_FALSE), "UtilityHex2Bin");


    BUINT8 to_big_ptr_A1[16]={0};
    BUINT32 from_host_integer_A1=0;
    BoatDisplayCellTestResult(result, 1 == UtilityUint32ToBigend(to_big_ptr_A1,from_host_integer_A1,TRIMBIN_LEFTTRIM), "UtilityUint32ToBigend");

    BUINT64 big_num_A1=0x1234;
    BoatDisplayCellTestResult(result, 2 == UtilityUint64ToBigend(to_big_ptr_A1,big_num_A1,TRIMBIN_LEFTTRIM), "UtilityUint64ToBigend");
    
    BUINT64 big_num_A2=0;
    BoatDisplayCellTestResult(result, 8 == UtilityUint64ToBigend(to_big_ptr_A1,big_num_A2,TRIMBIN_TRIM_NO), "UtilityUint64ToBigend");
    BoatDisplayCellTestResult(result, 1 == UtilityUint64ToBigend(to_big_ptr_A1,big_num_A2,TRIMBIN_LEFTTRIM), "UtilityUint64ToBigend");

    BUINT32 integer_A1=0x1234;
    BoatDisplayCellTestResult(result, 0x34120000 == Utilityhtonl(integer_A1), "Utilityhtonl");
    BoatDisplayCellTestResult(result, 0x34120000 == Utilityntohl(integer_A1), "Utilityntohl");

    BoatDisplayCellTestResult(result, 1.0 == UtilityWeiStrToEthDouble("0xDE0B6B3A7640000"), "UtilityWeiStrToEthDouble");

    return result;
}



//api_ethereum.c
BOAT_RESULT Case_apiethereumMain(BoatEthWallet *wallet_ptr)
{
    BOAT_RESULT result=BOAT_SUCCESS;

    BoatDisplayCellTestResult(result, NULL == BoatEthWalletInit(NULL, 0), "BoatEthWalletInit");

    BoatDisplayCellTestResult(result, -108 == BoatEthWalletSetNodeUrl(NULL, TEST_ETH_NODE_URLCOV_0), "BoatEthWalletSetNodeUrl");

    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthWalletSetNodeUrl(wallet_ptr, NULL), "BoatEthWalletSetNodeUrl");

    BoatDisplayCellTestResult(result, -108 == BoatEthWalletSetEIP155Comp(NULL, 1), "BoatEthWalletSetNodeUrl");
    BoatDisplayCellTestResult(result, -108 == BoatEthWalletSetChainId(NULL, 1), "BoatEthWalletSetChainId");

    BoatDisplayCellTestResult(result, NULL == BoatEthWalletGetBalance(NULL, NULL), "BoatEthWalletGetBalance");
    BoatDisplayCellTestResult(result, NULL != BoatEthWalletGetBalance(wallet_ptr, "0x39631C55F60C708406Aa7b98AcDBfdDe2cC80157"), "BoatEthWalletGetBalance");

    BoatEthTx tx_ptr;
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxInit(NULL,&tx_ptr,true,"0x1234","0x333333","0x39631C55F60C708406Aa7b98AcDBfdDe2cC80157"), "BoatEthTxInit");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS == BoatEthTxInit(wallet_ptr,&tx_ptr,true,"0x1234","0x333333","0x39631C55F60C708406Aa7b98AcDBfdDe2cC80157"), "BoatEthTxInit");

    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxSetNonce(NULL, 0x23), "BoatEthTxSetNonce");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxSetGasPrice(NULL, NULL), "BoatEthTxSetGasPrice");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS == BoatEthTxSetGasPrice(&tx_ptr, NULL), "BoatEthTxSetGasPrice");

    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxSetGasLimit(NULL, NULL), "BoatEthTxSetGasLimit");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxSetGasLimit(&tx_ptr, NULL), "BoatEthTxSetGasLimit");

    BUINT8 setrecipient[20]={0x12,0x13};
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxSetRecipient(NULL, setrecipient), "BoatEthTxSetRecipient");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxSetValue(NULL, NULL), "BoatEthTxSetValue");    

    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxSetData(NULL, NULL), "BoatEthTxSetData");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatEthTxSend(NULL), "BoatEthTxSend");    


    BoatDisplayCellTestResult(result, NULL == BoatEthCallContractFunc( NULL,NULL, NULL, 16), "BoatEthCallContractFunc");

    tx_ptr.wallet_ptr = wallet_ptr;   
    BoatDisplayCellTestResult(result, NULL == BoatEthCallContractFunc( &tx_ptr,NULL, NULL, 16), "BoatEthCallContractFunc"); 
                 
    BoatDisplayCellTestResult(result, BOAT_ERROR == BoatEthGetTransactionReceipt(NULL), "BoatEthGetTransactionReceipt");   

    return result;

}


//boatethereum.c
BOAT_RESULT Case_boatethereumMain(BoatEthWallet *wallet_ptr)
{
    BUINT32 result=BOAT_SUCCESS;
    BoatDisplayCellTestResult(result, -108 == EthSendRawtx( NULL ), "EthSendRawtx");

    BoatEthTx tx_toeth;
    BoatDisplayCellTestResult(result, BOAT_SUCCESS == BoatEthTxInit(wallet_ptr, &tx_toeth, BOAT_TRUE,NULL,"0x333333","0xEF5D6d9D037Faa6493637E26A2B5d604f7722779"), "BoatEthTxInit");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS ==  BoatEthTxSetNonce(&tx_toeth, BOAT_ETH_NONCE_AUTO), "BoatEthTxSetNonce");    

    BoatFieldMax32B value;
    BoatDisplayCellTestResult(result, BOAT_SUCCESS <= (value.field_len = UtilityHex2Bin(value.field,32, "0x00",TRIMBIN_LEFTTRIM, BOAT_TRUE )), "UtilityHex2Bin");    
    BoatDisplayCellTestResult(result, BOAT_SUCCESS ==  BoatEthTxSetValue(&tx_toeth, &value), "BoatEthTxSetValue");  
    BoatDisplayCellTestResult(result, BOAT_SUCCESS ==  BoatEthTxSetData(&tx_toeth, NULL), "BoatEthTxSetData"); 
    BoatDisplayCellTestResult(result, BOAT_SUCCESS ==  BoatEthTxSend(&tx_toeth), "BoatEthTxSend"); 

    return result;

}





BOAT_RESULT Case_web3Main(void)
{
    BOAT_RESULT result=BOAT_SUCCESS;
    Web3IntfContext *web3_init_A1;                            
    web3_init_A1 = web3_init();    
    BoatDisplayCellTestResult(result, NULL != web3_init(), "web3_init");
    web3_deinit(NULL);


    BoatDisplayCellTestResult(result, NULL == web3_eth_getTransactionCount(NULL,NULL,NULL), "web3_eth_getTransactionCount");

    BoatDisplayCellTestResult(result, NULL == web3_eth_getTransactionCount(web3_init_A1,NULL,NULL), "web3_eth_getTransactionCount");

    BoatDisplayCellTestResult(result, NULL == web3_eth_gasPrice(NULL,NULL), "web3_eth_gasPrice");

    BoatDisplayCellTestResult(result, NULL == web3_eth_getBalance(NULL,NULL,NULL), "web3_eth_getBalance");

    BoatDisplayCellTestResult(result, NULL == web3_eth_sendRawTransaction(NULL,NULL,NULL), "web3_eth_sendRawTransaction");  

    BoatDisplayCellTestResult(result, NULL == web3_eth_sendRawTransaction(web3_init_A1,NULL,NULL), "web3_eth_sendRawTransaction");                                 

    BoatDisplayCellTestResult(result, NULL == web3_eth_getStorageAt(NULL,NULL,NULL), "web3_eth_getStorageAt"); 

    BoatDisplayCellTestResult(result, NULL == web3_eth_getStorageAt(web3_init_A1,NULL,NULL), "web3_eth_getStorageAt"); 

    Param_eth_getStorageAt param_ptr_A1;    
    param_ptr_A1.address_str="0x123456789";
    param_ptr_A1.block_num_str="10";
    param_ptr_A1.position_str="11";                            

    BoatDisplayCellTestResult(result, NULL == web3_eth_getStorageAt(web3_init_A1,TEST_ETH_NODE_URLCOV_0,&param_ptr_A1), "web3_eth_getStorageAt"); 

    BoatDisplayCellTestResult(result, NULL == web3_eth_getTransactionReceiptStatus(NULL,NULL,NULL), "web3_eth_getTransactionReceiptStatus");                                 

    BoatDisplayCellTestResult(result, NULL == web3_eth_getTransactionReceiptStatus(web3_init_A1,NULL,NULL), "web3_eth_getTransactionReceiptStatus");  

    BoatDisplayCellTestResult(result, NULL == web3_eth_call(NULL,NULL,NULL), "web3_eth_call"); 
    BoatDisplayCellTestResult(result, NULL == web3_eth_call(web3_init_A1,NULL,NULL), "web3_eth_call");                      
                     
    web3_deinit(web3_init_A1);

    return BOAT_SUCCESS;
}


BOAT_RESULT Case_11_EthCovMain(void)
{
    BOAT_RESULT case_result = BOAT_SUCCESS;
    BSINT8 wallet_num=0;
    case_result += Case_utilitiesMain();
    case_result += Case_web3Main();
    case_result += Case_EthereumPreCondition(&wallet_num);
    case_result += Case_apiethereumMain(g_case_30_ethereum_wallet_ptr);
    BoatWalletUnload(wallet_num);

    g_case_30_ethereum_wallet_ptr = NULL;
    case_result += Case_EthereumPreCondition_A(&wallet_num);
    case_result += Case_boatethereumMain(g_case_30_ethereum_wallet_ptr);   
    BoatWalletUnload(wallet_num);   
    case_result += Case_EthereumPreCondition_B(&wallet_num);
    BoatWalletUnload(wallet_num);

    BoatLog(BOAT_LOG_NORMAL, "Case_11_EthCovMain case_result: %d.", case_result);

    return case_result;
}




