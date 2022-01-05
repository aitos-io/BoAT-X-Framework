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


BoatEthWallet *g_ethereum_wallet_ptr;

#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT ethereum_createOnetimeWallet()
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

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
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

	/* create ethereum wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, NULL, &wallet_config, sizeof(BoatEthWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_ethereum_wallet_ptr = BoatGetWalletByIndex(index);
    
    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT ethereum_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatEthWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

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
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

	/* create ethereum wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, wallet_name, &wallet_config, sizeof(BoatEthWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_ethereum_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT ethereum_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create ethereum wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM, wallet_name, NULL, sizeof(BoatEthWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_ethereum_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

BOAT_RESULT ethereum_call_ReadStore(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT32 index;
    BoatEthTx tx_ctx;
    BOAT_RESULT result;
    BoatFieldVariable prase_result = {NULL, 0};
    
    /* Set Contract Address */
    result = BoatEthTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   (BCHAR *)demoRecipientAddress);

    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "BoatEthTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    
    result_str = StoreRead_saveList(&tx_ctx, (BUINT8*)"HelloWorld");
    //BoatLog(BOAT_LOG_NORMAL, "saveList returns: %s", result_str);
    
    if (result_str != NULL)
    {
        result_str = StoreRead_readListLength(&tx_ctx);
        result = BoatEthPraseRpcResponseStringResult(result_str, &prase_result);
        if (result == BOAT_SUCCESS && result_str != NULL)
        {
            //BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);
            
            UtilityHexToBin((BUINT8*)index_u256_big, sizeof(index_u256_big),
							(BCHAR*)prase_result.field_ptr, TRIMBIN_TRIM_NO, BOAT_FALSE);
            UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));
            list_len = index_u256_big[0];      
            for (index = 0; index < list_len; index++)
            {
                result_str = StoreRead_readListByIndex(&tx_ctx, index);
                result     = BoatEthPraseRpcResponseStringResult(result_str, &prase_result);
                if (result == BOAT_SUCCESS && result_str != NULL)
                {
                    //BoatLog(BOAT_LOG_NORMAL, "readListByIndex returns: %s", prase_result.field_ptr);
                }
                else 
                {
                    return BOAT_ERROR;
                }
            }
        }
    }

    return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
    /* step-2: create ethereum wallet */
#if defined(USE_ONETIME_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = ethereum_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = ethereum_createPersistWallet("eth.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET) 
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
    result = ethereum_loadPersistWallet("eth.cfg");
#else
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
    //return -1;
    result = BOAT_ERROR;
#endif
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed: %d.", result);
		//return -1;
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    
    /* step-3: execute 'ethereum_call_ReadStore' */
    result = ethereum_call_ReadStore(g_ethereum_wallet_ptr);
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_NORMAL, "ethereum readStore access Failed: %d.", result);
    }
    else
    {
        //BoatLog(BOAT_LOG_NORMAL, "ethereum readStore access Passed.");
    }
    boat_catch(ethereum_storeread_demo_catch)
    {
    }
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return result;
}
