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
#include "StoreRead.h"

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
const BCHAR *native_demoKey = "0xf1395a1fc3f74f0c867b61292e28e0f6cc98a095535fd6bf04e4169ebc047e61";

/**
 * test node url
 */
const BCHAR * demoUrl = "http://127.0.0.1:7545";


/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0xDED9ea325f8D657614f0F96444ca9DF1d7E2f27c";


BoatCitaWallet *g_cita_wallet_ptr;

#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT cita_createOnetimeWallet()
{
    BSINT32 index;
    BoatCitaWalletConfig wallet_config;
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

    memset(&wallet_config,0,sizeof(wallet_config));

	/* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS) 
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #endif

    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_CITA_NODE_URL_MAX_LEN - 1);

	/* create cita wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_CITA, NULL, &wallet_config, sizeof(BoatCitaWalletConfig));
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_citaereum_wallet_ptr = BoatGetWalletByIndex(index);
    
    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT cita_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatCitaWalletConfig wallet_config;
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

    memset(&wallet_config,0,sizeof(wallet_config));

	/* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = &binFormatKey[0];
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #endif

    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_CITA_NODE_URL_MAX_LEN - 1);

	/* create cita wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_CITA, wallet_name, &wallet_config, sizeof(BoatCitaWalletConfig));
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_cita_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT cita_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create cita wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_CITA, wallet_name, NULL, sizeof(BoatCitaWalletConfig));
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_cita_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif


BOAT_RESULT cita_call_ReadStore(BoatCitaWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatCitaTx tx_ctx;
    BOAT_RESULT result;

    BoatLog(BOAT_LOG_NORMAL, "cita_call_ReadStore start.");

     /* Set Contract Address */
    result = BoatCitaTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
                           "0x333333",
                           (BCHAR *)demoRecipientAddress);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "BoatCitaTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    BUINT8 set_data[32] = {0};
    set_data[0]= 5;
    result_str = SimpleStorage_set(&tx_ctx, set_data);
    if(result_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "SimpleStorage_set failed: %s.", result_str);
        return BOAT_ERROR;
    }
    
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
#if defined(USE_ONETIME_WALLET)
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = cita_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = cita_createPersistWallet("cita.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET) 
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
    result = cita_loadPersistWallet("cita.cfg");
#else
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
    return -1;
    result = BOAT_ERROR;
#endif
    if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "citaWalletPrepare_create failed: %d.", result);
        boat_throw(result, cita_SimpleStorage_demo_catch);
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
    boat_catch(cita_SimpleStorage_demo_catch)
    {
    }
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return result;
}
