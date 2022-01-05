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
#include "boatconfig.h"
#include "boatinternal.h"
#include "boatwallet.h"

#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "cJSON.h"

#include "persiststore.h"

#if RPC_USE_LIBCURL == 1
#include "curl/curl.h"
#endif

BoatIotSdkContext g_boat_iot_sdk_context;


BOAT_RESULT BoatIotSdkInit(void)
{
    BUINT32 i;
#if ((PROTOCOL_USE_ETHEREUM == 1) || (PROTOCOL_USE_PLATON == 1) || (PROTOCOL_USE_PLATONE == 1) || (PROTOCOL_USE_FISCOBCOS == 1))    
    cJSON_Hooks hooks;
#endif    
    
#if RPC_USE_LIBCURL == 1
    CURLcode curl_result;
#endif

#if ((PROTOCOL_USE_ETHEREUM == 1) || (PROTOCOL_USE_PLATON == 1) || (PROTOCOL_USE_PLATONE == 1) || (PROTOCOL_USE_FISCOBCOS == 1))
    hooks.malloc_fn = BoatMalloc;
    hooks.free_fn   = BoatFree;
    cJSON_InitHooks(&hooks);
#endif

	// For Multi-Thread Support: CreateMutex Here

    for (i = 0; i < BOAT_MAX_WALLET_NUM; i++)
    {
        g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_FALSE;
        g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = NULL;
    }
    
#if RPC_USE_LIBCURL == 1
    curl_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    
    if (curl_result != CURLE_OK)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to initialize curl.");
        return BOAT_ERROR_CURL_INIT_FAIL;
    }
#endif

    return BOAT_SUCCESS;
}


void BoatIotSdkDeInit(void)
{
    BUINT32 i;
    
    for (i = 0; i < BOAT_MAX_WALLET_NUM; i++)
    {
        BoatWalletUnload(i);
    }

#if RPC_USE_LIBCURL == 1
    curl_global_cleanup();
#endif

// For Multi-Thread Support: DeleteMutex Here
}


