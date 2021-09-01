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

#define USE_ONETIME_WALLET

const BCHAR *huawei_client_demokey = "-----BEGIN PRIVATE KEY-----\n"
"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg9d6xlIhBpXL1jyGg\n"
"1cIqhPXvAzf/U/y1qfJAPnC1uiKhRANCAAQEiZePJltVa2rl8ew2lFkE0qMVhYm5\n"
"EzaEs67tFKPDe421v2datMggAo0gJop9NrRO1P2NN5WkvaSfCG8kwyMK\n"
									 "-----END PRIVATE KEY-----\n";

const BCHAR *huawei_client_democert = "-----BEGIN CERTIFICATE-----\n"
"MIIClDCCAjqgAwIBAgIQUoHlTcUu5HjLbv2HSUilBzAKBggqhkjOPQQDAjCBhjEO\n"
"MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
"bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
"Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
"NTE0OFoXDTMxMDgxOTA0NTE0OFowgYoxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
"EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
"DQYDVQQLEwZIdWF3ZWkxMTAvBgNVBAMTKHVzZXItMS5vcmcxLmJjcy1lcG1tbGku\n"
"c3ZjLmNsdXN0ZXIubG9jYWwwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQEiZeP\n"
"JltVa2rl8ew2lFkE0qMVhYm5EzaEs67tFKPDe421v2datMggAo0gJop9NrRO1P2N\n"
"N5WkvaSfCG8kwyMKo4GDMIGAMA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAA\n"
"MCsGA1UdIwQkMCKAIK1MsN0yPniDh8jtrSxAvxwR2XpL8dCm0lTvqN4tNlWVMDMG\n"
"A1UdEQQsMCqCKHVzZXItMS5vcmcxLmJjcy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9j\n"
"YWwwCgYIKoZIzj0EAwIDSAAwRQIhAPECJCuqWtLi9cfnxG31IlBGfD4OKII8svV3\n"
"URYn+CQKAiBzKCZk1g+5GH/DvqqJhspp6zB1GsoUXwN/o641s3l4bg==\n"
									  "-----END CERTIFICATE-----\n";

const BCHAR *huawei_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICrDCCAlKgAwIBAgIQdvsQdvTNG+rVbXHhnOS9mzAKBggqhkjOPQQDAjCBhjEO\n"
"MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
"bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
"Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
"NTE0OFoXDTMxMDgxOTA0NTE0OFowgYYxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
"EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
"DQYDVQQLEwZIdWF3ZWkxLTArBgNVBAMTJGNhLm9yZzEuYmNzLWVwbW1saS5zdmMu\n"
"Y2x1c3Rlci5sb2NhbDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABID+pcLWFgiw\n"
"4nlEQNma0QFtlLrDV0YXfyKB1EwmyGynOosbf1mz9qzbzCjtUyZhDJnOhTSbq8uf\n"
"D7OOf9m5JeijgZ8wgZwwDgYDVR0PAQH/BAQDAgGmMB0GA1UdJQQWMBQGCCsGAQUF\n"
"BwMBBggrBgEFBQcDAjAPBgNVHRMBAf8EBTADAQH/MCkGA1UdDgQiBCC+pd8v/ch2\n"
"igg6x8bgJHiBMFB/ABiul6O58eNruCn+IzAvBgNVHREEKDAmgiRjYS5vcmcxLmJj\n"
"cy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9jYWwwCgYIKoZIzj0EAwIDSAAwRQIgIR4F\n"
"YQDnjxHgoOha5N1Ec1aee+8R+n+un6Vu/q5G7RcCIQD0h5Ylb4ytyFxo21QhtFJR\n"
"Jq7sq/hJ+yziR0ZrVnKXeA==\n"
// "MIIClDCCAjqgAwIBAgIQUfKiHXReppD/PuCucebvDTAKBggqhkjOPQQDAjCBhjEO\n"
// "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
// "bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
// "Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
// "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYoxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
// "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
// "DQYDVQQLEwZIdWF3ZWkxMTAvBgNVBAMTKHVzZXItMS5vcmcxLmJjcy1lcG1tbGku\n"
// "c3ZjLmNsdXN0ZXIubG9jYWwwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQ8JbFm\n"
// "u20E7yHZWMkadZJ4QUBL96kWE97bmGukJs0nc6DS34OXmZHIiDetQkNkCX5bcsOw\n"
// "m0hVXQnrkGQdFEIFo4GDMIGAMA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAA\n"
// "MCsGA1UdIwQkMCKAIL6l3y/9yHaKCDrHxuAkeIEwUH8AGK6Xo7nx42u4Kf4jMDMG\n"
// "A1UdEQQsMCqCKHVzZXItMS5vcmcxLmJjcy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9j\n"
// "YWwwCgYIKoZIzj0EAwIDSAAwRQIhAMGaCGD1uKTW4B8PNHoMfTrBgJmjmSbOMV26\n"
// "JDK8ZAySAiBMMQGhCdMJlVCOLphkHYiExVQJa1nMaos0xzAOX9UBUQ==\n"

								   "-----END CERTIFICATE-----\n";

