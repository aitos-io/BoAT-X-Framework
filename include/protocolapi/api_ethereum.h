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

#define BOAT_ETH_MINE_INTERVAL                   3  //!< Mining Interval of the blockchain, in seconds
#define BOAT_ETH_WAIT_PENDING_TX_TIMEOUT         30 //!< Timeout waiting for a transaction being mined, in seconds

#define BOAT_ETH_NODE_URL_MAX_LEN                127 //!< Maxmum length for node's URL

#define BOAT_ETH_NONCE_AUTO                      0xFFFFFFFFFFFFFFFF
#define BOAT_ETH_ADDRESS_SIZE                    20

//!@brief Account information
//! An account's only identifier is its private key. An address is calculated
//! from the public key and the public key is calculated from the private key.
typedef struct TBoatEthAccountInfo
{
    BoatWalletPriKeyCtx prikeyCtx;         //!< prikey context                       

    BUINT8  address[BOAT_ETH_ADDRESS_SIZE];//!< Account address calculated from public key
}BoatEthAccountInfo;


/*!
 * @brief Blockchain network information
 *
 * EIP-155 (https://github.com/ethereum/EIPs/blob/master/EIPS/eip-155.md) requires
 * chain ID of the network being part of the transaction before it's signed.
 * If the network is NOT EIP-155 compatible, <eip155_compatibility> must be FALSE
 * and <chain_id> is ignored. Otherwise the chain ID must be set.\n
 * <node_url_ptr> must include the protocol descriptor, IP address or URL name and 
 * port. For example, http://a.b.com:8545
 */
typedef struct TBoatEthNetworkInfo
{
    BUINT32 chain_id;    //!< Chain ID (in host endian) of the blockchain network if the network is EIP-155 compatible

    /*!@brief Network EIP-155 compatibility

    If the network is EIP-155 compabible <eip155_compatibility> must be set to TRUE and <chain_id>
    must be set.\n
    Otherwise set it to FALSE and <chain_id> is ignored.*/
    BBOOL eip155_compatibility;
    BCHAR *node_url_ptr; //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
}BoatEthNetworkInfo;


//!@brief Wallet information

//! Wallet information consists of account and block chain network information.
//! Currently only one account per wallet is supported.
//! NOTE: Members  of account_info and network_info will be stored, DO NOT put pointer member in it
typedef struct TBoatEthWallet
{
    BoatEthAccountInfo account_info; //!< Account information
    BoatEthNetworkInfo network_info; //!< Network information

    // Ethereum wallet internal members. DO NOT access them from outside wallet protocol.
    struct TWeb3IntfContext *web3intf_context_ptr;  //!< Web3 Interface Context
}BoatEthWallet;


//!@brief Ethereum Wallet configuration

//! Ethereum wallet configuration is used in wallet creation.
typedef struct TBoatEthWalletConfig
{
	BoatWalletPriKeyCtx_config  prikeyCtx_config; //!< @NOTE This field MUST BE placed in the first member of the structure
	                                              //!< because in function BoatWalletCreate(), 
    BUINT32  chain_id;    //!< Chain ID (in host endian) of the blockchain network if the network is EIP-155 compatible
    BBOOL    eip155_compatibility;    //!< Network EIP-155 compatibility. See BoatEthNetworkInfo
    BCHAR    node_url_str[BOAT_ETH_NODE_URL_MAX_LEN]; //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
    BBOOL    load_existed_wallet;   //false: need creat key by Boat , true: not need creat key
}BoatEthWalletConfig;


//!@brief ECDSA signature struct
typedef struct TBoatEthTxFieldSig
{
    union
    {
        struct
        {
            BUINT8 r32B[32]; //!< r part of the signature
            BUINT8 s32B[32]; //!< s part of the signature
        };
        BUINT8 sig64B[64];   //!< consecutive signature composed of r+s
    };
    BUINT8 r_len;            //!< Effective length of r, either 0 for unsigned tx and 32 for signed tx
    BUINT8 s_len;            //!< Effective length of s, either 0 for unsigned tx and 32 for signed tx
}BoatEthTxFieldSig;

//!@brief EthereumRAW transaction fields

