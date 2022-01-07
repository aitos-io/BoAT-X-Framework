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
#include "boatlog.h"
#include <time.h>


const BCHAR *fabric_client_demokey = "-----BEGIN PRIVATE KEY-----\n"
"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgC7i8hacnI2wqQwk7\n"
"VdgA/o8v2QtsXqbahPi69og5tZShRANCAAS7Ml9eVj84iFqapPim8w/HWdg0mQJe\n"
"TEnR3lnc9HYpX9kKFv2fqr7jSjEgHnkl8b2jAhnxT5xdoR2oVXDzcwli\n"
									 "-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_democert = "-----BEGIN CERTIFICATE-----\n"
"MIICKjCCAdGgAwIBAgIRAI0L3xoUFFr70L1cOulWdKMwCgYIKoZIzj0EAwIwczEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh\n"
"Lm9yZzEuZXhhbXBsZS5jb20wHhcNMjExMjI0MDI1ODAwWhcNMzExMjIyMDI1ODAw\n"
"WjBsMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\n"
"U2FuIEZyYW5jaXNjbzEPMA0GA1UECxMGY2xpZW50MR8wHQYDVQQDDBZVc2VyMUBv\n"
"cmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEuzJfXlY/\n"
"OIhamqT4pvMPx1nYNJkCXkxJ0d5Z3PR2KV/ZChb9n6q+40oxIB55JfG9owIZ8U+c\n"
"XaEdqFVw83MJYqNNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYD\n"
"VR0jBCQwIoAgI+E696/ogzYugT7xV3JcmrPctsc+cn+bIaonLnqfMW0wCgYIKoZI\n"
"zj0EAwIDRwAwRAIgeBdvFkP2EDUZ+vCK4M7I439moFgCvT40V4rOiM7pGLQCIGmz\n"
"wQKqZqrS+jyM1rv6dhaf0K0fz3CMvXrfKlf0NG89\n"
									  "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICVzCCAf6gAwIBAgIRAKTtoztI4X22hzMVl9iX4XIwCgYIKoZIzj0EAwIwdjEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHzAdBgNVBAMTFnRs\n"
"c2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMjExMjI0MDI1ODAwWhcNMzExMjIyMDI1\n"
"ODAwWjB2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UE\n"
"BxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0G\n"
"A1UEAxMWdGxzY2Eub3JnMS5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49\n"
"AwEHA0IABL3qQfhWhTm7RleBJGlhJEAKl2qhaQ09yEw9NWfOEndyZxJBIXBjrieV\n"
"tbQqf1Mb1e/QmPm/G7+6xB7QH8TGsUijbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNV\n"
"HSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNV\n"
"HQ4EIgQg5MQFxUeKY3+lv9Lf2XZYwTD+IRH9pEUEpQN5B3odpN8wCgYIKoZIzj0E\n"
"AwIDRwAwRAIgZxj4KIa/okvdnr3Vx8fFlCMsVY5MA96oDQoAxrluIBMCIGsp/zjG\n"
"AFzgDUuOCXh1pq3fkKvSipwtbL8FX/QKmfHD\n"
								   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICVzCCAf2gAwIBAgIQJ8qLnN5Lzk9LWwIVSJVMJjAKBggqhkjOPQQDAjB2MQsw\n"
"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
"YW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
"Y2Eub3JnMi5leGFtcGxlLmNvbTAeFw0yMTEyMjQwMjU4MDBaFw0zMTEyMjIwMjU4\n"
"MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
"Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcyLmV4YW1wbGUuY29tMR8wHQYD\n"
"VQQDExZ0bHNjYS5vcmcyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
"AQcDQgAEJh9kb3uL2rJ5QDjNN1nlVWDVRqBYIw2Rz7wPT2UBBkZQK3+wm3fvFVYN\n"
"/yIRDUXxTHOMdVMFGWhtvYvD0n+0JKNtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
"DgQiBCBPIsOtEM4aVcN6ZQ69ePg1wVpOdunnuOxeaNDElaMBkjAKBggqhkjOPQQD\n"
"AgNIADBFAiEAzwwSpX2yvzLI9WDxBmEMRqCaFP2tWSdWfbuCjGuIONACIH2atidq\n"
"eUkmzUD2rlmbSLfTX3HS6WGheYACFd0MDWlW\n"
								   "-----END CERTIFICATE-----\n";
							   													
