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

/*!@brief BoAT IoT SDK interface header file for PlatONE

@file
api_platone.h is header file for BoAT IoT SDK PlatONE's interface.
*/

#ifndef __API_PLATONE_H__
#define __API_PLATONE_H__

#include "boatiotsdk.h"
#include "api_ethereum.h"

/*! @defgroup platone-api boat platone-API
 * @{
 */

#define BOAT_PLATONE_NODE_URL_MAX_LEN           BOAT_ETH_NODE_URL_MAX_LEN

#define BOAT_PLATONE_NONCE_AUTO                  BOAT_ETH_NONCE_AUTO
#define BOAT_PLATONE_ADDRESS_SIZE                BOAT_ETH_ADDRESS_SIZE

typedef enum
{
BOAT_PLATONE_TX_TYPE_NORMAL_TRANSFER = 0x00,
BOAT_PLATONE_TX_TYPE_CONTRACT = 0x02,
BOAT_PLATONE_TX_TYPE_CONTRACT_NULL_TERMED_STR = 0x09,
}BoatPlatoneTxtype;

typedef BoatEthAccountInfo  BoatPlatoneAccountInfo;
typedef BoatEthNetworkInfo  BoatPlatoneNetworkInfo;
typedef BoatEthWallet       BoatPlatoneWallet;
typedef BoatEthWalletConfig BoatPlatoneWalletConfig;
typedef BoatEthTxFieldSig   BoatPlatoneTxFieldSig;

//!@brief RAW PlatONE transaction fields

//! The only difference from Ethereum is appending a txtype field at the end of the
//! struct. Thus a BoatPlatoneRawtxFields pointer can be transfered to any function
//! that accepts a BoatEthRawtxFields argument. It behaves as a C implementation of
//! class inheritance, i.e. a pointer of an inherited class could be assigned to a
//! pointer of a base class.
typedef struct TBoatPlatoneRawtxFields
{
    // Following fields are inherited from Ethereum.
    // DO NOT modify these fields unless all of Ethereum and all other protocols
    // inherited from Ethereum are modified synchronously.
    BoatFieldMax32B nonce;        //!< nonce, uint256 in bigendian, equal to the transaction count of the sender's account address
    BoatFieldMax32B gasprice;     //!< gasprice in wei, uint256 in bigendian
    BoatFieldMax32B gaslimit;     //!< gaslimit, uint256 in bigendian
    BUINT8 recipient[BOAT_ETH_ADDRESS_SIZE]; //!< recipient's address, 160 bits
    BoatFieldMax32B value;        //!< value to transfer, uint256 in bigendian
    BoatFieldVariable data;       //!< data to transfer, unformatted stream
    BoatFieldMax4B v;             //!< chain id or recovery identifier, @see RawtxPerform()
    BoatEthTxFieldSig sig;        //!< ECDSA signature, including r and s parts
    
    // PlatONE specific fields are appended here.
    BoatPlatoneTxtype txtype;     //!< Transaction Type

}BoatPlatoneRawtxFields;

//!@brief PlatONE Transaction

//! The only difference between PlatONE transaction and Ethereum transaction is
//! BoatPlatoneRawtxFields has more fields than BoatEthRawtxFields. To allow
//! PlatONE to re-use Ethereum APIs that take BoatEthTx as function arguments,
//! <rawtx_fields> MUST be the last field.
typedef struct TBoatPlatoneTx
{
    BoatPlatoneWallet *wallet_ptr; //!< Wallet pointer the transaction is combined with
    BoatFieldMax32B tx_hash;       //!< Transaction hash returned from network
    BBOOL is_sync_tx;              //!< True to perform a synchronous transaction (wait for getting mined), False for asynchronous transaction

    // rawtx_fields MUST be the last field
    BoatPlatoneRawtxFields rawtx_fields;      //!< RAW transaction fields
}BoatPlatoneTx;

typedef struct T_wbe3_nodeInfo
{
    char * IP;
    BUINT32 rpcPort;
    
}wbe3_nodeInfo;

typedef struct T_nodesResult
{
    BUINT8 num;
    wbe3_nodeInfo *nodeInfo;
}nodesResult;


