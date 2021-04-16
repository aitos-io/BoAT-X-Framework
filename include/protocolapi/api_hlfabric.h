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

//!@brief fabric transaction type
//! 
typedef enum{
	HLFABRIC_TYPE_IDLE = 0,
	HLFABRIC_TYPE_PROPOSAL,
	HLFABRIC_TYPE_TRANSACTION,
}BoatHlfabricType;

//!@brief fabric key pairs structure
//!fabric key pairs structure
typedef struct TBoatHlfabricKeyPair{
	BoatWalletPriKeyCtx  prikeyCtx; //!< @NOTE This field MUST BE placed in the first member of the structure
	                                //!< because in function BoatWalletCreate(), 
	BoatFieldVariable    cert;      //!< client certificate content
}BoatHlfabricKeyPair;


//!@brief fabric all fully trusted top-level CAs
//! fabric all fully trusted top-level CAs
typedef struct TBoatHlfabricTlsCAchain{
	BoatFieldVariable      ca[HLFABRIC_ROOTCA_MAX_NUM]; //!< rootCA certificate content
}BoatHlfabricTlsCAchain;


//!@brief fabric node information
//! fabric node information
typedef struct TBoatHlfabricNodeInfo{
	BCHAR*  nodeUrl;  //!< URL of the blockchain node, e.g. "http://a.b.com:8545"	
	BCHAR*  hostName; //!< tls server hostname, it's 'CN' field in server certificate
					  //!< if HLFABRIC_TLS_SUPPORT is enabled, set is a correct value, otherwise set it NULL.
}BoatHlfabricNodeInfo;


//!@brief fabric network information structure
//! fabric network information structure
typedef struct TBoatHlfabricNetworkInfo{
	BUINT16 endorserNum;  //!< this field is update by SDK, caller should not modify it manually
	BUINT16 ordererNum;   //!< this field is update by SDK, caller should not modify it manually
    BoatHlfabricNodeInfo  endorser[HLFABRIC_ENDORSER_MAX_NUM]; //!< endorser node information
	BoatHlfabricNodeInfo  orderer[HLFABRIC_ORDERER_MAX_NUM];   //!< orderer node information
}BoatHlfabricNetworkInfo;


//!@brief fabric wallet structure
//! fabric wallet structure
typedef struct TBoatHlfabricWallet{
	BoatHlfabricKeyPair   account_info; //!< Account information
#if (HLFABRIC_TLS_SUPPORT == 1)	
#if (HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	BoatHlfabricKeyPair       tlsClinet_info;//!< tls information
#endif /* end of HLFABRIC_TLS_IDENTIFY_CLIENT */
	BoatHlfabricTlsCAchain    tlsCAchain;   //!< tls rootCA certificate list
#endif /* end of HLFABRIC_TLS_SUPPORT */
    BoatHlfabricNetworkInfo   network_info; //!< Network information
	
	struct Thttp2IntfContext  *http2Context_ptr; //!< http2 information
}BoatHlfabricWallet;


//!@brief fabric transaction timestamp structure
//! fabric transaction timestamp structure
typedef struct TBoatHlfabricTimestamp{
	BUINT64 sec;   //!< passing seconds from 1970-01-01 00:00:00
	BUINT64 nanos; //!< Non-negative fractions of a second at nanosecond resolution
}BoatHlfabricTimestamp;


//!@brief chaincodeId structure
//! chaincodeId structure
typedef struct TBoatHlfabricChaincodeId{
	BCHAR*     path;    //!< the chaincode path, if unused, set it as NULL.
	BCHAR*     name;    //!< the chaincode name
	BCHAR*     version; //!< the chaincode version, if unused, set it as NULL.
}BoatHlfabricChaincodeId;

//! transaction command arguments
typedef struct TBoatHlfabricArgs{
	BUINT32    nArgs;
	BCHAR*     args[HLFABRIC_ARGS_MAX_NUM];
}BoatHlfabricArgs;


