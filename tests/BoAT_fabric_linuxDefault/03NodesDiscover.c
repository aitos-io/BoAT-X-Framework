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
#include "tcase_fabric.h"
#include "protocolapi/api_hlfabric_discovery.h"
#define EXCEED_STR_MAX_LEN 4097
extern BoatIotSdkContext g_boat_iot_sdk_context;

#define peer0_org1_hostname "peer0.org1.example.com"
#define peer1_org1_hostname "peer1.org1.example.com"
#define peer0_org2_hostname "peer0.org2.example.com"
#define peer1_org2_hostname "peer1.org2.example.com"

#define peer0_org1_url  "peer0.org1.example.com:7051"
#define peer1_org1_url  "peer1.org1.example.com:8051"
#define peer0_org2_url  "peer0.org2.example.com:9051"
#define peer1_org2_url  "peer1.org2.example.com:10051"

#define order_hostname "orderer.example.com"
#define order_url "orderer.example.com:7050"


static BoatHlfabricWalletConfig get_fabric_wallet_settings()
{
    BoatHlfabricWalletConfig wallet_config = {0};
    wallet_config.accountPriKey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.accountPriKey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.accountPriKey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.accountPriKey_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_key_buf;
	wallet_config.accountPriKey_config.prikey_content.field_len = strlen(fabric_client_key_buf) + 1; //length contain terminator

	//set cert context
	wallet_config.accountCertContent.length = strlen(fabric_client_cert_buf) + 1;
	memcpy(wallet_config.accountCertContent.content, fabric_client_cert_buf, wallet_config.accountCertContent.length);

    wallet_config.nodesCfg.endorserLayoutNum = 1;
	wallet_config.nodesCfg.layoutCfg = BoatMalloc(wallet_config.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tls_cert_buf);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,fabric_org1_tls_cert_buf,strlen(fabric_org1_tls_cert_buf));

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG1_PEER0_PORT)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(ORG1_PEER0_HOSTNAME)+1);
   	memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,0,strlen(TEST_FABRIC_NODE_URL) + strlen(ORG1_PEER0_PORT)+1);
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,TEST_FABRIC_NODE_URL,strlen(TEST_FABRIC_NODE_URL));
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl+strlen(TEST_FABRIC_NODE_URL),ORG1_PEER0_PORT,strlen(ORG1_PEER0_PORT));
    memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,0,strlen(ORG1_PEER0_HOSTNAME)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,ORG1_PEER0_HOSTNAME,strlen(ORG1_PEER0_HOSTNAME));

    return wallet_config;
}

static BoatHlfabricWalletConfig get_fabric_wallet_settings_layoutCfg_NULL()
{
    BoatHlfabricWalletConfig wallet_config = {0};
    wallet_config.accountPriKey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.accountPriKey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.accountPriKey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.accountPriKey_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_key_buf;
	wallet_config.accountPriKey_config.prikey_content.field_len = strlen(fabric_client_key_buf) + 1; //length contain terminator

	//set cert context
	wallet_config.accountCertContent.length = strlen(fabric_client_cert_buf) + 1;
	memcpy(wallet_config.accountCertContent.content, fabric_client_cert_buf, wallet_config.accountCertContent.length);

    wallet_config.nodesCfg.endorserLayoutNum = 0;
	wallet_config.nodesCfg.layoutCfg = NULL;
    return wallet_config;
}


static BoatHlfabricWalletConfig get_fabric_wallet_settings_groupCfg_NULL()
{
    BoatHlfabricWalletConfig wallet_config = {0};
    wallet_config.accountPriKey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.accountPriKey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.accountPriKey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.accountPriKey_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_key_buf;
	wallet_config.accountPriKey_config.prikey_content.field_len = strlen(fabric_client_key_buf) + 1; //length contain terminator

	//set cert context
	wallet_config.accountCertContent.length = strlen(fabric_client_cert_buf) + 1;
	memcpy(wallet_config.accountCertContent.content, fabric_client_cert_buf, wallet_config.accountCertContent.length);

    wallet_config.nodesCfg.endorserLayoutNum = 1;
	wallet_config.nodesCfg.layoutCfg = BoatMalloc(wallet_config.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum = 0;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg = NULL;
    return wallet_config;
}


static BoatHlfabricWalletConfig get_fabric_wallet_settings_endorser_NULL()
{
    BoatHlfabricWalletConfig wallet_config = {0};
    wallet_config.accountPriKey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.accountPriKey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.accountPriKey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.accountPriKey_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_key_buf;
	wallet_config.accountPriKey_config.prikey_content.field_len = strlen(fabric_client_key_buf) + 1; //length contain terminator

	//set cert context
	wallet_config.accountCertContent.length = strlen(fabric_client_cert_buf) + 1;
	memcpy(wallet_config.accountCertContent.content, fabric_client_cert_buf, wallet_config.accountCertContent.length);

    wallet_config.nodesCfg.endorserLayoutNum = 1;
	wallet_config.nodesCfg.layoutCfg = BoatMalloc(wallet_config.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 0;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tls_cert_buf);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,fabric_org1_tls_cert_buf,strlen(fabric_org1_tls_cert_buf));

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser = NULL;

    return wallet_config;
}

