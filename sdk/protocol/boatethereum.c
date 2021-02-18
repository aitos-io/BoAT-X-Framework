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

/*!@brief Perform RAW transaction

@file
ethereum.c contains functions to construct a raw transaction, serialize it with RLP,
perform it and wait for its receipt.
*/

#include "boatinternal.h"
#include "web3intf.h"
#include "boatethereum.h"







/*!*****************************************************************************
@brief Construct a raw ethereum transacton asynchronously.

Function: EthSendRawtx()

    This function constructs a raw transacton and sends it asynchronously (i.e.
    don't wait for it being mined).
    
    AN INTRODUCTION OF HOW RAW TRANSACTION IS CONSTRUCTED
    
    [FIELDS IN A RAW TRANSACTION]
    
    A RAW transaction consists of following 9 fields:
        1. nonce;
        2. gasprice;
        3. gaslimit;
        4. recipient;
        5. value(optional);
        6. data(optional);
        7. v;
        8. signature.r;
        9. signature.s;

    These transaction fields are encoded as elements of a LIST in above order
    as per RLP encoding rules. "LIST" is a type of RLP field.


    EXCEPTION:
    
    For Ethereum any fields (except <recipient>) having a value of zero are
    treated as NULL stream in RLP encoding instead of 1-byte-size stream whose
    value is 0. For example, nonce = 0 is encoded as 0x80 which represents NULL
    instead of 0x00 which represents a 1-byte-size stream whose value is 0.


    [HOW TO CONSTRUCT A RAW TRANSACTION]
    
    A RAW transaction is constructed in 4 steps in different ways according to
    the blockchain network's EIP-155 compatibility. 

    See following article for details about EIP-155: 
    https://github.com/ethereum/EIPs/blob/master/EIPS/eip-155.md

    
    CASE 1: If the blockchain network does NOT support EIP-155:
    
        Step 1: Encode a LIST containing only the first 6 fields.
        Step 2: Calculate SHA3 hash of the encoded stream in Step 1.
        Step 3: Sign the hash in Step 2. This generates r, s and parity (0 or 1) for recovery identifier.
        Step 4: Encode a LIST containing all 9 fields, where
                First 6 fields are same as what they are;
                v = parity + 27, where parity is given in Step 3;
                r and s are given in Step 3.


    CASE 2: If the blockchain network DOES support EIP-155:

        Step 1: Encode all 9 fields (a LIST containing all 9 fields), where
                First 6 fields are same as what they are;
                v = Chain ID;
                r = 0;
                s = 0;

                NOTE: zero value fields other than <recipient> are encoded as NULL stream.

        Step 2: Same as CASE 1.
        Step 3: Same as CASE 1.

        Step 4: Encode a LIST containing all 9 fields, where
                First 6 fields are same as what they are;
                v = Chain ID * 2 + parity + 35, where parity is given in Step 3;
                r and s are given in Step 3.


@return
    This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
    of the error codes.
    

@param[in] tx_ptr
        A pointer to the context of the transaction.

*******************************************************************************/
BOAT_RESULT EthSendRawtx(BOAT_INOUT BoatEthTx *tx_ptr)
{
    unsigned int chain_id_len;
        
    BCHAR *tx_hash_str;

    RlpObject tx_rlp_object;
    RlpObject nonce_rlp_object;
    RlpObject gasprice_rlp_object;
    RlpObject gaslimit_rlp_object;
    RlpObject recipient_rlp_object;
    RlpObject value_rlp_object;
    RlpObject data_rlp_object;
    RlpObject v_rlp_object;
    RlpObject r_rlp_object;
    RlpObject s_rlp_object;
    
    BSINT32 v_index;
    BSINT32 r_index;
    BSINT32 s_index;
    
    RlpEncodedStreamObject *rlp_stream_storage_ptr;

    
    BCHAR *rlp_stream_hex_str = NULL;    // Storage for RLP stream HEX string for use with web3 interface

    
    BUINT8 message_digest[32];
    BUINT8 sig_parity;
    BUINT32 v;

    Param_eth_sendRawTransaction param_eth_sendRawTransaction;

#ifdef DEBUG_LOG  
    BUINT32 i;
#endif

    BOAT_RESULT result;
    boat_try_declare;


    
    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Transaction and wallet pointer cannot be null.");
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, EthSendRawtx_cleanup);
    }

    // In case the transaction should fail, tx_hash.field_len is initialized to 0
    tx_ptr->tx_hash.field_len = 0;
    
    
    result = RlpInitListObject(&tx_rlp_object);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to initialize Tx RLP objecte.");
        boat_throw(BOAT_ERROR_OUT_OF_MEMORY, EthSendRawtx_cleanup);
    }
    


 

    /**************************************************************************
    * STEP 1: Construction RAW transaction without real v/r/s                 *
    *         (See above description for details)                             *
    **************************************************************************/
    

    // Encode nonce
    result = RlpInitStringObject(&nonce_rlp_object,
                                 tx_ptr->rawtx_fields.nonce.field,
                                 tx_ptr->rawtx_fields.nonce.field_len);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to initialize nonce RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
  
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &nonce_rlp_object);
    if( result < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to append nonce to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }

    // Encode gasprice
    result = RlpInitStringObject(&gasprice_rlp_object,
                                 tx_ptr->rawtx_fields.gasprice.field,
                                 tx_ptr->rawtx_fields.gasprice.field_len);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to initialize gasprice RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &gasprice_rlp_object);
    if( result < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to append gasprice to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }

    
    // Encode gaslimit
    result = RlpInitStringObject(&gaslimit_rlp_object,
                                 tx_ptr->rawtx_fields.gaslimit.field,
                                 tx_ptr->rawtx_fields.gaslimit.field_len);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to initialize gaslimit RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &gaslimit_rlp_object);
    if( result < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to append gaslimit to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    
    
    // Encode recipient
    result = RlpInitStringObject(&recipient_rlp_object,
                                 tx_ptr->rawtx_fields.recipient,
                                 20);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to initialize recipient RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &recipient_rlp_object);
    if( result < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to append recipient to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }


    // Encode value
    result = RlpInitStringObject(&value_rlp_object,
                                 tx_ptr->rawtx_fields.value.field,
                                 tx_ptr->rawtx_fields.value.field_len);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to initialize value RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &value_rlp_object);
    if( result < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to append value to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }


    // Encode data
    result = RlpInitStringObject(&data_rlp_object,
                                 tx_ptr->rawtx_fields.data.field_ptr,
                                 tx_ptr->rawtx_fields.data.field_len);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to initialize data RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &data_rlp_object);
    if( result < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to append data to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }



    // If EIP-155 is required, encode v = chain id, r = s = NULL in this step
    if( tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE )
    {
        // v = Chain ID
        // Currently max chain id supported is (2^32 - 1 - 36)/2, because v is
        // finally calculated as chain_id * 2 + 35 or 36 as per EIP-155.
        v = tx_ptr->wallet_ptr->network_info.chain_id;
        chain_id_len = UtilityUint32ToBigend(tx_ptr->rawtx_fields.v.field,
                                             v,
                                             TRIMBIN_LEFTTRIM
                                            );
        tx_ptr->rawtx_fields.v.field_len = chain_id_len;
        
        // r = s = NULL
        tx_ptr->rawtx_fields.sig.r_len = 0;
        tx_ptr->rawtx_fields.sig.s_len = 0;

        
        // Encode v
        result = RlpInitStringObject(&v_rlp_object,
                                     tx_ptr->rawtx_fields.v.field,
                                     tx_ptr->rawtx_fields.v.field_len);
        if( result != BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to initialize v RLP object.");
            boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
        }
        
        v_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &v_rlp_object);
        if( v_index < BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to append v to Tx RLP object.");
            boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
        }
        


        // Encode r
        result = RlpInitStringObject(&r_rlp_object,
                                     tx_ptr->rawtx_fields.sig.r32B,
                                     tx_ptr->rawtx_fields.sig.r_len);
        if( result != BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to initialize r RLP object.");
            boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
        }
        
        r_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &r_rlp_object);
        if( r_index < BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to append r to Tx RLP object.");
            boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
        }


        // Encode s
        result = RlpInitStringObject(&s_rlp_object,
                                     tx_ptr->rawtx_fields.sig.s32B,
                                     tx_ptr->rawtx_fields.sig.s_len);
        if( result != BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to initialize s RLP object.");
            boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
        }
        
        s_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &s_rlp_object);
        if( s_index < BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to append s to Tx RLP object.");
            boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
        }

    }

    // Encode Tx RLP LIST
    
    result = RlpEncode(&tx_rlp_object, NULL);
    if( result == BOAT_SUCCESS )
    {
        
        rlp_stream_storage_ptr = RlpGetEncodedStream(&tx_rlp_object);

#ifdef DEBUG_LOG        
        BoatLog(BOAT_LOG_NORMAL, "Encoded RLP stream: %u bytes.", rlp_stream_storage_ptr->stream_len);
        for( i = 0; i < rlp_stream_storage_ptr->stream_len; i++ )
        {
            printf("%02x ", rlp_stream_storage_ptr->stream_ptr[i]);
        }
        putchar('\n');
#endif

    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to encode Tx.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    



    /**************************************************************************
    * STEP 2: Calculate SHA3 hash of message                                  *
    **************************************************************************/

    // Hash the message
    keccak_256(rlp_stream_storage_ptr->stream_ptr, rlp_stream_storage_ptr->stream_len, message_digest);



    /**************************************************************************
    * STEP 3: Sign the transaction                                            *
    **************************************************************************/

    // Sign the transaction
    ecdsa_sign_digest(
                       &secp256k1, // const ecdsa_curve *curve
                       tx_ptr->wallet_ptr->account_info.priv_key_array, //const uint8_t *priv_key
                       message_digest, //const uint8_t *digest
                       tx_ptr->rawtx_fields.sig.sig64B, //uint8_t *sig,
                       &sig_parity, //uint8_t *pby,
                       NULL  //int (*is_canonical)(uint8_t by, uint8_t sig[64]))
                       );


    // Trim r
    BUINT8 trimed_r[32];
    
    tx_ptr->rawtx_fields.sig.r_len =
         UtilityTrimBin(
                trimed_r,
                tx_ptr->rawtx_fields.sig.r32B,
                32,
                TRIMBIN_LEFTTRIM,
                BOAT_TRUE
                );

    memcpy(tx_ptr->rawtx_fields.sig.r32B,
           trimed_r,
           tx_ptr->rawtx_fields.sig.r_len);

    // Trim s
    BUINT8 trimed_s[32];
    
    tx_ptr->rawtx_fields.sig.s_len =
         UtilityTrimBin(
                trimed_s,
                tx_ptr->rawtx_fields.sig.s32B,
                32,
                TRIMBIN_LEFTTRIM,
                BOAT_TRUE
                );

    memcpy(tx_ptr->rawtx_fields.sig.s32B,
           trimed_s,
           tx_ptr->rawtx_fields.sig.s_len);
    
    /**************************************************************************
    * STEP 4: Encode full RAW transaction with updated v/r/s                  *
    *         (See above description for details)                             *
    **************************************************************************/

    // Re-encode v
    if( tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE )
    {
        // v = Chain ID * 2 + parity + 35
        v = tx_ptr->wallet_ptr->network_info.chain_id * 2 + sig_parity + 35;
    }
    else
    {
        // v = parity + 27
        v = sig_parity + 27;
    }
        
    chain_id_len = UtilityUint32ToBigend(tx_ptr->rawtx_fields.v.field,
                                         v,
                                         TRIMBIN_LEFTTRIM
                                        );
    tx_ptr->rawtx_fields.v.field_len = chain_id_len;


    result = RlpInitStringObject(&v_rlp_object,
                                 tx_ptr->rawtx_fields.v.field,
                                 tx_ptr->rawtx_fields.v.field_len);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to re-initialize v RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }

    if( tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE )
    {
        v_index = RlpEncoderReplaceObjectInList(&tx_rlp_object, v_index, &v_rlp_object);
    }
    else
    {
        v_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &v_rlp_object);
    }
    
    if( v_index < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to replace v in Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }




    // Re-encode r
    result = RlpInitStringObject(&r_rlp_object,
                                 tx_ptr->rawtx_fields.sig.r32B,
                                 tx_ptr->rawtx_fields.sig.r_len);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to re-initialize r RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    
    if( tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE )
    {
        r_index = RlpEncoderReplaceObjectInList(&tx_rlp_object, r_index, &r_rlp_object);
    }
    else
    {
        r_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &r_rlp_object);
    }

    if( r_index < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to replace r in Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }



    // Re-encode s
    result = RlpInitStringObject(&s_rlp_object,
                                 tx_ptr->rawtx_fields.sig.s32B,
                                 tx_ptr->rawtx_fields.sig.s_len);
    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to re-initialize s RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }

    if( tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE )
    {
        s_index = RlpEncoderReplaceObjectInList(&tx_rlp_object, s_index, &s_rlp_object);
    }
    else
    {
        s_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &s_rlp_object);
    }
    
    if( s_index < BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to replace s in Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }



    // Re-encode Tx RLP LIST
    
    result = RlpReEncode(&tx_rlp_object, NULL);
    if( result == BOAT_SUCCESS )
    {
        
        rlp_stream_storage_ptr = RlpGetEncodedStream(&tx_rlp_object);

#ifdef DEBUG_LOG        
        BoatLog(BOAT_LOG_NORMAL, "Re-Encoded RLP stream: %u bytes.", rlp_stream_storage_ptr->stream_len);
        for( i = 0; i < rlp_stream_storage_ptr->stream_len; i++ )
        {
            printf("%02x ", rlp_stream_storage_ptr->stream_ptr[i]);
        }
        putchar('\n');
#endif

    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to re-encode Tx.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, EthSendRawtx_cleanup);
    }
    

    // Allocate memory for RLP stream HEX string
    // It's a storage for HEX string converted from RLP stream binary. The
    // HEX string is used as input for web3. It's in a form of "0x1234ABCD".
    // Where *2 for binary to HEX conversion, +2 for "0x" prefix, + 1 for null terminator.
    rlp_stream_hex_str = BoatMalloc(rlp_stream_storage_ptr->stream_len * 2 + 2 + 1);

    if( rlp_stream_hex_str == NULL )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to dynamically allocate memory to store RLP HEX string.");
        boat_throw(BOAT_ERROR_OUT_OF_MEMORY, EthSendRawtx_cleanup);
    }


    // To save memory, re-use rlp_stream_hex_str to print debug information
    // As rlp_stream_hex_str's size is enough to hold the entire RLP hex string,
    // thus it's far more than enough to hold any field of the RLP hex string,
    // which is printed.
    
    // Print transaction recipient to log

    if( 0 == UtilityBin2Hex(
        rlp_stream_hex_str,
        tx_ptr->rawtx_fields.recipient,
        20,
        BIN2HEX_LEFTTRIM_UNFMTDATA,
        BIN2HEX_PREFIX_0x_YES,
        BOAT_FALSE
        ))
    {
        strcpy(rlp_stream_hex_str, "NULL");
    }

    BoatLog(BOAT_LOG_NORMAL, "Transaction to: %s", rlp_stream_hex_str);

