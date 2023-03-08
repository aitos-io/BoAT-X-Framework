
/**
****************************************************************************************
* @FilePath: boatPlatONEdemo.c
* @Author: aitos
* @Date: 2023-02-08 14:48:18
* @LastEditors:
* @LastEditTime: 2023-02-08 14:48:18
* @Descripttion:
****************************************************************************************
*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "api_platone.h"

#include "boatiotsdk.h"
#include "boatlog.h"

#include "cm_os.h"
#include "cm_atchart.h"
#include "cm_net_register.h"

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

// #define cm_printf
#define PERSIST_WALLET_NAME "/nvm/platone.cfg"
BoatPlatoneWallet *g_platone_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;

static int net_ready = 0;
static unsigned char virt_at_buffer[2048];
static osMutexId_t vir_at_mutex;
static osEventFlagsId_t at_task_flag;

static const osMutexAttr_t at_Mutex_attr =
    {
        "atThreadMutex",                       // human readable mutex name
        osMutexRecursive | osMutexPrioInherit, // attr_bits
        0,                                     // memory for control block
        0U                                     // size for control block
};

static void cm_sys_at_callback(void *param)
{
    osEventFlagsSet(at_task_flag, 0x00000001U);
}

/**
 *  \brief 虚拟AT指令消息通知回调函数
 *
 *  \param [in] pcu8NotifyLine URC消息数据接收指针
 *  \param [in] u32LineLen URC消息长度
 *  \param [in] eNotifyType URC消息类型
 *  \return 空
 *
 *  \details More details
 */
static void cm_test_at_notify(const uint8_t *pcu8NotifyLine, uint32_t u32LineLen, cm_atchart_notify_e eNotifyType)
{
    if (eNotifyType == CM_ATCHART_RSP_EMPTY_LINE)
        return;
    cm_net_register_notify(pcu8NotifyLine, u32LineLen, eNotifyType);
}

/**
 *  \brief  虚拟AT指令任务
 *
 *  \param [in]
 *  \return NULL
 *
 *  \details More details
 */
static void virture_at_task(void)
{
    while (1)
    {
        osEventFlagsWait(at_task_flag, 0x00000001U, osFlagsWaitAny, osWaitForever);
        memset(virt_at_buffer, 0, 2048);
        int virt_at_len = cm_vir_at_get(virt_at_buffer, 2048);
        virt_at_buffer[virt_at_len] = 0;
        cm_sys_log("%s", virt_at_buffer);
        cm_atchart_on_recv(virt_at_buffer, virt_at_len);
    }
}

static void boat_test_vir_at_init(void)
{
    osThreadAttr_t virat_task_attr = {0};
    at_task_flag = osEventFlagsNew(0);
    if (at_task_flag == 0)
    {
        return;
    }

    virat_task_attr.name = "virat_task";
    virat_task_attr.stack_size = 2048;

    vir_at_mutex = osMutexNew(&at_Mutex_attr);
    // osThreadDef(OC_AT_Task, virture_at_task, osPriorityNormal, 0, 2048);
    osThreadId_t OC_AT_TaskHandle = osThreadNew(virture_at_task, 0, &virat_task_attr);

    cm_vir_at_init(cm_sys_at_callback);
    cm_atchart_init();
    cm_atchart_open();
    cm_atchart_set_notify_cb(cm_test_at_notify);
}

/**
 *  \brief 等待网络注册成功
 *
 *  \return 0：成功 -1：超时失败
 *
 *  \details More details
 */
static int cm_test_wait_for_network(void)
{
    // 等待时间
    int retry_count = 50 * 60;
    // 将成功标志设为0
    net_ready = 0;
    // 手动触发网络注册模块更新状态，确保获取到最新网络状态
    cm_net_register_update_status();
    // 等待一段时间，当超过时间，network_state_change回调函数仍没有返回CM_NETWORK_STATE_PDP_READY，说明网络注册可能遇到问题。
    while ((net_ready == 0) && (retry_count > 0))
    {
        osDelay(1);
        retry_count--;
    }
    if (net_ready != 0) // 网络注册成功
    {
        cm_sys_log("boat network ready\n");
        return 0;
    }
    else // 网络注册尚未成功
    {
        cm_sys_log("boat network not ready\n");
        return -1;
    }
}
// 网络自注册接口回调函数，通知网络状态变化
static void network_state_change(cm_network_state_e state)
{
    cm_sys_log("[boat]:state:%d\n", state);
    switch (state)
    {
    // 自动注网模块初始化
    case CM_NETWORK_STATE_INIT:
        cm_sys_log("[boat]:Module init\n");
        break;
    // 无SIM卡
    case CM_NETWORK_STATE_NO_SIM:
        cm_sys_log("[boat]:No SIM\n");
        break;
    // SIM卡识别成功
    case CM_NETWORK_STATE_SIM_READY:
        cm_sys_log("[boat]:SIM ready\n");
        break;
    // 正在附着网络，CEREG:0,2
    case CM_NETWORK_STATE_ATTACHING:
        cm_sys_log("[boat]:Attaching\n");
        break;
    // 网络附着失败，CEREG:0,0
    case CM_NETWORK_STATE_ATTACH_FAIL:
        cm_sys_log("[boat]:Attach fail\n");
        break;
    // 网络附着成功，CEREG:0,1
    case CM_NETWORK_STATE_ATTACH_SUCCESS:
        cm_sys_log("[boat]:Attach success\n");
        break;
    // 正在激活PDP
    case CM_NETWORK_STATE_PDP_ACTIVATING:
        cm_sys_log("[boat]:PDP activating\n");
        break;
    // PDP激活失败
    case CM_NETWORK_STATE_PDP_NOT_READY:
        cm_sys_log("[boat]:PDP not ready\n");
        break;
    // PDP激活成功
    case CM_NETWORK_STATE_PDP_READY:
        net_ready = 1;
        cm_sys_log("[boat]:PDP ready\n");
        break;
    default:
        cm_sys_log("[boat]:Module stop\n");
    }
}

