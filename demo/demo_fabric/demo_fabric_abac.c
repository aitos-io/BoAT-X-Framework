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

const BCHAR *fabric_client_demokey = "-----BEGIN PRIVATE KEY-----\n"
"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgExGSgCJJjJNY+rm5\n"
"PzSIdT9T0yq5HeMZfLWGIv4MMgmhRANCAAQjz9vBAzGon94IO318sSEsLPzv/hUG\n"
"xpmVtgyP9M4kZiofXhHRFpd55yv1oSZVJ1qayXF72dVa3N1Efq3L6YPr\n"
									 "-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_democert = "-----BEGIN CERTIFICATE-----\n"
"MIICKzCCAdGgAwIBAgIRALjTWyXqQ91VfNkU3M4twDcwCgYIKoZIzj0EAwIwczEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh\n"
"Lm9yZzEuZXhhbXBsZS5jb20wHhcNMjEwODI0MDI0MDAwWhcNMzEwODIyMDI0MDAw\n"
"WjBsMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\n"
"U2FuIEZyYW5jaXNjbzEPMA0GA1UECxMGY2xpZW50MR8wHQYDVQQDDBZVc2VyMUBv\n"
"cmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEI8/bwQMx\n"
"qJ/eCDt9fLEhLCz87/4VBsaZlbYMj/TOJGYqH14R0RaXeecr9aEmVSdamslxe9nV\n"
"WtzdRH6ty+mD66NNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYD\n"
"VR0jBCQwIoAgPimayAehE+5QzYJ97IrO6+0If9ytCf9CmoQERy15+PYwCgYIKoZI\n"
"zj0EAwIDSAAwRQIhAJbvy8Xf9LR2Ms4xVqpBrokvumKuqV1Er9zsH9xU4EtlAiBi\n"
"7/vhTB1ciEDv1xAl/MGeCv5Fw2/kTuhWaqnweoljQA==\n"
									  "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICWDCCAf6gAwIBAgIRAMLHCXEQpMlZvIgUacdL1f8wCgYIKoZIzj0EAwIwdjEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHzAdBgNVBAMTFnRs\n"
"c2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMjEwODI0MDI0MDAwWhcNMzEwODIyMDI0\n"
"MDAwWjB2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UE\n"
"BxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0G\n"
"A1UEAxMWdGxzY2Eub3JnMS5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49\n"
"AwEHA0IABCOyMlRJgG6eXUEYlgiclBDQNMlhN0jHuc+ryPpZwEOdOY7SxOZwQlf3\n"
"vPUP2shVBWzb6m6LQcCj3pb/KK8SaY2jbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNV\n"
"HSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNV\n"
"HQ4EIgQg49uDUbku1O1WGsTBuGxDhXXqnzM1qquJ60hQzJdDkCMwCgYIKoZIzj0E\n"
"AwIDSAAwRQIhALP1edBV25XT0JbahsT9DmFHcG7jj4JnzW+T7P1lWSJiAiBOt1IY\n"
"lzlTjaqaqtRYCUNOLH0awrZh4eegEU/GqsloGA==\n"
								   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICVzCCAf2gAwIBAgIQB8uVnnAtHwXeE/cEN0r62jAKBggqhkjOPQQDAjB2MQsw\n"
"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
"YW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
"Y2Eub3JnMi5leGFtcGxlLmNvbTAeFw0yMTA4MDYwNjAzMDBaFw0zMTA4MDQwNjAz\n"
"MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
"Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcyLmV4YW1wbGUuY29tMR8wHQYD\n"
"VQQDExZ0bHNjYS5vcmcyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
"AQcDQgAEhu/t0amzDHLt3p3D4mr7eO4wZLp98BKT6VgpswuMg4Sp/324pkT5Pb6c\n"
"TogzGI3IZ5xDiYGrJ2lreaQ+bqOqKqNtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
"DgQiBCA/Ib/XHEMn9g9ZFgulJOSXI7rQLwYYyi2GfGd/7ixi2TAKBggqhkjOPQQD\n"
"AgNIADBFAiEAn0y9XaOS4qV8KWcZB/IkJvPiEIZyYwLhew6BD7BUAhUCIA5NiTi7\n"
"DncjS2X0syhZKmZv8hIIr/CAU6Imz2VDnaHF\n"
								   "-----END CERTIFICATE-----\n";
