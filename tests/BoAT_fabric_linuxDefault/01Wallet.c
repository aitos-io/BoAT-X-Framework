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
#define EXCEED_STR_MAX_LEN 4097
const uint8_t fabric_client_pubkey[] = {0xbb,0x32,0x5f,0x5e,0x56,0x3f,0x38,0x88,0x5a,0x9a,0xa4,0xf8,0xa6,0xf3,0x0f,0xc7,0x59,0xd8,0x34,0x99,0x02,0x5e,0x4c,0x49,0xd1,0xde,0x59,0xdc,0xf4,0x76,0x29,0x5f,0xd9,0x0a,0x16,0xfd,0x9f,0xaa,0xbe,0xe3,0x4a,0x31,0x20,0x1e,0x79,0x25,0xf1,0xbd,0xa3,0x02,0x19,0xf1,0x4f,0x9c,0x5d,0xa1,0x1d,0xa8,0x55,0x70,0xf3,0x73,0x09,0x62};
extern BoatIotSdkContext g_boat_iot_sdk_context;
BOAT_RESULT check_fabric_wallet(BoatHlfabricWallet *wallet_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if(wallet_ptr->account_info.prikeyCtx.prikey_format != BOAT_WALLET_PRIKEY_FORMAT_PKCS){
        return BOAT_ERROR;
    }
    if(wallet_ptr->account_info.prikeyCtx.prikey_type != BOAT_WALLET_PRIKEY_TYPE_SECP256R1){
        return BOAT_ERROR;
    }
    if(memcmp(wallet_ptr->account_info.prikeyCtx.pubkey_content,fabric_client_pubkey,sizeof(wallet_ptr->account_info.prikeyCtx.pubkey_content) ) != 0){
        return BOAT_ERROR;
    }
    if(wallet_ptr->account_info.prikeyCtx.pubkey_format != BOAT_WALLET_PUBKEY_FORMAT_NATIVE){
        return BOAT_ERROR;
    }
    if(wallet_ptr->account_info.prikeyCtx.extra_data.value_len != strlen(fabric_client_key_buf)+1){
        return BOAT_ERROR;
    }
    result = strncmp(wallet_ptr->account_info.prikeyCtx.extra_data.value, fabric_client_key_buf, strlen(fabric_client_key_buf));
    if (result != 0)
    {
        return result;
    }
    if(wallet_ptr->network_info.endorserLayoutNum != 1){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.layoutCfg == NULL){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.layoutCfg[0].endorserGroupNum != 2){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.layoutCfg[0].groupCfg == NULL){
        return BOAT_ERROR;
    }
    //org1
    if(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].endorserNumber != 2){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].endorser == NULL){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].quantities != 1){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length != strlen(fabric_org1_tls_cert_buf)){
        return BOAT_ERROR;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tls_cert_buf, strlen(fabric_org1_tls_cert_buf));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].endorser[0].hostName, ORG1_PEER0_HOSTNAME, strlen(ORG1_PEER0_HOSTNAME));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, TEST_FABRIC_NODE_URL, strlen(TEST_FABRIC_NODE_URL));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORG1_PEER0_PORT, strlen(ORG1_PEER0_PORT));
    if (result != 0)
    {
        return result;
    }

    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].endorser[1].hostName, ORG1_PEER1_HOSTNAME, strlen(ORG1_PEER1_HOSTNAME));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, TEST_FABRIC_NODE_URL, strlen(TEST_FABRIC_NODE_URL));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORG1_PEER1_PORT, strlen(ORG1_PEER1_PORT));
    if (result != 0)
    {
        return result;
    }

    // org2
   if(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].endorser == NULL){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].quantities != 1){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length != strlen(fabric_org2_tls_cert_buf)){
        return BOAT_ERROR;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, fabric_org2_tls_cert_buf, strlen(fabric_org2_tls_cert_buf));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].endorser[0].hostName, ORG2_PEER0_HOSTNAME, strlen(ORG2_PEER0_HOSTNAME));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, TEST_FABRIC_NODE_URL, strlen(TEST_FABRIC_NODE_URL));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORG2_PEER0_PORT, strlen(ORG2_PEER0_PORT));
    if (result != 0)
    {
        return result;
    }

    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].endorser[1].hostName, ORG2_PEER1_HOSTNAME, strlen(ORG2_PEER1_HOSTNAME));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, TEST_FABRIC_NODE_URL, strlen(TEST_FABRIC_NODE_URL));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORG2_PEER1_PORT, strlen(ORG2_PEER1_PORT));
    if (result != 0)
    {
        return result;
    }

    // order
    if(wallet_ptr->network_info.orderCfg.endorserNumber != 1){
        return BOAT_ERROR;
    }
    if(wallet_ptr->network_info.orderCfg.endorser == NULL){
        return BOAT_ERROR;
    }
    result = strncmp(wallet_ptr->network_info.orderCfg.endorser[0].hostName, ORDER_HOSTNAME, strlen(ORDER_HOSTNAME));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.orderCfg.endorser[0].nodeUrl, TEST_FABRIC_NODE_URL, strlen(TEST_FABRIC_NODE_URL));
    if (result != 0)
    {
        return result;
    }
    result = strncmp(wallet_ptr->network_info.orderCfg.endorser[0].nodeUrl + strlen(TEST_FABRIC_NODE_URL), ORDER_PORT, strlen(ORDER_PORT));
    if (result != 0)
    {
        return result;
    }
    if(wallet_ptr->network_info.orderCfg.tlsOrgCertContent.length != strlen(fabric_order_tls_cert_buf)){
        return BOAT_ERROR;
    }
    result = strncmp(wallet_ptr->network_info.orderCfg.tlsOrgCertContent.content, fabric_order_tls_cert_buf, strlen(fabric_order_tls_cert_buf));

    if (result != 0)
    {
        return result;
    }

    return BOAT_SUCCESS;
}