const BCHAR *fabric_order_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICQzCCAeqgAwIBAgIRAIarlRJamkgzt7h+aLZ4J7gwCgYIKoZIzj0EAwIwbDEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRowGAYDVQQDExF0bHNjYS5l\n"
"eGFtcGxlLmNvbTAeFw0yMTEyMjQwMjU4MDBaFw0zMTEyMjIwMjU4MDBaMGwxCzAJ\n"
"BgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4gRnJh\n"
"bmNpc2NvMRQwEgYDVQQKEwtleGFtcGxlLmNvbTEaMBgGA1UEAxMRdGxzY2EuZXhh\n"
"bXBsZS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQRS02uAjXsGSbYCNmw\n"
"Roi1NA1S8a/Tg2Q9N7RKSx6N05kIXKGXUZCGzv2cPrjzaxp3RYEfV3lgTw2OgCy7\n"
"Imleo20wazAOBgNVHQ8BAf8EBAMCAaYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsG\n"
"AQUFBwMBMA8GA1UdEwEB/wQFMAMBAf8wKQYDVR0OBCIEIAYq5WJ5DZlLLWSA8OLT\n"
"tJBl6ys9Nex8wDNnceJPx3O+MAoGCCqGSM49BAMCA0cAMEQCIGoaWICEuKYQTBjA\n"
"I74fTLgEzbphv7d2wWKc20dvTBc+AiBjKPXwCXmYSZ27zzPD4kAfDfACAyhYVX68\n"
"WLpF8/G5cg==\n"
								   "-----END CERTIFICATE-----\n";	


const BCHAR *fabric_demo_order1_url = "192.168.132.190:7050";
const BCHAR *fabric_demo_order1_hostName = "orderer.example.com";

const BCHAR *fabric_demo_endorser_peer0Org1_url = "192.168.132.190:7051";
const BCHAR *fabric_demo_endorser_peer0Org1_hostName = "peer0.org1.example.com";
const BCHAR *fabric_demo_endorser_peer1Org1_url = "192.168.132.190:8051";
const BCHAR *fabric_demo_endorser_peer1Org1_hostName = "peer1.org1.example.com";

const BCHAR *fabric_demo_endorser_peer0Org2_url = "192.168.132.190:9051";
const BCHAR *fabric_demo_endorser_peer0Org2_hostName = "peer0.org2.example.com";
const BCHAR *fabric_demo_endorser_peer1Org2_url = "192.168.132.190:10051";
const BCHAR *fabric_demo_endorser_peer1Org2_hostName = "peer1.org2.example.com";


BoatHlfabricWallet *g_fabric_wallet_ptr;
// DiscoverRes discoverResult;
BoatHlfabricWalletConfig wallet_config = {0};
__BOATSTATIC BOAT_RESULT fabricWalletPrepare(void)
{
	BOAT_RESULT index;

	//BoatHlfabricWalletConfig wallet_config = {0};
	//set private key context
	wallet_config.accountPriKey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.accountPriKey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.accountPriKey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.accountPriKey_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_demokey;
	wallet_config.accountPriKey_config.prikey_content.field_len = strlen(fabric_client_demokey) + 1; //length contain terminator

	//set cert context
	wallet_config.accountCertContent.length = strlen(fabric_client_democert) + 1;
	memcpy(wallet_config.accountCertContent.content, fabric_client_democert, wallet_config.accountCertContent.length);

#if(DISCOVERY_PEER_QUERY != 1 )

	wallet_config.nodesCfg.endorserLayoutNum = 1;
	wallet_config.nodesCfg.layoutCfg = BoatMalloc(wallet_config.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,fabric_org1_tlsCert,strlen(fabric_org1_tlsCert));

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org1_url,strlen(fabric_demo_endorser_peer0Org1_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,fabric_demo_endorser_peer0Org1_hostName,strlen(fabric_demo_endorser_peer0Org1_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl,fabric_demo_endorser_peer1Org1_url,strlen(fabric_demo_endorser_peer1Org1_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName,fabric_demo_endorser_peer1Org1_hostName,strlen(fabric_demo_endorser_peer1Org1_hostName)+1);

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content,fabric_org2_tlsCert,strlen(fabric_org2_tlsCert));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org2_url,strlen(fabric_demo_endorser_peer0Org2_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName,fabric_demo_endorser_peer0Org2_hostName,strlen(fabric_demo_endorser_peer0Org2_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl,fabric_demo_endorser_peer1Org2_url,strlen(fabric_demo_endorser_peer1Org2_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName,fabric_demo_endorser_peer1Org2_hostName,strlen(fabric_demo_endorser_peer1Org2_hostName)+1);

	//set orderer info
	wallet_config.nodesCfg.orderCfg.endorserNumber = 1;
	wallet_config.nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
	memcpy(wallet_config.nodesCfg.orderCfg.tlsOrgCertContent.content,fabric_order_tlsCert,strlen(fabric_order_tlsCert));
	wallet_config.nodesCfg.orderCfg.endorser = BoatMalloc(wallet_config.nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric_demo_order1_hostName)+1);
	wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_order1_url)+1);
	memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl,fabric_demo_order1_url,strlen(fabric_demo_order1_url)+1);
	memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].hostName,fabric_demo_order1_hostName,strlen(fabric_demo_order1_hostName)+1);