const BCHAR *fabric_org3_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICVzCCAf2gAwIBAgIQTm3owgqoEpuIwtOdOX/WOTAKBggqhkjOPQQDAjB2MQsw\n"
"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
"YW5jaXNjbzEZMBcGA1UEChMQb3JnMy5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
"Y2Eub3JnMy5leGFtcGxlLmNvbTAeFw0yMTA4MDYwNjAzMDBaFw0zMTA4MDQwNjAz\n"
"MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
"Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmczLmV4YW1wbGUuY29tMR8wHQYD\n"
"VQQDExZ0bHNjYS5vcmczLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
"AQcDQgAEAo/E152lnltILiB93p+maqTuMpTzxaHPP/en56CFlXUSGCIZDxGL02PM\n"
"Ct5baXsuMqw/BZX5IwWIGjtV6gi3r6NtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
"DgQiBCAtUULYJM0SesyrVgIctdaXK9NoZSORlRY1j/qlO9e9CjAKBggqhkjOPQQD\n"
"AgNIADBFAiEA+j2On3m869r/2p+LP5hAvKKnIaaQKWF7x/8dHVh7xRkCIHA4QqUy\n"
"Ei989Jdk9ePhXHO3+4gEuqOicO/StoI4MLST\n"
								   "-----END CERTIFICATE-----\n";	
const BCHAR *fabric_org4_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICVzCCAf2gAwIBAgIQJn/Pwk6yDecJisJDb8MA4jAKBggqhkjOPQQDAjB2MQsw\n"
"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
"YW5jaXNjbzEZMBcGA1UEChMQb3JnNC5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
"Y2Eub3JnNC5leGFtcGxlLmNvbTAeFw0yMTA4MDYwNjAzMDBaFw0zMTA4MDQwNjAz\n"
"MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
"Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmc0LmV4YW1wbGUuY29tMR8wHQYD\n"
"VQQDExZ0bHNjYS5vcmc0LmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
"AQcDQgAE9oHFN++C3aTuQ1jvcHuMThFIUCxnQV+Ifiqigk8BmtmEUSpz2RG+EAT4\n"
"g7rmkhGDX9E7kvul6f8df4ECxhA1r6NtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
"DgQiBCDb+6LQGU4cWmBRC4eNK0btI7VO2e23WVhnl0MhpmHv9zAKBggqhkjOPQQD\n"
"AgNIADBFAiEAijpIZ6wITdf6QlSOngxhJ3cdJrUxYqcfd3b37Vg0E4ICIG6nV5XT\n"
"t806m0r0UE9w4HWQaI10LKHIVGgPeTKQ2H5Q\n"
								   "-----END CERTIFICATE-----\n";
const BCHAR *fabric_org5_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICVjCCAf2gAwIBAgIQaerg8MYuK2AMniGrFsgacDAKBggqhkjOPQQDAjB2MQsw\n"
"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
"YW5jaXNjbzEZMBcGA1UEChMQb3JnNS5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
"Y2Eub3JnNS5leGFtcGxlLmNvbTAeFw0yMTA4MDYwNjAzMDBaFw0zMTA4MDQwNjAz\n"
"MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
"Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmc1LmV4YW1wbGUuY29tMR8wHQYD\n"
"VQQDExZ0bHNjYS5vcmc1LmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
"AQcDQgAEGwaGq1V2eL6hdJnLrB5hR0wuAGzfuhJ2GmYo4LGNQEqHX1FQ75y/FsyA\n"
"ISGrnhU36TH4ZN0Nj4ojfxcKIhm7PKNtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
"DgQiBCDk8/soRYRNFZ/CxB20/E9N+mrCIrwX01JdHdozbIfO9zAKBggqhkjOPQQD\n"
"AgNHADBEAiBSbnHFIwu3JZVlCDaRK86Twr8ueqLtUGAqtUU4iad5NgIgCU60BfLi\n"
"EYAfPQBFG6GKCD6iTPl08iewQdI6p+uevJk=\n"
								   "-----END CERTIFICATE-----\n";								   													
const BCHAR *fabric_order_tlsCert = "-----BEGIN CERTIFICATE-----\n"
"MIICRDCCAeqgAwIBAgIRAP/f8u4AxR/idbaatMbOA/wwCgYIKoZIzj0EAwIwbDEL\n"
"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
"cmFuY2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRowGAYDVQQDExF0bHNjYS5l\n"
"eGFtcGxlLmNvbTAeFw0yMTA4MDYwNjAzMDBaFw0zMTA4MDQwNjAzMDBaMGwxCzAJ\n"
"BgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4gRnJh\n"
"bmNpc2NvMRQwEgYDVQQKEwtleGFtcGxlLmNvbTEaMBgGA1UEAxMRdGxzY2EuZXhh\n"
"bXBsZS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAST3UZswR3f9F5Ll7p3\n"
"1mz0Fw32bNOpyep+MvSsttZZc8zuGN60wlLOJuT34AuvZZDTgv+N/diu5V9pGAjO\n"
"Yyouo20wazAOBgNVHQ8BAf8EBAMCAaYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsG\n"
"AQUFBwMBMA8GA1UdEwEB/wQFMAMBAf8wKQYDVR0OBCIEIMd0MLKv2pTct7IxlIsH\n"
"Adj2fl9KJFrXE+RCBsUIftfPMAoGCCqGSM49BAMCA0gAMEUCIQD6wZ2r1zV2gqV0\n"
"t2Q95/webUnNySZP4jEJ5G65Tw2WdwIgXOsy/6WFAeyqOuZvIuyqE5K504Oo+yys\n"
"Tj2rBd786Lw=\n"
								   "-----END CERTIFICATE-----\n";	


