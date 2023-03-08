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
#include "boatkeypair.h"

/* MA510 Includes ---------------------------------------------------------*/
#include "qflog_utils.h"
#include "qapi_uart.h"
#include "qapi_timer.h"
#define BOAT_TEST
#define USE_ONETIME_WALLET

const BCHAR *fabric_client_demokey = "-----BEGIN PRIVATE KEY-----\n"
									 "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg/tXXUwnCl5b0Un9q\n"
									 "KjexHujDnmqxN2F4PkyUVjAlYBShRANCAAS84e5YxBu7VISBc5O8dVUaQ5EVV6KO\n"
									 "84YIaQi7J64Te5gz8nSHM5jMPLeDQv6r0YknVKE3gl4BRvIWtDbxI3tI\n"
									 "-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_democert = "-----BEGIN CERTIFICATE-----\n"
									  "MIICKzCCAdGgAwIBAgIRAJVvD4lgmBO3Ov7TYwDyIhMwCgYIKoZIzj0EAwIwczEL\n"
									  "MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
									  "cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh\n"
									  "Lm9yZzEuZXhhbXBsZS5jb20wHhcNMjIwNTA2MDMwMTAwWhcNMzIwNTAzMDMwMTAw\n"
									  "WjBsMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\n"
									  "U2FuIEZyYW5jaXNjbzEPMA0GA1UECxMGY2xpZW50MR8wHQYDVQQDDBZVc2VyMUBv\n"
									  "cmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvOHuWMQb\n"
									  "u1SEgXOTvHVVGkORFVeijvOGCGkIuyeuE3uYM/J0hzOYzDy3g0L+q9GJJ1ShN4Je\n"
									  "AUbyFrQ28SN7SKNNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYD\n"
									  "VR0jBCQwIoAgxfxkzp5erq3p4AH/lMgNe44f0BqYmWGuB4hXIkG540UwCgYIKoZI\n"
									  "zj0EAwIDSAAwRQIhAOz8ruXrkkdK2EsfGrmr7paTnBhjwvbSx0GQXZk0CpJCAiBu\n"
									  "wFbiCN1EwHqROzJgGMiatSOARqndHJpeX03w0qeSew==\n"
									  "-----END CERTIFICATE-----\n";

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)

const BCHAR *fabric_client_tls_prikey = "-----BEGIN PRIVATE KEY-----\n"
										"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgHFtvXWryUMKKUHKJ\n"
										"3ajXvSG1aPvZnCvC1m57KjlSh1qhRANCAARi2dKHWoicjhXDeX/oGgMzh1oimvJy\n"
										"TZXp4b3coeZfPcmxrWJEU4ZbVvYOKxOfp7LnYyBc8ct/KM37ultOD7zK\n"
										"-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_tls_cert = "-----BEGIN CERTIFICATE-----\n"
									  "MIICOzCCAeGgAwIBAgIQfuVAt3+dJm873ZX/by2dpTAKBggqhkjOPQQDAjB2MQsw\n"
									  "CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
									  "YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
									  "Y2Eub3JnMS5leGFtcGxlLmNvbTAeFw0yMjA1MDYwMzAxMDBaFw0zMjA1MDMwMzAx\n"
									  "MDBaMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
									  "Ew1TYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZVc2VyMUBvcmcxLmV4YW1wbGUuY29t\n"
									  "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEYtnSh1qInI4Vw3l/6BoDM4daIpry\n"
									  "ck2V6eG93KHmXz3Jsa1iRFOGW1b2DisTn6ey52MgXPHLfyjN+7pbTg+8yqNsMGow\n"
									  "DgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAM\n"
									  "BgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAIBMm4MmGOC/2t1UTvnVvf5St1fbxyMam\n"
									  "CoUPXIdX0BVNMAoGCCqGSM49BAMCA0gAMEUCIQDJTu1jVW3ZqOqXq7B4vPMHdN5s\n"
									  "FYaxtCge3bdCgkjcNQIgRs1rFgxlFq7bkloGBucO/H43sG4wX4MDK4V4IfQVUkY=\n"
									  "-----END CERTIFICATE-----\n";

#endif

