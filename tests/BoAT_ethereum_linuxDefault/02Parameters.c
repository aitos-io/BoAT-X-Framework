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
#include "tcase_common.h"

static BoatEthWallet* g_ethereum_wallet_ptr;
static BoatEthWalletConfig wallet_config = {0};
#define USE_ONETIME_WALLET

static BOAT_RESULT ethereumWalletPrepare(void)
{
    BOAT_RESULT index;

    //set user private key context
	wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.prikeyCtx_config.prikey_type	  = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
	wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)ethereum_pkcs_key_buf;
	wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(ethereum_pkcs_key_buf) + 1;

	wallet_config.chain_id             = TEST_ETHEREUM_CHAIN_ID;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, TEST_ETHEREUM_NODE_URL, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    // create wallet
#if defined(USE_ONETIME_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig));
#elif defined(USE_CREATE_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum.cfg", &wallet_config, sizeof(BoatEthWalletConfig));
#elif defined(USE_LOAD_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, "ethereum.cfg", NULL, sizeof(BoatEthWalletConfig));
#else
    return BOAT_ERROR;
#endif
    if (index == BOAT_ERROR)
    {
        return BOAT_ERROR;
    }
    
    g_ethereum_wallet_ptr = BoatGetWalletByIndex(index);
    if (g_ethereum_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

static BOAT_RESULT param_check(BoatEthTx* tx_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (tx_ptr == NULL)
    {
        return BOAT_ERROR;
    }

}