static BOAT_RESULT check_fabric_nodesInfo(BoatHlfabricNodesCfg network_info)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /// peer
    if(network_info.endorserLayoutNum != 1){
        return BOAT_ERROR;
    }
    if(network_info.layoutCfg[0].endorserGroupNum != 2){
        BoatLog( BOAT_LOG_CRITICAL, "network_info.layoutCfg ERR : %d\n",network_info.layoutCfg->endorserGroupNum);
        return BOAT_ERROR;
    }
    for (size_t i = 0; i < network_info.layoutCfg[0].endorserGroupNum; i++)
    {
        /* code */
        if(network_info.layoutCfg[0].groupCfg[i].endorserNumber < 1){
            BoatLog( BOAT_LOG_CRITICAL, "groupCfg[0].endorserNumber ERR : %d\n",network_info.layoutCfg[0].groupCfg[0].endorserNumber);
            return BOAT_ERROR;
        }

        //  BoatLog( BOAT_LOG_CRITICAL, "network_info.layoutCfg[0].groupCfg[%d].endorserNumber  : %d\n",i,network_info.layoutCfg[0].groupCfg[i].endorserNumber);

        for (size_t j = 0; j < network_info.layoutCfg[0].groupCfg[i].endorserNumber; j++)
        {
            /* code */
            BoatLog( BOAT_LOG_CRITICAL, "network_info.layoutCfg[0].groupCfg[%d].endorser[%d].nodeUrl   :  %s ",i,j,network_info.layoutCfg[0].groupCfg[i].endorser[j].nodeUrl);
            if(memcmp(network_info.layoutCfg[0].groupCfg[i].endorser[j].nodeUrl,peer0_org1_url,strlen(peer0_org1_url)) != 0 &&
                memcmp(network_info.layoutCfg[0].groupCfg[i].endorser[j].nodeUrl,peer1_org1_url,strlen(peer1_org1_url)) != 0 &&
                memcmp(network_info.layoutCfg[0].groupCfg[i].endorser[j].nodeUrl,peer0_org2_url,strlen(peer0_org2_url)) != 0 &&
                memcmp(network_info.layoutCfg[0].groupCfg[i].endorser[j].nodeUrl,peer1_org2_url,strlen(peer1_org2_url)) != 0){
                BoatLog( BOAT_LOG_CRITICAL, "nodeurl != all_url");
                return BOAT_ERROR;
            }
            BoatLog( BOAT_LOG_CRITICAL, "network_info.layoutCfg[0].groupCfg[%d].endorser[%d].hostname  :  %s ",i,j,network_info.layoutCfg[0].groupCfg[i].endorser[j].hostName);
            if(memcmp(network_info.layoutCfg[0].groupCfg[i].endorser[j].hostName,peer0_org1_hostname,strlen(peer0_org1_hostname)) != 0 &&
                memcmp(network_info.layoutCfg[0].groupCfg[i].endorser[j].hostName,peer1_org1_hostname,strlen(peer1_org1_hostname)) != 0 &&
                memcmp(network_info.layoutCfg[0].groupCfg[i].endorser[j].hostName,peer0_org2_hostname,strlen(peer0_org2_hostname)) != 0 &&
                memcmp(network_info.layoutCfg[0].groupCfg[i].endorser[j].hostName,peer1_org2_hostname,strlen(peer1_org2_hostname)) != 0){
                BoatLog( BOAT_LOG_CRITICAL, "hostname != all hostname");
                return BOAT_ERROR;
            } 
        }
            BoatLog( BOAT_LOG_CRITICAL, "network_info.layoutCfg[0].groupCfg[i].quantities  :  %d ",i,network_info.layoutCfg[0].groupCfg[i].quantities);
        if(network_info.layoutCfg[0].groupCfg[i].quantities != 1){
            return BOAT_ERROR;
        }
        BoatLog( BOAT_LOG_CRITICAL, "network_info.layoutCfg[0].groupCfg[i].tlsOrgCertContent.length  :  %d ",i,network_info.layoutCfg[0].groupCfg[i].tlsOrgCertContent.length);
        if(network_info.layoutCfg[0].groupCfg[i].tlsOrgCertContent.length == 0){
            return BOAT_ERROR;
        }
        
    }
    BoatLog( BOAT_LOG_CRITICAL, "network_info.orderCfg.endorserNumber = %d ",network_info.orderCfg.endorserNumber );
    if(network_info.orderCfg.endorserNumber != 1){
        return BOAT_ERROR;
    }
        BoatLog( BOAT_LOG_CRITICAL, "network_info.orderCfg.tlsOrgCertContent.length = %d ",network_info.orderCfg.tlsOrgCertContent.length );
    if(network_info.orderCfg.tlsOrgCertContent.length == 0){
        return BOAT_ERROR;
    }
    for (size_t i = 0; i < network_info.orderCfg.endorserNumber; i++)
    {
        /* code */
        // BoatLog( BOAT_LOG_CRITICAL, "network_info.orderCfg.endorser[%d].hostName = %s ",i,network_info.orderCfg.endorser[i].hostName );
        // if(memcmp(network_info.orderCfg.endorser[i].hostName,order_hostname,strlen(order_hostname)) !=0 ){
        //     return BOAT_ERROR;
        // }
        BoatLog( BOAT_LOG_CRITICAL, "network_info.orderCfg.endorser[%d].nodeUrl = %s ",i,network_info.orderCfg.endorser[i].nodeUrl );
        if(memcmp(network_info.orderCfg.endorser[i].nodeUrl,order_url,strlen(order_url)) != 0){
            return BOAT_ERROR;
        }
    }
    


    
    

    return BOAT_SUCCESS;
}