//! These fields are to be inherited by other Ethereum compatible protocols (such as PlatONE).
//! Make sure any inherited struct only appends fields at the end of this struct.
//! This allow these Ethereum compatible protocols to re-use a lot of Ethereum
//! APIs by converting inherited struct pointer to base struct pointer, if only
//! members in base struct are visited.
typedef struct TBoatEthRawtxFields
{
    // Following fields are to be inherited by other Ethereum compatible protocols.
    // DO NOT modify these fields unless all inherited structures are modified
    // synchronously.
    //
    // Protocols inherited these fileds include:
    // PlatONE
    BoatFieldMax32B nonce;        //!< nonce, uint256 in bigendian, equal to the transaction count of the sender's account address
    BoatFieldMax32B gasprice;     //!< gasprice in wei, uint256 in bigendian
    BoatFieldMax32B gaslimit;     //!< gaslimit, uint256 in bigendian
    BUINT8 recipient[BOAT_ETH_ADDRESS_SIZE]; //!< recipient's address, 160 bits
    BoatFieldMax32B value;        //!< value to transfer, uint256 in bigendian
    BoatFieldVariable data;       //!< data to transfer, unformatted stream
    BoatFieldMax4B v;             //!< chain id or recovery identifier, @see RawtxPerform()
    BoatEthTxFieldSig sig;        //!< ECDSA signature, including r and s parts
    
    // To allow struct inheritance, other Ethereum compatible protocols can
    // define a BoatXXXRawtxFields struct type with all above fields the very
    // same as Ethereum and append its own fields following them.
    
}BoatEthRawtxFields;

//!@brief Ethereum Transaction

//! The <rawtx_fields> may be inherited by other Ethereum compatible protocols
//! (such as PlatONE). Thus <rawtx_fields> MUST be the last member in the struct
//! to allow Ethereum compatible protocols to re-use Ethereum APIs that take
//! BoatEthTx as function arguments.
typedef struct TBoatEthTx
{
    BoatEthWallet *wallet_ptr; //!< Wallet pointer the transaction is combined with
    BoatFieldMax32B tx_hash;   //!< Transaction hash returned from network
    BBOOL is_sync_tx;          //!< True to perform a synchronous transaction (wait for getting mined), False for asynchronous transaction

    // <rawtx_field> MUST be the last member in this struct to allow inheritance
    BoatEthRawtxFields rawtx_fields;       //!< RAW transaction fields
}BoatEthTx;


#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Initialize Boat Ethereum Wallet
 *
 * @details
 *   This function sets up context of Boat Ethereum Wallet based on given
 *   configuration.
 *   \n DO NOT call this function directly. Instead call BoatWalletCreate() with an Ethereum
 *   wallet configuration like:
 *   @verbatim
     BoatEthWalletConfig eth_config = {...};
     BSINT32 wallet_index;
     wallet_index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM,
                                     &eth_config,
                                     sizeof(BoatEthWalletConfig)
                                    );
     @endverbatim
 *   \n BoatEthWalletInit() MUST be called before any use of Boat Ethereum Wallet.
 *   \n BoatEthWalletDeInit() MUST be called after use of Boat Ethereum Wallet.
 *   
 * @param[in] config_ptr
 *   Pointer to Ethereum wallet configuration.
 *
 * @param[in] config_size
 *   Size (in byte) of Ethereum wallet configuration.
 *
 * @return
 *   This function returns instance pointer of BoatEthWallet if initialization is successful.\n
 *   Otherwise it returns NULL.
 *		
 * @see BoatEthWalletDeInit() BoatWalletCreate()
 ******************************************************************************/
BoatEthWallet *BoatEthWalletInit(const BoatEthWalletConfig *config_ptr, BUINT32 config_size);


/*!****************************************************************************
 * @brief De-initialize Boat Ethereum Wallet
 *
 * @details
 *   This function de-initialize context of Boat Ethereum Wallet.
 *   \n BoatEthWalletInit() MUST be called before any use of Boat Ethereum Wallet.
 *   \n BoatEthWalletDeInit() MUST be called after use of Boat Ethereum Wallet.
 *
 * @param[in] wallet_ptr
 *   Wallet context pointer
 *
 * @return 
 *   This function doesn't return any thing.
 *
 * @see BoatEthWalletInit()
 ******************************************************************************/
void BoatEthWalletDeInit(BoatEthWallet *wallet_ptr);


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
BOAT_RESULT BoatEthWalletSetNodeUrl(BoatEthWallet *wallet_ptr, const BCHAR *node_url_ptr);


