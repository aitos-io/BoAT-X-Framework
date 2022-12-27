/**
  ******************************************************************************
  * @file    sc_application.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source code for SIMCom OpenSDK application, void userSpace_Main(void * arg) is the app entry for OpenSDK application,customer should start application from this call.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include "sc_ApiMap.h"
#include "simcom_debug.h"
#include "simcom_os.h"
#include "simcom_uart.h"

extern void sAPP_SimcomUIDemo(void);
extern void sAPP_HelloWorldDemo(void);
extern void sAPP_UrcTask(void);
extern void sAPP_UartTask(void);
extern void sAPP_UsbVcomTask(void);
extern void sAPP_FabricDemo(void);

/**
  * @brief  OpenSDK app entry.
  * @param  Pointer arg
  * @note   This is the app entry,like main(),all functions must start from here!!!!!!
  * @retval void
  */
void userSpace_Main(void *arg)
{
    /*  simcom api init. Do not modify!  */
    ApiMapInit(arg);
    // sAPI_enableDUMP();
    /*  UI demo task for customer with CLI method for all demo running,
        customer need to define SIMCOM_UI_DEMO_TO_USB_AT_PORT or
        SIMCOM_UI_DEMO_TO_UART1_PORT to select hardware interface.
    */
    sleep(5);//Wait for USB initialization to complete and print catstudio log.

    // sAPP_SimcomUIDemo();

    sAPP_UartTask();
    sAPP_UrcTask();
    sAPP_UsbVcomTask();
    // sAPP_HelloWorldDemo();
    sAPP_FabricDemo();
    printf("user app is running...");

}
#define _appRegTable_attr_ __attribute__((unused, section(".userSpaceRegTable")))

#define appMainStackSize (1024*10)
userSpaceEntry_t userSpaceEntry _appRegTable_attr_ = {NULL, NULL, appMainStackSize, 30, "userSpaceMain", userSpace_Main, NULL };