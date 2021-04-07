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

#if defined(__unix__) || defined(__unix) || defined(unix)
	/* for timestamp source */
	#define _POSIX_C_SOURCE 200809L
	#include <time.h>
#endif

#include "boatiotsdk.h"

#if PROTOCOL_USE_HLFABRIC == 1

BoatHlfabricWallet *g_fabric_wallet_ptr;

__BOATSTATIC BOAT_RESULT fabricWalletPrepare(void)
{
	BOAT_RESULT index;

	BoatHlfabricWalletConfig wallet_config = {
		{}, // client private key context							
		"/mnt/sharework/boatiotsdk_fabric/demo/demo_key/fabric_client.cert",// client certificate path		

		{},    // tls client private key context		
		"",    // tls client certificate path	
		
		//root ca
		3, //--the number of rootCA file to be set
		{
			"/mnt/sharework/boatiotsdk_fabric/demo/demo_key/tlsca/tlsca.org1.example.com-cert.pem",
			"/mnt/sharework/boatiotsdk_fabric/demo/demo_key/tlsca/tlsca.org2.example.com-cert.pem",
			"/mnt/sharework/boatiotsdk_fabric/demo/demo_key/tlsca/tlsca.example.com-cert.pem"
		}, 

		// endorsor
		2, //--the number of endorsor to be set
        { 
			{ "172.22.0.5:7051", "peer0.org1.example.com" },
			{ "172.22.0.2:7051", "peer0.org2.example.com" }
		},        
		
		// orderer
		2, //--the number of orderer to be set
		{
			{ "172.22.0.6:7050", "orderer.example.com"},
			{ "172.22.0.6:7050", "orderer.example.com"}
		}
	};

	/* create fabric wallet */
	index = BoatWalletCreate( BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig) );
	if(index == BOAT_ERROR)
	{
		BoatLog( BOAT_LOG_CRITICAL, "fabricWalletPrepare failed." );
		return BOAT_ERROR;
	}
	g_fabric_wallet_ptr = BoatGetWalletByIndex( index );
	
	return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT       result  = BOAT_SUCCESS;
	BoatHlfabricTx    tx_ptr;
	
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
	
	/* step-2: prepare wallet */
	result = fabricWalletPrepare();
	if( result != BOAT_SUCCESS )
	{
		BoatLog( BOAT_LOG_CRITICAL, "fabricWalletPrepare failed." );
		return -1;
	}

	/* step-3: fabric transaction structure initialization */
	result = BoatHlfabricTxInit( &tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP" );
	if( result != BOAT_SUCCESS )
	{
		BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxInit failed." );
		return -1;
	}
	
	/* step-4: set transaction timestamp */
#if defined(__unix__) || defined(__unix) || defined(unix)
	struct timespec txTimestamp;
	clock_gettime(CLOCK_REALTIME, &txTimestamp);
	result = BoatHlfabricTxSetTimestamp( &tx_ptr, txTimestamp.tv_sec, txTimestamp.tv_nsec );
#else
	long int timesec = 0;
	time(&timesec);
	result = BoatHlfabricTxSetTimestamp( &tx_ptr, timesec, 0 );
#endif
	if( result != BOAT_SUCCESS )
	{
		BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSetTimestamp failed." );
		return -1;
	}
	
	/* step-5: set transaction 'invoke' command */
	result += BoatHlfabricTxSetArgs( &tx_ptr, "invoke", "a", "b", "10", NULL );
	result += BoatHlfabricTxSubmit( &tx_ptr );
	if( result != BOAT_SUCCESS )
	{
		BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSubmit(invoke) failed." );
		return -1;
	}
	
	/* step-6: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	result += BoatHlfabricTxSetArgs( &tx_ptr, "query", "a", NULL );
	result += BoatHlfabricTxSubmit( &tx_ptr );
	if( result != BOAT_SUCCESS )
	{
		BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSubmit(query) failed." );
		return -1;
	}
	
	/* step-7: fabric transaction structure Deinitialization */
	BoatHlfabricTxDeInit( &tx_ptr );
	
	/* step-8: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
#else
int main(int argc, char *argv[])
{
    BoatLog(BOAT_LOG_NORMAL, "undefine PROTOCOL_USE_HLFABRIC");
    return 0;
}
#endif /* end of PROTOCOL_USE_HLFABRIC */
