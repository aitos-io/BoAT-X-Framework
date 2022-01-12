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

#define OSI_LOG_TAG OSI_MAKE_LOG_TAG('M', 'Y', 'A', 'P')

#include "fibo_opencpu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "api_platone.h"
#include "boatiotsdk.h"
#include "my_contract.cpp.abi.h"

#define TEST_TASK_NAME "TEST_TASK"

#define PERSIST_WALLET_NAME     "/nvm/platone.cfg"

static char   g_tcppost[] = "POST / HTTP/1.1\r\n"
                            "Host: 58.215.142.223:7545\r\n"
                            "Content-Type:application/json;charset=UTF-8\r\n"
                            "Accept:application/json, text/javascript, */*;q=0.01\r\n"
                            "Accept-Language:zh-CN,zh;q=0.8\r\n"
                            "Content-Length: 213\r\n\r\n"
                            "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45\",\"gas\":\"0x1fffff\",\"gasPrice\":\"0x8250de00\",\"data\":\"0xd1880000000000000009876765744e616d65\"},\"latest\"],\"id\":948856337}\"";

BoatPlatoneWallet *g_platone_wallet_ptr;

void tcp_testcase(void)
{
    GAPP_TCPIP_ADDR_T addr;
    INT8 socketid;
    int rcvnum = 0;
    char rcvbuf[256];
    struct timeval timeout = {10, 0};
    struct timeval tmp;
    INT32 len;
    INT32 retcode, ret;

    memset(rcvbuf, 0, sizeof(rcvbuf));
    memset(&addr, 0, sizeof(GAPP_TCPIP_ADDR_T));
    OSI_PRINTFI("demo case 9 sys_sock_create start :\n");
    socketid = fibo_sock_create(GAPP_IPPROTO_TCP);
    OSI_PRINTFI("demo case 9 sys_sock_create retcode = %d\n", socketid);

    addr.sin_port = htons(7545);

    ip4addr_aton("58.215.142.223", &addr.sin_addr.u_addr.ip4);
    addr.sin_addr.u_addr.ip4.addr = htonl(addr.sin_addr.u_addr.ip4.addr);

    retcode = fibo_sock_connect(socketid, &addr);
    OSI_PRINTFI("BoAT: demo case 9 sys_sock_connect addr.sin_addr.addr = %u\n", addr.sin_addr.u_addr.ip4.addr);

    OSI_PRINTFI("demo case 9 sys_sock_connect addr.sin_addr.addr = %u\n", addr.sin_addr.u_addr.ip4.addr);
    OSI_PRINTFI("BoAT: demo case 9 sys_sock_create  retcode = %d\n", retcode);

    ret = fibo_sock_send(socketid, (UINT8 *)g_tcppost, strlen(g_tcppost));
    OSI_PRINTFI("BoAT: demo case 9 sys_sock_send  retcode = %d, data = %s\n", ret, g_tcppost);
    ret = fibo_sock_setOpt(socketid, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    OSI_PRINTFI("BoAT: demo case 9 sys_sock_setOpt  ret = %d\n", ret);

    fibo_taskSleep((UINT32)1000);
    len = sizeof(tmp);
    ret = fibo_sock_getOpt(socketid, SOL_SOCKET, SO_RCVTIMEO, &tmp, &len);
    OSI_PRINTFI("BoAT: demo case 9 sys_sock_getOpt tmp.sec =%d, tmp.usec =%d,  len = %d, ret = %d\n", tmp.tv_sec, tmp.tv_usec, len, ret);

    rcvnum = fibo_sock_recv(socketid, (UINT8 *)rcvbuf, 240);
    OSI_PRINTFI("BoAT: demo case 9 sys_sock_recv  rcvnum = %d, rcvbuf = %s\n", rcvnum, rcvbuf);

}
    


static void prvInvokeGlobalCtors(void)
{
    extern void (*__init_array_start[])();
    extern void (*__init_array_end[])();

    size_t count = __init_array_end - __init_array_start;
    for (size_t i = 0; i < count; ++i)
        __init_array_start[i]();
}

static BOAT_RESULT platone_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, NULL, sizeof(BoatPlatoneWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        OSI_LOGI(0, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}

static BOAT_RESULT platone_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
   
	/* wallet_config value assignment */
	/* for one-time wallet, the 'prikeyId' field should be cleared */
	//memset(wallet_config.prikeyId, 0, BOAT_KEYID_MAX_LEN); 
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
#if 1
    char *nativedemoKey = "0xfcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca";
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(wallet_config.prikeyCtx_config.prikey_content.field_ptr, 32, nativedemoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
#endif
  
	strncpy(wallet_config.node_url_str, "http://116.236.47.90:7545", BOAT_PLATONE_NODE_URL_MAX_LEN - 1); // "http://116.236.47.90:7545" //"http://106.14.94.165:8080"
 
	OSI_LOGI(0, "======node_url_str:%s", wallet_config.node_url_str);

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, PERSIST_WALLET_NAME, &wallet_config, sizeof(BoatPlatoneWalletConfig)); // "/nvm/test.txt"
    OSI_LOGI(0, "BoatWalletCreate index=%d", index);

    if (index < BOAT_SUCCESS)
	{
        OSI_LOGI(0, "create one-time wallet failed.");
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
							   "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45",
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if (result != BOAT_SUCCESS)
	{
        OSI_LOGI(0, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
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

	while(0)
    {
        tcp_testcase();
        fibo_taskSleep(5000);
    }
    
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
