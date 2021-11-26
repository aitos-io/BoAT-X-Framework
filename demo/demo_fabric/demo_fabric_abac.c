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
"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgRo7NTfu+hstvPUD2\n"
"5GzWT5iIzReYMy3zk5k1jMFRbvWhRANCAASsOILY3M96tu+5uVA0drU3oH/U/sYp\n"
"8Up42NA0KZOfcrUQqHGEs5rOBSrLvqXTV4bgD6xeWCEaqmbwO5NdlwzL\n"
									 "-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_democert = "-----BEGIN CERTIFICATE-----\n"
"MIICKjCCAdGgAwIBAgIRANxVVKQKNFaIMd1gLYrMMqkwCgYIKoZIzj0EAwIwczEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh\n"
"Lm9yZzEuZXhhbXBsZS5jb20wHhcNMjExMDE4MDczNDAwWhcNMzExMDE2MDczNDAw\n"
"WjBsMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\n"
"U2FuIEZyYW5jaXNjbzEPMA0GA1UECxMGY2xpZW50MR8wHQYDVQQDDBZVc2VyMUBv\n"
"cmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAErDiC2NzP\n"
"erbvublQNHa1N6B/1P7GKfFKeNjQNCmTn3K1EKhxhLOazgUqy76l01eG4A+sXlgh\n"
"Gqpm8DuTXZcMy6NNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYD\n"
"VR0jBCQwIoAgxfbo49aqjtj8mmA5NaPbmnQG0rjg3dXs/rGZcggYTdswCgYIKoZI\n"
"zj0EAwIDRwAwRAIgAkLbCpnTVmaexbtmRoW4POSXFH8f0HnuIvWsgiiOR7ICIB29\n"
"GRvXfWzNs13j4r2io6cSyqeCYmCegtfvkHDB/64G\n"
									  "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICWDCCAf6gAwIBAgIRALavTC3eN2nrCKme6Xcww0kwCgYIKoZIzj0EAwIwdjEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHzAdBgNVBAMTFnRs\n"
"c2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMjExMDE4MDczNDAwWhcNMzExMDE2MDcz\n"
"NDAwWjB2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UE\n"
"BxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0G\n"
"A1UEAxMWdGxzY2Eub3JnMS5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49\n"
"AwEHA0IABG2qqN+yTP+lybXy8w57n/xXluvV3V3mBgfxOaAXY7eHKsxFrmotmycJ\n"
"/iVB4qZvCJsN4Gc0ZfiHdmCWfxMG53CjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNV\n"
"HSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNV\n"
"HQ4EIgQg4UXq2eXIK607YxT11FuZ/KvQMWPJfXYYBBpKiyvapPEwCgYIKoZIzj0E\n"
"AwIDSAAwRQIhAMJSbkX0/VJJYjOCaBbIiO5737ypVjJCMxpATMAgHbIBAiASL7jB\n"
"9YhVBBkfKXDJeyFLZRBiZvbZRe6XriSaGk7Olg==\n"
								   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICWDCCAf6gAwIBAgIRAPIYbekJM20WSWo6WtKJTwIwCgYIKoZIzj0EAwIwdjEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzIuZXhhbXBsZS5jb20xHzAdBgNVBAMTFnRs\n"
"c2NhLm9yZzIuZXhhbXBsZS5jb20wHhcNMjExMDE4MDczNDAwWhcNMzExMDE2MDcz\n"
"NDAwWjB2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UE\n"
"BxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEfMB0G\n"
"A1UEAxMWdGxzY2Eub3JnMi5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49\n"
"AwEHA0IABE1BVJL8TtT1/0yq7/6m6xfH+3M05LrNnOWGIS+jESAObzufayctNtHr\n"
"dI+3Pc+EZlU1fInmaB0T4Z6l88SQHp+jbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNV\n"
"HSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNV\n"
"HQ4EIgQg+KemF/ny1HXUk2PyArZVealsefPrb18DGVO/uPyeW+4wCgYIKoZIzj0E\n"
"AwIDSAAwRQIhAKv3XfoMbucEuvmE/TkYe6zldnBj5BxWS2hfNOwMMBfFAiAnRoBG\n"
"HEA7sDYxLdt9tFSUgxNlz/apvkDOmyHcD+DLrg==\n"
								   "-----END CERTIFICATE-----\n";
							   													
const BCHAR *fabric_order_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICQzCCAeqgAwIBAgIRAKfIsEodsIVIcEP6/UyeQxswCgYIKoZIzj0EAwIwbDEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRowGAYDVQQDExF0bHNjYS5l\n"
"eGFtcGxlLmNvbTAeFw0yMTEwMTgwNzM0MDBaFw0zMTEwMTYwNzM0MDBaMGwxCzAJ\n"
"BgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4gRnJh\n"
"bmNpc2NvMRQwEgYDVQQKEwtleGFtcGxlLmNvbTEaMBgGA1UEAxMRdGxzY2EuZXhh\n"
"bXBsZS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASZrTs7XIY3vC0nJ4XN\n"
"vwy8mVEhY1Yf3L9LgUNRRcX/HKRCXZmov02YJ9i35vbhyI4UMV5rniAZ8T6sSdsS\n"
"JV85o20wazAOBgNVHQ8BAf8EBAMCAaYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsG\n"
"AQUFBwMBMA8GA1UdEwEB/wQFMAMBAf8wKQYDVR0OBCIEILh4pUZHxoGSuxuQcZVf\n"
"+gVoJlWQyUJjwVAyJYDmgDtgMAoGCCqGSM49BAMCA0cAMEQCIBEyiUrBGqHa/cnP\n"
"d/F4N97iyxJmCCGYW2w9Q6546gvHAiAmLA+pzfCmUzQlZ1vBXUWHnSiCMEVHmLmX\n"
"H9kXw3RJnw==\n"
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
	if (index == BOAT_ERROR)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "fabricWalletPrepare failed.");
		return BOAT_ERROR;
	}
	g_fabric_wallet_ptr = BoatGetWalletByIndex(index);
	
	return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHlfabricTx tx_ptr;
	
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
	
	/* step-2: prepare wallet */
	result = fabricWalletPrepare();
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "fabricWalletPrepare failed.");
		return -1;
	}

	/* step-3: fabric transaction structure initialization */
	result = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxInit failed.");
		return -1;
	}
#if(DISCOVERY_PEER_QUERY != 1 )
	result = BoatHlfabricWalletSetNetworkInfo(tx_ptr.wallet_ptr, wallet_config.nodesCfg);
	if (result != BOAT_SUCCESS)
	{
		//BoatLog( BOAT_LOG_CRITICAL, "BoatHlfabricTxSetTimestamp failed." );
		return -1;
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
		return -1;
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
			return -1;
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
		return -1;
	}
	/* step-6: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	result = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		return -1;
	}
	result = BoatHlfabricTxEvaluate(&tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxEvaluate() failed.");
		return -1;
	}
		BoatLog_hexasciidump(BOAT_LOG_NORMAL, "query result",
						 tx_ptr.evaluateRes.http2Res,
						 tx_ptr.evaluateRes.httpResLen);
	
	/* step-7: fabric transaction structure Deinitialization */
	BoatHlfabricTxDeInit(&tx_ptr);
	
	/* step-8: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
