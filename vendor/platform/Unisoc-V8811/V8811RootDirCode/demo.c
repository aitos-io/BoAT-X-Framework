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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "api_platone.h"
#include "boatiotsdk.h"
#include "my_contract.cpp.abi.h"
#include "boatplatform_internal.h"

#include "osi_api.h"
#include "osi_log.h"

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

static void prvThreadEntry(void *param)
{
    BCHAR readbuff[20] = {0};
    BUINT32 length;
    OSI_LOGI(0, "application thread enter, param 0x%x", param);

    for (int n = 0; n < 10; n++)
    {
        OSI_LOGI(0, "hello world %d", n);
        osiThreadSleep(500);
    }
    BoatWriteFile(PERSIST_WALLET_NAME, "123456", 6, NULL);
    BoatSleep(1);
    BoatGetFileSize(PERSIST_WALLET_NAME, &length, NULL);
    BoatSleep(1);
    BoatReadFile(PERSIST_WALLET_NAME, readbuff, length, NULL);
    BoatSleep(1);
    BoatRemoveFile(PERSIST_WALLET_NAME, NULL);
    BoatSleep(1);

    osiThreadExit();
}


int appimg_enter(void *param)
{
    OSI_LOGI(0, "application image enter, param 0x%x", param);
    OSI_LOGI(0, "data=%d (expect 1), bss=%d", gTestData, gTestBss);
    osiThreadCreate("mythread", prvThreadEntry, NULL, OSI_PRIORITY_NORMAL, 1024, 0);
    return 0;
}


void appimg_exit(void)
{
    OSI_LOGI(0, "application image exit");
}