BoatHlfabricWalletConfig get_fabric_wallet_settings()
{
    BoatHlfabricWalletConfig wallet_config = {0};
    BOAT_RESULT index;

	//BoatHlfabricWalletConfig wallet_config = {0};
	//set private key context
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
	wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tls_cert_buf);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content,fabric_org1_tls_cert_buf,strlen(fabric_org1_tls_cert_buf));

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG1_PEER0_PORT)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(ORG1_PEER0_HOSTNAME)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG1_PEER1_PORT)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = BoatMalloc(strlen(ORG1_PEER1_HOSTNAME)+1);
   	memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,0,strlen(TEST_FABRIC_NODE_URL) + strlen(ORG1_PEER0_PORT)+1);
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,TEST_FABRIC_NODE_URL,strlen(TEST_FABRIC_NODE_URL));
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl+strlen(TEST_FABRIC_NODE_URL),ORG1_PEER0_PORT,strlen(ORG1_PEER0_PORT));
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName,ORG1_PEER0_HOSTNAME,strlen(ORG1_PEER0_HOSTNAME));
	memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl,0,strlen(TEST_FABRIC_NODE_URL) + strlen(ORG1_PEER1_PORT)+1);
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl,TEST_FABRIC_NODE_URL,strlen(TEST_FABRIC_NODE_URL));
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl+strlen(TEST_FABRIC_NODE_URL),ORG1_PEER1_PORT,strlen(ORG1_PEER1_PORT));
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName,ORG1_PEER1_HOSTNAME,strlen(ORG1_PEER1_HOSTNAME));

	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tls_cert_buf);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content,fabric_org2_tls_cert_buf,strlen(fabric_org2_tls_cert_buf));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG2_PEER0_PORT)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(ORG2_PEER0_HOSTNAME)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG2_PEER1_PORT)+1);
	wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = BoatMalloc(strlen(ORG2_PEER1_HOSTNAME)+1);
	memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl,0,strlen(TEST_FABRIC_NODE_URL) + strlen(ORG2_PEER0_PORT)+1);
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl,TEST_FABRIC_NODE_URL,strlen(TEST_FABRIC_NODE_URL));
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl+strlen(TEST_FABRIC_NODE_URL),ORG2_PEER0_PORT,strlen(ORG2_PEER0_PORT));
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName,ORG2_PEER0_HOSTNAME,strlen(ORG2_PEER0_HOSTNAME));
    memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl,0,strlen(TEST_FABRIC_NODE_URL) + strlen(ORG2_PEER1_PORT)+1);
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl,TEST_FABRIC_NODE_URL,strlen(TEST_FABRIC_NODE_URL));
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl+strlen(TEST_FABRIC_NODE_URL),ORG2_PEER1_PORT,strlen(ORG2_PEER1_PORT));
	memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName,ORG2_PEER1_HOSTNAME,strlen(ORG2_PEER1_HOSTNAME));

	//set orderer info
	wallet_config.nodesCfg.orderCfg.endorserNumber = 1;
	wallet_config.nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tls_cert_buf);
	memcpy(wallet_config.nodesCfg.orderCfg.tlsOrgCertContent.content,fabric_order_tls_cert_buf,strlen(fabric_order_tls_cert_buf));
	wallet_config.nodesCfg.orderCfg.endorser = BoatMalloc(wallet_config.nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	wallet_config.nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(ORDER_HOSTNAME)+1);
	wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(TEST_FABRIC_NODE_URL)+strlen(ORDER_PORT)+1);
    memset(wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl,0,strlen(TEST_FABRIC_NODE_URL)+strlen(ORDER_PORT)+1);
	memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl,TEST_FABRIC_NODE_URL,strlen(TEST_FABRIC_NODE_URL));
    memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].nodeUrl + strlen(TEST_FABRIC_NODE_URL),ORDER_PORT,strlen(ORDER_PORT));
	memcpy(wallet_config.nodesCfg.orderCfg.endorser[0].hostName,ORDER_HOSTNAME,strlen(ORDER_HOSTNAME));

    return wallet_config;
}