//!@brief endorser node respond structure
//! 
typedef struct TBoatHlfabricSingleEndorserResponse{
	void*            contentPtr; //!< response content point
	BoatHlfabricType responseType;
	union{
		struct{ //!< endorserResponse
			BoatFieldVariable     payload;
			BoatFieldVariable     endorser;
			BoatFieldVariable     signature;
		};
		struct{ //!< ordererResponse
			BSINT32               status;//!< for 'query' operation, remote will respond a state code
		};
	};
}BoatHlfabricSingleEndorserResponse;


//!@brief endorser node respond array
//! endorser node respond array
typedef struct TBoatHlfabricProposalResponseArray{
	BUINT16                            responseCount;
	BoatHlfabricSingleEndorserResponse response[HLFABRIC_ENDORSER_MAX_NUM];
}BoatHlfabricEndorserResponse;


//!@brief fabric transaction variable structure
//! fabric transaction variable structure
typedef struct TBoatHlfabricVariable{
	BoatHlfabricType             type; //!< frame is proposal or trancaction,this field will maintenance by sdk,
	                                   //!< caller should not modify it manually. this field maybe removed in future version!
	BoatHlfabricTimestamp        timestamp;  //!< transaction occurred datatime
	BoatFieldMax24B              nonce;      //!< this field maintenanced by sdk, caller should not modify it manually
	BoatHlfabricChaincodeId      chaincodeId;//!< chaincode attribute(path,name,version)
	BoatHlfabricArgs             args;       //!< transaction arguments,e.g. "query a", "invoke a b 10".
	BCHAR*                       channelId;  //!< channel name
	BCHAR*                       orgName;    //!< organization name
}BoatHlfabricVariable;


//!@brief fabric transaction high level structure
//! fabric transaction high level structure
typedef struct TBoatHlfabricTx{
	BoatHlfabricWallet*          wallet_ptr;       //!< pointer of the transaction wallet
	BoatHlfabricVariable         var;              //!< necessary variable in transaction
	BoatHlfabricEndorserResponse endorserResponse; //!< endorser respond contents
}BoatHlfabricTx;

//!@brief fabric node information config structure
//! fabric node information config structure
typedef struct TBoatHlfabricNodeInfoCfg{
	BCHAR  nodeUrl[BOAT_FILENAME_MAX_LEN];  //!< URL of the blockchain node, e.g. "http://a.b.com:8545"	
	BCHAR  hostName[BOAT_FILENAME_MAX_LEN]; //!< tls server hostname, it's 'CN' field in server certificate
}BoatHlfabricNodeInfoCfg;

//!@brief fabric wallet config structure
//! fabric wallet config structure
typedef struct TBoatHlfabricWalletConfig{
	BoatWalletPriKeyCtx_config  accountPriKey_config;
	BoatFieldVariable           accountCertContent;   /*!< A pointer to account certificate content, the content of this field point to 
														 will be COPYED to the corresponding field of the wallet, if user dynamically 
														 allocated space for this pointer, then the user should release it after     
	                                                     @ref BoatWalletCreate invoked. 
														 @note For content of type string, the length contains the terminator '\0' */  

	BoatWalletPriKeyCtx_config  tlsPriKey_config;
	BoatFieldVariable           tlsClientCertContent; /*!< A pointer to account certificate content, the content of this field point to 
														 will be COPYED to the corresponding field of the wallet, if user dynamically 
														 allocated space for this pointer, then the user should release it after     
	                                                     @ref BoatWalletCreate invoked. 
														 @note For content of type string, the length contains the terminator '\0' */  	
	
	BUINT32 rootCaNumber; //!< the number of rootCA file to be set
	BoatFieldVariable           rootCaContent[HLFABRIC_ROOTCA_MAX_NUM];/*!< A pointer to rootCA content, the content of this field point to 
																			will be COPYED to the corresponding field of the wallet, if user dynamically 
																			allocated space for this pointer, then the user should release it after     
																			@ref BoatWalletCreate invoked. 
																			@note For content of type string, the length contains the terminator '\0' */  	

	BUINT32                  endorserNumber;//!< the number of endorser to be set
	BoatHlfabricNodeInfoCfg  endorser[HLFABRIC_ENDORSER_MAX_NUM];
	BUINT32                  ordererNumber; //!< the number of orderer to be set
	BoatHlfabricNodeInfoCfg  orderer[HLFABRIC_ORDERER_MAX_NUM];
}BoatHlfabricWalletConfig;

