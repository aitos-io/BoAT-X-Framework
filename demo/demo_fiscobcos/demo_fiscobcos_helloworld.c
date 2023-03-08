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

#include "HelloWorld.h"


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
 * "78a42562c1d19843fd6f5a0f07de0206fdcf2a682c5e0a9a814019abb531da3a"
 */
const BCHAR *pkcs_demoKey =  "-----BEGIN PRIVATE KEY-----\n"
"MIGEAgEAMBAGByqGSM49AgEGBSuBBAAKBG0wawIBAQQgzZK7QjW1qEm2daA3Pul1\n"
"VOhdZUlLUCdbCYfsMPOTLJGhRANCAAQs7hq3lwEwUk/2KN0XgX77fBDY7EnfvkRK\n"
"IVnzzKgkSWxrazOlB19SiFYJ1tq11BVisJFSwOfGWH0VG9UJykC/\n"
"-----END PRIVATE KEY-----\n";

/**
 * native demo key
 */
const BCHAR *native_demoKey = "0xcd92bb4235b5a849b675a0373ee97554e85d65494b50275b0987ec30f3932c91";

/**
 * test node url
 */
const BCHAR *demoUrl = "http://127.0.0.1:8545";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0x201e148616556b1f30c78961a137e3534e6c263f";


BoatFiscobcosWallet *g_fiscobcos_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;


__BOATSTATIC BOAT_RESULT fiscobcos_createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

	/* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = binFormatKey;
        //UtilityHexToBin(keypair_config.prikey_content, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #endif

	/* create fiscobcos wallet */
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
    BoatFiscobcosNetworkConfig network_config = {0};

    strncpy(network_config.node_url_str, demoUrl, BOAT_FISCOBCOS_NODE_URL_MAX_LEN - 1);

	/* create platone network */
#if defined(USE_ONETIME_WALLET)
    result = BoATFiscobcosNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoATFiscobcosNetworkCreate( &network_config, BOAT_STORE_TYPE_FLASH);
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

BOAT_RESULT fiscobcos_helloworld(BoatFiscobcosWallet *wallet_ptr)
{
    BCHAR           *result_str;
    BoatFiscobcosTx tx_ctx;
    BOAT_RESULT     result;
    
    /* Set Contract Address */
    result = BoatFiscobcosTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, 
								 "0x11E1A300", //gasprice
							     "0x33333333", //gaslimit
							     (BCHAR *)demoRecipientAddress, //recipient
								 "0x01", //chainid
								 "0x01"  //groupid
								);
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "BoatFiscobcosTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    result_str = HelloWorld_set(&tx_ctx, "HELLO FISCOBCOS!");
    if (result_str == NULL)
	{
        //BoatLog(BOAT_LOG_NORMAL, "HelloWorld_set failed: %s.", result_str);
		return BOAT_ERROR;
    }
	//BoatLog(BOAT_LOG_NORMAL, "set returns: %s", result_str);
    
    result_str = HelloWorld_get(&tx_ctx);
    if (result_str == NULL)
	{
        //BoatLog(BOAT_LOG_NORMAL, "HelloWorld_get failed: %s.", result_str);
		return BOAT_ERROR;
    }
	//BoatLog(BOAT_LOG_NORMAL, "get returns: %s", result_str);
	
    return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
	/* step-2: create fiscobcos wallet */
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = fiscobcos_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "fiscobcosWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, fiscobcos_demo_catch);
	}
    result = createNetwork();
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "fiscobcosWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, fiscobcos_demo_catch);
	}
    g_fiscobcos_wallet_ptr = BoatFiscobcosWalletInit(keypairIndex,networkIndex);
    if(g_fiscobcos_wallet_ptr == NULL){
        // BoatLog(BOAT_LOG_NORMAL,"BoatPlatoneWalletInit fail");
        boat_throw(BOAT_ERROR, fiscobcos_demo_catch);
    }
    
	/* step-3: execute 'fiscobcos_call_helloworld' */
	result = fiscobcos_helloworld(g_fiscobcos_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "fiscobcos helloworld access Failed: %d.", result);
    }
	else
	{
        //BoatLog(BOAT_LOG_NORMAL, "fiscobcos helloworld access Passed.");
    }
	boat_catch(fiscobcos_demo_catch)
    {
    }
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