const BCHAR *fabric_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
								   "MIICVzCCAf6gAwIBAgIRAOgZn4I4jGSdGbBD4m2VoVUwCgYIKoZIzj0EAwIwdjEL\n"
								   "MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
								   "cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHzAdBgNVBAMTFnRs\n"
								   "c2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMjIwNTA2MDMwMTAwWhcNMzIwNTAzMDMw\n"
								   "MTAwWjB2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UE\n"
								   "BxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0G\n"
								   "A1UEAxMWdGxzY2Eub3JnMS5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49\n"
								   "AwEHA0IABEtpvo/YvWG6ZlSmyE43LUNZZtgTg2B1LOKi2rJ+/6exBOiIHu0veF2D\n"
								   "e+bLinBR5FGGzGAk6TV7sfLsc4yUpACjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNV\n"
								   "HSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNV\n"
								   "HQ4EIgQgEybgyYY4L/a3VRO+dW9/lK3V9vHIxqYKhQ9ch1fQFU0wCgYIKoZIzj0E\n"
								   "AwIDRwAwRAIgHJayi2TyzJUd4ofYC1i+asm9ar5dOO0x4E+xCCUupIUCIFMJCg0Z\n"
								   "Yos13DpaCNoqNwAGJUcqLv4rtZPSPC4tkovr\n"
								   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
								   "MIICVjCCAf2gAwIBAgIQEnPt7b92LrvM7Y+lYDFvczAKBggqhkjOPQQDAjB2MQsw\n"
								   "CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
								   "YW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
								   "Y2Eub3JnMi5leGFtcGxlLmNvbTAeFw0yMjA1MDYwMzAxMDBaFw0zMjA1MDMwMzAx\n"
								   "MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
								   "Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcyLmV4YW1wbGUuY29tMR8wHQYD\n"
								   "VQQDExZ0bHNjYS5vcmcyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
								   "AQcDQgAEWu5iFCGmJg+M5Y0c3Tk6AuKA359x1z913vT3dIfsDVZs7CurWtyXnLe7\n"
								   "RB62rI2xhL4g1Q6cqyj0E6++/l1r/6NtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
								   "JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
								   "DgQiBCApfh1dtu1yeW1ES5r3J9M8yUQIdfukNV2OKARmvJCqujAKBggqhkjOPQQD\n"
								   "AgNHADBEAiBOWqG9hh8SPWLi6OIrHH+kjDXYcJTSgDUkKqP9nexBNwIgNA6A2EPw\n"
								   "0SKwSU3uC5c+3Tu+20ur3LZ4mjF7cFoXBSk=\n"
								   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_order_tlsCert = "-----BEGIN CERTIFICATE-----\n"
									"MIICQzCCAemgAwIBAgIQaQ52W1nx08Qn/Ib9Wzb4sDAKBggqhkjOPQQDAjBsMQsw\n"
									"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
									"YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xGjAYBgNVBAMTEXRsc2NhLmV4\n"
									"YW1wbGUuY29tMB4XDTIyMDUwNjAzMDEwMFoXDTMyMDUwMzAzMDEwMFowbDELMAkG\n"
									"A1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFu\n"
									"Y2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRowGAYDVQQDExF0bHNjYS5leGFt\n"
									"cGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABBfjiM3cLsoDt3tc5Nds\n"
									"cRPYccnZb3hj+J6e0/4C0ZAeZX6YjXh5gRmVvg36pITV7L1mlAy+9MusCts1UhiS\n"
									"MPmjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNVHSUEFjAUBggrBgEFBQcDAgYIKwYB\n"
									"BQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNVHQ4EIgQgsVjTxGcAStIyI7H59m+t\n"
									"Mgz3A0LvBl8+zw+Oa38ImA4wCgYIKoZIzj0EAwIDSAAwRQIhAN1+GWbcyN3i45rm\n"
									"9zr4ss8pD2Xgwe8t5/DuGGC/7KfaAiBwaH9saVyHOAvPMSNCRUsLSNkOgnOO7G0y\n"
									"Qy2dzpelIg==\n"
									"-----END CERTIFICATE-----\n";


const BCHAR *fabric_demo_order1_url = "172.20.255.199:7050";
const BCHAR *fabric_demo_order1_hostName = "orderer.example.com";

const BCHAR *fabric_demo_endorser_peer0Org1_url = "172.20.255.199:7051";
const BCHAR *fabric_demo_endorser_peer0Org1_hostName = "peer0.org1.example.com";
const BCHAR *fabric_demo_endorser_peer1Org1_url = "172.20.255.199:8051";
const BCHAR *fabric_demo_endorser_peer1Org1_hostName = "peer1.org1.example.com";

const BCHAR *fabric_demo_endorser_peer0Org2_url = "172.20.255.199:9051";
const BCHAR *fabric_demo_endorser_peer0Org2_hostName = "peer0.org2.example.com";
const BCHAR *fabric_demo_endorser_peer1Org2_url = "172.20.255.199:10051";
const BCHAR *fabric_demo_endorser_peer1Org2_hostName = "peer1.org2.example.com";


BoatHlfabricWallet *g_fabric_wallet_ptr;

BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;
// DiscoverRes discoverResult;


__BOATSTATIC BOAT_RESULT fabric_createkeypair()
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatKeypairPriKeyCtx_config keypair_config = {0};
	BUINT8 binFormatKey[32] = {0};

	(void)binFormatKey; // avoid warning

	/* keypair_config value assignment */
	keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
	keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
	keypair_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_demokey;
	keypair_config.prikey_content.field_len = strlen(fabric_client_demokey) + 1; // length contain terminator

	/* create ethereum keypair */
#if defined(USE_ONETIME_WALLET)
	result = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
	result = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
