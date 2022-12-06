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
 * "fcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca
 * "
 */
const BCHAR *pkcs_demoKey =  "-----BEGIN EC PRIVATE KEY-----\n"
                             "MHQCAQEEIPz212cG5mJQ26zJgnvEJzIe25VC1Yp0pnYkslOWBGXKoAcGBSuBBAAK\n"
                             "oUQDQgAEMU/3IAjKpQc8XdURIGQZZJQRHZhPDkp80ahiRAM7KKV9Gmn699pei5fL\n"
                             "qZlYLvlxdQJsoh2IPyObgGr87gBT7w==\n"
                             "-----END EC PRIVATE KEY-----\n";
/*** native demo key ***/
const BCHAR *native_demoKey = "0xae6ae8e5ccbfb04590405997ee2d52d2b330726137b875053c36d94e974d162f";

/*** test node url ***/
const BCHAR * demoUrl = "127.0.0.1:22000";

/*** transfer recipient address ***/
const BCHAR *demoRecipientAddress = "0xf17f52151EbEF6C7334FAD080c5704D77216b732";

// #define USE_ONETIME_WALLET
#define PRIVATE_TRUE  1
#define PRIVATE_FALSE 0

BoatQuorumWallet *g_quorum_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;


__BOATSTATIC BOAT_RESULT quorum_createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]               = {0};

    (void ) binFormatKey;
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



    /* create quorum keypair */
#if defined(USE_ONETIME_WALLET)
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypairIndex = result;
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createNetwork()
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatQuorumNetworkConfig network_config = {0};

    network_config.chain_id             = 10;
    network_config.eip155_compatibility = BOAT_TRUE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_QUORUM_NODE_URL_MAX_LEN - 1);

	/* create platone network */
#if defined(USE_ONETIME_WALLET)
    result = BoatQuorumNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatQuorumNetworkCreate( &network_config, BOAT_STORE_TYPE_FLASH);
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


BOAT_RESULT quorumGetBalance(BoatQuorumWallet *wallet_ptr)
{
    BCHAR *cur_balance_wei = NULL;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};

    cur_balance_wei = BoatQuorumWalletGetBalance(wallet_ptr, NULL);
	result          = BoatQuorumParseRpcResponseStringResult(cur_balance_wei, &parse_result);

    if (parse_result.field_ptr != NULL) 
    {
        BoatFree(parse_result.field_ptr);
    }
	
	return result;
}

BOAT_RESULT quorumTransfer(BoatQuorumWallet *wallet_ptr)
{
    BOAT_RESULT result;
    BoatQuorumTx tx_ctx;
    
    /* Set Recipient Address */
    result = BoatQuorumTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, PRIVATE_FALSE, NULL, "0x333333", (BCHAR *)demoRecipientAddress, NULL);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatQuorumTxInit failed.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    
	/* 0xDE0B6B3A7640000: 1ETH or 1e18 wei, value */
	/* 0x2386F26FC10000: 0.01ETH or 1e16 wei, value */
    result = BoatQuorumTransfer(&tx_ctx, "0xDE0B6B3A7640000");
    return result;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;


	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: create quorum wallet */
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = quorum_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "quorumWalletPrepare_create failed : %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
	}
    result = createNetwork(); 
    if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "quorumWalletPrepare_create failed : %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
	}
    g_quorum_wallet_ptr = BoatQuorumWalletInit(keypairIndex,networkIndex);
    if(g_quorum_wallet_ptr == NULL){
        // BoatLog(BOAT_LOG_NORMAL,"BoatPlatoneWalletInit fail");
        boat_throw(BOAT_ERROR, ethereum_trans_demo_catch);
    }
    
	/* step-3: execute balance transfer */
	result = quorumGetBalance(g_quorum_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        BoatLog(BOAT_LOG_NORMAL, "quorumGetBalance Failed: %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
    }

	result = quorumTransfer(g_quorum_wallet_ptr);    
    if (result != BOAT_SUCCESS)
	{
        BoatLog(BOAT_LOG_NORMAL, "quorumTransfer Failed: %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
    }

	result = quorumGetBalance(g_quorum_wallet_ptr);
    
    if (result != BOAT_SUCCESS)
	{
        BoatLog(BOAT_LOG_NORMAL, "CaseEthereum Failed: %d.", result);
    }
	
    boat_catch(ethereum_trans_demo_catch)
    {

    }	
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return result;
}
