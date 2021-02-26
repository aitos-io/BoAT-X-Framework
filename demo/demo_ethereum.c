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

#define BOAT_DEMO_CREATE_WALLET_NOT_STORE           1
#define BOAT_DEMO_CREATE_WALLET_AND_STORE           0
#define BOAT_DEMO_LOAD_WALLET_FROM_FILE             0

BoatEthWallet *g_boat_ethereum_wallet_ptr;

/*!*****************************************************************************
@brief boat call read store contract function.

Function: BoatCallReadStore()

    This function is boat call read store contract function.
    Init transaction, if success, then send the data to blockchain.
    if send data to blockchain success, then read the data from blockchain.

@return
    This function returns the result of call contract, if successful return BOAT_SUCCESS.\n
    Otherwise it returns BOAT_ERROR.

@param[in] wallet_ptr
    Pointer of the ethereum platone wallet.

*******************************************************************************/
BOAT_RESULT BoatCallReadStore(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT32 index;
    BoatEthTx tx_ctx;
    BOAT_RESULT result;
	BoatFieldVariable prase_result = {NULL, 0};
    
    // Set Contract Address
    result = BoatEthTxInit(wallet_ptr,
                          &tx_ctx,
                          BOAT_TRUE,
                          NULL,
                          "0x333333",
                          "0xE6337c2E09bF9D0BAd1fkD85D2222C5924C84867");

    if( BOAT_SUCCESS != result )
    {
        BoatLog(BOAT_LOG_NORMAL, "BoatEthTxInit fails.");
        return BOAT_ERROR;
    }

    result_str = StoreRead_saveList(&tx_ctx, (BUINT8*)"HelloWorld");
    BoatLog(BOAT_LOG_NORMAL, "saveList returns: %s", result_str);

    if( NULL != result_str )
    {
        result_str = StoreRead_readListLength(&tx_ctx);
        result = BoatEthPraseRpcResponseResult( result_str, "", &prase_result);
        if( result == BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", prase_result.field_ptr);
            
            UtilityHex2Bin(
                            (BUINT8*)index_u256_big,
                            sizeof(index_u256_big),
                            (BCHAR*)prase_result.field_ptr,
                            TRIMBIN_TRIM_NO,
                            BOAT_FALSE
                          );

            UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));

            list_len = index_u256_big[0];

            for( index = 0; index < list_len; index++ )
            {
                result_str = StoreRead_readListByIndex(&tx_ctx, index);
                result = BoatEthPraseRpcResponseResult( result_str, "", &prase_result);
				if( result == BOAT_SUCCESS )
                {
                    BoatLog( BOAT_LOG_NORMAL, "readListByIndex returns: %s", prase_result.field_ptr );
                }
                else
                {
                    return BOAT_ERROR;
                }
            }
        }
    }

    //free prase_result
    BoatFree(prase_result.field_ptr);
    prase_result.field_ptr = 0;

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief boat Ethereum PreCondition function.

Function: BoatEthereumPreCondition(void)

    This function is boat Ethereum PreCondition function.
    create wallet or load wallet.

@return
    This function returns the result of wallet init, if successful return BOAT_SUCCESS.\n
    Otherwise it returns BOAT_ERROR.

@param This function doesn't take any argument.

*******************************************************************************/
BOAT_RESULT BoatEthereumPreCondition(void)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;

    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    "904610d1013c8d11cb7fccdf4d3ca7d7d51e3c3661712099dd4d203d70cfba29", 
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;

    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, "http://123.123.123.123:8080", BOAT_NODE_URL_MAX_LEN-1);

#if BOAT_DEMO_CREATE_WALLET_NOT_STORE == 1

    // create one-time wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig));

#elif BOAT_DEMO_CREATE_WALLET_AND_STORE == 1

    // Create a new wallet or overwrite an existed wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "boateth.keystore", &wallet_config, sizeof(BoatEthWalletConfig));

