/*===========================================================================
                         boat_demo.C

DESCRIPTION
   This file is boat demo application.
===========================================================================*/

/* MA510 Includes ---------------------------------------------------------*/
//#include "test_app_module.h"
#include "test_utils.h"
#include "qflog_utils.h"
#include "odm_ght_log.h"
#include "qapi_uart.h"
#include "qapi_timer.h"

/* BoAT Includes ----------------------------------------------------------*/
#include "boatiotsdk.h"
#include "boatconfig.h"
#include "boatlog.h"
#include "my_contract.cpp.abi.h"


#define DEBUG_ENTER_CRITICAL()                            do { __asm("cpsid i"); } while(0)
#define DEBUG_EXIT_CRITICAL()                             do { __asm("cpsie i"); } while(0)
#define DEBUG_RECEIVE_EVENT_MASK                          (0x00000001)
extern debug_uart_context_t debug_uart_context_D;
static void dam_cli_rx_cb(uint32_t num_bytes, void *cb_data)
{

  int Length = num_bytes;
  debug_uart_context_t *debug_uart_context_D = (debug_uart_context_t *)cb_data;
  if(debug_uart_context_D->Rx_Buffers_Free != 0)
  {
      /* See how much data can be read. */
      if(Length > debug_uart_context_D->Rx_Buffers_Free)
      {
         Length = debug_uart_context_D->Rx_Buffers_Free;
      }
      debug_uart_context_D->Rx_In_Index += Length;
      if(debug_uart_context_D->Rx_In_Index == 128)
      {
         debug_uart_context_D->Rx_In_Index = 0;
      }
      DEBUG_ENTER_CRITICAL();
      debug_uart_context_D->Rx_Buffers_Free -= Length;
      DEBUG_EXIT_CRITICAL();
      if(Length > 0)
      {
         /* Signal the event that data was received.                       */
          tx_event_flags_set((debug_uart_context_D->Rx_Event), DEBUG_RECEIVE_EVENT_MASK, TX_OR);
      }

  }
}

static void dam_cli_tx_cb(uint32_t num_bytes, void *cb_data)
{

/* This is a dummy callback as now UART QAPI's has made it mandatory to have a TX callback
*/
}



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
//const BCHAR *demoUrl = "http://121.5.173.139:8080/public/eth_gasPrice";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";



#ifdef BOAT_TEST
BOAT_RESULT platone_createOnetimeWallet()
{
    BoatLog(BOAT_LOG_NORMAL,"Ready to run platone_createOnetimeWallet1.\n");
    BoatLog(BOAT_LOG_NORMAL,"Ready to run platone_createOnetimeWallet2.\n");
    BoatLog(BOAT_LOG_NORMAL,"Ready to run platone_createOnetimeWallet3.\n");
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

BOAT_RESULT platone_createPersistWallet(BCHAR *wallet_name)
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

BOAT_RESULT platone_loadPersistWallet(BCHAR *wallet_name)
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

  result_str = my_contract_cpp_abi_setName(&tx_ctx, "zzq_HelloWorld");
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
  BoatLog(BOAT_LOG_NORMAL,"=== platone create Wallet run success ===");

	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract( g_platone_wallet_ptr );
  if( result != BOAT_SUCCESS )
	{
      BoatLog(BOAT_LOG_CRITICAL,"platone_call_mycontract failed,result = %d.\n", result);
      goto end;
  }
  BoatLog(BOAT_LOG_NORMAL,"=== platone_call_mycontract run success ===");

end:	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

}

#endif




int fibocom_task_entry(void)
{

   qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
  
   qapi_UART_Open_Config_t open_properties;

   memset (&open_properties, 0, sizeof (open_properties));
   open_properties.parity_Mode = QAPI_UART_NO_PARITY_E;
   open_properties.num_Stop_Bits = QAPI_UART_1_0_STOP_BITS_E;
   open_properties.baud_Rate   = 115200;

   open_properties.bits_Per_Char = QAPI_UART_8_BITS_PER_CHAR_E;
   open_properties.rx_CB_ISR = dam_cli_rx_cb;
   open_properties.tx_CB_ISR = dam_cli_tx_cb;

   open_properties.enable_Flow_Ctrl = false;
   open_properties.enable_Loopback= false;

  if(qapi_UART_Open(debug_uart_context_D.uart_handle, QAPI_UART_PORT_012_E, &open_properties) != QAPI_OK)
  {
       return -1;
  }


  /* BoAT demo task ----------------------------------------------------------*/
  boat_platone_entry();


  while(1){
    QFLOG_MSG(MSG_SSID_DFLT,MSG_MASK_2,"123_QFLOG_MSG");
    qapi_UART_Transmit(debug_uart_context_D.uart_handle, "123", strlen("123"), NULL);
    qapi_UART_Transmit(debug_uart_context_D.uart_handle, "\r\n", strlen("\r\n"), NULL);

    qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);
  }

   return 0;
}
