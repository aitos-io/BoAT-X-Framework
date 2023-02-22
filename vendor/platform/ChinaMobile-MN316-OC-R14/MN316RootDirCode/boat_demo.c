/**
****************************************************************************************
* @FilePath: boat_demo.c
* @Author: aitos
* @Date: 2023-02-09 15:34:30
* @LastEditors:
* @LastEditTime: 2023-02-09 15:34:30
* @Descripttion:
****************************************************************************************
*/
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
const BCHAR *pkcs_demoKey = "-----BEGIN EC PRIVATE KEY-----\n"
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

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";

BoatPlatoneWallet *g_platone_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;

#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS
#define USE_ONETIME_WALLET

int test_asocket_init(void);

__BOATSTATIC BOAT_RESULT platone_createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32] = {0};

    (void)binFormatKey; // avoid warning

/* wallet_config value assignment */
#if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
                        // cm_log_printf(0, ">>>>>>>>>> wallet format: internal generated.");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
                        // cm_log_printf(0, ">>>>>>>>>> wallet format: external injection[pkcs].");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
    keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; // length contain terminator
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
                        // cm_log_printf(0, ">>>>>>>>>> wallet format: external injection[native].");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = binFormatKey;
    keypair_config.prikey_content.field_len = 32;
#else
    /* default is internal generation */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
#endif

    /* create platone wallet */
#if defined(USE_ONETIME_WALLET)
    result = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypairIndex = result;

    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createNetwork()
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatPlatoneNetworkConfig network_config = {0};

    network_config.chain_id = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    /* create platone network */
#if defined(USE_ONETIME_WALLET)
    result = BoATPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoATPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
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
    nodesResult result_out = {0, NULL};

    /* Set Contract Address */
    result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
                               "0x333333",
                               (BCHAR *)demoRecipientAddress,
                               BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if (result != BOAT_SUCCESS)
    {
        // cm_log_printf(0, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    result_str = BoatPlatoneGetNodesInfo(&tx_ctx, &result_out);
    for (BSINT32 i = 0; i < result_out.num; i++)
    {
        /* code */
        cm_log_printf(0, "node[%d] : IP[%s],port[%d]. \n", i, result_out.nodeInfo[i].IP, result_out.nodeInfo[i].rpcPort);
    }
    nodeResFree(&result_out);

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
    if (result_str == NULL)
    {
        // cm_log_printf(0, "my_contract_cpp_abi_setName failed: %s.", result_str);
        return BOAT_ERROR;
    }
    // cm_log_printf(0, "setName returns: %s", result_str);

    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
    {
        // cm_log_printf(0, "my_contract_cpp_abi_getName failed: %s.", result_str);
        return BOAT_ERROR;
    }
    cm_log_printf(0, "getName returns: %s", result_str);

    return BOAT_SUCCESS;
}

void boat_platone_entry(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
    cm_log_printf(0, " boat_platone_entry !!!\n");
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    cm_log_printf(0, " boat_platone_entry 000\n");
    /* step-2: create platone wallet */
    // cm_log_printf(0, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = platone_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
        // return -1;
        boat_throw(result, platone_demo_catch);
    }
    cm_log_printf(0, " boat_platone_entry 111\n");
    result = createNetwork();
    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
        // return -1;
        boat_throw(result, platone_demo_catch);
    }
    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
        // return -1;
        boat_throw(result, platone_demo_catch);
    }
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex, networkIndex);
    if (g_platone_wallet_ptr == NULL)
    {
        // cm_log_printf(0,"BoatPlatoneWalletInit fail");
        boat_throw(BOAT_ERROR, platone_demo_catch);
    }
    BoatLog(BOAT_LOG_VERBOSE, "======= platone: create_Wallet success ======\n");

    while (1)
    {
        /* step-3: execute 'platone_call_mycontract' */
        result = platone_call_mycontract(g_platone_wallet_ptr);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_CRITICAL, "platone_call_mycontract failed , result = %d.\n", result);
            break;
        }
        BoatLog(BOAT_LOG_VERBOSE, "======= platone:call_mycontract success ======\n");
        osDelay(1000);
    }

    boat_catch(platone_demo_catch)
    {
    }
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
}

static void *boat_task_entry(void *args)
{
    BUINT32 net_retry_time = 60;
    char tempbuf[64] = {0};
    cm_pm_work_lock();
    cm_pm_set_mode(0);
    cm_log_printf(0, "boat_task_entry   \n");
    cm_log_printf(0, "boat_task_entry   \n");
    cm_log_printf(0, "boat_task_entry   \n");
    if (cm_sim_get_cpin() < 0)
    {
        cm_log_printf(0, "sim not ready\n");
    }
    else
    {
        if (cm_sim_get_imsi(tempbuf) == 0)
        {
            cm_log_printf(0, "boat IMSI:%s\n", tempbuf);
        }
        else
        {
            cm_log_printf(0, "IMSI ERROR\n");
        }
        memset(tempbuf, 0, 64);
        if (cm_sim_get_iccid(tempbuf) == 0)
        {
            cm_log_printf(0, "boat ICCID:%s\n", tempbuf);
        }
        else
        {
            cm_log_printf(0, "ICCID ERROR\n");
        }
        memset(tempbuf, 0, 64);
        cm_log_printf(0, "sim ready\n");
        cm_log_printf(0, "Waiting for network connection...\n");
        while (net_retry_time--) // 等待驻网
        {
            if (cm_modem_get_pdp_state(0) > 0)
            {
                break;
            }

            osDelay(1000);
            cm_log_printf(0, "Waiting for network connection, try again \n");
        }

        if (net_retry_time <= 0)
        {
            cm_log_printf(0, "Network is not connected\n");
        }
        else
        {
            cm_log_printf(0, "Network is connected\n");
        }
    }

    // osDelay(10);
    // init_asocket_module();
    test_asocket_init();

    osDelay(10);
    boat_platone_entry();
    while (1)
    {
        /* code */
        cm_log_printf(0, "thread wait....\n");
        osDelay(10 * 1000);
    }

    cm_log_printf(0, "OpenCPU task exit\n");
    osThreadExit();
}

void cm_opencpu_entry(void)
{
    osThreadAttr_t attr = {
        .name = "boat_task",
        .priority = osPriorityNormal2,
        .stack_size = 1024 * 8, // WORD,size = 1024*4
    };

    osThreadNew(boat_task_entry, NULL, (const osThreadAttr_t *)&attr);
}
