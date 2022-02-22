#include "main.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "boatiotsdk.h"
#include "boatlog.h"
#include <iothub_device_client_ll.h>
#include <iothub_client_options.h>
#include <iothubtransportmqtt.h>
#include <iothub.h>




#include "boatiotsdk.h"
#include "boatlog.h"
#include "protocolapi/api_platone.h"
#include "my_contract.cpp.abi.h"
#include <pthread.h>
#include <applibs/wificonfig.h>
#include <applibs/certstore.h>
#include <tlsutils/deviceauth.h>

#include <wolfssl/wolfcrypt/ecc.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/ssl.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include <wolfssl/ssl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <wolfssl/ssl.h>

#include <applibs/log.h>
#include <applibs/networking.h>
#include <applibs/storage.h>
#include <applibs/eventloop.h>



static uint8_t mSsid[WIFICONFIG_SSID_MAX_LENGTH];
static char mPsk[WIFICONFIG_WPA2_KEY_MAX_BUFFER_SIZE];

const BCHAR* pkcs_demoKey = "-----BEGIN EC PRIVATE KEY-----\n"
"MHQCAQEEIPz212cG5mJQ26zJgnvEJzIe25VC1Yp0pnYkslOWBGXKoAcGBSuBBAAK\n"
"oUQDQgAEMU/3IAjKpQc8XdURIGQZZJQRHZhPDkp80ahiRAM7KKV9Gmn699pei5fL\n"
"qZlYLvlxdQJsoh2IPyObgGr87gBT7w==\n"
"-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
const BCHAR* native_demoKey = "0xfcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca";

/**
 * test node url
 */
const BCHAR* demoUrl = "http://116.236.47.90:7545";

/**
 * transfer recipient address
 */
const BCHAR* demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";


BoatPlatoneWallet* g_platone_wallet_ptr;
#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE
#define USE_ONETIME_WALLET

static int init(void);
static bool deinit(int);
extern void boat_demo_entry(void);


#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT platone_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config = { 0 };
    BUINT8 binFormatKey[32] = { 0 };

    (void)binFormatKey; //avoid warning

    /* wallet_config value assignment */
#if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8*)pkcs_demoKey;
    wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
#else  
    /* default is internal generation */
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
#endif

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    /* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if (index < BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT platone_createPersistWallet(BCHAR* wallet_name)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config = { 0 };
    BUINT8 binFormatKey[32] = { 0 };

    (void)binFormatKey; //avoid warning

    /* wallet_config value assignment */
#if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8*)pkcs_demoKey;
    wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
#else  
    /* default is internal generation */
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
#endif

    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    /* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if (index < BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_platone_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT platone_loadPersistWallet(BCHAR* wallet_name)
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
#endif

BOAT_RESULT platone_call_mycontract(BoatPlatoneWallet* wallet_ptr)
{
    BCHAR* result_str;
    BoatPlatoneTx tx_ctx;
    BOAT_RESULT result;
    nodesResult result_out = { 0,NULL };

    /* Set Contract Address */
    result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
        "0x333333",
        (BCHAR*)demoRecipientAddress,
        BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_NORMAL, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    
    result_str = BoatPlatoneGetNodesInfo(&tx_ctx, &result_out);
    for (size_t i = 0; i < result_out.num; i++)
    {
        /* code */
        printf("node[%d] : IP[%s],port[%d]. \n", i, result_out.nodeInfo[i].IP, result_out.nodeInfo[i].rpcPort);
    }
    nodeResFree(&result_out);
    result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
    if (result_str == NULL)
    {
        //BoatLog(BOAT_LOG_NORMAL, "my_contract_cpp_abi_setName failed: %s.", result_str);
        return BOAT_ERROR;
    }
    //BoatLog(BOAT_LOG_NORMAL, "setName returns: %s", result_str);

    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
    {
        //BoatLog(BOAT_LOG_NORMAL, "my_contract_cpp_abi_getName failed: %s.", result_str);
        return BOAT_ERROR;
    }
    //BoatLog(BOAT_LOG_NORMAL, "getName returns: %s", result_str);

    return BOAT_SUCCESS;
}

static int mWifiId = -1;

