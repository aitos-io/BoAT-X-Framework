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

/*!@brief BoAT IoT SDK interface header file for chainmaker

@file
api_chainmaker.h is header file for BoAT IoT SDK chainmaker's interface.
*/

#ifndef __API_CHAINMAKER_H__
#define __API_CHAINMAKER_H__

#include "boatiotsdk.h"
#include "http2intf.h"
/*! @defgroup chainmaker-api boat chainmaker-API
 * @{
 */
#define BOAT_CHAINMAKER_TLS_SUPPORT               BOAT_TLS_SUPPORT //!< If need client support TLS, set it to 1.
#define BOAT_CHAINMAKER_CERT_MAX_LEN              1024
#define BOAT_HLCHAINMAKER_HTTP2_SEND_BUF_MAX_LEN  8192 //!< The maximum length of HTTP2 send buffer
#define BOAT_HLCHAINMAKER_ARGS_MAX_NUM            10
#define BOAT_RESPONSE_CONTRACT_RESULT_MAX_LEN     128
#define BOAT_RESPONSE_MESSAGE_MAX_LEN             2048
#define BAOT_CHAINMAKER_NODE_STR_LEN              127
// call a pre created user contract, tx included in block
// query a pre-created user contract, tx not included in block
typedef enum {

	TXTYPE_INVOKE_USER_CONTRACT  = 0,
	TXTYPE_QUERY_USER_CONTRACT   = 1,
	TxType_QUERY_SYSTEM_CONTRACT = 3
} TxType;

typedef enum TBoatresponseCode {

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
} BoatresponseCode;


typedef struct TBoatResponseData {	

		BoatresponseCode code;
		char message[BOAT_RESPONSE_MESSAGE_MAX_LEN];
        char contract_result[BOAT_RESPONSE_CONTRACT_RESULT_MAX_LEN];
		BUINT32 gas_used;
} BoatResponseData;


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

typedef struct TBoatHlchainamkerResult {
	BUINT32             code;
	char*               message;
	BoatFieldVariable   payload;

} BoatHlchainamkerResult;

// chainmaker wallet config structure
typedef struct TBoatHlchainmakerWalletConfig {

    //usr private key
    BoatWalletPriKeyCtx_config    user_prikey_cfg;
    BoatHlchainmakerCertInfoCfg   user_cert_cfg;   

    BCHAR  node_url_cfg[BAOT_CHAINMAKER_NODE_STR_LEN];
    BCHAR  host_name_cfg[BAOT_CHAINMAKER_NODE_STR_LEN];
    BCHAR  chain_id_cfg[BAOT_CHAINMAKER_NODE_STR_LEN];
    BCHAR  org_id_cfg[BAOT_CHAINMAKER_NODE_STR_LEN];
    BoatHlchainmakerCertInfoCfg   tls_ca_cert_cfg;
} BoatHlchainmakerWalletConfig;

//!chainmaker key pairs structure
typedef struct TBoatHlchainmakerKeyPair {

	BoatWalletPriKeyCtx  prikeyCtx; //!< @NOTE This field MUST BE placed in the first member of the structure                              //!< because in function BoatWalletCreate(), 
	BoatFieldVariable    cert;      //!< client certificate content
} BoatHlchainmakerKeyPair;

typedef struct BoatChainmakerNodeInfo
{
    BCHAR*  node_url_info;
    BCHAR*  host_name_info;
    BCHAR*  chain_id_info;
    BCHAR*  org_id_info;
}BoatChainmakerNodeInfo;

//chainmaker wallet structure
typedef struct TBoatHlchainmakerWallet {

  BoatHlchainmakerKeyPair   user_cert_prikey_info; //!< user information
  BoatFieldVariable         tls_ca_cert_info;
  BoatChainmakerNodeInfo    node_info;
  struct Thttp2IntfContext  *http2Context_ptr; //!< http2 information
} BoatHlchainmakerWallet;

typedef struct TBoatHlchainamkerTx {

	BoatHlchainmakerWallet*     wallet_ptr;       //!< Pointer of the transaction wallet 
	BoatTransactionPara         trans_para; 
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
BoatHlchainmakerWallet *BoatHlchainmakerWalletInit(const BoatHlchainmakerWalletConfig *config_ptr);


/*!****************************************************************************
 * @brief 
 *   chainmaker transaction initinal.
 * @details
 *   This function used to Initialize fabric transaction.
 * 
 * @param wallet_ptr 
 *   Chainmaker wallet structure pointer to be initialized.
 *  
 * @param tx_ptr
 *   chainmaker transaction structure pointer to be initialized.
 * 
 * @return 
 *   Return \c BOAT_SUCCESS if transaction initinal success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlChainmakerTxInit(const BoatHlchainmakerWallet* wallet_ptr, BoatHlchainmakerTx* tx_ptr);

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
 * @param tx_ptr 
 *  Chainmaker transaction structure pointer.
 * 
 * @param method 
 * 	The pointer to Chainmaker contract method name.
 * 
 * @param contract_name 
 *  Chainmaker contarct name.
 * 
 * @param sync_result 
 *  Get invoke gas.
 * 
 * @param invoke_response 
 *  Node response data.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if submit success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlchainmakerContractInvoke(BoatHlchainmakerTx *tx_ptr, char* method, char* contract_name, bool sync_result, BoatResponseData *response_data);


/*!****************************************************************************
 * @brief 
 *   Contract query.
 *
 * @details
 *  This function used to query contract.
 * 
 * @param tx_ptr 
 *  Chainmaker transaction structure pointer.
 * 
 * @param method 
 *  The pointer to Chainmaker contract method name.
 * 
 * @param contract_name 
 *  Chainmaker contarct name.
 * 
 * @param query_response 
 *  Query response data.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if submit success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlchainmakerContractQuery(BoatHlchainmakerTx *tx_ptr, char* method, char* contract_name, BoatResponseData *response_data);
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
 * @brief Set BoatWallet: URL of blockchain node
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
 * @brief Set BoatWallet: ORG ID
 *
 * @details
 *   This function sets the chain ID of the network.
 *   If the network supports EIP-155, chain ID is part of the transaction
 *   message to sign.
 *   If the network doesn't support EIP-155, chain ID is ignored.
 *
 * @param[in] wallet_ptr
 *   Wallet context pointer.    
 *
 * @param[in] org_id_ptr
 *   Chain ID of the blockchain network to use.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.
 ******************************************************************************/
BOAT_RESULT BoatChainmakerWalletSetOrgId(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *org_id_ptr);

/*!****************************************************************************
 * @brief Set BoatWallet: Chain ID
 *
 * @details
 *   This function sets the chain ID of the network.
 *
 * @param[in] wallet_ptr
 *   Wallet context pointer.    
 *
 * @param[in] chain_id_ptr
 *   Chain ID of the blockchain network to use.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.
 ******************************************************************************/
BOAT_RESULT BoatChainmakerWalletSetChainId(BoatHlchainmakerWallet *wallet_ptr, const BCHAR *chain_id_ptr);

/*!****************************************************************************
 * @brief Set BoatWallet: Host name
 *
 * @details
 *   This function sets the host name of the network.
 * @param[in] wallet_ptr
 *   Wallet context pointer.    
 *
 * @param[in] host_name_ptr
 *  Host name of the blockchain network to use.
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
