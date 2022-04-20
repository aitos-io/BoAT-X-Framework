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

/*!@brief Venachain wallet API for BoAT IoT SDK

@file
boatVenachainwallet.c defines the Venachain wallet API for BoAT IoT SDK.
*/

#include "boatconfig.h"
#include "boatinternal.h"
#if PROTOCOL_USE_VENACHAIN == 1
#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "cJSON.h"

// As Venachain's wallet structure is mostly the same as Ethereum, thus it
// re-use a lot of Ethereum's data structure and API. APIs not listed 
// here are compatible with Ethereum.

BOAT_RESULT BoatVenachainTxInit(BoatVenachainWallet *wallet_ptr,
							  BoatVenachainTx *tx_ptr,
							  BBOOL is_sync_tx,
							  BCHAR *gasprice_str,
							  BCHAR *gaslimit_str,
							  BCHAR *recipient_str)
{
    BOAT_RESULT result;

    if ((wallet_ptr == NULL) || (tx_ptr == NULL) || (recipient_str == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;
    memset(&tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields));

    // Re-use Ethereum transaction initialization
    result = BoatEthTxInit((BoatEthWallet *)wallet_ptr, (BoatEthTx *)tx_ptr,
                           is_sync_tx, gasprice_str, gaslimit_str, recipient_str);
    if (result != BOAT_SUCCESS)
    {
		BoatLog(BOAT_LOG_CRITICAL, "venachain Tx init failed.");
        return result;
    }
    /*
    // Set transaction type
    result = BoatVenachainTxSetTxtype(tx_ptr, txtype);

    if (result != BOAT_SUCCESS)
    {
		BoatLog(BOAT_LOG_CRITICAL, "venachain set Tx type failed.");
        return result;
    }
    */
    return BOAT_SUCCESS;
}





BCHAR *BoatVenachainCallContractFunc(BoatVenachainTx *tx_ptr, BUINT8 *rlp_param_ptr,
								   BUINT32 rlp_param_len)
{
    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len*2 + 3]; // Compiler MUST support C99 to allow variable-size local array
	
    Param_eth_call param_eth_call;
    BOAT_RESULT result = BOAT_SUCCESS;
    BCHAR *retval_str;

    if (rlp_param_ptr == NULL && rlp_param_len != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return NULL;
    }

	if (rlp_param_len > BOAT_STRING_MAX_LEN)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return NULL;
	}

    if (BOAT_SUCCESS != UtilityStringLenCheck((BCHAR*)rlp_param_ptr))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return NULL;
    }

    BCHAR recipient_hexstr[BOAT_VENACHAIN_ADDRESS_SIZE*2+3];
    
    UtilityBinToHex(recipient_hexstr, tx_ptr->rawtx_fields.recipient, BOAT_VENACHAIN_ADDRESS_SIZE,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_eth_call.to = recipient_hexstr;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_eth_call.gas      = "0x1fffff";
    param_eth_call.gasPrice = "0x8250de00";

    // Set function parameters
    UtilityBinToHex(data_str, rlp_param_ptr, rlp_param_len,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_eth_call.method_name_str = "eth_call";
    param_eth_call.data = data_str;
    param_eth_call.block_num_str = "latest";
    retval_str = web3_call(tx_ptr->wallet_ptr->web3intf_context_ptr,
                           tx_ptr->wallet_ptr->network_info.node_url_ptr,
                           &param_eth_call,&result);
    if (retval_str == NULL){
        BoatLog(BOAT_LOG_CRITICAL, "web3 call fail, result = %d ",result);
    }
    return retval_str;
}



BOAT_RESULT BoatVenachainSendRawtxWithReceipt(BOAT_INOUT BoatVenachainTx *tx_ptr)
{
    BOAT_RESULT result = BOAT_ERROR;

    result = VenachainSendRawtx(tx_ptr);

    if (result == BOAT_SUCCESS)
    {
        result = BoatVenachainGetTransactionReceipt(tx_ptr);
    }

    return result;
}

BOAT_RESULT BoatVenachainTxSend(BoatVenachainTx *tx_ptr)
{
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (tx_ptr->is_sync_tx == BOAT_FALSE)
    {
        result = VenachainSendRawtx(tx_ptr);
    }
    else
    {
        result = BoatVenachainSendRawtxWithReceipt(tx_ptr);
    }
    
    return result;
}


BOAT_RESULT BoatVenachainTransfer(BoatVenachainTx *tx_ptr, BCHAR *value_hex_str)
{
    BoatFieldMax32B   value;
    BoatFieldVariable data;
//    BUINT64           tx_type_big;
    BOAT_RESULT       result;
   
    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL || value_hex_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    
    // Set nonce
    result = BoatVenachainTxSetNonce(tx_ptr, BOAT_VENACHAIN_NONCE_AUTO);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
    
    // Set value
    value.field_len = UtilityHexToBin(value.field, 32, value_hex_str,
									  TRIMBIN_LEFTTRIM, BOAT_TRUE);
    result = BoatVenachainTxSetValue(tx_ptr, &value);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
	
    // Set data (contains txtype only)
    // UtilityUint64ToBigend((BUINT8*)&tx_type_big, 0,  TRIMBIN_TRIM_NO);
    // data.field_ptr = (BUINT8*)&tx_type_big;
    // data.field_len = sizeof(BUINT64);
    
    // result = BoatVenachainTxSetData(tx_ptr, &data);
    // if (result != BOAT_SUCCESS)
    // {
    //     return result;
    // }
    
    // Perform the transaction
    // NOTE: Field v,r,s are calculated automatically
    result = BoatVenachainTxSend(tx_ptr);
    if (result != BOAT_SUCCESS)
    {
         return result;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatVenachainParseRpcResponseStringResult(const BCHAR *json_string, BoatFieldVariable *result_out)
{
    return venachain_parse_json_result(json_string, "", result_out);
}

BOAT_RESULT BoatVenachainParseRpcResponseResult(const BCHAR *json_string, 
										  	  const BCHAR *child_name, 
										  	  BoatFieldVariable *result_out)
{
    if (child_name == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
	return venachain_parse_json_result(json_string, child_name, result_out);
}
#endif /* end of PROTOCOL_USE_VENACHAIN */