const BCHAR *huawei_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICqzCCAlKgAwIBAgIQQgfD2Vp1Gr+aAo4w73iXbzAKBggqhkjOPQQDAjCBhjEO\n"
"MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
"bnpoZW4xDTALBgNVBAoTBG9yZzIxDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
"Y2Eub3JnMi5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
"NTE0OFoXDTMxMDgxOTA0NTE0OFowgYYxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
"EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcyMQ8w\n"
"DQYDVQQLEwZIdWF3ZWkxLTArBgNVBAMTJGNhLm9yZzIuYmNzLWVwbW1saS5zdmMu\n"
"Y2x1c3Rlci5sb2NhbDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABI1P9XWwX1Ub\n"
"c3kGY6OOfrh/jGVEM2Aci8+tZozrpxASfj3O5/YNPFjXJAbCDHs+/cqFBNTXxL0Y\n"
"zQ8G2mEzW/2jgZ8wgZwwDgYDVR0PAQH/BAQDAgGmMB0GA1UdJQQWMBQGCCsGAQUF\n"
"BwMBBggrBgEFBQcDAjAPBgNVHRMBAf8EBTADAQH/MCkGA1UdDgQiBCAWIw2jwEOU\n"
"TqzkfhdBOMtJmHphhAD0FLxlSCPeP8UGYzAvBgNVHREEKDAmgiRjYS5vcmcyLmJj\n"
"cy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9jYWwwCgYIKoZIzj0EAwIDRwAwRAIgKSMJ\n"
"YDSTPdX0BsiLKu0f0IFauU1E/pgi3jMU3SuhPxgCIFUuVfU/pfTRxK6UGFYH5uxc\n"
"AtrZMPym0MX872awTHPL\n"
								   "-----END CERTIFICATE-----\n";

const BCHAR *huawei_demo_endorser_peer0Org1_url = "49.4.115.203:30616";
const BCHAR *huawei_demo_endorser_peer0Org1_hostName = "node-0.org1.bcs-epmmli.svc.cluster.local";

