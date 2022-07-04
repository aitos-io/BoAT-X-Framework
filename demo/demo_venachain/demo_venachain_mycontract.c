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
#include "mycontract.cpp.abi.h"
#include "boatiotsdk.h"
#include "boatconfig.h"
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
const BCHAR *native_demoKey = "0xfcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca";

/**
 * test node url
 */
//const BCHAR *demoUrl = "http://116.236.47.90:7545";
const BCHAR *demoUrl = "http://127.0.0.1:6791";
/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0x344b541bb88Cc332d8f6aC7e57c2E3931d45B4B7"; //sol contract
//const BCHAR *demoRecipientAddress = "0x42267f36f2c5b1610bfb253cf044ad0bcd9a5b47";//wasm contract

BoatVenachainWallet *g_venachain_wallet_ptr;


#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT venachain_createOnetimeWallet()
{
    BSINT32 index;
    BoatVenachainWalletConfig wallet_config;
    BUINT8 binFormatKey[32]               = {0};

    memset(&wallet_config,0,sizeof(wallet_config));

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
	
    wallet_config.chain_id             = 300;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

	/* create venachain wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_VENACHAIN, NULL, &wallet_config, sizeof(BoatVenachainWalletConfig));
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_venachain_wallet_ptr = BoatGetWalletByIndex(index);
    
    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT venachain_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatVenachainWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]               = {0};

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

    wallet_config.chain_id             = 300;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

	/* create venachain wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_VENACHAIN, wallet_name, &wallet_config, sizeof(BoatVenachainWalletConfig));
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_venachain_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT venachain_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create venachain wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_VENACHAIN, wallet_name, NULL, sizeof(BoatVenachainWalletConfig));
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_venachain_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

BOAT_RESULT venachain_call_mycontract(BoatVenachainWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatVenachainTx tx_ctx;
    BOAT_RESULT result;
//    venachain_nodesResult result_out = {0,NULL};

    /* Set Contract Address */
    result = BoatVenachainTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
							   "0x333333",
							   (BCHAR *)demoRecipientAddress);

    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "BoatVenachainTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    
    result_str = mycontract_cpp_abi_store(&tx_ctx, "hello_venachain!");//sol contradct
    //result_str = mycontract_cpp_abi_setName(&tx_ctx, "hello_venachain!!");//wasm contract
    if (result_str == NULL)
	{
        //BoatLog(BOAT_LOG_NORMAL, "my_contract_cpp_abi_setName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_NORMAL, "store returns: %s", result_str);
    
    result_str = mycontract_cpp_abi_retrieve(&tx_ctx);//sol contract
    //result_str = mycontract_cpp_abi_getName(&tx_ctx);//wasm contract
    if (result_str == NULL)
	{
        //BoatLog(BOAT_LOG_NORMAL, "my_contract_cpp_abi_getName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_NORMAL, "retrieve returns: %s", result_str);
	
    return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
	/* step-2: create venachain wallet */
#if defined(USE_ONETIME_WALLET)	
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = venachain_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
	result = venachain_createPersistWallet("venachain.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
	result = venachain_loadPersistWallet("venachain.cfg");
#else
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
	//return -1;
    result = BOAT_ERROR;
#endif	
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "venachainWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, venachain_demo_catch);
	}
    
	/* step-3: execute 'venachain_call_mycontract' */
	result = venachain_call_mycontract(g_venachain_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "venachain mycontract access Failed: %d.", result);
    }
	else
	{
        //BoatLog(BOAT_LOG_NORMAL, "venachain mycontract access Passed.");
    }
	boat_catch(venachain_demo_catch)
    {
    }
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    BoatLog(BOAT_LOG_NORMAL, "======== over ======" );
    return 0;
}
