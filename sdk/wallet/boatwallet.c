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

#include "randgenerator.h"
#include "bignum.h"
#include "cJSON.h"

#include "persiststore.h"



BoatIotSdkContext g_boat_iot_sdk_context;



/******************************************************************************
@brief Initialize Boat IoT SDK

Function: BoatIotSdkInit()

    This function initialize global context of Boat IoT SDK.

    BoatIotSdkInit() MUST be called before any use of BoAT IoT SDK per process.
    BoatIotSdkDeInit() MUST be called after use of BoAT IoT SDK.
    

@see BoatIotSdkDeInit()

@return
    This function returns BOAT_SUCCESS if initialization is successful.\n
    Otherwise it returns BOAT_ERROR.
    

@param This function doesn't take any argument.
*******************************************************************************/
BOAT_RESULT BoatIotSdkInit(void)
{
    BUINT32 i;
    cJSON_Hooks hooks;
    
#if RPC_USE_LIBCURL == 1
    CURLcode curl_result;
#endif

    hooks.malloc_fn = BoatMalloc;
    hooks.free_fn = BoatFree;
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


/******************************************************************************
@brief De-initialize BoAT IoT SDK

Function: BoatIotSdkDeInit()

    This function de-initialize context of BoAT IoT SDK.

    BoatIotSdkInit() MUST be called before any use of BoAT IoT SDK per process.
    BoatIotSdkDeInit() MUST be called after use of BoAT IoT SDK.
    

@see BoatIotSdkInit()

@return This function doesn't return any thing.


*******************************************************************************/
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

    return;
}


/******************************************************************************
@brief Create BoAT wallet

Function: BoatWalletCreate()

    This function creates a wallet or loads an existed persistent wallet.

    A wallet is a configuration for blockchain parameters as well as context for
    blockchain operations. There are 2 types of wallet. One is a one-time wallet,
    which only exists in RAM and disappears once the device is powered down. The
    other is a persistent wallet, which is saved in non-volatile memory for later
    loading.
    
    If the given <wallet_name_str> is NULL, a one-time wallet is created. Otherwise
    a persistent wallet with the given name will be created or loaded. If no wallet
    with the given name exists, a new persistent wallet will be created with the
    name and immediately loaded into RAM. If a persistent wallet with the given
    name exists, it will be loaded into RAM.

    To create a one-time wallet, <wallet_name_str> MUST be NULL and a non-NULL
    <wallet_config_ptr> MUST be specified.

    To create a persistent wallet, <wallet_name_str>, <wallet_config_ptr> and
    <wallet_config_size> MUST be specified. If a persitent wallet with the given
    <wallet_name_str> exists, the specified configuration will overwrite the
    existed one, both in RAM and in non-volatile memroy.

    To load an existed persistent wallet into RAM, specify its name in <wallet_name_str>,
    specify <wallet_config_ptr> = NULL and <wallet_config_size> = the configuration
    size of the specified <protocol_type>.
    
    To unload a wallet from RAM, which was loaded by BoatWalletCreate(), call
    BoatWalletUnload(). For persisent wallet, this does NOT delete it from
    non-volatile memory.
    
    To delete a persistent wallet from non-volatile memroy, call BoatWalletDelete().
    This does NOT unload it from RAM. If a loaded persistent wallet is deleted
    without unloading it, it becomes a one-time wallet.


@see BoatWalletUnload() BoatWalletDelete()

@return
    This function returns the non-negative index of the loaded wallet.\n
    It returns -1 if wallet creation fails.

@param[in] protocol_type
    The blockchain protocol. See boattypes.h for supported protocol.

@param[in] wallet_name_str
    A string of wallet name.\n
    If the given <wallet_name_str> is NULL, a one-time wallet is created.\n
    Otherwise a persistent wallet with the given name will be created or loaded.

@param[in] wallet_config_ptr
    Configuration (e.g. crypto key) for the wallet.\n
    The exact configuration definition is determinted by the specified <protocol_type>.

@param[in] wallet_config_size
    Size (in byte) of configuration specified by <wallet_config_ptr>.

*******************************************************************************/
BSINT32 BoatWalletCreate(BoatProtocolType protocol_type, const BCHAR *wallet_name_str, const void * wallet_config_ptr, BUINT32 wallet_config_size)
{
    BSINT32 i;
    BUINT8 loaded_wallet_config_array[wallet_config_size];


    // Check wallet configuration
    if( wallet_name_str == NULL && wallet_config_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Invalid wallet configuration.");
        return -1;
    }

    // For Multi-Thread Support: ObtainMutex Here
    for( i = 0; i < BOAT_MAX_WALLET_NUM; i++ )
    {
        if( g_boat_iot_sdk_context.wallet_list[i].is_used == BOAT_FALSE )
        {
            g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_TRUE;
            break;
        }
    }
    // For Multi-Thread Support: ReleaseMutex Here

    if( i >= BOAT_MAX_WALLET_NUM )
    {
        BoatLog(BOAT_LOG_NORMAL, "Too many wallets was loaded.");
        return -1;
    }

    if( wallet_name_str != NULL )
    {
        if( wallet_config_ptr != NULL )
        {
            // Create persistent wallet / Overwrite existed configuration
            if( BOAT_SUCCESS != BoatPersistStore(wallet_name_str, wallet_config_ptr, wallet_config_size) )
            {
                return -1;
            }
            
            memcpy(loaded_wallet_config_array, wallet_config_ptr, wallet_config_size);
        }
        else
        {
            // Load persistent wallet;
            if( BOAT_SUCCESS != BoatPersistRead(wallet_name_str, loaded_wallet_config_array, wallet_config_size) )
            {
                return -1;
            }
        }
    }
    else
    {
        memcpy(loaded_wallet_config_array, wallet_config_ptr, wallet_config_size);
    }

    // Check protocol type
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
            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr  = BoatHLFabricWalletInit((BoatHLFabricWalletConfig*)loaded_wallet_config_array, wallet_config_size);
        break;
    #endif

    #if PROTOCOL_USE_PLATONE == 1
        case BOAT_PROTOCOL_PLATONE:
            g_boat_iot_sdk_context.wallet_list[i].wallet_ptr  = BoatPlatoneWalletInit((BoatPlatoneWalletConfig*)loaded_wallet_config_array, wallet_config_size);
        break;
    #endif
    
        default:
        g_boat_iot_sdk_context.wallet_list[i].wallet_ptr = NULL;
        
    }

    if( g_boat_iot_sdk_context.wallet_list[i].wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to create wallet: protocol type: %d.", (BSINT32)protocol_type);
        g_boat_iot_sdk_context.wallet_list[i].is_used = BOAT_FALSE;

        return -1;
    }

    

    
    return i;

}


