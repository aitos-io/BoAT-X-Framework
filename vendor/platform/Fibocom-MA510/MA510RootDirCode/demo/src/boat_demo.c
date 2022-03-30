/*===========================================================================
                         boat_demo.c

DESCRIPTION
   This file is boat demo application.
===========================================================================*/

/* MA510 Includes ---------------------------------------------------------*/
#include "qflog_utils.h"
#include "qapi_uart.h"
#include "qapi_timer.h"

/* BoAT Includes ----------------------------------------------------------*/
#include "boatiotsdk.h"
#include "boatconfig.h"
#include "boatlog.h"
#include "my_contract.cpp.abi.h"


BoatPlatoneWallet *g_platone_wallet_ptr;

#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE
//#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS
#define BOAT_TEST
#define USE_ONETIME_WALLET
//#define USE_CREATE_PERSIST_WALLET
//#define USE_LOAD_PERSIST_WALLET

/**
 * PKCS format demo key. The original private key of 'pkcs_demoKey' is 
 * "fcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca"
 */

const BCHAR *pkcs_demoKey =  "-----BEGIN EC PRIVATE KEY-----\n"
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



#ifdef BOAT_TEST
__BOATSTATIC BOAT_RESULT platone_createOnetimeWallet()
{
    BoatLog(BOAT_LOG_NORMAL,"Ready to run platone_createOnetimeWallet\n");

    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]               = {0};

    (void)binFormatKey; //avoid warning

	/* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #endif
	
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, NULL, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if (index == BOAT_ERROR)
	{
        BoatLog(BOAT_LOG_CRITICAL,"create one-time wallet failed.\n");
        return BOAT_ERROR;
    }

    g_platone_wallet_ptr = BoatGetWalletByIndex(index);
 
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT platone_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config = {0};
    BUINT8 binFormatKey[32]               = {0};

    (void)binFormatKey; //avoid warning

	/* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #endif

    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
    strncpy(wallet_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, &wallet_config, sizeof(BoatPlatoneWalletConfig));
    if(index == BOAT_ERROR)
	  {
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR;
    }

    g_platone_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT platone_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create platone wallet */
  index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, NULL, sizeof(BoatPlatoneWalletConfig));
  if (index == BOAT_ERROR)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR;
  }
  g_platone_wallet_ptr = BoatGetWalletByIndex(index);

  return BOAT_SUCCESS;
}

BOAT_RESULT platone_call_mycontract(BoatPlatoneWallet *wallet_ptr)
{
  BCHAR *result_str;
  BoatPlatoneTx tx_ctx;
  BOAT_RESULT result;
  BoatLog(BOAT_LOG_NORMAL, "Ready to call contract!\n");

  /* Set Contract Address */
  result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
							   "0x333333",
							   (BCHAR *)demoRecipientAddress,
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

  if (result != BOAT_SUCCESS)
	{
    BoatLog(BOAT_LOG_CRITICAL,"BoatPlatoneTxInit fails.");
    return BOAT_ERROR;
  }

  result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
  if (result_str == NULL)
	{
    BoatLog(BOAT_LOG_CRITICAL,"my_contract_cpp_abi_setName failed: %s.\n", result_str);
		return BOAT_ERROR;
  }
	BoatLog(BOAT_LOG_NORMAL,"setName returns: %s\n", result_str);
    
  result_str = my_contract_cpp_abi_getName(&tx_ctx);
  if (result_str == NULL)
	{
    BoatLog(BOAT_LOG_CRITICAL,"my_contract_cpp_abi_getName failed: %s.\n", result_str);
		return BOAT_ERROR;
  }
	BoatLog(BOAT_LOG_NORMAL, "getName returns: %s\n", result_str);
	
  return BOAT_SUCCESS;
}


void boat_platone_entry(void)
{
  BOAT_RESULT  result  = BOAT_SUCCESS;
  int ret=0;
  BoatLog(BOAT_LOG_NORMAL,"======= Ready to INIT BoatIotSdk ======\n");

  
	/* step-1: Boat SDK initialization */
  result = BoatIotSdkInit();
  if(result != BOAT_SUCCESS)
  {
    BoatLog(BOAT_LOG_CRITICAL,"BoatIotSdkInit failed ,result = %d.\n", result);
    return;
  }

  /* step-2: create platone wallet */
#if defined(USE_ONETIME_WALLET)	
	BoatLog(BOAT_LOG_NORMAL,">>>>>>>>>> wallet type: create one-time wallet.");
	result = platone_createOnetimeWallet();
#elif defined(USE_CREATE_PERSIST_WALLET)
	BoatLog(BOAT_LOG_NORMAL,">>>>>>>>>> wallet type: create persist wallet.");
	result = platone_createPersistWallet("platone.cfg");
#elif defined(USE_LOAD_PERSIST_WALLET)
	BoatLog(BOAT_LOG_NORMAL,">>>>>>>>>> wallet type: load persist wallet.");
	result = platone_loadPersistWallet("platone.cfg");
#else
	BoatLog(BOAT_LOG_NORMAL,">>>>>>>>>> none wallet type selected.");
	return -1;
#endif	
  if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_CRITICAL,"platone create Wallet failed ,result = %d.", result);
		goto end;
	}
  BoatLog(BOAT_LOG_NORMAL,"=== platone：create_Wallet run success ===");
	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract( g_platone_wallet_ptr );
  if( result != BOAT_SUCCESS )
	{
      BoatLog(BOAT_LOG_CRITICAL,"platone_call_mycontract failed,result = %d.\n", result);
      goto end;
  }
  BoatLog(BOAT_LOG_NORMAL,"=== platone：call_mycontract run success ===");

end:	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

}

#endif




int fibocom_task_entry(void)
{
  int ret=0;
  qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
  
  debug_uart_init(&debug_uart_context_D);

  ret = dam_byte_pool_init();
  if(ret != TX_SUCCESS)
  {
    BoatLog(BOAT_LOG_CRITICAL,"Create DAM byte pool fail!\n");
    return ret;
  }

  /* BoAT demo task */
  boat_platone_entry();

   return 0;
}
