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
"MIGEAgEAMBAGByqGSM49AgEGBSuBBAAKBG0wawIBAQQgz3wfbqBIV+8EOR49yA4x\n"
"4o6uVigkoU+WwaAKdQe2fZahRANCAAQpe8iOa9nDLr9r7/2aokClH7/Gqy7F+ep/\n"
"SOf94NvnLoQILdWeBzeCotF1BoJyrfnXiMSgDZYexSsFFgNz8dJx\n"
                              "-----END PRIVATE KEY-----\n";

/**
 * native demo key
 */
const BCHAR *native_demoKey = "0xcf7c1f6ea04857ef04391e3dc80e31e28eae562824a14f96c1a00a7507b67d96";

/**
 * test node url
 */
const BCHAR *demoUrl = "http://127.0.0.1:8545";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0xcfafa80763d106702c312ca6719e3f7b6fc8cf1c";


BoatFiscobcosWallet *g_fiscobcos_wallet_ptr;

#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT fiscobcos_createOnetimeWallet()
{
    BSINT32 index;
    BoatFiscobcosWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]                 = {0};

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
        //UtilityHexToBin(wallet_config.prikeyCtx_config.prikey_content, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #endif

    strncpy(wallet_config.node_url_str, demoUrl, BOAT_FISCOBCOS_NODE_URL_MAX_LEN - 1);

	/* create fiscobcos wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, NULL, &wallet_config, sizeof(BoatFiscobcosWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_fiscobcos_wallet_ptr = BoatGetWalletByIndex(index);
    
    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT fiscobcos_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatFiscobcosWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]                 = {0};

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
    #elif (defined USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE) 
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
    
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_FISCOBCOS_NODE_URL_MAX_LEN - 1);

	/* create fiscobcos wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, wallet_name, &wallet_config, sizeof(BoatFiscobcosWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_fiscobcos_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT fiscobcos_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create fiscobcos wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, wallet_name, NULL, sizeof(BoatFiscobcosWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_fiscobcos_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

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
#if defined(USE_ONETIME_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = fiscobcos_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
	result = fiscobcos_createPersistWallet("fiscobcos.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
	result = fiscobcos_loadPersistWallet("fiscobcos.cfg");
#else
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
	//return -1;
    result = BOAT_ERROR;
#endif	
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "fiscobcosWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, fiscobcos_demo_catch);
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
