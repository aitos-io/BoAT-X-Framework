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

/*!@brief Boatwallet SDK header file

@file
boatwallet.h is the SDK header file.
*/

#ifndef __BOATWALLET_H__
#define __BOATWALLET_H__

#include "boatiotsdk.h"

/*! @defgroup wallet boat wallet
 * @{
 */

//!@brief XXX
typedef enum
{
    BOAT_WALLET_PRIKEY_UNKNOWN = 0,     //!< Placeholder for unknown prikey
    BOAT_WALLET_PRIKEY_FORMAT_PKCS_PEM,        //!< xx
    BOAT_WALLET_PRIKEY_FORMAT_PKCS_DER,        //!< xx
    BOAT_WALLET_PRIKEY_FORMAT_PRIMORDIAL,      //!< xx
    BOAT_WALLET_PRIKEY_FORMAT_MNEMONIC,        //!< xx
    BOAT_WALLET_PRIKEY_FORMAT_GENERATION,      //!< xx
}BoatWalletPriKeyFormat;

typedef enum
{
    BOAT_WALLET_PRIKEY_TYPE_UNKNOWN = 0,  //!< Placeholder for unknown signature algorithm
	BOAT_WALLET_PRIKEY_TYPE_SECP256K1,    //!< secp256k1 algorithm
	BOAT_WALLET_PRIKEY_TYPE_SECP256R1,    //!< secp256r1 algorithm
}BoatWalletPriKeyType;

//!@brief XXX
typedef enum
{
	BOAT_WALLET_PUBKEY_UNKNOWN = 0,     //!< Placeholder for unknown prikey
	//! @todo
}BoatWalletPubKeyType;

//!@brief XXX
typedef struct TBoatWalletExtraData
{
    BUINT32 map_key;
    BUINT8  map_value[512]; //!< xxx
}BoatWalletExtraData;


//!@brief XXX
typedef struct TBoatWalletPriKeyId
{
    BUINT32                 prikey_index;       //!< xxx
	BoatWalletPriKeyFormat  prikey_format;
	BoatWalletPriKeyType    prikey_type;        //!< SDK according to this field to execute signature
	BoatWalletPubKeyType    pubkey_type;        //!< xxx
    BUINT8                  pubkey_content[64]; //!< xxx
    BoatWalletExtraData     extra_data;         //!< used for crypto implemented by software, to store the prikey info.
}BoatWalletPriKeyId;


//!@brief XXX
typedef struct TBoatWalletPriKeyId_config
{
	BoatWalletPriKeyFormat  prikey_format;
    BoatWalletPriKeyType    prikey_type;    //!< xxx
    BUINT8                  prikey_content[512]; //!< xxx
	BUINT32                 prikey_content_size;//!< xxx
	
	//! this field will be updated by internal
	BoatWalletPriKeyId  private_KeyId;  //!< xxx
}BoatWalletPriKeyId_config;


//!@brief XXX
typedef struct TBoatWalletList
{
    BBOOL is_used;
    BoatProtocolType protocol_type; //!< Blockchain protocol type
    BCHAR *wallet_name_str;         //!< Wallet name for persist wallet, NULL for one-time wallet
    void  *wallet_ptr;              //!< Wallet context of one of BoatWalletInfo type
}BoatWalletList;


#define BOAT_MAX_WALLET_NUM 5
//!@brief BoAT IoT SDK Context
typedef struct TBoatIotSdkContext
{
    // Protocol specifiec properties are defined in protocol specific WalletInfo structure
    BoatWalletList wallet_list[BOAT_MAX_WALLET_NUM];  //!< Wallet Info List
}BoatIotSdkContext;


#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Initialize Boat IoT SDK
 *
 * @details
 *   This function initialize global context of Boat IoT SDK.
 *   \n BoatIotSdkInit() MUST be called before any use of BoAT IoT SDK per process.
 *   BoatIotSdkDeInit() MUST be called after use of BoAT IoT SDK.
 *
 * @param This function doesn't take any argument.
 *
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns BOAT_ERROR.
 * @see BoatIotSdkDeInit()
 ******************************************************************************/
BOAT_RESULT BoatIotSdkInit(void);


/*!****************************************************************************
 * @brief De-initialize BoAT IoT SDK
 *
 * @details
 *   This function de-initialize context of BoAT IoT SDK.
 *   \n BoatIotSdkInit() MUST be called before any use of BoAT IoT SDK per process.
 *   BoatIotSdkDeInit() MUST be called after use of BoAT IoT SDK.
 *   
 * @return This function doesn't return any thing.
 *
 * @see BoatIotSdkInit()
 ******************************************************************************/
