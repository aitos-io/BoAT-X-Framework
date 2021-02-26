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

/*!@brief BoAT IoT SDK interface header file for PlatON

@file
api_platon.h is header file for BoAT IoT SDK PlatON's interface.
*/

#ifndef __API_PLATON_H__
#define __API_PLATON_H__

#include "boatiotsdk.h"
#include "api_ethereum.h"

/*! @defgroup platon-api boat platon-API 
 * @{
 */

#define BOAT_PLATON_NONCE_AUTO                   BOAT_ETH_NONCE_AUTO
#define BOAT_PLATON_ADDRESS_SIZE                 BOAT_ETH_ADDRESS_SIZE

typedef enum
{
    BOAT_PLATON_TX_TYPE_NORMAL_TRANSFER = 0x00,
    BOAT_PLATON_TX_TYPE_CONTRACT = 0x02,
    BOAT_PLATON_TX_TYPE_CONTRACT_NULL_TERMED_STR = 0x09,
}BoatPlatonTxtype;

typedef BoatEthAccountInfo  BoatPlatonAccountInfo;
typedef BoatEthNetworkInfo  BoatPlatonNetworkInfo;
typedef BoatEthWallet       BoatPlatonWallet;
typedef BoatEthWalletConfig BoatPlatonWalletConfig;

//!@brief RAW PlatON transaction fields

//! The only difference from Ethereum is appending a txtype field at the end of the
//! struct. Thus a BoatPlatonRawtxFields pointer can be transfered to any function
//! that accepts a BoatEthRawtxFields argument. It behaves as a C implementation of
//! class inheritance, i.e. a pointer of an inherited class could be assigned to a
//! pointer of a base class.
typedef struct TBoatPlatonRawtxFields
{
    // Following fields are inherited from Ethereum.
    // DO NOT modify these fields unless all of Ethereum and all other protocols
    // inherited from Ethereum are modified synchronously.
    BoatFieldMax32B nonce;        //!< nonce, uint256 in bigendian, equal to the transaction count of the sender's account address
    BoatFieldMax32B gasprice;     //!< gasprice in wei, uint256 in bigendian
    BoatFieldMax32B gaslimit;     //!< gaslimit, uint256 in bigendian
    BUINT8 recipient[BOAT_ETH_ADDRESS_SIZE];          //!< recipient's address, 160 bits
    BoatFieldMax32B value;        //!< value to transfer, uint256 in bigendian
    BoatFieldVariable data;       //!< data to transfer, unformatted stream
    BoatFieldMax4B v;             //!< chain id or recovery identifier, @see RawtxPerform()
    BoatEthTxFieldSig sig;        //!< ECDSA signature, including r and s parts
    
    // PlatON specific fields are appended here.
    BoatPlatonTxtype txtype;      //!< Transaction Type

}BoatPlatonRawtxFields;

//!@brief PlatON Transaction

//! The only difference between PlatON transaction and Ethereum transaction is
//! BoatPlatonRawtxFields has more fields than BoatEthRawtxFields. To allow
//! PlatON to re-use Ethereum APIs that take BoatEthTx as function arguments,
//! <rawtx_fields> MUST be the last field.
typedef struct TBoatPlatonTx
{
    BoatPlatonWallet *wallet_ptr; //!< Wallet pointer the transaction is combined with
    BoatFieldMax32B tx_hash;      //!< Transaction hash returned from network
    BBOOL is_sync_tx;             //!< True to perform a synchronous transaction (wait for getting mined), False for asynchronous transaction

    // rawtx_fields MUST be the last field
    BoatPlatonRawtxFields rawtx_fields;      //!< RAW transaction fields
}BoatPlatonTx;

