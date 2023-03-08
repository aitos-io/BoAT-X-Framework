/**
****************************************************************************************
* @FilePath: boatFabricdemo.c
* @Author: aitos
* @Date: 2023-02-08 11:21:00
* @LastEditors:
* @LastEditTime: 2023-02-08 11:21:00
* @Descripttion:
****************************************************************************************
*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// #include "api_hlfabric.h"

#include "boatiotsdk.h"
#include "boatlog.h"

#include "cm_os.h"
#include "cm_atchart.h"
#include "cm_net_register.h"

static int net_ready = 0;
static unsigned char virt_at_buffer[2048];
static osMutexId_t vir_at_mutex;
static osEventFlagsId_t at_task_flag;

const BCHAR *fabric_client_demokey = "-----BEGIN PRIVATE KEY-----\n"
									 "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgQelGsrrI1bSHfhn7\n"
									 "cE8eZhW0vcN/zxwQSSvGGjC//XWhRANCAAQF5s7NQ68grY1w8QqWst8nAYhGKIZm\n"
									 "lWAuUJEex+bm1kZmxfWjGTRTTxBxiW0nEHOoKh3180brsVtF4O/SqYRm\n"
									 "-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_democert = "-----BEGIN CERTIFICATE-----\n"
									  "MIICKzCCAdGgAwIBAgIRALK+tyjVhClDWyt/ggWKgEQwCgYIKoZIzj0EAwIwczEL\n"
									  "MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
									  "cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh\n"
									  "Lm9yZzEuZXhhbXBsZS5jb20wHhcNMjMwMjAyMDIzMjAwWhcNMzMwMTMwMDIzMjAw\n"
									  "WjBsMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\n"
									  "U2FuIEZyYW5jaXNjbzEPMA0GA1UECxMGY2xpZW50MR8wHQYDVQQDDBZVc2VyMUBv\n"
									  "cmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEBebOzUOv\n"
									  "IK2NcPEKlrLfJwGIRiiGZpVgLlCRHsfm5tZGZsX1oxk0U08QcYltJxBzqCod9fNG\n"
									  "67FbReDv0qmEZqNNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYD\n"
									  "VR0jBCQwIoAgLlMBAU/4icXPJc4Nj7zn/7wWiuLPlUHmBVZ/xvPochEwCgYIKoZI\n"
									  "zj0EAwIDSAAwRQIhAJy4GMtVyuLUcEBHNXIJUT5p4VCd8qDHBOuT24oi3/k7AiB2\n"
									  "Us4Uu9EgwXe5ZtogeUvY2t+T1clQkgD6O9QqmcYEVA==\n"
									  "-----END CERTIFICATE-----\n";

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)

const BCHAR *fabric_client_tls_prikey = "-----BEGIN PRIVATE KEY-----\n"
										"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgJi3O2cMWUMaMqT1P\n"
										"PnZT+QW3o2UMsoZSRC4kRlpprvOhRANCAAQDc5l684LtyVJgyTshzXGASAVhHOLq\n"
										"HGQmvm/kuJhEB7drImUunhSr1or8+7KdANPoHF9k+yzYNBCb5rYc/rh2\n"
										"-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_tls_cert = "-----BEGIN CERTIFICATE-----\n"
									  "MIICOzCCAeGgAwIBAgIQDO1oaMNMWjFlNY0PJ1RyQTAKBggqhkjOPQQDAjB2MQsw\n"
									  "CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
									  "YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
									  "Y2Eub3JnMS5leGFtcGxlLmNvbTAeFw0yMzAyMDIwMjMyMDBaFw0zMzAxMzAwMjMy\n"
									  "MDBaMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
									  "Ew1TYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZVc2VyMUBvcmcxLmV4YW1wbGUuY29t\n"
									  "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEA3OZevOC7clSYMk7Ic1xgEgFYRzi\n"
									  "6hxkJr5v5LiYRAe3ayJlLp4Uq9aK/PuynQDT6BxfZPss2DQQm+a2HP64dqNsMGow\n"
									  "DgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAM\n"
									  "BgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAIDL1YKxMZjB/zgNqXKzD/lQvL2B1VkJJ\n"
									  "fLTWbAniSmArMAoGCCqGSM49BAMCA0gAMEUCIQCf16/NCH0BX9Ka0VmUII8aObS/\n"
									  "AOUE1+5zN8O7UIE7PgIgBsx3KOhRlg05ThrbtuESEeukgMekruWo2XdfTsI4DVM=\n"
									  "-----END CERTIFICATE-----\n";

#endif

const BCHAR *fabric_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
								   "MIICWDCCAf6gAwIBAgIRAIq8VU7QVaMbCmO4XtlPcGwwCgYIKoZIzj0EAwIwdjEL\n"
								   "MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
								   "cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHzAdBgNVBAMTFnRs\n"
								   "c2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMjMwMjAyMDIzMjAwWhcNMzMwMTMwMDIz\n"
								   "MjAwWjB2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UE\n"
								   "BxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0G\n"
								   "A1UEAxMWdGxzY2Eub3JnMS5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49\n"
								   "AwEHA0IABBsBufHeiF3lhn73kMNSb4OgPtAvkOhF6Km46DGjJiJhkeFpB7Lz0/AO\n"
								   "1eBX4vVXyPXmGfz/r/1jW+t9kVPsPdKjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNV\n"
								   "HSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNV\n"
								   "HQ4EIgQgMvVgrExmMH/OA2pcrMP+VC8vYHVWQkl8tNZsCeJKYCswCgYIKoZIzj0E\n"
								   "AwIDSAAwRQIhAPRrhZgY8AZwXzLXifFhw6DfLB1kYTPykKJCZDAkl8EsAiBrZLND\n"
								   "i/FJpYxm3NdeHxmpe98bTccpksZovAd9WqWypA==\n"
								   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
								   "MIICVzCCAf2gAwIBAgIQD7y+U4BnohOSCBonGS+jOTAKBggqhkjOPQQDAjB2MQsw\n"
								   "CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
								   "YW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
								   "Y2Eub3JnMi5leGFtcGxlLmNvbTAeFw0yMzAyMDIwMjMyMDBaFw0zMzAxMzAwMjMy\n"
								   "MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
								   "Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcyLmV4YW1wbGUuY29tMR8wHQYD\n"
								   "VQQDExZ0bHNjYS5vcmcyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
								   "AQcDQgAEtG6QUB0YbZuDSJHkAa+KSnVtAVvO3WZBExabZShKxcA2yyn0zuXI+9uQ\n"
								   "1SZdFMEZw9mCL2+MDyagvPOWeCTnlKNtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
								   "JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
								   "DgQiBCBnrIPU8XwE+SVbbV/wTZolaX1PBv5vXAtonEJi6wHYEjAKBggqhkjOPQQD\n"
								   "AgNIADBFAiEAxxDBxlSt6fHzZhx0Xj/LW2UrGNY17IPBNxd27W8qPrcCIA7LBgC+\n"
								   "6Kkh21m2a1TTnQ8FoSgaTv6D8/WHcopSL3CG\n"
								   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_order_tlsCert = "-----BEGIN CERTIFICATE-----\n"
									"MIICQzCCAemgAwIBAgIQeqDTsZGBmA/T0auvq8uZ4zAKBggqhkjOPQQDAjBsMQsw\n"
									"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
									"YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xGjAYBgNVBAMTEXRsc2NhLmV4\n"
									"YW1wbGUuY29tMB4XDTIzMDIwMjAyMzIwMFoXDTMzMDEzMDAyMzIwMFowbDELMAkG\n"
									"A1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFu\n"
									"Y2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRowGAYDVQQDExF0bHNjYS5leGFt\n"
									"cGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABMiaV0Nzr2/N83pHiPdH\n"
									"6jOp1HftD+vGPfSx9fXzjH4ik3Wl0uRzBK+nRM6zbblFe9Bp6Ndr1MoOIPjzEgFh\n"
									"YCCjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNVHSUEFjAUBggrBgEFBQcDAgYIKwYB\n"
									"BQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNVHQ4EIgQgwrK+n/KzIvMIvlMbDzKV\n"
									"sB2eOXdJ/Y5Qqu/cEiZbY9UwCgYIKoZIzj0EAwIDSAAwRQIhAL9+6AuCgHwXC1f9\n"
									"Um9uM4yvBleBzHKmxswP0CivSbdpAiBQkm/iP66IYMElwJN8UIacoQY/yAuTTiBW\n"
									"8M0cQoS49Q==\n"
									"-----END CERTIFICATE-----\n";

const BCHAR *fabric_demo_order1_url = "127.0.0.1:7050";
const BCHAR *fabric_demo_order1_hostName = "orderer.example.com";

const BCHAR *fabric_demo_endorser_peer0Org1_url = "127.0.0.1:7051";
const BCHAR *fabric_demo_endorser_peer0Org1_hostName = "peer0.org1.example.com";
const BCHAR *fabric_demo_endorser_peer1Org1_url = "127.0.0.1:8051";
const BCHAR *fabric_demo_endorser_peer1Org1_hostName = "peer1.org1.example.com";

const BCHAR *fabric_demo_endorser_peer0Org2_url = "127.0.0.1:9051";
const BCHAR *fabric_demo_endorser_peer0Org2_hostName = "peer0.org2.example.com";
const BCHAR *fabric_demo_endorser_peer1Org2_url = "127.0.0.1:10051";
const BCHAR *fabric_demo_endorser_peer1Org2_hostName = "peer1.org2.example.com";

BoatHlfabricWallet *g_fabric_wallet_ptr;

BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;

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
	result = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);

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

	index = BoatHlfabricNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);

	if (index < 0)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "fabricWalletPrepare failed.");
		return BOAT_ERROR_WALLET_CREATE_FAIL;
	}

	networkIndex = index;
	fabricWalletConfigFree(networkConfig);
	return index;
}

static const osMutexAttr_t at_Mutex_attr =
	{
		"atThreadMutex",					   // human readable mutex name
		osMutexRecursive | osMutexPrioInherit, // attr_bits
		0,									   // memory for control block
		0U									   // size for control block
};

static void cm_sys_at_callback(void *param)
{
	osEventFlagsSet(at_task_flag, 0x00000001U);
}

/**
 *  \brief 虚拟AT指令消息通知回调函数
 *
 *  \param [in] pcu8NotifyLine URC消息数据接收指针
 *  \param [in] u32LineLen URC消息长度
 *  \param [in] eNotifyType URC消息类型
 *  \return 空
 *
 *  \details More details
 */
