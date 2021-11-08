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

#ifndef __API_CHAINMAKER_H__
#define __API_CHAINMAKER_H__

#include "boatiotsdk.h"

/*! @defgroup eth-api boat chainmaker-API
 * @{
 */
#define BOAT_HLFABRIC_TLS_SUPPORT                1 //!< If need client support TLS, set it to 1.
#define BOAT_CHAINMAKER_CERT_MAX_LEN 1024
#define BOAT_CHAINMAKER_ROOTCA_MAX_NUM 3
#define BOAT_HLCHAINMAKER_HTTP2_SEND_BUF_MAX_LEN     8192 //!< The maximum length of HTTP2 send buffer

// call a pre created user contract, tx included in block
// query a pre-created user contract, tx not included in block
typedef enum {

	TxType_INVOKE_USER_CONTRACT = 0,
	TxType_QUERY_USER_CONTRACT
} TxType;

//! chainmaker certificate information config structure
typedef struct TBoatHlchainmakerCertInfoCfg {

	BUINT32  length;                                //!< length of certificate content, this length contains the terminator '\0'.
	BCHAR    content[BOAT_CHAINMAKER_CERT_MAX_LEN]; //!< content of certificate.
} BoatHlchainmakerCertInfoCfg;

typedef struct  TBoatKeyValuePair {

  char *key;
  char *value;
} BoatKeyValuePair;

typedef struct TBoatTransactionPara {
	
	BUINT32 n_parameters;
	BoatKeyValuePair *parameters; 
} BoatTransactionPara;

typedef struct TBoatHlchainmakerNode{

	bool   use_tls;
	BCHAR  *node_url;
	BCHAR  *host_name;
	BoatHlchainmakerCertInfoCfg org_tls_ca_cert;
} BoatHlchainmakerNode;

typedef struct TBoatHlchainamkerClient {

	BCHAR* chain_id;
	BCHAR* org_id;
	BCHAR* contract_name;
} BoatHlchainamkerClient;

typedef struct TBoatHlchainamkerResult {
	BUINT32             code;
	char*               message;
	BoatFieldVariable   payload;

} BoatHlchainamkerResult;

typedef struct TBoatHlchainmakerResponse {

	BoatHlchainamkerResult   http_result;
	BUINT32			         httpResLen;
	BUINT8 			         *http2Res;
}BoatHlchainmakerResponse;

// chainmaker wallet config structure
typedef struct TBoatHlchainmakerWalletConfig {

	//usr private key
	BoatWalletPriKeyCtx_config    user_prikey_config;
	BoatHlchainmakerCertInfoCfg   user_cert_content;   

	BoatWalletPriKeyCtx_config    tls_PriKey_config;
	BoatHlchainmakerCertInfoCfg   tls_cert_content;

	BoatHlchainmakerNode          node_cfg;
}BoatHlchainmakerWalletConfig;

//!chainmaker key pairs structure
typedef struct TBoatHlchainmakerKeyPair {

	BoatWalletPriKeyCtx  prikeyCtx; //!< @NOTE This field MUST BE placed in the first member of the structure                              //!< because in function BoatWalletCreate(), 
	BoatFieldVariable    cert;      //!< client certificate content
} BoatHlchainmakerKeyPair;

//chainmaker wallet structure
typedef struct TBoatHlchainmakerWallet {

	BoatHlchainmakerKeyPair   user_client_info; //!< user information
	BoatHlchainmakerKeyPair   tls_client_info;  //!< tls information
	BoatHlchainmakerNode      node_info;        //!< node information
	
	struct Thttp2IntfContext  *http2Context_ptr; //!< http2 information
} BoatHlchainmakerWallet;

typedef struct TBoatHlchainamkerTx {

	BoatHlchainmakerWallet*     wallet_ptr;       //!< Pointer of the transaction wallet
	BoatHlchainmakerResponse    tx_reponse;   
	BoatHlchainamkerClient      client_info;
}BoatHlchainmakerTx;

BoatHlchainmakerWallet *BoatHlchainmakerWalletInit(const BoatHlchainmakerWalletConfig *config_ptr,
										   BUINT32 config_size);
BOAT_RESULT BoatHlChainmakerTxInit(const BoatHlchainmakerWallet* wallet_ptr,const BCHAR* chain_id, const BCHAR* org_id,
								                   const BCHAR* contract_name, BoatHlchainmakerTx* tx_ptr);
void BoatHlchainmakerTxDeInit(BoatHlchainmakerTx *tx_ptr);
BOAT_RESULT BoatHlchainmakerContractClaimInvoke(BoatHlchainmakerTx *tx_ptr,  BoatTransactionPara *transaction_para, char* method);
BOAT_RESULT BoatHlchainmakerContractClaimQuery(BoatHlchainmakerTx *tx_ptr,  BoatTransactionPara *transaction_para, char* method);


/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
