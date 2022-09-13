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

#define BOAT_TLS_SUPPORT                1 //!< If need client support TLS, set it to 1.
#define BOAT_TLS_IDENTIFY_CLIENT        0 //!< If server need identify client, set it to 1.

#define BOAT_KEYPAIR_NUM_LEN        4   // AA55+NUM+CRC
#define BOAT_KEYPAIR_ALL_LEN_MAX    3  //82XXXX
#define BOAT_KEYPAIR_INDEX_LEN      2  //01+index
#define BOAT_KEYPAIR_NAME_LEN_MAX   65 //L+64 the max len of keypair name is 64
#define BOAT_KEYPAIR_FORMAT_LEN     5  // L + format   0401000000
#define BOAT_KEYPAIR_TYPE_LEN       5  // L + type     0401000000
#define BOAT_KEYPAIR_PUBKEY_LEN     65 // L + pubkey   
#define BOAT_KEYPAIR_PRIKEY_LEN     33 // L + prikey   


#define BOAT_MAX_WALLET_NUM 5

/*
 * @Description: 
 * store type
 * @Author: aitos
 * @Date: 2022-08-19 14:34:52
 * @LastEditors: zt222
 * @LastEditTime: 2022-08-30 11:06:40
 */


//! @brief The generate mode of the used private key
typedef enum
{
    BOAT_WALLET_PRIKEY_GENMODE_UNKNOWN = 0,         //!< Placeholder for unknown private key format
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
    BOAT_WALLET_PRIKEY_TYPE_UNKNOWN = 0,  //!< Placeholder for unknown ecliptic curve
	BOAT_WALLET_PRIKEY_TYPE_SECP256K1,    //!< secp256k1 ecliptic curve
	BOAT_WALLET_PRIKEY_TYPE_SECP256R1,    //!< secp256r1 ecliptic curve
}BoatWalletPriKeyType;



//!@brief The extension field of prikey context
//!  This field is only used for secret key storage when secure storage environment is not available. 
//!  \n The practice of the security specification is still to store it in TEE/SE.
typedef struct TBoatWalletExtraData
{
    BUINT32  value_len;  //!< Length of the stored private key
    BUINT8   value[512]; //!< Private key content when a secure storage environment is not available
}BoatWalletExtraData;

typedef struct TBoatWalletKeypair
{
   BoatWalletExtraData prikey;  // wallet prikey
   BoatWalletExtraData pubkey; // wallet pubkey
}BoatWalletKeypair;


//!@brief Boat wallet private key context
typedef struct TBoatWalletPriKeyCtx
{
    BUINT8                  wallet_index;       //!< Index of wallet
    BCHAR*                  wallet_name;        //!< name of wallet
	BoatWalletPriKeyFormat  prikey_format;      //!< Format of private key
	BoatWalletPriKeyType    prikey_type;        //!< Type of private key, SDK according to this field to execute corresponding signature
	
    BUINT8                  pubkey_content[64]; //!< Content of public key, current only native format be supported.
    
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
}BoatWalletPriKeyCtx_config;


//!@brief Boat wallet data context
typedef struct TBoatWalletDataCtx
{
    BoatWalletPriKeyCtx prikeyCtx;             //!< wallet key content , not contain prikey data
    BoatWalletExtraData extraData;             //!< prikey data , only used for storing wallet data, delete content immediately after storing;
                                               //!< this parameter could be used for onetime wallet, 
}BoatWalletDataCtx;



//!@brief BoAT IoT SDK Context
typedef struct TBoatIotWalletContext
{
    BUINT8 walletNum;
    // Protocol specific properties are defined in protocol specific WalletInfo structure
    BoatWalletDataCtx wallets[BOAT_MAX_WALLET_NUM];  //!< Wallet Info List
}BoatIotWalletContext;


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
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
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
 *   This function doesn't return anything.
 *
 * @see BoatIotSdkInit()
 ******************************************************************************/
void BoatIotSdkDeInit(void);

/**
 * @description: 
 * This function creat wallet
 * @param[in] {BoatWalletPriKeyCtx_config} walletConfig
 * @param[in] {BCHAR} *walletName
 *  If walletName is NULL , BoAT will slect one testing name.
 * @param[in] {BoatStoreType} storeType
 *  For onetime wallet or test , select store in ram .
 *  For persistent wallet ,slect store in flash.
 * @return 
 *   This function returns wallet index if creat wallet successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoatWalletCreate(BoatWalletPriKeyCtx_config *walletConfig,BCHAR *walletName,BoatStoreType storeType);


/**
 * @description: 
 *  This function get persistent wallet list
 * @param[out] {BoatIotWalletContext} *walletList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATWallet_GetWalletList(BoatIotWalletContext *walletList);



/**
 * @description: 
 *  This function deletes the wallet which walletIndex equals index.
 *  This function will delete the wallet data in Nvram.
 * @param {BUINT8} index
 *  the index want to delete
 * @return {*}
 *   This function returns BOAT_SUCCESS if delete successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATIotWalletDelete(BUINT8 index);

/**
 * @description: 
 *  This function use to free BoatIotWalletContext param.
 * @param[in] {BoatIotWalletContext} walletList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATWallet_FreeWalletContext(BoatIotWalletContext walletList);


/**
 * @description: 
 *  This function read wallet data from Nvram by index. Not read prikey content.
 * @param {BoatWalletPriKeyCtx} *PriKeyCtx
 * @param {BUINT8} index
 *  the wallet index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATWallet_GetWalletByIndex(BoatWalletPriKeyCtx *PriKeyCtx ,BUINT8 index);



/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