#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief 
 *   set fabric transaction related private key index and certificate information
 * 
 * @details
 *   this function used to set transaction releated key pairs 
 *   information. the private key format is PCKS#8. for fabric, 
 *   the default used curve is SECP256R1. 
 *
 * @param wallet_ptr
 *   fabric wallet pointer
 *
 * @param prikeyCtx_config
 *   xxxxxx
 *
 * @param certContent
 *   xxxxxx
 *
 * @return BOAT_RESULT
 *   return BOAT_SUCCESS if set successed, otherwise return a failed code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricWalletSetAccountInfo( BoatHlfabricWallet *wallet_ptr, 
											  const BoatWalletPriKeyCtx_config prikeyCtx_config,
											  const BoatFieldVariable certContent );


#if (HLFABRIC_TLS_SUPPORT == 1) && (HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
/*!****************************************************************************
 * @brief 
 *   set TLS key pairs of client.
 *
 * @details
 *   this function used to set TLS key pairs of client. If server needs to 
 *   authenticate the client, this function needs to be enabled.
 *
 * @param wallet_ptr 
 *   fabric wallet pointer
 *
 * @param prikeyCtx_config 
 *   xxxxx
 *
 * @param xxxxxx
 *   full path of certificate used by TLS.
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if set successed, otherwise return a failed code.
 *
 *@note this function is reserved for future.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricWalletSetTlsClientInfo( BoatHlfabricWallet *wallet_ptr, 
											    const BoatWalletPriKeyCtx_config prikeyCtx_config,
											    const BoatFieldVariable certContent );
#endif


#if ( HLFABRIC_TLS_SUPPORT == 1 )
/*!****************************************************************************
 * @brief set root CA certificate for TLS connection
 * 
 * @details
 *   set root CA certificate for TLS connection
 *
 * @param wallet_ptr 
 *   fabric wallet pointer
 *
 * @param rootCaFileName
 *   array of root CA certificate filename
 *
 * @param rootCaNumber
 *   number of root CA certificate to be set
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if set successed, otherwise return a failed code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricWalletSetRootCaInfo( BoatHlfabricWallet *wallet_ptr, 
											 const BoatFieldVariable *rootCaContent,
											 BUINT32 rootCaNumber );
#endif


/*!****************************************************************************
 * @brief 
 *   set fabric network information
 * 
 * @details
 *   this function used to set fabric network information, which include 
 *   endorser and orderor node network information.
 *
 * @param wallet_ptr 
 *   fabric wallet pointer
 *
 * @param endorserInfo_ptr
 *   first member address of endorser node info array, the array include node 
 *   URL and node hostname. 
 *   \n node URL is endorser addresss in "IP/domain name:port" format; node 
 *   hostname is the CN field of endorser certificate, it is valid when TLS 
 *   be enabled only.
 *
 * @param endorserNumber
 *   number of endorser to be set
 *
 * @param ordererInfo_ptr
 *   first member address of orderer node info array, the array include node 
 *   URL and node hostname. 
 *   \n node URL is orderer addresss in "IP/domain name:port" format; node 
 *   hostname is the CN field of orderer certificate, it is valid when TLS 
 *   be enabled only.
 *
 * @param ordererNumber
 *   number of orderer to be set
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if set successed, otherwise return a failed code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricWalletSetNetworkInfo( BoatHlfabricWallet *wallet_ptr, 
							const BoatHlfabricNodeInfoCfg *endorserInfo_ptr, BUINT32 endorserNumber,
							const BoatHlfabricNodeInfoCfg *ordererInfo_ptr , BUINT32 ordererNumber );


/*!****************************************************************************
 * @brief 
 *   fabric wallet initinal
 *
 * @details
 *   this function used to initinal fabric wallet, include alloc wallet structrue
 *   memory, setup fabric account information, TLS information and network information.
 *
 * @param config_ptr
 *   the fabric wallet configuration structure pointer
 *
 * @param config_size 
 *   the fabric wallet configuration structure size
 *
 * @return BoatHlfabricWallet* 
 *   if initinal success, return fabric wallet pointer, otherwise return NULL.
 ******************************************************************************/
