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

/*!@brief Web3 interface header file

@file
web3intf.h is the header file for web3 interface.
*/

#ifndef __WEB3INTF_H__
#define __WEB3INTF_H__

/*! @defgroup web3intf communication-web3 
 * @{
 */
#include "boatlog.h"

//!@brief step size of the buffer to store RPC "REQUEST/RESPONSE" string and RPC "result" string.
#define WEB3_STRING_BUF_STEP_SIZE       1024

typedef struct TWeb3IntfContext
{
    void *rpc_context_ptr;   //!< The RPC context. The exact type depends on the RPC method
    BUINT32 web3_message_id;  //!< Random Message ID to distinguish different messages.
	BoatFieldVariable web3_json_string_buf;   //!< A JSON string buffer used for both REQUEST and RESPONSE
	BoatFieldVariable web3_result_string_buf; //!< A string buffer to store RPC "result" string parsed from JSON RESPONSE
}Web3IntfContext;

//!@brief Parameter for web3_getTransactionCount()
typedef struct TParam_eth_getTransactionCount
{
    BCHAR *method_name_str; //!< String of this method name, e.g. Ethereum "eth_getTransactionCount" PlatON "platon_getTransactionCount"
    BCHAR *address_str; //!< String of Ethereum 20-byte address,or PlatON bech32 address, e.g. Ethereum "0x123456..." PlatON "lax1gp7h8k9ynm4ct5ev73j4qlwhr4g8zqxpnkqrx3"
    BCHAR *block_num_str;  //!< String of either block number or one of "latest", "earliest" and "pending"
}Param_eth_getTransactionCount;

//!@brief Parameter for web3_getBalance()
typedef struct TParam_eth_getBalance
{
    BCHAR *method_name_str; //!< String of this method name, e.g. Ethereum "eth_getTransactionCount" PlatON "platon_getTransactionCount"
    BCHAR *address_str;  //!< String of 20-byte Ethereum address, e.g. "0x123456..."
    BCHAR *block_num_str;//!< String of either block number or one of "latest", "earliest" and "pending"
}Param_eth_getBalance;

//!@brief Parameter for web3_sendRawTransaction()
typedef struct TParam_eth_sendRawTransaction
{
    BCHAR *method_name_str; //!< String of this method name, e.g. Ethereum "eth_getTransactionCount" PlatON "platon_getTransactionCount"
    BCHAR *signedtx_str;  //!< String of the signed transaction in HEX with "0x" prefixed
}Param_eth_sendRawTransaction;

//!@brief Parameter for web3_getStorageAt()
typedef struct TParam_eth_getStorageAt
{
    BCHAR *method_name_str; //!< String of this method name, e.g. Ethereum "eth_getTransactionCount" PlatON "platon_getTransactionCount"
    BCHAR *address_str;   //!< String of 20-byte Ethereum address, e.g. "0x123456..."
    BCHAR *position_str;  //!< String of storage position
    BCHAR *block_num_str; //!< String of either block number or one of "latest", "earliest" and "pending"
}Param_eth_getStorageAt;

//!@brief Parameter for web3_getTransactionReceiptStatus()
typedef struct TParam_eth_getTransactionReceipt
{
    BCHAR *method_name_str; //!< String of this method name, e.g. Ethereum "eth_getTransactionCount" PlatON "platon_getTransactionCount"
    BCHAR *tx_hash_str; //!< String of 32-byte transaction hash, e.g. "0x123456..."
}Param_eth_getTransactionReceipt;

//!@brief Parameter for web3_call()
typedef struct TParam_eth_call
{
    BCHAR *method_name_str; //!< String of this method name, e.g. Ethereum "eth_getTransactionCount" PlatON "platon_getTransactionCount"
    BCHAR *to;       //!< The address of the contract.
    BCHAR *gas;      //!< The gasLimit.
    BCHAR *gasPrice; //!< The gasPrice in wei.
    BCHAR *data;     //!< The function selector followed by parameters.
    BCHAR *block_num_str;  //!< String of either block number or one of "latest", "earliest" and "pending"
}Param_eth_call;