#endif
	if (result < BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
		return BOAT_ERROR_WALLET_CREATE_FAIL;
	}
	keypairIndex = result;
	return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT fabric_creatNetwork(void)
{
	BOAT_RESULT index;
	BoatHlfabricNetworkConfig networkConfig;

	// set cert context
	networkConfig.accountCertContent.length = strlen(fabric_client_democert) + 1;
	strcpy(networkConfig.accountCertContent.content, fabric_client_democert);

	networkConfig.nodesCfg.endorserLayoutNum = 1;
	networkConfig.nodesCfg.layoutCfg = BoatMalloc(networkConfig.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	networkConfig.nodesCfg.layoutCfg[0].endorserGroupNum = 2;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert);

	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_url) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_hostName) + 1);
	// memset(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org1_url) + 1);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org1_url);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, fabric_demo_endorser_peer0Org1_hostName);
	// memset(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, 0, strlen(fabric_demo_endorser_peer1Org1_url) + 1);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org1_url);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName, fabric_demo_endorser_peer1Org1_hostName);

	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, fabric_org2_tlsCert);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_url) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_hostName) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_url) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_hostName) + 1);
	// memset(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org2_url) + 1);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org2_url);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, fabric_demo_endorser_peer0Org2_hostName);
	// memset(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, 0, strlen(fabric_demo_endorser_peer1Org2_url) + 1);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org2_url);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName, fabric_demo_endorser_peer1Org2_hostName);

	// set orderer info
	networkConfig.nodesCfg.orderCfg.endorserNumber = 1;
	networkConfig.nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
	strcpy(networkConfig.nodesCfg.orderCfg.tlsOrgCertContent.content, fabric_order_tlsCert);
	networkConfig.nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig.nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	networkConfig.nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric_demo_order1_hostName) + 1);
	networkConfig.nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_order1_url) + 1);
	// memset(networkConfig.nodesCfg.orderCfg.endorser[0].nodeUrl, 0, strlen(fabric_demo_order1_url) + 1);
	strcpy(networkConfig.nodesCfg.orderCfg.endorser[0].nodeUrl, fabric_demo_order1_url);
	strcpy(networkConfig.nodesCfg.orderCfg.endorser[0].hostName, fabric_demo_order1_hostName);

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	networkConfig.accountClientTlsPrikey.value_len = strlen(fabric_client_tls_prikey);
	strcpy((BCHAR *)networkConfig.accountClientTlsPrikey.value, fabric_client_tls_prikey);
	networkConfig.accountClientTlsCert.length = strlen(fabric_client_tls_cert);
	strcpy((BCHAR *)networkConfig.accountClientTlsCert.content, fabric_client_tls_cert);
#endif
	/* create fabric wallet */
#if defined(USE_ONETIME_WALLET)
	index = BoatHlfabricNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
	index = BoatHlfabricNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
#else
	return BOAT_ERROR;
#endif
	if (index < 0)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "fabricWalletPrepare failed.");
		return BOAT_ERROR_WALLET_CREATE_FAIL;
	}

	networkIndex = index;
	fabricWalletConfigFree(networkConfig);
	return index;
}

int boat_fabric_entry()
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHlfabricTx tx_ptr;
	boat_try_declare;
	/* step-1: Boat SDK initialization */
	BoatIotSdkInit();

	/* step-2: prepare wallet */
	result = fabric_createkeypair();
	if (result < BOAT_ERROR)
	{
		boat_throw(result, fabric_key_network_catch);
	}
	result = fabric_creatNetwork();
	if (result < BOAT_ERROR)
	{
		boat_throw(result, fabric_key_network_catch);
	}
	g_fabric_wallet_ptr = BoatHlfabricWalletInit(keypairIndex, networkIndex);
	if (g_fabric_wallet_ptr == NULL)
	{

		boat_throw(result, fabric_key_network_catch);
	}

	/* step-3: fabric transaction structure initialization */
	result = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxInit failed.");
		boat_throw(result, fabric_demo_catch);
	}
	/* step-4: set transaction timestamp */
#if defined(__unix__) || defined(__unix) || defined(unix)
	struct timespec txTimestamp;
	clock_gettime(CLOCK_REALTIME, &txTimestamp);
	result = BoatHlfabricTxSetTimestamp(&tx_ptr, txTimestamp.tv_sec, txTimestamp.tv_nsec);
#else
	long int timesec = 0;
	time(&timesec);
	result = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
#endif
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetTimestamp failed.");
		boat_throw(result, fabric_demo_catch);
	}

	/* step-5: set transaction 'invoke' command */
	result += BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
	result += BoatHlfabricTxSubmit(&tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		boat_throw(result, fabric_demo_catch);
	}
	/* step-6: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	result = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		boat_throw(result, fabric_demo_catch);
	}
	result = BoatHlfabricTxEvaluate(&tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxEvaluate() failed.");
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
	boat_catch(fabric_key_network_catch)
	{
		BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
	}
	/* step-8: Boat SDK Deinitialization */
	BoatIotSdkDeInit();
	// fabricWalletConfigFree(networkConfig);

	return result;
}




int fibocom_task_entry(void)
{
  int ret=0;
  qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
  
  debug_uart_init(&debug_uart_context_D);

  ret = dam_byte_pool_init();
  if(ret != TX_SUCCESS)
  {
    BoatLog(BOAT_LOG_CRITICAL,"Create DAM byte pool fail!\n");
    return ret;
  }

  /* BoAT demo task */
#ifdef BOAT_TEST
    boat_fabric_entry();
#endif

   return 0;
}
