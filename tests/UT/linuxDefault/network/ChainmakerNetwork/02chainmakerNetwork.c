/**
****************************************************************************************
* @FilePath: ChainmakerNetwork_test.c
* @Author: aitos
* @Date: 2022-10-29 14:57:56
* @LastEditors:
* @LastEditTime: 2022-10-29 14:57:56
* @Descripttion:
****************************************************************************************
*/
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
#include "tcase_network.h"
#include "boatiotsdk.h"
#include "boatlog.h"

/**
 * test node url
 */

const BCHAR *chainmaker_client_sign_prikey = "-----BEGIN EC PRIVATE KEY-----\n"
                                             "MHcCAQEEIOyaQAt/ksxq8VOlkJqvFyne60m8IjaRMCaDgmVACOF3oAoGCCqGSM49\n"
                                             "AwEHoUQDQgAEvhmUZnXAdpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVW\n"
                                             "CYCeeChl4OBno9aCLc5BvFk4tmKDdOp96g==\n"
                                             "-----END EC PRIVATE KEY-----\n";

const BCHAR *native_demoKey = "0xec9a400b7f92cc6af153a5909aaf1729deeb49bc22369130268382654008e177";

const BCHAR *chainmaker_clinet_tls_prikey = "-----BEGIN EC PRIVATE KEY-----\n"
                                            "MHcCAQEEIOyaQAt/ksxq8VOlkJqvFyne60m8IjaRMCaDgmVACOF3oAoGCCqGSM49\n"
                                            "AwEHoUQDQgAEvhmUZnXAdpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVW\n"
                                            "CYCeeChl4OBno9aCLc5BvFk4tmKDdOp96g==\n"
                                            "-----END EC PRIVATE KEY-----\n";

const BCHAR *chainmaker_client_sign_cert = "-----BEGIN CERTIFICATE-----\n"
                                           "MIICdzCCAh6gAwIBAgIDC9o/MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
                                           "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
                                           "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
                                           "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMDExMTA4NDk0OFoXDTI3\n"
                                           "MDExMDA4NDk0OFowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
                                           "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
                                           "MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
                                           "LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvhmUZnXA\n"
                                           "dpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVWCYCeeChl4OBno9aCLc5B\n"
                                           "vFk4tmKDdOp96qNqMGgwDgYDVR0PAQH/BAQDAgbAMCkGA1UdDgQiBCDfUK7TyHf+\n"
                                           "FJl27KT6uoRgcDOmNgwmc5G0A2dhn5fQfTArBgNVHSMEJDAigCAMxN5G+OtJXgmo\n"
                                           "0MEU1OcJTBuDkfakZV07kDTZI9TcpjAKBggqhkjOPQQDAgNHADBEAiBEmn2YPZ0r\n"
                                           "zzPwilsgtOcoPc6PkOawUnWIU6d7iehfIQIgaqDwoWO6nwT0q0Tasr2+suqidv9V\n"
                                           "WOhSqMzOl8zMQ0c=\n"
                                           "-----END CERTIFICATE-----\n";

const BCHAR *chainmaker_client_tls_cert = "-----BEGIN CERTIFICATE-----\n"
                                          "MIICnzCCAkSgAwIBAgIDBXs2MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
                                          "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
                                          "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
                                          "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMDExMTA4NDk0OFoXDTMy\n"
                                          "MDEwOTA4NDk0OFowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
                                          "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
                                          "MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
                                          "YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASb/f+wD0tJVZ78bXBF\n"
                                          "LU/tar8wOoJi8+5qFwifEUwe4WdTBZJeqcqxn2/pcErzXnzGfcHMY609CU8sW2Bb\n"
                                          "zOyTo4GWMIGTMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
                                          "DgQiBCAMxN5G+OtJXgmo0MEU1OcJTBuDkfakZV07kDTZI9TcpjBFBgNVHREEPjA8\n"
                                          "gg5jaGFpbm1ha2VyLm9yZ4IJbG9jYWxob3N0ghljYS53eC1vcmcxLmNoYWlubWFr\n"
                                          "ZXIub3JnhwR/AAABMAoGCCqGSM49BAMCA0kAMEYCIQCoXy5qAIGrfnxRop9pgy9A\n"
                                          "XQdXmu1mlryNGVmfisnm2gIhAMjQP7CTwoJDeu8bs36i+7ha4bIHct+pmE5BpjYh\n"
                                          "ln6O\n"
                                          "-----END CERTIFICATE-----\n";
