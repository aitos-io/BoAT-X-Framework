/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

//BoAT head file
#include "boatiotsdk.h"

//demo contract head file
#include "my_contract.cpp.abi.h"

//FM650 head file
#include "fibo_oe.h"

#define BUF_SIZE        512

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
const BCHAR *demoUrl = "http://121.0.0.1:7545";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";


BoatPlatoneWallet *g_platone_wallet_ptr;



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
        //BoatLog(BOAT_LOG_NORMAL, "node[%d] : IP[%s],port[%d]. \n",i,result_out.nodeInfo[i].IP,result_out.nodeInfo[i].rpcPort);
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


BOAT_RESULT demo_call_BoAT()
{
	BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;

	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
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
	boat_catch(platone_demo_catch)
    {
    }
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return result;
}


char *addr2str(fibo_data_call_addr_t *addr, char *buf, int len, int *pIsIpv6)
{
    char *ptr;
    int i;
    int isIpv6 = 0;

    if(addr->valid_addr==0)
    {
        return NULL;
    }
    
    if(addr->addr.ss_family == AF_INET6)
    {
        isIpv6 = 1;
    }

    if(pIsIpv6)
    {
        pIsIpv6[0] = isIpv6;
    }
    if(isIpv6==0)
    {
        snprintf(buf, len, "%d.%d.%d.%d", addr->addr.__ss_padding[0], 
                addr->addr.__ss_padding[1],addr->addr.__ss_padding[2],addr->addr.__ss_padding[3]);
    }
    else {
        inet_ntop(AF_INET6, &(addr->addr.__ss_padding), buf, len);
    }

    return buf;
}

void fibo_data_call_evt_cb(e_fibo_data_net_event_msg_id_t ind_flag, 
    void                  *ind_msg_buf, 
    uint32_t              ind_msg_len)
{
    fibo_data_call_indication_t *data_call_ind = (void *)ind_msg_buf;

    switch(ind_flag)
    {
    case E_FIBO_DATA_NET_UP_EVENT:
        {
            int is_ipv6 = 0;
            int idx = 0;
            char *ptr = NULL;
            char tmpBuf[BUF_SIZE] = {0};
            fibo_data_call_addr_info_t *addr = NULL;
            printf("DATA UP device name:%s,profile:%d,call_id:%d\n",data_call_ind->addr_info_list.iface_name,data_call_ind->profile_idx,data_call_ind->call_id);
            for(idx = 0; idx < data_call_ind->addr_info_list.addr_info_num; idx ++)
            {
                addr = &data_call_ind->addr_info_list.addr_info[idx];

                ptr = addr2str(&addr->iface_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
                if(ptr)
                {
                        printf("ipaddr : %s/%d \n", ptr, addr->iface_mask);
                }
                else
                {
                    printf("Failed to parse addr\n");
                }

                ptr = addr2str(&addr->gtwy_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
                if(ptr)
                {
                    printf("gateway : %s/%d \n", ptr, addr->gtwy_mask);
                }
                else
                {
                    printf("Failed to parse addr\n");
                }

                ptr = addr2str(&addr->dnsp_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
                if(ptr) {
                    printf("Primary DNS : %s\n", ptr);
                }
                else
                {
                    printf("Failed to parse addr\n");
                }

                ptr = addr2str(&addr->dnss_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
                if(ptr) {
                    printf("Secondary DNS : %s\n", ptr);
                }
                else
                {
                    printf("Failed to parse addr\n");
                }
            }
            break;
        }
    case E_FIBO_DATA_NET_DOWN_EVENT:
        printf("DATA DOWN profile:%d,call_id:%d,call_state:%d\n",data_call_ind->profile_idx,data_call_ind->call_id, data_call_ind->call_state);
        break;
    }

}


int main(int argc, char *argv[])
{
    int ret = 0;
    //init fibo sdk
    ret = fibo_sdk_init("127.0.0.1");
    if (ret != FIBO_NO_ERR)
    {
        FIBO_LOG(FIBO_LOG_ERROR,"ret = %d, fibo_sdk_init fail !!!!!!\n",ret);
        return ret;
    }

    //init data client
    ret = fibo_data_client_init(fibo_data_call_evt_cb);
    if (ret != FIBO_NO_ERR)
    {
        FIBO_LOG(FIBO_LOG_ERROR,"ret = %d, fibo_data_client_init fail !!!!!!\n",ret);
        goto end;
    }

    //set apn config
    fibo_data_call_config_info_t apn;
    apn.profile_idx = 1;
    apn.profile_valid = 1;

    apn.ip_family = 10;
    apn.ip_family_valid = 1;

    memset(apn.apn_name,0U,sizeof(apn.apn_name));
    memcpy(apn.apn_name,"test",strlen("test"));
    apn.apn_name_valid = 1;

    apn.auth_pref = 0;
    apn.auth_pref_valid = 1;

    apn.tech_pref = 3;
    apn.tech_pref_valid = 1;

    memset(apn.user_name,0U,sizeof(apn.user_name));
    apn.user_name_valid = 0;
    memset(apn.password,0U,sizeof(apn.password));
    apn.password_valid = 0;

    apn.reconnect = 1;

    ret = fibo_data_apn_set(&apn);
    if (ret != FIBO_NO_ERR)
    {
        FIBO_LOG(FIBO_LOG_ERROR,"ret = %d, fibo_data_apn_set fail !!!!!!\n",ret);
        goto client_deinit;
    }

    //data call
    fibo_data_call_result_info_t result_info;
    memset(&result_info, 0,sizeof(fibo_data_call_result_info_t));
    ret = fibo_data_call_start(1, &result_info);
    if (ret != FIBO_NO_ERR)
    {
        FIBO_LOG(FIBO_LOG_ERROR,"ret = %d, fibo_data_call_start fail !!!!!!\n",ret);
        goto stop_data_call;
    }

    ret = demo_call_BoAT();

stop_data_call:
    ret = fibo_data_call_stop(1);
    printf("fibo_data_call_stop ret=%d\n", ret);

client_deinit:
    fibo_data_client_deinit();
    printf("fibo_data_client_deinit\n");
end:
    fibo_sdk_deinit();
    return ret;
}
