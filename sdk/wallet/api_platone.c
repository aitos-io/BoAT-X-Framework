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

/*!@brief Platone wallet API for BoAT IoT SDK

@file
boatPlatonewallet.c defines the Platone wallet API for BoAT IoT SDK.
*/

#include "boatinternal.h"

#if PROTOCOL_USE_PLATONE == 1

#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "randgenerator.h"
#include "bignum.h"
#include "cJSON.h"

#if BOAT_USE_OPENSSL != 0
#include <openssl/evp.h>
#include <openssl/aes.h>
#endif


// As Platone's wallet structure is mostly the same as Ethereum and PlatON, thus it
// re-use a lot of Ethereum and PlatON's data structure and API.
// APIs not listed here are compatible with Ethereum/PlatON.


/******************************************************************************
@brief Set Platone Transaction Parameter: Transaction Type

Function: BoatPlatoneTxSetTxtype()

    This function sets the type of transaction


@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.


@param[in] tx_ptr
    The nonce obtained from network is set in BoatPlatoneTx structure.

@param[in] txtype
    The transaction type.
        
*******************************************************************************/
BOAT_RESULT BoatPlatoneTxSetTxtype(BoatPlatoneTx *tx_ptr, BoatPlatoneTxtype txtype)
{
    if( tx_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    tx_ptr->rawtx_fields.txtype = txtype;
    
    return BOAT_SUCCESS;

}

/******************************************************************************
@brief Sign and send a transaction. Also call a stateful contract function.

Function: BoatPlatoneTxSend()

    This function sign and set a transaction.

    BoatWalletSetXXX() and BoatTxSetXXX() functions must be properly called
    before call this function.

    A transaction whose recipient may be an EOA address or a contract address.
    In latter case it's usually a contract function call.

    This function invokes the eth_sendRawTransaction RPC method.
    eth_sendRawTransaction method only applies the transaction and returns a
    transaction hash. The transaction is not verified (got mined) until the
    nodes in the network get into consensus about the transaction. This
    function will invoke eth_getTransactionReceipt method to wait for the
    transaction being mined or timeout.

    If the transaction is a contract function call, the caller cannot get its
    return value because the transaction is asynchronously executed. It's a
    good practice to save the return value in a state variable and use
    BoatPlatoneCallContractFunc() to call a "read" contract function that could read
    and return the state variable.


    NOTE:

    Any contract function that may change the state of the contract shall
    be called in a transaction way. "state" is the "global variable" used in a
    contract.

    Any contract function that doesn't change the state of the contract can
    be called either in a transaction way or by BoatPlatoneCallContractFunc(), which
    invokes the eth_call RPC method. However the former will consume gas and
    latter doesn't consume gas.

@see BoatPlatonTxSend()
    
    
@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.
    

@param[in] tx_ptr
    Pointer to the transaction structure.
*******************************************************************************/
BOAT_RESULT BoatPlatoneTxSend(BoatPlatoneTx *tx_ptr)
{
    BOAT_RESULT result;

    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }


    if( tx_ptr->is_sync_tx == BOAT_FALSE )
    {
        result = PlatoneSendRawtx(tx_ptr);
    }
    else
    {
        result = PlatoneSendRawtxWithReceipt(tx_ptr);
    }
    
    return result;
}

