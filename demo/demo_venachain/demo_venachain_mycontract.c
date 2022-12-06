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
const BCHAR *demoRecipientAddress = "0xBDB623f91bBD1B37d17c8ED9d78c70954865247C"; //sol contract
//const BCHAR *demoRecipientAddress = "0x42267f36f2c5b1610bfb253cf044ad0bcd9a5b47";//wasm contract

BoatVenachainWallet *g_venachain_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;



__BOATSTATIC BOAT_RESULT venachain_createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]               = {0};

    (void)binFormatKey; //avoid warning

	/* wallet_config value assignment */
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
	
	/* create venachain keypair */
#if defined(USE_ONETIME_WALLET)
    result = BoatKeypairCreate(&keypair_config, keypairName,BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatKeypairCreate(&keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
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
    BoatVenachainNetworkConfig network_config = {0};

    network_config.chain_id             = 300;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

	/* create venachain wallet */
#if defined(USE_ONETIME_WALLET)
    result = BoatVenachainNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatVenachainNetworkCreate( &network_config, BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    networkIndex = result;
    
    return BOAT_SUCCESS;
}



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
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = venachain_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "venachainWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, venachain_demo_catch);
	}
    result = createNetwork();
	
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "venachainWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, venachain_demo_catch);
	}
    
    g_venachain_wallet_ptr = BoatVenachainWalletInit(keypairIndex,networkIndex);
    if(g_venachain_wallet_ptr == NULL){
        // BoatLog(BOAT_LOG_NORMAL,"BoatEthWalletInit fail");
        boat_throw(BOAT_ERROR, venachain_demo_catch);
    }

	/* step-3: execute 'venachain_call_mycontract' */
	result = venachain_call_mycontract(g_venachain_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        BoatLog(BOAT_LOG_NORMAL, "venachain mycontract access Failed: %d.", result);
    }
	else
	{
        BoatLog(BOAT_LOG_NORMAL, "venachain mycontract access Passed.");
    }
	boat_catch(venachain_demo_catch)
    {
    }

    BoatVenachainWalletDeInit(g_venachain_wallet_ptr);
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    BoatLog(BOAT_LOG_NORMAL, "======== over ======" );
    return 0;
}
