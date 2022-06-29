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

#if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_HWBCS == 1)
#include "mbedtls/x509_crt.h"
#endif

BoatIotSdkContext g_boat_iot_sdk_context;


BOAT_RESULT BoatIotSdkInit(void)
{
    BUINT32 i;
#if ((PROTOCOL_USE_ETHEREUM == 1) || (PROTOCOL_USE_PLATON == 1) || (PROTOCOL_USE_PLATONE == 1) || (PROTOCOL_USE_FISCOBCOS == 1) || (PROTOCOL_USE_VENACHAIN == 1))    
    cJSON_Hooks hooks;
#endif    
    
#if RPC_USE_LIBCURL == 1
    CURLcode curl_result;
#endif

#if ((PROTOCOL_USE_ETHEREUM == 1) || (PROTOCOL_USE_PLATON == 1) || (PROTOCOL_USE_PLATONE == 1) || (PROTOCOL_USE_FISCOBCOS == 1) || (PROTOCOL_USE_VENACHAIN == 1))
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

static BOAT_RESULT BoatWalletCreatParaCheck(BoatProtocolType protocol_type,const void *wallet_config_ptr)
{
    switch (protocol_type)
    {

        #if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_HWBCS == 1)
        case BOAT_PROTOCOL_HLFABRIC:
        case BOAT_PROTOCOL_HWBCS:
            if (wallet_config_ptr != NULL)
            {
                BoatHlfabricWalletConfig* fabric_config_ptr = wallet_config_ptr;
                if (fabric_config_ptr->accountPriKey_config.prikey_genMode != BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION &&
                    fabric_config_ptr->accountPriKey_config.prikey_genMode != BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION)
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet prikey_genMode err.");
                    return BOAT_ERROR_WALLET_KEY_GENMODE_ERR;
                }
                if (fabric_config_ptr->accountPriKey_config.prikey_type != BOAT_WALLET_PRIKEY_TYPE_SECP256K1 &&
                    fabric_config_ptr->accountPriKey_config.prikey_type != BOAT_WALLET_PRIKEY_TYPE_SECP256R1)
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet prikey_type err.");
                    return BOAT_ERROR_WALLET_KEY_TYPE_ERR;
                }
                if (fabric_config_ptr->accountPriKey_config.prikey_format != BOAT_WALLET_PRIKEY_FORMAT_PKCS &&
                    fabric_config_ptr->accountPriKey_config.prikey_format != BOAT_WALLET_PRIKEY_FORMAT_NATIVE &&
                    fabric_config_ptr->accountPriKey_config.prikey_format != BOAT_WALLET_PRIKEY_FORMAT_MNEMONIC)
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet prikey_format err.");
                    return BOAT_ERROR_WALLET_KEY_FORMAT_ERR;
                }
                mbedtls_x509_crt m_certificate;
	            mbedtls_x509_crt_init(&m_certificate);
	            int status = mbedtls_x509_crt_parse(&m_certificate,fabric_config_ptr->accountCertContent.content, fabric_config_ptr->accountCertContent.length);
                if (status != BOAT_SUCCESS)
                {
                    BoatLog(BOAT_LOG_NORMAL, "persistent wallet account cert err. %x ",-status);
                    return BOAT_ERROR;
                }
                for (int i = 0; i < fabric_config_ptr->nodesCfg.endorserLayoutNum; i++)
                {
                    /* code */
                    for (int j = 0; j < fabric_config_ptr->nodesCfg.layoutCfg[i].endorserGroupNum; j++)
                    {
                        if(DISCOVERY_PEER_QUERY == 0){
                            if (fabric_config_ptr->nodesCfg.layoutCfg[i].groupCfg[j].quantities == 0 ||
                            fabric_config_ptr->nodesCfg.layoutCfg[i].groupCfg[j].quantities > fabric_config_ptr->nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber)
                            {
                                BoatLog(BOAT_LOG_NORMAL, "quantities ERR ");
                                return BOAT_ERROR;
                            }
                        }

                        /* code */
                        for (int k = 0; k < fabric_config_ptr->nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber; k++)
                        {
                            /* code */
                            if (strlen(fabric_config_ptr->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl)==0)
                            {
                                BoatLog(BOAT_LOG_NORMAL, "url len is 0 ");
                                return BOAT_ERROR;
                            }
                            
                            if (UtilityStringLenCheck(fabric_config_ptr->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl) != BOAT_SUCCESS)
                            {
                                BoatLog(BOAT_LOG_NORMAL, "url len check ERR ");
                                return BOAT_ERROR;
                            }
                            for (int l = 0; l < strlen(fabric_config_ptr->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl); l++)
                            {
                                /* code */
                                if (fabric_config_ptr->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl[l] < 0x20)
                                {
                                    BoatLog(BOAT_LOG_NORMAL, "url code ERR ");
                                    return BOAT_ERROR;
                                }
                            }
                        }
                    }
                    
                }
                for (int i = 0; i < fabric_config_ptr->nodesCfg.orderCfg.endorserNumber; i++)
                {
                    /* code */
                    if (strlen(fabric_config_ptr->nodesCfg.orderCfg.endorser[i].nodeUrl) == 0)
                    {
                       BoatLog(BOAT_LOG_NORMAL, "order url len is  0 ");
                        return BOAT_ERROR;
                    }
                    if (UtilityStringLenCheck(fabric_config_ptr->nodesCfg.orderCfg.endorser[i].nodeUrl) != BOAT_SUCCESS)
                    {
                        BoatLog(BOAT_LOG_NORMAL, "order url len check ERR ");
                        return BOAT_ERROR;
                    }
                    BoatLog(BOAT_LOG_NORMAL, "order url : %s ",fabric_config_ptr->nodesCfg.orderCfg.endorser[i].nodeUrl);
                    for (int l = 0; l < strlen(fabric_config_ptr->nodesCfg.orderCfg.endorser[i].nodeUrl); l++)
                    {
                        /* code */
                        if (fabric_config_ptr->nodesCfg.orderCfg.endorser[i].nodeUrl[l] < 0x20)
                        {
                            BoatLog(BOAT_LOG_NORMAL, "order url ode ERR ");
                            return BOAT_ERROR;
                        }
                    }
                }
                
            }
            break;
        #endif
        default:
            break;
    }
    return BOAT_SUCCESS;
}