START_TEST(test_001CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0002CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

     /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));
    ck_assert_int_eq(rtnVal, 1);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS); 
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0003CreateOneTimeWalletFail_KeyGenmodeErr) 
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    wallet_config.accountPriKey_config.prikey_genMode = BOAT_WALLET_PRIKEY_FORMAT_UNKNOWN;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, BOAT_ERROR_WALLET_KEY_GENMODE_ERR);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0004CreateOneTimeWalletFail_KeyTypeErr) 
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    wallet_config.accountPriKey_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_UNKNOWN;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, BOAT_ERROR_WALLET_KEY_TYPE_ERR);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0005CreateOneTimeWalletFail_KeyFormatERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    wallet_config.accountPriKey_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_UNKNOWN;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, BOAT_ERROR_WALLET_KEY_FORMAT_ERR);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0006CreateOneTimeWalletFail_KeyContentERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    wallet_config.accountPriKey_config.prikey_content.field_ptr = fabric_client_key_false_buf;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0007CreateOneTimeWalletFail_AccountCertERR) 
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    memset(wallet_config.accountCertContent.content,0,wallet_config.accountCertContent.length);
    wallet_config.accountCertContent.length = strlen(fabric_client_cert_false_buf);
    memcpy(wallet_config.accountCertContent.content,fabric_client_cert_false_buf,strlen(fabric_client_cert_false_buf));

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0008CreateOneTimeWalletFail_UrlERR)
{
    BSINT32 rtnVal;
    BUINT8 testURL[3] = {1,2,3};
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    memset(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,0,strlen(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl));
    memcpy(wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl,testURL,sizeof(testURL));

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, BOAT_ERROR);
    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0009CreateOneTimeWalletFail_NodesCfgERR)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 0;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    wallet_config.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 3;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, BOAT_ERROR);

    BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0010CreateOneTimeWalletFail_WalletConfigNULL)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;

    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, NULL, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);


    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001CreateWallet_0011CreatePersistWalletSuccess)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);
    // BoatIotSdkDeInit();
    BoatWalletDelete("fabric.cfg");
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0012CreatePersistWalletSuccess_10wallets)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric01.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);
    fabricWalletConfigFree(wallet_config);
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric02.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 1);
    ck_assert(g_boat_iot_sdk_context.wallet_list[1].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

    fabricWalletConfigFree(wallet_config);
    wallet_config = get_fabric_wallet_settings();   
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric03.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 2);
    ck_assert(g_boat_iot_sdk_context.wallet_list[2].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

    fabricWalletConfigFree(wallet_config);  
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric04.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 3);
    ck_assert(g_boat_iot_sdk_context.wallet_list[3].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

    fabricWalletConfigFree(wallet_config); 
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric05.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 4);
    ck_assert(g_boat_iot_sdk_context.wallet_list[4].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

    fabricWalletConfigFree(wallet_config); 
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric06.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 5);
    ck_assert(g_boat_iot_sdk_context.wallet_list[5].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

    // wallet_config = get_fabric_wallet_settings();
    // rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric07.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    // ck_assert_int_eq(rtnVal, 6);
    // ck_assert(g_boat_iot_sdk_context.wallet_list[6].is_used == true);
    // g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    // ck_assert(g_fabric_wallet_ptr != NULL);
	// rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	// ck_assert(rtnVal == BOAT_SUCCESS);
    // ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

    // wallet_config = get_fabric_wallet_settings();
    // rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric08.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    // ck_assert_int_eq(rtnVal, 7);
    // ck_assert(g_boat_iot_sdk_context.wallet_list[7].is_used == true);
    // g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    // ck_assert(g_fabric_wallet_ptr != NULL);
	// rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	// ck_assert(rtnVal == BOAT_SUCCESS);
    // ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

    // wallet_config = get_fabric_wallet_settings();
    // rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric09.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    // ck_assert_int_eq(rtnVal, 8);
    // ck_assert(g_boat_iot_sdk_context.wallet_list[8].is_used == true);
    // g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    // ck_assert(g_fabric_wallet_ptr != NULL);
	// rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	// ck_assert(rtnVal == BOAT_SUCCESS);
    // ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);

    // wallet_config = get_fabric_wallet_settings();
    // rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric10.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    // ck_assert_int_eq(rtnVal, 9);
    // ck_assert(g_boat_iot_sdk_context.wallet_list[9].is_used == true);
    // g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    // ck_assert(g_fabric_wallet_ptr != NULL);
	// rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	// ck_assert(rtnVal == BOAT_SUCCESS);
    // ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);


    // BoatIotSdkDeInit();
    fabricWalletConfigFree(wallet_config); 
    BoatWalletDelete("fabric01.cfg");
    BoatWalletDelete("fabric02.cfg");
    BoatWalletDelete("fabric03.cfg");
    BoatWalletDelete("fabric04.cfg");
    BoatWalletDelete("fabric05.cfg");
    BoatWalletDelete("fabric06.cfg");
}
END_TEST