void BoatIotSdkDeInit(void);


/*!****************************************************************************
 * @brief Create BoAT wallet
 *
 * @details
 *   This function creates a wallet or loads an existed persistent wallet.
 *   \n A wallet is a configuration for blockchain parameters as well as context for
 *   blockchain operations. There are 2 types of wallet. One is a one-time wallet,
 *   which only exists in RAM and disappears once the device is powered down. The
 *   other is a persistent wallet, which is saved in non-volatile memory for later
 *   loading.
 *   \n If the given <wallet_name_str> is NULL, a one-time wallet is created. Otherwise
 *   a persistent wallet with the given name will be created or loaded. If no wallet
 *   with the given name exists, a new persistent wallet will be created with the
 *   name and immediately loaded into RAM. If a persistent wallet with the given
 *   name exists, it will be loaded into RAM.
 *   \n To create a one-time wallet, <wallet_name_str> MUST be NULL and a non-NULL
 *   <wallet_config_ptr> MUST be specified.
 *   \n To create a persistent wallet, <wallet_name_str>, <wallet_config_ptr> and
 *   <wallet_config_size> MUST be specified. If a persitent wallet with the given
 *   <wallet_name_str> exists, the specified configuration will overwrite the
 *   existed one, both in RAM and in non-volatile memroy.
 *   \n To load an existed persistent wallet into RAM, specify its name in <wallet_name_str>,
 *   specify <wallet_config_ptr> = NULL and <wallet_config_size> = the configuration
 *   size of the specified <protocol_type>.  
 *   \n To unload a wallet from RAM, which was loaded by BoatWalletCreate(), call
 *   BoatWalletUnload(). For persisent wallet, this does NOT delete it from
 *   non-volatile memory.   
 *   \n To delete a persistent wallet from non-volatile memroy, call BoatWalletDelete().
 *   This does NOT unload it from RAM. If a loaded persistent wallet is deleted
 *   without unloading it, it becomes a one-time wallet.
 *
 * @param[in] protocol_type
 *   The blockchain protocol. See boattypes.h for supported protocol.
 *
 * @param[in] wallet_name_str
 *   A string of wallet name.\n
 *   If the given <wallet_name_str> is NULL, a one-time wallet is created.\n
 *   Otherwise a persistent wallet with the given name will be created or loaded.
 *
 * @param[in] wallet_config_ptr
 *   Configuration (e.g. crypto key) for the wallet.\n
 *   The exact configuration definition is determinted by the specified <protocol_type>.
 *	
 * @param[in] wallet_config_size
 *   Size (in byte) of configuration specified by <wallet_config_ptr>.
 *
 * @return
 *   This function returns the non-negative index of the loaded wallet.\n
 *   It returns -1 if wallet creation fails.
 *	
 * @see BoatWalletUnload() BoatWalletDelete()
 ******************************************************************************/
BSINT32 BoatWalletCreate( BoatProtocolType protocol_type, const BCHAR *wallet_name_str, 
						  const void * wallet_config_ptr, BUINT32 wallet_config_size );


/*!****************************************************************************
 * @brief Unload a wallet
 *
 * @details
 *   This function unloads a wallet from RAM if no more operations on this wallet
 *   is required. If the wallet is of persistent type, this function will NOT
 *   delete it from non-volatile memory. To delete a persistent wallet from
 *   non-volatile memory, call BoatWalletDelete().
 *
 * @param[in] wallet_index
 *   The wallet index to unload.
 *
 * @return 
 *   This function doesn't return any thing.
 *
 * @see BoatWalletCreate() BoatWalletDelete()
 ******************************************************************************/
void BoatWalletUnload(BSINT32 wallet_index);


/*!****************************************************************************
 * @brief Delete a persistent wallet
 *
 * @details
 *   This function deletes a persistent wallet from non-volatile memory. This
 *   does NOT unload it from RAM. If a loaded persistent wallet is deleted
 *   without unloading it, it becomes a one-time wallet.
 *
 * @param[in] wallet_name_str
 *   The wallet name to delete.
 *
 * @return
 *   This function doesn't return any thing.
 *
 * @see BoatWalletCreate() BoatWalletUnload()
 ******************************************************************************/
void BoatWalletDelete(BCHAR * wallet_name_str);


/*!****************************************************************************
 * @brief Get the BoAT wallet context by index.
 *
 * @details
 *   This function gets the BoAT wallet context by index.
 * 
 * @param[in] wallet_index
 *   The wallet index.
 *
 * @return This function returns a pointer to the wallet context.
 ******************************************************************************/
void * BoatGetWalletByIndex(BSINT32 wallet_index);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
