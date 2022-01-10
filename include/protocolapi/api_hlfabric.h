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
#include "http2intf.h"
// #include "protocolapi/api_hlfabric_discovery.h"

/*! @defgroup fabric-api boat fabric-API 
 * @{
 */

#define BOAT_HLFABRIC_NODE_URL_MAX_LEN           127  //!< Maxmum length of node's URL
#define BOAT_HLFABRIC_HOSTNAME_MAX_LEN           127  //!< Maxmum length of hostname
#define BOAT_HLFABRIC_CERT_MAX_LEN               1024 //!< Maxmum length of certificate


#define BOAT_HLFABRIC_ARGS_MAX_NUM               10   //!< Arguments max number in fabric command
#define BOAT_HLFABRIC_ENDORSER_MAX_NUM           10  //!< Support endorser max number
#define BOAT_HLFABRIC_ORDERER_MAX_NUM            4   //!< Support orderer max number

#define BOAT_HLFABRIC_TLS_SUPPORT                BOAT_TLS_SUPPORT //!< If need client support TLS, set it to 1.
#define BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT        BOAT_TLS_IDENTIFY_CLIENT //!< If server need identify client, set it to 1.

                                                   //!< This macro valid only BOAT_HLFABRIC_TLS_SUPPORT has
                                                   //!< set to 1. 
												   //!< @note This macro is reserved for future.
#define BOAT_HLFABRIC_ROOTCA_MAX_NUM             3 //!< Support ROOT CA certificate max number

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
	BoatWalletPriKeyCtx  prikeyCtx; //!< @NOTE This field MUST BE placed in the first member of the structure
	                                //!< because in function BoatWalletCreate(), 
	BoatFieldVariable    cert;      //!< client certificate content
}BoatHlfabricKeyPair;


//!@brief fabric all fully trusted top-level CAs
//! fabric all fully trusted top-level CAs
typedef struct TBoatHlfabricTlsCAchain
{
	BoatFieldVariable      ca[BOAT_HLFABRIC_ROOTCA_MAX_NUM]; //!< rootCA certificate content
}BoatHlfabricTlsCAchain;


//!@brief fabric node information
//! fabric node information
typedef struct TBoatHlfabricNodeInfo
{
	BCHAR*  nodeUrl;  //!< URL of the blockchain node, e.g. "http://a.b.com:8545"	
	BCHAR*  hostName; //!< tls server hostname, it's 'CN' field in server certificate
					  //!< if BOAT_HLFABRIC_TLS_SUPPORT is enabled, set is a correct value, otherwise set it NULL.
}BoatHlfabricNodeInfo;


//!@brief fabric network information structure
//! fabric network information structure
// typedef struct TBoatHlfabricNetworkInfo{
// 	BUINT16 endorserNum;  //!< this field is update by SDK, caller should not modify it manually
// 	BUINT16 ordererNum;   //!< this field is update by SDK, caller should not modify it manually
//     BoatHlfabricNodeInfo  endorser[BOAT_HLFABRIC_ENDORSER_MAX_NUM]; //!< endorser node information
// 	BoatHlfabricNodeInfo  orderer[BOAT_HLFABRIC_ORDERER_MAX_NUM];   //!< orderer node information
// }BoatHlfabricNetworkInfo;


//!@brief fabric certificate information config structure
//! fabric certificate information config structure
typedef struct TBoatHlfabricCertInfoCfg{
	BUINT32  length;                              //!< length of certificate content, this length contains the terminator '\0'.
	BCHAR    content[BOAT_HLFABRIC_CERT_MAX_LEN]; //!< content of certificate.
}BoatHlfabricCertInfoCfg;

//!@brief fabric node information config structure
//! fabric node information config structure
typedef struct TBoatHlfabricNodeInfoCfg{
	BCHAR  *nodeUrl;  //!< URL of the blockchain node, e.g. "http://a.b.com:8545"	
	BCHAR  *hostName; //!< TLS server hostname, it's 'CN' field in server certificate
}BoatHlfabricNodeInfoCfg;

