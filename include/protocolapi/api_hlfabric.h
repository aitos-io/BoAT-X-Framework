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

/*!@brief Header file for performing fabric transaction

@file
api_hlfabric.h is header file for fabric transaction construction and performing.
*/

#ifndef __API_HLFABRIC_H__
#define __API_HLFABRIC_H__

#include "boatiotsdk.h"
/*! @defgroup fabric-api boat fabric-API 
 * @{
 */




#define BOAT_HLFABRIC_ARGS_MAX_NUM               10   //!< Arguments max number in fabric command
#define BOAT_HLFABRIC_ENDORSER_MAX_NUM           10  //!< Support endorser max number
#define BOAT_HLFABRIC_ORDERER_MAX_NUM            4   //!< Support orderer max number



#define BOAT_HLFABRIC_HTTP2_SEND_BUF_MAX_LEN     8192 //!< The maximum length of HTTP2 send buffer



//!@brief fabric transaction type
//! 
typedef enum
{
	HLFABRIC_TYPE_IDLE = 0,
	HLFABRIC_TYPE_PROPOSAL,
	HLFABRIC_TYPE_TRANSACTION,
	HLFABRIC_TYPE_DISCOVER,
}BoatHlfabricType;

//!@brief fabric function type
//! 
typedef enum
{
	HLFABRIC_FUN_EVALUATE = 0,
	HLFABRIC_FUN_SUBMIT ,
}BoatHlfabricFunType;

//!@brief fabric key pairs structure
//!fabric key pairs structure
typedef struct TBoatHlfabricKeyPair
{
	BoatKeypairPriKeyCtx  prikeyCtx; //!< @NOTE This field MUST BE placed in the first member of the structure
	                                //!< because in function BoatWalletCreate(), 
}BoatHlfabricKeyPair;


//!@brief fabric all fully trusted top-level CAs
//! fabric all fully trusted top-level CAs
typedef struct TBoatHlfabricTlsCAchain
{
	BoatFieldVariable      ca; //!< rootCA certificate content
}BoatHlfabricTlsCAchain;




//!@brief fabric wallet structure
//! fabric wallet structure
typedef struct TBoatHlfabricWallet
{
	BoatHlfabricKeyPair   account_info; //!< Account information
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)	
	BoatHlfabricTlsCAchain    tlsCAchain;   //!< tls rootCA certificate list
#endif /* end of BOAT_HLFABRIC_TLS_SUPPORT */
    // BoatHlfabricNetworkInfo   network_info; //!< Network information
	BoatHlfabricNetworkData network_info;
	
	void  *http2Context_ptr; //!< http2 information
}BoatHlfabricWallet;


//!@brief fabric transaction timestamp structure
//! fabric transaction timestamp structure
typedef struct TBoatHlfabricTimestamp
{
	BUINT64 sec;   //!< passing seconds from 1970-01-01 00:00:00
	BUINT64 nanos; //!< Non-negative fractions of a second at nanosecond resolution
}BoatHlfabricTimestamp;


//!@brief chaincodeId structure
//! chaincodeId structure
typedef struct TBoatHlfabricChaincodeId
{
	BCHAR*     path;    //!< The chaincode path, if unused, set it as NULL.
	BCHAR*     name;    //!< The chaincode name
	BCHAR*     version; //!< The chaincode version, if unused, set it as NULL.
}BoatHlfabricChaincodeId;

//! Transaction command arguments
typedef struct TBoatHlfabricArgs
{
	BUINT32    nArgs;
	BCHAR*     args[BOAT_HLFABRIC_ARGS_MAX_NUM];
}BoatHlfabricArgs;


//!@brief endorser node respond structure
//! 
typedef struct TBoatHlfabricSingleEndorserResponse
{
	void*            contentPtr; //!< response content point
	BoatHlfabricType responseType;
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
}BoatHlfabricSingleEndorserResponse;


//!@brief endorser node respond array
//! endorser node respond array
typedef struct TBoatHlfabricProposalResponseArray
{
	BUINT16                            responseCount;
	BoatHlfabricSingleEndorserResponse response[BOAT_HLFABRIC_ENDORSER_MAX_NUM];
}BoatHlfabricEndorserResponse;


//!@brief fabric transaction variable structure
//! fabric transaction variable structure
typedef struct TBoatHlfabricVariable
{
	BoatHlfabricType             type; //!< Frame is proposal or trancaction,this field will maintenance by sdk,
	                                   //!< caller should not modify it manually. this field maybe removed in future version!
	BoatHlfabricTimestamp        timestamp;  //!< Transaction occurred datatime
	BoatFieldMax24B              nonce;      //!< This field maintenanced by sdk, caller should not modify it manually
	BoatHlfabricChaincodeId      chaincodeId;//!< Chaincode attribute(path,name,version)
	BoatHlfabricArgs             args;       //!< Transaction arguments,e.g. "query a", "invoke a b 10".
	BCHAR*                       channelId;  //!< Channel name
	BCHAR*                       orgName;    //!< Organization name
	BCHAR*                       contract_name;    //!< contract name
	BCHAR*                       creator_id;    //!< creator id 
}BoatHlfabricVariable;

