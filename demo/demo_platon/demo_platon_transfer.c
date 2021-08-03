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
const BCHAR *native_demoKey = "0x370ea4b099f43f6afb3dc09741bfebce846e3c6027b48b12eb26261897cd1316";

/**
 * PlatON test network node url
 */
//const BCHAR *demoUrl = "http://47.241.98.219:6789";
const BCHAR *demoUrl = "http://192.168.132.151:7000";

/**
 * PlatON test network human-readable part
 */
const BCHAR *hrp = "lat";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "lat1u89782npf3s0a3e7zlmmwqqq58yhjctuqmmjh9";


BoatPlatONWallet *g_platon_wallet_ptr;

#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT platon_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatONWalletConfig wallet_config = {0};
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

    wallet_config.chain_id             = 100;//210309;
    wallet_config.eip155_compatibility = BOAT_TRUE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATON_NODE_URL_MAX_LEN - 1);

	/* create platon wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATON, NULL, &wallet_config, sizeof(BoatPlatONWalletConfig));
    if (index == BOAT_ERROR)
	{
        //BoatLog( BOAT_LOG_CRITICAL, "create one-time wallet failed." );
        return BOAT_ERROR;
    }
    g_platon_wallet_ptr = BoatGetWalletByIndex(index);
    
    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT platon_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatPlatONWalletConfig wallet_config = {0};
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
        UtilityHexToBin( binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #endif

    wallet_config.chain_id                      = 1;
    wallet_config.eip155_compatibility          = BOAT_TRUE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATON_NODE_URL_MAX_LEN - 1);

	/* create platon wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATON, wallet_name, &wallet_config, sizeof(BoatPlatONWalletConfig));
    if (index == BOAT_ERROR)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR;
    }

    g_platon_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT platon_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create platon wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATON, wallet_name, NULL, sizeof(BoatPlatONWalletConfig));
    if (index == BOAT_ERROR)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR;
    }
    g_platon_wallet_ptr = BoatGetWalletByIndex( index );

    return BOAT_SUCCESS;
}
#endif

BOAT_RESULT platonGetBalance(BoatPlatONWallet *wallet_ptr)
{
    //BCHAR * balance_wei;
    BCHAR * cur_balance_von = NULL;
    BOAT_RESULT result;
    BoatFieldVariable prase_result = {NULL, 0};

    cur_balance_von = BoatPlatONWalletGetBalance(wallet_ptr, "lat1utcm54t2x6c5z9e6mm7menauuqtmzl68hdm3nr");
	result          = BoatPlatONPraseRpcResponseResult(cur_balance_von, "", &prase_result);
	if (result == BOAT_SUCCESS)
	{
		//BoatLog( BOAT_LOG_NORMAL, "BoatPlatONWalletGetBalance returns: %s", prase_result.field_ptr );
	}
	else
	{
		return BOAT_ERROR;
	}

    //BoatLog(BOAT_LOG_NORMAL, "Balance: %s von", prase_result.field_ptr);

    return BOAT_SUCCESS;
}


BOAT_RESULT platonTransfer(BoatPlatONWallet *wallet_ptr)
{
    BOAT_RESULT result;
    BoatPlatONTx tx_ctx;
    
    /* Set Recipient Address */
    result = BoatPlatONTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						      "0x33333",
						      (BCHAR *)demoRecipientAddress,
                              hrp);

    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONTxInit failed.");
        return BOAT_ERROR;
    }
    
	/* 0xDE0B6B3A7640000: 1ETH or 1e18 wei, value */
	/* 0x2386F26FC10000: 0.01ETH or 1e16 wei, value */
    result = BoatPlatONTransfer(&tx_ctx, "0xdbfe49bc6728ffe");

    return result;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 bechaddresslen, i;
    BUINT8 address[50];
	
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
	/* step-2: create platon wallet */
#if defined(USE_ONETIME_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = platon_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
	result = platon_createPersistWallet("platon.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
	result = platon_loadPersistWallet("platon.cfg");
#else
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>> none wallet type selected.");
	return -1;
#endif	
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "platonWalletPrepare_create failed : %d.", result);
		return -1;
	}
    
	/* step-3: execute balance transfer */
	result = platonGetBalance(g_platon_wallet_ptr);
	result = platonTransfer(g_platon_wallet_ptr);
	result = platonGetBalance(g_platon_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "CasePlatON Failed: %d.", result);
    }
	else
	{
        //BoatLog(BOAT_LOG_NORMAL, "CasePlatON Passed.");
    }
    
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