/******************************************************************************
@brief Initialize PlatONE Transaction

Function: BoatPlatoneTxInit()

    This function initialize an PlatONE transaction.
    
    

@see BoatEthTxInit() BoatPlatoneTxSetTxtype()

@return
    This function returns BOAT_SUCCESS if initialization is successful.\n
    Otherwise it returns BOAT_ERROR.
    

@param[in] wallet_ptr
    The wallet pointer that this transaction is combined with.

@param[in] tx_ptr
    Pointer a transaction object.

@param[in] is_sync_tx
    For a stateful transaction, specify BOAT_TRUE to wait until the transaction is mined.\n
    Specifiy BOAT_FALSE to allow multiple transactions to be sent continuously in a short time.\n
    For a state-less contract call, this option is ignored.

@param[in] gasprice_str
    A HEX string representing the gas price (unit: wei) to be used in the transaction.\n
    Set <gasprice> = NULL to obtain gas price from network.\n
    BoatPlatoneTxSetGasPrice() can later be called to modify the gas price at any\n
    time before the transaction is executed.

@param[in] gaslimit_str
    A HEX string representing the gas limit to be used in the transaction.\n
    BoatPlatoneTxSetGasLimit() can later be called to modify the gas limit at any\n
    time before the transaction is executed.

@param[in] recipient_str
    A HEX string representing the recipient address, in HEX format like\n
    "0x19c91A4649654265823512a457D2c16981bB64F5".\n
    BoatPlatoneTxSetRecipient() can later be called to modify the recipient at any\n
    time before the transaction is executed.

@param[in] txtype
    Transaction type defined by BoatPlatoneTxtype.

*******************************************************************************/
BOAT_RESULT BoatPlatoneTxInit(BoatPlatoneWallet *wallet_ptr,
                          BoatPlatoneTx *tx_ptr,
                          BBOOL is_sync_tx,
                          BCHAR * gasprice_str,
                          BCHAR * gaslimit_str,
                          BCHAR *recipient_str,
                          BoatPlatoneTxtype txtype)
{
    BOAT_RESULT result;

    if( wallet_ptr == NULL || tx_ptr == NULL || recipient_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;

    memset(&tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields));

    // Re-use Ethereum transaction initialization
    result = BoatEthTxInit( (BoatEthWallet *)wallet_ptr,
                            (BoatEthTx *)tx_ptr,
                            is_sync_tx,
                            gasprice_str,
                            gaslimit_str,
                            recipient_str );

    if( result != BOAT_SUCCESS )
    {
        return BOAT_ERROR;
    }


    // Set transaction type
    result = BoatPlatoneTxSetTxtype(tx_ptr, txtype);

    if( result != BOAT_SUCCESS )
    {
        return BOAT_ERROR;
    }
    
    return BOAT_SUCCESS;
}
/******************************************************************************
@brief Transfer PlatONE value to spceified recipient

Function: BoatPlatoneTransfer()

    This function transfer PlatONE value from the wallet's owner account to the
    specified recipient account.

    Before calling this function, all necessary wallet fields such as private key,
    node URL and etc. must be set correctly.


@return
    This function returns BOAT_SUCCESS if transfer is successful.\n
    Otherwise it returns one of the error codes.
    

@param[in] tx_ptr
    Transaction pointer.

@param[in] value_hex_str
    A string representing the value (Unit: wei) to transfer, in HEX format like\n
    "0x89AB3C".\n
    Note that decimal value is not accepted. If a decimal value such as "1234"\n
    is specified, it's treated as "0x1234".
        
*******************************************************************************/
BOAT_RESULT BoatPlatoneTransfer(BoatPlatoneTx *tx_ptr, BCHAR * value_hex_str)
{
    BoatFieldMax32B value;
    BoatFieldVariable data;
    BUINT64 tx_type_big;
    BOAT_RESULT result;
   
    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL || value_hex_str == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
    // Set nonce
    result = BoatPlatoneTxSetNonce(tx_ptr, BOAT_PLATONE_NONCE_AUTO);
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;
    

    // Set value

    value.field_len =
    UtilityHex2Bin(
                    value.field,
                    32,
                    value_hex_str,
                    TRIMBIN_LEFTTRIM,
                    BOAT_TRUE
                  );

    result = BoatPlatoneTxSetValue(tx_ptr, &value);

    if( result != BOAT_SUCCESS ) return BOAT_ERROR;


    // Set data (contains txtype only)
    UtilityUint64ToBigend(
                            (BUINT8*)&tx_type_big,
                            0,
                            TRIMBIN_TRIM_NO
                            );

    data.field_ptr = (BUINT8*)&tx_type_big;
    data.field_len = sizeof(BUINT64);
    
    result = BoatPlatoneTxSetData(tx_ptr, &data);
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;

    
    // Perform the transaction
    // NOTE: Field v,r,s are calculated automatically
    result = BoatPlatoneTxSend(tx_ptr);
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;

    return BOAT_SUCCESS;
}

/******************************************************************************
@brief Call a state-less contract function

Function: BoatPlatoneCallContractFunc()

    This function calls contract function that doesn't change the state of the
    contract. "state" is the "global variable" used in a contract.

    This function invokes the eth_call RPC method. eth_call method requests the
    blockchain node to execute the function without affecting the block chain.
    The execution runs only on the requested node thus it can return immediately
    after the execution. This function synchronously return the return value
    of the eth_call method, which is the return value of the contract function.

    To call contract function that may change the state, use BoatPlatoneTxSend()
    instead.

    If call a contract function that may change the state with
    BoatPlatoneCallContractFunc(), the function will be executed and return a value,
    but none of the states will change.

@see BoatPlatoneTxSend()

@return
    This function returns a HEX string representing the return value of the\n
    called contract function.\n
    If any error occurs, it returns NULL.
    

@param[in] tx_ptr
    Transaction pointer

@param[in] rlp_param_ptr
    A byte stream containing the parameters to pass to the function.\n
    The stream is encoded in RLP format including the function name followed by\n
    all parameters.

@param[in] rlp_param_len
    Length of <rlp_param_ptr> in byte.
        
*******************************************************************************/
BCHAR * BoatPlatoneCallContractFunc(
                    BoatPlatoneTx *tx_ptr,
                    BUINT8 *rlp_param_ptr,
                    BUINT32 rlp_param_len)
{

    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len*2 + 3]; // Compiler MUST support C99 to allow variable-size local array
 
    Param_eth_call param_eth_call;
    BCHAR *retval_str;

    if( rlp_param_ptr == NULL && rlp_param_len != 0 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return NULL;
    }

    BCHAR recipient_hexstr[BOAT_PLATONE_ADDRESS_SIZE*2+3];
    
    UtilityBin2Hex(
				recipient_hexstr,
				tx_ptr->rawtx_fields.recipient,
				BOAT_PLATONE_ADDRESS_SIZE,
				BIN2HEX_LEFTTRIM_UNFMTDATA,
				BIN2HEX_PREFIX_0x_YES,
				BOAT_FALSE
                );
    param_eth_call.to = recipient_hexstr;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_eth_call.gas = "0x1fffff";
    param_eth_call.gasPrice = "0x8250de00";


    // Set function parameters
    UtilityBin2Hex(
			data_str,
			rlp_param_ptr,
			rlp_param_len,
			BIN2HEX_LEFTTRIM_UNFMTDATA,
			BIN2HEX_PREFIX_0x_YES,
            BOAT_FALSE);

    
    param_eth_call.data = data_str;

    param_eth_call.block_num_str = "latest";

    retval_str = web3_eth_call( tx_ptr->wallet_ptr->web3intf_context_ptr,
                                tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                &param_eth_call);

    
    return retval_str;

}

#endif /* end of PROTOCOL_USE_PLATONE */
