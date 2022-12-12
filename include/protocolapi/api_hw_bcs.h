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

/*!@brief Header file for performing hauwei chain transaction

@file
api_hw_bcs.h is header file for hauwei chain transaction construction and performing.
*/

#ifndef __API_HW_BCS_H__
#define __API_HW_BCS_H__

#include "boatiotsdk.h"


#define    BOAT_HWBCS_NODE_URL_MAX_LEN     127   //!< Maxmum length of node's URL
#define    BOAT_HWBCS_HOSTNAME_MAX_LEN     127 //!< Maxmum length of hostname
#define    BOAT_HWBCS_CERT_MAX_LEN         1024//!< Maxmum length of certificate


#define    BOAT_HWBCS_ARGS_MAX_NUM         10//!< Arguments max number in hwbcs command
#define    BOAT_HWBCS_ENDORSER_MAX_NUM     10  //!< Support endorser max number
#define    BOAT_HWBCS_ORDERER_MAX_NUM      4   //!< Support orderer max number

#define    BOAT_HWBCS_TLS_SUPPORT          BOAT_TLS_SUPPORT//!< If need client support TLS, set it to 1.
#define    BOAT_HWBCS_TLS_IDENTIFY_CLIENT  BOAT_TLS_IDENTIFY_CLIENT     //!< If server need identify client, set it to 1.

                                                   //!< This macro valid only BOAT_HWBCS_TLS_SUPPORT has
                                                   //!< set to 1. 
												   //!< @note This macro is reserved for future.

#define    BOAT_HWBCS_HTTP2_SEND_BUF_MAX_LEN 8192 //!< The maximum length of HTTP2 send buffer


//!@brief hwbcs transaction type
//! 
typedef enum
{
	HWBCS_TYPE_IDLE = 0,
	HWBCS_TYPE_PROPOSAL,
	HWBCS_TYPE_TRANSACTION,
	HWBCS_TYPE_DISCOVER,
}BoatHwbcsType;

//!@brief hwbcs function type
//! 
typedef enum
{
	HWBCS_FUN_EVALUATE = 0,
	HWBCS_FUN_SUBMIT ,
}BoatHwbcsFunType;

typedef struct ThwbcsResponse
{
	BUINT32								httpResLen;
	BUINT8 								*http2Res;
}hwbcsResponse;

//!@brief hwbcs key pairs structure
//!hwbcs key pairs structure
typedef struct TBoatHwbcsKeyPair
{
	BoatKeypairPriKeyCtx  prikeyCtx; //!< @NOTE This field MUST BE placed in the first member of the structure
	                                //!< because in function BoatWalletCreate(), 
}BoatHwbcsKeyPair;

//!@brief hwbcs all fully trusted top-level CAs
//! hwbcs all fully trusted top-level CAs
typedef struct TBoatHwbcsTlsCAchain
{
	BoatFieldVariable      ca; //!< rootCA certificate content
}BoatHwbcsTlsCAchain;

//!@brief hwbcs wallet structure
//! hwbcs wallet structure
typedef struct TBoatHwbcsWallet
{
	BoatHwbcsKeyPair   account_info; //!< Account information
#if (BOAT_HWBCS_TLS_SUPPORT == 1)	
	BoatHwbcsTlsCAchain    tlsCAchain;   //!< tls rootCA certificate list
#endif /* end of BOAT_HWBCS_TLS_SUPPORT */
    // BoatHwbcsNetworkInfo   network_info; //!< Network information
	BoatHwbcsNetworkData network_info;
	
	void  *http2Context_ptr; //!< http2 information
}BoatHwbcsWallet;

//!@brief hwbcs transaction timestamp structure
//! hwbcs transaction timestamp structure
typedef struct TBoatHwbcsTimestamp
{
	BUINT64 sec;   //!< passing seconds from 1970-01-01 00:00:00
	BUINT64 nanos; //!< Non-negative fractions of a second at nanosecond resolution
}BoatHwbcsTimestamp;

//!@brief chaincodeId structure
//! chaincodeId structure
typedef struct TBoatHwbcsChaincodeId
{
	BCHAR*     path;    //!< The chaincode path, if unused, set it as NULL.
	BCHAR*     name;    //!< The chaincode name
	BCHAR*     version; //!< The chaincode version, if unused, set it as NULL.
}BoatHwbcsChaincodeId;

