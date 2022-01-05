
//#include <timers.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "boatwallet.h"
#include "nwy_osi_api.h"
#include "boatiotsdk.h"
#include "api_platone.h"

#include "nwy_network.h"
#include "nwy_osi_api.h"
#include "nwy_data.h"

nwy_osiThread_t *g_boatsdk_thread = NULL;

BoatPlatoneWallet *g_platone_wallet_ptr;

// printf log
#define BOAT_NWY_EXT_SIO_RX_MAX          (2*1024)

static void boat_nwy_ext_echo(char* fmt, ...)
{
    static char    echo_str[BOAT_NWY_EXT_SIO_RX_MAX];
	static nwy_osiMutex_t *echo_mutex = NULL;
    va_list     a;
	int i, size;

	if(NULL == echo_mutex)
		echo_mutex = nwy_create_mutex();
	if(NULL == echo_mutex)
		return;
	nwy_lock_mutex(echo_mutex, 0);	
    va_start(a, fmt);
    vsnprintf(echo_str, BOAT_NWY_EXT_SIO_RX_MAX, fmt, a);
    va_end(a);
	size = strlen((char *)echo_str);
	i = 0;
	while(1)
    {
    	int tx_size;
		
    	tx_size = nwy_usb_serial_send((char *)echo_str + i, size - i);
		if(tx_size <= 0)
			break;
		i += tx_size;
		if((i < size))
			nwy_sleep(10);
		else
			break;
	}
	nwy_unlock_mutex(echo_mutex);
}
// end of printf log
BUINT8 prikeyContent[32] = {0}; 

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
    UtilityBinToHex( prikeyContent, nativedemoKey, 32, BIN2HEX_LEFTTRIM_QUANTITY, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
	wallet_config.prikeyCtx_config.prikey_content.field_ptr = prikeyContent;
    wallet_config.prikeyCtx_config.prikey_content.field_len = 32;
#endif

    strncpy( wallet_config.node_url_str, "http://116.236.47.90:7545", BOAT_PLATONE_NODE_URL_MAX_LEN - 1 ); // "http://116.236.47.90:7545"

	/* create platone wallet */
    index = BoatWalletCreate( BOAT_PROTOCOL_PLATONE, "platone.cfg", &wallet_config, sizeof(BoatPlatoneWalletConfig) );
    if( index < BOAT_SUCCESS )
	{
        boat_nwy_ext_echo("create one-time wallet failed." );
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
        boat_nwy_ext_echo("BoatPlatoneTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    boat_nwy_ext_echo("N58 HelloWorld 0408");
    result_str = my_contract_cpp_abi_setName(&tx_ctx, "N58 HelloWorld 0430");
    if( result_str == NULL )
	{
        boat_nwy_ext_echo("my_contract_cpp_abi_setName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	boat_nwy_ext_echo("setName returns: %s", result_str);
    
    result_str = my_contract_cpp_abi_getName(&tx_ctx);
    if( result_str == NULL )
	{
        boat_nwy_ext_echo("my_contract_cpp_abi_getName failed: %s.", result_str);
		return BOAT_ERROR;
    }
	boat_nwy_ext_echo("getName returns: %s", result_str);
	
    return BOAT_SUCCESS;
}

void boat_platone_entry(void)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    /* step-2: create platone wallet */
    result = platone_createOnetimeWallet();
    if( result != BOAT_SUCCESS )
	{
		 boat_nwy_ext_echo("platoneWalletPrepare_create failed : %d.", result);
		return -1;
	}
	/* step-3: execute 'platone_call_mycontract' */
	result = platone_call_mycontract( g_platone_wallet_ptr );
    if( result != BOAT_SUCCESS )
	{
        boat_nwy_ext_echo("platone mycontract access Failed: %d.", result);
    }
	else
	{
        boat_nwy_ext_echo("platone mycontract access Passed.");
    }
	
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
}

static int ppp_state[10] = {0};
static void nwy_data_cb_fun(   
    int hndl,
    nwy_data_call_state_t ind_state)
{
  if (hndl > 0 && hndl <= 8)
  {
    ppp_state[hndl-1] = ind_state;
    boat_nwy_ext_echo("\r\nData call status update, handle_id:%d,state:%d\r\n",hndl,ind_state);
  }
}

void boat_nwy_data_start(void)
{
    int ret = -1;
    char data_buff[256] = {0};
    nwy_data_profile_info_t profile_info;
    int hndl;
    int profile_id = 1;
    int auto_interval_ms = 200;
    //int len = 0;
    nwy_data_start_call_v02_t param_t;
    //nwy_data_addr_t_info addr_info;
        
    hndl = nwy_data_get_srv_handle(nwy_data_cb_fun);
    boat_nwy_ext_echo("\r\nCreate a Resource Handle id: %d success\r\n",hndl);

    //set profile_info
    memset(&profile_info,0,sizeof(nwy_data_profile_info_t));
    profile_info.auth_proto = 0;
    profile_info.pdp_type = 1;
    memset(data_buff, 0, sizeof(data_buff));
    memcpy(data_buff, "CMIOT", strlen("CMIOT") + 1);

    memcpy(profile_info.apn, data_buff, sizeof(profile_info.apn));

    memset(data_buff, 0, sizeof(data_buff));
    memcpy(data_buff, "gsm", strlen("gsm") + 1);
    memcpy(profile_info.user_name, data_buff, sizeof(profile_info.user_name));

    memset(data_buff, 0, sizeof(data_buff));
    memcpy(data_buff,"1234",strlen("1234") + 1);
    memcpy(profile_info.pwd,data_buff,sizeof(profile_info.pwd));

    ret = nwy_data_set_profile(profile_id,NWY_DATA_PROFILE_3GPP,&profile_info);
    if (ret != NWY_RES_OK)
    {
        boat_nwy_ext_echo("\r\nSet profile %d info fail, result<%d>\r\n",profile_id,ret);
    }
    else 
    {
        boat_nwy_ext_echo("\r\nSet profile %d info success\r\n",profile_id);
    }
    nwy_sleep(5000);

    //data start
    memset(&param_t,0,sizeof(nwy_data_start_call_v02_t));
    param_t.profile_idx = profile_id;

    param_t.is_auto_recon = 1;
    param_t.auto_re_max = 0;
    param_t.auto_interval_ms = auto_interval_ms;

    ret = nwy_data_start_call(hndl,&param_t);
    if (ret != NWY_RES_OK)
    {
        boat_nwy_ext_echo("\r\nStart data call fail, result<%d>\r\n",ret);
    }
    else 
    {
        boat_nwy_ext_echo("\r\nStart data call sucess \r\n");
    }
}

static void prvBoatSdkThreadEntry(void *param)
{
    nwy_sleep(50*1000);
    //ret = nwy_sdk_at_cmd_send(&at_cmd, resp, NWY_AT_TIMEOUT_DEFAULT);
    boat_nwy_data_start();
    
    nwy_sleep(40*1000);
    boat_platone_entry();
    
    nwy_exit_thread();
}

void boat_demo_entry(void)
{
    g_boatsdk_thread = nwy_create_thread("boatsdkthread", prvBoatSdkThreadEntry, NULL, NWY_OSI_PRIORITY_NORMAL, 1024*28, 4);
}
nwy_osiThread_t *boat_get_app_thread(void)
{
    return g_boatsdk_thread;
}

static const char APP_VERSION[65] = "BOATSDK_APP_V1.0.1";

int appimg_enter(void *param)
{
    //warning:app build time length <= 64
    char APP_BUILD_TIME[65]= {0};
    char version[70]={0};

    sprintf(version,"\"%s\"", APP_VERSION);
    sprintf(APP_BUILD_TIME,"\"%s,%s\"", __DATE__,__TIME__);    
    if(false == nwy_app_version(version, APP_BUILD_TIME))
    {
      boat_nwy_ext_echo("app set version fail");
      return 0;
    }
    
    boat_demo_entry();
    return 0;
}

void appimg_exit(void)
{
    boat_nwy_ext_echo("application image exit");
}

#if 1 // 链接报错，根据实际情况决定是否定义
void __aeabi_l2d(void){};
void __aeabi_d2lz(void){};
void gettimeofday(void){};
void __aeabi_ul2d(void){};
#endif