START_TEST(test_003NodesDiscover_0001Discover_Success) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = check_fabric_nodesInfo(tx_ptr.wallet_ptr->network_info);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_003NodesDiscover_0002Discover_Fail_nodesCfg_nodeUrl_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatFree(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl);
    wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = NULL;
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_003NodesDiscover_0003Discover_Fail_nodesCfg_tlsCert_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,0x00,wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length);
    wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = 0;
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_003NodesDiscover_0004Discover_Fail_nodesCfg_tlsCert_ERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,0x00,wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length);
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,fabric_client_cert_false_buf,strlen(fabric_client_cert_false_buf));
    wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_client_cert_false_buf);
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_003NodesDiscover_0005Discover_Fail_nodesCfg_nodeUrl_ERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,'1',strlen(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl));
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_003NodesDiscover_0006Discover_Fail_nodesCfg_hostName_ERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    // memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,'1',strlen(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName));
    BoatFree(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName);
    wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(5);
    memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,0,5);
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,"1111",strlen("1111"));
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_003NodesDiscover_0007Discover_Fail_nodesCfg_layoutCfg_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings_layoutCfg_NULL();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_003NodesDiscover_0008Discover_Fail_nodesCfg_groupCfg_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings_groupCfg_NULL();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_003NodesDiscover_0009Discover_Fail_nodesCfg_endorser_NULL) 
{
    BSINT32 rtnVal;
    BoatHlfabricTx tx_ptr;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings_endorser_NULL();
    BoatIotSdkInit();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
    rtnVal = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    long int timesec = 0;
	time(&timesec);
	rtnVal = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    rtnVal = BoatHlfabricDiscoverySubmit(&tx_ptr,wallet_config.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatHlfabricTxDeInit(&tx_ptr);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

Suite *make_nodesDiscover_suite(void) 
{
    /* Create Suite */
    Suite *s_nodesDiscover = suite_create("nodesDiscover");

    /* Create test cases */
    TCase *tc_nodesDiscover_api = tcase_create("nodesDiscover_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_nodesDiscover, tc_nodesDiscover_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0001Discover_Success);
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0002Discover_Fail_nodesCfg_nodeUrl_NULL);
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0003Discover_Fail_nodesCfg_tlsCert_NULL);
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0004Discover_Fail_nodesCfg_tlsCert_ERR);
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0005Discover_Fail_nodesCfg_nodeUrl_ERR);
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0006Discover_Fail_nodesCfg_hostName_ERR);
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0007Discover_Fail_nodesCfg_layoutCfg_NULL);
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0008Discover_Fail_nodesCfg_groupCfg_NULL);
    tcase_add_test(tc_nodesDiscover_api, test_003NodesDiscover_0009Discover_Fail_nodesCfg_endorser_NULL);

    return s_nodesDiscover;
}

