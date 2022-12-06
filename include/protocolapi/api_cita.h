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

/*!@brief BoAT IoT SDK interface header file for cita

@file
api_cita.h is header file for BoAT IoT SDK cita's interface.
*/

#ifndef __API_CITA_H__
#define __API_CITA_H__

#include "boatiotsdk.h"

/*! @defgroup cita-api boat cita-API
 * @{
 */

#define BOAT_CITA_MINE_INTERVAL                  3  //!< Mining Interval of the blockchain, in seconds
#define BOAT_CITA_WAIT_PENDING_TX_TIMEOUT        30 //!< Timeout waiting for a transaction being mined, in seconds

#define BOAT_CITA_NODE_URL_MAX_LEN               127 //!< Maxmum length for node's URL

#define BOAT_CITA_NONCE_AUTO                     0xFFFFFFFFFFFFFFFF
#define BOAT_CITA_ADDRESS_SIZE                   20
#define BOAT_CITA_VALUE_SIZE                     32

//!@brief Account information
//! An account's only identifier is its private key. An address is calculated
//! from the public key and the public key is calculated from the private key.
typedef struct TBoatCitaAccountInfo
{
    BoatKeypairPriKeyCtx prikeyCtx;         //!< prikey context                       
    BUINT8  address[BOAT_CITA_ADDRESS_SIZE];//!< Account address calculated from public key
}BoatCitaAccountInfo;

//!@brief Wallet information

//! Wallet information consists of account and block chain network information.
//! Currently only one account per wallet is supported.
//! NOTE: Members  of account_info and network_info will be stored, DO NOT put pointer member in it
typedef struct TBoatCitaWallet
{
    BoatCitaAccountInfo account_info; //!< Account information
    BoatCitaNetworkData network_info; //!< Network information

    struct TWeb3IntfContext *web3intf_context_ptr;  //!< Web3 Interface Context
}BoatCitaWallet;

//!@brief CitaRAW transaction fields

//! These fields are to be inherited by other cita compatible protocols
//! Make sure any inherited struct only appends fields at the end of this struct.
//! This allow these cita compatible protocols to re-use a lot of cita
//! APIs by converting inherited struct pointer to base struct pointer, if only
//! members in base struct are visited.
typedef struct TBoatCitaRawtxFields
{
    // Following fields are to be inherited by other cita compatible protocols.
    // DO NOT modify these fields unless all inherited structures are modified
    // synchronously.
    //
    // Protocols inherited these fileds include:
    BoatFieldMax32B nonce;        //!< nonce, uint256 in bigendian, equal to the transaction count of the sender's account address
    BUINT64 quota;                  //!< quota
    BUINT64 valid_until_block ;     //!< valid_until_block
    BUINT8 recipient[BOAT_CITA_ADDRESS_SIZE]; //!< recipient's address, 160 bits
    BoatFieldMax32B  value;        //!< value to transfer, uint256 in bigendian
    BoatFieldVariable data;       //!< data to transfer, unformatted stream
    // To allow struct inheritance, other cita compatible protocols can
    // define a BoatXXXRawtxFields struct type with all above fields the very
    // same as cita and append its own fields following them.
    
}BoatCitaRawtxFields;

//!@brief Cita Transaction

//! The <rawtx_fields> may be inherited by other cita compatible protocols
//! Thus <rawtx_fields> MUST be the last member in the struct
//! to allow cita compatible protocols to re-use cita APIs that take
//! BoatCitaTx as function arguments.
typedef struct TBoatCitaTx
{
    BoatCitaWallet *wallet_ptr; //!< Wallet pointer the transaction is combined with
    BoatFieldMax32B tx_hash;   //!< Transaction hash returned from network
    BBOOL is_sync_tx;          //!< True to perform a synchronous transaction (wait for getting mined), False for asynchronous transaction

    // <rawtx_field> MUST be the last member in this struct to allow inheritance
    BoatCitaRawtxFields rawtx_fields;       //!< RAW transaction fields
}BoatCitaTx;


