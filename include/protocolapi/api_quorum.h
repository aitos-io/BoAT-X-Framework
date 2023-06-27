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

/*!@brief BoAT IoT SDK interface header file for Quorum

@file
api_Quorum.h is header file for BoAT IoT SDK Quorum's interface.
*/

#ifndef __API_QUORUM_H__
#define __API_QUORUM_H__

#include "boatiotsdk.h"

/*! @defgroup Quorum-api boat Quorumereum-API
 * @{
 */

#define BOAT_QUORUM_MINE_INTERVAL 3            //!< Mining Interval of the blockchain, in seconds
#define BOAT_QUORUM_WAIT_PENDING_TX_TIMEOUT 30 //!< Timeout waiting for a transaction being mined, in seconds

#define BOAT_QUORUM_NODE_URL_MAX_LEN 127 //!< Maxmum length for node's URL

#define BOAT_QUORUM_NONCE_AUTO 0xFFFFFFFFFFFFFFFF
#define BOAT_QUORUM_ADDRESS_SIZE 20
#define BOAT_QUORUM_PUBLIC_KEY_SIZE 44

//!@brief Account information
//! An account's only identifier is its private key. An address is calculated
//! from the public key and the public key is calculated from the private key.
typedef struct TBoatQuorumAccountInfo
{
    BoatKeypairPriKeyCtx prikeyCtx; //!< prikey context

    BUINT8 address[BOAT_QUORUM_ADDRESS_SIZE]; //!< Account address calculated from public key
} BoatQuorumAccountInfo;

//!@brief Wallet information

//! Wallet information consists of account and block chain network information.
//! Currently only one account per wallet is supported.
//! NOTE: Members  of account_info and network_info will be stored, DO NOT put pointer member in it
typedef struct TBoatQuorumWallet
{
    BoatQuorumAccountInfo account_info; //!< Account information
    BoatQuorumNetworkData network_info; //!< Network information

    // Quorumereum wallet internal members. DO NOT access them from outside wallet protocol.
    struct TWeb3IntfContext *web3intf_context_ptr; //!< Web3 Interface Context
} BoatQuorumWallet;

//!@brief ECDSA signature struct
typedef struct TBoatQuorumTxFieldSig
{
    union
    {
        struct
        {
            BUINT8 r32B[32]; //!< r part of the signature
            BUINT8 s32B[32]; //!< s part of the signature
        };
        BUINT8 sig64B[64]; //!< consecutive signature composed of r+s
    };
    BUINT8 r_len; //!< Effective length of r, either 0 for unsigned tx and 32 for signed tx
    BUINT8 s_len; //!< Effective length of s, either 0 for unsigned tx and 32 for signed tx
} BoatQuorumTxFieldSig;

//!@brief QuorumRAW transaction fields

//! These fields are to be inherited by other Quorum compatible protocols (such as PlatONE).
//! Make sure any inherited struct only appends fields at the end of this struct.
//! This allow these Quorum compatible protocols to re-use a lot of Quorum
//! APIs by converting inherited struct pointer to base struct pointer, if only
//! members in base struct are visited.
typedef struct TBoatQuorumRawtxFields
{
    // Following fields are to be inherited by other Quorum compatible protocols.
    // DO NOT modify these fields unless all inherited structures are modified
    // synchronously.
    //
    // Protocols inherited these fileds include:
    // PlatONE
    BoatFieldMax32B nonce;                      //!< nonce, uint256 in bigendian, equal to the transaction count of the sender's account address
    BoatFieldMax32B gasprice;                   //!< gasprice in wei, uint256 in bigendian
    BoatFieldMax32B gaslimit;                   //!< gaslimit, uint256 in bigendian
    BUINT8 recipient[BOAT_QUORUM_ADDRESS_SIZE]; //!< recipient's address, 160 bits
    BCHAR privatefor[BOAT_QUORUM_PUBLIC_KEY_SIZE];
    BoatFieldMax32B value;    //!< value to transfer, uint256 in bigendian
    BoatFieldVariable data;   //!< data to transfer, unformatted stream
    BoatFieldMax4B v;         //!< chain id or recovery identifier, @see RawtxPerform()
    BoatQuorumTxFieldSig sig; //!< ECDSA signature, including r and s parts

    // To allow struct inheritance, other Quorum compatible protocols can
    // define a BoatXXXRawtxFields struct type with all above fields the very
    // same as Quorum and append its own fields following them.

} BoatQuorumRawtxFields;