#ifdef DEBUG_LOG


    printf("Transaction Message:\n");

    // Print nonce
    if(0 == UtilityBin2Hex(
        rlp_stream_hex_str,
        tx_ptr->rawtx_fields.nonce.field,
        tx_ptr->rawtx_fields.nonce.field_len,
        BIN2HEX_LEFTTRIM_QUANTITY,
        BIN2HEX_PREFIX_0x_YES,
        BOAT_FALSE
        ))
    {
        strcpy(rlp_stream_hex_str, "NULL");
    }

    printf("Nonce: %s\n", rlp_stream_hex_str);
            

    // Print Sender

    if( 0 == UtilityBin2Hex(
        rlp_stream_hex_str,
        tx_ptr->wallet_ptr->account_info.address,
        20,
        BIN2HEX_LEFTTRIM_UNFMTDATA,
        BIN2HEX_PREFIX_0x_YES,
        BOAT_FALSE
        ))
    {
        strcpy(rlp_stream_hex_str, "NULL");
    }

    printf("Sender: %s\n", rlp_stream_hex_str);

    
    // Print recipient

    if( 0 == UtilityBin2Hex(
        rlp_stream_hex_str,
        tx_ptr->rawtx_fields.recipient,
        20,
        BIN2HEX_LEFTTRIM_UNFMTDATA,
        BIN2HEX_PREFIX_0x_YES,
        BOAT_FALSE
        ))
    {
        strcpy(rlp_stream_hex_str, "NULL");
    }

    printf("Recipient: %s\n", rlp_stream_hex_str);


    // Print value

    if( 0 == UtilityBin2Hex(
        rlp_stream_hex_str,
        tx_ptr->rawtx_fields.value.field,
        tx_ptr->rawtx_fields.value.field_len,
        BIN2HEX_LEFTTRIM_UNFMTDATA,
        BIN2HEX_PREFIX_0x_YES,
        BOAT_FALSE
        ))
    {
        strcpy(rlp_stream_hex_str, "NULL");
    }
        
    printf("Value: %s\n", rlp_stream_hex_str);

   
    // Print data

    if( 0 == UtilityBin2Hex(
        rlp_stream_hex_str,
        tx_ptr->rawtx_fields.data.field_ptr,
        tx_ptr->rawtx_fields.data.field_len,
        BIN2HEX_LEFTTRIM_UNFMTDATA,
        BIN2HEX_PREFIX_0x_YES,
        BOAT_FALSE
        ))
    {
        strcpy(rlp_stream_hex_str, "NULL");
    }

    printf("Data: %s\n\n", rlp_stream_hex_str);
        
