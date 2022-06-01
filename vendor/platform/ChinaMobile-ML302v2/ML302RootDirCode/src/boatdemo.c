#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "cm_main.h" 

#include "boatiotsdk.h"
#include "api_platone.h"

#define PERSIST_WALLET_NAME     "/nvm/platone.cfg"
BoatPlatoneWallet *g_platone_wallet_ptr;

struct   in_addr test_remote_addr;
uint16_t test_remote_port;
char test_msg[] = "network test\n";
char test_rx_buf[100];
int cm_global_socket_id = -1;
osThreadId_t cm_socket_TaskHandle = 0;
unsigned char cm_socket_buf[1024] = {0};
static void _cm_socket_select_task(void *not_used);
/*
网络初始化配置
*/
static bool bNetAutoRegNeedReinit = 0; /* 标识自动网络注册需要初始化 */

//自动注网模块需要添加ATCHART模块才能生效
#ifdef CM_NET_AUTOREG_SUPPORT

#ifndef CM_ATCHART_SUPPORT
#error "NET_AUTOREG relies on ATCHART"
#endif
static int net_ready = 0;
//网络自注册接口回调函数，通知网络状态变化
static void network_state_change(cm_network_state_e state)
{
	cm_printf("[NET_REGISTER]:state:%d\n",state);
	switch(state)
	{
		//自动注网模块初始化
		case CM_NETWORK_STATE_INIT:
			cm_printf("[NET_REGISTER]:Module init\n");
			break;
		//无SIM卡
		case CM_NETWORK_STATE_NO_SIM:
			cm_printf("[NET_REGISTER]:No SIM\n");
			break;
		//SIM卡识别成功
		case CM_NETWORK_STATE_SIM_READY:
			cm_printf("[NET_REGISTER]:SIM ready\n");
			break;
		//正在附着网络，CEREG:0,2
		case CM_NETWORK_STATE_ATTACHING:
			cm_printf("[NET_REGISTER]:Attaching\n");
			break;
		//网络附着失败，CEREG:0,0
		case CM_NETWORK_STATE_ATTACH_FAIL:
			cm_printf("[NET_REGISTER]:Attach fail\n");
			break;
		//网络附着成功，CEREG:0,1
		case CM_NETWORK_STATE_ATTACH_SUCCESS:
			cm_printf("[NET_REGISTER]:Attach success\n");
			break;
		//正在激活PDP
		case CM_NETWORK_STATE_PDP_ACTIVATING:
			cm_printf("[NET_REGISTER]:PDP activating\n");
			break;
		//PDP激活失败
		case CM_NETWORK_STATE_PDP_NOT_READY:
			cm_printf("[NET_REGISTER]:PDP not ready\n");
			break;
		//PDP激活成功
		case CM_NETWORK_STATE_PDP_READY:
			net_ready = 1;
			cm_printf("[NET_REGISTER]:PDP ready\n");
			break;
		default:
			cm_printf("[NET_REGISTER]:Module stop\n");
			
	}
}
/**
 *  \brief 网络自注册模块初始化
 *  
 *  \return 空
 *  
 *  \details More details
 */
static void cm_test_net_register_init(void)
{
	//网络配置结构体
	cm_network_param_t network_param;
	
	//填写APN等信息，
	network_param.cid = 1;
	network_param.apn = "CMNET";
	network_param.pdp_type = "IP";
	network_param.use_auth = 0;

	
	//初始化
	cm_net_register_init();	
	//网络参数配置
	cm_net_register_config(&network_param);
	//设置网络状态变化时的回调函数
	cm_net_register_set_callback(network_state_change);
	//启动网络自注册模块
	cm_net_register_start();
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
	//等待时间
	int retry_count = 50*60; 
	//将成功标志设为0
	net_ready = 0;
	//手动触发网络注册模块更新状态，确保获取到最新网络状态
	cm_net_register_update_status();
	//等待一段时间，当超过时间，network_state_change回调函数仍没有返回CM_NETWORK_STATE_PDP_READY，说明网络注册可能遇到问题。
	while((net_ready == 0)&&(retry_count>0))
	{
		osDelay(1);
		retry_count--;
	}
	if(net_ready != 0) //网络注册成功
	{
		cm_printf("network ready\n");
		return 0;
	}
	else //网络注册尚未成功
	{
		cm_printf("network not ready\n");
		return -1;
	}
		
	
}
#endif
/**
 *  \brief 网络注册配置
 *  
 *  \return 空
 *  
 *  \details More details	
 */