static void cm_test_at_notify(const uint8_t *pcu8NotifyLine, uint32_t u32LineLen, cm_atchart_notify_e eNotifyType)
{
	if (eNotifyType == CM_ATCHART_RSP_EMPTY_LINE)
		return;
	cm_net_register_notify(pcu8NotifyLine, u32LineLen, eNotifyType);
}

/**
 *  \brief  虚拟AT指令任务
 *
 *  \param [in]
 *  \return NULL
 *
 *  \details More details
 */
static void virture_at_task(void)
{
	while (1)
	{
		osEventFlagsWait(at_task_flag, 0x00000001U, osFlagsWaitAny, osWaitForever);
		memset(virt_at_buffer, 0, 2048);
		int virt_at_len = cm_vir_at_get(virt_at_buffer, 2048);
		virt_at_buffer[virt_at_len] = 0;
		cm_sys_log("%s", virt_at_buffer);
		cm_atchart_on_recv(virt_at_buffer, virt_at_len);
	}
}

static void boat_test_vir_at_init(void)
{
	osThreadAttr_t virat_task_attr = {0};
	at_task_flag = osEventFlagsNew(0);
	if (at_task_flag == 0)
	{
		return;
	}

	virat_task_attr.name = "virat_task";
	virat_task_attr.stack_size = 2048;

	vir_at_mutex = osMutexNew(&at_Mutex_attr);
	// osThreadDef(OC_AT_Task, virture_at_task, osPriorityNormal, 0, 2048);
	osThreadId_t OC_AT_TaskHandle = osThreadNew(virture_at_task, 0, &virat_task_attr);

	cm_vir_at_init(cm_sys_at_callback);
	cm_atchart_init();
	cm_atchart_open();
	cm_atchart_set_notify_cb(cm_test_at_notify);
}