#else
	wallet_config.nodesCfg.endorserLayoutNum = 1;
	wallet_config.nodesCfg.layoutCfg = BoatMalloc(wallet_config.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org1_url,strlen(fabric_demo_endorser_peer0Org1_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,fabric_demo_endorser_peer0Org1_hostName,strlen(fabric_demo_endorser_peer0Org1_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,fabric_org1_tlsCert,strlen(fabric_org1_tlsCert));

#endif
	/* create fabric wallet */
#if defined(USE_ONETIME_WALLET)
	index = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
#elif defined(USE_CREATE_PERSIST_WALLET)
	index = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));
#elif defined(USE_LOAD_PERSIST_WALLET)
	index = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", NULL, sizeof(BoatHlfabricWalletConfig));
#else
	return BOAT_ERROR;
#endif
	if (index < BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "fabricWalletPrepare failed.");
		return BOAT_ERROR_WALLET_CREATE_FAIL;
	}
	g_fabric_wallet_ptr = BoatGetWalletByIndex(index);
	
	return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHlfabricTx tx_ptr;
	boat_try_declare;
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
	
	/* step-2: prepare wallet */
	result = fabricWalletPrepare();
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "fabricWalletPrepare failed.");
		boat_throw(result, fabric_demo_catch);
	}

	/* step-3: fabric transaction structure initialization */
	result = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxInit failed.");
		boat_throw(result, fabric_demo_catch);
	}
#if(DISCOVERY_PEER_QUERY != 1 )
	result = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	if (result != BOAT_SUCCESS)
	{
		//BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSetTimestamp failed." );
		boat_throw(result, fabric_demo_catch);
	}
#endif
	/* step-4: set transaction timestamp */
#if defined(__unix__) || defined(__unix) || defined(unix)
	struct timespec txTimestamp;
	clock_gettime(CLOCK_REALTIME, &txTimestamp);
	result = BoatHlfabricTxSetTimestamp( &tx_ptr, txTimestamp.tv_sec, txTimestamp.tv_nsec);
#else
	long int timesec = 0;
	time(&timesec);
	result = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
#endif
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetTimestamp failed.");
		boat_throw(result, fabric_demo_catch);
	}

/*
	fabric discover nodes
	*/