void cm_test_network_config(void)
{
#ifdef CM_NET_AUTOREG_SUPPORT
	//采用ATCHART模块实现的网络自注册方式，实现自动注网
    cm_test_net_register_init();
	cm_test_wait_for_network();
#else
	//旧网络注册方法，直接使用AT虚拟通道发送AT命令进行网络注册，后续版本可能不再支持，在这种情况下不能使能ATCHART模块，否则会造成冲突
#ifndef CM_ATCHART_SUPPORT
	 int i;
     unsigned char *p =NULL;

	//查询SIM卡
    for(i = 0;i< 100;i++)
    {
        p = NULL;
        p = cm_send_atchart("AT+CPIN?\r\n");
        if(p !=NULL)
        {
            if(strstr(p,"READY") )
            {
                break;
                
            }   
        }
    }        
    if(i>=100)
    {
        cm_printf("sim card error\n");
        cm_test_dump_at(0,0);
        return;
    }
    
    p =NULL;
    p = cm_send_atchart("AT+CFUN=1\r\n");
    if(p !=NULL)
    {
        if(strstr(p,"OK") ==NULL)
        {
            cm_printf("cfun error\n");
            return;
            
        }   
    }
    
    //查询CEREG
    for(i = 0;i< 120;i++)
    { 
        p =NULL;
        p = cm_send_atchart("AT+CEREG?\r\n");
        if(p !=NULL)
        {
            if(strstr(p,"0,1") ||strstr(p,"1,5") ||strstr(p,"0,5") ||strstr(p,"1,1"))
            {
                break;       
            }   
        }
        osDelay(50);
    }     
    
    if(i>=120)
    {
        cm_printf("cereg error\n");
        cm_test_dump_at(0,0);
        return;
    }
    
	//配置APN
    p =NULL;
    p = cm_send_atchart("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
    if(p !=NULL)
    {
        if(strstr(p,"OK") ==NULL)
        {
            cm_printf("CGDCONT error\n");
            return;
        }   
    }
    
	//激活PDP
    p =NULL;
    p = cm_send_atchart("AT+CGACT=1,1\r\n");
    if(p !=NULL)
    {
        if(strstr(p,"OK") ==NULL)
        {
            cm_printf("cgact error\n");
            return;
        }   
    }
    cm_printf("network ready\n");
#else
	cm_printf("Network Not Config\n");
#endif
#endif
}

__BOATSTATIC BOAT_RESULT platone_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, wallet_name, NULL, sizeof(BoatPlatoneWalletConfig));
    if (index < 0)
	{
        cm_sys_log("load wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_platone_wallet_ptr = BoatGetWalletByIndex(index);

    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT platone_createOnetimeWallet()
{
    BSINT32 index;
    BoatPlatoneWalletConfig wallet_config;
    BUINT8 binFormatKey[32]               = {0};
   
	/* wallet_config value assignment */
	/* for one-time wallet, the 'prikeyId' field should be cleared */
	//memset(wallet_config.prikeyId, 0, BOAT_KEYID_MAX_LEN); 
    wallet_config.chain_id             = 1;
    wallet_config.eip155_compatibility = BOAT_FALSE;

    char *nativedemoKey = "0xfcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca";
    wallet_config.prikeyCtx_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.prikeyCtx_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
    wallet_config.prikeyCtx_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_ptr = binFormatKey;
    UtilityHexToBin(wallet_config.prikeyCtx_config.prikey_content.field_ptr, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;

 
	strncpy(wallet_config.node_url_str, "http://116.236.47.90:7545", BOAT_PLATONE_NODE_URL_MAX_LEN - 1); // "http://116.236.47.90:7545" //"http://106.14.94.165:8080"
 
	cm_sys_log("======node_url_str:%s", wallet_config.node_url_str);

	/* create platone wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_PLATONE, PERSIST_WALLET_NAME, &wallet_config, sizeof(BoatPlatoneWalletConfig)); // "/nvm/test.txt"

    if (index < 0)
	{
        cm_sys_log("create one-time wallet failed.");
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


	
    /* Set Contract Address */
    result = BoatPlatoneTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
							   "0x333333",
							   "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45",
							   BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR);

    if (result != BOAT_SUCCESS)
	{
        cm_sys_log("BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    result_str = my_contract_cpp_abi_setName(&tx_ctx, "ML302 HelloWorld 0519");
    if (result_str == NULL)
	{
        cm_sys_log("my_contract_cpp_abi_setName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	cm_sys_log("setName returns: %s", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if (result_str == NULL)
	{
        cm_sys_log("my_contract_cpp_abi_getName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	cm_sys_log("getName returns: %s", result_str);
	
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
    if (result != BOAT_SUCCESS)
	{
		cm_sys_log("platoneWalletPrepare_create failed : %d.", result);
		return -1;
	}
	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract(g_platone_wallet_ptr);
    if (result != BOAT_SUCCESS)
	{
        cm_sys_log("platone mycontract access Failed: %d.", result);
    }
	else
	{
        cm_sys_log("platone mycontract access Passed.");
    }
	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
}


void cm_main_task(void *p)
{  
    boat_platone_entry();
    while (1)
    {
        osDelay(5000);
    }
}

void ML302_OpenCPU_Entry()
{
    cm_test_net_register_init();

	osThreadAttr_t main_task_attr = {0};
	main_task_attr.name  = "main_task";
	main_task_attr.stack_size = 8192;

	osDelay(10);
    OC_Main_TaskHandle = osThreadNew(cm_main_task, 0, &main_task_attr);    
}

//SDK中调用，不可删除
int cm_wdt_config_cb(void)
{
    return 0;//关闭看门狗
    //return 1;//开启看门狗
}