#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Initialize Boat Cita Wallet
 *
 * @details
 *   This function sets up context of Boat Cita Wallet based on given
 *   configuration.
 *   \n DO NOT call this function directly. Instead call BoatWalletCreate() with an Cita
 *   wallet configuration like:
 *   @verbatim
     BoatCitaWalletConfig cita_config = {...};
     BSINT32 wallet_index;
     wallet_index = BoatWalletCreate(BOAT_PROTOCOL_CITA,
                                     &cita_config,
                                     sizeof(BoatCitaWalletConfig)
                                    );
     @endverbatim
 *   \n BoatCitaWalletInit() MUST be called before any use of Boat Cita Wallet.
 *   \n BoatCitaWalletDeInit() MUST be called after use of Boat Cita Wallet.
 *   
 * @param[in] config_ptr
 *   Pointer to Cita wallet configuration.
 *
 * @param[in] config_size
 *   Size (in byte) of Cita wallet configuration.
 *
 * @return
 *   This function returns instance pointer of BoatCitaWallet if initialization is successful.\n
 *   Otherwise it returns NULL.
 *		
 * @see BoatCitaWalletDeInit() BoatWalletCreate()
 ******************************************************************************/
BoatCitaWallet *BoatCitaWalletInit(BUINT8 walletIndex, BUINT8 networkIndex);


/*!****************************************************************************
 * @brief De-initialize Boat cita Wallet
 *
 * @details
 *   This function de-initialize context of Boat cita Wallet.
 *   \n BoatCitaWalletInit() MUST be called before any use of Boat cita Wallet.
 *   \n BoatCItaWalletDeInit() MUST be called after use of Boat cita Wallet.
 *
 * @param[in] wallet_ptr
 *   Wallet context pointer
 *
 * @return 
 *   This function doesn't return any thing.
 *
 * @see BoatCitaWalletInit()
 ******************************************************************************/
void BoatCitaWalletDeInit(BoatCitaWallet *wallet_ptr);

/*!*****************************************************************************
@brief Parse RPC method RESPONSE

   This function Parse "result" segment.
   "result" object must be cjson_string, this function will returns contents of "result" . 

@param[in] json_string
	 The json to be parsed.

@param[out] result_out
	 The buffer to store parse result.
	 Caller can allocate memory for this param, or can initial it with {NULL, 0},
	 this function will expand the memory if it too small to store parse result.
	 
@return
    This function returns BOAT_SUCCESS if parse successed. Otherwise
    it returns an error code.
*******************************************************************************/
BOAT_RESULT BoatCitaParseRpcResponseStringResult(const BCHAR *json_string, 
										        BoatFieldVariable *result_out);

/*!*****************************************************************************
@brief Parse RPC method RESPONSE

   This function Parse "result" segment.
   If "result" object must be json object, the parameter named "child_name" will actived,
   if "child_name" object is string, this function will returns contents of "child_name"; 
   if "child_name" object is other types, his function will prompt "un-implemention yet".
   For other types of "result" this function is not support yet.

@param[in] json_string
	 The json to be parsed.

@param[in] child_name
	 This parameter is mandatory.

@param[out] result_out
	 The buffer to store parse result.
	 Caller can allocate memory for this param, or can initial it with {NULL, 0},
	 this function will expand the memory if it too small to store parse result.
	 
@return
    This function returns BOAT_SUCCESS if parse successed. Otherwise
    it returns an error code.
*******************************************************************************/
BOAT_RESULT BoatCitaParseRpcResponseResult(const BCHAR *json_string, 
										  const BCHAR *child_name, 
										  BoatFieldVariable *result_out);

                                           
/*!****************************************************************************
 * @brief Initialize cita Transaction
 *
 * @details
 *   This function initialize an cita transaction.

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
 * @param[in] recipient_str
 *   A HEX string representing the recipient address, in HEX format like\n
 *   "0x19c91A4649654265823512a457D2c16981bB64F5".\n
 *   BoatEthTxSetRecipient() can later be called to modify the recipient at any\n
 *   time before the transaction is executed.
 * 
 * @param[in] quota
 *   BUINT64 representing the gas price (unit: wei) to be used in the transaction.\n
 *   quota is used as gas
 *   time before the transaction is executed.
 *	
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes.
 *
 * @see
*******************************************************************************/

