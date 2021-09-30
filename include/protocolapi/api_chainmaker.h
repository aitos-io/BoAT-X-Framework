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

/*!@brief BoAT IoT SDK interface header file for ethereum

@file
api_ethereum.h is header file for BoAT IoT SDK ethereum's interface.
*/

#ifndef __API_ETHEREUM_H__
#define __API_ETHEREUM_H__

#include "boatiotsdk.h"

/*! @defgroup eth-api boat ethereum-API
 * @{
 */
#define BOAT_CHAINMAKER_CERT_MAX_LEN 1024
#define BOAT_CHAINMAKER_ROOTCA_MAX_NUM 3
typedef struct TBoatChainmakerSender {
	// organization identifier of the member
	char* orgId;
	// member identity related info bytes
	char* memberInfo;
	// use cert compression
	// todo: is_full_cert -> compressed
	bool  isFullCert;

} BoatChainmakerSender;


typedef struct TBoatChainmakerTxHeader {
	// blockchain identifier
	char*                chainId;     
	// sender identifier
	BoatChainmakerSender sender;
	// transaction type
	BUINT32              txType;   
	// transaction id set by sender, should be unique
	char*                txId;
	// transaction timestamp, in unix timestamp format, seconds
	BUINT64              timestamp;
	// expiration timestamp in unix timestamp format
	// after that the transaction is invalid if it is not included in block yet
	BUINT64              expirationTime;
	
} BoatChainmakerTxHeader;

typedef struct TKeyValuePair {
	char* key;  
	char* value;
} KeyValuePair;

typedef struct TBoatChainmakerTransactPayload {
	// smart contract name
	char* contractName; 
	// invoke method
	char* method; 
	// invoke parameters in k-v format
	KeyValuePair* Parameters; 
} BoatChainmakerTransactPayload;

//request 
typedef struct TBoatChainmkaerTxRequest {
	BoatChainmakerTxHeader  tx_header;
	BoatChainmakerTransactPayload* payload;
	char* signature;
} BoatChainmkaerTxRequest;

//! chainmaker certificate information config structure
typedef struct TBoatChainmakerCertInfoCfg {
	BUINT32  length;                                //!< length of certificate content, this length contains the terminator '\0'.
	BCHAR    content[BOAT_CHAINMAKER_CERT_MAX_LEN]; //!< content of certificate.
} BoatChainmakerCertInfoCfg;

typedef struct TBoatChainmakerNode{
	BCHAR*  addr;  
	BUINT32 connCnt;
	bool    useTLS;
	BCHAR*  tlsHostName; 
	
} BoatChainmakerNode;

// chainmaker wallet config structure
typedef struct TBoatChainmakerWalletConfig
{
	BoatWalletPriKeyCtx_config  user_pri_key_config;
	// certificate content of account
	BoatChainmakerCertInfoCfg   user_cert_content;   
	BoatWalletPriKeyCtx_config  user_sign_pri_key_config;
	//certificate content of TLS 
	BoatChainmakerCertInfoCfg   user_sign_cert_content;	

	BUINT32 rootCaNumber; //!< The number of rootCA file to be set
	BoatChainmakerCertInfoCfg     roo_ca_cert[BOAT_CHAINMAKER_ROOTCA_MAX_NUM];//!< certificate content of rootCA

	BoatChainmakerNode 		    node_info;
}BoatChainmakerWalletConfig;

//!chainmaker key pairs structure
typedef struct TBoatChainmakerKeyPair {
	BoatWalletPriKeyCtx  prikeyCtx; //!< @NOTE This field MUST BE placed in the first member of the structure                              //!< because in function BoatWalletCreate(), 
	BoatFieldVariable    cert;      //!< client certificate content
	
} BoatChainmakerKeyPair;

//! chainmaker all fully trusted top-level CAs
typedef struct TBoatChainmakerRootCA
{
	BoatFieldVariable      ca[BOAT_CHAINMAKER_ROOTCA_MAX_NUM]; //!< rootCA certificate content
}BoatChainmakerRootCA;

//chainmaker wallet structure
typedef struct TBoatChainmakerWallet
{
	BoatChainmakerKeyPair   signClient_info; //!< sign information
	BoatChainmakerKeyPair   tlsClinet_info;  //!< tls information
	BoatChainmakerRootCA    tlsRootCA_info;  //!< rootCA certificate list
	BoatChainmakerNode      node_info;       //!< node information
	
	struct Thttp2IntfContext  *http2Context_ptr; //!< http2 information
}BoatChainmakerWallet;

typedef struct TBoatChainmakerTimestamp
{
	BUINT64 sec;   //!< passing seconds from 1970-01-01 00:00:00
	BUINT64 nanos; //!< Non-negative fractions of a second at nanosecond resolution
}BoatChainmakerTimestamp;


/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
