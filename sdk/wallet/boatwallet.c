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

/*!@brief Boatwallet SDK entry

@file
boatwallet.c is the SDK main entry.

@author aitos.io
*/

#include "boatinternal.h"
#include "boatwallet.h"

#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "cJSON.h"

#include "persiststore.h"

BoatIotSdkContext g_boat_iot_sdk_context;


BOAT_RESULT BoatIotSdkInit(void)
{
    BUINT32 i;
    cJSON_Hooks hooks;
    
#if RPC_USE_LIBCURL == 1
    CURLcode curl_result;
#endif

    hooks.malloc_fn = BoatMalloc;
    hooks.free_fn   = BoatFree;
    cJSON_InitHooks(&hooks);

	// For Multi-Thread Support: CreateMutex Here

    for( i = 0; i < BOAT_MAX_WALLET_NUM; i++ )
    {
        g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_FALSE;
        g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = NULL;
    }
    
#if RPC_USE_LIBCURL == 1
    curl_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    
    if( curl_result != CURLE_OK)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to initialize curl.");
        return BOAT_ERROR;
    }
#endif

    return BOAT_SUCCESS;
}


void BoatIotSdkDeInit(void)
{
    BUINT32 i;
    
    for( i = 0; i < BOAT_MAX_WALLET_NUM; i++ )
    {
        BoatWalletUnload(i);
    }

#if RPC_USE_LIBCURL == 1
    curl_global_cleanup();
#endif

// For Multi-Thread Support: DeleteMutex Here
}


BSINT32 BoatWalletCreate( BoatProtocolType protocol_type, const BCHAR *wallet_name_str, 
						  const void * wallet_config_ptr, BUINT32 wallet_config_size )
{
    BSINT32 i;
    BUINT8  loaded_wallet_config_array[wallet_config_size];
	BoatWalletPriKeyCtx          priKeyCtxTmp = {0};
    BoatWalletPriKeyCtx_config*  priKeyCtx_configTmp = NULL;

    /* Check wallet configuration */ 
    if( (wallet_name_str == NULL) && (wallet_config_ptr == NULL) )
    {
        BoatLog(BOAT_LOG_NORMAL, "Invalid wallet configuration.");
        return BOAT_ERROR;
    }
    /* Check the parameter of config is valid or not */
    //! @todo Check the parameter of config is valid or not
    
    /* For Multi-Thread Support: ObtainMutex Here */
    for( i = 0; i < BOAT_MAX_WALLET_NUM; i++ )
    {
        if( g_boat_iot_sdk_context.wallet_list[i].is_used == BOAT_FALSE )
        {
            g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_TRUE;
            break;
        }
    }
    /* For Multi-Thread Support: ReleaseMutex Here */
    if( i >= BOAT_MAX_WALLET_NUM )
    {
        BoatLog(BOAT_LOG_NORMAL, "Too many wallets was loaded.");
        return BOAT_ERROR;
    }


    if( wallet_config_ptr != NULL )
    {
        /* private key context update */
        /* -step-1:  generate prikeyCtxTmp */
        switch(protocol_type)
        {
            case BOAT_PROTOCOL_ETHEREUM:
            case BOAT_PROTOCOL_PLATONE:
            case BOAT_PROTOCOL_FISCOBCOS:
                priKeyCtx_configTmp = &((BoatEthWalletConfig*)wallet_config_ptr)->prikeyCtx_config;
                if( BOAT_SUCCESS != BoatPort_keyCreate( priKeyCtx_configTmp, &priKeyCtxTmp ) )
                {
                    BoatLog( BOAT_LOG_CRITICAL, "Failed to exec BoatPort_keyCreate." );
                    return BOAT_ERROR;
                }
            break;
            default:
            break;
        }
        
        printf("priKeyCtxTmp.pubkey_format        : %d\n", priKeyCtxTmp.pubkey_format);
        BoatLog_hexasciidump(BOAT_LOG_VERBOSE, "priKeyCtxTmp.pubkey_content", priKeyCtxTmp.pubkey_content, 64);
        BoatLog_hexasciidump(BOAT_LOG_VERBOSE, "priKeyCtxTmp.extra_data.value", \
                                                priKeyCtxTmp.extra_data.value, priKeyCtxTmp.extra_data.value_len);

        /* -step-2:  assign value of prikeyIdTmp to wallet_config_ptr */
        memcpy( &( priKeyCtx_configTmp->private_KeyCtx), &priKeyCtxTmp, sizeof(priKeyCtxTmp) );
        
        /* -step-3:  clear sensitive information in wallet_config_ptr */
        priKeyCtx_configTmp->prikey_genMode        = BOAT_WALLET_PRIKEY_GENMODE_UNKNOWN;
        priKeyCtx_configTmp->prikey_format         = BOAT_WALLET_PRIKEY_FORMAT_UNKNOWN;
        priKeyCtx_configTmp->prikey_type           = BOAT_WALLET_PRIKEY_TYPE_UNKNOWN;
        priKeyCtx_configTmp->prikey_content_length = 0;
        memset( priKeyCtx_configTmp->prikey_content, 0, sizeof(priKeyCtx_configTmp->prikey_content) );
        
        if( wallet_name_str != NULL )
        {
            /* -step-4: create persistent wallet / Overwrite existed configuration */
            if( BOAT_SUCCESS != BoatPersistStore(wallet_name_str, wallet_config_ptr, wallet_config_size) )
            {
				BoatLog(BOAT_LOG_NORMAL, "persistent wallet create failed.");
                return BOAT_ERROR;
            }
            memcpy(loaded_wallet_config_array, wallet_config_ptr, wallet_config_size);
        }
        else
        {
            /* create one-time wallet */
            memcpy(loaded_wallet_config_array, wallet_config_ptr, wallet_config_size);
        }    
    }
    else
    {
        /* Load persistent wallet */
        if( BOAT_SUCCESS != BoatPersistRead(wallet_name_str, loaded_wallet_config_array, wallet_config_size) )
        {
            BoatLog(BOAT_LOG_NORMAL, "persistent wallet load failed.");
            return BOAT_ERROR;
        }
    }

    /* Check protocol type */
    g_boat_iot_sdk_context.wallet_list[i].protocol_type = protocol_type;
    
    switch(protocol_type)
    {

    #if PROTOCOL_USE_ETHEREUM == 1
        case BOAT_PROTOCOL_ETHEREUM:

            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = BoatEthWalletInit((BoatEthWalletConfig*)loaded_wallet_config_array, wallet_config_size);
        break;
    #endif

    #if PROTOCOL_USE_HLFABRIC == 1
        case BOAT_PROTOCOL_HLFABRIC:
            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr  = BoatHlfabricWalletInit((BoatHlfabricWalletConfig*)loaded_wallet_config_array, wallet_config_size);
        break;
    #endif

    #if PROTOCOL_USE_PLATONE == 1
        case BOAT_PROTOCOL_PLATONE:
            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr  = BoatPlatoneWalletInit((BoatPlatoneWalletConfig*)loaded_wallet_config_array, wallet_config_size);
        break;
    #endif
		
	#if PROTOCOL_USE_FISCOBCOS == 1
		case BOAT_PROTOCOL_FISCOBCOS:
			g_boat_iot_sdk_context.wallet_list[i].wallet_ptr  = BoatFiscobcosWalletInit((BoatFiscobcosWalletConfig*)loaded_wallet_config_array, wallet_config_size);
			break;
	#endif
    
        default:
        g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = NULL;
        
    }

    if( g_boat_iot_sdk_context.wallet_list[i].wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet: protocol type: %d.", (BSINT32)protocol_type);
        g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_FALSE;

        return BOAT_ERROR;
    }

    return i;
}


