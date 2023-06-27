/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-04 11:28:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-13 14:37:50
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

 const BCHAR* chainmaker_ca_tls_cert = 
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

BCHAR *chainmaker_node_url = "127.0.0.0:12301";
BCHAR *chainmaker_host_name = "chainmaker.org";
BCHAR *chainmaker_chain_id = "chain1";
BCHAR *chainmaker_org_id = "wx-org1.chainmaker.org";

BCHAR *chainmaker_node_url2 = "127.0.0.1:12301";
BCHAR *chainmaker_host_name2 = "chainmaker.org2";
BCHAR *chainmaker_chain_id2 = "chain2";
BCHAR *chainmaker_org_id2 = "wx-org1.chainmaker.org2";

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
BOAT_RESULT getChainmakerNetworkConfig(BoatChainmakerNetworkData *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
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

    networkConfig->node_url  = BoatMalloc(strlen(chainmaker_node_url) + 1);
    networkConfig->host_name = BoatMalloc(strlen(chainmaker_host_name) + 1);
    networkConfig->chain_id  = BoatMalloc(strlen(chainmaker_chain_id) + 1);
    networkConfig->org_id    = BoatMalloc(strlen(chainmaker_org_id) + 1);

    strcpy(networkConfig->node_url,  chainmaker_node_url);
    strcpy(networkConfig->host_name, chainmaker_host_name);
    strcpy(networkConfig->chain_id,  chainmaker_chain_id);
    strcpy(networkConfig->org_id,    chainmaker_org_id);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1) 
     networkConfig->ca_tls_cert_content.length = strlen(chainmaker_ca_tls_cert);
    if (networkConfig->ca_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig->ca_tls_cert_content.content, chainmaker_ca_tls_cert);

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        //set tls cert context
        networkConfig->client_tls_cert_content.length = strlen(chainmaker_client_tls_cert);
        if (networkConfig->client_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
        {
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        strcpy(networkConfig->client_tls_cert_content.content, chainmaker_client_tls_cert);

        networkConfig->client_tls_privkey_data.value_len = strlen(chainmaker_clinet_tls_prikey);
        if (networkConfig->client_tls_privkey_data.value_len  > BOAT_CHAINMAKER_PRIKEY_MAX_LEN)
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
*  This function get the configuration to create chainmaker network with other url/hostname/chain_id/org_id
* @param[in] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getChainmakerNetworkConfig2(BoatChainmakerNetworkData *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
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


    networkConfig->node_url  = BoatMalloc(strlen(chainmaker_node_url2) + 1);
    networkConfig->host_name = BoatMalloc(strlen(chainmaker_host_name2) + 1);
    networkConfig->chain_id  = BoatMalloc(strlen(chainmaker_chain_id2) + 1);
    networkConfig->org_id    = BoatMalloc(strlen(chainmaker_org_id2) + 1);

    strcpy(networkConfig->node_url,  chainmaker_node_url2);
    strcpy(networkConfig->host_name, chainmaker_host_name2);
    strcpy(networkConfig->chain_id,  chainmaker_chain_id2);
    strcpy(networkConfig->org_id,    chainmaker_org_id2);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1) 
     networkConfig->ca_tls_cert_content.length = strlen(chainmaker_ca_tls_cert);
    if (networkConfig->ca_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig->ca_tls_cert_content.content, chainmaker_ca_tls_cert);

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        //set tls cert context
        networkConfig->client_tls_cert_content.length = strlen(chainmaker_client_tls_cert);
        if (networkConfig->client_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
        {
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        strcpy(networkConfig->client_tls_cert_content.content, chainmaker_client_tls_cert);

        networkConfig->client_tls_privkey_data.value_len = strlen(chainmaker_clinet_tls_prikey);
        if (networkConfig->client_tls_privkey_data.value_len  > BOAT_CHAINMAKER_PRIKEY_MAX_LEN)
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
START_TEST(test_001chainmakerNetwork_0001CreateOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config);
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
    ck_assert_int_eq(strcmp(networkdata.chain_id, chainmaker_chain_id), 0);
    /* check hostname in the struct, must be same to the configuration */
    ck_assert(strcmp(networkdata.host_name, chainmaker_host_name) == 0);
    /* check org_id in the struct, must be same to the configuration*/
    ck_assert(strcmp(networkdata.org_id, chainmaker_org_id) == 0);
    /* check url in the struct , must be same to the configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url, chainmaker_node_url), 0);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
    /* delete the onetime networl */
    BoATChainmakerNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network success
*  index of persist netsorks is from 1 to BOAT_MAX_NETWORK_NUM
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0002CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ,this is the first persist network ,
        so the index must be 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* get network data by index , index = 1 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 1);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 1 */
    ck_assert_int_eq(networkdata.networkIndex, 1);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_int_eq(strcmp(networkdata.chain_id, chainmaker_chain_id2), 0);
    /* check hostname in the struct, must be same to the configuration */
    ck_assert(strcmp(networkdata.host_name, chainmaker_host_name2) == 0);
    /* check org_id in the struct, must be same to the configuration*/
    ck_assert(strcmp(networkdata.org_id, chainmaker_org_id2) == 0);
    /* check url in the struct , must be same to the configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url, chainmaker_node_url2), 0);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
    /* delete the persist network */
    BoATChainmakerNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network fail.
*  signCert in the configuration is empty.
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0003CreatetNetworkFail_signCert_content_empty)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* clear the sign cert content*/
    memset(network_config.client_sign_cert_content.content, 0x00, sizeof(network_config.client_sign_cert_content.content));
    network_config.client_sign_cert_content.length = 0;
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network fail.
*  node_url in the configuration is empty.
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0004CreatetNetworkFail_node_url_empty)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* clear the nodeurl content*/
    memset(network_config.node_url, 0x00, sizeof(network_config.node_url));
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network fail.
*  host_name in the configuration is empty.
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0005CreatetNetworkFail_host_name_empty)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* clear the hostname content*/
    memset(network_config.host_name, 0x00, sizeof(network_config.host_name));
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network fail.
*  chainID in the configuration is empty.
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0006CreatetNetworkFail_chain_id_empty)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* clear the chain_id content*/
    memset(network_config.chain_id, 0x00, sizeof(network_config.chain_id));
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist chainmaker network fail.
*  orgID in the configuration is empty.
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0007CreatetNetworkFail_org_id_empty)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* clear the org_id content*/
    memset(network_config.org_id, 0x00, sizeof(network_config.org_id));
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete onetime chainmaker network success
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0008DeleteOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* delete the onetime network */
    rtnVal = BoATChainmakerNetworkDelete(0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get the onetime network , index of onetime network must be 0  */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data ,
        the onetime network has been deleted ,so return error */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete persist chainmaker network success
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0009DeletePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData network_config;
    BoatChainmakerNetworkData networkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , the index from 1 to BOAT_MAX_NETWORK_NUM
        this is the first persist network ,so the index must be 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* delete the persist network */
    rtnVal = BoATChainmakerNetworkDelete(1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get the persist network , index of onetime network must be 1  */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 1);
    /* check the result of getting network data ,
        the persist network has been deleted ,so return error */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete chainmaker network;
*  There is no network, return error;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0010DeleteNetwork_NoNetwork)
{
    BSINT32 rtnVal;
    for (size_t i = 0; i <= BOAT_MAX_NETWORK_NUM; i++)
    {
        /* delete the network  */
        rtnVal = BoATChainmakerNetworkDelete(i);
        /* have no network ,so return error */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete chainmaker network;
*  index of network from 0 to BOAT_MAX_NETWORK_NUM;
*  If the index exceeds to BOAT_MAX_NETWORK_NUM, boat return error;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0011DeleteNetwork_IndexExceed)
{
    BSINT32 rtnVal;

    /* delete the network  */
    rtnVal = BoATChainmakerNetworkDelete(BOAT_MAX_NETWORK_NUM + 1);
    /* index exceeds to BOAT_MAX_NETWORK_NUM ,so return error */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network list with NULL param;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0012GetNetworkList_ParamNULL)
{
    BSINT32 rtnVal;

    /* get the network list */
    rtnVal = BoATChainmaker_GetNetworkList(NULL);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network list with no network;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0013GetNetworkList_NONetwork)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATChainmaker_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network ;
        have no network and the num must be 0*/
    ck_assert_int_eq(networkList.networkNum, 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network list with only one onetime network;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0014GetNetworkList_OnlyOnetimeNetwork)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkContext networkList;
    BoatChainmakerNetworkData network_config;
    /* creat onetime network */
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get the network list */
    rtnVal = BoATChainmaker_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network ; only one network , num of network must be 1*/
    ck_assert_int_eq(networkList.networkNum, 1);
    /* check index of onetime network in the list , index of onetime must 0 */
    ck_assert_int_eq(networkList.networks[0].networkIndex, 0);
    /* check chainId in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[0].chain_id, chainmaker_chain_id), 0);
    /* check hostname in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[0].host_name, chainmaker_host_name), 0);
    /* check orgID in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[0].org_id, chainmaker_org_id), 0);
    /* check url ,must be same to created */
    ck_assert_int_eq(strcmp(networkList.networks[0].node_url, chainmaker_node_url), 0);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network list with only a onetime network and a persist network;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0015GetNetworkList_OnetimeAndPersistNetwork)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkContext networkList;
    BoatChainmakerNetworkData network_config;
    /* creat onetime network */
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* creat a persist network*/
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ;
        index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network, the index must equal 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* get the network list */
    rtnVal = BoATChainmaker_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network ; a onetime network and a persist network , num of network must be 2*/
    ck_assert_int_eq(networkList.networkNum, 2);
    /* read persist networks and then read the onetime network ;
        there are a onetime network and a persist network ,so list[0] is persist network , list[1] is onetime network*/
    /* check index of persist network in the list , index of persist netwoek must 1 */
    ck_assert_int_eq(networkList.networks[0].networkIndex, 1);
    /* check chainId in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[0].chain_id, chainmaker_chain_id2), 0);
    /* check hostname in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[0].host_name, chainmaker_host_name2), 0);
    /* check orgID in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[0].org_id, chainmaker_org_id2), 0);
    /* check url ,must be same to created */
    ck_assert_int_eq(strcmp(networkList.networks[0].node_url, chainmaker_node_url2), 0);
    /* check index of onetime network in the list , index of onetime netwoek must 0 */
    ck_assert_int_eq(networkList.networks[1].networkIndex, 0);
    /* check chainId in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[1].chain_id, chainmaker_chain_id), 0);
    /* check hostname in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[1].host_name, chainmaker_host_name), 0);
    /* check orgID in the list ,must be same to cteated */
    ck_assert_int_eq(strcmp(networkList.networks[1].org_id, chainmaker_org_id), 0);
    /* check url ,must be same to created */
    ck_assert_int_eq(strcmp(networkList.networks[1].node_url, chainmaker_node_url), 0);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
    /* delete all the networks*/
    BoATChainmakerNetworkDelete(0);
    BoATChainmakerNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network data by index with NULL param;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0016GetNetworkByIndex_ParamNULL)
{
    BSINT32 rtnVal;
    /* get network data by index , the first param set NULL */
    rtnVal = BoATChainmaker_GetNetworkByIndex(NULL, 0);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network data by index with exceeded index;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0017GetNetworkByIndex_IndexExceed)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData networkdata;
    /* get network data by index , but index bigger than BOAT_MAX_NETWORK_NUM */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, BOAT_MAX_NETWORK_NUM + 1);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get onetime network data by index;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0018GetNetworkByIndex_Onetime)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData networkdata;
    BoatChainmakerNetworkData network_config;
    /* creat onetime network */
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* creat a persist network*/
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ;
        index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network, the index must equal 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* get network data by index , index of onetime network must be 0 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 0);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index of the onetime network in the struct , index of onetime netwoek must 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the struct ,must be same to cteated */
    ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id);
    /* check hostname in the struct ,must be same to cteated */
    ck_assert_str_eq(networkdata.host_name, chainmaker_host_name);
    /* check orgID in the struct ,must be same to cteated */
    ck_assert_str_eq(networkdata.org_id, chainmaker_org_id);
    /* check url ,must be same to created */
    ck_assert_str_eq(networkdata.node_url, chainmaker_node_url);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
    /* delete all the networks*/
    BoATChainmakerNetworkDelete(0);
    BoATChainmakerNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get persist network data by index;
