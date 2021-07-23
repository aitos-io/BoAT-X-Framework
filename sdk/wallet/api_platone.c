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

#include "boatconfig.h"
#include "boatinternal.h"
#if PROTOCOL_USE_PLATONE == 1
#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "cJSON.h"

// As Platone's wallet structure is mostly the same as Ethereum, thus it
// re-use a lot of Ethereum's data structure and API. APIs not listed 
// here are compatible with Ethereum.

BOAT_RESULT BoatPlatoneTxInit( BoatPlatoneWallet *wallet_ptr,
							   BoatPlatoneTx *tx_ptr,
							   BBOOL is_sync_tx,
							   BCHAR *gasprice_str,
							   BCHAR *gaslimit_str,
							   BCHAR *recipient_str,
							   BoatPlatoneTxtype txtype)
{
    BOAT_RESULT result;

    if( (wallet_ptr == NULL) || (tx_ptr == NULL) || (recipient_str == NULL) )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;
    memset( &tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields) );

    // Re-use Ethereum transaction initialization
    result = BoatEthTxInit( (BoatEthWallet *)wallet_ptr, (BoatEthTx *)tx_ptr,
                            is_sync_tx, gasprice_str, gaslimit_str, recipient_str );
    if( result != BOAT_SUCCESS )
    {
		BoatLog(BOAT_LOG_CRITICAL, "platone Tx init failed.");
        return result;
    }

    // Set transaction type
    result = BoatPlatoneTxSetTxtype(tx_ptr, txtype);

    if( result != BOAT_SUCCESS )
    {
		BoatLog(BOAT_LOG_CRITICAL, "platone set Tx type failed.");
        return result;
    }
    
    return BOAT_SUCCESS;
}


BOAT_RESULT BoatPlatoneTxSetTxtype(BoatPlatoneTx *tx_ptr, BoatPlatoneTxtype txtype)
{
    if( tx_ptr == NULL )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    tx_ptr->rawtx_fields.txtype = txtype;
    
    return BOAT_SUCCESS;
}


BCHAR *BoatPlatoneCallContractFunc( BoatPlatoneTx *tx_ptr, BUINT8 *rlp_param_ptr,
									 BUINT32 rlp_param_len )
{
    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len*2 + 3]; // Compiler MUST support C99 to allow variable-size local array
	
    Param_eth_call param_eth_call;
    BCHAR *retval_str;

    if( rlp_param_ptr == NULL && rlp_param_len != 0 )
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

    BCHAR recipient_hexstr[BOAT_PLATONE_ADDRESS_SIZE*2+3];
    
    UtilityBinToHex( recipient_hexstr, tx_ptr->rawtx_fields.recipient, BOAT_PLATONE_ADDRESS_SIZE,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE );
    param_eth_call.to = recipient_hexstr;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_eth_call.gas      = "0x1fffff";
    param_eth_call.gasPrice = "0x8250de00";

    // Set function parameters
    UtilityBinToHex( data_str, rlp_param_ptr, rlp_param_len,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE );
    param_eth_call.data = data_str;
    param_eth_call.block_num_str = "latest";
    retval_str = web3_eth_call( tx_ptr->wallet_ptr->web3intf_context_ptr,
                                tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                &param_eth_call);

    return retval_str;

}


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


BOAT_RESULT BoatPlatoneTransfer(BoatPlatoneTx *tx_ptr, BCHAR *value_hex_str)
{
    BoatFieldMax32B   value;
    BoatFieldVariable data;
    BUINT64           tx_type_big;
    BOAT_RESULT       result;
   
    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL || value_hex_str == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
    // Set nonce
    result = BoatPlatoneTxSetNonce(tx_ptr, BOAT_PLATONE_NONCE_AUTO);
    if( result != BOAT_SUCCESS )
    {
         return result;
    }
    
    // Set value
    value.field_len = UtilityHexToBin( value.field, 32, value_hex_str,
									  TRIMBIN_LEFTTRIM, BOAT_TRUE  );
    result = BoatPlatoneTxSetValue(tx_ptr, &value);
    if( result != BOAT_SUCCESS )
    {
         return result;
    }
	
    // Set data (contains txtype only)
    UtilityUint64ToBigend( (BUINT8*)&tx_type_big, 0,  TRIMBIN_TRIM_NO );
    data.field_ptr = (BUINT8*)&tx_type_big;
    data.field_len = sizeof(BUINT64);
    
    result = BoatPlatoneTxSetData(tx_ptr, &data);
    if( result != BOAT_SUCCESS )
    {
         return result;
    }
    
    // Perform the transaction
    // NOTE: Field v,r,s are calculated automatically
    result = BoatPlatoneTxSend( tx_ptr );
    if( result != BOAT_SUCCESS )
    {
         return result;
    }

    return BOAT_SUCCESS;
}
#endif /* end of PROTOCOL_USE_PLATONE */