const BCHAR *chainmaker_ca_tls_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIICnjCCAkOgAwIBAgICK7swCgYIKoZIzj0EAwIwgYoxCzAJBgNVBAYTAkNOMRAw\n"
    "DgYDVQQIEwdCZWlqaW5nMRAwDgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1v\n"
    "cmcxLmNoYWlubWFrZXIub3JnMRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMT\n"
    "GWNhLnd4LW9yZzEuY2hhaW5tYWtlci5vcmcwHhcNMjIxMTI1MDYyNTMxWhcNMzIx\n"
    "MTIyMDYyNTMxWjCBijELMAkGA1UEBhMCQ04xEDAOBgNVBAgTB0JlaWppbmcxEDAO\n"
    "BgNVBAcTB0JlaWppbmcxHzAdBgNVBAoTFnd4LW9yZzEuY2hhaW5tYWtlci5vcmcx\n"
    "EjAQBgNVBAsTCXJvb3QtY2VydDEiMCAGA1UEAxMZY2Eud3gtb3JnMS5jaGFpbm1h\n"
    "a2VyLm9yZzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABMbHVgzI2LlAMNsPCnZO\n"
    "IUfYpkqRNDwjFalPs3gUQndwUYWCl1BiguaWed9fHorsfnc+GwTGjcIwE7iO/nVu\n"
    "1najgZYwgZMwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wKQYDVR0O\n"
    "BCIEIP8JF6Wzp59FtiT7y15PbQ75V22Z437e+kiBdsotqtNnMEUGA1UdEQQ+MDyC\n"
    "DmNoYWlubWFrZXIub3Jngglsb2NhbGhvc3SCGWNhLnd4LW9yZzEuY2hhaW5tYWtl\n"
    "ci5vcmeHBH8AAAEwCgYIKoZIzj0EAwIDSQAwRgIhAKN3jHuuMUsfY9UG6cyrRLtN\n"
    "MtqVL1KAr+h/KBxwCRWcAiEAx3uFKTj/RCNqJVL35ULz5pWcIaK/0TWRuq6sisXd\n"
    "wVk=\n"
    "-----END CERTIFICATE-----\n";

BCHAR *chainmaker_node_url[6] = {"127.0.0.0:12301", "127.0.0.1:12301", "127.0.0.2:12301", "127.0.0.3:12301", "127.0.0.4:12301", "127.0.0.5:12301"};
BCHAR *chainmaker_host_name[6] = {"chainmaker.org0", "chainmaker.org1", "chainmaker.org2", "chainmaker.org3", "chainmaker.org4", "chainmaker.org5"};
BCHAR *chainmaker_chain_id[6] = {"chain0", "chain1", "chain2", "chain3", "chain4", "chain5"};
BCHAR *chainmaker_org_id[6] = {"wx-org1.chainmaker.org0", "wx-org1.chainmaker.org1", "wx-org1.chainmaker.org2", "wx-org1.chainmaker.org3", "wx-org1.chainmaker.org4", "wx-org1.chainmaker.org5"};

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create chainmaker network;
* @param[in] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getChainmakerNetworkConfig(BoatChainmakerNetworkData *networkConfig, BUINT8 networkIndex)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig || networkIndex > BOAT_MAX_NETWORK_NUM)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set user cert context
    networkConfig->client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    if (networkConfig->client_sign_cert_content.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig->client_sign_cert_content.content, chainmaker_client_sign_cert, networkConfig->client_sign_cert_content.length);
    networkConfig->node_url = BoatMalloc(strlen(chainmaker_node_url[networkIndex]) + 1);
    networkConfig->host_name = BoatMalloc(strlen(chainmaker_host_name[networkIndex]) + 1);
    networkConfig->chain_id = BoatMalloc(strlen(chainmaker_chain_id[networkIndex]) + 1);
    networkConfig->org_id = BoatMalloc(strlen(chainmaker_org_id[networkIndex]) + 1);

    strcpy(networkConfig->node_url, chainmaker_node_url[networkIndex]);
    strcpy(networkConfig->host_name, chainmaker_host_name[networkIndex]);
    strcpy(networkConfig->chain_id, chainmaker_chain_id[networkIndex]);
    strcpy(networkConfig->org_id, chainmaker_org_id[networkIndex]);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    networkConfig->ca_tls_cert_content.length = strlen(chainmaker_ca_tls_cert);
    if (networkConfig->ca_tls_cert_content.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig->ca_tls_cert_content.content, chainmaker_ca_tls_cert);

