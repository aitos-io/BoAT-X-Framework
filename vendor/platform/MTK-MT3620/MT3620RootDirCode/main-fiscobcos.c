#include "main.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "boatiotsdk.h"
#include <iothub_device_client_ll.h>
#include <iothub_client_options.h>
#include <iothubtransportmqtt.h>
#include <iothub.h>




#include "boatiotsdk.h"
#include "HelloWorld.h"
#include "protocolapi/api_fiscobcos.h"
#include "HelloWorld.h"
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

#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE
#define USE_ONETIME_WALLET

static int init(void);
static bool deinit(int);
extern void boat_demo_entry(void);


#define  USE_CREATE_PERSIST_WALLET //if expect create a persist wallet, uncomment this definition
//#define  USE_LOAD_PERSIST_WALLET   // if expect load a persist wallet, uncomment this definition

static uint8_t mSsid[WIFICONFIG_SSID_MAX_LENGTH];
static char mPsk[WIFICONFIG_WPA2_KEY_MAX_BUFFER_SIZE];

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
  * "78a42562c1d19843fd6f5a0f07de0206fdcf2a682c5e0a9a814019abb531da3a"
  */
const BCHAR* pkcs_demoKey = "-----BEGIN PRIVATE KEY-----\n"
"MIGEAgEAMBAGByqGSM49AgEGBSuBBAAKBG0wawIBAQQgeKQlYsHRmEP9b1oPB94C\n"
"Bv3PKmgsXgqagUAZq7Ux2jqhRANCAAQc/KEqonVm+mcT4I1Gqz0onHHSXQhqICEG\n"
"1w2bbtfzYbkc7HnkEUyGHBdwus55Js8RoZdxcKjC9OTHsrgvOHts\n"
"-----END PRIVATE KEY-----\n";

/**
 * native demo key
 */
const BCHAR* native_demoKey = "0x78a42562c1d19843fd6f5a0f07de0206fdcf2a682c5e0a9a814019abb531da3a";

/**
 * test node url
 */
const BCHAR* demoUrl = "192.168.132.10:9545";

/**
 * transfer recipient address
 */
const BCHAR* demoRecipientAddress = "0x706578ff2a43709f92e7b358940ea23b19605f08";


BoatFiscobcosWallet* g_fiscobcos_wallet_ptr;

static int mWifiId = -1;