// const BCHAR *fabric_demo_endorser1_url = "139.224.234.74:7051";
const BCHAR * fabric_demo_endorser1_url      = "192.168.132.190:7051";
const BCHAR *fabric_demo_endorser1_hostName = "peer0.org1.example.com";
const BCHAR *fabric_demo_endorser2_url = "192.168.132.190:9051";
// const BCHAR *fabric_demo_endorser2_url = "139.224.234.74:7051";
const BCHAR *fabric_demo_endorser2_hostName = "peer0.org2.example.com";
const BCHAR *fabric_demo_order1_url = "192.168.132.190:7050";
// const BCHAR *fabric_demo_order1_url = "139.224.234.74:7050";
const BCHAR *fabric_demo_order1_hostName = "orderer.example.com";

const BCHAR *fabric_demo_endorser_peer0Org1_url = "192.168.132.190:7051";
const BCHAR *fabric_demo_endorser_peer0Org1_hostName = "peer0.org1.example.com";
const BCHAR *fabric_demo_endorser_peer1Org1_url = "192.168.132.190:8051";
const BCHAR *fabric_demo_endorser_peer1Org1_hostName = "peer1.org1.example.com";

const BCHAR *fabric_demo_endorser_peer0Org2_url = "192.168.132.190:9051";
const BCHAR *fabric_demo_endorser_peer0Org2_hostName = "peer0.org2.example.com";
const BCHAR *fabric_demo_endorser_peer1Org2_url = "192.168.132.190:10051";
const BCHAR *fabric_demo_endorser_peer1Org2_hostName = "peer1.org2.example.com";

const BCHAR *fabric_demo_endorser_peer0Org3_url = "192.168.132.190:6051";
const BCHAR *fabric_demo_endorser_peer0Org3_hostName = "peer0.org3.example.com";
const BCHAR *fabric_demo_endorser_peer1Org3_url = "192.168.132.190:11051";
const BCHAR *fabric_demo_endorser_peer1Org3_hostName = "peer1.org3.example.com";

const BCHAR *fabric_demo_endorser_peer0Org4_url = "192.168.132.190:6061";
const BCHAR *fabric_demo_endorser_peer0Org4_hostName = "peer0.org4.example.com";
const BCHAR *fabric_demo_endorser_peer1Org4_url = "192.168.132.190:11061";
const BCHAR *fabric_demo_endorser_peer1Org4_hostName = "peer1.org4.example.com";

const BCHAR *fabric_demo_endorser_peer0Org5_url = "192.168.132.190:6071";
const BCHAR *fabric_demo_endorser_peer0Org5_hostName = "peer0.org5.example.com";
const BCHAR *fabric_demo_endorser_peer1Org5_url = "192.168.132.190:11071";
const BCHAR *fabric_demo_endorser_peer1Org5_hostName = "peer1.org5.example.com";

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