#if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
    // set tls cert context
    networkConfig->client_tls_cert_content.length = strlen(chainmaker_client_tls_cert);
    if (networkConfig->client_tls_cert_content.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig->client_tls_cert_content.content, chainmaker_client_tls_cert);

    networkConfig->client_tls_privkey_data.value_len = strlen(chainmaker_clinet_tls_prikey);
    if (networkConfig->client_tls_privkey_data.value_len > BOAT_CHAINMAKER_PRIKEY_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy((BCHAR *)networkConfig->client_tls_privkey_data.value, chainmaker_clinet_tls_prikey);
#endif
#endif
    return result;
}
/**
****************************************************************************************
* @brief:
*  This function free the configuration to create chainmaker network with other url/hostname/chain_id/org_id
* @param[in] networkConfig
* @return
*  This function returns NULL
****************************************************************************************
*/
void chainmakerWalletConfigFree(BoatChainmakerNetworkData networkConfig)
{
    if (networkConfig.node_url != NULL)
    {
        BoatFree(networkConfig.node_url);
    }

    if (networkConfig.host_name != NULL)
    {
        BoatFree(networkConfig.host_name);
    }

    if (networkConfig.chain_id != NULL)
    {
        BoatFree(networkConfig.chain_id);
    }

    if (networkConfig.org_id != NULL)
    {
        BoatFree(networkConfig.org_id);
    }
}
/**
****************************************************************************************
* @brief:
*  This function test case : create onetime chainmaker network success
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0001CreateOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data */
    BoatSleep(1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id[Index]);
    BoatSleep(1);
    /* check hostname in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.host_name, chainmaker_host_name[Index]);
    /* check orgID in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.org_id, chainmaker_org_id[Index]);
    /* check url in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.node_url, chainmaker_node_url[Index]);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
    /* delete the onetime networl */
    BoATChainmakerNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime chainmaker network success;
*  Create a onetime network and again , check the result.
*  The second onetime network will cover the first one.
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0002CreateMoreOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    BoatSleep(1);
    /* create onetime network twice*/
    for (size_t i = 0; i < 2; i++)
    {
        /* get configuration */
        rtnVal = getChainmakerNetworkConfig(&network_config, i);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* create onetime network , store type must be RAM */
        rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
        /* check index of onetime network , the index must equal 0 */
        ck_assert_int_eq(rtnVal, 0);
        /* free configuration struct */
        chainmakerWalletConfigFree(network_config);
    }
    /* get network data by index , index = 0 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id[Index]);
    /* check hostname in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.host_name, chainmaker_host_name[Index]);
    /* check orgID in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.org_id, chainmaker_org_id[Index]);
    /* check url in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.node_url, chainmaker_node_url[Index]);
    /* delete the onetime networl */
    BoATChainmakerNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network success;
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0003CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* create persist network */
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ;
        index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network, the index must equal 1  */
    ck_assert_int_eq(rtnVal, 1);
    /* get network data by index , index = 1 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 1);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 1 */
    ck_assert_int_eq(networkdata.networkIndex, 1);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id[Index]);
    /* check hostname in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.host_name, chainmaker_host_name[Index]);
    /* check orgID in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.org_id, chainmaker_org_id[Index]);
    /* check url in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.node_url, chainmaker_node_url[Index]);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network success;
*   have creted a persist network in above case and will create all the other 4 persist networks;
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0004CreateMorePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* create persist network */
    for (Index = 2; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* get configuration */
        rtnVal = getChainmakerNetworkConfig(&network_config, Index);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* create persist network , store type must be FLASH */
        rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
        /* check index of persist network ;
            index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM*/
        ck_assert_int_eq(rtnVal, Index);
        /* get network data by index  */
        rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check index in the struct ,must be Index */
        ck_assert_int_eq(networkdata.networkIndex, Index);
        /* check chainId in the struct , must be same to the configuration */
        ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id[Index]);
        /* check hostname in the struct , must be same to the configuration */
        ck_assert_str_eq(networkdata.host_name, chainmaker_host_name[Index]);
        /* check orgID in the struct , must be same to the configuration */
        ck_assert_str_eq(networkdata.org_id, chainmaker_org_id[Index]);
        /* check url in the struct , must be same to the configuration */
        ck_assert_str_eq(networkdata.node_url, chainmaker_node_url[Index]);
        /* free configuration struct */
        chainmakerWalletConfigFree(network_config);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network ;