START_TEST(test_001CreateWallet_0013CreatePersistWalletFail_11wallets)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric01.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 0);

    fabricWalletConfigFree(wallet_config);
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric02.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 1);


    fabricWalletConfigFree(wallet_config);
    wallet_config = get_fabric_wallet_settings();   
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric03.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 2);


    fabricWalletConfigFree(wallet_config);  
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric04.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 3);


    fabricWalletConfigFree(wallet_config); 
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric05.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 4);


    fabricWalletConfigFree(wallet_config); 
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric06.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 5);

    fabricWalletConfigFree(wallet_config); 
    wallet_config = get_fabric_wallet_settings();
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric07.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, BOAT_ERROR_WALLET_NUM_EXCEED);



    fabricWalletConfigFree(wallet_config); 
    BoatWalletDelete("fabric01.cfg");
    BoatWalletDelete("fabric02.cfg");
    BoatWalletDelete("fabric03.cfg");
    BoatWalletDelete("fabric04.cfg");
    BoatWalletDelete("fabric05.cfg");
    BoatWalletDelete("fabric06.cfg");
}
END_TEST

START_TEST(test_001CreateWallet_0014CreatePersistWalletFail_KeyTypeErr)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    wallet_config.accountPriKey_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_UNKNOWN;
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, BOAT_ERROR_WALLET_KEY_TYPE_ERR);

    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0015GetWalletByIndex_Success)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
 /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0016GetWalletByIndex_Index_ERR)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
 /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(1);
    ck_assert(g_fabric_wallet_ptr == NULL);
    fabricWalletConfigFree(wallet_config);
}
END_TEST