void BoatWalletUnload(BSINT32 wallet_index)
{
    BoatProtocolType protocol;

    if( wallet_index >= 0 && wallet_index < BOAT_MAX_WALLET_NUM
        && g_boat_iot_sdk_context.wallet_list[wallet_index].is_used != BOAT_FALSE
        && g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr != NULL )
    {
        protocol = g_boat_iot_sdk_context.wallet_list[wallet_index].protocol_type;

        switch(protocol)
        {

        #if PROTOCOL_USE_ETHEREUM == 1
            case BOAT_PROTOCOL_ETHEREUM:
                BoatEthWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
            break;
        #endif

        #if PROTOCOL_USE_HLFABRIC == 1
            case BOAT_PROTOCOL_HLFABRIC:
                BoatHlfabricWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
            break;
        #endif

        #if PROTOCOL_USE_PLATON == 1
            case BOAT_PROTOCOL_PLATON:
                BoatEthWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
            break;
        #endif

        #if PROTOCOL_USE_PLATONE == 1
            case BOAT_PROTOCOL_PLATONE:
                BoatPlatoneWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
            break;
		#endif
			
		#if PROTOCOL_USE_FISCOBCOS == 1
		case BOAT_PROTOCOL_FISCOBCOS:
				BoatFiscobcosWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
            break;
        #endif
			
            default:
                BoatLog(BOAT_LOG_VERBOSE, "Unknown blockchain protocol type: %u.", protocol);
                protocol = BOAT_PROTOCOL_UNKNOWN;
        }

        if( protocol != BOAT_PROTOCOL_UNKNOWN )
        {
            g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr = NULL;
            g_boat_iot_sdk_context.wallet_list[wallet_index].is_used = BOAT_FALSE;
        }
    }
}


void BoatWalletDelete(BCHAR * wallet_name_str)
{
    // Delete persistent wallet
    BoatPersistDelete(wallet_name_str);
}


void * BoatGetWalletByIndex(BSINT32 wallet_index)
{

    if( wallet_index >= 0 && wallet_index < BOAT_MAX_WALLET_NUM )
    {
        if(    g_boat_iot_sdk_context.wallet_list[wallet_index].is_used != BOAT_FALSE
            && g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr != NULL )
        {
            return(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
        }
    }

    return NULL;
}


