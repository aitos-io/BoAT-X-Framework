/**
****************************************************************************************
* @FilePath: demo.c
* @Author: aitos
* @Date: 2022-12-15 15:35:58
* @LastEditors:
* @LastEditTime: 2022-12-15 15:35:58
* @Descripttion:
****************************************************************************************
*/
/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#define OSI_LOG_TAG OSI_MAKE_LOG_TAG('M', 'Y', 'A', 'P')

#include "fibo_opencpu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "boatiotsdk.h"
#include "boatlog.h"

void test_printf(void)
{
    for (int n = 0; n < 10; n++)
    {
        OSI_PRINTFI("boat test %d", n);
        fibo_taskSleep(1000);
    }
}
const BCHAR *chainmaker_client_sign_prikey =
    "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEIDrrKeqtN86CbUOahnx0BgJLjm/jVP0Rl7DHDpS4A7itoAoGCCqGSM49\n"
    "AwEHoUQDQgAE/r7XkJJJ3Lbyh7DNb3HJNvxQJsabtZi84DIwMnK70dUf+VyUpyRr\n"
    "tCyWvVUXDoXBILT0pDzF/6449j246yWfXA==\n"
    "-----END EC PRIVATE KEY-----\n";

const BCHAR *chainmaker_client_sign_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIICXzCCAgSgAwIBAgIIKgcPrw/PYtYwCgYIKoZIzj0EAwIwZDELMAkGA1UEBhMC\n"
    "Q04xEDAOBgNVBAgTB0JlaWppbmcxEDAOBgNVBAcTB0JlaWppbmcxFzAVBgNVBAoT\n"
    "Dm9yZzUuY210ZXN0bmV0MQswCQYDVQQLEwJjYTELMAkGA1UEAxMCY2EwHhcNMjIw\n"
    "NzE4MTE0NjE4WhcNMjMwMTE0MTE0NjE4WjCBijELMAkGA1UEBhMCQ04xEDAOBgNV\n"
    "BAgTB0JlaUppbmcxEDAOBgNVBAcTB0JlaUppbmcxFzAVBgNVBAoTDm9yZzUuY210\n"
    "ZXN0bmV0MQ8wDQYDVQQLEwZjbGllbnQxLTArBgNVBAMTJGphdmFzZGswMS5jbGll\n"
    "bnQuc2lnbi5vcmc1LmNtdGVzdG5ldDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IA\n"
    "BP6+15CSSdy28oewzW9xyTb8UCbGm7WYvOAyMDJyu9HVH/lclKcka7Qslr1VFw6F\n"
    "wSC09KQ8xf+uOPY9uOsln1yjeTB3MA4GA1UdDwEB/wQEAwIGwDApBgNVHQ4EIgQg\n"
    "jgr4w6VkdvtDlLh8gA+36FxuTc0alFnHDQs9uq9TfE4wKwYDVR0jBCQwIoAgQpSw\n"
    "XN4NFYUfRnCX8ZmfXyguJ6vwJyMUUg2h2tR+7rYwDQYDVR0RBAYwBIIAggAwCgYI\n"
    "KoZIzj0EAwIDSQAwRgIhANUuawiuIjU8nVgxL3nnKDnI6gfzVtoP8Opc2D8r1beL\n"
    "AiEAjStmSKD94LnyGHsdU/bz3NDxQjDAivwDn9Z013AzNMw=\n"
    "-----END CERTIFICATE-----\n";

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
const BCHAR *chainmaker_ca_tls_cert = "-----BEGIN CERTIFICATE-----\n"
                                      "MIICSDCCAe6gAwIBAgIIJpmfvV5S33EwCgYIKoZIzj0EAwIwYjELMAkGA1UEBhMC\n"
                                      "Q04xEDAOBgNVBAgTB0JlaWppbmcxEDAOBgNVBAcTB0JlaWppbmcxETAPBgNVBAoT\n"
                                      "CG9yZy1yb290MQ0wCwYDVQQLEwRyb290MQ0wCwYDVQQDEwRyb290MB4XDTIyMDUx\n"
                                      "MDEwNDQ0M1oXDTMyMDkxNDEwNDQ0M1owZDELMAkGA1UEBhMCQ04xEDAOBgNVBAgT\n"
                                      "B0JlaWppbmcxEDAOBgNVBAcTB0JlaWppbmcxFzAVBgNVBAoTDm9yZzEuY210ZXN0\n"
                                      "bmV0MQswCQYDVQQLEwJjYTELMAkGA1UEAxMCY2EwWTATBgcqhkjOPQIBBggqhkjO\n"
                                      "PQMBBwNCAAS1+7yCgRCAG1UzA0UvwlkJgH9bUT03VSmleA7CNXunMjeEOK6/UiVP\n"
                                      "OPL0ea3d9vtZKUCZ0qIJkFUjiGoyjGTvo4GLMIGIMA4GA1UdDwEB/wQEAwIBBjAP\n"
                                      "BgNVHRMBAf8EBTADAQH/MCkGA1UdDgQiBCCTzC3I+UX0JV+VdPw2tVX1efP22/Zu\n"
                                      "/cYIZDH/nf65UjArBgNVHSMEJDAigCAYphLqTPoKnaRWgPAHUrD4iEyNdA+m4K3c\n"
                                      "xR2etMgZWTANBgNVHREEBjAEggCCADAKBggqhkjOPQQDAgNIADBFAiBCoPIRayeB\n"
                                      "02EQdYl2N8MG9jsrCyzk373fQw/2mmlIlQIhAP+u01GdcqLieyO3f6xQIykATkx/\n"
                                      "rN7pJvitJn/eP13y\n"
                                      "-----END CERTIFICATE-----\n";

