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
#include "SimpleStorage.h"
#include "boatlog.h"
/**
 * macro used to select wallet type:
 * - USE_ONETIME_WALLET          : create a one-time wallet
 * - USE_CREATE_PERSIST_WALLET   : create a persist wallet
 * - USE_LOAD_PERSIST_WALLET     : load a persist wallet
 * 
 * macro used to select private key format:
 * - USE_PRIKEY_FORMAT_INTERNAL_GENERATION       : key is internal generated
 * - USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS   : key is pkcs format via external injection
 * - USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE : key is native format via external injection
 */

/**
 * PKCS format demo key. The original private key of 'pkcs_demoKey' is 
 * "fcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca"
 */
const BCHAR *pkcs_demoKey =  "-----BEGIN EC PRIVATE KEY-----\n"
                             "MHQCAQEEIPz212cG5mJQ26zJgnvEJzIe25VC1Yp0pnYkslOWBGXKoAcGBSuBBAAK\n"
                             "oUQDQgAEMU/3IAjKpQc8XdURIGQZZJQRHZhPDkp80ahiRAM7KKV9Gmn699pei5fL\n"
                             "qZlYLvlxdQJsoh2IPyObgGr87gBT7w==\n"
                             "-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
const BCHAR *native_demoKey = "0x3ef2627393529fed043c7dbfd9358a4ae47a88a59949b07e7631722fd6959002";

/**
 * test node url
 */
const BCHAR * demoUrl = "http://127.0.0.1:1337";


/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0x776daaa7443599c2ef54fc41b1d26b98d916fead";
const BCHAR *demoChainIdStr       = "0x1";


BoatCitaWallet *g_cita_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;


__BOATSTATIC BOAT_RESULT cita_createKeypair(BCHAR * keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

    /* keypair_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: internal generated.");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[pkcs].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[native].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #endif


    /* create cita keypair */
#if defined(USE_ONETIME_WALLET) 
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypairIndex = result;
    
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createNetwork()
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatCitaNetworkConfig network_config = {0};
    BUINT8 binChainId[32]                = {0};
    BUINT32 bin_len;

    network_config.version = 2;
    bin_len = UtilityHexToBin(binChainId, 32, demoChainIdStr, TRIMBIN_TRIM_NO, BOAT_FALSE);
    if (bin_len > 0)
    {
        memcpy(&network_config.chain_id_buf[32 - bin_len], binChainId, bin_len);
    }

    strncpy(network_config.node_url_buf, demoUrl, BOAT_CITA_NODE_URL_MAX_LEN - 1);

    /* create ciat wallet */
#if defined(USE_ONETIME_WALLET) 
    result = BoATCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoATCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    networkIndex = result;
    
    return BOAT_SUCCESS;
}

BOAT_RESULT cita_call_ReadStore(BoatCitaWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT32 index;
    BoatCitaTx tx_ctx;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};

    /* Set Contract Address */
    result = BoatCitaTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, 
                                 (BCHAR *)demoRecipientAddress, //recipient
                                 10000000  //quota
                                );
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "BoatCitaTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    BUINT8 set_data[32] = {0};
    set_data[0]= 6;
    result_str = SimpleStorage_set(&tx_ctx, set_data);
    if(result_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "SimpleStorage_set failed: %s.", result_str);
        return BOAT_ERROR;
    }
    
    BoatSleep(3);
    result_str = SimpleStorage_get(&tx_ctx);
    if (result_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "SimpleStorage_get %s.", result_str);
        return BOAT_ERROR;
    }
    BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);

    return BOAT_SUCCESS;
}
    
int main(int argc, char *argv[])
{
    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
    /* step-2: create cita wallet */
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = cita_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
    {
         BoatLog(BOAT_LOG_CRITICAL, "citaWalletPrepare_create failed: %d.", result);
        boat_throw(result, cita_storeread_demo_catch);
    }
    result = createNetwork();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "citaWalletPrepare_create failed: %d.", result);
        boat_throw(result, cita_storeread_demo_catch);
    }

    g_cita_wallet_ptr = BoatCitaWalletInit(keypairIndex,networkIndex);
    if (g_cita_wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL,"BoatCitaWalletInit fail");
        boat_throw(BOAT_ERROR, cita_storeread_demo_catch);
    }
    /* step-3: execute 'cita_call_ReadStore' */
    result = cita_call_ReadStore(g_cita_wallet_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "cita readStore access Failed: %d.", result);
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "cita readStore access Passed.");
    }
    boat_catch(cita_storeread_demo_catch)
    {

    }
    BoatCitaWalletDeInit(g_cita_wallet_ptr);

    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return result;
}