#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Initialize PlatONE Transaction
 *
 * @details
 *   This function initialize an PlatONE transaction.
 *   
 * @param[in] wallet_ptr
 *   The wallet pointer that this transaction is combined with.
 *
 * @param[in] tx_ptr
 *   Pointer a transaction object.
 *
 * @param[in] is_sync_tx
 *   For a stateful transaction, specify BOAT_TRUE to wait until the transaction is mined.
 *   Specifiy BOAT_FALSE to allow multiple transactions to be sent continuously in a short time.
 *   For a state-less contract call, this option is ignored.
 *
 * @param[in] gasprice_str
 *   A HEX string representing the gas price (unit: wei) to be used in the transaction.
 *   Set <gasprice> = NULL to obtain gas price from network.
 *   BoatPlatonTxSetGasPrice() can later be called to modify the gas price at any
 *   time before the transaction is executed.
 *
 * @param[in] gaslimit_str
 *   A HEX string representing the gas limit to be used in the transaction.
 *   BoatPlatonTxSetGasLimit() can later be called to modify the gas limit at any
 *   time before the transaction is executed.
 *
 * @param[in] recipient_str
 *   A HEX string representing the recipient address, in HEX format like
 *   "0x19c91A4649654265823512a457D2c16981bB64F5".
 *   BoatPlatonTxSetRecipient() can later be called to modify the recipient at any
 *   time before the transaction is executed.
 *
 * @param[in] txtype
 *   Transaction type defined by BoatPlatonTxtype.
 *
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns BOAT_ERROR.
 *
 * @see BoatEthTxInit()
 ******************************************************************************/
BOAT_RESULT BoatPlatoneTxInit(BoatPlatoneWallet *wallet_ptr,
							  BoatPlatoneTx *tx_ptr,
							  BBOOL is_sync_tx,
							  BCHAR *gasprice_str,
							  BCHAR *gaslimit_str,
							  BCHAR *recipient_str,
							  BoatPlatoneTxtype txtype);


/*!****************************************************************************
 * @brief Set PlatONE Transaction Parameter: Transaction Type
 *
 * @details
 *   This function sets the type of transaction
 *
 * @param[in] tx_ptr
 *   The nonce obtained from network is set in BoatPlatoneTx structure.
 *
 * @param[in] txtype
 *   The transaction type.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.      
 ******************************************************************************/
BOAT_RESULT BoatPlatoneTxSetTxtype(BoatPlatoneTx *tx_ptr, BoatPlatoneTxtype txtype);


/*!****************************************************************************
 * @brief Call a state-less contract function
 *
 * @details
 *   This function calls contract function that doesn't change the state of the
 *   contract. "state" is the "global variable" used in a contract.
 *   \n This function invokes the eth_call RPC method. eth_call method requests the
 *   blockchain node to execute the function without affecting the block chain.
 *   The execution runs only on the requested node thus it can return immediately
 *   after the execution. This function synchronously return the return value
 *   of the eth_call method, which is the return value of the contract function.
 *   \n To call contract function that may change the state, use BoatPlatonTxSend()
 *   instead.
 *   \n If call a contract function that may change the state with
 *   BoatPlatoneCallContractFunc(), the function will be executed and return a value,
 *   but none of the states will change.
 *
 * @param[in] tx_ptr
 *   Transaction pointer
 *
 * @param[in] rlp_param_ptr
 *   A byte stream containing the parameters to pass to the function.\n
 *   The stream is encoded in RLP format including the function name followed by
 *   all parameters.
 *
 * @param[in] rlp_param_len
 *   Length of <rlp_param_ptr> in byte.
 *
 * @return
 *   This function returns a HEX string representing the return value of the
 *   called contract function.\n
 *   If any error occurs, it returns NULL.
 *
 * @see BoatPlatoneTxSend()
 ******************************************************************************/
BCHAR *BoatPlatoneCallContractFunc(BoatPlatoneTx *tx_ptr, BUINT8 *rlp_param_ptr, BUINT32 rlp_param_len);