/***************************************************************************************************
                                  FISCO BCOS JSONRPC INTERFACE
***************************************************************************************************/
//typedef struct TParam_fiscobcos_call
typedef struct TParam_fiscobcos_call
{
	BCHAR *groupid;  //!< groupid
	BCHAR *from;     //!< The address of the sender.
    BCHAR *to;       //!< The address of the contract.
    BCHAR *value;    //!< transfer value(optional).
	BCHAR *data;     //!< transfer data(optional).
}Param_fiscobcos_call;

//!@brief Parameter for web3_fiscobcos_sendRawTransaction()
typedef struct TParam_fiscobcos_sendRawTransaction
{
	BCHAR *groupid;       //!< groupid
    BCHAR *signedtx_str;  //!< String of the signed transaction in HEX with "0x" prefixed
}Param_fiscobcos_sendRawTransaction;

//!@brief Parameter for web3_fiscobcos_getTransactionReceiptStatus()
typedef struct TParam_fiscobcos_getTransactionReceipt
{
	BCHAR *groupid;     //!< groupid
    BCHAR *tx_hash_str; //!< String of 32-byte transaction hash, e.g. "0x123456..."
}Param_fiscobcos_getTransactionReceipt;

//!@brief Parameter for web3_fiscobcos_getBlockNumber()
typedef struct TParam_fiscobcos_getBlockNumber
{
	BCHAR *groupid;     //!< groupid
}Param_fiscobcos_getBlockNumber;