static void cm_test_net_register_init(void)
{
    // 网络配置结构体
    cm_network_param_t network_param;

    // 填写APN等信息，
    network_param.cid = 1;
    network_param.apn = "CMNET";
    network_param.pdp_type = "IP";
    network_param.use_auth = 0;

    // 初始化
    cm_net_register_init();
    // 网络参数配置
    cm_net_register_config(&network_param);
    // 设置网络状态变化时的回调函数
    cm_net_register_set_callback(network_state_change);
    // 启动网络自注册模块
    cm_net_register_start();
}

#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE
#define USE_ONETIME_WALLET

__BOATSTATIC BOAT_RESULT platone_createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32] = {0};

    (void)binFormatKey; // avoid warning

/* wallet_config value assignment */
#if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
                        // BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
                        // BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
    keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; // length contain terminator
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
                        // BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
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
        // BoatLog(BOAT_LOG_NORMAL, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    result_str = BoatPlatoneGetNodesInfo(&tx_ctx, &result_out);
    for (BSINT32 i = 0; i < result_out.num; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL, "node[%d] : IP[%s],port[%d]. \n", i, result_out.nodeInfo[i].IP, result_out.nodeInfo[i].rpcPort);
    }
    nodeResFree(&result_out);

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
    if (result_str == NULL)
    {
        // BoatLog(BOAT_LOG_NORMAL, "my_contract_cpp_abi_setName failed: %s.", result_str);
        return BOAT_ERROR;
    }
    // BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);

    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
    {
        // BoatLog(BOAT_LOG_NORMAL, "my_contract_cpp_abi_getName failed: %s.", result_str);
        return BOAT_ERROR;
    }
    // BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);

    return BOAT_SUCCESS;
}

void boat_platone_entry(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: create platone wallet */
    // BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = platone_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
        // return -1;
        boat_throw(result, platone_demo_catch);
    }
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
        // BoatLog(BOAT_LOG_NORMAL,"BoatPlatoneWalletInit fail");
        boat_throw(BOAT_ERROR, platone_demo_catch);
    }
    /* step-3: execute 'platone_call_mycontract' */
    result = platone_call_mycontract(g_platone_wallet_ptr);
    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_NORMAL, "platone mycontract access Failed: %d.", result);
    }
    else
    {
        // BoatLog(BOAT_LOG_NORMAL, "platone mycontract access Passed.");
    }
    boat_catch(platone_demo_catch)
    {
    }
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}

static void boat_test_network_config(void)
{
    // 采用ATCHART模块实现的网络自注册方式，实现自动注网
    cm_test_net_register_init();
    cm_test_wait_for_network();
}

void cm_main_task(void *p)
{
    boat_test_vir_at_init();
    boat_test_network_config();
    osDelay(10);
    boat_platone_entry();
    while (1)
    {
        cm_sys_log("[boat] app is running\n");
        osDelay(500);
    }
}

void ML302_OpenCPU_Entry()
{
    osThreadAttr_t main_task_attr = {0};
    main_task_attr.name = "main_task";
    main_task_attr.stack_size = 1024 * 24;

    osDelay(10);
    osThreadId_t OC_Main_TaskHandle = osThreadNew(cm_main_task, 0, &main_task_attr);
}

// SDK中调用，不可删除
int cm_wdt_config_cb(void)
{
    return 0; // 关闭看门狗
    // return 1;//开启看门狗
}

// SDK中调用，GNSS功能硬件版本自适应配置项，不可删除，亦无需修改，修改可能引起功能异常
int cm_gnss_uc6226_cb(void)
{
#if ((defined CM_HARDWARE_FNLM_SUPPORT) || (defined CM_HARDWARE_KNLM_SUPPORT) || (defined CM_HARDWARE_GNLM_SUPPORT) || (defined CM_HARDWARE_LNLM_SUPPORT))
    cm_sys_log("[boat] cm_gnss_uc6226_cb 1\n");
#ifdef CM_PROD_VERSION_H3
    cm_uc6226_gnss_reset(1);
#endif
    return 1;
#else
    cm_sys_log("[boat] cm_gnss_uc6226_cb 0\n");
    return 0;
#endif
}

int cm_gnss_uctd1036_cb(void)
{
#if ((defined CM_HARDWARE_ENLM_SUPPORT) || (defined CM_HARDWARE_PNLM_SUPPORT))
    cm_sys_log("[boat] cm_gnss_uctd1036_cb 1\n");
    return 1;
#else
    cm_sys_log("[boat] cm_gnss_uctd1036_cb 0\n");
    return 0;
#endif
}

/*
    function used internal, name is unchangeable
*/
int cm_sntp_call_cb(int result)
{
    if (result == 0)
    {
        cm_sys_log("[boat SNTP SYNC CB]failed\n");
    }
    else
    {
        cm_sys_log("[boat SNTP SYNC CB]success\n");
    }
}

void cm_printf(char *str, ...)
{
}
