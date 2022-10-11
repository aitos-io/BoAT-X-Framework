/******************************************************************************
 * Copyright (C) 2018-2022 aitos.io
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
#include "tcase_chainmaker.h"
#define EXCEED_STR_MAX_LEN 4097

BOAT_RESULT check_network_data(BoatChainmakerNetworkData *network_data)
{
    BOAT_RESULT result;

    result = strcmp(network_data->client_sign_cert_content.content, chainmaker_client_sign_cert);
    if (result != 0)
    {
        ck_abort_msg("check_network_data client_sign_cert_content error");
        return BOAT_ERROR;
    }

    result = strcmp(network_data->node_url, TEST_CHAINMAKER_NODE_URL);
    if (result != 0)
    {
        ck_abort_msg("check_network_data node_url error");
        return BOAT_ERROR;
    }

    result = strcmp(network_data->host_name, TEST_CHAINMAKER_NODE_URL);
    if (result != 0)
    {
        ck_abort_msg("check_network_data host_name error");
        return BOAT_ERROR;
    }

    result = strcmp(network_data->chain_id, TEST_CHAINMAKER_CHAIN_ID);
    if (result != 0)
    {
        ck_abort_msg("check_network_data chain_id error");
        return BOAT_ERROR;
    }

    result = strcmp(network_data->org_id, TEST_CHAINMAKER_ORG_ID);
    if (result != 0)
    {
        ck_abort_msg("check_network_data org_id error");
        return BOAT_ERROR;
    }

    result = strcmp(network_data->client_tls_cert_content.content, chainmaker_client_tls_cert);
    if (result != 0)
    {
        ck_abort_msg("check_network_data client_tls_cert_content error");
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

START_TEST(test_001keypair_0001CreateOneTimeNetworkSuccess) 
{
    BOAT_RESULT index;
    BOAT_RESULT result;
    BoatChainmakerNetworkData networkConfig = {0};
    BoatIotSdkInit();

    //set user cert context
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    memcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert,  networkConfig.client_sign_cert_content.length);

    strncpy(networkConfig.node_url,  TEST_CHAINMAKER_NODE_URL,  strlen(TEST_CHAINMAKER_NODE_URL));
    strncpy(networkConfig.host_name, TEST_CHAINMAKER_HOST_NAME, strlen(TEST_CHAINMAKER_HOST_NAME));
    strncpy(networkConfig.chain_id,  TEST_CHAINMAKER_CHAIN_ID,  strlen(TEST_CHAINMAKER_CHAIN_ID));
    strncpy(networkConfig.org_id,    TEST_CHAINMAKER_ORG_ID,    strlen(TEST_CHAINMAKER_ORG_ID));

    //set tls cert context
    networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert) + 1;
    memcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert,  networkConfig.client_tls_cert_content.length);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(index, 0);

    
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0002CreateOneTimeNetworkFailedWithNetworkConfigNuLL) 
{
    BOAT_RESULT index;
    BoatIotSdkInit();

    index = BoatChainmakerNetworkCreate(NULL, BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(index, -1);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0003CreateFivePersistNetworkAndDeleteSuccess) 
{
    BOAT_RESULT index;
    BOAT_RESULT result;
    BoatChainmakerNetworkContext networkList;
    BoatChainmakerNetworkData networkConfig = {0};
    BoatIotSdkInit();

    //set user cert context
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    memcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert,  networkConfig.client_sign_cert_content.length);

    strncpy(networkConfig.node_url,  TEST_CHAINMAKER_NODE_URL,  strlen(TEST_CHAINMAKER_NODE_URL));
    strncpy(networkConfig.host_name, TEST_CHAINMAKER_HOST_NAME, strlen(TEST_CHAINMAKER_HOST_NAME));
    strncpy(networkConfig.chain_id,  TEST_CHAINMAKER_CHAIN_ID,  strlen(TEST_CHAINMAKER_CHAIN_ID));
    strncpy(networkConfig.org_id,    TEST_CHAINMAKER_ORG_ID,    strlen(TEST_CHAINMAKER_ORG_ID));

    //set tls cert context
    networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert) + 1;
    memcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert,  networkConfig.client_tls_cert_content.length);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 1);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 2);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 3);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 4);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 5);

    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 5);


    result = check_network_data(&networkList.networks[0]);
    ck_assert_int_eq(result, 0);

    result = check_network_data(&networkList.networks[1]);
    ck_assert_int_eq(result, 0);

    result = check_network_data(&networkList.networks[2]);
    ck_assert_int_eq(result, 0);

    result = check_network_data(&networkList.networks[3]);
    ck_assert_int_eq(result, 0);

    result = check_network_data(&networkList.networks[4]);
    ck_assert_int_eq(result, 0);

    ck_assert_int_eq(networkList.networks[0].networkIndex, 1);
    ck_assert_int_eq(networkList.networks[1].networkIndex, 2);
    ck_assert_int_eq(networkList.networks[2].networkIndex, 3);
    ck_assert_int_eq(networkList.networks[3].networkIndex, 4);
    ck_assert_int_eq(networkList.networks[4].networkIndex, 5);

    result = BoATChainmakerNetworkDelete(1);
    ck_assert_int_eq(result, 0);
    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 4);

    result = BoATChainmakerNetworkDelete(2);
    ck_assert_int_eq(result, 0);
    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 3);

    result = BoATChainmakerNetworkDelete(3);
    ck_assert_int_eq(result, 0);
    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 2);

    result = BoATChainmakerNetworkDelete(4);
    ck_assert_int_eq(result, 0);
    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 1);

    result = BoATChainmakerNetworkDelete(5);
    ck_assert_int_eq(result, 0);
    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 0);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0004CreateSixPersistNetworkFailed) 
{
    BOAT_RESULT index;
    BOAT_RESULT result;
    BoatChainmakerNetworkContext networkList;
    BoatChainmakerNetworkData networkConfig = {0};
    BoatIotSdkInit();

    //set user cert context
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    memcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert,  networkConfig.client_sign_cert_content.length);

    strncpy(networkConfig.node_url,  TEST_CHAINMAKER_NODE_URL,  strlen(TEST_CHAINMAKER_NODE_URL));
    strncpy(networkConfig.host_name, TEST_CHAINMAKER_HOST_NAME, strlen(TEST_CHAINMAKER_HOST_NAME));
    strncpy(networkConfig.chain_id,  TEST_CHAINMAKER_CHAIN_ID,  strlen(TEST_CHAINMAKER_CHAIN_ID));
    strncpy(networkConfig.org_id,    TEST_CHAINMAKER_ORG_ID,    strlen(TEST_CHAINMAKER_ORG_ID));

    //set tls cert context
    networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert) + 1;
    memcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert,  networkConfig.client_tls_cert_content.length);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 1);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 2);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 3);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 4);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 5);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, -1);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0005CreateFivePersistNetworkDeletefour) 
{
    BOAT_RESULT index;
    BOAT_RESULT result;
    BoatChainmakerNetworkContext networkList;
    BoatChainmakerNetworkData networkConfig = {0};
    BoatIotSdkInit();

    //set user cert context
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    memcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert,  networkConfig.client_sign_cert_content.length);

    strncpy(networkConfig.node_url,  TEST_CHAINMAKER_NODE_URL,  strlen(TEST_CHAINMAKER_NODE_URL));
    strncpy(networkConfig.host_name, TEST_CHAINMAKER_HOST_NAME, strlen(TEST_CHAINMAKER_HOST_NAME));
    strncpy(networkConfig.chain_id,  TEST_CHAINMAKER_CHAIN_ID,  strlen(TEST_CHAINMAKER_CHAIN_ID));
    strncpy(networkConfig.org_id,    TEST_CHAINMAKER_ORG_ID,    strlen(TEST_CHAINMAKER_ORG_ID));

    //set tls cert context
    networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert) + 1;
    memcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert,  networkConfig.client_tls_cert_content.length);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 1);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 2);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 3);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 4);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 5);

    result = BoATChainmakerNetworkDelete(1);
    ck_assert_int_eq(result, 0);

    result = BoATChainmakerNetworkDelete(2);
    ck_assert_int_eq(result, 0);

    result = BoATChainmakerNetworkDelete(4);
    ck_assert_int_eq(result, 0);

    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 1);
    ck_assert_int_eq(networkList.networks[0].networkIndex, 3);


    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 1);
    ck_assert_int_eq(result, -1);

    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 2);
    ck_assert_int_eq(result, -1);

    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 3);
    ck_assert_int_eq(result, 0);

    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 4);
    ck_assert_int_eq(result, -1);

    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 5);
    ck_assert_int_eq(result, -1);


    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0006CreateFivePersistNetworkDeleteThreeWriteTwo) 
{
    BOAT_RESULT index;
    BOAT_RESULT result;
    BoatChainmakerNetworkContext networkList;
    BoatChainmakerNetworkData networkConfig = {0};
    BoatIotSdkInit();

    //set user cert context
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    memcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert,  networkConfig.client_sign_cert_content.length);

    strncpy(networkConfig.node_url,  TEST_CHAINMAKER_NODE_URL,  strlen(TEST_CHAINMAKER_NODE_URL));
    strncpy(networkConfig.host_name, TEST_CHAINMAKER_HOST_NAME, strlen(TEST_CHAINMAKER_HOST_NAME));
    strncpy(networkConfig.chain_id,  TEST_CHAINMAKER_CHAIN_ID,  strlen(TEST_CHAINMAKER_CHAIN_ID));
    strncpy(networkConfig.org_id,    TEST_CHAINMAKER_ORG_ID,    strlen(TEST_CHAINMAKER_ORG_ID));

    //set tls cert context
    networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert) + 1;
    memcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert,  networkConfig.client_tls_cert_content.length);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 1);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 2);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 3);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 4);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 5);

    result = BoATChainmakerNetworkDelete(1);
    ck_assert_int_eq(result, 0);

    result = BoATChainmakerNetworkDelete(2);
    ck_assert_int_eq(result, 0);

    result = BoATChainmakerNetworkDelete(5);
    ck_assert_int_eq(result, 0);

    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 2);
    ck_assert_int_eq(networkList.networks[0].networkIndex, 3);


    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 1);
    ck_assert_int_eq(result, -1);

    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 2);
    ck_assert_int_eq(result, -1);

    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 3);
    ck_assert_int_eq(result, 0);

    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 4);
    ck_assert_int_eq(result, 0);

    result = BoATChainmaker_GetNetworkByIndex(&networkConfig, 5);
    ck_assert_int_eq(result, -1);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 3);

    index = BoatChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 4);

    result = BoATChainmaker_GetNetworkList(&networkList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(networkList.networkNum, 4);


    BoatIotSdkDeInit();
}
END_TEST


Suite *make_network_suite(void) 
{
    /* Create Suite */
    Suite *s_network = suite_create("network");

    /* Create test cases */
    TCase *tc_network_api = tcase_create("network_api");
    /* Add a test case to the Suite */
    suite_add_tcase(s_network, tc_network_api);       
    /* Test cases are added to the test set */

    tcase_add_test(tc_network_api, test_001keypair_0001CreateOneTimeNetworkSuccess); 
    tcase_add_test(tc_network_api, test_001keypair_0002CreateOneTimeNetworkFailedWithNetworkConfigNuLL); 
    tcase_add_test(tc_network_api, test_001keypair_0003CreateFivePersistNetworkAndDeleteSuccess); 
    tcase_add_test(tc_network_api, test_001keypair_0004CreateSixPersistNetworkFailed); 
    tcase_add_test(tc_network_api, test_001keypair_0005CreateFivePersistNetworkDeletefour); 
    tcase_add_test(tc_network_api, test_001keypair_0006CreateFivePersistNetworkDeleteThreeWriteTwo);

    return s_network;
}