BOAT_RESULT BoatCitaTxInit(BoatCitaWallet *wallet_ptr,
                                BoatCitaTx *tx_ptr,
                                BBOOL is_sync_tx,
                                BCHAR *recipient_str,
                                BUINT64 quota);


/*!****************************************************************************
 * @brief Set Transaction Parameter: Transaction Nonce
 *
 * @details
 *   This function initialize an cita transaction.
 *   \n This function sets the nonce to the transaction count of the account
 *   obtained from network.
 *   \n This function can be called after BoatcitaTxInit() has been called.
 *
 * @param[in] tx_ptr
 *   The pointer to the transaction.
 *
 * @param[in] nonce
 *   The nonce to use in the transaction.\n
 *   If BOAT_CITA_NONCE_AUTO (0xFFFFFFFFFFFFFFFF) is specified, the nonce is\n
 *   obtained from network.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.
 * @see BoatCitaTxInit()       
 ******************************************************************************/
BOAT_RESULT BoatCitaTxSetNonce(BoatCitaTx *tx_ptr, BUINT64 nonce);

/*!****************************************************************************
 * @brief Set Transaction Parameter: Recipient Address
 *
 * @details
 *   This function sets the address of the transaction recipient.
 *
 * @param[in] tx_ptr
 *   Pointer to the transaction structure.   
 *
 * @param[in] address
 *   The address of the recipient
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.     
 ******************************************************************************/
BOAT_RESULT BoatCitaTxSetRecipient(BoatCitaTx *tx_ptr, BUINT8 address[BOAT_CITA_ADDRESS_SIZE]);

/*!****************************************************************************
 * @brief Construct a raw cita transaction synchronously.
 *
 * @details
 *   This function is similar to CitaSendRawtx except that it waits for the
 *   transaction being mined.
 *   
 * @param[in] tx_ptr
 *   A pointer to the context of the transaction.
 *
 * @return
 *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
 *   of the error codes.
 *	
 * @see CitaSendRawtx() BoatCitaGetTransactionReceipt() 
*******************************************************************************/
BOAT_RESULT BoatCitaSendRawtxWithReceipt(BOAT_INOUT BoatCitaTx *tx_ptr);

/*!****************************************************************************
 * @brief Set Transaction Parameter: Value
 *
 * @details
 *   This function sets the value of the transaction.
 * 
 * @param[in] tx_ptr
 *   Pointer to the transaction structure.    
 *
 * @param[in] value_ptr
 *   The value of the transaction.\n
 *   If <value_ptr> is NULL, it's treated as no value being transfered.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.      
 ******************************************************************************/
BOAT_RESULT BoatCitaTxSetValue(BoatCitaTx *tx_ptr, BoatFieldMax32B *value_ptr);


/*!****************************************************************************
 * @brief Set Transaction Parameter: Data
 *
 * @details
 *   This function sets the data of the transaction.
 *
 * @param[in] tx_ptr
 *   Pointer to the transaction structure.     
 *
 * @param[in] data_ptr
 *   The data of the transaction. Note that data_ptr->field_ptr itself is only\n
 *   a pointer without any associated storage by default. The caller must\n
 *   allocate appropriate memory for it.\n
 *   If <data_ptr> is NULL, it's treated as no data being transfered.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.       
 ******************************************************************************/
BOAT_RESULT BoatCitaTxSetData(BoatCitaTx *tx_ptr, BoatFieldVariable *data_ptr);