/**
 *  \brief 等待网络注册成功
 *
 *  \return 0：成功 -1：超时失败
 *
 *  \details More details
 */
static int cm_test_wait_for_network(void)
{
	// 等待时间
	int retry_count = 50 * 60;
	// 将成功标志设为0
	net_ready = 0;
	// 手动触发网络注册模块更新状态，确保获取到最新网络状态
	cm_net_register_update_status();
	// 等待一段时间，当超过时间，network_state_change回调函数仍没有返回CM_NETWORK_STATE_PDP_READY，说明网络注册可能遇到问题。
	while ((net_ready == 0) && (retry_count > 0))
	{
		osDelay(1);
		retry_count--;
	}
	if (net_ready != 0) // 网络注册成功
	{
		cm_sys_log("boat network ready\n");
		return 0;
	}
	else // 网络注册尚未成功
	{
		cm_sys_log("boat network not ready\n");
		return -1;
	}
}
// 网络自注册接口回调函数，通知网络状态变化
static void network_state_change(cm_network_state_e state)
{
	cm_sys_log("[boat]:state:%d\n", state);
	switch (state)
	{
	// 自动注网模块初始化
	case CM_NETWORK_STATE_INIT:
		cm_sys_log("[boat]:Module init\n");
		break;
	// 无SIM卡
	case CM_NETWORK_STATE_NO_SIM:
		cm_sys_log("[boat]:No SIM\n");
		break;
	// SIM卡识别成功
	case CM_NETWORK_STATE_SIM_READY:
		cm_sys_log("[boat]:SIM ready\n");
		break;
	// 正在附着网络，CEREG:0,2
	case CM_NETWORK_STATE_ATTACHING:
		cm_sys_log("[boat]:Attaching\n");
		break;
	// 网络附着失败，CEREG:0,0
	case CM_NETWORK_STATE_ATTACH_FAIL:
		cm_sys_log("[boat]:Attach fail\n");
		break;
	// 网络附着成功，CEREG:0,1
	case CM_NETWORK_STATE_ATTACH_SUCCESS:
		cm_sys_log("[boat]:Attach success\n");
		break;
	// 正在激活PDP
	case CM_NETWORK_STATE_PDP_ACTIVATING:
		cm_sys_log("[boat]:PDP activating\n");
		break;
	// PDP激活失败
	case CM_NETWORK_STATE_PDP_NOT_READY:
		cm_sys_log("[boat]:PDP not ready\n");
		break;
	// PDP激活成功
	case CM_NETWORK_STATE_PDP_READY:
		net_ready = 1;
		cm_sys_log("[boat]:PDP ready\n");
		break;
	default:
		cm_sys_log("[boat]:Module stop\n");
	}
}