/*!****************************************************************************
 * @brief Set BoatWallet: EIP-155 Compatibility
 *
 * @details
 *   This function sets if the network supports EIP-155.
 *   If the network supports EIP-155, set it to BOAT_TRUE.
 *   Otherwise set it to BOAT_FALSE.

 * @param[in] wallet_ptr
 *   Wallet context pointer.   
 *
 * @param[in] eip155_compatibility
 *   BOAT_TRUE if the network supports EIP-155. Otherwise BOAT_FALSE.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.      
 ******************************************************************************/
BOAT_RESULT BoatEthWalletSetEIP155Comp(BoatEthWallet *wallet_ptr, BBOOL eip155_compatibility);


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
BOAT_RESULT BoatEthWalletSetChainId(BoatEthWallet *wallet_ptr, BUINT32 chain_id);


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
 *   i.e. 1e-18 ETH) of the account.\n
 *   If any error occurs, it returns NULL.
 ******************************************************************************/
BCHAR *BoatEthWalletGetBalance(BoatEthWallet *wallet_ptr, BCHAR *alt_address_ptr);

/*!*****************************************************************************
@brief Prase RPC method RESPONSE

   This function Prase "result" segment.
   "result" object must be cjson_string, this function will returns contents of "result" . 

@param[in] json_string
	 The json to be parsed.

@param[out] result_out
	 The buffer to store prase result.
	 Caller can allocate memory for this param, or can initial it with {NULL, 0},
	 this function will expand the memory if it too small to store prase result.
	 
@return
    This function returns BOAT_SUCCESS if prase successed. Otherwise
    it returns an error code.
*******************************************************************************/
BOAT_RESULT BoatEthPraseRpcResponseStringResult(const BCHAR *json_string, 
										        BoatFieldVariable *result_out);

/*!*****************************************************************************
@brief Prase RPC method RESPONSE

   This function Prase "result" segment.
   If "result" object must be json object, the parameter named "child_name" will actived,
   if "child_name" object is string, this function will returns contents of "child_name"; 
   if "child_name" object is other types, his function will prompt "un-implemention yet".
   For other types of "result" this function is not support yet.

@param[in] json_string
	 The json to be parsed.

@param[in] child_name
	 This parameter is mandatory.

@param[out] result_out
	 The buffer to store prase result.
	 Caller can allocate memory for this param, or can initial it with {NULL, 0},
	 this function will expand the memory if it too small to store prase result.
	 
@return
    This function returns BOAT_SUCCESS if prase successed. Otherwise
    it returns an error code.
*******************************************************************************/
BOAT_RESULT BoatEthPraseRpcResponseResult(const BCHAR *json_string, 
										  const BCHAR *child_name, 
										  BoatFieldVariable *result_out);

                                           
/*!****************************************************************************
 * @brief Initialize Ethereum Transaction
 *
 * @details
 *   This function initialize an Ethereum transaction.

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
 *   BoatEthTxSetGasPrice() can later be called to modify the gas price at any\n
 *   time before the transaction is executed.
 *
 * @param[in] gaslimit_str
 *   A HEX string representing the gas limit to be used in the transaction.\n
 *   BoatEthTxSetGasLimit() can later be called to modify the gas limit at any\n
 *   time before the transaction is executed.
 *
 * @param[in] recipient_str
 *   A HEX string representing the recipient address, in HEX format like\n
 *   "0x19c91A4649654265823512a457D2c16981bB64F5".\n
 *   BoatEthTxSetRecipient() can later be called to modify the recipient at any\n
 *   time before the transaction is executed.
 *	
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes.
 *
 * @see BoatEthTxSetGasPrice() BoatEthTxSetGasLimit() BoatEthTxSetRecipient()
*******************************************************************************/
BOAT_RESULT BoatEthTxInit(BoatEthWallet *wallet_ptr,
                          BoatEthTx *tx_ptr,
                          BBOOL is_sync_tx,
                          BCHAR *gasprice_str,
                          BCHAR *gaslimit_str,
                          BCHAR *recipient_str);


