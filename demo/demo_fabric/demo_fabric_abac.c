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
#include "boatconfig.h"
#include "boatiotsdk.h"

const BCHAR * fabric_client_demokey  = "-----BEGIN PRIVATE KEY-----\n"
			 						   "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgZv0mwHQMEGry976y\n"
									   "yJK38MQq+s98MpPmkKCAxAYLm8+hRANCAAT8KxmQX+3tjzRp1KBJmT02inKXNLyE\n"
									   "8zyXeva18ib+GgF0Iu9VwydQQ9OPUJcHDDXFUARk/neouR+EhJwXzl98\n"
									   "-----END PRIVATE KEY-----\n";

const BCHAR * fabric_client_democert =  "-----BEGIN CERTIFICATE-----\n"
										"MIICKTCCAdCgAwIBAgIQbidboH9ELvx/J9mItlQF7DAKBggqhkjOPQQDAjBzMQsw\n"
										"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
										"YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\n"
										"b3JnMS5leGFtcGxlLmNvbTAeFw0yMDA4MDUwMjQ2MDBaFw0zMDA4MDMwMjQ2MDBa\n"
										"MGwxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\n"
										"YW4gRnJhbmNpc2NvMQ8wDQYDVQQLEwZjbGllbnQxHzAdBgNVBAMMFkFkbWluQG9y\n"
										"ZzEuZXhhbXBsZS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAT8KxmQX+3t\n"
										"jzRp1KBJmT02inKXNLyE8zyXeva18ib+GgF0Iu9VwydQQ9OPUJcHDDXFUARk/neo\n"
										"uR+EhJwXzl98o00wSzAOBgNVHQ8BAf8EBAMCB4AwDAYDVR0TAQH/BAIwADArBgNV\n"
										"HSMEJDAigCAB71fDwWrZRYkhqsRX9seAeHd0/stBjokMM3WCDI9d4DAKBggqhkjO\n"
										"PQQDAgNHADBEAiBGh6koZBjIYhFVDeopES8kIhhpB8u842wE/X3L0bS8zgIgStgp\n"
										"kdXoRNjOlCG+XF7cxGCo77e1fRebDPuniOT/ZSg=\n"
										"-----END CERTIFICATE-----\n";

const BCHAR * fabric_ca1_democert    =  "-----BEGIN CERTIFICATE-----\n"
										"MIICRDCCAeqgAwIBAgIRAOATr0Krm9Rv+57iYJTuLEMwCgYIKoZIzj0EAwIwbDEL\n"
										"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
										"cmFuY2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRowGAYDVQQDExF0bHNjYS5l\n"
										"eGFtcGxlLmNvbTAeFw0yMDA4MDUwMjQ2MDBaFw0zMDA4MDMwMjQ2MDBaMGwxCzAJ\n"
										"BgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4gRnJh\n"
										"bmNpc2NvMRQwEgYDVQQKEwtleGFtcGxlLmNvbTEaMBgGA1UEAxMRdGxzY2EuZXhh\n"
										"bXBsZS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATrW0uxJWeqTpT7/yG6\n"
										"IkB42AjmvOz9ZEIkYOdGrG7fCwi2KLvqW+2RSJkQXJTqtNVPTRrUSdqOBIAg3u5G\n"
										"6/V8o20wazAOBgNVHQ8BAf8EBAMCAaYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsG\n"
										"AQUFBwMBMA8GA1UdEwEB/wQFMAMBAf8wKQYDVR0OBCIEICRC9l3Em13MwZMSklSX\n"
										"Er+MYZhQmWljFLtRHt6kw5XLMAoGCCqGSM49BAMCA0gAMEUCIQDt30JMkvsHuNoj\n"
										"mdO6Jfu+Y6aNwKfGqtKNHjjjDqYZ4gIgR2aw/fgADnrneridJIMfa8o1dtrNDY8+\n"
										"D4GPkR5WdCg=\n"
										"-----END CERTIFICATE-----\n";