//!@brief fabric node group config structure
//! fabric node group config structure
typedef struct TBoatHlfabricNodeGroupCfg{
	BUINT8 endorserNumber;//!< The number of endorser to be set
	BUINT8 quantities;
	BoatHlfabricNodeInfoCfg *endorser;
	BoatHlfabricCertInfoCfg tlsOrgCertContent;
}BoatHlfabricNodeGroupCfg;

//!@brief fabric node layout config structure
//! fabric node layout config structure
typedef struct TBoatHlfabricNodeLayoutCfg{
	BUINT8 endorserGroupNum;
	BoatHlfabricNodeGroupCfg *groupCfg;
}BoatHlfabricNodeLayoutCfg;

//!@brief fabric node  config structure
//! fabric node layout config structure
typedef struct TBoatHlfabricNodesCfg{
	BUINT8 endorserLayoutNum;
	BoatHlfabricNodeLayoutCfg *layoutCfg;
	BoatHlfabricNodeGroupCfg orderCfg;
}BoatHlfabricNodesCfg;

//!@brief fabric wallet structure
//! fabric wallet structure
typedef struct TBoatHlfabricWallet
{
	BoatHlfabricKeyPair   account_info; //!< Account information
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)	
#if (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	BoatHlfabricKeyPair       tlsClinet_info;//!< tls information
#endif /* end of BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT */
	BoatHlfabricTlsCAchain    tlsCAchain;   //!< tls rootCA certificate list
#endif /* end of BOAT_HLFABRIC_TLS_SUPPORT */
    // BoatHlfabricNetworkInfo   network_info; //!< Network information
	BoatHlfabricNodesCfg network_info;
	
	struct Thttp2IntfContext  *http2Context_ptr; //!< http2 information
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


//!@brief fabric transaction high level structure
//! fabric transaction high level structure
typedef struct TBoatHlfabricTx
{
	BoatHlfabricWallet*          wallet_ptr;       //!< Pointer of the transaction wallet
	BoatHlfabricVariable         var;              //!< Necessary variable in transaction
	BoatHlfabricEndorserResponse endorserResponse; //!< Endorser respond contents
	Http2Response                evaluateRes;
}BoatHlfabricTx;





//!@brief fabric wallet config structure
//! fabric wallet config structure
typedef struct TBoatHlfabricWalletConfig
{
	BoatWalletPriKeyCtx_config  accountPriKey_config;
	BoatHlfabricCertInfoCfg     accountCertContent;   //!< certificate content of account

	BoatWalletPriKeyCtx_config  tlsPriKey_config;
	BoatHlfabricCertInfoCfg     tlsClientCertContent; //!< certificate content of TLS 	
	
	BUINT32 rootCaNumber; //!< The number of rootCA file to be set
	BoatHlfabricCertInfoCfg     rootCaContent[BOAT_HLFABRIC_ROOTCA_MAX_NUM];//!< certificate content of rootCA
 
	// BUINT32                     endorserNumber;//!< The number of endorser to be set
	// BoatHlfabricNodeInfoCfg     endorser[BOAT_HLFABRIC_ENDORSER_MAX_NUM];
	BoatHlfabricNodesCfg 		nodesCfg;
	// BUINT32                     ordererNumber; //!< The number of orderer to be set
	// BoatHlfabricNodeInfoCfg     orderer[BOAT_HLFABRIC_ORDERER_MAX_NUM];
}BoatHlfabricWalletConfig;



