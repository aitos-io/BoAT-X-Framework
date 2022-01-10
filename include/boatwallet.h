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

/*! @defgroup wallet boat wallet API
 * @{
 */

#include "boattypes.h"

//! @brief The generate mode of the used private key
typedef enum
{
    BOAT_WALLET_PRIKEY_GENMODE_UNKNOWN = 0,         //!< Placeholder for unknown prikey format
    BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION,  //!< The private key is injected externally
    BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION, //!< The private key is generated internally
}BoatWalletPriKeyGenMode;

//! @brief The format of the externally injected private key
//! This field will be actived when BOAT_WALLET_PRIKEY_FORMAT_EXTERN_INJECTION be selected.
typedef enum
{
    BOAT_WALLET_PRIKEY_FORMAT_UNKNOWN = 0,     //!< Placeholder for unknown prikey format
    BOAT_WALLET_PRIKEY_FORMAT_PKCS,            //!< Contain PEM and DER format
    BOAT_WALLET_PRIKEY_FORMAT_NATIVE,          //!< The 32 bytes format private key
    BOAT_WALLET_PRIKEY_FORMAT_MNEMONIC,        //!< Mnemonic words that meet BIP39 format
}BoatWalletPriKeyFormat;

//! Type of private key
//! @note For PKCS format private key, the key type is already included in it,
//!       but it's still suggest to fill this field.
typedef enum
{
    BOAT_WALLET_PRIKEY_TYPE_UNKNOWN = 0,  //!< Placeholder for unknown signature algorithm
	BOAT_WALLET_PRIKEY_TYPE_SECP256K1,    //!< secp256k1 algorithm
	BOAT_WALLET_PRIKEY_TYPE_SECP256R1,    //!< secp256r1 algorithm
}BoatWalletPriKeyType;

//!@brief format of public key
typedef enum
{
	BOAT_WALLET_PUBKEY_FORMAT_UNKNOWN = 0, //!< Placeholder for unknown prikey
    BOAT_WALLET_PUBKEY_FORMAT_NATIVE,      //!< The 64 bytes public key, it's a coordinate of an elliptic curve 
}BoatWalletPubKeyFormat;

//!@brief The extension field of prikey context
//!  This field is only used for secret key storage when secure storage environment is not available. 
//!  \n The practice of the security specification is still to store it in TE/SE.
typedef struct TBoatWalletExtraData
{
    BUINT32  value_len;  //!< Length of the stored private key
    BUINT8   value[256]; //!< Private key content when a secure storage environment is not available
}BoatWalletExtraData;


//!@brief Boat wallet private key context
typedef struct TBoatWalletPriKeyCtx
{
    BUINT32                 prikey_index;       //!< Index of private key
	BoatWalletPriKeyFormat  prikey_format;      //!< Format of private key
	BoatWalletPriKeyType    prikey_type;        //!< Type of private key, SDK according to this field to execute corresponding signature
	
	BoatWalletPubKeyFormat  pubkey_format;      //!< Format of public key
    BUINT8                  pubkey_content[64]; //!< Content of public key, current only native format be supported.
    
	BoatWalletExtraData     extra_data;         //!< Used for crypto implemented by software, to store the prikey info.
}BoatWalletPriKeyCtx;


//!@brief Boat wallet config context  
typedef struct TBoatWalletPriKeyCtx_config
{
	BoatWalletPriKeyGenMode  prikey_genMode;        //!< Generate mode of private key
	BoatWalletPriKeyFormat   prikey_format;         //!< Format of private key
    BoatWalletPriKeyType     prikey_type;           //!< Type of private key
    BoatFieldVariable        prikey_content;        //!< The externally injected private key contents.
                                                    //!< \n A pointer to externally injected private key content, the content of this 
													//!< field point to  will be COPYED to the corresponding field of the wallet, 
													//!< if user dynamically allocated space for this pointer, then the user should 
	                                                //!< free it after BoatWalletCreate invoked. 
													//!< @note For content of type string, such as PEM format data, the length includes 
                                                    //!< the terminating null byte.
    BBOOL                    load_existed_wallet;    //false : need creat key by boat ; true : not need creat key
}BoatWalletPriKeyCtx_config;


//!@brief Boat wallet list
typedef struct TBoatWalletList
{
    BBOOL is_used;
    BoatProtocolType protocol_type; //!< Blockchain protocol type
    BCHAR *wallet_name_str;         //!< Wallet name for persist wallet, NULL for one-time wallet
    void  *wallet_ptr;              //!< Wallet context of one of BoatWalletInfo type
}BoatWalletList;


#define BOAT_MAX_WALLET_NUM 6
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
 *   BoatIotSdkInit() MUST be called only once unless BoatIotSdkDeInit() is called.
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
 * @return 
 *   This function doesn't return any thing.
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
BSINT32 BoatWalletCreate(BoatProtocolType protocol_type, const BCHAR *wallet_name_str, 
						 const void *wallet_config_ptr, BUINT32 wallet_config_size);


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
void BoatWalletDelete(BCHAR *wallet_name_str);


/*!****************************************************************************
 * @brief Get the BoAT wallet context by index.
 *
 * @details
 *   This function gets the BoAT wallet context by index.
 * 
 * @param[in] wallet_index
 *   The wallet index.
 *
 * @return 
 *   This function returns a pointer to the wallet context. 
 *   \n Otherwise returns a \c NULL if the indexed wallet is not exist.
 ******************************************************************************/
void *BoatGetWalletByIndex(BSINT32 wallet_index);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