#ifdef __cplusplus
extern "C" {
#endif


/*!*****************************************************************************
@brief Prase RPC method RESPONSE

   This function Prase "result" segment.
   If "result" object is string, this function will returns contents of "result" . 
   If "result" object is still json object, the parameter named "child_name" will actived,
   if "child_name" object is string, this function will returns contents of "child_name"; 
   if "child_name" object is other types, this function will prompt "un-implemention yet".
   For other types of "result" this function is not support yet.

@param[in] json_string
	 The json to be parsed.

@param[in] child_name
	 if "result" item is json object, this param will actived.

@param[out] result_out
	 The buffer to store prase result.
	 Caller can allocate memory for this param, or can initial it with {NULL, 0},
	 this function will expand the memory if it too small to store prase result.
	 
@return
    This function returns BOAT_SUCCESS if prase successed. Otherwise
    it returns an error code.
*******************************************************************************/
BOAT_RESULT web3_parse_json_result(const BCHAR *json_string, 
								   const BCHAR *child_name, 
								   BoatFieldVariable *result_out);


/*!****************************************************************************
 * @brief Initialize web3 interface
 *
 * @details
 *   This function initialize resources for web3 interface.
 *
 * @param This function doesn't take any argument.
 *
 * @return
 *   This function always returns a pointer to the Web3 Interface context.\n
 *   It returns NULL if initialization fails.
 ******************************************************************************/
Web3IntfContext *web3_init(void);


/*!****************************************************************************
 * @brief Deinitialize web3 interface.
 *
 * @details
 *   This function de-initializes web3 interface.
 *
 * @param[in] web3intf_context_ptr
 *   Pointer to the web3 interface context.
 * @see web3_init()    
 *
 * @return
 *   This function doesn't return any value.
 ******************************************************************************/
void web3_deinit(Web3IntfContext *web3intf_context_ptr);


/*!****************************************************************************
 * @brief 
 *   Perform eth_getTransactionCount RPC method and get the transaction count
 *   of the specified account
 *
 * @details
 *   This function calls RPC method eth_getTransactionCount and returns a string
 *   representing the transaction count of the specified address.
 *   \n The typical RPC REQUEST is similar to:
 *   {"jsonrpc":"2.0","method":"eth_getTransactionCount","params":
 *	 ["0xc94770007dda54cF92009BFF0dE90c06F603a09f","latest"],"id":1}
 *   \n The typical RPC RESPONSE from blockchain node is similar to:
 *   {"id":1,"jsonrpc": "2.0","result": "0x1"}
 *   \n This function takes 2 arguments. The first is the URL to the blockchian
 *   node. Protocol and port must be explicitly specified in URL, such as
 *   "http://127.0.0.1:7545". The second argument is a struct containing all
 *   RPC call parameters as specified by ethereum.
 *   \n See following wiki for details about RPC parameters and return value:
 *   https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api
 *   \n This function returns a string representing the item "result" of the
 *   RESPONSE from the RPC call. The buffer storing the string is maintained by
 *   web3intf and the caller shall NOT modify it, free it or save the address
 *   for later use.
 *   
 * @param web3intf_context_ptr
 *   A pointer to Web3 Interface context
 *
 * @param node_url_str
 *   A string indicating the URL of blockchain node.
 *
 * @param param_ptr
 *   The parameters of the eth_getTransactionCount RPC method.\n
 *   address_str:\n
 *       DATA, 20 Bytes - address, a string such as "0x123456..."\n
 *   block_num_str:\n
 *       QUANTITY|TAG - a string of integer block number, or "latest", "earliest" or "pending"
 * @param web3Result
 *  return BOAT_RESULT
 * 
 * @return
 *   This function returns a string representing the transaction count of the\n
 *   address specified in <param_ptr>.\n
 *   For example, an address that has initiated 3 transactions will return\n
 *   "0x3". Note the leading zeros are trimmed as specified in ethereum RPC\n
 *   interface.\n
 *   The transaction count is typically used as the "nonce" in a new transaction.\n
 *   If any error occurs or RPC call timeouts, it returns NULL.
 ******************************************************************************/
BCHAR *web3_getTransactionCount(Web3IntfContext *web3intf_context_ptr,
                                BCHAR *node_url_str,
                                const Param_eth_getTransactionCount *param_ptr,
                                BOAT_RESULT *web3Result);


/*!****************************************************************************
 * @brief 
 *   Perform eth_getBalance RPC method and get the balance of the specified account
 *
 * @details
 *   This function calls RPC method eth_getBalance and returns a string
 *   representing the balance of the specified address.
 *   \n The typical RPC REQUEST is similar to:
 *   {"jsonrpc":"2.0","method":"eth_getBalance","params":
 *	 ["0xc94770007dda54cF92009BFF0dE90c06F603a09f", "latest"],"id":1}
 *   \n The typical RPC RESPONSE from blockchain node is similar to:
 *   {"id":1,"jsonrpc": "2.0","result": "0x0234c8a3397aab58"}
 *   \n This function takes 2 arguments. The first is the URL to the blockchian
 *   node. Protocol and port must be explicitly specified in URL, such as
 *   "http://127.0.0.1:7545". The second argument is a struct containing all
 *   RPC call parameters as specified by ethereum.
 *   \n See following wiki for details about RPC parameters and return value:
 *   https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api
 *   \n This function returns a string representing the item "result" of the
 *   RESPONSE from the RPC call. The buffer storing the string is maintained by
 *   web3intf and the caller shall NOT modify it, free it or save the address
 *   for later use.
 *
 * @return
 *   This function returns a string representing the balance (Unit: wei, i.e.\n
 *   1e-18 ETH) of the address specified in <param_ptr>.\n
 *   For example, an address that has a balance of 1024 wei will return "0x400".\n
 *   Note the leading zeros are trimmed as specified in ethereum RPC interface.\n
 *   If any error occurs or RPC call timeouts, it returns NULL.
 *   
 * @param web3intf_context_ptr
 *   A pointer to Web3 Interface context
 *
 * @param node_url_str
 *   A string indicating the URL of blockchain node.
 *
 * @param web3Result
 *   return BOAT_RESULT
 * 
 * @param param_ptr
 *   The parameters of the eth_getTransactionCount RPC method.\n
 *   address_str:\n
 *       DATA, 20 Bytes - address to check for balance.\n
 *   block_num_str:\n
 *       QUANTITY|TAG - a string of integer block number, or "latest", "earliest" or "pending"       
 ******************************************************************************/
BCHAR *web3_getBalance(Web3IntfContext *web3intf_context_ptr,
                       BCHAR *node_url_str,
                       const Param_eth_getBalance *param_ptr,
                       BOAT_RESULT *web3Result);


/*!****************************************************************************
 * @brief Perform eth_sendRawTransaction RPC method.
 *
 * @details
 *   This function calls RPC method eth_sendRawTransaction and returns a string
 *   representing the transaction hash.
 *   \n The typical RPC REQUEST is similar to:
 *   {"jsonrpc":"2.0","method":"eth_sendRawTransaction","params":
 * 	 ["0xd46e8dd67c5d32be8d46e8dd67c5d32be8058bb8eb970870f072445675058bb8eb970870f072445675"],"id":1}
 *   \n The typical RPC RESPONSE from blockchain node is similar to:
 *   {"id":1,"jsonrpc": "2.0","result": "0xe670ec64341771606e55d6b4ca35a1a6b75ee3d5145a99d05921026d1527331"}
 *   \n This function takes 2 arguments. The first is the URL to the blockchian
 *   node. Protocol and port must be explicitly specified in URL, such as
 *   "http://127.0.0.1:7545". The second argument is a struct containing all
 *   RPC call parameters as specified by ethereum.
 *   \n See following wiki for details about RPC parameters and return value:
 *   https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api
 *   \n This function returns a string representing the item "result" of the
 *   RESPONSE from the RPC call. The buffer storing the string is maintained by
 *   web3intf and the caller shall NOT modify it, free it or save the address
 *   for later use.
 *
 * @param web3intf_context_ptr
 *   A pointer to Web3 Interface context
 *
 * @param node_url_str
 *   A string indicating the URL of blockchain node.
 *
 * @param param_ptr
 *   The parameters of the eth_sendRawTransaction RPC method.\n
 *   signedtx_str:\n
 *       DATA, The signed transaction data as a HEX string, with "0x" prefix.
 *
 * @param web3Result
 *  return BOAT_RESULT
 * 
 * @return
 *   This function returns a string representing a 32-byte transaction hash\n
 *   of the transaction if the blockchain node accepts the transaction in its\n
 *   pool. If this transaction is not yet avaialble, the returned hash is "0x0".\n    
 *   If the blockchain node returns error or RPC call timeouts, it returns NULL.\n\n
 *   Note: A successful return from eth_sendRawTransaction DOES NOT mean the\n
 *   transaction is confirmed. The caller shall priodically polling the receipt\n
 *   using eth_getTransactionReceipt method with the transaction hash returned\n
 *   by eth_sendRawTransaction.
 ******************************************************************************/
BCHAR *web3_sendRawTransaction(Web3IntfContext *web3intf_context_ptr,
                               BCHAR *node_url_str,
                               const Param_eth_sendRawTransaction *param_ptr,
                               BOAT_RESULT *web3Result);


/*!****************************************************************************
 * @brief 
 *   Perform eth_gasPrice RPC method and get the current price per gas in wei
 *   of the specified network.
 *
 * @details
 *   This function calls RPC method eth_gasPrice and returns a string
 *   representing the current price per gas in wei of the specified network.
 *   \n The typical RPC REQUEST is similar to:
 *   {"jsonrpc":"2.0","method":"eth_gasPrice","params":[],"id":73}
 *   \n The typical RPC RESPONSE from blockchain node is similar to:
 *   {"id":73,"jsonrpc": "2.0","result": "0x09184e72a000"}
 *   \n This function takes 1 arguments, which is the URL to the blockchian
 *   node. Protocol and port must be explicitly specified in URL, such as
 *   "http://127.0.0.1:7545". The eth_gasPrice RPC method itself doesn't take
 *   any parameter.
 *   \n See following wiki for details about RPC parameters and return value:
 *   https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api
 *   \n This function returns a string representing the item "result" of the
 *   RESPONSE from the RPC call. The buffer storing the string is maintained by
 *   web3intf and the caller shall NOT modify it, free it or save the address
 *   for later use.
 *   
 * @param web3intf_context_ptr
 *   A pointer to Web3 Interface context
 *
 * @param node_url_str
 *   A string indicating the URL of blockchain node.
 * 
 * @param web3Result
 *   return BOAT_RESULT
 *
 * @return
 *   This function returns a string representing the current price per gas in\n
 *   wei of the specified network.\n
 *   For example, if current gas price is 2000000000/gas, it will return\n
 *   "0x77359400". Note the leading zeros are trimmed as specified in ethereum RPC\n
 *   interface.\n
 *   The gasPrice returned from network is a reference for use in a transaction.\n
 *   Specify a higher gasPrice in transaction may increase the posibility that\n
 *   the transcaction is get mined quicker and vice versa.\n
 *   If any error occurs or RPC call timeouts, it returns NULL.
 ******************************************************************************/
BCHAR *web3_gasPrice(Web3IntfContext *web3intf_context_ptr, BCHAR *node_url_str, BCHAR *method_name_str,BOAT_RESULT *web3Result);


/*!****************************************************************************
 * @brief Perform eth_getStorageAt RPC method.
 *
 * @details
 *   This function calls RPC method eth_getStorageAt and returns a string
 *   representing the storage data.
 *   \n The typical RPC REQUEST is similar to:
 *   {"jsonrpc":"2.0", "method": "eth_getStorageAt", "params": 
 *   ["0x295a70b2de5e3953354a6a8344e616ed314d7251", "0x0", "latest"], "id": 1}
 *   \n The typical RPC RESPONSE from blockchain node is similar to:
 *   {"jsonrpc":"2.0","id":1,"result":"0x00000000000000000000000000000000000000000000000000000000000004d2"}
 *   \n This function takes 2 arguments. The first is the URL to the blockchian
 *   node. Protocol and port must be explicitly specified in URL, such as
 *   "http://127.0.0.1:7545". The second argument is a struct containing all
 *   RPC call parameters as specified by ethereum.
 *   \n See following wiki for details about RPC parameters and return value:
 *   https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api
 *   The way to calculate <position_str> is quite complicated if the datum is
 *   not a simple element type. The position conforms to the storage slot as
 *   specified by Solidity memory layout.
 *   \n Solidity Memory Layout:
 *   https://solidity.readthedocs.io/en/latest/miscellaneous.html#layout-of-state-variables-in-storage
 *   \n A easy-to-understand explanation can be found at:
 *   https://medium.com/aigang-network/how-to-read-ethereum-contract-storage-44252c8af925
 *   \n This function returns a string representing the item "result" of the
 *   RESPONSE from the RPC call. The buffer storing the string is maintained by
 *   web3intf and the caller shall NOT modify it, free it or save the address
 *   for later use.
 *
 * @param web3intf_context_ptr
 *   A pointer to Web3 Interface context
 *
 * @param node_url_str
 *   A string indicating the URL of blockchain node.
 *
 * @param param_ptr
 *   The parameters of the eth_getStorageAt RPC method.\n        
 *   address_str:\n
 *       DATA, 20 Bytes - address, a string such as "0x123456..."\n
 *   position_str:\n
 *       QUANTITY - integer of the position in the storage.\n        
 *   block_num_str:\n
 *       QUANTITY|TAG - a string of integer block number, or "latest", "earliest" or "pending"
 *
 * @param web3Result
 *  return BOAT_RESULT
 * 
 * @return
 *   This function returns a string representing a 32-byte value of the data\n
 *   stored at slot <position_str> of <address_str> of <block_num_str> as\n
 *   specified in <param_ptr>. Note that multiple data may be packed in one\n
 *   storage slot. See Solidity Memory Layout to understand how they are packed.\n
 *   If the blockchain node returns error or RPC call timeouts, it returns NULL.
 ******************************************************************************/
BCHAR *web3_getStorageAt(Web3IntfContext *web3intf_context_ptr,
						 BCHAR *node_url_str,
						 const Param_eth_getStorageAt *param_ptr,
                         BOAT_RESULT *web3Result);


/*!****************************************************************************
 * @brief Perform eth_getTransactionReceipt RPC method.
 *
 * @details
 *   This function calls RPC method eth_getTransactionReceipt and returns a
 *   string representing the result.status of the receipt object of the
 *   specified transaction hash.
 *   \n The typical RPC REQUEST is similar to:
 *   {"jsonrpc":"2.0","method":"eth_getTransactionReceipt","params":
 *   ["0xb903239f8543d04b5dc1ba6579132b143087c68db1b2168786408fcbce568238"],"id":1}
 *   \n The typical RPC RESPONSE from blockchain node is similar to:
    @verbatim
    {
    "id":1,
    "jsonrpc":"2.0",
    "result": {
         transactionHash: '0xb903239f8543d04b5dc1ba6579132b143087c68db1b2168786408fcbce568238',
         transactionIndex:  '0x1', // 1
         blockNumber: '0xb', // 11
         blockHash: '0xc6ef2fc5426d6ad6fd9e2a26abeab0aa2411b7ab17f30a99d3cb96aed1d1055b',
         cumulativeGasUsed: '0x33bc', // 13244
         gasUsed: '0x4dc', // 1244
         contractAddress: '0xb60e8dd61c5d32be8058bb8eb970870f07233155', // or null, if none was created
         logs: [{
             // logs as returned by getFilterLogs, etc.
         }, ...],
         logsBloom: "0x00...0", // 256 byte bloom filter
         status: '0x1'
      }
    }
    @endverbatim
 *   \n This function takes 2 arguments. The first is the URL to the blockchian
 *   node. Protocol and port must be explicitly specified in URL, such as
 *   "http://127.0.0.1:7545". The second argument is a struct containing all
 *   RPC call parameters as specified by ethereum.
 *   \n See following wiki for details about RPC parameters and return value:
 *   https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api
 *   \n This function returns a string representing the item "result.status" of the
 *   RESPONSE from the RPC call. The buffer storing the string is maintained by
 *   web3intf and the caller shall NOT modify it, free it or save the address
 *   for later use.
 *
 * @param web3intf_context_ptr
 *   A pointer to Web3 Interface context
 *
 * @param node_url_str
 *   A string indicating the URL of blockchain node.
 *
 * @param param_ptr
 *   The parameters of the eth_getTransactionReceipt RPC method.\n        
 *   tx_hash_str:\n
 *       DATA, 32 Bytes - hash of a transaction
 *
 * @param web3Result
 *  return BOAT_RESULT
 * 
 * @return
 *   This function returns a string representing the status of the transaction\n
 *   receipt, "0x1" for success and "0x0" for failure. If the transaction is\n
 *   pending, it returns a null string i.e. a string containing only '\0'\n
 *   instead of a NULL pointer.\n
 *   If any error occurs or RPC call timeouts, it returns NULL.  
 ******************************************************************************/
BCHAR *web3_getTransactionReceiptStatus(Web3IntfContext *web3intf_context_ptr,
										BCHAR *node_url_str,
										const Param_eth_getTransactionReceipt *param_ptr,
                                        BOAT_RESULT *web3Result);


/*!****************************************************************************
 * @brief Perform eth_call RPC method.
 *
 * @details
 *   This function calls RPC method eth_call and returns the return value of the
 *   specified contract function.
 *   \n This function can only call contract functions that don't change the block
 *   STATE. To call contract functions that may change block STATE, use
 *   eth_sendRawTransaction instead.
 *   \n The typical RPC REQUEST is similar to:
    @verbatim
    {"jsonrpc":"2.0","method":"eth_call","params":[{
      "from": "0xb60e8dd61c5d32be8058bb8eb970870f07233155", // Optional
      "to": "0xd46e8dd67c5d32be8058bb8eb970870f07244567",
      "gas": "0x76c0", // Optional, 30400
      "gasPrice": "0x9184e72a000", // Optional, 10000000000000
      "value": "0x0", // Optional
      "data": "0xd46e8dd67c5d32be8d46e8dd67c5d32be8058bb8eb970870f072445675058bb8eb970870" //Optional
    }],"id":1}
    @endverbatim
 *   \n The typical RPC RESPONSE from blockchain node is similar to:
 *   {"id":1,"jsonrpc": "2.0","result": "0x0"}
 *   \n This function takes 2 arguments. The first is the URL to the blockchian
 *   node. Protocol and port must be explicitly specified in URL, such as
 *   "http://127.0.0.1:7545". The second argument is a struct containing all
 *   RPC call parameters as specified by ethereum.
 *   \n See following wiki for details about RPC parameters and return value:
 *   https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api
 *   \n This function could call a contract function without creating a transaction
 *   on the block chain. The only mandatory parameter of eth_call is "to", which
 *   is the address of the contract. Typically parameter "data" is also mandatory,
 *   which is composed of 4-byte function selector followed by 0 or more parameters
 *   regarding the contract function being called. See Ethereum Contract ABI for
 *   the details about how to compose "data" field:
 *   https://github.com/ethereum/wiki/wiki/Ethereum-Contract-ABI
 *   \n An eth_call doesn't consume gas, but it's a good practice to specifiy the
 *   "gas" parameter for better compatibility.
 *   \n This function returns a string representing the item "result" of the
 *   RESPONSE from the RPC call. The buffer storing the string is maintained by
 *   web3intf and the caller shall NOT modify it, free it or save the address
 *   for later use.
 *
 * @param web3intf_context_ptr
 *   A pointer to Web3 Interface context
 *
 * @param node_url_str
 *   A string indicating the URL of blockchain node.
 *
 * @param param_ptr
 *   The parameters of the eth_call RPC method. Some optional parameters are omitted.\n
 *   to: The address of the contract.\n
 *   gas: The gasLimit.\n
 *   gasPrice: The gasPrice in wei.\n
 *   data: The function selector followed by parameters.\n
 *   block_num_str:\n
 *       QUANTITY|TAG - a string of integer block number, or "latest", "earliest" or "pending"
 *
 * @return
 *   This function returns a string representing the returned value of the called\n
 *   contract function.
 ******************************************************************************/
BCHAR *web3_call(Web3IntfContext *web3intf_context_ptr,
                 BCHAR *node_url_str,
                 const Param_eth_call *param_ptr,
                 BOAT_RESULT *web3Result);













/***************************************************************************************************
                                  FISCO BCOS JSONRPC INTERFACE
***************************************************************************************************/


/*!****************************************************************************
 * @brief Perform fiscobcos_call RPC method.
 * @see web3_call
 ******************************************************************************/
BCHAR *web3_fiscobcos_call(Web3IntfContext *web3intf_context_ptr,
						   BCHAR *node_url_str,
						   const Param_fiscobcos_call *param_ptr,
                           BOAT_RESULT *web3Result);

/*!****************************************************************************
 * @brief Perform fiscobcos_sendRawTransaction RPC method.
 * @see web3_sendRawTransaction
 ******************************************************************************/
BCHAR *web3_fiscobcos_sendRawTransaction(Web3IntfContext *web3intf_context_ptr,
										 BCHAR *node_url_str,
										 const Param_fiscobcos_sendRawTransaction *param_ptr,
                                         BOAT_RESULT *web3Result);

/*!****************************************************************************
 * @brief Perform web3_fiscobcos_getTransactionReceiptStatus RPC method.
 * @see web3_getTransactionReceiptStatus
 ******************************************************************************/
BCHAR *web3_fiscobcos_getTransactionReceiptStatus(Web3IntfContext *web3intf_context_ptr,
												  BCHAR *node_url_str,
												  const Param_fiscobcos_getTransactionReceipt *param_ptr,
                                                  BOAT_RESULT *web3Result);

/*!****************************************************************************
 * @brief Perform web3_fiscobcos_getBlockNumber RPC method.
 ******************************************************************************/
BCHAR *web3_fiscobcos_getBlockNumber(Web3IntfContext *web3intf_context_ptr,
									 BCHAR *node_url_str,
									 const Param_fiscobcos_getBlockNumber *param_ptr,
                                     BOAT_RESULT *web3Result);



/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