#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Initialize PlatON Transaction
 *
 * @details
 *   This function initialize an PlatON transaction.
 *   
 * @param[in] wallet_ptr
 *   The wallet pointer that this transaction is combined with.
 *
 * @param[in] tx_ptr
 *   Pointer a transaction object.
 *
 * @param[in] is_sync_tx
 *   For a stateful transaction, specify BOAT_TRUE to wait until the transaction is mined.\n
 *   Specifiy BOAT_FALSE to allow multiple transactions to be sent continuously in a short time.\n
 *   For a state-less contract call, this option is ignored.
 *
 * @param[in] gasprice_str
 *   A HEX string representing the gas price (unit: wei) to be used in the transaction.\n
 *   Set <gasprice> = NULL to obtain gas price from network.\n
 *   BoatPlatonTxSetGasPrice() can later be called to modify the gas price at any\n
 *   time before the transaction is executed.
 *
 * @param[in] gaslimit_str
 *   A HEX string representing the gas limit to be used in the transaction.\n
 *   BoatPlatonTxSetGasLimit() can later be called to modify the gas limit at any\n
 *   time before the transaction is executed.
 *
 * @param[in] recipient_str
 *   A HEX string representing the recipient address, in HEX format like\n
 *   "0x19c91A4649654265823512a457D2c16981bB64F5".\n
 *   BoatPlatonTxSetRecipient() can later be called to modify the recipient at any\n
 *   time before the transaction is executed.
 *
 * @param[in] txtype
 *   Transaction type defined by BoatPlatonTxtype.
 *
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns BOAT_ERROR.
 *
 * @see BoatEthTxInit() BoatPlatonTxSetTxtype()
 ******************************************************************************/
BOAT_RESULT BoatPlatonTxInit(BoatPlatonWallet *wallet_ptr,
                          BoatPlatonTx *tx_ptr,
                          BBOOL is_sync_tx,
                          BCHAR * gasprice_str,
                          BCHAR * gaslimit_str,
                          BCHAR *recipient_str,
                          BoatPlatonTxtype txtype);


/*!****************************************************************************
 * @brief Set Platon Transaction Parameter: Transaction Type
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
BOAT_RESULT BoatPlatonTxSetTxtype(BoatPlatonTx *tx_ptr, BoatPlatonTxtype txtype);


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
 *   BoatPlatonCallContractFunc() to call a "read" contract function that could read
 *   and return the state variable.
 *
 * @note  
 *   Any contract function that may change the state of the contract shall
 *   be called in a transaction way. "state" is the "global variable" used in a
 *   contract.
 *   \n Any contract function that doesn't change the state of the contract can
 *   be called either in a transaction way or by BoatPlatonCallContractFunc(), which
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
 * @see BoatPlatonCallContractFunc()
 ******************************************************************************/
BOAT_RESULT BoatPlatonTxSend(BoatPlatonTx *tx_ptr);


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
 *   BoatPlatonCallContractFunc(), the function will be executed and return a value,
 *   but none of the states will change.
 *
 * @param[in] tx_ptr
 *   Transaction pointer
 *
 * @param[in] rlp_param_ptr
 *   A byte stream containing the parameters to pass to the function.\n
 *   The stream is encoded in RLP format including the function name followed by\n
 *   all parameters.
 *
 * @param[in] rlp_param_len
 *   Length of <rlp_param_ptr> in byte.
 *
 * @return
 *   This function returns a HEX string representing the return value of the\n
 *   called contract function.\n
 *   If any error occurs, it returns NULL.
 *
 * @see BoatPlatonTxSend()
 ******************************************************************************/
BCHAR * BoatPlatonCallContractFunc(
                    BoatPlatonTx *tx_ptr,
                    BUINT8 *rlp_param_ptr,
                    BUINT32 rlp_param_len);


/*!*****************************************************************************
 * @brief Transfer PlatON Energon to spceified recipient
 *
 * @details
 *   This function transfer PlatON Energon from the wallet's owner account to the
 *   specified recipient account.
 *   \n Before calling this function, all necessary wallet fields such as private key,
 *   node URL and etc. must be set correctly.
 *
 * @param[in] tx_ptr
 *   Transaction pointer.
 *
 * @param[in] value_hex_str
 *   A string representing the value (Unit: wei) to transfer, in HEX format like\n
 *   "0x89AB3C".\n
 *   Note that decimal value is not accepted. If a decimal value such as "1234"\n
 *   is specified, it's treated as "0x1234".
 *
 * @return
 *   This function returns BOAT_SUCCESS if transfer is successful.\n
 *   Otherwise it returns BOAT_ERROR.       
 ******************************************************************************/
BOAT_RESULT BoatPlatonTransfer(BoatPlatonTx *tx_ptr, BCHAR * value_hex_str);


// Ethereum APIs compatible for PlatON