//! Transaction command arguments
typedef struct TBoatHwbcsArgs
{
	BUINT32    nArgs;
	BCHAR*     args[BOAT_HWBCS_ARGS_MAX_NUM];
}BoatHwbcsArgs;

//!@brief endorser node respond structure
//! 
typedef struct TBoatHwbcsSingleEndorserResponse
{
	void*            contentPtr; //!< response content point
	BoatHwbcsType responseType;
	union
	{
		struct
		{ //!< endorserResponse
			BoatFieldVariable     payload;
			BoatFieldVariable     endorser;
			BoatFieldVariable     signature;
		};
		struct
		{ //!< ordererResponse
			BSINT32               status;//!< for 'query' operation, remote will respond a state code
		};
	};
}BoatHwbcsSingleEndorserResponse;

//!@brief endorser node respond array
//! endorser node respond array
typedef struct TBoatHwbcsProposalResponseArray
{
	BUINT16                            responseCount;
	BoatHwbcsSingleEndorserResponse response[BOAT_HWBCS_ENDORSER_MAX_NUM];
}BoatHwbcsEndorserResponse;

//!@brief hwbcs transaction variable structure
//! hwbcs transaction variable structure
typedef struct TBoatHwbcsVariable
{
	BoatHwbcsType             type; //!< Frame is proposal or trancaction,this field will maintenance by sdk,
	                                   //!< caller should not modify it manually. this field maybe removed in future version!
	BoatHwbcsTimestamp        timestamp;  //!< Transaction occurred datatime
	BoatFieldMax24B              nonce;      //!< This field maintenanced by sdk, caller should not modify it manually
	BoatHwbcsChaincodeId      chaincodeId;//!< Chaincode attribute(path,name,version)
	BoatHwbcsArgs             args;       //!< Transaction arguments,e.g. "query a", "invoke a b 10".
	BCHAR*                       channelId;  //!< Channel name
	BCHAR*                       orgName;    //!< Organization name
	BCHAR*                       contract_name;    //!< contract name
	BCHAR*                       creator_id;    //!< creator id 
}BoatHwbcsVariable;

//!@brief hwbcs transaction high level structure
//! hwbcs transaction high level structure
typedef struct TBoatHwbcsTx
{
	BoatHwbcsWallet*          wallet_ptr;       //!< Pointer of the transaction wallet
	BoatHwbcsVariable         var;              //!< Necessary variable in transaction
	BoatHwbcsEndorserResponse endorserResponse; //!< Endorser respond contents
	hwbcsResponse                evaluateRes;
}BoatHwbcsTx;