const BCHAR * fabric_ca2_democert    =  "-----BEGIN CERTIFICATE-----\n"
										"MIICVzCCAf2gAwIBAgIQW0b9mh4yNgH+axd+V7Om8jAKBggqhkjOPQQDAjB2MQsw\n"
										"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
										"YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
										"Y2Eub3JnMS5leGFtcGxlLmNvbTAeFw0yMDA4MDUwMjQ2MDBaFw0zMDA4MDMwMjQ2\n"
										"MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
										"Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcxLmV4YW1wbGUuY29tMR8wHQYD\n"
										"VQQDExZ0bHNjYS5vcmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
										"AQcDQgAEkyNak1/sm+w4KvMZ0hDXdcWHyr5Lxyo/MjxfsdYyXApqR6/y8Q1UZfOe\n"
										"p1melqcBsD8XqbP3kuOO5GDjhFB2a6NtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
										"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
										"DgQiBCDI/TuNQoTL09NgoUn4pQUiL+APeDIDOCF9/alO1glcNjAKBggqhkjOPQQD\n"
										"AgNIADBFAiEAj4506XYUf0agFjgPN2ZthNQRmZP0RZDDBTWLAQO7MIMCIBr7NnOM\n"
										"SmNAylm4AGsA4IwAEouddvWs6RNOQDudp4PT\n"
										"-----END CERTIFICATE-----\n";

const BCHAR * fabric_ca3_democert    =  "-----BEGIN CERTIFICATE-----\n"
										"MIICVjCCAf2gAwIBAgIQafAbhNWN6YxNmaCfuDE2RjAKBggqhkjOPQQDAjB2MQsw\n"
										"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
										"YW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
										"Y2Eub3JnMi5leGFtcGxlLmNvbTAeFw0yMDA4MDUwMjQ2MDBaFw0zMDA4MDMwMjQ2\n"
										"MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
										"Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcyLmV4YW1wbGUuY29tMR8wHQYD\n"
										"VQQDExZ0bHNjYS5vcmcyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
										"AQcDQgAE1tqwF+tFY+1vG00p24nrrSbH2Jx1+geJKBWfnVt15ce8nSdnjyXeh8nq\n"
										"n+zfJCGYXarAATq0INDql0D3+GfhdaNtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
										"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
										"DgQiBCA74Y5aGIZdMDDA1q44ECa+2PwFAOwOhlUf4ts7ZwvIYzAKBggqhkjOPQQD\n"
										"AgNHADBEAiBa77f+WNSv/UNmBf99yLXMb3kA8TyJl/4oHNuChc5U+QIgfJ69NgEg\n"
										"b+hmfwiSjKQxlc4DDDfoFLdDAi4WtBO/QLM=\n"
										"-----END CERTIFICATE-----\n";

const BCHAR * fabric_demo_endorser1_url      = "172.18.0.5:7051";
const BCHAR * fabric_demo_endorser1_hostName = "peer0.org1.example.com";
const BCHAR * fabric_demo_endorser2_url      = "172.18.0.4:7051";
const BCHAR * fabric_demo_endorser2_hostName = "peer0.org2.example.com";
const BCHAR * fabric_demo_order1_url         = "172.18.0.3:7050";
const BCHAR * fabric_demo_order1_hostName    = "orderer.example.com";

BoatHlfabricWallet *g_fabric_wallet_ptr;