/*!****************************************************************************
 * @brief Construct a raw PlatONE transaction synchronously.
 *
 * @details
 *   This function is similar to PlatoneSendRawtx except that it waits for the
 *   transaction being mined.
 *	 
 * @param[in] tx_ptr
 *   A pointer to the context of the transaction.
 *
 * @return
 *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
 *   of the error codes.
 *	 
 * @see PlatoneSendRawtx()
 ******************************************************************************/
BOAT_RESULT BoatPlatoneSendRawtxWithReceipt(BOAT_INOUT BoatPlatoneTx *tx_ptr);

/*!****************************************************************************
 * @brief Sign and send a transaction. Also call a stateful contract function.
 *
 * @details
 *   This function sign and set a transaction.
 *   \n BoatWalletSetXXX() and BoatTxSetXXX() functions must be properly called
 *   before call this function.
 *   \n A transaction whose recipient may be an EOA address or a contract address.
 *   In latter case it's usually a contract function call.
 *   \n This function invokes the eth_sendRawTransaction RPC method.
 *   eth_sendRawTransaction method only applies the transaction and returns a
 *   transaction hash. The transaction is not verified (got mined) until the
 *   nodes in the network get into consensus about the transaction. This
 *   function will invoke eth_getTransactionReceipt method to wait for the
 *   transaction being mined or timeout.
 *   \n If the transaction is a contract function call, the caller cannot get its
 *   return value because the transaction is asynchronously executed. It's a
 *   good practice to save the return value in a state variable and use
 *   BoatPlatoneCallContractFunc() to call a "read" contract function that could read
 *   and return the state variable.
 *
 * @note
 *   Any contract function that may change the state of the contract shall
 *   be called in a transaction way. "state" is the "global variable" used in a
 *   contract.
 *   \n Any contract function that doesn't change the state of the contract can
 *   be called either in a transaction way or by BoatPlatoneCallContractFunc(), which
 *   invokes the eth_call RPC method. However the former will consume gas and
 *   latter doesn't consume gas.
 *
 * @param[in] tx_ptr
 *   Pointer to the transaction structure.
 *   
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.
 *
 * @see BoatPlatoneTxSend()
 ******************************************************************************/
BOAT_RESULT BoatPlatoneTxSend(BoatPlatoneTx *tx_ptr);


/*!****************************************************************************
 * @brief Transfer PlatONE value to spceified recipient
 *
 * @details
 *   This function transfer PlatONE value from the wallet's owner account to the
 *   specified recipient account.
 *   \n Before calling this function, all necessary wallet fields such as private key,
 *   node URL and etc. must be set correctly.
 *
 * @param[in] tx_ptr
 *   Transaction pointer.
 *
 * @param[in] value_hex_str
 *   A string representing the value (Unit: wei) to transfer, in HEX format like
 *   "0x89AB3C".\n
 *   Note that decimal value is not accepted. If a decimal value such as "1234"
 *   is specified, it's treated as "0x1234".
 *
 * @return
 *   This function returns BOAT_SUCCESS if transfer is successful.\n
 *   Otherwise it returns one of the error codes.
 ******************************************************************************/
BOAT_RESULT BoatPlatoneTransfer(BoatPlatoneTx *tx_ptr, BCHAR * value_hex_str);


// Ethereum APIs compatible for PlatONE

