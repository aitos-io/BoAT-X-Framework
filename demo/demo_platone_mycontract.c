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
#define  USE_CREATE_PERSIST_WALLET //if expect create a persist wallet, uncomment this definition
//#define  USE_LOAD_PERSIST_WALLET   // if expect load a persist wallet, uncomment this definition


#include "boatiotsdk.h"

#if PROTOCOL_USE_PLATONE == 1

#include "my_contract.cpp.abi.h"

BoatPlatoneWallet *g_platone_wallet_ptr;

#if defined( USE_ONETIME_WALLET )
__BOATSTATIC BOAT_RESULT platone_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;

	/* wallet_config value assignment */
	/* for one-time wallet, the 'prikeyId' field should be cleared */
	//memset(wallet_config.prikeyId, 0, BOAT_KEYID_MAX_LEN); 
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy( wallet_config.node_url_str, "http://116.236.47.90:7545", BOAT_NODE_URL_MAX_LEN - 1 );

	/* create platone wallet */
    index = BoatWalletCreate( BOAT_PROTOCOL_PLATONE, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig) );
    if( index == BOAT_ERROR )
	{
        BoatLog( BOAT_LOG_CRITICAL, "create one-time wallet failed." );
        return BOAT_ERROR;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex( index );
    
    return BOAT_SUCCESS;
}
#endif

#if defined( USE_CREATE_PERSIST_WALLET )
__BOATSTATIC BOAT_RESULT platone_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;

	/* wallet_config value assignment */
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;

    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy( wallet_config.node_url_str, "http://116.236.47.90:7545", BOAT_NODE_URL_MAX_LEN - 1 );

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if( index == BOAT_ERROR )
	{
        BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR;
    }

    g_platone_wallet_ptr = BoatGetWalletByIndex( index );

    return BOAT_SUCCESS;
}
#endif

#if defined( USE_LOAD_PERSIST_WALLET )
__BOATSTATIC BOAT_RESULT platone_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, NULL, sizeof(BoatPlatoneWalletConfig));
    if( index == BOAT_ERROR )
	{
        BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex( index );

    return BOAT_SUCCESS;
}
#endif

BOAT_RESULT platone_call_mycontract(BoatPlatoneWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    BOAT_RESULT result;
    
    /* Set Contract Address */
    result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
							   "0x333333",
							   "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45",
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_NORMAL, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
    if( result_str == NULL )
	{
        BoatLog(BOAT_LOG_NORMAL, "my_contract_cpp_abi_setName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if( result_str == NULL )
	{
        BoatLog(BOAT_LOG_NORMAL, "my_contract_cpp_abi_getName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);
	
    return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT  result  = BOAT_SUCCESS;

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
	/* step-2: create platone wallet */
#if defined( USE_ONETIME_WALLET )	
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = platone_createOnetimeWallet();
#elif defined( USE_CREATE_PERSIST_WALLET )
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
	result = platone_createPersistWallet("platone.cfg");
#elif defined( USE_LOAD_PERSIST_WALLET )
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
	result = platone_loadPersistWallet("platone.cfg");
#else
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
	return -1;
#endif	
    if( result != BOAT_SUCCESS )
	{
		 BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
		return -1;
	}
    
	/* step-3: execute 'platone_call_mycontract' */
	result += platone_call_mycontract( g_platone_wallet_ptr );
    if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_NORMAL, "platone mycontract access Failed: %d.", result);
    }
	else
	{
        BoatLog(BOAT_LOG_NORMAL, "platone mycontract access Passed.");
    }
	
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
#else
int main(int argc, char *argv[])
{
    BoatLog(BOAT_LOG_NORMAL, "undefine PROTOCOL_USE_PLATONE");
    return 0;
}
#endif /* end of PROTOCOL_USE_PLATONE */