static void cm_test_net_register_init(void)
{
	// 网络配置结构体
	cm_network_param_t network_param;

	// 填写APN等信息，
	network_param.cid = 1;
	network_param.apn = "CMNET";
	network_param.pdp_type = "IP";
	network_param.use_auth = 0;

	// 初始化
	cm_net_register_init();
	// 网络参数配置
	cm_net_register_config(&network_param);
	// 设置网络状态变化时的回调函数
	cm_net_register_set_callback(network_state_change);
	// 启动网络自注册模块
	cm_net_register_start();
}

void boat_fabric_entry(void)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHlfabricTx tx_ptr;
	boat_try_declare;
	/* step-1: Boat SDK initialization */

	BoatLog(BOAT_LOG_VERBOSE, "Boat SDK initialization start");
	osDelay(50);

	BoatIotSdkInit();

	/* step-2: prepare wallet */

	BoatLog(BOAT_LOG_VERBOSE, "prepare wallet start");
	osDelay(50);

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

	BoatLog(BOAT_LOG_VERBOSE, "fabric transaction structure initialization start");
	osDelay(50);

	result = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxInit failed.");
		boat_throw(result, fabric_demo_catch);
	}
	/* step-4: set transaction timestamp */

	BoatLog(BOAT_LOG_VERBOSE, "set transaction timestamp start");
	osDelay(50);

	long int timesec = osiEpochSecond();
	result = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);

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

static void boat_test_network_config(void)
{
	// 采用ATCHART模块实现的网络自注册方式，实现自动注网
	cm_test_net_register_init();
	cm_test_wait_for_network();
}

void cm_main_task(void *p)
{
	boat_test_vir_at_init();
	boat_test_network_config();
	osDelay(10);
	boat_fabric_entry();
	while (1)
	{
		cm_sys_log("[boat] app is running\n");
		osDelay(500);
	}
}

void ML302_OpenCPU_Entry()
{
	osThreadAttr_t main_task_attr = {0};
	main_task_attr.name = "main_task";
	main_task_attr.stack_size = 1024 * 34;
	osDelay(20);
	osThreadId_t OC_Main_TaskHandle = osThreadNew(cm_main_task, 0, &main_task_attr);
}

// SDK中调用，不可删除
int cm_wdt_config_cb(void)
{
	return 0; // 关闭看门狗
			  // return 1;//开启看门狗
}

// SDK中调用，GNSS功能硬件版本自适应配置项，不可删除，亦无需修改，修改可能引起功能异常
int cm_gnss_uc6226_cb(void)
{
#if ((defined CM_HARDWARE_FNLM_SUPPORT) || (defined CM_HARDWARE_KNLM_SUPPORT) || (defined CM_HARDWARE_GNLM_SUPPORT) || (defined CM_HARDWARE_LNLM_SUPPORT))
	cm_sys_log("[boat] cm_gnss_uc6226_cb 1\n");
#ifdef CM_PROD_VERSION_H3
	cm_uc6226_gnss_reset(1);
#endif
	return 1;
#else
	cm_sys_log("[boat] cm_gnss_uc6226_cb 0\n");
	return 0;
#endif
}

int cm_gnss_uctd1036_cb(void)
{
#if ((defined CM_HARDWARE_ENLM_SUPPORT) || (defined CM_HARDWARE_PNLM_SUPPORT))
	cm_sys_log("[boat] cm_gnss_uctd1036_cb 1\n");
	return 1;
#else
	cm_sys_log("[boat] cm_gnss_uctd1036_cb 0\n");
	return 0;
#endif
}

/*
	function used internal, name is unchangeable
*/
int cm_sntp_call_cb(int result)
{
	if (result == 0)
	{
		cm_sys_log("[boat SNTP SYNC CB]failed\n");
	}
	else
	{
		cm_sys_log("[boat SNTP SYNC CB]success\n");
	}
}

// void cm_printf(char *str, ...)
// {
// }
