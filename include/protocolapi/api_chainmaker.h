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
#define BOAT_CHAINMAKER_TLS_SUPPORT               0 //!< If need client support TLS, set it to 1.
#define BOAT_CHAINMAKER_CERT_MAX_LEN              1024
#define BOAT_HLCHAINMAKER_HTTP2_SEND_BUF_MAX_LEN  8192 //!< The maximum length of HTTP2 send buffer
#define BOAT_HLCHAINMAKER_ARGS_MAX_NUM            10
#define BOAT_RESPONSE_CONTRACT_RESULT_MAX_LEN     100
#define BOAT_RESPONSE_MESSAGE_MAX_LEN             10
#define BAOT_CHAINMAKER_URL_HOSTNAME_LEN          127
// call a pre created user contract, tx included in block
// query a pre-created user contract, tx not included in block
typedef enum {

	TXTYPE_INVOKE_USER_CONTRACT  = 0,
	TXTYPE_QUERY_USER_CONTRACT   = 1,
	TxType_QUERY_SYSTEM_CONTRACT = 3
} TxType;

typedef enum TBoatReponseCode {

	SUCCESS                                = 0,
	TIMEOUT                                = 1,
	INVALIDPARAMETER                       = 2,
	NOPERMISSION                           = 3,
	CONTRACTFAIL                           = 4,
	INTERNALERROR                          = 5,
	INVALIDCONTRACTTRANSACTIONTYPE         = 10,
	INVALIDCONTRACTPARAMETERCONTRACTNAME   = 11,
	INVALIDCONTRACTPARAMETERMETHOD         = 12,
	INVALIDCONTRACTPARAMETERINITMETHOD     = 13,
	INVALIDCONTRACTPARAMETERUPGRADEMETHOD  = 14,
	INVALIDCONTRACTPARAMETERBYTECODE       = 15,
	INVALIDCONTRACTPARAMETERRUNTIMETYPE    = 16,
	INVALIDCONTRACTPARAMETERVERSION        = 17,
	GETFROMTXCONTEXTFAILED                 = 20,
	PUTINTOTXCONTEXTFAILED                 = 21,
	CONTRACTVERSIONEXISTFAILED             = 22,
	CONTRACTVERSIONNOTEXISTFAILED          = 23,
	CONTRACTBYTECODENOTEXISTFAILED         = 24,
	MARSHALSENDERFAILED                    = 25,
	INVOKEINITMETHODFAILED                 = 26,
	INVOKEUPGRADEMETHODFAILED              = 27,
	CREATERUNTIMEINSTANCEFAILED            = 28,
	UNMARSHALCREATORFAILED                 = 29,
	UNMARSHALSENDERFAILED                  = 30,
	GETSENDERPKFAILED                      = 31,
	GETCREATORPKFAILED                     = 32,
	GETCREATORFAILED                       = 33,
	GETCREATORCERTFAILED                   = 34,
	GETSENDERCERTFAILED                    = 35,
	CONTRACTFREEZEFAILED                   = 36,
	CONTRACTTOODEEPFAILED                  = 37,
	CONTRACTREVOKEFAILED                   = 38,
	CONTRACTINVOKEMETHODFAILED             = 39,
	ARCHIVEDTX                             = 40,
	ARCHIVEDBLOCK                          = 41
} BoatReponseCode;


typedef struct TBoatInvokeReponse {	

		BoatReponseCode code;
		char message[BOAT_RESPONSE_MESSAGE_MAX_LEN];
		BUINT32 gas_used;
} BoatInvokeReponse;

typedef struct TBoatQueryReponse {	
    BoatReponseCode code;
		char message[BOAT_RESPONSE_MESSAGE_MAX_LEN];
		char contract_result[BOAT_RESPONSE_CONTRACT_RESULT_MAX_LEN];
		BUINT32 gas_used;
} BoatQueryReponse;

//! chainmaker certificate information config structure
typedef struct TBoatHlchainmakerCertInfoCfg {

	BUINT32  length;                             
	BCHAR    content[BOAT_CHAINMAKER_CERT_MAX_LEN]; 
} BoatHlchainmakerCertInfoCfg;

typedef struct  TBoatKeyValuePair {

  char *key;
  char *value;
} BoatKeyValuePair;

typedef struct TBoatTransactionPara {
	
	BUINT32 n_parameters;
	BoatKeyValuePair parameters[BOAT_HLCHAINMAKER_ARGS_MAX_NUM]; 
} BoatTransactionPara;

typedef struct TBoatHlchainmakerNode {

	bool   use_tls;
	BCHAR  *node_url;
	BCHAR  *host_name;
	BoatHlchainmakerCertInfoCfg org_tls_ca_cert;
} BoatHlchainmakerNode;

typedef struct TBoatHlchainamkerClient {

	BCHAR* chain_id;
	BCHAR* org_id;
} BoatHlchainamkerClient;

typedef struct TBoatHlchainamkerResult {
	BUINT32             code;
	char*               message;
	BoatFieldVariable   payload;

} BoatHlchainamkerResult;

// chainmaker wallet config structure
typedef struct TBoatHlchainmakerWalletConfig {

	//usr private key
	BoatWalletPriKeyCtx_config    user_prikey_config;
	BoatHlchainmakerCertInfoCfg   user_cert_content;   

	BoatWalletPriKeyCtx_config    tls_PriKey_config;
	BoatHlchainmakerCertInfoCfg   tls_cert_content;

	BCHAR  node_url_arry[BAOT_CHAINMAKER_URL_HOSTNAME_LEN];
  BCHAR  host_name_arry[BAOT_CHAINMAKER_URL_HOSTNAME_LEN];
  BoatHlchainmakerCertInfoCfg org_tls_ca_cert;
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
	
  BCHAR*  node_url;
  BCHAR*  host_name;
  BoatHlchainmakerCertInfoCfg org_tls_ca_cert;
	
	struct Thttp2IntfContext  *http2Context_ptr; //!< http2 information
} BoatHlchainmakerWallet;

