/******************************************************************************
Copyright (C) 2018-2020 aitos.io

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "boatiotsdk.h"
#include "my_contract.h"
#include "softap_api.h"
#include "boatconfig.h"
#include "protocolapi/api_platone.h"

#include "pem.h"

static int g_boat_task_Handle = -1;
BoatPlatoneWallet *g_platone_wallet_ptr;
//#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE
#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS
#define BOAT_TEST
#define USE_ONETIME_WALLET
//#define USE_CREATE_PERSIST_WALLET
//#define USE_LOAD_PERSIST_WALLET

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
const BCHAR *demoUrl = "http://116.236.47.90:7545";
//const BCHAR *demoUrl = "http://121.5.173.139:8080/public/eth_gasPrice";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";

typedef struct
{
    unsigned char *buf;     /*!< buffer for decoded data             */
    size_t buflen;          /*!< length of the buffer                */
    unsigned char *info;    /*!< buffer for extra header information */
}
mbedtls_pem_context;

#ifdef BOAT_TEST
BOAT_RESULT platone_createOnetimeWallet()
{
    xy_printf("Ready to run platone_createOnetimeWallet1.\n");
    xy_printf("Ready to run platone_createOnetimeWallet2.\n");
    xy_printf("Ready to run platone_createOnetimeWallet3.\n");
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config = {0};
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
	
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        xy_printf("create one-time wallet failed.\n");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_platone_wallet_ptr = BoatGetWalletByIndex(index);
 
    return BOAT_SUCCESS;
}

BOAT_RESULT platone_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]               = {0};

    (void)binFormatKey; //avoid warning

	/* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        UtilityHex2Bin( binFormatKey,
                    32,
                    "0xe55464c12b9e034ab00f7dddeb01874edcf514b3cd77a9ad0ad8796b4d3b1faf",
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        //wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        //wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32; //length contain terminator
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

    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if(index < BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_platone_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}

BOAT_RESULT platone_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, NULL, sizeof(BoatPlatoneWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}

BOAT_RESULT platone_call_mycontract(BoatPlatoneWallet *wallet_ptr)
{
    BCHAR *result_str;
    BoatPlatoneTx tx_ctx;
    BOAT_RESULT result;
    
    /* Set Contract Address */
    result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
							   "0x333333",
							   (BCHAR *)demoRecipientAddress,
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if (result != BOAT_SUCCESS)
	{
        xy_printf( "BoatPlatoneTxInit fails.\n");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "zzq_HelloWorld");
    if (result_str == NULL)
	{
        xy_printf( "my_contract_cpp_abi_setName failed: %s.\n", result_str);
		return BOAT_ERROR;
    }
	xy_printf("setName returns: %s\n", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
	{
        xy_printf("my_contract_cpp_abi_getName failed: %s.\n", result_str);
		return BOAT_ERROR;
    }
	xy_printf( "getName returns: %s\n", result_str);
	
    return BOAT_SUCCESS;
}


void boat_platone_entry(void)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    int ret=0;
    xy_printf("======= Ready to INIT BoatIotSdk ======\n");
	/* step-1: Boat SDK initialization */
    result = BoatIotSdkInit();
    if(result != BOAT_SUCCESS)
    {
        xy_printf("BoatIotSdkInit failed ,result = %d.\n", result);
        return;

    }
        
    xy_printf("g_check_valid_sim = %d\n",g_check_valid_sim); 

    if((g_check_valid_sim == 1) && (1 == xy_wait_tcpip_ok(2*60)))
    {
       xy_printf("tcp/ip is ok!!!\n");
    }
    else
    {
       xy_printf("xy_wait_tcpip_ok error, ret=%x\n",ret);
       xy_printf("tcp/ip is not ok!!!\n");
       return ;
    }

    /* step-2: create platone wallet */
#if defined(USE_ONETIME_WALLET)	
	xy_printf( ">>>>>>>>>> wallet type: create one-time wallet.");
	result = platone_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
	xy_printf(">>>>>>>>>> wallet type: create persist wallet.");
	result = platone_createPersistWallet("platone.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET)
	xy_printf(">>>>>>>>>> wallet type: load persist wallet.");
	result = platone_loadPersistWallet("platone.cfg");
#else
	xy_printf(">>>>>>>>>> none wallet type selected.");
	return -1;
#endif	
    if( result != BOAT_SUCCESS )
	{
		xy_printf("platone create Wallet failed ,result = %d.\n", result);
		goto end;
	}
    xy_printf("======= platone create Wallet run success ======\n");

	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract( g_platone_wallet_ptr );
    if( result != BOAT_SUCCESS )
	{
        xy_printf("platone_call_mycontract failed , result = %d.\n", result);
        goto end;
    }
    xy_printf("======= platone_call_mycontract run success ======\n");

end:	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

}

#endif

static void* boat_task_entry(void *args)
{
    xy_work_lock(1);
    xy_printf("boatiotsdk  boat_task_entry!!! \n");
    xy_sleep(30000);

#ifdef BOAT_TEST
    boat_platone_entry();
#endif
    while(1)
    {
        xy_sleep(1000);
        xy_printf("Boat demo end!!!\n");
    }

    xy_work_unlock();
	xy_TaskDelete(g_boat_task_Handle);
    return (void *)0;
}

void boat_task_init()
{
    xy_TaskCreate((TaskFunc_t)boat_task_entry, "boat task", 20 * 1024, NULL, 10, &g_boat_task_Handle);
}

/*
* undef DEMO_TEST
* make all error, so add next code
*/
#ifndef DEMO_TEST
int at_encode_thread_id = -1;

void at_encode_creat_thread(char *encode_buf)
{
}
#endif

