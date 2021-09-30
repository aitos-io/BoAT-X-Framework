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

	char* contractName; 
	char* method; 
	KeyValuePair* Parameters; 
} BoatChainmakerTransactPayload;

//request 
typedef struct TBoatChainmkaerTxRequest {
	BoatChainmakerTxHeader  tx_header;
	BoatChainmakerTransactPayload* payload;
	char* signature;
} BoatChainmkaerTxRequest;





/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