typedef struct TBoatHlchainamkerTx {

	BoatHlchainmakerWallet*     wallet_ptr;       //!< Pointer of the transaction wallet 
	BoatTransactionPara         trans_para;
	BoatHlchainamkerClient      client_info;     
}BoatHlchainmakerTx;


/*!****************************************************************************
 * @brief 
 *   chainmaker wallet initinal.
 *
 * @details
 *   This function used to initinal chainmaker wallet, include alloc wallet structrue
 *   memory, setup chainmaker account information, TLS information and network information.
 *
 * @param config_ptr
 *   The chainmaker wallet configuration structure pointer.
 *
 * @param config_size 
 *   The chainmaker wallet configuration structure size.
 *
 * @return
 *   If initinal success, return chainmaker wallet pointer, otherwise return \c NULL.
 ******************************************************************************/
BoatHlchainmakerWallet *BoatHlchainmakerWalletInit(const BoatHlchainmakerWalletConfig *config_ptr,BUINT32 config_size);


/*!****************************************************************************
 * @brief 
 *   chainmaker transaction initinal.
 * @details
 *   This function used to Initialize fabric transaction.
 * 
 * @param wallet_ptr 
 *   Fabric wallet structure pointer to be initialized.
 * 
 * @param chain_id 
 *   Channel identification to be initialized.
 * 
 * @param org_id 
 *   Channel Organization id to be initialized.
 *  
 * @param tx_ptr
 *   chainmaker transaction structure pointer to be initialized.
 * 
 * @return 
 *   Return \c BOAT_SUCCESS if transaction initinal success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlChainmakerTxInit(const BoatHlchainmakerWallet* wallet_ptr, BCHAR* chain_id, BCHAR* org_id,
								                   BoatHlchainmakerTx* tx_ptr);

/*!****************************************************************************
 * @brief 
 *   Set transaction datatime.
 *   
 * @details
 *   This function used to set transaction occurred datatime.
 *
 * @param tx_ptr 
 *   Chainmaker transaction structure pointer.
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
void BoatHlchainmakerTxDeInit(BoatHlchainmakerTx *tx_ptr);


/*!****************************************************************************
 * @brief 
 *   Set transaction command arguments.
 *
 * @details
 *   This function used to set transaction command arguments.
 *
 * @param tx_ptr 
 *   Chainmaker transaction structure pointer.
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
BOAT_RESULT BoatHlchainmakerAddTxParam(BoatHlchainmakerTx *tx_ptr, BUINT8 length, const BCHAR *arg, ...);


/*!****************************************************************************
 * @brief 
 *   Invoke transaction.
 *
 * @details
 *  This function used to excute invoke process.
 * 
 * @param method 
 *   Chainmaker transaction structure pointer.
 * 
 * @param contract_name 
 *   Chainmaker contarct name.
 * 
 * @param sync_result 
 *   Get invoke gas.
 * 
 * @param invoke_reponse 
 *   Node response data.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if submit success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlchainmakerContractInvoke(BoatHlchainmakerTx *tx_ptr, char* method, char* contract_name, bool sync_result, BoatInvokeReponse *invoke_reponse);


/*!****************************************************************************
 * @brief 
 *   Invoke transaction.
 *
 * @details
 *  This function used to excute invoke process.
 * 
 * @param method 
 *   Chainmaker transaction structure pointer.
 * 
 * @param contract_name 
 *   Chainmaker contarct name.
 * 
 * @param invoke_reponse 
 *   Node response data.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if submit success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlchainmakerContractQuery(BoatHlchainmakerTx *tx_ptr, char* method, char* contract_name, BoatQueryReponse *query_reponse);
/*!****************************************************************************
 * @brief 
 *   chainmaker wallet de-initialize.
 * @details
 *   De-initialize chainmaker wallet, include free alloc memory and reset releated pointer
 *   to NULL.
 *
 * @param wallet_ptr 
 *   To be de-initialized chainmaker wallet pointer.
 ******************************************************************************/
void BoatHlchainmakerWalletDeInit(BoatHlchainmakerWallet *wallet_ptr);

/*!****************************************************************************
 * @brief Set BoatHlchainmakerWallet: URL of blockchain node
 *
 * @details
 *   This function sets the URL of the blockchain node to connect to.
 *   \n A URL is composed of protocol, IP address/name and port, in a form:
 *   http://a.b.com:8545
 *
 * @param[in] wallet_ptr
 *   Wallet context pointer.    
 *
 * @param[in] node_url_ptr
 *   A string indicating the URL of blockchain node to connect to.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.        
 ******************************************************************************/
BOAT_RESULT BoatChainmakerWalletSetNodeUrl(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *node_url_ptr);

/*!****************************************************************************
 * @brief Set BoatHlchainmakerWallet: host name of blockchain node
 *
 * @details
 *   This function sets the URL of the blockchain node to connect to.
 *   \n A URL is composed of protocol, IP address/name and port, in a form:
 *   http://a.b.com:8545
 *
 * @param[in] wallet_ptr
 *   Wallet context pointer.    
 *
 * @param[in] host_name_ptr
 *   A string indicating the URL of blockchain node to connect to.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.        
 ******************************************************************************/
BOAT_RESULT BoatChainmakerWalletSetHostName(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *host_name_ptr);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