int boat_connect_wifi(const uint8_t* ssid, const char* psk)
{
    if (WifiConfig_ForgetAllNetworks() < 0)
    {
        printf( "ERROR: WifiConfig_ForgetAllNetworks failed: %s (%d).\n", strerror(errno), errno);
    }
    else
    {
        mWifiId = WifiConfig_AddNetwork();
        if (mWifiId < 0)
        {
            printf("ERROR: WifiConfig_AddNetwork failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetSecurityType(mWifiId, WifiConfig_Security_Wpa2_Psk) < 0)
        {
            printf("ERROR: WifiConfig_SetSecurityType failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetPSK(mWifiId, psk, strnlen(psk, WIFICONFIG_WPA2_KEY_MAX_BUFFER_SIZE)) < 0)
        {
            printf("ERROR: WifiConfig_SetPSK failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetSSID(mWifiId, ssid, strnlen(ssid, WIFICONFIG_SSID_MAX_LENGTH)) < 0)
        {
            printf("ERROR: WifiConfig_SetSSID failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetTargetedScanEnabled(mWifiId, true) < 0)
        {
            printf("ERROR: WifiConfig_SetTargetedScanEnabled failed: %s (%d).\n", strerror(errno), errno);
        }
        else if (WifiConfig_SetNetworkEnabled(mWifiId, true))
        {
            printf("ERROR: WifiConfig_SetNetworkEnabled failed: %s (%d).\n", strerror(errno), errno);
        }
        else
        {
            printf("SUCCESS: wifi");
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


#if defined(USE_ONETIME_WALLET)
__BOATSTATIC BOAT_RESULT fiscobcos_createOnetimeWallet()
{
    BSINT32 index;
    BoatFiscobcosWalletConfig wallet_config = { 0 };
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
#elif(defined USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
    //UtilityHexToBin(wallet_config.prikeyCtx_config.prikey_content, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
#else  
    /* default is internal generation */
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
    wallet_config.prikeyCtx_config.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
#endif

    strncpy(wallet_config.node_url_str, demoUrl, BOAT_FISCOBCOS_NODE_URL_MAX_LEN - 1);

    /* create fiscobcos wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, NULL, &wallet_config, sizeof(BoatFiscobcosWalletConfig));
    if (index < BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_fiscobcos_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT fiscobcos_createPersistWallet(BCHAR* wallet_name)
{
    BSINT32 index;
    BoatFiscobcosWalletConfig wallet_config = { 0 };
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
#elif (defined USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE) 
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

    strncpy(wallet_config.node_url_str, demoUrl, BOAT_FISCOBCOS_NODE_URL_MAX_LEN - 1);

    /* create fiscobcos wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, wallet_name, &wallet_config, sizeof(BoatFiscobcosWalletConfig));
    if (index < BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    g_fiscobcos_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

#if defined(USE_LOAD_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT fiscobcos_loadPersistWallet(BCHAR* wallet_name)
{
    BSINT32 index;

    /* create fiscobcos wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, wallet_name, NULL, sizeof(BoatFiscobcosWalletConfig));
    if (index < BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_fiscobcos_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}
#endif

BOAT_RESULT fiscobcos_helloworld(BoatFiscobcosWallet* wallet_ptr)
{
    BCHAR* result_str;
    BoatFiscobcosTx tx_ctx;
    BOAT_RESULT     result;

    /* Set Contract Address */
    result = BoatFiscobcosTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE,
        "0x11E1A300", //gasprice
        "0x33333333", //gaslimit
        (BCHAR*)demoRecipientAddress, //recipient
        "0x01", //chainid
        "0x01"  //groupid
    );
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_NORMAL, "BoatFiscobcosTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    result_str = HelloWorld_set(&tx_ctx, "HELLO FISCOBCOS!");
    if (result_str == NULL)
    {
        //BoatLog(BOAT_LOG_NORMAL, "HelloWorld_set failed: %s.", result_str);
        return BOAT_ERROR;
    }
    //BoatLog(BOAT_LOG_NORMAL, "set returns: %s", result_str);

    result_str = HelloWorld_get(&tx_ctx);
    if (result_str == NULL)
    {
        //BoatLog(BOAT_LOG_NORMAL, "HelloWorld_get failed: %s.", result_str);
        return BOAT_ERROR;
    }
    //BoatLog(BOAT_LOG_NORMAL, "get returns: %s", result_str);

    return BOAT_SUCCESS;
}



void boat_fiscobcos_entry(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: create fiscobcos wallet */
#if defined(USE_ONETIME_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = fiscobcos_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = fiscobcos_createPersistWallet("fiscobcos.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET)
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
    result = fiscobcos_loadPersistWallet("fiscobcos.cfg");
#else
    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
    return -1;
#endif	
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_CRITICAL, "fiscobcosWalletPrepare_create failed : %d.", result);
        return -1;
    }

    /* step-3: execute 'fiscobcos_call_helloworld' */
    result = fiscobcos_helloworld(g_fiscobcos_wallet_ptr);
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_NORMAL, "fiscobcos helloworld access Failed: %d.", result);
    }
    else
    {
        //BoatLog(BOAT_LOG_NORMAL, "fiscobcos helloworld access Passed.");
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
                printf("INFO: SSID \"%.*s\", BSSID %02x:%02x:%02x:%02x:%02x:%02x, Frequency %dMHz, %d dB.\n",
                network.ssidLength, network.ssid, network.bssid[0], network.bssid[1],
                network.bssid[2], network.bssid[3], network.bssid[4], network.bssid[5],
                network.frequencyMHz, network.signalRssi);

            BoatSleep(10);
            boat_fiscobcos_entry();
            break;
        }
        else
        {
            printf("wifi cannot connect");
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
        printf("ERROR: Create thread_boat_fiscobcos error\n");
    }
    else
    {
        printf("INFO: This is the thread_boat_fiscobcos process.\n");
    }
}


int main(int argc, char* argv[])
{
    printf("INFO: Application starting.\n");

    printf("INFO: Application init.\n");
    int err = init();

    boat_demo_entry();

    if (err < 0)
    {
        printf( "INFO: Application error.\n");
    }
    else
    {
        while (1)
        {
            ;
        }
    }

    while (!deinit(err)) {
        if (DEBUG) printf("INFO: Application deinit.\n");
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
        printf("ERROR: Cannot initialize Azure IoT Hub SDK.\n");
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
