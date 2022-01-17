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

#define OSI_LOG_TAG OSI_MAKE_LOG_TAG('B', 'O', 'A', 'T')

#include "fibo_opencpu.h"
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

static void prvInvokeGlobalCtors(void)
{
    extern void (*__init_array_start[])();
    extern void (*__init_array_end[])();

    size_t count = __init_array_end - __init_array_start;
    for (size_t i = 0; i < count; ++i)
        __init_array_start[i]();
}

static BOAT_RESULT platone_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BUINT8 binFormatKey[32]               = {0};

    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
    UtilityHexToBin(wallet_config.prikeyCtx_config.prikey_content.field_ptr, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
  
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);


	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig)); // "/nvm/test.txt"
    OSI_LOGI(0, "BoatWalletCreate index=%d", index);

    if (index != BOAT_SUCCESS)
	{
        OSI_LOGI(0, "create one-time wallet failed.");
        return BOAT_ERROR;
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
        OSI_LOGI(0, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "L610 HelloWorld 0602");
    if (result_str == NULL)
	{
        OSI_LOGI(0, "my_contract_cpp_abi_setName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	OSI_LOGI(0, "setName returns: %s", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
	{
        OSI_LOGI(0, "my_contract_cpp_abi_getName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	OSI_LOGI(0, "getName returns: %s", result_str);
	
    return BOAT_SUCCESS;
}

void boat_platone_entry(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    /* step-2: create platone wallet */
    result = platone_createOnetimeWallet();
    //result = platone_loadPersistWallet(PERSIST_WALLET_NAME);
    if (result != BOAT_SUCCESS)
	{
		OSI_LOGI(0, "platoneWalletPrepare_create failed : %d.", result);
		return ;
	}
	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract(g_platone_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        OSI_LOGI(0, "platone mycontract access Failed: %d.", result);
    }
	else
	{
        OSI_LOGI(0, "platone mycontract access Passed.");
    }
	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
}

static void prvThreadEntry(void *param)
{
    OSI_LOGI(0, "application thread enter, param 0x%x", param);
    //srand(100);
    int ret = 0;
    int i = 0;
    
    // Disable sleep
    fibo_setSleepMode(0);
    while (0)//i < 60)
    {
        i++;
        fibo_taskSleep(1000);
        OSI_PRINTFI("OSI_PRINTFI ==== i=%d", i);
        OSI_LOGI(0,"OSI_LOGI ==== i=%d", i);
    }
    i = 0;
#if 1
    fibo_taskSleep(25000);
	OSI_PRINTFI("[%s-%d] osiThreadCurrent() = %p", __FUNCTION__, __LINE__,osiThreadCurrent());
	
	UINT8 ip[50];
    
    #define MAX_PDPRETRY 10
    
    OSI_LOGI(0, "BoAT: Activate PDP");

    for (i = 0; i < MAX_PDPRETRY; i++)
    {
        OSI_LOGI(0, "BoAT: Try to activate PDP: %d", i);
        fibo_taskSleep(5000);

        memset(&ip, 0, sizeof(ip));
        ret = fibo_PDPActive(1, NULL, NULL, NULL, 0, 0, ip);

        OSI_PRINTFI("[%s-%d] ret = %d", __FUNCTION__, __LINE__, ret);

        OSI_LOGI(0, "BoAT: Try to activate PDP: %d, returns %d", i, ret);
        OSI_LOGI(0, "BoAT: IP = %u.%u.%u.%u", ip[0],ip[1],ip[2],ip[3]);

        fibo_taskSleep(10000);
        
        if (ret == 0 /*&& ip[0] != 0 && ip[0] != '0'*/)
        {
            OSI_LOGI(0, "Activate PDP succeeds");
            fibo_taskSleep(5000);
            break;
        }
    }
    
    // Fail to activate PDP after MAX_PDPRETRY tries
    if (i >= MAX_PDPRETRY)
    {
        OSI_LOGI(0, "Activate PDP fails");
        fibo_taskSleep(5000);
        return;
    }

    OSI_LOGI(0, "BoAT: tcp_testcase");
    
    boat_platone_entry();
        
    srand(fibo_getSysTick());

	ret = 0;/*fibo_PDPRelease(0, 0);*/
    OSI_PRINTFI("[%s-%d] ret = %d", __FUNCTION__, __LINE__, ret);

	fibo_taskSleep(10000);

#endif
	
	fibo_thread_delete();

}


static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    switch (sig)
    {
	
        default:
        {
            break;
        }
    }
    OSI_LOGI(0, "test");
}

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback};

void *appimg_enter(void *param)
{
    OSI_LOGI(0, "application image enter, param 0x%x", param);
    prvInvokeGlobalCtors();
    fibo_thread_create(prvThreadEntry, "mythread", 1024 * 16, NULL, OSI_PRIORITY_HIGH);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    OSI_LOGI(0, "application image exit");
}