#if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
const BCHAR *chainmaker_clinet_tls_prikey =
    "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEIEdPlQIJVidJ+ae/yR9aeaQeh4K5PjVe05cC/4LyGm2ioAoGCCqGSM49\n"
    "AwEHoUQDQgAE9CyQrGH8NcVZHQMftJ9DBjqRb3xFtmQLT4QhPyHKU0ZLzrp2BYbX\n"
    "y9Kdyk7TQ+JbKCaFF5eRqb0JTwE3xH7qpg==\n"
    "-----END EC PRIVATE KEY-----\n";

const BCHAR *chainmaker_client_tls_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIICczCCAhqgAwIBAgIIH2JtCaET7gwwCgYIKoZIzj0EAwIwZDELMAkGA1UEBhMC\n"
    "Q04xEDAOBgNVBAgTB0JlaWppbmcxEDAOBgNVBAcTB0JlaWppbmcxFzAVBgNVBAoT\n"
    "Dm9yZzUuY210ZXN0bmV0MQswCQYDVQQLEwJjYTELMAkGA1UEAxMCY2EwHhcNMjIw\n"
    "NzE4MTE0NjE4WhcNMjMwMTE0MTE0NjE4WjCBiTELMAkGA1UEBhMCQ04xEDAOBgNV\n"
    "BAgTB0JlaUppbmcxEDAOBgNVBAcTB0JlaUppbmcxFzAVBgNVBAoTDm9yZzUuY210\n"
    "ZXN0bmV0MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2phdmFzZGswMS5jbGll\n"
    "bnQudGxzLm9yZzUuY210ZXN0bmV0MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE\n"
    "9CyQrGH8NcVZHQMftJ9DBjqRb3xFtmQLT4QhPyHKU0ZLzrp2BYbXy9Kdyk7TQ+Jb\n"
    "KCaFF5eRqb0JTwE3xH7qpqOBjzCBjDAOBgNVHQ8BAf8EBAMCA/gwEwYDVR0lBAww\n"
    "CgYIKwYBBQUHAwIwKQYDVR0OBCIEIHaDj7FKLvcTsaQPiSg0MDZVixgVBiLFkicB\n"
    "cz64PARkMCsGA1UdIwQkMCKAIEKUsFzeDRWFH0Zwl/GZn18oLier8CcjFFINodrU\n"
    "fu62MA0GA1UdEQQGMASCAIIAMAoGCCqGSM49BAMCA0cAMEQCIBOYgu7l19JGZ2n0\n"
    "mMwaN0grYCDEHWYDc+APJ5Wg3vACAiBiLc5xzM8KdU5eV+SDP+4a0JUr16rr+25F\n"
    "N61UcV2tcQ==\n"
    "-----END CERTIFICATE-----\n";
#endif
#endif

BCHAR *chainmaker_node_url = "127.0.0.1:12302";
BCHAR *chainmaker_host_name = "xxxxxx";
BCHAR *chainmaker_chain_id = "xxxxxx";
BCHAR *chainmaker_org_id = "xxxxxx";

BoatChainmakerWallet *g_chaninmaker_wallet_ptr;
BoatChainmakerWallet wallet_config = {0};
BUINT8 keypair_index = 0;
BUINT8 network_index = 0;

__BOATSTATIC BOAT_RESULT chainmaker_create_keypair()
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 binFormatKey[32] = {0};

    (void)binFormatKey; // avoid warning

    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; // length contain terminator

    /* create ethereum keypair */
    BoatLog(BOAT_LOG_NORMAL, "create keypair persist");
    result = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);

    if (result < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypair_index = result;

    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT chainmaker_create_network(void)
{
    BOAT_RESULT index;
    BoatChainmakerNetworkData networkConfig = {0};

    // set user cert context
    networkConfig.client_member_type = BOAT_MEMBER_TYPE_CERT;
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert);
    if (networkConfig.client_sign_cert_content.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert);

    networkConfig.node_url = BoatMalloc(strlen(chainmaker_node_url) + 1);
    networkConfig.host_name = BoatMalloc(strlen(chainmaker_host_name) + 1);
    networkConfig.chain_id = BoatMalloc(strlen(chainmaker_chain_id) + 1);
    networkConfig.org_id = BoatMalloc(strlen(chainmaker_org_id) + 1);

    strcpy(networkConfig.node_url, chainmaker_node_url);
    strcpy(networkConfig.host_name, chainmaker_host_name);
    strcpy(networkConfig.chain_id, chainmaker_chain_id);
    strcpy(networkConfig.org_id, chainmaker_org_id);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    networkConfig.ca_tls_cert_content.length = strlen(chainmaker_ca_tls_cert);
    if (networkConfig.ca_tls_cert_content.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig.ca_tls_cert_content.content, chainmaker_ca_tls_cert);

#if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
    // set tls cert context
    networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert);
    if (networkConfig.client_tls_cert_content.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert);

    networkConfig.client_tls_privkey_data.value_len = strlen(chainmaker_clinet_tls_prikey);
    if (networkConfig.client_tls_privkey_data.value_len > BOAT_CHAINMAKER_PRIKEY_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy((BCHAR *)networkConfig.client_tls_privkey_data.value, chainmaker_clinet_tls_prikey);
#endif
#endif

    BoatLog(BOAT_LOG_NORMAL, "startting create persist wallet");
    index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    network_index = index;

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

    return index;
}