BSINT32 BoatWalletCreate(BoatProtocolType protocol_type, const BCHAR *wallet_name_str, 
						 const void *wallet_config_ptr, BUINT32 wallet_config_size)
{
    BSINT32 i;
    BUINT8 *boatwalletStore_ptr = NULL;
    void   *wallet_ptr          = NULL;
#if ((PROTOCOL_USE_ETHEREUM == 1) || (PROTOCOL_USE_PLATON == 1) || (PROTOCOL_USE_PLATONE == 1) || (PROTOCOL_USE_FISCOBCOS == 1))    
    BUINT8  pubkeyHashDummy[32];
    BUINT8  hashLenDummy;
#endif    

    /* Check wallet configuration */ 
    if ((wallet_name_str == NULL) && (wallet_config_ptr == NULL))
    {
        BoatLog(BOAT_LOG_NORMAL, "Invalid wallet configuration.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    
    boatwalletStore_ptr = BoatMalloc(wallet_config_size + sizeof(BoatWalletPriKeyCtx));
    if (NULL == boatwalletStore_ptr)
    {
        BoatLog(BOAT_LOG_NORMAL, "Failed to allocate memory.");
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memset(boatwalletStore_ptr, 0, wallet_config_size + sizeof(BoatWalletPriKeyCtx));
    
    /* For Multi-Thread Support: ObtainMutex Here */
    for (i = 0; i < BOAT_MAX_WALLET_NUM; i++)
    {
        if (g_boat_iot_sdk_context.wallet_list[i].is_used == BOAT_FALSE)
        {
            g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_TRUE;
            break;
        }
    }
    /* For Multi-Thread Support: ReleaseMutex Here */
    if (i >= BOAT_MAX_WALLET_NUM)
    {
        BoatLog(BOAT_LOG_NORMAL, "Too many wallets was loaded.");
        BoatFree(boatwalletStore_ptr);
        return BOAT_ERROR_WALLET_NUM_EXCEED;
    }

    /* Check protocol type */
    g_boat_iot_sdk_context.wallet_list[i].protocol_type = protocol_type;

    switch (protocol_type)
    {
        #if PROTOCOL_USE_ETHEREUM == 1
        case BOAT_PROTOCOL_ETHEREUM:
            if (wallet_config_ptr != NULL)
            {
                
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                ((BoatEthWalletConfig*)wallet_config_ptr)->load_existed_wallet = false;
                wallet_ptr = BoatEthWalletInit((BoatEthWalletConfig*)wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatEthWallet*)wallet_ptr)->account_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
                    if (wallet_name_str != NULL)
                    {
                        /* create persistent wallet / Overwrite existed configuration */
                        if (BOAT_SUCCESS != BoatPersistStore(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                        {
                            BoatLog(BOAT_LOG_NORMAL, "persistent wallet create failed.");
                            BoatFree(boatwalletStore_ptr);
                            return BOAT_ERROR_PERSISTER_STORE_FAIL;
                        }
                    }
                    else
                    {
                        /* create one-time wallet */
                        // nothing to do
                    } 
                }    
            }
            else
            {
                /* Load persistent wallet */
                if (BOAT_SUCCESS != BoatPersistRead(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet load failed.");
                    BoatFree(boatwalletStore_ptr);
                    return BOAT_ERROR_PERSISTER_READ_FAIL;
                }
                //
                BoatEthWalletConfig *load_wallet_config_ptr = (BoatEthWalletConfig*)boatwalletStore_ptr; 
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_ptr = NULL;
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_len = 0;
                load_wallet_config_ptr->load_existed_wallet = true;
                wallet_ptr = BoatEthWalletInit(load_wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy(&((BoatEthWallet*)wallet_ptr)->account_info.prikeyCtx, boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));
                    // compute address
                    BoatHash(BOAT_HASH_KECCAK256, ((BoatEthWallet*)wallet_ptr)->account_info.prikeyCtx.pubkey_content, 64, pubkeyHashDummy, &hashLenDummy, NULL);
                    memcpy(((BoatEthWallet*)wallet_ptr)->account_info.address, &pubkeyHashDummy[32 - BOAT_ETH_ADDRESS_SIZE], BOAT_ETH_ADDRESS_SIZE);     
                }
            }

            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = wallet_ptr;
           
            break;
        #endif

        #if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_HWBCS == 1)
        case BOAT_PROTOCOL_HLFABRIC:
        case BOAT_PROTOCOL_HWBCS:
            if (wallet_config_ptr != NULL)
            {
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                ((BoatHlfabricWalletConfig*)wallet_config_ptr)->accountPriKey_config.load_existed_wallet = false;
                wallet_ptr = BoatHlfabricWalletInit((BoatHlfabricWalletConfig*)wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatHlfabricWallet*)wallet_ptr)->account_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
                    if (wallet_name_str != NULL)
                    {
                        /* create persistent wallet / Overwrite existed configuration */
                        if (BOAT_SUCCESS != BoatPersistStore(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                        {
                            BoatLog(BOAT_LOG_NORMAL, "persistent wallet create failed.");
                            BoatFree(boatwalletStore_ptr);
                            return BOAT_ERROR_PERSISTER_STORE_FAIL;
                        }
                    }
                    else
                    {
                        /* create one-time wallet */
                        // nothing to do
                    } 
                }   
            }
            else
            {
                /* Load persistent wallet */
                if (BOAT_SUCCESS != BoatPersistRead(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet load failed.");
                    BoatFree(boatwalletStore_ptr);
                    return BOAT_ERROR_PERSISTER_READ_FAIL;
                }
                // 
                //BoatLog_hexasciidump(BOAT_LOG_CRITICAL, "ERERER", boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx));
                BoatHlfabricWalletConfig *load_wallet_config_ptr = (BoatHlfabricWalletConfig*)boatwalletStore_ptr; 
                load_wallet_config_ptr->accountPriKey_config.prikey_content.field_ptr = NULL;
                load_wallet_config_ptr->accountPriKey_config.prikey_content.field_len = 0;
                load_wallet_config_ptr->accountPriKey_config.load_existed_wallet = true;
                wallet_ptr = BoatHlfabricWalletInit(load_wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy(&((BoatHlfabricWallet*)wallet_ptr)->account_info.prikeyCtx, boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));                
                }
            }

            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = wallet_ptr;
            break;
        #endif
        
        #if PROTOCOL_USE_PLATON == 1
        case BOAT_PROTOCOL_PLATON:
            if (wallet_config_ptr != NULL)
            {
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                ((BoatEthWalletConfig*)wallet_config_ptr)->load_existed_wallet = false;
                wallet_ptr = BoatPlatONWalletInit((BoatPlatONWalletConfig*)wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatPlatONWallet*)wallet_ptr)->account_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
                    if (wallet_name_str != NULL)
                    {
                        /* create persistent wallet / Overwrite existed configuration */
                        if (BOAT_SUCCESS != BoatPersistStore(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                        {
                            BoatLog(BOAT_LOG_NORMAL, "persistent wallet create failed.");
                            BoatFree(boatwalletStore_ptr);
                            return BOAT_ERROR_PERSISTER_STORE_FAIL;
                        }
                    }
                    else
                    {
                        /* create one-time wallet */
                        // nothing to do
                    }
                }   
            }
            else
            {
                /* Load persistent wallet */
                if (BOAT_SUCCESS != BoatPersistRead(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet load failed.");
                    BoatFree(boatwalletStore_ptr);
                    return BOAT_ERROR_PERSISTER_READ_FAIL;
                }
                // 
                BoatPlatONWalletConfig *load_wallet_config_ptr = (BoatPlatONWalletConfig*)boatwalletStore_ptr; 
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_ptr = NULL;
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_len = 0;
                load_wallet_config_ptr->load_existed_wallet = true;
                wallet_ptr = BoatPlatONWalletInit(load_wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy(&((BoatPlatONWallet*)wallet_ptr)->account_info.prikeyCtx,  boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));
                    // compute address
                    BoatHash(BOAT_HASH_KECCAK256, ((BoatPlatONWallet*)wallet_ptr)->account_info.prikeyCtx.pubkey_content, 64, pubkeyHashDummy, &hashLenDummy, NULL);
                    memcpy(((BoatPlatONWallet*)wallet_ptr)->account_info.address, &pubkeyHashDummy[32 - BOAT_PLATON_ADDRESS_SIZE], BOAT_PLATON_ADDRESS_SIZE);     
                }
            }

            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = wallet_ptr;
           
            break;
        #endif

        #if PROTOCOL_USE_PLATONE == 1
        case BOAT_PROTOCOL_PLATONE:
            if (wallet_config_ptr != NULL)
            {
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                ((BoatEthWalletConfig*)wallet_config_ptr)->load_existed_wallet = false;
                wallet_ptr = BoatPlatoneWalletInit((BoatPlatoneWalletConfig*)wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatPlatoneWallet*)wallet_ptr)->account_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
                    if (wallet_name_str != NULL)
                    {
                        /* create persistent wallet / Overwrite existed configuration */
                        if (BOAT_SUCCESS != BoatPersistStore(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                        {
                            BoatLog(BOAT_LOG_NORMAL, "persistent wallet create failed.");
                            BoatFree(boatwalletStore_ptr);
                            return BOAT_ERROR_PERSISTER_STORE_FAIL;
                        }
                    }
                    else
                    {
                        /* create one-time wallet */
                        // nothing to do
                    }
                }   
            }
            else
            {
                /* Load persistent wallet */
                if (BOAT_SUCCESS != BoatPersistRead(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet load failed.");
                    BoatFree(boatwalletStore_ptr);
                    return BOAT_ERROR_PERSISTER_READ_FAIL;
                }
                // 
                BoatPlatoneWalletConfig *load_wallet_config_ptr = (BoatPlatoneWalletConfig*)boatwalletStore_ptr; 
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_ptr = NULL;
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_len = 0;
                load_wallet_config_ptr->load_existed_wallet = true;
                wallet_ptr = BoatPlatoneWalletInit(load_wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy(&((BoatPlatoneWallet*)wallet_ptr)->account_info.prikeyCtx,  boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));
                    // compute address
                    BoatHash(BOAT_HASH_KECCAK256, ((BoatPlatoneWallet*)wallet_ptr)->account_info.prikeyCtx.pubkey_content, 64, pubkeyHashDummy, &hashLenDummy, NULL);
                    memcpy(((BoatPlatoneWallet*)wallet_ptr)->account_info.address, &pubkeyHashDummy[32 - BOAT_PLATONE_ADDRESS_SIZE], BOAT_PLATONE_ADDRESS_SIZE);     
                }
            }

            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = wallet_ptr;
            break;
        #endif
		
	    #if PROTOCOL_USE_FISCOBCOS == 1
		case BOAT_PROTOCOL_FISCOBCOS:
            if (wallet_config_ptr != NULL)
            {
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                ((BoatEthWalletConfig*)wallet_config_ptr)->load_existed_wallet = false;
                wallet_ptr = BoatFiscobcosWalletInit((BoatFiscobcosWalletConfig*)wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatFiscobcosWallet*)wallet_ptr)->account_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
                    if (wallet_name_str != NULL)
                    {
                        /* create persistent wallet / Overwrite existed configuration */
                        if (BOAT_SUCCESS != BoatPersistStore(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                        {
                            BoatLog(BOAT_LOG_NORMAL, "persistent wallet create failed.");
                            BoatFree(boatwalletStore_ptr);
                            return BOAT_ERROR_PERSISTER_STORE_FAIL;
                        }
                    }
                    else
                    {
                        /* create one-time wallet */
                        // nothing to do
                    } 
                }   
            }
            else
            {
                /* Load persistent wallet */
                if (BOAT_SUCCESS != BoatPersistRead(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet load failed.");
                    BoatFree(boatwalletStore_ptr);
                    return BOAT_ERROR_PERSISTER_READ_FAIL;
                }
                // 
                BoatFiscobcosWalletConfig *load_wallet_config_ptr = (BoatFiscobcosWalletConfig*)boatwalletStore_ptr; 
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_ptr = NULL;
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_len = 0;
                load_wallet_config_ptr->load_existed_wallet = true;
                wallet_ptr = BoatFiscobcosWalletInit(load_wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy( &((BoatFiscobcosWallet*)wallet_ptr)->account_info.prikeyCtx,  boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));
                    // compute address
                    BoatHash(BOAT_HASH_KECCAK256, ((BoatFiscobcosWallet*)wallet_ptr)->account_info.prikeyCtx.pubkey_content, 64, pubkeyHashDummy, &hashLenDummy, NULL);
                    memcpy(((BoatFiscobcosWallet*)wallet_ptr)->account_info.address, &pubkeyHashDummy[32 - BOAT_FISCOBCOS_ADDRESS_SIZE], BOAT_FISCOBCOS_ADDRESS_SIZE);     
                }
            }

            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = wallet_ptr;
			break;
	    #endif

         #if PROTOCOL_USE_CHAINMAKER == 1
        case BOAT_PROTOCOL_CHAINMAKER:
            if (wallet_config_ptr != NULL)
            {
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                wallet_ptr = BoatHlchainmakerWalletInit((BoatHlchainmakerWalletConfig*)wallet_config_ptr, wallet_config_size);

                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatHlchainmakerWallet*)wallet_ptr)->user_cert_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
                    if (wallet_name_str != NULL)
                    {
                        /* create persistent wallet / Overwrite existed configuration */
                        if (BOAT_SUCCESS != BoatPersistStore(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                        {
                            BoatLog(BOAT_LOG_NORMAL, "persistent wallet create failed.");
                            BoatFree(boatwalletStore_ptr);
                            return BOAT_ERROR_PERSISTER_STORE_FAIL;
                        }
                    }
                    else
                    {
                        /* create one-time wallet */
                        // nothing to do
                    } 
                }   
            }
            else
            {
                /* Load persistent wallet */
                if (BOAT_SUCCESS != BoatPersistRead(wallet_name_str, boatwalletStore_ptr, wallet_config_size + sizeof(BoatWalletPriKeyCtx)))
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet load failed.");
                    BoatFree(boatwalletStore_ptr);
                    return BOAT_ERROR_PERSISTER_READ_FAIL;
                }
                BoatHlchainmakerWalletConfig *load_wallet_config_ptr = (BoatHlchainmakerWalletConfig*)boatwalletStore_ptr; 
                load_wallet_config_ptr->user_prikey_cfg.prikey_content.field_ptr = NULL;
                load_wallet_config_ptr->user_prikey_cfg.prikey_content.field_len = 0;
                wallet_ptr = BoatHlchainmakerWalletInit(load_wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy(&((BoatHlchainmakerWallet*)wallet_ptr)->user_cert_info.prikeyCtx, boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));                
                }
            }
            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = wallet_ptr;
            
            break;
        #endif
    
        default:
            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = NULL;
            break;
        
    }

    if (g_boat_iot_sdk_context.wallet_list[i].wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet: protocol type: %d.", (BSINT32)protocol_type);
        g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_FALSE;
        BoatFree(boatwalletStore_ptr);
        return BOAT_ERROR;
    }

    BoatFree(boatwalletStore_ptr);
    
    return i;
}