__BOATSTATIC BOAT_RESULT fabricWalletPrepare(void)
{
	BOAT_RESULT index;

	BoatHlfabricWalletConfig wallet_config = {0};
	//set private key context
	wallet_config.accountPriKey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.accountPriKey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.accountPriKey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.accountPriKey_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_demokey;
	wallet_config.accountPriKey_config.prikey_content.field_len = strlen(fabric_client_demokey) + 1; //length contain terminator

	//set cert context
	wallet_config.accountCertContent.length = strlen(fabric_client_democert) + 1;
	memcpy(wallet_config.accountCertContent.content , fabric_client_democert, wallet_config.accountCertContent.length);

	//set rootCA info
	wallet_config.rootCaNumber = 3;
	wallet_config.rootCaContent[0].length  = strlen(fabric_ca1_democert) + 1;
	memcpy(wallet_config.rootCaContent[0].content , fabric_ca1_democert,wallet_config.rootCaContent[0].length );
	wallet_config.rootCaContent[1].length  = strlen(fabric_ca2_democert) + 1;
	memcpy(wallet_config.rootCaContent[1].content , fabric_ca2_democert,wallet_config.rootCaContent[1].length );
	wallet_config.rootCaContent[2].length  = strlen(fabric_ca3_democert) + 1;
	memcpy(wallet_config.rootCaContent[2].content , fabric_ca3_democert,wallet_config.rootCaContent[2].length );

	//set endorser info
	wallet_config.endorserNumber = 2;
	memcpy(wallet_config.endorser[0].nodeUrl , fabric_demo_endorser1_url, strlen(fabric_demo_endorser1_url) + 1);
	memcpy(wallet_config.endorser[0].hostName, fabric_demo_endorser1_hostName, strlen(fabric_demo_endorser1_hostName) + 1);
	memcpy(wallet_config.endorser[1].nodeUrl , fabric_demo_endorser2_url, strlen(fabric_demo_endorser2_url) + 1);
	memcpy(wallet_config.endorser[1].hostName, fabric_demo_endorser2_hostName, strlen(fabric_demo_endorser2_hostName) + 1);

	//set orderer info
	wallet_config.ordererNumber = 1;
	memcpy(wallet_config.orderer[0].nodeUrl , fabric_demo_order1_url, strlen(fabric_demo_order1_url) + 1);
	memcpy(wallet_config.orderer[0].hostName, fabric_demo_order1_hostName, strlen(fabric_demo_order1_hostName) + 1);

	/* create fabric wallet */
#if defined( USE_ONETIME_WALLET )
	index = BoatWalletCreate( BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig) );
#elif defined( USE_CREATE_PERSIST_WALLET )
	index = BoatWalletCreate( BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig) );
#elif defined( USE_LOAD_PERSIST_WALLET )
	index = BoatWalletCreate( BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", NULL, sizeof(BoatHlfabricWalletConfig) );
#else
	return BOAT_ERROR;
#endif
	if(index == BOAT_ERROR)
	{
		//BoatLog( BOAT_LOG_CRITICAL, "fabricWalletPrepare failed." );
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
		//BoatLog( BOAT_LOG_CRITICAL, "fabricWalletPrepare failed." );
		return -1;
	}

	/* step-3: fabric transaction structure initialization */
	result = BoatHlfabricTxInit( &tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP" );
	if( result != BOAT_SUCCESS )
	{
		//BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxInit failed." );
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
		//BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSetTimestamp failed." );
		return -1;
	}
	
	/* step-5: set transaction 'invoke' command */
	result += BoatHlfabricTxSetArgs( &tx_ptr, "invoke", "a", "b", "10", NULL );
	result += BoatHlfabricTxSubmit( &tx_ptr );
	if( result != BOAT_SUCCESS )
	{
		//BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSubmit(invoke) failed." );
		return -1;
	}
	
	/* step-6: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	result += BoatHlfabricTxSetArgs( &tx_ptr, "query", "a", NULL );
	result += BoatHlfabricTxEvaluate( &tx_ptr );
	if( result != BOAT_SUCCESS )
	{
		//BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSubmit(query) failed." );
		return -1;
	}
	
	/* step-7: fabric transaction structure Deinitialization */
	BoatHlfabricTxDeInit( &tx_ptr );
	
	/* step-8: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