int chainmaker_test(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatChainmakerTx tx_ptr;
    BoatResponseData response_data;
    BoatKeypairPriKeyCtx keypair;
    BoatChainmakerNetworkData network;
    boat_try_declare;
    // fibo_file_delete("boat_wallet_file");
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2 check keypair and network be created or not,
        if not be created, creat the persist keypair or network.
        Only creat one keypair and network ,so the index must be both 1*/
    keypair.keypair_name = NULL;
    result = BoATKeypair_GetKeypairByIndex(&keypair, 1);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "boat chainmaker has no keypair.");
        result = chainmaker_create_keypair();
        if (result < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "boat chainmaker create keypair failed.");
            boat_throw(result, chainmaker_demo_catch);
        }
        if (keypair_index != 1)
        {
            BoatLog(BOAT_LOG_CRITICAL, "boat chainmaker keypair index error.");
            boat_throw(BOAT_ERROR, chainmaker_demo_catch);
        }
    }
    else
    {
        if (keypair.keypair_name != NULL)
        {
            BoatFree(keypair.keypair_name);
        }
        keypair_index = 1;
    }
    network.chain_id = NULL;
    network.host_name = NULL;
    network.node_url = NULL;
    network.org_id = NULL;
    result = BoATChainmaker_GetNetworkByIndex(&network, 1);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "boat chainmaker has no network.");
        result = chainmaker_create_network();
        if (result < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "boat chainmaker create network failed.");
            boat_throw(result, chainmaker_demo_catch);
        }
        if (network_index != 1)
        {
            BoatLog(BOAT_LOG_CRITICAL, "boat chainmaker network index error.");
            boat_throw(BOAT_ERROR, chainmaker_demo_catch);
        }
    }
    else
    {
        if (network.chain_id != NULL)
        {
            BoatFree(network.chain_id);
        }
        if (network.host_name != NULL)
        {
            BoatFree(network.host_name);
        }
        if (network.node_url != NULL)
        {
            BoatFree(network.node_url);
        }
        if (network.org_id != NULL)
        {
            BoatFree(network.org_id);
        }
        network_index = 1;
    }

    BoatLog(BOAT_LOG_CRITICAL, "boat wallet init keypair_index =%d, network_index = %d\n", keypair_index, network_index);

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    if (g_chaninmaker_wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerWalletInit failed.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, chainmaker_demo_catch);
    }

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, &tx_ptr, 0);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerTxInit failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    // /* step-4: add invoke parameters */
    result = BoatChainmakerAddTxParam(&tx_ptr, 6, "time", "1669200547", "file_hash", "a3f485ea14d946168d2a448adea8d47123", "file_name", "sss", NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerAddTxParam() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-5: execute invoke command */
    result = BoatChainmakerContractInvoke(&tx_ptr, "save", "fact", true, &response_data);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerContractInvoke() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }
    BoatLog(BOAT_LOG_CRITICAL, "response code = %d,  message = %s,  contract_result = %s", response_data.code, response_data.message, response_data.contract_result);

    // /* step-6: add query parameters*/
    BoatSleep(2);
    result = BoatChainmakerAddTxParam(&tx_ptr, 2, "file_hash", "a3f485ea14d946168d2a448adea8d47123", NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerAddTxParam() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-7: execute query command*/
    result = BoatChainmakerContractQuery(&tx_ptr, "find_by_file_hash", "fact", &response_data);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerContractQuery() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }
    // BoatLog(BOAT_LOG_CRITICAL, "response code = %d,  message = %s,  contract_result = %s",
    //         response_data.code, response_data.message, response_data.contract_result);
    BoatLog(BOAT_LOG_CRITICAL, "boat response code = %d", response_data.code);
    BoatLog(BOAT_LOG_CRITICAL, "boat response  message = %s", response_data.message);
    BoatLog(BOAT_LOG_CRITICAL, "boat response contract_result = %s", response_data.contract_result);

    boat_catch(chainmaker_demo_catch)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
    }

    /* step-6: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}