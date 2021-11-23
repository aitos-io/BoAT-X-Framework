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

/*! @defgroup PlatON-api boat PlatON-API
 * @{
 */


#define BOAT_PLATON_MINE_INTERVAL             2  //!< Mining Interval of the blockchain, in seconds
#define BOAT_PLATON_WAIT_PENDING_TX_TIMEOUT   30 //!< Timeout waiting for a transaction being mined, in seconds

#define BOAT_PLATON_NODE_URL_MAX_LEN          BOAT_ETH_NODE_URL_MAX_LEN
#define BOAT_PLATON_NONCE_AUTO                BOAT_ETH_NONCE_AUTO
#define BOAT_PLATON_ADDRESS_SIZE              BOAT_ETH_ADDRESS_SIZE
#define BOAT_PLATON_BECH32_ADDRESS_SIZE       45

typedef BoatEthAccountInfo  BoatPlatONAccountInfo;
typedef BoatEthNetworkInfo  BoatPlatONNetworkInfo;
typedef BoatEthWallet       BoatPlatONWallet;
typedef BoatEthWalletConfig BoatPlatONWalletConfig;
typedef BoatEthTxFieldSig   BoatPlatONTxFieldSig;

//! The only difference from Ethereum is appending a txtype field at the end of the
//! struct. Thus a BoatPlatONRawtxFields pointer can be transfered to any function
//! that accepts a BoatEthRawtxFields argument. It behaves as a C implementation of
//! class inheritance, i.e. a pointer of an inherited class could be assigned to a
//! pointer of a base class.
typedef struct TBoatPlatONRawtxFields
{
    // Following fields are inherited from Ethereum.
    // DO NOT modify these fields unless all of Ethereum and all other protocols
    // inherited from Ethereum are modified synchronously.
    BoatFieldMax32B nonce;        //!< nonce, uint256 in bigendian, equal to the transaction count of the sender's account address
    BoatFieldMax32B gasprice;     //!< gasprice in wei, uint256 in bigendian
    BoatFieldMax32B gaslimit;     //!< gaslimit, uint256 in bigendian
    BUINT8 recipient[BOAT_PLATON_ADDRESS_SIZE]; //!< recipient's address, 160 bits
    BoatFieldMax32B value;        //!< value to transfer, uint256 in bigendian
    BoatFieldVariable data;       //!< data to transfer, unformatted stream
    BoatFieldMax4B v;             //!< chain id or recovery identifier, @see RawtxPerform()
    BoatEthTxFieldSig sig;        //!< ECDSA signature, including r and s parts
    
    // PlatON specific fields are appended here.
    BCHAR recipientbech32[BOAT_PLATON_BECH32_ADDRESS_SIZE]; //!< recipient's address in PlatON's bech32 format, string
}BoatPlatONRawtxFields;

//! The only difference between PlatON transaction and Ethereum transaction is
//! PlatON has an additional address format. To allow PlatON to re-use Ethereum APIs that 
//! take BoatEthTx as function arguments, the <address> MUST be the last field.
typedef struct TBoatPlatONTx
{
    BoatPlatONWallet *wallet_ptr; //!< Wallet pointer the transaction is combined with
    BoatFieldMax32B tx_hash;       //!< Transaction hash returned from network
    BBOOL is_sync_tx;              //!< True to perform a synchronous transaction (wait for getting mined), False for asynchronous transaction

    // rawtx_fields MUST be the last field
    BoatPlatONRawtxFields rawtx_fields;      //!< RAW transaction fields
    BCHAR address[BOAT_PLATON_BECH32_ADDRESS_SIZE]; //!< Wallet's address in PlatON's bech32 format, string
}BoatPlatONTx;