int boat_connect_wifi(const uint8_t* ssid, const char* psk)
{
    if (WifiConfig_ForgetAllNetworks() < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "ERROR: WifiConfig_ForgetAllNetworks failed: %s (%d).\n", strerror(errno), errno);
    }
    else
    {
        mWifiId = WifiConfig_AddNetwork();
        if (mWifiId < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_AddNetwork failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetSecurityType(mWifiId, WifiConfig_Security_Wpa2_Psk) < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetSecurityType failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetPSK(mWifiId, psk, strnlen(psk, WIFICONFIG_WPA2_KEY_MAX_BUFFER_SIZE)) < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetPSK failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetSSID(mWifiId, ssid, strnlen(ssid, WIFICONFIG_SSID_MAX_LENGTH)) < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetSSID failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetTargetedScanEnabled(mWifiId, true) < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetTargetedScanEnabled failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetNetworkEnabled(mWifiId, true))
        {
            BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetNetworkEnabled failed: %s (%d).\n", strerror(errno), errno);
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "SUCCESS: wifi");
        }
    }
    return mWifiId;
}

int boat_set_wifi_name_passwd(void)
{
    
    memset(mSsid, 0x00, sizeof(mSsid));
    strncpy(mSsid, "AITOS_1E7080", strlen("AITOS_1E7080"));

    memset(mPsk, 0x00, sizeof(mPsk));
    strncpy(mPsk, "aitos_io", strlen("aitos_io"));
    


}





void boat_fiscobcos_entry(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: create platone wallet */
#if defined(USE_ONETIME_WALLET)	
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = platone_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = platone_createPersistWallet("platone.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
    result = platone_loadPersistWallet("platone.cfg");
#else
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
    return -1;
#endif	
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
        return -1;
    }

    /* step-3: execute 'platone_call_mycontract' */
    result = platone_call_mycontract(g_platone_wallet_ptr);
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_NORMAL, "platone mycontract access Failed: %d.", result);
    }
    else
    {
        //BoatLog(BOAT_LOG_NORMAL, "platone mycontract access Passed.");
    }

    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
}

static void* thread_boat_fiscobcos(void* arg)
{
    int count = 0;
    WifiConfig_ConnectedNetwork network;

    boat_set_wifi_name_passwd();
    boat_connect_wifi(mSsid, mPsk);

    while (1)
    {
        if (WifiConfig_GetCurrentNetwork(&network) >= 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "INFO: SSID \"%.*s\", BSSID %02x:%02x:%02x:%02x:%02x:%02x, Frequency %dMHz, %d dB.\n",
                network.ssidLength, network.ssid, network.bssid[0], network.bssid[1],
                network.bssid[2], network.bssid[3], network.bssid[4], network.bssid[5],
                network.frequencyMHz, network.signalRssi);

            BoatSleep(10);
            boat_fiscobcos_entry();
            break;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "wifi cannot connect");
            BoatSleep(1);
        }
    }
}

void boat_demo_entry(void)
{
    pthread_t th;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&th, &attr, thread_boat_fiscobcos, NULL);
    if (ret != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "ERROR: Create thread_boat_fiscobcos error\n");
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "INFO: This is the thread_boat_fiscobcos process.\n");
    }
}


int main(int argc, char* argv[])
{
    BoatLog(BOAT_LOG_NORMAL, "INFO: Application starting.\n");

    BoatLog(BOAT_LOG_NORMAL, "INFO: Application init.\n");
    int err = init();

    boat_demo_entry();

    if (err < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "INFO: Application error.\n");
    }
    else
    {
        while (1)
        {
            ;
        }
    }

    while (!deinit(err)) {
        if (DEBUG) BoatLog(BOAT_LOG_NORMAL, "INFO: Application deinit.\n");
    }

    return 0;
}

bool AzureIoT_Initialize(void)
{
    if (IoTHub_Init() != 0) {
        //Log(TAG, "ERROR: failed initializing platform.\n");
        return false;
    }
    return true;
}

/// <summary>
///     Deinitializes the Azure IoT Hub SDK.
/// </summary>
void AzureIoT_Deinitialize(void)
{
    IoTHub_Deinit();
}

static int init(void)
{
    if (!AzureIoT_Initialize())
    {
        BoatLog(BOAT_LOG_NORMAL, "ERROR: Cannot initialize Azure IoT Hub SDK.\n");
        return -1;
    }
    return 1;
}

static bool deinit(int err)
{
    bool fin = true;
    AzureIoT_Deinitialize();
    return fin;
}

/*
 ***************************************************************************
 */
