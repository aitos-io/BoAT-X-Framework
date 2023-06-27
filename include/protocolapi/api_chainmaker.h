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
/*! @defgroup chainmaker-api boat chainmaker-API
 * @{
 */

#define BOAT_TXID_LEN 65
#define BOAT_RETRY_CNT 10
#define BOAT_CHAINMAKER_MINE_INTERVAL 3 //!< Mining Interval of the blockchain, in seconds
#define BOAT_CHAINMAKER_PRIKEY_MAX_LEN 512
#define BOAT_CHAINMAKER_CERT_MAX_LEN 1024
#define BOAT_HLCHAINMAKER_HTTP2_SEND_BUF_MAX_LEN 8192 //!< The maximum length of HTTP2 send buffer
#define BOAT_HLCHAINMAKER_ARGS_MAX_NUM 10
// call a pre created user contract, tx included in block
// query a pre-created user contract, tx not included in block
typedef enum
{
  TXTYPE_INVOKE_USER_CONTRACT = 0,
  TXTYPE_QUERY_USER_CONTRACT = 1,
  TXTYPE_QUERY_SYSTEM_CONTRACT = 3
} TxType;

typedef enum TBoatContractResultCode 
{
  BOAT_CONTRACT_RESULT_CODE_OK   = 0,
  BOAT_CONTRACT_RESULT_CODE_FAIL = 1
} BoatContractResultCode;

typedef enum TBoatResponseCode
{
  SUCCESS = 0,
  TIMEOUT = 1,
  INVALIDPARAMETER = 2,
  NOPERMISSION = 3,
  CONTRACTFAIL = 4,
  INTERNALERROR = 5,
  INVALIDCONTRACTTRANSACTIONTYPE = 10,
  INVALIDCONTRACTPARAMETERCONTRACTNAME = 11,
  INVALIDCONTRACTPARAMETERMETHOD = 12,
  INVALIDCONTRACTPARAMETERINITMETHOD = 13,
  INVALIDCONTRACTPARAMETERUPGRADEMETHOD = 14,
  INVALIDCONTRACTPARAMETERBYTECODE = 15,
  INVALIDCONTRACTPARAMETERRUNTIMETYPE = 16,
  INVALIDCONTRACTPARAMETERVERSION = 17,
  GETFROMTXCONTEXTFAILED = 20,
  PUTINTOTXCONTEXTFAILED = 21,
  CONTRACTVERSIONEXISTFAILED = 22,
  CONTRACTVERSIONNOTEXISTFAILED = 23,
  CONTRACTBYTECODENOTEXISTFAILED = 24,
  MARSHALSENDERFAILED = 25,
  INVOKEINITMETHODFAILED = 26,
  INVOKEUPGRADEMETHODFAILED = 27,
  CREATERUNTIMEINSTANCEFAILED = 28,
  UNMARSHALCREATORFAILED = 29,
  UNMARSHALSENDERFAILED = 30,
  GETSENDERPKFAILED = 31,
  GETCREATORPKFAILED = 32,
  GETCREATORFAILED = 33,
  GETCREATORCERTFAILED = 34,
  GETSENDERCERTFAILED = 35,
  CONTRACTFREEZEFAILED = 36,
  CONTRACTTOODEEPFAILED = 37,
  CONTRACTREVOKEFAILED = 38,
  CONTRACTINVOKEMETHODFAILED = 39,
  ARCHIVEDTX = 40,
  ARCHIVEDBLOCK = 41
} BoatResponseCode;

typedef struct TBoatKeyValuePair
{
  char *key;
  BoatFieldVariable value;
} BoatKeyValuePair;

typedef struct TBoatTransactionParams
{
  BUINT32 n_parameters;
  BoatKeyValuePair parameters[BOAT_HLCHAINMAKER_ARGS_MAX_NUM];
} BoatTransactionParams;

typedef struct TBoatContractResult
{
  BUINT64                gas_used;
  BoatContractResultCode contract_code;
  BCHAR                  *contract_message;
  BoatFieldVariable      payload;
} BoatContractResult;

typedef struct TBoatResponseData
{
  BoatResponseCode code;
  char *message;
  char tx_id[BOAT_TXID_LEN];
  BoatContractResult contract_result;
} BoatResponseData;


//! chainmaker certificate information config structure
typedef struct TBoatHlchainmakerCertInfoCfg
{
  BUINT32 length;
  BCHAR content[BOAT_CHAINMAKER_CERT_MAX_LEN];
} BoatHlchainmakerCertInfoCfg;

//!@brief chainmaker key pairs structure
//! chainmaker key pairs structure
typedef struct TBoatChainmakerKeyPair
{
  BoatKeypairPriKeyCtx prikeyCtx; //!< @NOTE This field MUST BE placed in the first member of the structure
} BoatChainmakerKeyPair;

// chainmaker wallet structure
typedef struct TBoatChainmakerWallet
{

  BoatChainmakerKeyPair account_info; //!< Account information
  BoatChainmakerNetworkData network_info;
  void *http2Context_ptr; //!< http2 information
} BoatChainmakerWallet;

typedef struct TBoatChainamkerTx
{
  BoatChainmakerWallet  *wallet_ptr; //!< Pointer of the transaction wallet
  BoatTransactionParams trans_para;
  BUINT64 gas_limit;
} BoatChainmakerTx;

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
BoatChainmakerWallet *BoatChainmakerWalletInit(BUINT8 keypairIndex, BUINT8 networkIndex);

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
BOAT_RESULT BoatChainmakerTxInit(const BoatChainmakerWallet *wallet_ptr, BoatChainmakerTx *tx_ptr, BUINT64 gas_limit);

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
BOAT_RESULT BoatChainmakerAddTxParam(BoatChainmakerTx *tx_ptr, BUINT8 length, const BCHAR *arg, ...);

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
 *  The pointer to Chainmaker contract method name.
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
BOAT_RESULT BoatChainmakerContractInvoke(BoatChainmakerTx *tx_ptr, char *method, char *contract_name, bool sync_result, BoatResponseData *response_data);

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
BOAT_RESULT BoatChainmakerContractQuery(BoatChainmakerTx *tx_ptr, char *method, char *contract_name, BoatResponseData *response_data);
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
void BoatChainmakerWalletDeInit(BoatChainmakerWallet *wallet_ptr);

void BoatChainmakerResponseFree(BoatResponseData *response_data);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