//!@brief Quorum Transaction

//! The <rawtx_fields> may be inherited by other Quorum compatible protocols
//! (such as PlatONE). Thus <rawtx_fields> MUST be the last member in the struct
//! to allow Quorum compatible protocols to re-use Quorum APIs that take
//! BoatQuorumTx as function arguments.
typedef struct TBoatQuorumTx
{
    BoatQuorumWallet *wallet_ptr; //!< Wallet pointer the transaction is combined with
    BoatFieldMax32B tx_hash;      //!< Transaction hash returned from network
    BBOOL is_sync_tx;             //!< True to perform a synchronous transaction (wait for getting mined), False for asynchronous transaction
    BBOOL is_private;
    // <rawtx_field> MUST be the last member in this struct to allow inheritance
    BoatQuorumRawtxFields rawtx_fields; //!< RAW transaction fields
} BoatQuorumTx;

#ifdef __cplusplus
extern "C"
{
#endif

    /*!****************************************************************************/
    /**
     * @description:
     *  This function create BoatQuorumWallet by walletindex and networkIndex.
     * @param {BUINT8} walletIndex
     * @param {BUINT8} networkIndex
     * @return {*}
     *  This function returns BOAT_SUCCESS if successfully executed.
     *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
     *  for details.
     * @author: aitos
     *
     ******************************************************************************/
    BoatQuorumWallet *BoatQuorumWalletInit(BUINT8 walletIndex, BUINT8 networkIndex);

    /*!****************************************************************************
     * @brief De-initialize Boat Quorum Wallet
     *
     * @details
     *   This function de-initialize context of Boat Quorum Wallet.
     *   \n BoatQuorumWalletInit() MUST be called before any use of Boat Quorum Wallet.
     *   \n BoatQuorumWalletDeInit() MUST be called after use of Boat Quorum Wallet.
     *
     * @param[in] wallet_ptr
     *   Wallet context pointer
     *
     * @return
     *   This function doesn't return any thing.
     *
     * @see BoatQuorumWalletInit()
     ******************************************************************************/
    void BoatQuorumWalletDeInit(BoatQuorumWallet *wallet_ptr);

    /*!****************************************************************************
     * @brief Set BoatWallet: Chain ID
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
     * @param[in] chain_id
     *   Chain ID of the blockchain network to use.
     *
     * @return
     *   This function returns BOAT_SUCCESS if setting is successful.\n
     *   Otherwise it returns one of the error codes.
     ******************************************************************************/
    BOAT_RESULT BoatQuorumWalletSetChainId(BoatQuorumWallet *wallet_ptr, BUINT32 chain_id);

    /*!****************************************************************************
     * @brief Get Balance of the wallet account
     *
     * @details
     *   This function gets the balance of the wallet account from network.
     *   \n If the account is not available (i.e. the address of the wallet account
     *   never appears as a recipient in any successful non-zero-value transaction),
     *   the balance returned is 0. It's not possible to distinguish an unavailable
     *   account from a zero-balance account.
     *
     * @param[in] wallet_ptr
     *   Wallet context pointer.
     *
     * @param[in] alt_address_str
     *   A string representing which address to get balance from.
     *   If NULL, get balance of the wallet's owner address.\n
     *   Otherwise, get balance of the specified altered address, in HEX format like\n
     *   "0x19c91A4649654265823512a457D2c16981bB64F5".
     *
     * @return
     *   This function returns a HEX string representing the balance (Unit: wei,\n
     *   i.e. 1e-18 Quorum) of the account.\n
     *   If any error occurs, it returns NULL.
     ******************************************************************************/
    BCHAR *BoatQuorumWalletGetBalance(BoatQuorumWallet *wallet_ptr, BCHAR *alt_address_ptr);

    /*!*****************************************************************************
    @brief Parse RPC mQuorumod RESPONSE

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
    BOAT_RESULT BoatQuorumParseRpcResponseStringResult(const BCHAR *json_string,
                                                       BoatFieldVariable *result_out);

    /*!*****************************************************************************
    @brief Parse RPC Quorum RESPONSE

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
    BOAT_RESULT BoatQuorumParseRpcResponseResult(const BCHAR *json_string,
                                                 const BCHAR *child_name,
                                                 BoatFieldVariable *result_out);

    /*!****************************************************************************
     * @brief Initialize Quorumereum Transaction
     *
     * @details
     *   This function initialize an Quorumereum transaction.

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
     *   BoatQuorumTxSetGasPrice() can later be called to modify the gas price at any\n
     *   time before the transaction is executed.
     *
     * @param[in] gaslimit_str
     *   A HEX string representing the gas limit to be used in the transaction.\n
     *   BoatQuorumTxSetGasLimit() can later be called to modify the gas limit at any\n
     *   time before the transaction is executed.
     *
     * @param[in] recipient_str
     *   A HEX string representing the recipient address, in HEX format like\n
     *   "0x19c91A4649654265823512a457D2c16981bB64F5".\n
     *   BoatQuorumTxSetRecipient() can later be called to modify the recipient at any\n
     *   time before the transaction is executed.
     *
     * @return
     *   This function returns BOAT_SUCCESS if initialization is successful.\n
     *   Otherwise it returns one of the error codes.
     *
     * @see BoatQuorumTxSetGasPrice() BoatQuorumTxSetGasLimit() BoatQuorumTxSetRecipient()
    *******************************************************************************/

    BOAT_RESULT BoatQuorumTxInit(BoatQuorumWallet *wallet_ptr,
                                 BoatQuorumTx *tx_ptr,
                                 BBOOL is_sync_tx,
                                 BBOOL is_private,
                                 BCHAR *gasprice_str,
                                 BCHAR *gaslimit_str,
                                 BCHAR *recipient_str,
                                 BCHAR *privatefor_str);

    /*!****************************************************************************
     * @brief Set Transaction Parameter: Transaction Nonce
     *
     * @details
     *   This function initialize an Quorumereum transaction.
     *   \n This function sets the nonce to the transaction count of the account
     *   obtained from network.
     *   \n This function can be called after BoatQuorumTxInit() has been called.
     *
     * @param[in] tx_ptr
     *   The pointer to the transaction.
     *
     * @param[in] nonce
     *   The nonce to use in the transaction.\n
     *   If BOAT_Quorum_NONCE_AUTO (0xFFFFFFFFFFFFFFFF) is specified, the nonce is\n
     *   obtained from network.
     *
     * @return
     *   This function returns BOAT_SUCCESS if setting is successful.\n
     *   Otherwise it returns one of the error codes.
     * @see BoatQuorumTxInit()
     ******************************************************************************/
    BOAT_RESULT BoatQuorumTxSetNonce(BoatQuorumTx *tx_ptr, BUINT64 nonce);

    /*!****************************************************************************
     * @brief Set Transaction Parameter: GasLimit
     *
     * @details
     *   This function sets the gas limit of the transaction.
     *
     * @param[in] tx_ptr
     *   Pointer to the transaction structure.
     *
     * @param[in] gas_limit_ptr
     *  The gas limit is generally a fixed value on the corresponding chain.
     *
     * @return
     *   This function returns BOAT_SUCCESS if setting is successful.\n
     *   Otherwise it returns one of the error codes.
     ******************************************************************************/
    BOAT_RESULT BoatQuorumTxSetGasLimit(BoatQuorumTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr);

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
    BOAT_RESULT BoatQuorumTxSetRecipient(BoatQuorumTx *tx_ptr, BUINT8 address[BOAT_QUORUM_ADDRESS_SIZE]);

    /*!****************************************************************************
     * @brief Construct a raw Quorumereum transaction synchronously.
     *
     * @details
     *   This function is similar to QuorumSendRawtx except that it waits for the
     *   transaction being mined.
     *
     * @param[in] tx_ptr
     *   A pointer to the context of the transaction.
     *
     * @return
     *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
     *   of the error codes.
     *
     * @see QuorumSendRawtx() BoatQuorumGetTransactionReceipt()
     *******************************************************************************/
    BOAT_RESULT BoatQuorumSendRawtxWithReceipt(BOAT_INOUT BoatQuorumTx *tx_ptr);

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
    BOAT_RESULT BoatQuorumTxSetValue(BoatQuorumTx *tx_ptr, BoatFieldMax32B *value_ptr);

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
    BOAT_RESULT BoatQuorumTxSetData(BoatQuorumTx *tx_ptr, BoatFieldVariable *data_ptr);

    /*!****************************************************************************
     * @brief Sign and send a transaction. Also call a stateful contract function.
     *
     * @details
     *   This function sign and set a transaction.
     *   \n BoatWalletSetXXX() and BoatTxSetXXX() functions must be properly called
     *   before call this function.
     *   \n A transaction whose recipient may be an EOA address or a contract address.
     *   In latter case it's usually a contract function call.
     *   \n This function invokes the Quorum_sendRawTransaction RPC mQuorumod.
     *   Quorum_sendRawTransaction mQuorumod only applies the transaction and returns a
     *   transaction hash. The transaction is not verified (got mined) until the
     *   nodes in the network get into consensus about the transaction. This
     *   function will invoke Quorum_getTransactionReceipt mQuorumod to wait for the
     *   transaction being mined or timeout.
     *   \n If the transaction is a contract function call, the caller cannot get its
     *   return value because the transaction is asynchronously executed. It's a
     *   good practice to save the return value in a state variable and use
     *   BoatQuorumCallContractFunc() to call a "read" contract function that could read
     *   and return the state variable.
     *
     *   @note
     *   Any contract function that may change the state of the contract shall
     *   be called in a transaction way. "state" is the "global variable" used in a
     *   contract.
     *   \n Any contract function that doesn't change the state of the contract can
     *   be called either in a transaction way or by BoatQuorumCallContractFunc(), which
     *   invokes the Quorum_call RPC mQuorumod. However the former will consume gas and
     *   latter doesn't consume gas.
     *
     * @param[in] tx_ptr
     *   Pointer to the transaction structure.
     *
     * @return
     *   This function returns BOAT_SUCCESS if setting is successful.\n
     *   Otherwise it returns one of the error codes.
     *
     * @see BoatQuorumCallContractFunc()
     ******************************************************************************/
    BOAT_RESULT BoatQuorumTxSend(BoatQuorumTx *tx_ptr);

    /*!****************************************************************************
     * @brief Call a state-less contract function
     *
     * @details
     *   This function calls contract function that doesn't change the state of the
     *   contract. "state" is the "global variable" used in a contract.
     *   \n This function invokes the Quorum_call RPC mQuorumod. Quorum_call mQuorumod requests the
     *   blockchain node to execute the function without affecting the block chain.
     *   The execution runs only on the requested node thus it can return immediately
     *   after the execution. This function synchronously return the return value
     *   of the Quorum_call mQuorumod, which is the return value of the contract function.
     *   \n To call contract function that may change the state, use BoatQuorumTxSend()
     *   instead.
     *   \n If call a contract function that may change the state with
     *   BoatQuorumCallContractFunc(), the function will be executed and return a value,
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
     *   The layout conforms to Quorumereum ABI: https://solidity.readthedocs.io/en/develop/abi-spec.html\n
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
     * @see BoatQuorumTxSend()
     ******************************************************************************/
    BCHAR *BoatQuorumCallContractFunc(BoatQuorumTx *tx_ptr,
                                      BCHAR *func_proto_str,
                                      BUINT8 *func_param_ptr,
                                      BUINT32 func_param_len);

    /*!****************************************************************************
     * @brief Transfer Quorumer to spceified recipient
     *
     * @details
     *   This function transfer Quorumer from the wallet's owner account to the specified
     *   recipient account.
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
     *   Otherwise it returns one of the error codes.
     ******************************************************************************/
    BOAT_RESULT BoatQuorumTransfer(BoatQuorumTx *tx_ptr, BCHAR *value_hex_str);

    /*!****************************************************************************
     * @brief Wait for a transaction being mined.
     *
     * @details
     *   This function polls receipt by transaction hash and waits for the transaction
     *   being mined.
     *   Be sure the transaction object pointed by <tx_ptr> has been called with
     *   QuorumSendRawtxis().
     *
     * @param[in] tx_ptr
     *   A pointer to the context of the transaction.
     *
     * @return
     *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
     *   of the error codes.
     ******************************************************************************/
    BOAT_RESULT BoatQuorumGetTransactionReceipt(BoatQuorumTx *tx_ptr);

    /*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
