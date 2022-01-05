#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "onemo_main.h" 

#include "boatiotsdk.h"
#include "api_platone.h"

osThreadId OC_Main_TaskHandle;

osMutexDef( VIR_AT_Mutex );
osMutexId vir_at_mutex;

#define PERSIST_WALLET_NAME     "/nvm/platone.cfg"

#define BOATDEMO_VIRT_AT_RESPONSE_BUF_LEN 2048

unsigned char virt_at_buffer[BOATDEMO_VIRT_AT_RESPONSE_BUF_LEN];
int virt_at_len = 0;

BoatPlatoneWallet *g_platone_wallet_ptr;


void boatdemo_clear_virt_at_buffer()
{
    osMutexWait( vir_at_mutex, osWaitForever );
    virt_at_len = 0;
    osMutexRelease( vir_at_mutex );
}


unsigned char * boatdemo_check_at_str(const char *str)
{
    int i,j; 
    unsigned char *p;
    int at_len;
    
    osMutexWait( vir_at_mutex, osWaitForever );
    at_len = virt_at_len;
    osMutexRelease( vir_at_mutex );

    if(at_len > 0)
    {
        for(i = 0; i < at_len; i++) //将有效长度内所有含0的地方替换成'!'，以方便进行字符串检测
        {
            if(virt_at_buffer[i] == 0)
            {
                virt_at_buffer[i] = '!';
            }
        }
        virt_at_buffer[at_len] = 0;
        p = strstr((const char *)virt_at_buffer,str);
        if(p != 0)
            return p;
    }

    return 0;
}


int boatdemo_wait_for_at_response(const char * response_to_wait, int delay)
{
    int i;

    osSignalWait(0x0004, osWaitForever);
    
    for( i = 0; i < delay; i++ )
    {
        if(virt_at_len >= BOATDEMO_VIRT_AT_RESPONSE_BUF_LEN)
        {
            osMutexWait( vir_at_mutex, osWaitForever );
            virt_at_len = 0;
            osMutexRelease( vir_at_mutex );
        }
        osMutexWait( vir_at_mutex, osWaitForever );
        virt_at_len += onemo_vir_at_get(virt_at_buffer+virt_at_len, BOATDEMO_VIRT_AT_RESPONSE_BUF_LEN - virt_at_len);
        osMutexRelease( vir_at_mutex );
        
        if( boatdemo_check_at_str(response_to_wait) != 0 )
        {
            break;
        }
        else
        {
            osDelay(100);
        }
    }
    
    if( i < delay )
    {
        // The response string matched
        return 0;
    }
    else
    {
        return -1;
    }
}

void boatdemo_at_callback(void *param)
{
    osSignalSet(OC_Main_TaskHandle, 0x0004);
}

void boatdemo_at_init(void)
{
    vir_at_mutex = osMutexCreate( osMutex(VIR_AT_Mutex) );
    onemo_vir_at_init(boatdemo_at_callback);
}

void boatdemo_at_deinit(void)
{
    osMutexDelete(vir_at_mutex);
    onemo_virt_at_deinit();
}