#if(DISCOVERY_PEER_QUERY != 1 )

	wallet_config.nodesCfg.endorserLayoutNum = 2;
	wallet_config.nodesCfg.layoutCfg = BoatMalloc(wallet_config.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum = 4;
	wallet_config.nodesCfg.layoutCfg[1].endorserGroupNum = 4;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg = BoatMalloc(wallet_config.nodesCfg.layoutCfg[1].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
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

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].tlsOrgCertContent.length = strlen(fabric_org3_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].tlsOrgCertContent.content,fabric_org3_tlsCert,strlen(fabric_org3_tlsCert));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org3_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org3_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org3_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org3_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org3_url,strlen(fabric_demo_endorser_peer0Org3_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser[0].hostName,fabric_demo_endorser_peer0Org3_hostName,strlen(fabric_demo_endorser_peer0Org3_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser[1].nodeUrl,fabric_demo_endorser_peer1Org3_url,strlen(fabric_demo_endorser_peer1Org3_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[2].endorser[1].hostName,fabric_demo_endorser_peer1Org3_hostName,strlen(fabric_demo_endorser_peer1Org3_hostName)+1);

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].tlsOrgCertContent.length = strlen(fabric_org5_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].tlsOrgCertContent.content,fabric_org5_tlsCert,strlen(fabric_org5_tlsCert));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org5_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org5_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org5_url)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org5_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org5_url,strlen(fabric_demo_endorser_peer0Org5_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser[0].hostName,fabric_demo_endorser_peer0Org5_hostName,strlen(fabric_demo_endorser_peer0Org5_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser[1].nodeUrl,fabric_demo_endorser_peer1Org5_url,strlen(fabric_demo_endorser_peer1Org5_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[3].endorser[1].hostName,fabric_demo_endorser_peer1Org5_hostName,strlen(fabric_demo_endorser_peer1Org5_hostName)+1);

	wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].tlsOrgCertContent.content,fabric_org1_tlsCert,strlen(fabric_org1_tlsCert));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_url)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_hostName)+1);	
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org1_url,strlen(fabric_demo_endorser_peer0Org1_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser[0].hostName,fabric_demo_endorser_peer0Org1_hostName,strlen(fabric_demo_endorser_peer0Org1_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser[1].nodeUrl,fabric_demo_endorser_peer1Org1_url,strlen(fabric_demo_endorser_peer1Org1_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[0].endorser[1].hostName,fabric_demo_endorser_peer1Org1_hostName,strlen(fabric_demo_endorser_peer1Org1_hostName)+1);

	wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].tlsOrgCertContent.content,fabric_org2_tlsCert,strlen(fabric_org2_tlsCert));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_url)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_url)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_hostName)+1);		
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org2_url,strlen(fabric_demo_endorser_peer0Org2_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser[0].hostName,fabric_demo_endorser_peer0Org2_hostName,strlen(fabric_demo_endorser_peer0Org2_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser[1].nodeUrl,fabric_demo_endorser_peer1Org2_url,strlen(fabric_demo_endorser_peer1Org2_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[1].endorser[1].hostName,fabric_demo_endorser_peer1Org2_hostName,strlen(fabric_demo_endorser_peer1Org2_hostName)+1);

	wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].tlsOrgCertContent.length = strlen(fabric_org4_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].tlsOrgCertContent.content,fabric_org4_tlsCert,strlen(fabric_org4_tlsCert));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org4_url)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org4_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org4_url)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org4_hostName)+1);	
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org4_url,strlen(fabric_demo_endorser_peer0Org4_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser[0].hostName,fabric_demo_endorser_peer0Org4_hostName,strlen(fabric_demo_endorser_peer0Org4_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser[1].nodeUrl,fabric_demo_endorser_peer1Org4_url,strlen(fabric_demo_endorser_peer1Org4_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[2].endorser[1].hostName,fabric_demo_endorser_peer1Org4_hostName,strlen(fabric_demo_endorser_peer1Org4_hostName)+1);

	wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].tlsOrgCertContent.length = strlen(fabric_org5_tlsCert);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].tlsOrgCertContent.content,fabric_org5_tlsCert,strlen(fabric_org5_tlsCert));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org5_url)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org5_hostName)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org5_url)+1);
	wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org5_hostName)+1);		
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser[0].nodeUrl,fabric_demo_endorser_peer0Org5_url,strlen(fabric_demo_endorser_peer0Org5_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser[0].hostName,fabric_demo_endorser_peer0Org5_hostName,strlen(fabric_demo_endorser_peer0Org5_hostName)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser[1].nodeUrl,fabric_demo_endorser_peer1Org5_url,strlen(fabric_demo_endorser_peer1Org5_url)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[1].groupCfg[3].endorser[1].hostName,fabric_demo_endorser_peer1Org5_hostName,strlen(fabric_demo_endorser_peer1Org5_hostName)+1);

	
	wallet_config.nodesCfg.orderCfg.endorserNumber = 1;
	wallet_config.nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
	memcpy(wallet_config.nodesCfg.orderCfg.tlsOrgCertContent.content,fabric_order_tlsCert,strlen(fabric_order_tlsCert));
	wallet_config.nodesCfg.orderCfg.endorser = BoatMalloc(wallet_config.nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric_demo_order1_hostName)+1);
	wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_order1_url)+1);
	memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl,fabric_demo_order1_url,strlen(fabric_demo_order1_url)+1);
	memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].hostName,fabric_demo_order1_hostName,strlen(fabric_demo_order1_hostName)+1);
	//set orderer info
	// wallet_config.ordererNumber = 1;
	// memcpy(wallet_config.orderer[0].nodeUrl, fabric_demo_order1_url, strlen(fabric_demo_order1_url) + 1);
	// memcpy(wallet_config.orderer[0].hostName, fabric_demo_order1_hostName, strlen(fabric_demo_order1_hostName) + 1);
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
	result = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP",NULL,NULL);
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
		result += BoatHlfabricDiscoverSubmit(&tx_ptr,wallet_config.nodesCfg);
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
						 tx_ptr.endorserResponse.http2Res,
						 tx_ptr.endorserResponse.httpResLen);
	
	/* step-7: fabric transaction structure Deinitialization */
	BoatHlfabricTxDeInit(&tx_ptr);
	
	/* step-8: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