#endif



    UtilityBin2Hex(
                rlp_stream_hex_str,
                rlp_stream_storage_ptr->stream_ptr,
                rlp_stream_storage_ptr->stream_len,
                BIN2HEX_LEFTTRIM_UNFMTDATA,
                BIN2HEX_PREFIX_0x_YES,
                BOAT_FALSE
                );

    param_eth_sendRawTransaction.signedtx_str = rlp_stream_hex_str;
    
    tx_hash_str = web3_eth_sendRawTransaction( tx_ptr->wallet_ptr->web3intf_context_ptr,
                                               tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                               &param_eth_sendRawTransaction);
	result = BoatEthPraseRpcResponseResult( tx_hash_str, "", 
											&tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
	if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_NORMAL, "Fail to send raw transaction to network.");
		boat_throw(BOAT_ERROR_RPC_FAIL, EthSendRawtx_cleanup);
	}

    tx_ptr->tx_hash.field_len =
            UtilityHex2Bin(
                            tx_ptr->tx_hash.field,
                            32,
                            (BCHAR*)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
                            TRIMBIN_TRIM_NO,
                            BOAT_FALSE
                           );


    result = BOAT_SUCCESS;

    // Clean Up

    boat_catch(EthSendRawtx_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    // Free RLP Objects
    RlpRecursiveDeleteObject(&tx_rlp_object);
    
    // Free RLP hex string buffer
    if( rlp_stream_hex_str != NULL )
    {
        BoatFree(rlp_stream_hex_str);
    }

    return result;

}


/*!*****************************************************************************
@brief Construct a raw ethereum transaction synchronously.

Function: EthSendRawtxWithReceipt()

    This function is similar to EthSendRawtx except that it waits for the
    transaction being mined.
    
@see EthSendRawtx() BoatEthGetTransactionReceipt()


@return
    This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
    of the error codes.
    

@param[in] tx_ptr
        A pointer to the context of the transaction.

*******************************************************************************/
BOAT_RESULT EthSendRawtxWithReceipt(BOAT_INOUT BoatEthTx *tx_ptr)
{
    BOAT_RESULT result = BOAT_ERROR;

    result = EthSendRawtx(tx_ptr);

    if( result == BOAT_SUCCESS )
    {
        result = BoatEthGetTransactionReceipt(tx_ptr);
    }

    return result;
}





