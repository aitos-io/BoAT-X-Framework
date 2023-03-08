/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "mbtk_api.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "api_platone.h"
#include "boatiotsdk.h"
#include "my_contract.cpp.abi.h"

#define TEST_TASK_NAME "TEST_TASK"

const char *native_demoKey = "0xfcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca";

/**
 * test node url
 */
const BCHAR *demoUrl = "http://116.236.47.90:7545";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";

BoatPlatoneWallet *g_platone_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;

int (*user_app_printf)(const char *fmt, ...);

__BOATSTATIC BOAT_RESULT platone_createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]               = {0};

    (void)binFormatKey; //avoid warning

    //user_app_printf(">>>>>>>>>> wallet format: external injection[native].");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = binFormatKey;
    keypair_config.prikey_content.field_len = 32;

	/* create platone wallet */
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_RAM);

    if (result < 0)
	{
        //user_app_printf("create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypairIndex = result;
    
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createOnetimeNetwork(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatPlatoneNetworkConfig network_config = {0};

    network_config.chain_id             = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

	/* create platone network */
    result = BoATPlatoneNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);

    if (result < 0)
	{
        //user_app_printf("create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    networkIndex = result;
    
    return BOAT_SUCCESS;
}

BOAT_RESULT platone_call_mycontract(BoatPlatoneWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    BOAT_RESULT result;
    nodesResult result_out = {0,NULL};

    /* Set Contract Address */
    result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
							   "0x333333",
							   (BCHAR *)demoRecipientAddress,
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if (result != BOAT_SUCCESS)
	{
        //user_app_printf("BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
    if (result_str == NULL)
	{
        //user_app_printf("my_contract_cpp_abi_setName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	//user_app_printf("setName returns: %s", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
	{
        //user_app_printf("my_contract_cpp_abi_getName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	//user_app_printf("getName returns: %s", result_str);
	
    return BOAT_SUCCESS;
}

BOAT_RESULT PlatoneDemo(void)
{
	BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
	/* step-2: create platone wallet */
	//user_app_printf(">>>>>>>>>> wallet type: create one-time wallet.");
	result = platone_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
	{
		 //user_app_printf("platoneWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, platone_demo_catch);
	}    
    result = createOnetimeNetwork();
    if (result != BOAT_SUCCESS)
	{
		 //user_app_printf("platoneWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, platone_demo_catch);
	}
    if (result != BOAT_SUCCESS)
	{
		 //user_app_printf("platoneWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, platone_demo_catch);
	}
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex,networkIndex);
    if(g_platone_wallet_ptr == NULL){
        // user_app_printf("BoatPlatoneWalletInit fail");
        boat_throw(BOAT_ERROR, platone_demo_catch);
    }
	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract(g_platone_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        //user_app_printf("platone mycontract access Failed: %d.", result);
    }
	else
	{
        //user_app_printf("platone mycontract access Passed.");
    }
	boat_catch(platone_demo_catch)
    {
    }
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return result;
}