/*!****************************************************************************
 * @brief Sign and send a transaction. Also call a stateful contract function.
 *
 * @details
 *   This function sign and set a transaction.
 *   \n BoatWalletSetXXX() and BoatTxSetXXX() functions must be properly called
 *   before call this function.
 *   \n A transaction whose recipient may be an EOA address or a contract address.
 *   In latter case it's usually a contract function call.
 *   \n This function invokes the cita_sendRawTransaction RPC method.
 *   cita_sendRawTransaction method only applies the transaction and returns a
 *   transaction hash. The transaction is not verified (got mined) until the
 *   nodes in the network get into consensus about the transaction. This
 *   function will invoke cita_getTransactionReceipt method to wait for the
 *   transaction being mined or timeout.
 *   \n If the transaction is a contract function call, the caller cannot get its
 *   return value because the transaction is asynchronously executed. It's a
 *   good practice to save the return value in a state variable and use
 *   BoatCitaCallContractFunc() to call a "read" contract function that could read
 *   and return the state variable.
 *
 *   @note
 *   Any contract function that may change the state of the contract shall
 *   be called in a transaction way. "state" is the "global variable" used in a
 *   contract.
 *   \n Any contract function that doesn't change the state of the contract can
 *   be called either in a transaction way or by BoatCitaCallContractFunc(), which
 *   invokes the call RPC method. However the former will consume gas and
 *   latter doesn't consume gas.
 *
 * @param[in] tx_ptr
 *   Pointer to the transaction structure.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.
 *
 * @see BoatCitaCallContractFunc()    
 ******************************************************************************/
BOAT_RESULT BoatCitaTxSend(BoatCitaTx *tx_ptr);


/*!****************************************************************************
 * @brief Call a state-less contract function
 *
 * @details
 *   This function calls contract function that doesn't change the state of the
 *   contract. "state" is the "global variable" used in a contract.
 *   \n This function invokes the call RPC method. call method requests the
 *   blockchain node to execute the function without affecting the block chain.
 *   The execution runs only on the requested node thus it can return immediately
 *   after the execution. This function synchronously return the return value
 *   of the call method, which is the return value of the contract function.
 *   \n To call contract function that may change the state, use BoatCitaTxSend()
 *   instead.
 *   \n If call a contract function that may change the state with
 *   BoatCitaCallContractFunc(), the function will be executed and return a value,
 *   but none of the states will change.
 *
 * @param[in] tx_ptr
 *   Transaction pointer
 *
 * @param[in] func_proto_str
 *   A string representing the prototype of the called function.\n
 *   Note: uint is treated as uint256.\n
 *   e.g.: for the contract function:\n
 *       function readListByIndex(uint index) public view returns (bytes32 event_)\n
 *   its prototype is "readListByIndex(uint256)"
 *
 * @param[in] func_param_ptr
 *   A byte stream containing the parameters to pass to the function.\n
 *   The layout conforms to cita ABI: https://solidity.readthedocs.io/en/develop/abi-spec.html\n
 *   If <func_param_ptr> is NULL, this function doesn't take any parameter.
 *
 * @param[in] func_param_len
 *   Length of <func_param_ptr> in byte.
 *	
 * @return
 *   This function returns a HEX string representing the return value of the\n
 *   called contract function.\n
 *   If any error occurs, it returns NULL.
 *
 * @see BoatCitaTxSend()
 ******************************************************************************/
BCHAR *BoatCitaCallContractFunc(BoatCitaTx *tx_ptr,
								BCHAR *func_proto_str,
								BUINT8 *func_param_ptr,
								BUINT32 func_param_len);


/*!****************************************************************************
 * @brief Wait for a transaction being mined.
 *
 * @details
 *   This function polls receipt by transaction hash and waits for the transaction
 *   being mined.  
 *   Be sure the transaction object pointed by <tx_ptr> has been called with
 *   CitaSendRawtxis().
 *
 * @param[in] tx_ptr
 *   A pointer to the context of the transaction.
 *		
 * @return
 *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
 *   of the error codes.
 ******************************************************************************/
BOAT_RESULT BoatCitaGetTransactionReceipt(BoatCitaTx *tx_ptr);


/*!****************************************************************************
 * @brief query current group block number.
 ******************************************************************************/
BOAT_RESULT BoatCitaGetBlockNumber(BoatCitaTx *tx_ptr, BUINT64 *block_number);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
