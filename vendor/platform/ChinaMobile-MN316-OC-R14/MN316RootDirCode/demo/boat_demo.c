/*===========================================================================
                         boat_demo.c
DESCRIPTION
   This file is boat demo application.
===========================================================================*/

#include "my_contract.h"
#include "api_platone.h"
#include "boatiotsdk.h"
#include "boatlog.h"

#include "cm_mem.h"
#include "cm_pm.h"
#include "cm_sys.h"
#include "cm_sim.h"
#include "cm_modem.h"
#include "cm_rtc.h"
#include "cm_os.h"
#include "cm_asocket.h"
#include "cm_eloop.h"
#include "cm_async_dns.h"


static int g_boat_task_Handle = -1;
BoatPlatoneWallet *g_platone_wallet_ptr;

/**
 * native demo key
 */
const BCHAR *native_demoKey = "0xfcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca";

/**
 * test node url
 */
const BCHAR *demoUrl = "http://116.236.47.90:7545";


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

__BOATSTATIC BOAT_RESULT platone_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]               = {0};

    (void)binFormatKey; //avoid warning

	/* wallet_config value assignment */


    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;

    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
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
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatoneTxInit fails.\n");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "zzq_HelloWorld");
    if (result_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "my_contract_cpp_abi_setName failed: %s.\n", result_str);
		return BOAT_ERROR;
    }
    BoatLog(BOAT_LOG_VERBOSE, "setName returns: %s\n", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "my_contract_cpp_abi_getName failed: %s.\n", result_str);
		return BOAT_ERROR;
    }
    BoatLog(BOAT_LOG_VERBOSE, "getName returns: %s\n", result_str);
	
    return BOAT_SUCCESS;
}

void init_asocket_module()
{
    /* 初始化异步eloop模块 */
    cm_eloop_init();

    /* 创建事件循环 */
    s_main_eloop = cm_eloop_create(MAX_EVENT_NUM);

    /* 初始化异步socket模块 */
    cm_asocket_init(MAX_ASOCKET_NUM, s_main_eloop);

    /* 初始化异步DNS模块 */
    cm_async_dns_init(MAX_ASYNC_DNS_NUM, s_main_eloop);
}

void boat_platone_entry(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    int ret = 0;
	/* step-1: Boat SDK initialization */
    result = BoatIotSdkInit();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatIotSdkInit failed ,result = %d.\n", result);
        return;

    }


	result = platone_createPersistWallet("platone.cfg");

    if (result != BOAT_SUCCESS)
    {
	    BoatLog(BOAT_LOG_CRITICAL, "platone create Wallet failed ,result = %d.\n", result);
	    goto end;
    }
    BoatLog(BOAT_LOG_VERBOSE, "======= platone: create_Wallet success ======\n");

    while (1)
    {
        /* step-3: execute 'platone_call_mycontract' */
        result = platone_call_mycontract(g_platone_wallet_ptr);
        if( result != BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_CRITICAL, "platone_call_mycontract failed , result = %d.\n", result);
            goto end;
        }
        BoatLog(BOAT_LOG_VERBOSE, "======= platone:call_mycontract success ======\n");
    }

end:	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

}

static void* boat_task_entry(void *args)
{
    BUINT32 net_retry_time = 60;

    if (cm_sim_get_cpin()<0)
    {
        cm_demo_printf("sim not ready\n");
    }
    else
    {
        cm_demo_printf("sim ready\n");
        cm_demo_printf("Waiting for network connection...\n");
        while(net_retry_time--)//等待驻网
        {
            if (cm_modem_get_pdp_state(0) > 0)
            {
                break;
            }

            osDelay(1000);
        }

        if (net_retry_time <= 0)
        {
            cm_demo_printf("Network is not connected\n");
        }
        else
        {
            cm_demo_printf("Network is connected\n");
        }
    }

    osDelay(10);
    init_asocket_module();

    osDelay(10);
    struct tm cur_time = {0};
    cm_rtc_get_time_tm(&cur_time);
    cm_demo_printf("Boat Current time:%04d-%02d-%02d %02d:%02d:%02d\n", 
        cur_time.tm_year,
        cur_time.tm_mon,
        cur_time.tm_mday,
        cur_time.tm_hour,
        cur_time.tm_min,
        cur_time.tm_sec);

    boat_platone_entry();

    return (void *)0;
}

void cm_opencpu_entry(void)
{
	cmd_task_flag = osEventFlagsNew(0);
    osThreadAttr_t attr = {
        .name = "boat task",
        .priority = osPriorityNormal2,
        .stack_size = 1024,//WORD,size = 1024*4
    };

    osThreadNew(boat_task_entry, NULL, (const osThreadAttr_t *)&attr);
}