#elif BOAT_DEMO_LOAD_WALLET_FROM_FILE == 1

    // Load an existed wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "boateth.keystore", NULL, sizeof(BoatEthWalletConfig));

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

    g_boat_ethereum_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);
	
    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief boat Ethereum PreCondition load wallet function.

Function: BoatEthereumPreConLoadWallet()

    This function is boat Ethereum PreCondition load wallet function.
    Load an existed wallet, 
    if fail, then Create a new wallet or overwrite an existed wallet,
    and store the wallet.

@return
    This function returns the result of wallet init, if successful return BOAT_SUCCESS.\n
    Otherwise it returns BOAT_ERROR.

@param This function doesn't take any argument.

*******************************************************************************/
BOAT_RESULT BoatEthereumPreConLoadWallet(void)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;
    BoatEthWallet *wallet_ptr;

    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    "0xfceb45ddae69a0e787b886c58d00e724f34985e3b362d5efe68f46e3e4b6b337", 
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;

    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, "http://123.123.123.123:8080", BOAT_NODE_URL_MAX_LEN-1); 

    // Load an existed wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "boateth.keystore", NULL, sizeof(BoatEthWalletConfig));

    if( -1 == index )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to load wallet, will create and store wallet...");

        // Create a new wallet or overwrite an existed wallet
        index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "boateth.keystore", &wallet_config, sizeof(BoatEthWalletConfig));

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

    g_boat_ethereum_wallet_ptr = wallet_ptr;

    // Destroy private key in local variable
    memset(wallet_config.priv_key_array, 0x00, 32);

    return BOAT_SUCCESS;
}


/*!*****************************************************************************
@brief boat Ethereum entry main function.

Function: BoatEthereumMainEntry(void)

    This function is boat Ethereum entry main function.
    get wallet, if success, and the wallet balance is enough ETH, then call contract.

@return
    This function returns the result of call contract, if successful return BOAT_SUCCESS.\n
    Otherwise it returns BOAT_ERROR.

@param This function doesn't take any argument.

*******************************************************************************/
BOAT_RESULT BoatEthereumMainEntry(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BCHAR * cur_balance_wei = NULL;
    BoatFieldVariable prase_result = {NULL, 0};
	double cur_balance_eth = 0;

#if 1
    result = BoatEthereumPreCondition();
#else
    result = BoatEthereumPreConLoadWallet();
#endif

    if ( BOAT_SUCCESS == result )
    {
        cur_balance_wei = BoatEthWalletGetBalance(g_boat_ethereum_wallet_ptr, NULL);
        result = BoatEthPraseRpcResponseResult( cur_balance_wei, "", &prase_result);
        if( result == BOAT_SUCCESS )
        {
            BoatLog( BOAT_LOG_NORMAL, "BoatEthWalletGetBalance returns: %s", prase_result.field_ptr );
        }
        else
        {
            return BOAT_ERROR;
        }

        BoatLog(BOAT_LOG_VERBOSE, "cur_balance_wei: %s wei", prase_result.field_ptr);
        cur_balance_eth = UtilityWeiStrToEthDouble((BCHAR*)prase_result.field_ptr);
        BoatLog(BOAT_LOG_VERBOSE, "cur_balance_eth: %10lf ETH", cur_balance_eth);
        if (cur_balance_eth < 0.000001)
        {
            BoatLog(BOAT_LOG_NORMAL, "the account balance is not enough ETH, the current balance is: %10lf ETH", cur_balance_eth);
            return BOAT_ERROR;
        }

        result = BoatCallReadStore(g_boat_ethereum_wallet_ptr);
    }

    if ( BOAT_SUCCESS != result )
    {
        BoatLog(BOAT_LOG_NORMAL, "Block to Ethereum Chain Failed: %d.", result);
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Block to Ethereum Chain Passed.");
    }

    //free prase_result
    BoatFree(prase_result.field_ptr);
    prase_result.field_ptr = 0;
	
    return result;
}