*  created BOAT_MAX_NETWORK_NUM persist netsorks, can't create another persist network,
*  this case will return error.
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0005Create6PersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatChainmakerNetworkData network_config;
    /* create persist network */
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check result of creating persist network  */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get chainmaker network list ;
*  created BOAT_MAX_NETWORK_NUM persist netsorks.
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0006GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATChainmaker_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have created BOAT_MAX_NETWORK_NUM persist networks,
        num of network in the list must be BOAT_MAX_NETWORK_NUM*/
    ck_assert_int_eq(networkList.networkNum, BOAT_MAX_NETWORK_NUM);
    /* check data of every network in the list */
    for (size_t i = 0; i < BOAT_MAX_NETWORK_NUM; i++)
    {
        /* check index of network , index of persist network begin with 1 */
        ck_assert_int_eq(networkList.networks[i].networkIndex, i + 1);
        /* check chainID of network , chainID[0] used by onetime network,
            chainID of persist network begin with chainID[1]*/
        ck_assert_str_eq(networkList.networks[i].chain_id, chainmaker_chain_id[i + 1]);
        /* check hostname of network */
        ck_assert_str_eq(networkList.networks[i].host_name, chainmaker_host_name[i + 1]);
        /* check hostname of network */
        ck_assert_str_eq(networkList.networks[i].org_id, chainmaker_org_id[i + 1]);
        /* check url of network, demoUrl[0] used by onetime network,
            persist network begin with demoUrl[1] */
        ck_assert_str_eq(networkList.networks[i].node_url, chainmaker_node_url[i + 1]);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete chainmaker network which index equal 3 ;
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0007DeletePersistNetworkSuccess_index3)
{
    BSINT32 rtnVal;
    /* delete network*/
    rtnVal = BoATChainmakerNetworkDelete(3);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get chainmaker network list ;
*  created BOAT_MAX_NETWORK_NUM persist netsorks and deleted the network which index equal 3,
*  there are BOAT_MAX_NETWORK_NUM-1 networks in the list.
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0008GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatChainmakerNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATChainmaker_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have created BOAT_MAX_NETWORK_NUM persist networks,
        and deleted one which index equal 3 , num of network in the list must be BOAT_MAX_NETWORK_NUM - 1*/
    ck_assert_int_eq(networkList.networkNum, BOAT_MAX_NETWORK_NUM - 1);
    /* check data of every network in the list */
    for (size_t i = 0; i < BOAT_MAX_NETWORK_NUM - 1; i++)
    {
        index = i + 1;
        if (index > 2)
        {
            index++;
        }
        /* check index of network , index of persist network begin with 1 */
        ck_assert_int_eq(networkList.networks[i].networkIndex, index);
        /* check chainID of network , chainID[0] used by onetime network,
            chainID of persist network begin with chainID[1]*/
        ck_assert_str_eq(networkList.networks[i].chain_id, chainmaker_chain_id[index]);
        /* check hostname of network */
        ck_assert_str_eq(networkList.networks[i].host_name, chainmaker_host_name[index]);
        /* check orgID of network */
        ck_assert_str_eq(networkList.networks[i].org_id, chainmaker_org_id[index]);
        /* check url of network, demoUrl[0] used by onetime network,
            persist network begin with demoUrl[1] */
        ck_assert_str_eq(networkList.networks[i].node_url, chainmaker_node_url[index]);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network success;
*  created BOAT_MAX_NETWORK_NUM persist netsorks and deleted the network which index equal 3,
*   if create new persist network, index of the new persist network must be 3.
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0009CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 3;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* create persist network */
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index must be 3 */
    ck_assert_int_eq(rtnVal, 3);
    /* get network data by index , index = 3 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 3);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 3 */
    ck_assert_int_eq(networkdata.networkIndex, 3);
    /* check chainId in the struct , must be same to the  configuration */
    ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id[Index]);
    /* check chainId in the struct , must be same to the  configuration */
    ck_assert_str_eq(networkdata.host_name, chainmaker_host_name[Index]);
    /* check chainId in the struct , must be same to the  configuration */
    ck_assert_str_eq(networkdata.org_id, chainmaker_org_id[Index]);
    /* check url in the struct , must be same to the  configuration */
    ck_assert_str_eq(networkdata.node_url, chainmaker_node_url[Index]);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime chainmaker network success
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0010CreateOnetimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id[Index]);
    /* check hostname in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.host_name, chainmaker_host_name[Index]);
    /* check orgid in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.org_id, chainmaker_org_id[Index]);
    /* check url in the struct , must be same to the configuration */
    ck_assert_str_eq(networkdata.node_url, chainmaker_node_url[Index]);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
    /* delete the onetime networl */
    BoATChainmakerNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get persist chainmaker networks by index;
