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
#include "boatconfig.h"
#include "boatiotsdk.h"
#include "boatlog.h"
#include "network_ethereum.h"

/**
 * test node url
 */

const BCHAR *demoUrl = "http://192.168.132.190:7545";

// #if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT createOnetimeNetwork()
{
    BSINT32 index;
    BoatEthNetworkConfig network_config = {0};

    network_config.chain_id = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    /* create ethereum wallet */
    index = BoatEthNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    if (index < 0)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    return BOAT_SUCCESS;
}

// #endif

// #if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT createPersistNetwork(void)
{
    BSINT32 index;
    BoatEthNetworkConfig network_config = {0};

    network_config.chain_id = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    /* create ethereum wallet */
    index = BoatEthNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    if (index < 0)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    return BOAT_SUCCESS;
}
// #endif

int main(int argc, char *argv[])
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatEthNetworkContext networkList;
    boat_try_declare;
    BoatLog(BOAT_LOG_NORMAL, "begin to test ");
    result = createOnetimeNetwork();
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = createPersistNetwork();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet create failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
    }
    result = createPersistNetwork();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet create failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
    }
    result = createPersistNetwork();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet create failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
    }
    result = BoATEthNetworkDelete(2);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet delete failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
    }
    result = BoATEth_GetNetworkList(&networkList);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get wallet list failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
    }

    for (size_t i = 0; i < networkList.networkNum; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL, "network[%d].index = %d ", i, networkList.networks[i].networkIndex);
        BoatLog(BOAT_LOG_NORMAL, "network[%d].chainID  = %d ", i, networkList.networks[i].chain_id);
        BoatLog(BOAT_LOG_NORMAL, "network[%d].eip55 = %d ", i, networkList.networks[i].eip155_compatibility);
        BoatLog(BOAT_LOG_NORMAL, "network[%d].url = %s ", i, networkList.networks[i].node_url_str);
    }
    BoATEth_FreeNetworkContext(networkList);

    result = BoATEthNetworkDelete(0);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet delete failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
    }

    result = BoATEth_GetNetworkList(&networkList);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get wallet list failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
    }

    for (size_t i = 0; i < networkList.networkNum; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL, "network[%d].index = %d ", i, networkList.networks[i].networkIndex);
        BoatLog(BOAT_LOG_NORMAL, "network[%d].chainID  = %d ", i, networkList.networks[i].chain_id);
        BoatLog(BOAT_LOG_NORMAL, "network[%d].eip55 = %d ", i, networkList.networks[i].eip155_compatibility);
        BoatLog(BOAT_LOG_NORMAL, "network[%d].url = %s ", i, networkList.networks[i].node_url_str);
    }
    BoATEth_FreeNetworkContext(networkList);

    BoatLog(BOAT_LOG_CRITICAL, "network test success: ");
    boat_catch(ethereum_storeread_demo_catch)
    {
    }
}