* @return
****************************************************************************************
*/
START_TEST(test_001chainmakerNetwork_0019GetNetworkByIndex_Persist)
{
    BSINT32 rtnVal;
    BoatChainmakerNetworkData networkdata;
    BoatChainmakerNetworkData network_config;
    /* creat onetime network */
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* creat a persist network*/
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig2(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ;
        index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network, the index must equal 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* get network data by index , index of persist network must be 1 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&networkdata, 1);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index of the persist network in the struct , index of persist netwoek must 1 */
    ck_assert_int_eq(networkdata.networkIndex, 1);
    /* check chainId in the struct ,must be same to cteated */
    ck_assert_str_eq(networkdata.chain_id, chainmaker_chain_id2);
    /* check hostname in the struct ,must be same to cteated */
    ck_assert_str_eq(networkdata.host_name, chainmaker_host_name2);
    /* check orgId in the struct ,must be same to cteated */
    ck_assert_str_eq(networkdata.org_id, chainmaker_org_id2);
    /* check url ,must be same to created */
    ck_assert_str_eq(networkdata.node_url, chainmaker_node_url2);
    /* free configuration struct */
    chainmakerWalletConfigFree(network_config);
    /* delete all the networks*/
    BoATChainmakerNetworkDelete(0);
    BoATChainmakerNetworkDelete(1);
}
END_TEST

Suite *make_chainmakerNetworkIntfTest_suite(void)
{
    /* Create Suite */
    Suite *s_networkcreate = suite_create("networkCreat");

    /* Create test cases */
    TCase *tc_networkCreat_api = tcase_create("networkCreat_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_networkcreate, tc_networkCreat_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0001CreateOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0002CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0003CreatetNetworkFail_signCert_content_empty);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0004CreatetNetworkFail_node_url_empty);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0005CreatetNetworkFail_host_name_empty);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0006CreatetNetworkFail_chain_id_empty);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0007CreatetNetworkFail_org_id_empty);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0008DeleteOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0009DeletePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0010DeleteNetwork_NoNetwork);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0011DeleteNetwork_IndexExceed);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0012GetNetworkList_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0013GetNetworkList_NONetwork);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0014GetNetworkList_OnlyOnetimeNetwork);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0015GetNetworkList_OnetimeAndPersistNetwork);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0016GetNetworkByIndex_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0017GetNetworkByIndex_IndexExceed);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0018GetNetworkByIndex_Onetime);
    tcase_add_test(tc_networkCreat_api, test_001chainmakerNetwork_0019GetNetworkByIndex_Persist);

    return s_networkcreate;
}