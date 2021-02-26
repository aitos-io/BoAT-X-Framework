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

#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "cJSON.h"

// As Platone's wallet structure is mostly the same as Ethereum and PlatON, thus it
// re-use a lot of Ethereum and PlatON's data structure and API.
// APIs not listed here are compatible with Ethereum/PlatON.

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


BOAT_RESULT BoatPlatoneTransfer(BoatPlatoneTx *tx_ptr, BCHAR * value_hex_str)
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
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;
    
    // Set value
    value.field_len = UtilityHex2Bin( value.field, 32, value_hex_str,
									  TRIMBIN_LEFTTRIM, BOAT_TRUE  );
    result = BoatPlatoneTxSetValue(tx_ptr, &value);
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;
	
    // Set data (contains txtype only)
    UtilityUint64ToBigend( (BUINT8*)&tx_type_big, 0,  TRIMBIN_TRIM_NO );
    data.field_ptr = (BUINT8*)&tx_type_big;
    data.field_len = sizeof(BUINT64);
    
    result = BoatPlatoneTxSetData(tx_ptr, &data);
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;
    
    // Perform the transaction
    // NOTE: Field v,r,s are calculated automatically
    result = BoatPlatoneTxSend( tx_ptr );
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;

    return BOAT_SUCCESS;
}