/*!****************************************************************************
 * @brief Initialize Boat PlatONE Wallet
 * @see BoatEthWalletInit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BoatPlatoneWallet *BoatPlatoneWalletInit(const BoatPlatoneWalletConfig *config_ptr, BUINT32 config_size)
{
    return BoatEthWalletInit((const BoatEthWalletConfig *)config_ptr, config_size);
}


/*!****************************************************************************
 * @brief De-initialize Boat PlatONE Wallet
 * @see BoatEthWalletDeInit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE void BoatPlatoneWalletDeInit(BoatPlatoneWallet *wallet_ptr)
{
    BoatEthWalletDeInit((BoatEthWallet *)wallet_ptr);
}


/*!****************************************************************************
 * @brief Set Node Url
 * @see BoatEthWalletSetNodeUrl()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneWalletSetNodeUrl(BoatPlatoneWallet *wallet_ptr, const BCHAR *node_url_ptr)
{
    return BoatEthWalletSetNodeUrl((BoatEthWallet *)wallet_ptr, node_url_ptr);
}


/*!****************************************************************************
 * @brief Set EIP155
 * @see BoatEthWalletSetEIP155Comp()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneWalletSetEIP155Comp(BoatPlatoneWallet *wallet_ptr, BBOOL eip155_compatibility)
{
    return BoatEthWalletSetEIP155Comp((BoatEthWallet *)wallet_ptr,eip155_compatibility);
}


/*!****************************************************************************
 * @brief Set ChainId
 * @see BoatEthWalletSetChainId()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneWalletSetChainId(BoatPlatoneWallet *wallet_ptr, BUINT32 chain_id)
{
    return BoatEthWalletSetChainId((BoatEthWallet *)wallet_ptr,chain_id);
}


/*!****************************************************************************
 * @brief Get Balance
 * @see BoatEthWalletGetBalance()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BCHAR *BoatPlatoneWalletGetBalance(BoatPlatoneWallet *wallet_ptr, BCHAR *alt_address_str)
{
    return BoatEthWalletGetBalance((BoatEthWallet *)wallet_ptr, alt_address_str);
}


/*!****************************************************************************
 * @brief Set Nonce
 * @see BoatEthTxSetNonce()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetNonce(BoatPlatoneTx *tx_ptr, BUINT64 nonce)
{
    return BoatEthTxSetNonce((BoatEthTx *)tx_ptr, nonce);
}


/*!****************************************************************************
 * @brief Set GasPrice
 * @see BoatEthTxSetGasPrice()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetGasPrice(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *gas_price_ptr)
{
    return BoatEthTxSetGasPrice((BoatEthTx *)tx_ptr, gas_price_ptr);
}


/*!****************************************************************************
 * @brief Set GasLimit
 * @see BoatEthTxSetGasLimit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetGasLimit(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
{
    return BoatEthTxSetGasLimit((BoatEthTx *)tx_ptr, gas_limit_ptr);
}


/*!****************************************************************************
 * @brief Set Recipient
 * @see BoatEthTxSetRecipient()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetRecipient(BoatPlatoneTx *tx_ptr, BUINT8 address[BOAT_PLATONE_ADDRESS_SIZE])
{
    return BoatEthTxSetRecipient((BoatEthTx *)tx_ptr,address);
}


/*!****************************************************************************
 * @brief Set Value
 * @see BoatEthTxSetValue()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetValue(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *value_ptr)
{
    return BoatEthTxSetValue((BoatEthTx *)tx_ptr,value_ptr);
}


/*!****************************************************************************
 * @brief Set Data
 * @see BoatEthTxSetData()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetData(BoatPlatoneTx *tx_ptr, BoatFieldVariable *data_ptr)
{
    return BoatEthTxSetData((BoatEthTx *)tx_ptr,data_ptr);
}


/*!****************************************************************************
 * @brief Wait for a transaction being mined.
 * @see BoatEthGetTransactionReceipt()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneGetTransactionReceipt(BoatPlatoneTx *tx_ptr)
{
    return BoatEthGetTransactionReceipt((BoatEthTx *)tx_ptr);
}

/*!****************************************************************************
 * @brief Prase RPC method RESPONSE.
 * @see web3_parse_json_result()
 ******************************************************************************/
BOAT_RESULT BoatPlatonePraseRpcResponseStringResult(const BCHAR *json_string, BoatFieldVariable *result_out);

/*!****************************************************************************
 * @brief Prase RPC method RESPONSE.
 * @see web3_parse_json_result()
 ******************************************************************************/
BOAT_RESULT BoatPlatonePraseRpcResponseResult(const BCHAR *json_string, 
                                              const BCHAR *child_name, 
                                              BoatFieldVariable *result_out);

/*!****************************************************************************
 * @brief Get Nodes Info e.g. IP/Port.
 * @see BoatPlatoneGetNodesInfo()
 ******************************************************************************/
BCHAR * BoatPlatoneGetNodesInfo(BoatPlatoneTx *tx_ptr,nodesResult *result_out);

/*! @}*/

void nodeResFree(nodesResult *result_out);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