*   have creted BOAT_MAX_NETWORK_NUM persist networks in above case and
*   get network data by index from 1 to BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0011GetNetworkByIndex)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatChainmakerNetworkData networkdata;
    /* create persist network */
    for (Index = 1; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* get network data by index  */
        rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check index in the struct ,must be Index */
        ck_assert_int_eq(networkdata.networkIndex, Index);
        /* check chainId in the struct , must be same to the  configuration */
        ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id[Index]);
        /* check hostname in the struct , must be same to the  configuration */
        ck_assert_str_eq(networkdata.host_name, chainmaker_host_name[Index]);
        /* check orgid in the struct , must be same to the  configuration */
        ck_assert_str_eq(networkdata.org_id, chainmaker_org_id[Index]);
        /* check url in the struct , must be same to the  configuration */
        ck_assert_str_eq(networkdata.node_url, chainmaker_node_url[Index]);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete all the persist networks ;
*   have creted BOAT_MAX_NETWORK_NUM persist networks in above case and
*   delete network  by index from 1 to BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0012DeleteAllNetworkSuccess)
{
    BSINT32 rtnVal;
    for (size_t i = 1; i <= BOAT_MAX_NETWORK_NUM; i++)
    {
        /* delete network*/
        rtnVal = BoATChainmakerNetworkDelete(i);
        /* check the result */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get chainmaker network list ;
*  deleted all the netsorks ,have no networks in the list.
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0013GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatChainmakerNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATChainmaker_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have no networks,
        num of network in the list must be 0*/
    ck_assert_int_eq(networkList.networkNum, 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create all the chainmaker networks success;
*  create a onetime network and BOAT_MAX_NETWORK_NUM persist networks.
* @return
****************************************************************************************
*/
START_TEST(test_002chainmakerNetwork_0014CreateAllNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* create all networks */
    for (Index = 0; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* get configuration */
        rtnVal = getChainmakerNetworkConfig(&network_config, Index);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        if (Index == 0)
        {
            /* create onetime network , store type must be RAM */
            rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
        }
        else
        {
            /* create persist network , store type must be FLASH */
            rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
        }
        /* check index */
        ck_assert_int_eq(rtnVal, Index);
        /* get network data by index  */
        rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check index in the struct ,must be Index */
        ck_assert_int_eq(networkdata.networkIndex, Index);
        /* check chainId in the struct , must be same to the  configuration */
        ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id[Index]);
        /* check hostname in the struct , must be same to the  configuration */
        ck_assert_str_eq(networkdata.host_name, chainmaker_host_name[Index]);
        /* check orgid in the struct , must be same to the  configuration */
        ck_assert_str_eq(networkdata.org_id, chainmaker_org_id[Index]);
        /* check url in the struct , must be same to the  configuration */
        ck_assert_str_eq(networkdata.node_url, chainmaker_node_url[Index]);
        /* free configuration struct */
        chainmakerWalletConfigFree(network_config);
    }
}
END_TEST

Suite *make_chainmakerNetworkUtTest_suite(void)
{
    /* Create Suite */
    Suite *s_networkcreate = suite_create("networkCreat");

    /* Create test cases */
    TCase *tc_networkCreat_api = tcase_create("networkCreat_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_networkcreate, tc_networkCreat_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0001CreateOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0002CreateMoreOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0003CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0004CreateMorePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0005Create6PersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0006GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0007DeletePersistNetworkSuccess_index3);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0008GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0009CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0010CreateOnetimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0011GetNetworkByIndex);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0012DeleteAllNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0013GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0014CreateAllNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002chainmakerNetwork_0012DeleteAllNetworkSuccess);

    return s_networkcreate;
}