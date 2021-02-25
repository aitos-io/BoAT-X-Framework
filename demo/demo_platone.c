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

#if PROTOCOL_USE_PLATONE == 1

#include "my_contract.cpp.abi.h"

#define BOAT_DEMO_CREATE_WALLET_NOT_STORE           1
#define BOAT_DEMO_CREATE_WALLET_AND_STORE           0
#define BOAT_DEMO_LOAD_WALLET_FROM_FILE             0

BoatPlatoneWallet *g_boat_platone_wallet_ptr;

/*!*****************************************************************************
@brief boat call my contract function.

Function: BoatCallMyContract()

    This function is boat call my contract function.
    Init transaction, if success, then send the data to blockchain.
    if send data to blockchain success, then get the data from blockchain.

@return
    This function returns the result of call my contract, if call successful return BOAT_SUCCESS.\n
    Otherwise it returns BOAT_ERROR.

@param[in] wallet_ptr
    Pointer of the boat platone wallet.

*******************************************************************************/
BOAT_RESULT BoatCallMyContract(BoatPlatoneWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    BOAT_RESULT result;
    
    // Set Contract Address
    result = BoatPlatoneTxInit(wallet_ptr,
                          &tx_ctx,
                          BOAT_TRUE,
                          NULL,
                          "0x333333",
                          "0xde7f3ae22cf97939a2e4cd68def3bb29268a1eck",
                          BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if( BOAT_SUCCESS != result )
    {
        BoatLog(BOAT_LOG_NORMAL, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");

    if( NULL != result_str )
    {
        BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);
    }

    result_str = my_contract_cpp_abi_getName(&tx_ctx);

    if( NULL != result_str )
    {
        BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);
    }

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief boat Platone PreCondition function.

Function: BoatPlatonePreCondition()

    This function is boat Ethereum PreCondition function.
    create wallet or load wallet.

@return
    This function returns the result of wallet init, if successful return BOAT_SUCCESS.\n
    Otherwise it returns BOAT_ERROR.

@param This function doesn't take any argument.

*******************************************************************************/
BOAT_RESULT BoatPlatonePreCondition(void)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;

    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    "0xe55464c12b9e034ab00f7dddeb01874edcf514b3cd77a9ad0ad8796b4d3b1fdb",
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;

    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, "http://123.123.123.123:8080", BOAT_NODE_URL_MAX_LEN-1);

#if BOAT_DEMO_CREATE_WALLET_NOT_STORE == 1

    // create one-time wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));

#elif BOAT_DEMO_CREATE_WALLET_AND_STORE == 1

    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, "boatplatone.keystore", &wallet_config, sizeof(BoatPlatoneWalletConfig));

#elif BOAT_DEMO_LOAD_WALLET_FROM_FILE == 1

    // Load an existed wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, "boatplatone.keystore", NULL, sizeof(BoatPlatoneWalletConfig));

#else

    BoatLog(BOAT_LOG_NORMAL, "error, must define one wallet switch");
    return BOAT_ERROR;

#endif

    if( -1 == index )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
        return BOAT_ERROR;
    }
    
    wallet_ptr = BoatGetWalletByIndex(index);

    g_boat_platone_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief boat Platone PreCondition load wallet function.

Function: BoatPlatonePreConLoadWallet()

    This function is boat Platone PreCondition load wallet function.
    Load an existed wallet, 
    if fail, then Create a new wallet or overwrite an existed wallet,
    and store the wallet.

@return
    This function returns the result of wallet init, if successful return BOAT_SUCCESS.\n
    Otherwise it returns BOAT_ERROR.

@param This function doesn't take any argument.

*******************************************************************************/
BOAT_RESULT BoatPlatonePreConLoadWallet(void)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BoatPlatoneWallet *wallet_ptr;

    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    "0xe55464c12b9e034ab00f7dddeb01874edcf514b3cd77a9ad0ad8796b4d3b1fdb",
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;

    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, "http://123.123.123.123:8080", BOAT_NODE_URL_MAX_LEN-1);

    // Load an existed wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, "boatplatone.keystore", NULL, sizeof(BoatPlatoneWalletConfig));

    if( -1 == index )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to load wallet, will create and store wallet...");

        // Create a new wallet or overwrite an existed wallet
        index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, "boatplatone.keystore", &wallet_config, sizeof(BoatPlatoneWalletConfig));

        if( -1 == index )
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet.");
            return BOAT_ERROR;
        }
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Load wallet success.");
    }
    
    wallet_ptr = BoatGetWalletByIndex(index);

    g_boat_platone_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief boat PlatOne entry main function.

Function: BoatPlatoneMainEntry()

    This function is boat PlatOne entry main function.
    get wallet, if success, then call my contract.

@return
    This function returns the result of call my contract, if successful return BOAT_SUCCESS.\n
    Otherwise it returns BOAT_ERROR.

@param This function doesn't take any argument.

*******************************************************************************/
BOAT_RESULT BoatPlatoneMainEntry(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;

#if 0
    result = BoatPlatonePreCondition();
#else
    result = BoatPlatonePreConLoadWallet();
#endif

    if( BOAT_SUCCESS == result )
    {    
        result = BoatCallMyContract(g_boat_platone_wallet_ptr);
    }

    if( BOAT_SUCCESS != result )
    {
        BoatLog(BOAT_LOG_NORMAL, "Block to Platone Chain Failed: %d.", result);
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Block to Platone Chain  Passed.");
    }

    return result;
}

#endif /* end of PROTOCOL_USE_PLATONE */

