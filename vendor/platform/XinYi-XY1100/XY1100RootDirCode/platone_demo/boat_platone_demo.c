/*===========================================================================
                         boat_demo.c
DESCRIPTION
   This file is boat demo application.
===========================================================================*/

#include "boatiotsdk.h"
#include "my_contract.h"
#include "softap_api.h"
#include "boatconfig.h"


static int g_boat_task_Handle = -1;

//#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE
#define USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS
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


BoatPlatoneWallet *g_platone_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;


#ifdef BOAT_TEST
__BOATSTATIC BOAT_RESULT platone_createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]               = {0};

    (void)binFormatKey; //avoid warning


	/* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #endif
	

	/* create platone wallet */
#if defined(USE_ONETIME_WALLET)
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypairIndex = result;
    
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createNetwork()
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatPlatoneNetworkConfig network_config = {0};

    network_config.chain_id             = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

	/* create platone network */
#if defined(USE_ONETIME_WALLET)
    result = BoATPlatoneNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoATPlatoneNetworkCreate( &network_config, BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
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
    nodesResult result_out = {0,NULL};

    /* Set Contract Address */
    result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
							   "0x333333",
							   (BCHAR *)demoRecipientAddress,
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    result_str = BoatPlatoneGetNodesInfo(&tx_ctx,&result_out);
    for (BSINT32 i = 0; i < result_out.num; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL, "node[%d] : IP[%s],port[%d]. \n",i,result_out.nodeInfo[i].IP,result_out.nodeInfo[i].rpcPort);
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



void boat_platone_entry(void)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    int ret=0;
	/* step-1: Boat SDK initialization */
    result = BoatIotSdkInit();
    if(result != BOAT_SUCCESS)
    {
        xy_printf("BoatIotSdkInit failed ,result = %d.\n", result);
        return;

    }
        
    xy_printf("g_check_valid_sim = %d\n",g_check_valid_sim); 

    if((g_check_valid_sim == 1) && (1 == xy_wait_tcpip_ok(2*60)))
    {
       xy_printf("tcp/ip is ok!!!\n");
    }
    else
    {
       xy_printf("xy_wait_tcpip_ok error, ret=%x\n",ret);
       return ;
    }

    /* step-2: create platone wallet */
    result = platone_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, platone_demo_catch);
	}    
    result = createNetwork();
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, platone_demo_catch);
	}
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "platoneWalletPrepare_create failed : %d.", result);
		//return -1;
        boat_throw(result, platone_demo_catch);
	}
    g_platone_wallet_ptr = BoatPlatoneWalletInit(keypairIndex,networkIndex);
    if(g_platone_wallet_ptr == NULL){
        // BoatLog(BOAT_LOG_NORMAL,"BoatPlatoneWalletInit fail");
        boat_throw(BOAT_ERROR, platone_demo_catch);
    }

    xy_printf("======= platone: create_Wallet success ======\n");

	/* step-3: execute 'platone_call_mycontract' */
    result = platone_call_mycontract( g_platone_wallet_ptr );
    if( result != BOAT_SUCCESS )
    {
        xy_printf("platone_call_mycontract failed , result = %d.\n", result);
        goto end;
    }
    xy_printf("======= platone:call_mycontract success ======\n");

    boat_catch(platone_demo_catch)
    {
    }
end:	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

}

#endif

static void* boat_task_entry(void *args)
{
    xy_work_lock(1);
    xy_sleep(30000);

#ifdef BOAT_TEST
    boat_platone_entry();
#endif

    xy_work_unlock();
    xy_TaskDelete(g_boat_task_Handle);
    return (void *)0;
}

void boat_task_init()
{
    xy_TaskCreate((TaskFunc_t)boat_task_entry, "boat task", 20 * 1024, NULL, 10, &g_boat_task_Handle);
}

/*
* undef DEMO_TEST
* make all error, so add next code
*/
#ifndef DEMO_TEST
int at_encode_thread_id = -1;

void at_encode_creat_thread(char *encode_buf)
{
}
#endif

