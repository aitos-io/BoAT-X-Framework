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

#include "boatplaton.h"
#include "helloworld.h"


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
                             "MHQCAQEEIKCZUqej4lfP1a+MhE+M139WgJzymsUYk0UKffOqwUbDoAcGBSuBBAAK\n"
                             "oUQDQgAEzwOizQ0JTLu25FDpsqsY6D08bLC2qd3D81rGFnnxT4BBLRtKrhOquITJ\n"
                             "AabGZ+8FMprcN70skT0gQ2GjQ6aQ9Q==\n"
                             "-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
//const BCHAR *native_demoKey = "0xa09952a7a3e257cfd5af8c844f8cd77f56809cf29ac51893450a7df3aac146c3";
const BCHAR *native_demoKey = "0xa7f372a0b5b1e985a9ef631ce5d51777b41d534ff71d6211849d9febc9614616";
/**
 * PlatON test network node url
 */
//const BCHAR *demoUrl = "http://35.247.155.162:6789";
const BCHAR *demoUrl = "https://devnet2openapi.platon.network/rpc";

/**
 * PlatON test network human-readable part
 */
const BCHAR *hrp = "lat";

/**
 * transfer recipient address
 */
//const BCHAR *demoRecipientAddress = "lat1y7qathqkx0s8fjqazskrdqw76j7f5mx95ujstt";
const BCHAR *demoRecipientAddress1 = "lat159js9hw63x2y8m65mhgprm3eu9f4c7xmv3lym4";

const BCHAR *demoRecipientAddress2 = "lat1d5d09nnegcv72sj7a33uwn6vlzup9l25nj73k5"; //SOL contract address

BoatPlatONWallet *g_platon_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;



__BOATSTATIC BOAT_RESULT platon_createKeypair(BCHAR *keypairName)
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
        keypair_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #endif

	/* create platon wallet */
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
    BoatPlatONNetworkConfig network_config = {0};

    //network_config.chain_id             = 210309;
    network_config.chain_id             = 2206132;
    network_config.eip155_compatibility = BOAT_TRUE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_PLATON_NODE_URL_MAX_LEN - 1);

	/* create platone network */
#if defined(USE_ONETIME_WALLET)
    result = BoATPlatONNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoATPlatONNetworkCreate( &network_config, BOAT_STORE_TYPE_FLASH);
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

BOAT_RESULT platonGetBalance(BoatPlatONWallet *wallet_ptr)
{
    //BCHAR * balance_wei;
    BCHAR *cur_balance_von = NULL;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};

    cur_balance_von = BoatPlatONWalletGetBalance(wallet_ptr, "lat");
	result          = BoatPlatONParseRpcResponseStringResult(cur_balance_von, &parse_result);
    if (parse_result.field_ptr != NULL)
    {
        BoatFree(parse_result.field_ptr);
        parse_result.field_ptr = NULL;
        parse_result.field_len = 0;
    }
    //BoatLog(BOAT_LOG_NORMAL, "Balance: %s von", parse_result.field_ptr);
	if (result == BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_NORMAL, "BoatPlatONWalletGetBalance returns: %s", parse_result.field_ptr);
	}
	else
	{
		return BOAT_ERROR;
	}


    return BOAT_SUCCESS;
}


BOAT_RESULT platonTransfer(BoatPlatONWallet *wallet_ptr)
{
    BOAT_RESULT result;
    BoatPlatONTx tx_ctx;
    
    /* Set Recipient Address */
    result = BoatPlatONTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						      "0x33333",
						      (BCHAR *)demoRecipientAddress1,
                              hrp);

    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONTxInit failed.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    
	/* 0xDE0B6B3A7640000: 1ETH or 1e18 wei, value */
	/* 0x2386F26FC10000: 0.01ETH or 1e16 wei, value */
    result = BoatPlatONTransfer(&tx_ctx, "0x01");

    return result;
}


BOAT_RESULT call_contract(BoatPlatONWallet *wallet_ptr)
{
    BOAT_RESULT result;
    BoatPlatONTx tx_ctx;

    /* Set Recipient Address */
    result = BoatPlatONTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						      "0x33333",
						      (BCHAR *)demoRecipientAddress2,
                              hrp);

    if (result != BOAT_SUCCESS)
    {
        printf("BoatPlatONTxInit failed.\n");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    
    helloworld_setName(&tx_ctx,"hello!");
 


    helloworld_getName(&tx_ctx);

    return result;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
	/* step-2: create platon wallet */
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = platon_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, platon_demo_catch);
	}
    result = createNetwork();
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, platon_demo_catch);
	}
    g_platon_wallet_ptr = BoatPlatONWalletInit(keypairIndex,networkIndex);
    if(g_platon_wallet_ptr == NULL){
        // BoatLog(BOAT_LOG_NORMAL,"BoatPlatoneWalletInit fail");
        boat_throw(BOAT_ERROR, platon_demo_catch);
    }
    
    //Transfer
	/* step-3: execute balance transfer */
	result = platonGetBalance(g_platon_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "PlatON get balance Failed: %d.", result);
        boat_throw(result, platon_demo_catch);
    }

	result = platonTransfer(g_platon_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "PlatON get balance Failed: %d.", result);
        boat_throw(result, platon_demo_catch);
    }


	result = platonGetBalance(g_platon_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "PlatON get balance Failed: %d.", result);
    }
	else
	{
        //BoatLog(BOAT_LOG_NORMAL, "PlatON get balance success.");
    }

    //Call demo contract
    /* step-3: call contract */
    call_contract(g_platon_wallet_ptr);

    boat_catch(platon_demo_catch)
    {
    }
    /* step-4: Boat wallet Deinitialization */
    BoatPlatONWalletDeInit(g_platon_wallet_ptr);

	/* step-5: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return 0;
}