BoatHlfabricWallet* BoatHlfabricWalletInit( const BoatHlfabricWalletConfig *config_ptr, 
											BUINT32 config_size );


/*!****************************************************************************
 * @brief 
 *   fabric wallet De-initinal
 * @details
 *   de-init fabric wallet, include free alloc memory and reset releated pointer
 *   to NULL.
 *
 * @param wallet_ptr 
 *   to be de-initinal fabric wallet pointer
 ******************************************************************************/
void BoatHlfabricWalletDeInit( BoatHlfabricWallet *wallet_ptr );


/*!****************************************************************************
 * @brief 
 *   fabric transaction initinal
 * @details
 *   this function used 
 * @param tx_ptr
 *   fabric transaction structure pointer
 *
 * @param wallet_ptr 
 *   fabric wallet structure pointer
 *
 * @param chaincodeId_path_str 
 *   chaincode identification member named path
 *
 * @param chaincodeId_name_str
 *   chaincode identification member named name
 *
 * @param chaincodeId_version_str
 *   chaincode identification member named version
 *
 * @param channelId_str 
 *   channel identification
 *
 * @param orgName_str 
 *   organization name
 *
 * @return BOAT_RESULT 
 *   reuturn BOAT_SUCCESS if transaction initinal success, otherwise return failed code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricTxInit( BoatHlfabricTx *tx_ptr, 
							    const BoatHlfabricWallet *wallet_ptr,
							    const BCHAR *chaincodeId_path_str,
							    const BCHAR *chaincodeId_name_str,
							    const BCHAR *chaincodeId_version_str,
							    const BCHAR *channelId_str,
						        const BCHAR *orgName_str );


/*!****************************************************************************
 * @brief 
 *   fabric transaction de-initinal
 *
 * @details
 *
 * @param tx_ptr 
 *        to be de-initinal fabric transaction structure pointer
 ******************************************************************************/
void BoatHlfabricTxDeInit( BoatHlfabricTx *tx_ptr );


/*!****************************************************************************
 * @brief 
 *   set transaction datatime
 *   
 * @details
 *   this function used to set transaction occurred datatime.
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param sec
 *   passing seconds from 1970-01-01 00:00:00
 *
 * @param nanos
 *   Non-negative fractions of a second at nanosecond resolution.
 *   Must be from 0 to 999,999,999
 *
 * @return BOAT_RESULT 
 *   reuturn BOAT_SUCCESS if set success, otherwise return failed code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricTxSetTimestamp( BoatHlfabricTx *tx_ptr, 
										const BUINT64 sec, 
										const BUINT64 nanos );


/*!****************************************************************************
 * @brief 
 *   set transaction command arguments
 *
 * @details
 *   this function used to set transaction command arguments.
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param arg1
 *   the first argument of command
 *
 * @param ...
 *   remaining arguments, last argument must be NULL
 *
 * @return BOAT_RESULT 
 *   reuturn BOAT_SUCCESS if set success, otherwise return failed code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricTxSetArgs( BoatHlfabricTx *tx_ptr, 
								   const BCHAR *arg1, 
								   ... );


/*!****************************************************************************
 * @brief 
 *   submit transaction 
 *
 * @details
 *   this function should be invoked after #BoatHlfabricTxSetTimestamp(...) and
 *   #BoatHlfabricTxSetArgs(...) had excuted.
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @return BOAT_RESULT 
 *   reuturn BOAT_SUCCESS if submit success, otherwise return failed code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricTxSubmit( BoatHlfabricTx *tx_ptr );

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