const BCHAR *huawei_demo_endorser_peer0Org2_url = "49.4.115.203:30606";
const BCHAR *huawei_demo_endorser_peer0Org2_hostName = "node-0.org2.bcs-epmmli.svc.cluster.local";

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
	wallet_config.accountPriKey_config.prikey_content.field_ptr = (BUINT8 *)huawei_client_demokey;
	wallet_config.accountPriKey_config.prikey_content.field_len = strlen(huawei_client_demokey) + 1; //length contain terminator

	//set cert context
	wallet_config.accountCertContent.length = strlen(huawei_client_democert) + 1;
	memcpy(wallet_config.accountCertContent.content, huawei_client_democert, wallet_config.accountCertContent.length);

	//set rootCA info
	// wallet_config.rootCaNumber = 3;
	// wallet_config.rootCaContent[0].length = strlen(fabric_ca1_democert) + 1;
	// memcpy(wallet_config.rootCaContent[0].content, fabric_ca1_democert, wallet_config.rootCaContent[0].length);
	// wallet_config.rootCaContent[1].length = strlen(fabric_ca2_democert) + 1;
	// memcpy(wallet_config.rootCaContent[1].content, fabric_ca2_democert, wallet_config.rootCaContent[1].length);
	// wallet_config.rootCaContent[2].length = strlen(fabric_ca3_democert) + 1;
	// memcpy(wallet_config.rootCaContent[2].content, fabric_ca3_democert, wallet_config.rootCaContent[2].length);

	//set endorser info
	// wallet_config.endorserNumber = 2;
	// memcpy(wallet_config.endorser[0].nodeUrl, fabric_demo_endorser1_url, strlen(fabric_demo_endorser1_url) + 1);
	// memcpy(wallet_config.endorser[0].hostName, fabric_demo_endorser1_hostName, strlen(fabric_demo_endorser1_hostName) + 1);
	// memcpy(wallet_config.endorser[1].nodeUrl, fabric_demo_endorser2_url, strlen(fabric_demo_endorser2_url) + 1);
	// memcpy(wallet_config.endorser[1].hostName, fabric_demo_endorser2_hostName, strlen(fabric_demo_endorser2_hostName) + 1);



	wallet_config.nodesCfg.endorserLayoutNum = 1;
	wallet_config.nodesCfg.layoutCfg = BoatMalloc(wallet_config.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(huawei_demo_endorser_peer0Org1_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(huawei_demo_endorser_peer0Org1_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,huawei_demo_endorser_peer0Org1_url,strlen(huawei_demo_endorser_peer0Org1_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,huawei_demo_endorser_peer0Org1_hostName,strlen(huawei_demo_endorser_peer0Org1_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(huawei_org1_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,huawei_org1_tlsCert,strlen(huawei_org1_tlsCert));

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(huawei_demo_endorser_peer0Org2_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(huawei_demo_endorser_peer0Org2_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl,huawei_demo_endorser_peer0Org2_url,strlen(huawei_demo_endorser_peer0Org2_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName,huawei_demo_endorser_peer0Org2_hostName,strlen(huawei_demo_endorser_peer0Org2_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(huawei_org2_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content,huawei_org2_tlsCert,strlen(huawei_org2_tlsCert));

	wallet_config.nodesCfg.orderCfg.endorserNumber = 1;
	wallet_config.nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(huawei_org1_tlsCert);
	memcpy(wallet_config.nodesCfg.orderCfg.tlsOrgCertContent.content,huawei_org1_tlsCert,strlen(huawei_org1_tlsCert));
	wallet_config.nodesCfg.orderCfg.endorser = BoatMalloc(wallet_config.nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(huawei_demo_endorser_peer0Org1_hostName)+1);
	wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(huawei_demo_endorser_peer0Org1_url)+1);
	memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl,huawei_demo_endorser_peer0Org1_url,strlen(huawei_demo_endorser_peer0Org1_url)+1);
	memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].hostName,huawei_demo_endorser_peer0Org1_hostName,strlen(huawei_demo_endorser_peer0Org1_hostName)+1);
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
	BOAT_RESULT       result  = BOAT_SUCCESS;
	BoatHlfabricTx    tx_ptr;
	
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
	result = BoatHlhuaweiTxInit(&tx_ptr, g_fabric_wallet_ptr, "default", "chaincode", NULL, "user-1.org1.bcs-epmmli.svc.cluster.local", "org1");
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

	/* step-5: set transaction 'invoke' command */
	result += BoatHlfabricTxSetArgs(&tx_ptr, "initMarble", "a", "1", NULL, NULL);
	result += BoatHlhuaweiTxSubmit(&tx_ptr); 
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		return -1;
	}
	/* step-6: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	result = BoatHlfabricTxSetArgs(&tx_ptr, "getMarble", "a", NULL);
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		return -1;
	}
	result = BoatHlhuaweiTxEvaluate(&tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxEvaluate() failed.");
		return -1;
	}
	
	/* step-7: fabric transaction structure Deinitialization */
	BoatHlfabricTxDeInit(&tx_ptr);
	
	/* step-8: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
