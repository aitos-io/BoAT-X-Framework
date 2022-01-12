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
#include "protocolapi/api_platon.h"
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

#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE
#define USE_ONETIME_WALLET

static int init(void);
static bool deinit(int);
extern void boat_demo_entry(void);

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
const BCHAR* pkcs_demoKey = "-----BEGIN EC PRIVATE KEY-----\n"
"MHQCAQEEIPz212cG5mJQ26zJgnvEJzIe25VC1Yp0pnYkslOWBGXKoAcGBSuBBAAK\n"
"oUQDQgAEMU/3IAjKpQc8XdURIGQZZJQRHZhPDkp80ahiRAM7KKV9Gmn699pei5fL\n"
"qZlYLvlxdQJsoh2IPyObgGr87gBT7w==\n"
"-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
const BCHAR* native_demoKey = "0xa09952a7a3e257cfd5af8c844f8cd77f56809cf29ac51893450a7df3aac146c3";

/**
 * PlatON test network node url
 */
 //const BCHAR *demoUrl = "http://47.241.98.219:6789";
//const BCHAR* demoUrl = "http://192.168.132.151:7000";
const BCHAR* demoUrl = "http://35.247.155.162:6789";

/**
 * PlatON test network human-readable part
 */
const BCHAR* hrp = "lat";

/**
 * transfer recipient address
 */
const BCHAR* demoRecipientAddress = "lat1u89782npf3s0a3e7zlmmwqqq58yhjctuqmmjh9";


BoatPlatONWallet* g_platon_wallet_ptr;

#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT platon_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatONWalletConfig wallet_config = { 0 };
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

    wallet_config.chain_id = 100;//210309;
    wallet_config.eip155_compatibility = BOAT_TRUE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATON_NODE_URL_MAX_LEN - 1);

    /* create platon wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATON, NULL, &wallet_config, sizeof(BoatPlatONWalletConfig));
    if (index < BOAT_SUCCESS)
    {
        //BoatLog( BOAT_LOG_CRITICAL, "create one-time wallet failed." );
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platon_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT platon_createPersistWallet(BCHAR* wallet_name)
{
    BSINT32 index;
    BoatPlatONWalletConfig wallet_config = { 0 };
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
    wallet_config.eip155_compatibility = BOAT_TRUE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATON_NODE_URL_MAX_LEN - 1);

    /* create platon wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATON, wallet_name, &wallet_config, sizeof(BoatPlatONWalletConfig));
    if (index < BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_platon_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT platon_loadPersistWallet(BCHAR* wallet_name)
{
    BSINT32 index;

    /* create platon wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATON, wallet_name, NULL, sizeof(BoatPlatONWalletConfig));
    if (index < BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platon_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

BOAT_RESULT platonGetBalance(BoatPlatONWallet* wallet_ptr)
{
    //BCHAR * balance_wei;
    BCHAR* cur_balance_von = NULL;
    BOAT_RESULT result;
    BoatFieldVariable prase_result = { NULL, 0 };

    cur_balance_von = BoatPlatONWalletGetBalance(wallet_ptr, "lat1utcm54t2x6c5z9e6mm7menauuqtmzl68hdm3nr");
    result = BoatPlatONPraseRpcResponseStringResult(cur_balance_von, &prase_result);
    if(prase_result.field_ptr != NULL){
        BoatFree(prase_result.field_ptr);
    }
    if (result == BOAT_SUCCESS)
    {
        //BoatLog( BOAT_LOG_NORMAL, "BoatPlatONWalletGetBalance returns: %s", prase_result.field_ptr );
    }
    else
    {
        return result;
    }

    //BoatLog(BOAT_LOG_NORMAL, "Balance: %s von", prase_result.field_ptr);

    return BOAT_SUCCESS;
}


BOAT_RESULT platonTransfer(BoatPlatONWallet* wallet_ptr)
{
    BOAT_RESULT result;
    BoatPlatONTx tx_ctx;

    /* Set Recipient Address */
    result = BoatPlatONTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
        "0x33333",
        (BCHAR*)demoRecipientAddress,
        hrp);

    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONTxInit failed.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    /* 0xDE0B6B3A7640000: 1ETH or 1e18 wei, value */
    /* 0x2386F26FC10000: 0.01ETH or 1e16 wei, value */
    result = BoatPlatONTransfer(&tx_ctx, "0xdbfe49bc6728ffe");

    return result;
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
    BUINT32 bechaddresslen, i;
    BUINT8 address[50];

    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: create platon wallet */
#if defined(USE_ONETIME_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = platon_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = platon_createPersistWallet("platon.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
    result = platon_loadPersistWallet("platon.cfg");
#else
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>> none wallet type selected.");
    return -1;
#endif	
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "platonWalletPrepare_create failed : %d.", result);
        return -1;
    }

    /* step-3: execute balance transfer */
    result = platonGetBalance(g_platon_wallet_ptr);
    result = platonTransfer(g_platon_wallet_ptr);
    result = platonGetBalance(g_platon_wallet_ptr);
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_NORMAL, "CasePlatON Failed: %d.", result);
    }
    else
    {
        //BoatLog(BOAT_LOG_NORMAL, "CasePlatON Passed.");
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
