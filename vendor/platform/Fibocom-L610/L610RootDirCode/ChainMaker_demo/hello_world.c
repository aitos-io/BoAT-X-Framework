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

int chainmaker_test(void);
void test_printf(void);

void ntp_callback(void *param)
{
    fibo_ntp_rsp rsp;
    memcpy(&rsp, param, sizeof(fibo_ntp_rsp));
    OSI_PRINTFI("[%s-%d]result:%d, timestamp=%d", __FUNCTION__, __LINE__, rsp.result, rsp.time);
}

static void prvThreadEntry(void *param)
{
    OSI_LOGI(0, "application thread enter, param 0x%x", param);

    int test = 1;
    OSI_PRINTFI("mqttapi wait network");
    reg_info_t reg_info;
    while (test)
    {
        fibo_getRegInfo(&reg_info, 0);
        fibo_taskSleep(1000);
        OSI_PRINTFI("[%s-%d]", __FUNCTION__, __LINE__);
        if (reg_info.nStatus == 1)
        {
            test = 0;
            fibo_PDPActive(1, NULL, NULL, NULL, 0, 0, NULL);
            fibo_taskSleep(1000);
            OSI_PRINTFI("[%s-%d]", __FUNCTION__, __LINE__);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int ret = fibo_sock_ntp("ntp.aliyun.com", 123, 0, ntp_callback);
        OSI_PRINTFI("[%s-%d] ret = %d", __FUNCTION__, __LINE__, ret);
        fibo_taskSleep(5 * 1000);
    }
    chainmaker_test();
    while (1)
    {
        test_printf();
    }

    fibo_thread_delete();
}

void *appimg_enter(void *param)
{
    OSI_LOGI(0, "application image enter, param 0x%x", param);

    fibo_thread_create(prvThreadEntry, "mythread", 1024 * 32, NULL, OSI_PRIORITY_NORMAL);
    return 0;
}

void appimg_exit(void)
{
    OSI_LOGI(0, "application image exit");
}