BSINT32 BoatWalletCreate(BoatProtocolType protocol_type, const BCHAR *wallet_name_str, 
						 const void *wallet_config_ptr, BUINT32 wallet_config_size)
{
    BSINT32 i;
    BUINT8 *boatwalletStore_ptr = NULL;
    void   *wallet_ptr          = NULL;
#if ((PROTOCOL_USE_ETHEREUM == 1) || (PROTOCOL_USE_PLATON == 1) || (PROTOCOL_USE_PLATONE == 1) || (PROTOCOL_USE_FISCOBCOS == 1) || (PROTOCOL_USE_VENACHAIN == 1))    
    BUINT8  pubkeyHashDummy[32];
    BUINT8  hashLenDummy;
#endif    
    BOAT_RESULT result = BOAT_SUCCESS;

    /* Check wallet configuration */ 
    if ((wallet_name_str == NULL) && (wallet_config_ptr == NULL))
    {
        BoatLog(BOAT_LOG_NORMAL, "Invalid wallet configuration.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    result = BoatWalletCreatParaCheck(protocol_type,wallet_config_ptr);
    if (result != BOAT_SUCCESS)
    {
        return result;
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
                    memcpy(&((BoatFiscobcosWallet*)wallet_ptr)->account_info.prikeyCtx,  boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));
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
            if (wallet_config_size < sizeof(BoatHlchainmakerWalletConfig))
            {
                BoatLog(BOAT_LOG_NORMAL, "wallet_config_size out of memory");
                BoatFree(boatwalletStore_ptr);
                return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            }

            if (wallet_config_ptr != NULL)
            {
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                wallet_ptr = BoatHlchainmakerWalletInit((BoatHlchainmakerWalletConfig*)wallet_config_ptr);

                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatHlchainmakerWallet*)wallet_ptr)->user_cert_prikey_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
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
                        BoatLog(BOAT_LOG_NORMAL, "onetime wallet create");
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
                wallet_ptr = BoatHlchainmakerWalletInit(load_wallet_config_ptr);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy(&((BoatHlchainmakerWallet*)wallet_ptr)->user_cert_prikey_info.prikeyCtx, boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));                
                }
            }
            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = wallet_ptr;
            
            break;
        #endif

        #if PROTOCOL_USE_VENACHAIN == 1
        case BOAT_PROTOCOL_VENACHAIN:
            if (wallet_config_ptr != NULL)
            {
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                ((BoatEthWalletConfig*)wallet_config_ptr)->load_existed_wallet = false;
                wallet_ptr = BoatVenachainWalletInit((BoatVenachainWalletConfig*)wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatVenachainWallet*)wallet_ptr)->account_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
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
                BoatVenachainWalletConfig *load_wallet_config_ptr = (BoatVenachainWalletConfig*)boatwalletStore_ptr; 
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_ptr = NULL;
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_len = 0;
                load_wallet_config_ptr->load_existed_wallet = true;
                wallet_ptr = BoatVenachainWalletInit(load_wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy(&((BoatVenachainWallet*)wallet_ptr)->account_info.prikeyCtx,  boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));
                    // compute address
                    BoatHash(BOAT_HASH_KECCAK256, ((BoatVenachainWallet*)wallet_ptr)->account_info.prikeyCtx.pubkey_content, 64, pubkeyHashDummy, &hashLenDummy, NULL);
                    memcpy(((BoatVenachainWallet*)wallet_ptr)->account_info.address, &pubkeyHashDummy[32 - BOAT_VENACHAIN_ADDRESS_SIZE], BOAT_VENACHAIN_ADDRESS_SIZE);     
                }
            }

            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = wallet_ptr;
            break;
        #endif
             #if PROTOCOL_USE_QUORUM == 1
        case BOAT_PROTOCOL_QUORUM:
            if (wallet_config_ptr != NULL)
            {
                memcpy(boatwalletStore_ptr, wallet_config_ptr, wallet_config_size);
                ((BoatQuorumWalletConfig*)wallet_config_ptr)->load_existed_wallet = false;
                wallet_ptr = BoatQuorumWalletInit((BoatQuorumWalletConfig*)wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    memcpy(boatwalletStore_ptr + wallet_config_size, &((BoatQuorumWallet*)wallet_ptr)->account_info.prikeyCtx, sizeof(BoatWalletPriKeyCtx));
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
                BoatQuorumWalletConfig *load_wallet_config_ptr = (BoatQuorumWalletConfig*)boatwalletStore_ptr; 
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_ptr = NULL;
                load_wallet_config_ptr->prikeyCtx_config.prikey_content.field_len = 0;
                load_wallet_config_ptr->load_existed_wallet = true;
                wallet_ptr = BoatQuorumWalletInit(load_wallet_config_ptr, wallet_config_size);
                if (wallet_ptr != NULL)
                {
                    // re-assign private key context
                    memcpy( &((BoatQuorumWallet*)wallet_ptr)->account_info.prikeyCtx,  boatwalletStore_ptr + wallet_config_size, sizeof(BoatWalletPriKeyCtx));
                    // compute address
                    BoatHash(BOAT_HASH_KECCAK256, ((BoatQuorumWallet*)wallet_ptr)->account_info.prikeyCtx.pubkey_content, 64, pubkeyHashDummy, &hashLenDummy, NULL);
                    memcpy(((BoatQuorumWallet*)wallet_ptr)->account_info.address, &pubkeyHashDummy[32 - BOAT_QUORUM_ADDRESS_SIZE], BOAT_QUORUM_ADDRESS_SIZE);     
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

    g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_TRUE;
    
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

            #if PROTOCOL_USE_VENACHAIN == 1
            case BOAT_PROTOCOL_VENACHAIN:
                BoatVenachainWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
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


BOAT_RESULT BoatWalletDelete(BCHAR *wallet_name_str)
{
    // Delete persistent wallet
    return BoatPersistDelete(wallet_name_str);
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