#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief 
 *   Set fabric transaction related private key index and certificate information.
 * 
 * @details
 *   This function used to set transaction releated key pairs information.
 *   The private key format is PCKS#8. for fabric, the default used curve is SECP256R1.
 *
 * @param wallet_ptr
 *   Fabric wallet pointer.
 *
 * @param prikeyCtx_config
 *   The private key information to be settings.
 *
 * @param certContent
 *   The content of the certificate to be settings.
 *
 * @return
 *   Return \c BOAT_SUCCESS if set successed, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricWalletSetAccountInfo(BoatHlfabricWallet *wallet_ptr, 
											 const BoatWalletPriKeyCtx_config prikeyCtx_config,
											 const BoatHlfabricCertInfoCfg certContent);


#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
/*!****************************************************************************
 * @brief 
 *   Set TLS key pairs of client.
 *
 * @details
 *   This function used to set TLS key pairs of client. If server needs to 
 *   authenticate the client, this function needs to be enabled.
 *
 * @param wallet_ptr 
 *   Fabric wallet pointer.
 *
 * @param prikeyCtx_config 
 *   The private key information to be settings.
 *
 * @param certContent
 *   The content of the certificate to be settings.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if set successed, otherwise return a error code.
 *
 *@note This function is reserved for future. DO NOT use it in your application.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricWalletSetTlsClientInfo( BoatHlfabricWallet *wallet_ptr, 
											    const BoatWalletPriKeyCtx_config prikeyCtx_config,
											    const BoatHlfabricCertInfoCfg certContent );
#endif


#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
/*!****************************************************************************
 * @brief Set root CA certificate for TLS connection.
 * 
 * @details
 *   Set root CA certificate for TLS connection.
 *
 * @param wallet_ptr 
 *   Fabric wallet pointer.
 *
 * @param rootCaContent
 *   Array of root CA certificate content.
 *
 * @param rootCaNumber
 *   Number of root CA certificate to be set.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if set successed, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricWalletSetRootCaInfo(BoatHlfabricWallet *wallet_ptr, 
											const BoatHlfabricCertInfoCfg *rootCaContent,
											BUINT32 rootCaNumber);
#endif


/*!****************************************************************************
 * @brief 
 *   Set fabric network information.
 * 
 * @details
 *   This function used to set fabric network information, which include 
 *   endorser and orderor node network information.
 *
 * @param wallet_ptr 
 *   Fabric wallet pointer.
 *
 * @param endorserInfo_ptr
 *   First member address of endorser node info array, the array include node 
 *   URL and node hostname. 
 *   \n Node URL is endorser addresss in "IP/domain name:port" format; node 
 *   hostname is the CN field of endorser certificate, it is valid when TLS 
 *   be enabled only.
 *
 * @param endorserNumber
 *   Number of endorser to be set.
 *
 * @param ordererInfo_ptr
 *   First member address of orderer node info array, the array include node 
 *   URL and node hostname. 
 *   \n Node URL is orderer addresss in "IP/domain name:port" format; node 
 *   hostname is the CN field of orderer certificate, it is valid when TLS 
 *   be enabled only.
 *
 * @param ordererNumber
 *   Number of orderer to be set.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if set successed, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricWalletSetNetworkInfo( BoatHlfabricWallet *wallet_ptr, 
							const BoatHlfabricNodesCfg endorserInfo_ptr );


/*!****************************************************************************
 * @brief 
 *   Fabric wallet initinal.
 *
 * @details
 *   This function used to initinal fabric wallet, include alloc wallet structrue
 *   memory, setup fabric account information, TLS information and network information.
 *
 * @param config_ptr
 *   The fabric wallet configuration structure pointer.
 *
 * @param config_size 
 *   The fabric wallet configuration structure size.
 *
 * @return
 *   If initinal success, return fabric wallet pointer, otherwise return \c NULL.
 ******************************************************************************/
BoatHlfabricWallet *BoatHlfabricWalletInit(const BoatHlfabricWalletConfig *config_ptr, 
										   BUINT32 config_size);


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
 * @param contract_name 
 *   contract name to be initialized.
 * 
 * @param creator_id 
 *   cteator id to be initialized.
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


void fabricWalletConfigFree(BoatHlfabricWalletConfig wallet_config);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