void BoatWalletUnload(BSINT32 wallet_index)
{
    BoatProtocolType protocol;

    if (wallet_index >= 0 && wallet_index < BOAT_MAX_WALLET_NUM
        && g_boat_iot_sdk_context.wallet_list[wallet_index].is_used != BOAT_FALSE
        && g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr != NULL)
    {
        protocol = g_boat_iot_sdk_context.wallet_list[wallet_index].protocol_type;

        switch (protocol)
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
                BoatPlatONWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
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

            #if PROTOCOL_USE_CHAINMAKER == 1
            case BOAT_PROTOCOL_CHAINMAKER:
                BoatHlchainmakerWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
            break;
            #endif
			
            default:
                BoatLog(BOAT_LOG_VERBOSE, "Unknown blockchain protocol type: %u.", protocol);
                protocol = BOAT_PROTOCOL_UNKNOWN;
            break;
        }

        if (protocol != BOAT_PROTOCOL_UNKNOWN)
        {
            g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr = NULL;
            g_boat_iot_sdk_context.wallet_list[wallet_index].is_used = BOAT_FALSE;
        }
    }
}


void BoatWalletDelete(BCHAR *wallet_name_str)
{
    // Delete persistent wallet
    BoatPersistDelete(wallet_name_str);
}


void *BoatGetWalletByIndex(BSINT32 wallet_index)
{
    if (wallet_index >= 0 && wallet_index < BOAT_MAX_WALLET_NUM)
    {
        if (g_boat_iot_sdk_context.wallet_list[wallet_index].is_used != BOAT_FALSE &&
            g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr != NULL)
        {
            return (g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
        }
    }

    return NULL;
}


