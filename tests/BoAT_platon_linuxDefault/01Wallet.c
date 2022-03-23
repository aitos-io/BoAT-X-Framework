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
 #include "tcase_platon.h"

#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_PLATON_CHAIN_ID      210309

BOAT_RESULT check_platon_wallet(BoatPlatONWallet *wallet_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    result = strncmp(wallet_ptr->account_info.prikeyCtx.extra_data.value, platon_private_key_buf, strlen(platon_private_key_buf));
    if (result != 0) 
    {
        return result;
    }
    
    result = strncmp(wallet_ptr->network_info.node_url_ptr, TEST_PLATON_NODE_URL, strlen(TEST_PLATON_NODE_URL));
    if (result != 0)
    {
        return result;
    }

    return BOAT_SUCCESS;
}

BoatPlatONWalletConfig get_platon_wallet_settings()
{
    //set user private key context
        
    if (TEST_KEY_TYPE == BOAT_WALLET_PRIKEY_FORMAT_NATIVE)
    {
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(binFormatKey, 32, platon_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    }
    else
    {
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)platon_private_key_buf;
        wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(platon_private_key_buf) + 1;
    }
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;


    wallet_config.chain_id             = TEST_PLATON_CHAIN_ID;
    wallet_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    strncpy(wallet_config.node_url_str, TEST_PLATON_NODE_URL, BOAT_PLATON_NODE_URL_MAX_LEN - 1);
    
    return wallet_config;
}