/*!****************************************************************************
 * @brief Set Transaction Parameter: Transaction Nonce
 *
 * @details
 *   This function initialize an Ethereum transaction.
 *   \n This function sets the nonce to the transaction count of the account
 *   obtained from network.
 *   \n This function can be called after BoatEthTxInit() has been called.
 *
 * @param[in] tx_ptr
 *   The pointer to the transaction.
 *
 * @param[in] nonce
 *   The nonce to use in the transaction.\n
 *   If BOAT_ETH_NONCE_AUTO (0xFFFFFFFFFFFFFFFF) is specified, the nonce is\n
 *   obtained from network.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.
 * @see BoatEthTxInit()       
 ******************************************************************************/
BOAT_RESULT BoatEthTxSetNonce(BoatEthTx *tx_ptr, BUINT64 nonce);


/*!****************************************************************************
 * @brief Set Transaction Parameter: GasPrice
 *
 * @details
 *   This function sets the gas price of the transaction.
 *
 * @param[in] tx_ptr
 *   Pointer to the transaction structure.
 *
 *@param[in] gas_price_ptr
 *   The gas price in wei.\n
 *   If <gas_price_ptr> is NULL, the gas price obtained from network is used.
 *
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.      
 ******************************************************************************/
BOAT_RESULT BoatEthTxSetGasPrice(BoatEthTx *tx_ptr, BoatFieldMax32B *gas_price_ptr);


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
BOAT_RESULT BoatEthTxSetGasLimit(BoatEthTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr);


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
BOAT_RESULT BoatEthTxSetRecipient(BoatEthTx *tx_ptr, BUINT8 address[BOAT_ETH_ADDRESS_SIZE]);

/*!****************************************************************************
 * @brief Construct a raw ethereum transaction synchronously.
 *
 * @details
 *   This function is similar to EthSendRawtx except that it waits for the
 *   transaction being mined.
 *   
 * @param[in] tx_ptr
 *   A pointer to the context of the transaction.
 *
 * @return
 *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
 *   of the error codes.
 *	
 * @see EthSendRawtx() BoatEthGetTransactionReceipt() 
*******************************************************************************/
BOAT_RESULT BoatEthSendRawtxWithReceipt(BOAT_INOUT BoatEthTx *tx_ptr);

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
BOAT_RESULT BoatEthTxSetValue(BoatEthTx *tx_ptr, BoatFieldMax32B *value_ptr);


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
BOAT_RESULT BoatEthTxSetData(BoatEthTx *tx_ptr, BoatFieldVariable *data_ptr);


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
 *   BoatEthCallContractFunc() to call a "read" contract function that could read
 *   and return the state variable.
 *
 *   @note
 *   Any contract function that may change the state of the contract shall
 *   be called in a transaction way. "state" is the "global variable" used in a
 *   contract.
 *   \n Any contract function that doesn't change the state of the contract can
 *   be called either in a transaction way or by BoatEthCallContractFunc(), which
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
 * @see BoatEthCallContractFunc()    
 ******************************************************************************/
BOAT_RESULT BoatEthTxSend(BoatEthTx *tx_ptr);


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
 *   \n To call contract function that may change the state, use BoatEthTxSend()
 *   instead.
 *   \n If call a contract function that may change the state with
 *   BoatEthCallContractFunc(), the function will be executed and return a value,
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
 *   The layout conforms to Ethereum ABI: https://solidity.readthedocs.io/en/develop/abi-spec.html\n
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
 * @see BoatEthTxSend()
 ******************************************************************************/
BCHAR *BoatEthCallContractFunc(BoatEthTx *tx_ptr,
								BCHAR *func_proto_str,
								BUINT8 *func_param_ptr,
								BUINT32 func_param_len);


/*!****************************************************************************
 * @brief Transfer Ether to spceified recipient
 *
 * @details
 *   This function transfer Ether from the wallet's owner account to the specified
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
BOAT_RESULT BoatEthTransfer(BoatEthTx *tx_ptr, BCHAR *value_hex_str);


/*!****************************************************************************
 * @brief Wait for a transaction being mined.
 *
 * @details
 *   This function polls receipt by transaction hash and waits for the transaction
 *   being mined.  
 *   Be sure the transaction object pointed by <tx_ptr> has been called with
 *   EthSendRawtxis().
 *
 * @param[in] tx_ptr
 *   A pointer to the context of the transaction.
 *		
 * @return
 *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
 *   of the error codes.
 ******************************************************************************/
BOAT_RESULT BoatEthGetTransactionReceipt(BoatEthTx *tx_ptr);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