__BOATSTATIC BOAT_RESULT platone_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, NULL, sizeof(BoatPlatoneWalletConfig));
    if( index < BOAT_SUCCESS )
	{
        onemo_sys_log("load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex( index );

    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT platone_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
   
	/* wallet_config value assignment */
	/* for one-time wallet, the 'prikeyId' field should be cleared */
	//memset(wallet_config.prikeyId, 0, BOAT_KEYID_MAX_LEN); 
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;
#if 1
    char * nativedemoKey = "0xfcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca";
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin( wallet_config.prikeyCtx_config.prikey_content.field_ptr, 32, nativedemoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
#endif
 
	strncpy( wallet_config.node_url_str, "http://116.236.47.90:7545", BOAT_PLATONE_NODE_URL_MAX_LEN - 1 ); // "http://116.236.47.90:7545" //"http://106.14.94.165:8080"
 
	onemo_sys_log("======node_url_str:%s", wallet_config.node_url_str);

	/* create platone wallet */
    index = BoatWalletCreate( BOAT_PROTOCOL_PLATONE, PERSIST_WALLET_NAME, &wallet_config, sizeof(BoatPlatoneWalletConfig) ); // "/nvm/test.txt"

    if( index < BOAT_SUCCESS )
	{
        onemo_sys_log("create one-time wallet failed." );
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex( index );
    
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
							   "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45",
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if( result != BOAT_SUCCESS )
	{
        onemo_sys_log("BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "ML302 HelloWorld 0519");
    if( result_str == NULL )
	{
        onemo_sys_log("my_contract_cpp_abi_setName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	onemo_sys_log("setName returns: %s", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if( result_str == NULL )
	{
        onemo_sys_log("my_contract_cpp_abi_getName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	onemo_sys_log("getName returns: %s", result_str);
	
    return BOAT_SUCCESS;
}

void boat_platone_entry(void)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    /* step-2: create platone wallet */
    result = platone_createOnetimeWallet();
    //result = platone_loadPersistWallet(PERSIST_WALLET_NAME);
    if( result != BOAT_SUCCESS )
	{
		 onemo_sys_log("platoneWalletPrepare_create failed : %d.", result);
		return -1;
	}
	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract( g_platone_wallet_ptr );
    if( result != BOAT_SUCCESS )
	{
        onemo_sys_log("platone mycontract access Failed: %d.", result);
    }
	else
	{
        onemo_sys_log("platone mycontract access Passed.");
    }
	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
}

int boatdemo_wait_for_4g_ready(void)
{
    int i;
    /**************************************************/

    onemo_sys_log("== wait_for_4g 1");
    
    for(i = 0;i< 100;i++)
    {
        onemo_sys_log("Try AT+CPIN?");
        boatdemo_clear_virt_at_buffer();
        onemo_vir_at_send("AT+CPIN?\r\n");
        if(boatdemo_wait_for_at_response("READY", 2) == 0)
        {
            onemo_sys_log("READY");
            break;
        }
        osDelay(1000);
    }
    onemo_sys_log("== wait_for_4g 2");
    if( i >= 100 )
    {
        onemo_sys_log("sim card error\n");
        return -1;
    }
    onemo_sys_log("== wait_for_4g 3");
    /**************************************************/
    onemo_sys_log("Try AT+CFUN=1");
    boatdemo_clear_virt_at_buffer();
    onemo_vir_at_send("AT+CFUN=1\r\n");
    if(boatdemo_wait_for_at_response("OK", 10) == 0)
    {
        onemo_sys_log("OK");
    }
    else
    {
        onemo_sys_log("cfun error\n");
        return -1;
    }
    onemo_sys_log("== wait_for_4g 4");
   
    /**************************************************/
    for(i = 0;i< 30;i++)
    { 
        onemo_sys_log("Try AT+CEREG?");
        boatdemo_clear_virt_at_buffer();
        onemo_vir_at_send("AT+CEREG?\r\n");
        if(boatdemo_wait_for_at_response("1,1", 10) == 0)
        {
            onemo_sys_log("1,1");
            break;
        }

        boatdemo_clear_virt_at_buffer();
        onemo_vir_at_send("AT+CEREG?\r\n");
        if(boatdemo_wait_for_at_response("0,1", 10) == 0)
        {
            onemo_sys_log("0,1");
            break;
        }
        osDelay(1000);
    }        
    onemo_sys_log("== wait_for_4g 5");
    if(i>=30)
    {
        onemo_sys_log("cereg error\n");
        return -1;
    }
    
    onemo_sys_log("Try AT+CGDCONT=1,\"IP\",\"CMNET\"");
    boatdemo_clear_virt_at_buffer();
    onemo_vir_at_send("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
    if(boatdemo_wait_for_at_response("OK", 10) == 0)
    {
        onemo_sys_log("OK");
    }
    else
    {
        onemo_sys_log("CGDCONT error\n");
        return -1;
    }    
    onemo_sys_log("== wait_for_4g 6");
    onemo_sys_log("Try AT+CGACT=1,1");
    boatdemo_clear_virt_at_buffer();
    onemo_vir_at_send("AT+CGACT=1,1\r\n");
    if(boatdemo_wait_for_at_response("OK", 30) == 0)
    {
        onemo_sys_log("OK");
    }
    else
    {
        onemo_sys_log("cgact error\n");
        return -1;
    }
    onemo_sys_log("== wait_for_4g 7");
    onemo_sys_log("network ready\n");
    return 0;
}

void onemo_main_task(void *p)
{  

    int i;
    int ret;
   
    // Wait for CoolWatcher to (re-try) connect
    for (i = 0; i < 50; i++)
    {
        onemo_sys_log("===Wait for CoolWatcher connection conut:%d", i);
        osDelay(1000);
    }
    onemo_sys_log("== before 4g");

    boatdemo_at_init(); 
    
    onemo_sys_log("== before 4g 1");
    ret = boatdemo_wait_for_4g_ready();
    
    onemo_sys_log("== end 4g");
    boatdemo_at_deinit();
    
    onemo_sys_log("== end 4g 1");
    if( ret == 0 )
    {
        boat_platone_entry();
    }
    
    
   #ifdef ONEMO_FOTA_SUPPORT   //启用OneNet FOTA不可删除
    char *version = NULL;
    onVersion(&version);
    onemo_printf("OneNet FOTA version:%s\n", version);
    #endif

    while(1)
    {
        osDelay(5000);
    }
}



void ML302_OpenCPU_Entry()
{
    osThreadDef(OC_Main_Task, onemo_main_task, osPriorityNormal, 0, 12*1024);
    OC_Main_TaskHandle = osThreadCreate(osThread(OC_Main_Task), 0);  
#ifdef ONEMO_FOTA_SUPPORT   //启用OneNet FOTA不可删除
    onemo_onenet_fota_regcbex();
#endif
}
//SDK中调用，不可删除
int onemo_wdt_config_cb(void)
{
    return 0;//关闭看门狗
    //return 1;//开启看门狗
}