/******************************************************************************
@brief Unload a wallet

Function: BoatWalletUnload()

    This function unloads a wallet from RAM if no more operations on this wallet
    is required. If the wallet is of persistent type, this function will NOT
    delete it from non-volatile memory. To delete a persistent wallet from
    non-volatile memory, call BoatWalletDelete().


@see BoatWalletCreate() BoatWalletDelete()

@return This function doesn't return any thing.

@param[in] wallet_index
    The wallet index to unload.

*******************************************************************************/
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
                BoatHLFabricWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
            break;
        #endif

        #if PROTOCOL_USE_PLATONE == 1
            case BOAT_PROTOCOL_PLATONE:
                BoatPlatoneWalletDeInit(g_boat_iot_sdk_context.wallet_list[wallet_index].wallet_ptr);
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
    

    return;
}


/******************************************************************************
@brief Delete a persistent wallet

Function: BoatWalletDelete()

    This function deletes a persistent wallet from non-volatile memory. This
    does NOT unload it from RAM. If a loaded persistent wallet is deleted
    without unloading it, it becomes a one-time wallet.


@see BoatWalletCreate() BoatWalletUnload()

@return This function doesn't return any thing.

@param[in] wallet_name_str
    The wallet name to delete.

*******************************************************************************/
void BoatWalletDelete(BCHAR * wallet_name_str)
{
    // Delete persistent wallet
    BoatPersistDelete(wallet_name_str);
    return;
}


/******************************************************************************
@brief Get the BoAT wallet context by index.

Function: BoatGetWalletByIndex()

    This function gets the BoAT wallet context by index.


@return This function returns a pointer to the wallet context.

@param[in] wallet_index
    The wallet index.

*******************************************************************************/
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