#ifdef __cplusplus
extern "C" {
#endif

	// #if (BOAT_HWBCS_TLS_SUPPORT == 1) && (BOAT_HWBCS_TLS_IDENTIFY_CLIENT == 1)
	// /*!****************************************************************************
	//  * @brief
	//  *   Set TLS key pairs of client.
	//  *
	//  * @details
	//  *   This function used to set TLS key pairs of client. If server needs to
	//  *   authenticate the client, this function needs to be enabled.
	//  *
	//  * @param wallet_ptr
	//  *   huawei chain wallet pointer.
	//  *
	//  * @param prikeyCtx_config
	//  *   The private key information to be settings.
	//  *
	//  * @param certContent
	//  *   The content of the certificate to be settings.
	//  *
	//  * @return
	//  *   Return \c BOAT_SUCCESS if set successed, otherwise return a error code.
	//  *
	//  *@note This function is reserved for future. DO NOT use it in your application.
	//  ******************************************************************************/
	// BOAT_RESULT BoatHwbcsWalletSetTlsClientInfo( BoatHwbcsWallet *wallet_ptr,
	// 											    const BoatWalletPriKeyCtx_config prikeyCtx_config,
	// 											    const BoatHwbcsCertInfoCfg certContent );
	// #endif

/**
 * @description: 
 * 	This function init hwbcs wallet by keypair index and network index;
 * @param {BUINT8} keypairIndex
 * @param {BUINT8} networkIndex
 * @return {*}
 *  This function returns hwbcswallet if successfully executed.
 *  Otherwise it returns NULL.
 * @author: aitos
 */
BoatHwbcsWallet *BoatHwbcsWalletInit(BUINT8 keypairIndex,BUINT8 networkIndex);

/*!****************************************************************************
 * @brief 
 *   huawei chain wallet de-initialize.
 * @details
 *   De-initialize huawei chain wallet, include free alloc memory and reset releated pointer
 *   to NULL.
 *
 * @param wallet_ptr 
 *   To be de-initialized huawei chain wallet pointer.
 ******************************************************************************/
void BoatHwbcsWalletDeInit( BoatHwbcsWallet *wallet_ptr );


/*!****************************************************************************
 * @brief 
 *   huawei chain transaction initinal.
 * @details
 *   This function used to Initialize huawei chain transaction. 
 * @param tx_ptr
 *   huawei chain transaction structure pointer to be initialized.
 *
 * @param wallet_ptr 
 *   huawei chain wallet structure pointer to be initialized.
 *
 * @param chaincodeId_path_str 
 *   Chaincode identification member named path to be initialized.
 *
 * @param chaincodeId_name_str
 *   Chaincode identification member named name to be initialized.
 *
 * @param chaincodeId_version_str
 *   Chaincode identification member named version to be initialized.
 *
 * @param channelId_str 
 *   Channel identification to be initialized.
 *
 * @param orgName_str 
 *   Organization name to be initialized.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if transaction initinal success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHwbcsTxInit(BoatHwbcsTx *tx_ptr,
							   const BoatHwbcsWallet *wallet_ptr,
							   const BCHAR *chaincodeId_path_str,
							   const BCHAR *chaincodeId_name_str,
							   const BCHAR *chaincodeId_version_str,
							   const BCHAR *channelId_str,
							   const BCHAR *orgName_str,
							   const BCHAR *contract_name);


/*!****************************************************************************
 * @brief 
 *   huawei chain transaction de-initinal.
 *
 * @details
 *   This function used to De-Initialize huawei chain transaction. 
 *
 * @param tx_ptr 
 *   To be de-initialized huawei chain transaction structure pointer.
 * 
 * @return 
 *   This function doesn't return any thing.
 ******************************************************************************/
void BoatHwbcsTxDeInit(BoatHwbcsTx *tx_ptr);


/*!****************************************************************************
 * @brief 
 *   Set transaction datatime.
 *   
 * @details
 *   This function used to set transaction occurred datatime.
 *
 * @param tx_ptr 
 *   huawei chain transaction structure pointer.
 *
 * @param sec
 *   Passing seconds from 1970-01-01 00:00:00.
 *
 * @param nanos
 *   Non-negative fractions of a second at nanosecond resolution.
 *   Must be from 0 to 999,999,999.
 *
 * @return  
 *   Return \c BOAT_SUCCESS if set success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHwbcsTxSetTimestamp(BoatHwbcsTx *tx_ptr, 
									   const BUINT64 sec, 
									   const BUINT64 nanos);


/*!****************************************************************************
 * @brief 
 *   Set transaction command arguments.
 *
 * @details
 *   This function used to set transaction command arguments.
 *
 * @param tx_ptr 
 *   huawei chain transaction structure pointer.
 *
 * @param arg1
 *   The first argument of command to be settings.
 *
 * @param ...
 *   Remaining arguments, last argument must be NULL.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if set success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHwbcsTxSetArgs(BoatHwbcsTx *tx_ptr,
								  const BCHAR *arg1,
								  ...);

/*!****************************************************************************
 * @brief 
 *   Evaluate transaction.
 *
 * @details
 *   This function should be invoked after BoatHwbcsTxSetTimestamp() and
 *   BoatHwbcsTxSetArgs() had excuted.When you need to get states from
 *   Hwbcs,use this function.
 *
 * @param tx_ptr 
 *   huawei chain transaction structure pointer.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if evaluate success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHwbcsTxEvaluate(BoatHwbcsTx *tx_ptr);

/*!****************************************************************************
 * @brief 
 *   Submit transaction.
 *
 * @details
 *   This function should be invoked after BoatHwbcsTxSetTimestamp() and
 *   BoatHwbcsTxSetArgs() had excuted.When you need to set states to
 *   Hlhuawei,use this function.
 * @param tx_ptr 
 *   huawei chain transaction structure pointer.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if submit success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHwbcsTxSubmit(BoatHwbcsTx *tx_ptr);



/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
