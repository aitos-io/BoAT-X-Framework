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
const BCHAR *native_demoKey = "0x8f2a55949038a9610f50fb23b5883af3b4ecb3c3bb792cbcefbd1542c692be63";

/*** test node url ***/
const BCHAR * demoUrl = "127.0.0.1:22000";

/*** transfer recipient address ***/
const BCHAR *demoRecipientAddress = "0xf17f52151EbEF6C7334FAD080c5704D77216b732";

// #define USE_ONETIME_WALLET
#define PRIVATE_TRUE  1
#define PRIVATE_FALSE 0

BoatQuorumWallet *g_quorum_wallet_ptr;


#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT quorum_createOnetimeWallet()
{
    BSINT32 index;
    BoatQuorumWalletConfig wallet_config;
    BUINT8 binFormatKey[32]              = {0};

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

    wallet_config.chain_id             = 1337;
    wallet_config.eip155_compatibility = BOAT_TRUE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_QUORUM_NODE_URL_MAX_LEN - 1);

    /* create quorum wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_QUORUM, NULL, &wallet_config, sizeof(BoatQuorumWalletConfig));
    if (index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_quorum_wallet_ptr = BoatGetWalletByIndex(index);
    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT quorum_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatQuorumWalletConfig wallet_config;
    BUINT8 binFormatKey[32]           = {0};

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

    wallet_config.chain_id                      = 1337;
    wallet_config.eip155_compatibility          = BOAT_TRUE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_QUORUM_NODE_URL_MAX_LEN - 1);

    /* create quorum wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_QUORUM, wallet_name, &wallet_config, sizeof(BoatQuorumWalletConfig));
    if (index < 0)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_quorum_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT quorum_loadPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;

    /* create platon wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_QUORUM, wallet_name, NULL, sizeof(BoatQuorumWalletConfig));
    if (index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_quorum_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

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
        BoatLog(BOAT_LOG_CRITICAL, "BoatEthTxInit failed.");
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

    /* step-2: create ethereum wallet */
#if defined(USE_ONETIME_WALLET)
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = quorum_createOnetimeWallet();

#elif defined(USE_CREATE_PERSIST_WALLET)
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = quorum_createPersistWallet("quorum.cfg");

#elif defined(USE_LOAD_PERSIST_WALLET)
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
    result = quorum_loadPersistWallet("quorum.cfg");
#else
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
    result = BOAT_ERROR;
#endif  

    if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "quorumWalletPrepare_create failed : %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
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