typedef struct TfabricResponse
{
	BUINT32								httpResLen;
	BUINT8 								*http2Res;
}fabricResponse;

//!@brief fabric transaction high level structure
//! fabric transaction high level structure
typedef struct TBoatHlfabricTx
{
	BoatHlfabricWallet*          wallet_ptr;       //!< Pointer of the transaction wallet
	BoatHlfabricVariable         var;              //!< Necessary variable in transaction
	BoatHlfabricEndorserResponse endorserResponse; //!< Endorser respond contents
	fabricResponse                evaluateRes;
}BoatHlfabricTx;








#ifdef __cplusplus
extern "C" {
#endif






/*!****************************************************************************
 * @brief 
 *   Fabric wallet initinal.
 *
 * @details
 *   This function used to initinal fabric wallet, include alloc wallet structrue
 *   memory, setup fabric account information, TLS information and network information.
 *
 * @param keypairIndex
 *   The keypair index.
 *
 * @param config_size 
 *   The fabric wallet configuration structure size.
 *
 * @return
 *   If initinal success, return fabric wallet pointer, otherwise return \c NULL.
 ******************************************************************************/
BoatHlfabricWallet *BoatHlfabricWalletInit(BUINT8 keypairIndex,BUINT8 networkIndex);


/*!****************************************************************************
 * @brief 
 *   Fabric wallet de-initialize.
 * @details
 *   De-initialize fabric wallet, include free alloc memory and reset releated pointer
 *   to NULL.
 *
 * @param wallet_ptr 
 *   To be de-initialized fabric wallet pointer.
 ******************************************************************************/
void BoatHlfabricWalletDeInit( BoatHlfabricWallet *wallet_ptr );


/*!****************************************************************************
 * @brief 
 *   Fabric transaction initinal.
 * @details
 *   This function used to Initialize fabric transaction. 
 * @param tx_ptr
 *   Fabric transaction structure pointer to be initialized.
 *
 * @param wallet_ptr 
 *   Fabric wallet structure pointer to be initialized.
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
BOAT_RESULT BoatHlfabricTxInit(BoatHlfabricTx *tx_ptr,
							   const BoatHlfabricWallet *wallet_ptr,
							   const BCHAR *chaincodeId_path_str,
							   const BCHAR *chaincodeId_name_str,
							   const BCHAR *chaincodeId_version_str,
							   const BCHAR *channelId_str,
							   const BCHAR *orgName_str);


/*!****************************************************************************
 * @brief 
 *   Fabric transaction de-initinal.
 *
 * @details
 *   This function used to De-Initialize fabric transaction. 
 *
 * @param tx_ptr 
 *   To be de-initialized fabric transaction structure pointer.
 * 
 * @return 
 *   This function doesn't return any thing.
 ******************************************************************************/
void BoatHlfabricTxDeInit(BoatHlfabricTx *tx_ptr);


/*!****************************************************************************
 * @brief 
 *   Set transaction datatime.
 *   
 * @details
 *   This function used to set transaction occurred datatime.
 *
 * @param tx_ptr 
 *   Fabric transaction structure pointer.
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
BOAT_RESULT BoatHlfabricTxSetTimestamp(BoatHlfabricTx *tx_ptr, 
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
 *   Fabric transaction structure pointer.
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
BOAT_RESULT BoatHlfabricTxSetArgs(BoatHlfabricTx *tx_ptr, 
								  const BCHAR *arg1, 
								  ...);

/*!****************************************************************************
 * @brief 
 *   Evaluate transaction.
 *
 * @details
 *   This function should be invoked after BoatHlfabricTxSetTimestamp() and
 *   BoatHlfabricTxSetArgs() had excuted.When you need to get states from
 *   Hlfabric,use this function.
 *
 * @param tx_ptr 
 *   Fabric transaction structure pointer.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if evaluate success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricTxEvaluate(BoatHlfabricTx *tx_ptr);

/*!****************************************************************************
 * @brief 
 *   Submit transaction.
 *
 * @details
 *   This function should be invoked after BoatHlfabricTxSetTimestamp() and
 *   BoatHlfabricTxSetArgs() had excuted.When you need to set states to
 *   Hlfabric,use this function.
 * @param tx_ptr 
 *   Fabric transaction structure pointer.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if submit success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricTxSubmit(BoatHlfabricTx *tx_ptr);


void fabricWalletConfigFree(BoatHlfabricNetworkConfig wallet_config);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
