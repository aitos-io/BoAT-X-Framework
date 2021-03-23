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

//#define  USE_ONETIME_WALLET        // if expect create a one-time wallet, uncomment this definition
//#define  USE_CREATE_PERSIST_WALLET // if expect create a persist wallet, uncomment this definition
#define  USE_LOAD_PERSIST_WALLET   // if expect load a persist wallet, uncomment this definition


#include "boatiotsdk.h"

BoatEthWallet *g_ethereum_wallet_ptr;

#if defined( USE_ONETIME_WALLET )
__BOATSTATIC BOAT_RESULT ethereum_createOnetimeWallet()
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;

	/* wallet_config value assignment */
	/* for one-time wallet, the 'prikeyId' field should be cleared */
	memset(wallet_config.prikeyId, 0, BOAT_KEYID_MAX_LEN); 
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, "http://192.168.59.1:7545", BOAT_NODE_URL_MAX_LEN-1);

	/* create ethereum wallet */
    index = BoatWalletCreate( BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig) );
    if( index == BOAT_ERROR )
	{
        BoatLog( BOAT_LOG_CRITICAL, "create one-time wallet failed." );
        return BOAT_ERROR;
    }
    g_ethereum_wallet_ptr = BoatGetWalletByIndex( index );
    
    return BOAT_SUCCESS;
}
#endif

#if defined( USE_CREATE_PERSIST_WALLET )
__BOATSTATIC BOAT_RESULT ethereum_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config;

	/* wallet_config value assignment */
	strncpy( (char*)wallet_config.prikeyId, 
			 "/mnt/sharework/boatiotsdk_fabric/demo/demo_key/ethereum_client.key", 
			 BOAT_KEYID_MAX_LEN - 1 );
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy( wallet_config.node_url_str, 
			 "http://192.168.59.1:7545", 
			 BOAT_NODE_URL_MAX_LEN - 1 );

	/* create ethereum wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, wallet_name, &wallet_config, sizeof(BoatEthWalletConfig));
    if( index == BOAT_ERROR )
	{
        BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR;
    }

    g_ethereum_wallet_ptr = BoatGetWalletByIndex( index );

    return BOAT_SUCCESS;
}
#endif

#if defined( USE_LOAD_PERSIST_WALLET )
__BOATSTATIC BOAT_RESULT ethereum_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create ethereum wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, wallet_name, NULL, sizeof(BoatEthWalletConfig));
    if( index == BOAT_ERROR )
	{
        BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR;
    }
    g_ethereum_wallet_ptr = BoatGetWalletByIndex( index );

    return BOAT_SUCCESS;
}
#endif

BOAT_RESULT ethereumGetBalance(BoatEthWallet *wallet_ptr)
{
    //BCHAR * balance_wei;
    BCHAR * cur_balance_wei = NULL;
    BOAT_RESULT result;
    BoatFieldVariable prase_result = {NULL, 0};

    cur_balance_wei = BoatEthWalletGetBalance(wallet_ptr, NULL);
	result          = BoatEthPraseRpcResponseResult( cur_balance_wei, "", &prase_result);
	if( result == BOAT_SUCCESS )
	{
		BoatLog( BOAT_LOG_NORMAL, "BoatEthWalletGetBalance returns: %s", prase_result.field_ptr );
	}
	else
	{
		return BOAT_ERROR;
	}

    BoatLog(BOAT_LOG_NORMAL, "Balance: %s wei", prase_result.field_ptr);

    return BOAT_SUCCESS;
}


BOAT_RESULT ethereumTransfer(BoatEthWallet *wallet_ptr)
{
    BOAT_RESULT result;
    BoatEthTx tx_ctx;
    
    /* Set Recipient Address */
    result = BoatEthTxInit( wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						    "0x333333",
						    "0x6720AE6BB9bccFE2F0b8c4a72D3e94b78dF17ddD" );

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatEthTxInit failed.");
        return BOAT_ERROR;
    }
    
	/* 0xDE0B6B3A7640000: 1ETH or 1e18 wei, value */
	/* 0x2386F26FC10000: 0.01ETH or 1e16 wei, value */
    result = BoatEthTransfer( &tx_ctx, "0x2386F26FC10000" );

    return result;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT  result  = BOAT_SUCCESS;
	
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
	/* step-2: create ethereum wallet */
#if defined( USE_ONETIME_WALLET )
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = ethereum_createOnetimeWallet();
#elif defined( USE_CREATE_PERSIST_WALLET )
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
	result = ethereum_createPersistWallet("eth.cfg");
#elif defined( USE_LOAD_PERSIST_WALLET )
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
	result = ethereum_loadPersistWallet("eth.cfg");
#else
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
	return -1;
#endif	
    if( result != BOAT_SUCCESS )
	{
		 BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed : %d.", result);
		return -1;
	}
    
	/* step-3: execute balance transfer */
	result += ethereumGetBalance( g_ethereum_wallet_ptr );
	result += ethereumTransfer( g_ethereum_wallet_ptr );
	result += ethereumGetBalance( g_ethereum_wallet_ptr );
    if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_NORMAL, "CaseEthereum Failed: %d.", result);
    }
	else
	{
        BoatLog(BOAT_LOG_NORMAL, "CaseEthereum Passed.");
    }
	
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