#ifdef __cplusplus
extern "C" {
#endif

/*!****************************************************************************
 * @brief Get Balance of the wallet account
 *
 * @param[in] hrp_str
 *   for PlatON, it is "lat", for Alaya, it is "atp". string
 *   
 * @return
 *   This function returns a HEX string representing the balance (Unit: von,\n
 *   ) of the account.\n
 *   If any error occurs, it returns NULL.
 ******************************************************************************/
BCHAR *BoatPlatONWalletGetBalance(BoatPlatONWallet *wallet_ptr, const BCHAR *hrp_str);


/*!****************************************************************************
 * @brief Initialize a transaction
 *
 * @param[in] hrp_str
 *   for PlatON, it is "lat", for Alaya, it is "atp". string
 * 
 * @see BoatEthTxInit()
 ******************************************************************************/
BOAT_RESULT BoatPlatONTxInit(BoatPlatONWallet *wallet_ptr,
                             BoatPlatONTx *tx_ptr,
                             const BBOOL is_sync_tx,
                             const BCHAR *gasprice_str,
                             const BCHAR *gaslimit_str,
                             const BCHAR *recipient_str,
                             const BCHAR *hrp_str);


/*!****************************************************************************
 * @brief Set Nonce
 * @see BoatEthTxSetNonce()
 ******************************************************************************/
BOAT_RESULT BoatPlatONTxSetNonce(BoatPlatONTx *tx_ptr, BUINT64 nonce);

/*!****************************************************************************
 * @brief Construct a raw PlatON transaction synchronously.
 *
 * @details
 *   This function is similar to PlatONSendRawtx except that it waits for the
 *   transaction being mined.
 *   
 * @param[in] tx_ptr
 *   A pointer to the context of the transaction.
 *
 * @return
 *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
 *   of the error codes.
 *	
 * @see PlatONSendRawtx() BoatPlatONGetTransactionReceipt() 
*******************************************************************************/
BOAT_RESULT BoatPlatONSendRawtxWithReceipt(BOAT_INOUT BoatPlatONTx *tx_ptr);

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
 *   BoatPlatONCallContractFunc() to call a "read" contract function that could read
 *   and return the state variable.
 *
 * @note
 *   Any contract function that may change the state of the contract shall
 *   be called in a transaction way. "state" is the "global variable" used in a
 *   contract.
 *   \n Any contract function that doesn't change the state of the contract can
 *   be called either in a transaction way or by BoatPlatONCallContractFunc(), which
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
 * @see BoatEthTxSend()
 ******************************************************************************/
BOAT_RESULT BoatPlatONTxSend(BoatPlatONTx *tx_ptr);

/*!****************************************************************************
 * @brief Transfer PlatON value to spceified recipient
 *
 * @see BoatEthTxSend()
 ******************************************************************************/
BOAT_RESULT BoatPlatONTransfer(BoatPlatONTx *tx_ptr, BCHAR *value_hex_str);

/*!****************************************************************************
 * @brief Call a state-less contract function
 * @see BoatEthCallContractFunc()
 ******************************************************************************/
BCHAR *BoatPlatONCallContractFunc(BoatPlatONTx *tx_ptr, 
                                  BCHAR *func_proto_str, 
								  BUINT8 *rlp_param_ptr, 
                                  BUINT32 rlp_param_len);


/*!****************************************************************************
 * @brief Wait for a transaction being mined.
 * @see BoatEthGetTransactionReceipt()
 ******************************************************************************/
BOAT_RESULT BoatPlatONGetTransactionReceipt(BoatPlatONTx *tx_ptr);

/*!****************************************************************************
 * @brief query current group block number.
 ******************************************************************************/
BCHAR *BoatPlatONGetBlockNumber(BoatPlatONTx *tx_ptr);

/*!****************************************************************************
 * @brief Set Recipient
 * @see BoatEthTxSetRecipient()
 ******************************************************************************/
BOAT_RESULT BoatPlatONTxSetRecipient(BoatPlatONTx *tx_ptr, BUINT8 address[BOAT_PLATON_ADDRESS_SIZE]);

// Ethereum APIs compatible for PlatON

/*!****************************************************************************
 * @brief Initialize Boat PlatON Wallet
 * @see BoatEthWalletInit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BoatPlatONWallet *BoatPlatONWalletInit(const BoatPlatONWalletConfig *config_ptr, BUINT32 config_size)
{
    return BoatEthWalletInit((const BoatEthWalletConfig *)config_ptr, config_size);
}


/*!****************************************************************************
 * @brief De-initialize Boat PlatON Wallet
 * @see BoatEthWalletDeInit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE void BoatPlatONWalletDeInit(BoatPlatONWallet *wallet_ptr)
{
    return BoatEthWalletDeInit((BoatEthWallet *)wallet_ptr);
}


/*!****************************************************************************
 * @brief Set Node Url
 * @see BoatEthWalletSetNodeUrl()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatONWalletSetNodeUrl(BoatPlatONWallet *wallet_ptr, const BCHAR *node_url_ptr)
{
    return BoatEthWalletSetNodeUrl((BoatEthWallet *)wallet_ptr, node_url_ptr);
}


/*!****************************************************************************
 * @brief Set ChainId
 * @see BoatEthWalletSetChainId()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatONWalletSetChainId(BoatPlatONWallet *wallet_ptr, BUINT32 chain_id)
{
    return BoatEthWalletSetChainId((BoatEthWallet *)wallet_ptr,chain_id);
}


/*!****************************************************************************
 * @brief Set GasPrice
 * @see BoatEthTxSetGasPrice()
 ******************************************************************************/
BOAT_RESULT BoatPlatONTxSetGasPrice(BoatPlatONTx *tx_ptr, BoatFieldMax32B *gas_price_ptr);


/*!****************************************************************************
 * @brief Set GasLimit
 * @see BoatEthTxSetGasLimit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatONTxSetGasLimit(BoatPlatONTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
{
    return BoatEthTxSetGasLimit((BoatEthTx *)tx_ptr, gas_limit_ptr);
}


/*!*****************************************************************************
 * @brief Set Value
 * @see BoatEthTxSetValue()
 *******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatONTxSetValue(BoatPlatONTx *tx_ptr, BoatFieldMax32B *value_ptr)
{
    return BoatEthTxSetValue((BoatEthTx *)tx_ptr, value_ptr);
}


/*!****************************************************************************
 * @brief Set Data
 * @see BoatEthTxSetData()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatONTxSetData(BoatPlatONTx *tx_ptr, BoatFieldVariable *data_ptr)
{
    return BoatEthTxSetData((BoatEthTx *)tx_ptr, data_ptr);
}

/*!****************************************************************************
 * @brief Prase RPC method RESPONSE.
 * @see web3_parse_json_result()
 ******************************************************************************/
BOAT_RESULT BoatPlatONPraseRpcResponseStringResult(const BCHAR *json_string, BoatFieldVariable *result_out);


/*!****************************************************************************
 * @brief Prase RPC method RESPONSE.
 * @see web3_parse_json_result()
 ******************************************************************************/
BOAT_RESULT BoatPlatONPraseRpcResponseResult(const BCHAR *json_string, 
                                             const BCHAR *child_name, 
                                             BoatFieldVariable *result_out);

/*!****************************************************************************
 * @brief Set EIP155
 * @see BoatEthWalletSetEIP155Comp()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatONWalletSetEIP155Comp(BoatPlatONWallet *wallet_ptr, BBOOL eip155_compatibility)
{
    return BoatEthWalletSetEIP155Comp((BoatEthWallet *)wallet_ptr,eip155_compatibility);
}


/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
