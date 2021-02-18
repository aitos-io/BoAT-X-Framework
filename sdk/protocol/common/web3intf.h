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

//!@brief step size of the buffer to store RPC "REQUEST/RESPONSE" string and RPC "result" string.
#define WEB3_STRING_BUF_STEP_SIZE       1024

typedef struct TWeb3IntfContext
{
    void * rpc_context_ptr;   //!< The RPC context. The exact type depends on the RPC method
    BUINT32 web3_message_id;  //!< Random Message ID to distinguish different messages.
	BoatFieldVariable web3_json_string_buf;   //!< A JSON string buffer used for both REQUEST and RESPONSE
	BoatFieldVariable web3_result_string_buf; //!< A string buffer to store RPC "result" string parsed from JSON RESPONSE
}Web3IntfContext;

#ifdef __cplusplus
extern "C" {
#endif

BOAT_RESULT web3_parse_json_result(const BCHAR * json_string, 
								   const BCHAR * child_name, 
								   BoatFieldVariable *result_out);

Web3IntfContext * web3_init(void);
void web3_deinit(Web3IntfContext *web3intf_context_ptr);

//!@brief Parameter for web3_eth_getTransactionCount()
typedef struct TParam_eth_getTransactionCount
{
    BCHAR *address_str; //!< String of 20-byte Ethereum address, e.g. "0x123456..."
    BCHAR *block_num_str;  //!< String of either block number or one of "latest", "earliest" and "pending"
}Param_eth_getTransactionCount;

BCHAR *web3_eth_getTransactionCount(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_getTransactionCount *param_ptr);


//!@brief Parameter for web3_eth_getBalance()
typedef struct TParam_eth_getBalance
{
    BCHAR *address_str; //!< String of 20-byte Ethereum address, e.g. "0x123456..."
    BCHAR *block_num_str;  //!< String of either block number or one of "latest", "earliest" and "pending"
}Param_eth_getBalance;

BCHAR *web3_eth_getBalance(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_getBalance *param_ptr);


//!@brief Parameter for web3_eth_sendRawTransaction()
typedef struct TParam_eth_sendRawTransaction
{
    BCHAR *signedtx_str;  //!< String of the signed transaction in HEX with "0x" prefixed
}Param_eth_sendRawTransaction;

BCHAR *web3_eth_sendRawTransaction(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_sendRawTransaction *param_ptr);

BCHAR *web3_eth_gasPrice(Web3IntfContext *web3intf_context_ptr, BCHAR *node_url_str);


//!@brief Parameter for web3_eth_getStorageAt()
typedef struct TParam_eth_getStorageAt
{
    BCHAR *address_str;    //!< String of 20-byte Ethereum address, e.g. "0x123456..."
    BCHAR *position_str;   //!< String of storage position
    BCHAR *block_num_str;  //!< String of either block number or one of "latest", "earliest" and "pending"

}Param_eth_getStorageAt;

BCHAR *web3_eth_getStorageAt(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_getStorageAt *param_ptr);


//!@brief Parameter for web3_eth_getTransactionReceiptStatus()
typedef struct TParam_eth_getTransactionReceipt
{
    BCHAR *tx_hash_str; //!< String of 32-byte transaction hash, e.g. "0x123456..."
}Param_eth_getTransactionReceipt;

BCHAR *web3_eth_getTransactionReceiptStatus(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_getTransactionReceipt *param_ptr);

//!@brief Parameter for web3_eth_call()
typedef struct TParam_eth_call
{
    BCHAR *to;       //!< The address of the contract.
    BCHAR *gas;      //!< The gasLimit.
    BCHAR *gasPrice; //!< The gasPrice in wei.
    BCHAR *data;     //!< The function selector followed by parameters.
    BCHAR *block_num_str;  //!< String of either block number or one of "latest", "earliest" and "pending"
}Param_eth_call;

BCHAR *web3_eth_call(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_call *param_ptr);

typedef struct TParam_platone_call
{
    BCHAR *to;       //!< The address of the contract.
    BCHAR *gas;      //!< The gasLimit.
    BCHAR *gasPrice; //!< The gasPrice in wei.
    BCHAR *data;     //!< The function selector followed by parameters.
    BCHAR *txtype;   //!< The type of contract invoked.
    BCHAR *block_num_str;  //!< String of either block number or one of "latest", "earliest" and "pending"
}Param_platone_call;



#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