/*!*****************************************************************************
 * @brief Initialize Boat PlatON Wallet
 * @see BoatEthWalletInit()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BoatPlatonWallet * BoatPlatonWalletInit(const BoatPlatonWalletConfig *config_ptr, BUINT32 config_size)
{
    return BoatEthWalletInit((const BoatEthWalletConfig *) config_ptr, config_size);
}


/*!*****************************************************************************
 * @brief De-initialize Boat PlatON Wallet
 * @see BoatEthWalletDeInit()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE void BoatPlatonWalletDeInit(BoatPlatonWallet *wallet_ptr)
{
    return BoatEthWalletDeInit((BoatEthWallet *) wallet_ptr);
}


/*!*****************************************************************************
 * @brief Set Node Url
 * @see BoatEthWalletSetNodeUrl()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonWalletSetNodeUrl(BoatPlatonWallet *wallet_ptr, const BCHAR *node_url_ptr)
{
    return BoatEthWalletSetNodeUrl((BoatEthWallet *)wallet_ptr, node_url_ptr);
}


/*!*****************************************************************************
 * @brief Set EIP155
 * @see BoatEthWalletSetEIP155Comp()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonWalletSetEIP155Comp(BoatPlatonWallet *wallet_ptr, BBOOL eip155_compatibility)
{
    return BoatEthWalletSetEIP155Comp((BoatEthWallet *)wallet_ptr,eip155_compatibility);
}


/*!*****************************************************************************
 * @brief Set ChainId
 * @see BoatEthWalletSetChainId()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonWalletSetChainId(BoatPlatonWallet *wallet_ptr, BUINT32 chain_id)
{
    return BoatEthWalletSetChainId((BoatEthWallet *)wallet_ptr,chain_id);
}


/*!*****************************************************************************
 * @brief Get Balance
 * @see BoatEthWalletGetBalance()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BCHAR * BoatPlatonWalletGetBalance(BoatPlatonWallet *wallet_ptr, BCHAR *alt_address_str)
{
    return BoatEthWalletGetBalance((BoatEthWallet *)wallet_ptr, alt_address_str);
}


/*!*****************************************************************************
 * @brief Get Nonce
 * @see BoatEthTxSetNonce()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonTxSetNonce(BoatPlatonTx *tx_ptr, BUINT64 nonce)
{
    return BoatEthTxSetNonce((BoatEthTx *)tx_ptr, nonce);
}


/*!*****************************************************************************
 * @brief Set GasPrice
 * @see BoatEthTxSetGasPrice()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonTxSetGasPrice(BoatPlatonTx *tx_ptr, BoatFieldMax32B *gas_price_ptr)
{
    return BoatEthTxSetGasPrice((BoatEthTx *)tx_ptr, gas_price_ptr);
}


/*!*****************************************************************************
 * @brief Set GasLimit
 * @see BoatEthTxSetGasLimit()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonTxSetGasLimit(BoatPlatonTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
{
    return BoatEthTxSetGasLimit((BoatEthTx *)tx_ptr, gas_limit_ptr);
}


/*!*****************************************************************************
 * @brief Set Recipient
 * @see BoatEthTxSetRecipient()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonTxSetRecipient(BoatPlatonTx *tx_ptr, BUINT8 address[BOAT_PLATON_ADDRESS_SIZE])
{
    return BoatEthTxSetRecipient((BoatEthTx *)tx_ptr,address);
}


/*!*****************************************************************************
 * @brief Set Value
 * @see BoatEthTxSetValue()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonTxSetValue(BoatPlatonTx *tx_ptr, BoatFieldMax32B *value_ptr)
{
    return BoatEthTxSetValue((BoatEthTx *)tx_ptr,value_ptr);
}


/*!*****************************************************************************
 * @brief Set Data
 * @see BoatEthTxSetData()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonTxSetData(BoatPlatonTx *tx_ptr, BoatFieldVariable *data_ptr)
{
    return BoatEthTxSetData((BoatEthTx *)tx_ptr,data_ptr);
}


/*!*****************************************************************************
 * @brief Wait for a transaction being mined.
 * @see BoatEthGetTransactionReceipt()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatonGetTransactionReceipt(BoatPlatonTx *tx_ptr)
{
    return BoatEthGetTransactionReceipt((BoatEthTx *) tx_ptr);
}

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
