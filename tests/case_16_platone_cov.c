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
#include "boatplatone.h"

#include "my_contract.cpp.abi.h"
#include "testcommon.h"


#define TEST_PONE_PRIVATE_KEY_COV_0 "0xe55464c12b9e034ab00f7dddeb01874edcf514b3cd77a9ad0ad8796b4d3b1fdb"

#define TEST_PONE_NODE_URL_COV_0    "http://116.236.47.90:7545"

#define TEST_PONE_WALLET_NAME_COV_0 "./tests/platone.key"


BoatPlatoneWallet *g_case_16_platone_wallet_ptr;

BoatPlatoneWallet *wallet_ptr;

BOAT_RESULT Case_15_PlatonePreCondition_A(void)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    TEST_PONE_PRIVATE_KEY_COV_0,
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, TEST_PONE_NODE_URL_COV_0, BOAT_NODE_URL_MAX_LEN-1);
    
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, TEST_PONE_WALLET_NAME_COV_0, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if( index == -1 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
        
    wallet_ptr = BoatGetWalletByIndex(index);

    g_case_16_platone_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
    
    return BOAT_SUCCESS;    
}

BOAT_RESULT Case_boatplatoneMain(BoatPlatoneWallet *wallet_ptr)
{
    BSINT32 result=BOAT_SUCCESS;
    BoatDisplayCellTestResult(result, -108 == PlatoneSendRawtx( NULL ), "PlatoneSendRawtx");

    BoatPlatoneTx tx_toplatone;
    BoatDisplayCellTestResult(result, BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr, &tx_toplatone, BOAT_TRUE,NULL,"0x333333","0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45",BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit");


    BoatDisplayCellTestResult(result, BOAT_SUCCESS ==  BoatPlatoneTxSetNonce(&tx_toplatone, 0xff), "BoatPlatoneTxSetNonce");    

    BoatFieldMax32B value;
    BoatDisplayCellTestResult(result, BOAT_SUCCESS <= (value.field_len = UtilityHex2Bin(value.field,32, "0x00",TRIMBIN_LEFTTRIM, BOAT_TRUE )), "UtilityHex2Bin");    
    
    BoatDisplayCellTestResult(result, BOAT_SUCCESS ==  BoatPlatoneTxSetValue(&tx_toplatone, &value), "BoatPlatoneTxSetValue");  

    BoatFieldVariable data;
    BUINT64 tx_type_big;
    // Set data (contains txtype only)
    UtilityUint64ToBigend(
                            (BUINT8*)&tx_type_big,
                            0,
                            TRIMBIN_TRIM_NO
                            );

    data.field_ptr = (BUINT8*)&tx_type_big;
    data.field_len = sizeof(BUINT64);

    BoatDisplayCellTestResult(result, BOAT_SUCCESS ==  BoatPlatoneTxSetData(&tx_toplatone, &data), "BoatPlatoneTxSetData"); 
    BoatDisplayCellTestResult(result, BOAT_SUCCESS ==  BoatPlatoneTxSend(&tx_toplatone), "BoatPlatoneTxSend"); 


    return BOAT_SUCCESS;

}

//api_ethereum.c
BOAT_RESULT Case_apiPlatoneMain(BoatPlatoneWallet *wallet_ptr)
{
    BSINT32 result=BOAT_SUCCESS;

    BoatDisplayCellTestResult(result, NULL == BoatPlatoneWalletInit(NULL, 0), "BoatPlatoneWalletInit");

    BoatDisplayCellTestResult(result, -108 == BoatPlatoneWalletSetNodeUrl(NULL, "HTTP://127.0.0.1:7545"), "BoatPlatoneWalletSetNodeUrl 0");

    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneWalletSetNodeUrl(wallet_ptr, NULL), "BoatPlatoneWalletSetNodeUrl 1");

    BoatDisplayCellTestResult(result, -108 == BoatPlatoneWalletSetEIP155Comp(NULL, 1), "BoatPlatoneWalletSetEIP155Comp");
    BoatDisplayCellTestResult(result, -108 == BoatPlatoneWalletSetChainId(NULL, 1), "BoatPlatoneWalletSetChainId");

    BoatDisplayCellTestResult(result, NULL == BoatPlatoneWalletGetBalance(NULL, NULL), "BoatPlatoneWalletGetBalance 0");
    BoatDisplayCellTestResult(result, NULL != BoatPlatoneWalletGetBalance(wallet_ptr, "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45"), "BoatPlatoneWalletGetBalance 1");

    BoatPlatoneTx tx_ptr;
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxInit(NULL,&tx_ptr,true,"0x1234","0x333333","0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45",BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit 0");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS == BoatPlatoneTxInit(wallet_ptr,&tx_ptr,true,"0x1234","0x333333","0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45",BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR), "BoatPlatoneTxInit 1");

    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxSetNonce(NULL, 0x23), "BoatPlatoneTxSetNonce");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxSetGasPrice(NULL, NULL), "BoatPlatoneTxSetGasPrice 0");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS == BoatPlatoneTxSetGasPrice(&tx_ptr, NULL), "BoatPlatoneTxSetGasPrice 1");

    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxSetGasLimit(NULL, NULL), "BoatPlatoneTxSetGasLimit 0");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxSetGasLimit(&tx_ptr, NULL), "BoatPlatoneTxSetGasLimit 1");

    BUINT8 setrecipient[20]={0x12,0x13};
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxSetRecipient(NULL, setrecipient), "BoatPlatoneTxSetRecipient");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxSetValue(NULL, NULL), "BoatPlatoneTxSetValue");    

    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxSetData(NULL, NULL), "BoatPlatoneTxSetData");
    BoatDisplayCellTestResult(result, BOAT_SUCCESS != BoatPlatoneTxSend(NULL), "BoatPlatoneTxSend");    


    BoatDisplayCellTestResult(result, NULL == BoatPlatoneCallContractFunc( NULL,NULL, 16), "BoatPlatoneCallContractFunc 0");

    tx_ptr.wallet_ptr = wallet_ptr;   
    BoatDisplayCellTestResult(result, NULL == BoatPlatoneCallContractFunc( &tx_ptr,NULL, 16), "BoatPlatoneCallContractFunc 1"); 
                 
    BoatDisplayCellTestResult(result, BOAT_ERROR == BoatPlatoneGetTransactionReceipt(NULL), "BoatPlatoneGetTransactionReceipt");   

    return result;

}




BOAT_RESULT Case_16_PlatONECovMain(void)
{
    BOAT_RESULT case_result = BOAT_SUCCESS;

    case_result += Case_15_PlatonePreCondition_A();

    case_result += Case_boatplatoneMain(g_case_16_platone_wallet_ptr);

    case_result += Case_apiPlatoneMain(g_case_16_platone_wallet_ptr);

    BoatLog(BOAT_LOG_NORMAL, "[case_result]: %d.", case_result);
    BoatWalletDelete(TEST_PONE_WALLET_NAME_COV_0);
    return case_result;
}





