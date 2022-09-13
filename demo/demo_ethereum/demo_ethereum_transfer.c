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
 * "fcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca"
 */
const BCHAR *pkcs_demoKey =  "-----BEGIN EC PRIVATE KEY-----\n"
                             "MHQCAQEEIJFHEUCiikP/iCuftqsvWt1kN+q3YxUNsP91gK2rSOhVoAcGBSuBBAAKoUQDQgAEFMuZrEghYdcix5oEP005i1qiKayJKe+ag4kQs85aNCx6tGMpqawC6JhdZi8r3FZLMVx1+hk+4rHLY3qS2TCCwg==\n"
                             "-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
const BCHAR *native_demoKey = "0x70861e08674f83ffa9f16f5fb1afe7f63dc4261d2ecc4b66ce7c983ae3a5c8df";

/**
 * test node url
 */

const BCHAR * demoUrl = "http://127.0.0.1:7545";


/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0x4BeC3cDD520B7985067219F6f596EF7a55Ee5963";

BoatEthWallet *g_ethereum_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;

#if defined(USE_ONETIME_WALLET) 
__BOATSTATIC BOAT_RESULT ethereum_createOnetimeWallet()
{
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


	/* create ethereum keypair */
    keypairIndex = BoatKeypairCreate( &keypair_config, "keypairOnetime",BOAT_STORE_TYPE_RAM);
    if (keypairIndex < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    BoatLog(BOAT_LOG_NORMAL," creat keypair index = %d ",keypairIndex);
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createOnetimeNetwork()
{
    BoatEthNetworkConfig network_config = {0};

    network_config.chain_id             = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

	/* create ethereum wallet */
    networkIndex = BoatEthNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);
    if (networkIndex < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    BoatLog(BOAT_LOG_NORMAL," creat network index = %d ",networkIndex);
    return BOAT_SUCCESS;
}



#endif

#if defined(USE_CREATE_PERSIST_WALLET)

__BOATSTATIC BOAT_RESULT ethereum_createPersistWallet(BCHAR *keypair_name)
{
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

    // BoatLog(BOAT_LOG_NORMAL,"prikey genmode = %d ",keypair_config.prikey_genMode);
	/* create ethereum keypair */
    keypairIndex = BoatKeypairCreate( &keypair_config , keypair_name,BOAT_STORE_TYPE_FLASH);
    if (keypairIndex < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    BoatLog(BOAT_LOG_NORMAL," creat keypair index = %d ",keypairIndex);


    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createPersistNetwork(void)
{
    BoatEthNetworkConfig network_config = {0};

    network_config.chain_id             = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

	/* create ethereum wallet */
    networkIndex = BoatEthNetworkCreate( &network_config, BOAT_STORE_TYPE_FLASH);
    if (networkIndex < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    BoatLog(BOAT_LOG_NORMAL," creat network index = %d ",networkIndex);
    return BOAT_SUCCESS;
}


#endif


BOAT_RESULT ethereumGetBalance(BoatEthWallet *wallet_ptr)
{
    //BCHAR *balance_wei;
    BCHAR *cur_balance_wei = NULL;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};

    cur_balance_wei = BoatEthWalletGetBalance(wallet_ptr, NULL);
	result          = BoatEthParseRpcResponseStringResult(cur_balance_wei, &parse_result);
    //BoatLog(BOAT_LOG_NORMAL, "Balance: %s wei", parse_result.field_ptr);
    if(parse_result.field_ptr != NULL){
        BoatFree(parse_result.field_ptr);
    }
	if (result == BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_NORMAL, "BoatEthWalletGetBalance returns: %s", parse_result.field_ptr);
	}
	else
	{
		return result;
	}


    return BOAT_SUCCESS;
}


BOAT_RESULT ethereumTransfer(BoatEthWallet *wallet_ptr)
{
    BOAT_RESULT result;
    BoatEthTx tx_ctx;
    
    /* Set Recipient Address */
    result = BoatEthTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   (BCHAR *)demoRecipientAddress);

    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "BoatEthTxInit failed.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    
	/* 0xDE0B6B3A7640000: 1ETH or 1e18 wei, value */
	/* 0x2386F26FC10000: 0.01ETH or 1e16 wei, value */
    result = BoatEthTransfer(&tx_ctx, "0x2386F26FC10000");

    return result;
}

int main(int argc, char *argv[])
{
	BOAT_RESULT result = BOAT_SUCCESS;
    BoatIotKeypairContext walletList;
	boat_try_declare;
    BUINT8 prikey[] = {0x91,0x47,0x11,0x40,0xa2,0x8a,0x43,0xff,0x88,0x2b,0x9f,0xb6,0xab,0x2f,0x5a,0xdd,0x64,0x37,0xea,0xb7,0x63,0x15,0x0d,0xb0,0xff,0x75,0x80,0xad,0xab,0x48,0xe8,0x55};
    BUINT8 pubkey[64] = {0};
    BUINT32 len = 0;
    result = BoAT_Common_getPubkey(BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1,BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE,prikey,sizeof(prikey),pubkey,&len);
    BoatLog(BOAT_LOG_NORMAL,"get pubkey result = %d ",result);
    BoatLog_hexdump(BOAT_LOG_NORMAL,"pubkey : ",pubkey,len);
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
	/* step-2: create ethereum wallet */
#if defined(USE_ONETIME_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
	result = ethereum_createOnetimeWallet();
    result = createOnetimeNetwork();
#elif defined(USE_CREATE_PERSIST_WALLET)
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
	result = ethereum_createPersistWallet("eth.cfg");
    result = createPersistNetwork();
#else
	//BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
	//return -1;
    result = BOAT_ERROR;
#endif	
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, ethereum_trans_demo_catch);
	}
    result = BoATKeypair_GetKeypairList(&walletList);
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "get wallet list failed: %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
	}

    BoatLog(BOAT_LOG_NORMAL,"eth wallet init begin num = %d",walletList.keypairNum);
    BoatLog(BOAT_LOG_NORMAL,"eth wallet init walletIndex = %d ,networkIndex = %d ",keypairIndex,networkIndex);
    g_ethereum_wallet_ptr = BoatEthWalletInit(keypairIndex,networkIndex);
    if(g_ethereum_wallet_ptr == NULL){
        BoatLog(BOAT_LOG_NORMAL,"BoatEthWalletInit fail");
        boat_throw(BOAT_ERROR, ethereum_trans_demo_catch);
    }
    BoatLog(BOAT_LOG_NORMAL,"eth wallet init OK");
    BoatLog(BOAT_LOG_NORMAL,"eth wallet url = %s ",g_ethereum_wallet_ptr->network_info.node_url_str);
    
	/* step-3: execute balance transfer */
	result = ethereumGetBalance(g_ethereum_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "ethereumGetBalance Failed: %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
    }
	result = ethereumTransfer(g_ethereum_wallet_ptr);
    
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "ethereumTransfer Failed: %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
    }
	result = ethereumGetBalance(g_ethereum_wallet_ptr);
    
    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "CaseEthereum Failed: %d.", result);
    }
	else
	{
        //BoatLog(BOAT_LOG_NORMAL, "CaseEthereum Passed.");
    }
	
    boat_catch(ethereum_trans_demo_catch)
    {
    }
    BoatEthWalletDeInit(g_ethereum_wallet_ptr);
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return result;
}