#if(DISCOVERY_PEER_QUERY == 1 )
	
		BoatLog( BOAT_LOG_CRITICAL, "begin discover node   " );
		tx_ptr.var.args.nArgs = 0;
		BoatSleep(3);
		result += BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
		if (result != BOAT_SUCCESS)
		{
			//BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSubmit(query) failed." );
			boat_throw(result, fabric_demo_catch);
		}
		// discoverResult = tx_ptr.discooverRes;
		BoatHlfabricNodesCfg network_info = tx_ptr.wallet_ptr->network_info;
		BoatLog( BOAT_LOG_CRITICAL, "***************** peers ****************\n");
		for (int i = 0; i < network_info.endorserLayoutNum; i++)
		{
			BoatLog( BOAT_LOG_CRITICAL, "*************** layout[%d] ****************",i);
			for (int j = 0; j < network_info.layoutCfg[i].endorserGroupNum; j++)
			{
				BoatLog( BOAT_LOG_CRITICAL, "     quantities : %d ",network_info.layoutCfg[i].groupCfg[j].quantities);
				BoatLog( BOAT_LOG_CRITICAL, "      tlsCert :  \n%s",network_info.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content);
				for (int k = 0; k < network_info.layoutCfg[i].groupCfg[j].endorserNumber; k++)
				{
					BoatLog( BOAT_LOG_CRITICAL, "        host :  %s",network_info.layoutCfg[i].groupCfg[j].endorser[k].hostName);
					BoatLog( BOAT_LOG_CRITICAL, "        URL :  %s",network_info.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl);
				}
				
			}
			
		}
		BoatLog( BOAT_LOG_CRITICAL, "***************************************\n");
		BoatLog( BOAT_LOG_CRITICAL, "***************** orders ****************\n");
		BoatLog( BOAT_LOG_CRITICAL, "        tlsCert : %s ",network_info.orderCfg.tlsOrgCertContent.content);
		for (int i = 0; i < network_info.orderCfg.endorserNumber; i++)
		{
			BoatLog( BOAT_LOG_CRITICAL, "        URL : %s ",network_info.orderCfg.endorser[i].nodeUrl);
			
		}
		BoatLog( BOAT_LOG_CRITICAL, "***************************************\n");
		


		// BoatLog( BOAT_LOG_CRITICAL, "mychannel has [%d] orgs ",discoverResult.Peership.orgNum );
		// for (int i = 0; i < discoverResult.Peership.orgNum; i++)
		// {
		// 	BoatLog( BOAT_LOG_CRITICAL, "***************************************");
		// 	BoatLog( BOAT_LOG_CRITICAL, " [org%d] info :  ",i );
		// 	BoatLog( BOAT_LOG_CRITICAL, "        name     : %s   ",discoverResult.Peership.orgs[i].orgName );
		// 	BoatLog( BOAT_LOG_CRITICAL, "        peer num : %d   ",discoverResult.Peership.orgs[i].peerNum );
		// 	for (int j = 0; j < discoverResult.Peership.orgs[i].peerNum; j++)
		// 	{
		// 	BoatLog( BOAT_LOG_CRITICAL, "        peer[%d] URL  : %s   ",j,discoverResult.Peership.orgs[i].peers[j].url );
		//     BoatLog( BOAT_LOG_CRITICAL, "        peer[%d] cert : \n%s   ",j,discoverResult.Peership.orgs[i].peers[j].cert );					
		// 	}
			
		// 	BoatLog( BOAT_LOG_CRITICAL, "        [org%d] tlscert : \n%s   ",i,discoverResult.Peership.orgs[i].tlsCert );


		// 	BoatLog( BOAT_LOG_CRITICAL, "***************************************");
		// }

		
		// BoatLog( BOAT_LOG_CRITICAL, "*************  config  ***************");

		// for (int i = 0; i < discoverResult.cc_res.num; i++)
		// {
		// 	for (int j = 0; j < discoverResult.cc_res.layouts[i].num; j++)
		// 	{
		// 		BoatLog( BOAT_LOG_CRITICAL, "***layout[%d]-group[%d] : ",i,j );
		// 		BoatLog( BOAT_LOG_CRITICAL, "  %s :   ",discoverResult.cc_res.layouts[i].groups[j].key );
		// 		BoatLog( BOAT_LOG_CRITICAL, "      quantities :  %d ",discoverResult.cc_res.layouts[i].groups[j].value );
		// 		for (int k = 0; k < discoverResult.cc_res.layouts[i].groups[j].numEndorsers; k++)
		// 		{
		// 		BoatLog( BOAT_LOG_CRITICAL, "       MSPID    :  %s ",discoverResult.cc_res.layouts[i].groups[j].endorsers[k].MSPID );
		// 		BoatLog( BOAT_LOG_CRITICAL, "       Endpoint :  %s ",discoverResult.cc_res.layouts[i].groups[j].endorsers[k].Endpoint );
		// 		}
				
		// 	}
			
		// }
		// BoatLog( BOAT_LOG_CRITICAL, "****************************************\n");
		// BoatLog( BOAT_LOG_CRITICAL, "*************** msps *******************");

		// for (int i = 0; i < discoverResult.discoverConfig.discoverMsps.num; i++)
		// {
		// 	BoatLog( BOAT_LOG_CRITICAL, "  %s :   ",discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].name);
		// 	BoatLog( BOAT_LOG_CRITICAL, "      tlseCert:   \n\n%s",discoverResult.discoverConfig.discoverMsps.discoverMspInfo[i].tlsCert);
		// }
		// BoatLog( BOAT_LOG_CRITICAL, "****************************************\n");
		// BoatLog( BOAT_LOG_CRITICAL, "*************** orders *******************");

		// for (int i = 0; i < discoverResult.discoverConfig.discoverOrders.num; i++)
		// {
		// 	BoatLog( BOAT_LOG_CRITICAL, "  %s :   ",discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].name);
		// 	BoatLog( BOAT_LOG_CRITICAL, "      host :   %s",discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].host);
		// 	BoatLog( BOAT_LOG_CRITICAL, "      port :   %s",discoverResult.discoverConfig.discoverOrders.discoverOrderinfo[i].port);
		// }

		// BoatLog( BOAT_LOG_CRITICAL, "***************************************");
		
		
		
		// DiscoverResFree(discoverResult);
#endif


	/* step-5: set transaction 'invoke' command */
	result += BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
	result += BoatHlfabricTxSubmit(&tx_ptr); 
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		boat_throw(result, fabric_demo_catch);
	}
	/* step-6: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	result = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		boat_throw(result, fabric_demo_catch);
	}
	result = BoatHlfabricTxEvaluate(&tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxEvaluate() failed.");
		boat_throw(result, fabric_demo_catch);
	}
		BoatLog_hexasciidump(BOAT_LOG_NORMAL, "query result",
						 tx_ptr.evaluateRes.http2Res,
						 tx_ptr.evaluateRes.httpResLen);
	
    boat_catch(fabric_demo_catch)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
    }	
	/* step-7: fabric transaction structure Deinitialization */
	BoatHlfabricTxDeInit(&tx_ptr);
	
	/* step-8: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
	fabricWalletConfigFree(wallet_config);
    
    return result;
}
