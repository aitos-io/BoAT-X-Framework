

#include "osi_api.h"
#include "mg_os_api.h"
#include "cfw.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "sockets.h"
#include "osi_log.h"
#include "osi_api.h"


#include "boatiotsdk.h"
#include "api_platone.h"
#include "meig_http_api.h"

#include "oem.h"

//#include "osi_log_imp.h"

extern osiThread_t *meig_Net_IND_Handle;
extern osiThread_t *meig_TCP_Handle, *meig_baolan_lcd_Handle;

#define LOG_TAG OSI_MAKE_LOG_TAG('M', 'A', 'I', 'N')
#define PERSIST_WALLET_NAME     "/nvm/platone.cfg"

BoatPlatoneWallet *g_platone_wallet_ptr;
static uint8_t g_cid = 1;


__BOATSTATIC BOAT_RESULT platone_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, NULL, sizeof(BoatPlatoneWalletConfig));
    if (index < BOAT_SUCCESS)
	{
        MG_osiTracePrintf(LOG_TAG,"load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT platone_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BUINT8 binFormatKey[32] = {0};
   
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
    UtilityHexToBin(binFormatKey, 32, nativedemoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
#endif
 
	strncpy(wallet_config.node_url_str, "http://116.236.47.90:7545", BOAT_PLATONE_NODE_URL_MAX_LEN - 1); // "http://116.236.47.90:7545" //"http://106.14.94.165:8080"
 
	/* create platone wallet */
    //index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, PERSIST_WALLET_NAME, &wallet_config, sizeof(BoatPlatoneWalletConfig)); // "/nvm/test.txt"
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig)); 

    if (index < BOAT_SUCCESS)
	{
        MG_osiTracePrintf(LOG_TAG, "create one-time wallet failed.");
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

	MG_osiTracePrintf(LOG_TAG, "======node_url_ptr:%s", wallet_ptr->network_info.node_url_ptr);

	
    /* Set Contract Address */
    result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
							   "0x333333",
							   "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45",
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    MG_osiTracePrintf(LOG_TAG, "=====platone init result=%d", result);

    if (result != BOAT_SUCCESS)
	{
        MG_osiTracePrintf(LOG_TAG, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "Yanfei HelloWorld 0520");
    if (result_str == NULL)
	{
        MG_osiTracePrintf(LOG_TAG, "my_contract_cpp_abi_setName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	MG_osiTracePrintf(LOG_TAG, "setName returns: %s", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
	{
        MG_osiTracePrintf(LOG_TAG, "my_contract_cpp_abi_getName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	MG_osiTracePrintf(LOG_TAG, "getName returns: %s", result_str);
	
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
		MG_osiTracePrintf(LOG_TAG, "platoneWalletPrepare_create failed : %d.", result);
		return -1;
	}
	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract( g_platone_wallet_ptr );
    if (result != BOAT_SUCCESS)
	{
        MG_osiTracePrintf(LOG_TAG, "platone mycontract access Failed: %d.", result);
    }
	else
	{
        MG_osiTracePrintf(LOG_TAG, "platone mycontract access Passed.");
    }
	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
}

static bool getApn(char name[THE_APN_MAX_LEN])
{
	CFW_GPRS_PDPCONT_DFTPDN_INFO PdnInfo = {0,};
	memset(&PdnInfo, 0, sizeof(CFW_GPRS_PDPCONT_DFTPDN_INFO));
	MG_CFW_GprsGetDefaultPdnInfo(&PdnInfo, 0);
	memcpy(name, PdnInfo.AccessPointName, PdnInfo.APNLen);
	for (int i = 0; ((i < PdnInfo.APNLen) && (PdnInfo.AccessPointName[i] != '\0')); ++i)
	{
		if (PdnInfo.AccessPointName[i] == '.')
		{
			name[i] = '\0';
			break;
		}
		name[i] = PdnInfo.AccessPointName[i];
	}
	return (name[0]) ? false : true ;
}

static void simQuery(int *simStatus, int *registerStatus, char name[THE_APN_MAX_LEN])
{
	//int len = 8;
	if (simStatus == NULL || registerStatus == NULL) 
	{
		return;
	}
	
	MG_osiTracePrintf(LOG_TAG, "==SIM status : %d", *simStatus);
	*simStatus = MG_cfwGetSimStatus(0);
	MG_osiTracePrintf(LOG_TAG, "==SIM status : %d", *simStatus);

	while(getApn(name))
    {
		MG_osiThreadSleep(1000);
	}
	MG_osiTracePrintf(LOG_TAG, "Operators : %s", name);

	CFW_NW_STATUS_INFO sStatus;
	uint32_t uErrCode = MG_CFW_GprsGetstatus(&sStatus, 0);
	if (uErrCode == 0)
	{
		*registerStatus = sStatus.nStatus;
	}
	else
	{
		*registerStatus = -1;
	}
	MG_osiTracePrintf(LOG_TAG, "register status: %d", *registerStatus);

}

void boat_init_network(void)
{
	int simStatus = 0, registerStatus = 0;
	char name[THE_APN_MAX_LEN] = {0};
	uint8_t siglevel = 0;
	bool active = false;
	int count = 0;

	while(count < 50)
	{
		count++;
		MG_osiTracePrintf(LOG_TAG, "==hyc count=%d",count);
		MG_osiThreadSleep(1000);
	}
	count = 0;
	simQuery(&simStatus, &registerStatus, name);
	while(simStatus == 0)
    {
        MG_osiThreadSleep(1000);
        simStatus = MG_cfwGetSimStatus(0);
        MG_osiTracePrintf(LOG_TAG, "==sim status:%d",simStatus);
	}
    //MG_osiThreadSleep(28000);
	osiTracePrintf(LOG_TAG, "==boat_init_network name: %s", name);
	if (MG_NwModifyPdpContext(0, g_cid, 1, name))
	{
		MG_osiTracePrintf(LOG_TAG, "==meig modify pdp ctxt success");
        MG_osiThreadSleep(5000);
        
		MG_osiTracePrintf(LOG_TAG, "==meig modify pdp ctxt sleep");
        if (MG_NwActivePdpContext(0, g_cid))
            MG_osiTracePrintf(LOG_TAG, "==meig activate pdp ctxt success");
        else
            MG_osiTracePrintf(LOG_TAG, "==meig activate pdp ctxt failure");

		active = MG_NwHasActivePdp(0);
		while(!active)
		{
			MG_osiThreadSleep(1000);
			active = MG_NwHasActivePdp(0);
			MG_osiTracePrintf(LOG_TAG, "==no active pdp context,check again");
        }
	}
	else
	{
		MG_osiTracePrintf(LOG_TAG, "==meig modify pdp ctxt fail");
	}
	
}

void Boat_iot_Thread(void *param)
{
	boat_init_network();
	
	boat_platone_entry();
	
	MG_osiThreadExit();
}

int appimg_enter(void *param)
{
    MG_osiTracePrintf(LOG_TAG, "demo example enter");
    MG_osiThreadCreateE("boat_iot_entry", Boat_iot_Thread, NULL, OSI_PRIORITY_NORMAL, 24 * 1024, 10);

    return 0;
}

void appimg_exit(void)
{
    MG_osiTracePrintf(LOG_TAG, "demo example exit");
}