START_TEST(test_001CreateWallet_0017BoatWalletUnload_Success)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
 /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);
    fabricWalletConfigFree(wallet_config);
    BoatWalletUnload(0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == false);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(0);
    ck_assert(g_fabric_wallet_ptr == NULL);

}
END_TEST

START_TEST(test_001CreateWallet_0018BoatWalletUnload_index_ERR)
{
    BSINT32 rtnVal;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
 /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, NULL, &wallet_config, sizeof(BoatHlfabricWalletConfig));

    
    ck_assert_int_eq(rtnVal, 0);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(rtnVal);
    ck_assert(g_fabric_wallet_ptr != NULL);
	rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);
    fabricWalletConfigFree(wallet_config);
    BoatWalletUnload(1);
    ck_assert(g_boat_iot_sdk_context.wallet_list[0].is_used == true);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(0);
    ck_assert(g_fabric_wallet_ptr != NULL);

}
END_TEST

START_TEST(test_001CreateWallet_0019BoatWalletDelete_Success)
{
    BSINT32 rtnVal;
    BSINT32 index;
    BoatHlfabricWallet *g_fabric_wallet_ptr = NULL;
    BoatHlfabricWalletConfig wallet_config = get_fabric_wallet_settings();
    
    /* 1. execute unit test */
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", &wallet_config, sizeof(BoatHlfabricWalletConfig));

    ck_assert_int_eq(rtnVal, 0);
    BoatWalletUnload(0);

	index = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", NULL, sizeof(BoatHlfabricWalletConfig));
	ck_assert_int_eq(rtnVal, 0);
    g_fabric_wallet_ptr = BoatGetWalletByIndex(index);
    rtnVal = BoatHlfabricWalletSetNetworkInfo(g_fabric_wallet_ptr, wallet_config.nodesCfg);
	ck_assert(rtnVal == BOAT_SUCCESS);

    ck_assert(check_fabric_wallet(g_fabric_wallet_ptr) == BOAT_SUCCESS);
    fabricWalletConfigFree(wallet_config); 
    BoatWalletDelete("fabric.cfg");
    rtnVal = BoatWalletCreate(BOAT_PROTOCOL_HLFABRIC, "fabric.cfg", NULL, sizeof(BoatHlfabricWalletConfig));
	ck_assert_int_eq(rtnVal, BOAT_ERROR_PERSISTER_READ_FAIL);
}
END_TEST


Suite *make_wallet_suite(void) 
{
    /* Create Suite */
    Suite *s_wallet = suite_create("wallet");

    /* Create test cases */
    TCase *tc_wallet_api = tcase_create("wallet_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_wallet, tc_wallet_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0001CreateOneTimeWalletSuccess);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0002CreateOneTimeWalletSuccess);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0003CreateOneTimeWalletFail_KeyGenmodeErr);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0004CreateOneTimeWalletFail_KeyTypeErr);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0005CreateOneTimeWalletFail_KeyFormatERR);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0006CreateOneTimeWalletFail_KeyContentERR);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0007CreateOneTimeWalletFail_AccountCertERR);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0008CreateOneTimeWalletFail_UrlERR);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0009CreateOneTimeWalletFail_NodesCfgERR);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0010CreateOneTimeWalletFail_WalletConfigNULL);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0011CreatePersistWalletSuccess);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0012CreatePersistWalletSuccess_10wallets);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0013CreatePersistWalletFail_11wallets);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0014CreatePersistWalletFail_KeyTypeErr);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0015GetWalletByIndex_Success);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0016GetWalletByIndex_Index_ERR);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0017BoatWalletUnload_Success);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0018BoatWalletUnload_index_ERR);
    tcase_add_test(tc_wallet_api, test_001CreateWallet_0019BoatWalletDelete_Success);


    return s_wallet;
}

