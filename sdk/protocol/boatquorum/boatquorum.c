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
boatquorum.c contains functions to construct a raw transaction, serialize it with RLP,
perform it and wait for its receipt.
*/

#include "boatinternal.h"
#include "web3intf.h"
#include "boatquorum.h"
#include "cJSON.h"

BOAT_RESULT QuorumSendRawtx(BOAT_INOUT BoatQuorumTx *tx_ptr)
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
    
    BSINT32 v_index = 0;
    BSINT32 r_index = 0;
    BSINT32 s_index = 0;
    BSINT32 rlp_index = 0;
    
    RlpEncodedStreamObject *rlp_stream_storage_ptr;

    BCHAR *rlp_stream_hex_str = NULL;// Storage for RLP stream HEX string for use with web3 interface

    BUINT8 message_digest[32];
	BUINT8 message_digestLen;
    BUINT8 sig_parity = 0;
    BUINT32 v;

    Param_web3_sendRawTransaction param_quorum_sendRawTransaction;
    Param_quorum_sendRawPrivateTransaction param_quorum_sendRawPrivateTransaction;

    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Transaction and wallet pointer cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, EthSendRawtx_cleanup);
    }

    // In case the transaction should fail, tx_hash.field_len is initialized to 0
    tx_ptr->tx_hash.field_len = 0;
    
    result = RlpInitListObject(&tx_rlp_object);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Tx RLP object initialize failed.");
        boat_throw(BOAT_ERROR_RLP_LIST_INIT_FAIL, EthSendRawtx_cleanup);
    }

    /************************** Encode nonce end ****************************************/
    result = RlpInitStringObject(&nonce_rlp_object,
                                 tx_ptr->rawtx_fields.nonce.field,
                                 tx_ptr->rawtx_fields.nonce.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Nonce RLP object initialize failed.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
    }
   

    rlp_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &nonce_rlp_object);
    if (rlp_index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Append nonce to Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
    }

     result = RlpEncode(&tx_rlp_object, NULL);

   /************************** Encode nonce end *****************************************/



   /************************** Encode gasprice start ***********************************/
    result = RlpInitStringObject(&gasprice_rlp_object,
                                 tx_ptr->rawtx_fields.gasprice.field,
                                 tx_ptr->rawtx_fields.gasprice.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Gasprice RLP object initialize failed.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
    }
    
    rlp_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &gasprice_rlp_object);
    if (rlp_index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Append gasprice to Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
    }

     result = RlpEncode(&tx_rlp_object, NULL);
    /************************** Encode gasprice end ************************************/


    /************************** Encode gaslimit start *********************************/
    result = RlpInitStringObject(&gaslimit_rlp_object,
                                 tx_ptr->rawtx_fields.gaslimit.field,
                                 tx_ptr->rawtx_fields.gaslimit.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Gaslimit RLP object initialize failed.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
    }
    
    rlp_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &gaslimit_rlp_object);
    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Append gaslimit to Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
    }
      result = RlpEncode(&tx_rlp_object, NULL);
    /************************** Encode gaslimit end *********************************/


    /************************** Encode recipient start *********************************/

    result = RlpInitStringObject(&recipient_rlp_object,
                                 tx_ptr->rawtx_fields.recipient, 20);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Recipient RLP object initialize failed.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
    }
    
    rlp_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &recipient_rlp_object);
    if (rlp_index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Append recipient to Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
    }

    result = RlpEncode(&tx_rlp_object, NULL);
    /************************** Encode recipient end *********************************/

    /************************** Encode value start **********************************/  
    result = RlpInitStringObject(&value_rlp_object,
                                 tx_ptr->rawtx_fields.value.field,
                                 tx_ptr->rawtx_fields.value.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Value RLP object initialize failed.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
    }
    
    rlp_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &value_rlp_object);
    if (rlp_index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Append value to Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
    }

    result = RlpEncode(&tx_rlp_object, NULL);
    /************************** Encode value end **********************************/  

    /************************** Encode data start ****************************/  
    if (tx_ptr->is_private)
    {
        tx_ptr->rawtx_fields.data.field_ptr = BoatMalloc(64);
        if(NULL == tx_ptr->rawtx_fields.data.field_ptr){
            BoatLog(BOAT_LOG_CRITICAL, "Unable to dynamically allocate rawtx_fields.data ");
            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, EthSendRawtx_cleanup);
        }
       UtilityHexToBin(tx_ptr->rawtx_fields.data.field_ptr, 64, (BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr, TRIMBIN_TRIM_NO, BOAT_FALSE);
       tx_ptr->rawtx_fields.data.field_len = 64;   
    }
       
    result = RlpInitStringObject(&data_rlp_object,
                                 tx_ptr->rawtx_fields.data.field_ptr,
                                 tx_ptr->rawtx_fields.data.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Data RLP object initialize failed.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
    }
    
    rlp_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &data_rlp_object);
    if (rlp_index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Append data to Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
    }
     /************************** Encode data end ******************************/   

    /**************************************************************************
    * STEP 1: Construction RAW transaction without real v/r/s                 *
    *         (See above description for details)                             *
    **************************************************************************/
    result = RlpEncode(&tx_rlp_object, NULL);

    // If EIP-155 is required, encode v = chain id, r = s = NULL in this step
    if (tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE)
    {
        // v = Chain ID
        // Currently max chain id supported is (2^32 - 1 - 36)/2, because v is
        // finally calculated as chain_id * 2 + 35 or 36 as per EIP-155.
        v = tx_ptr->wallet_ptr->network_info.chain_id;
        chain_id_len = UtilityUint32ToBigend(tx_ptr->rawtx_fields.v.field,
                                              v, TRIMBIN_LEFTTRIM);
        tx_ptr->rawtx_fields.v.field_len = chain_id_len;
        
        // r = s = NULL
        tx_ptr->rawtx_fields.sig.r_len = 0;
        tx_ptr->rawtx_fields.sig.s_len = 0;
        
        // Encode v
        result = RlpInitStringObject(&v_rlp_object,
                                     tx_ptr->rawtx_fields.v.field,
                                     tx_ptr->rawtx_fields.v.field_len);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_CRITICAL, "V RLP object initialize failed.");
            boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
        }
        
        v_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &v_rlp_object);
        if (v_index < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Append v to Tx RLP object failed.");
            boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
        }

        // Encode r
        result = RlpInitStringObject(&r_rlp_object,
                                     tx_ptr->rawtx_fields.sig.r32B,
                                     tx_ptr->rawtx_fields.sig.r_len);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_CRITICAL, "R RLP object initialize failed.");
            boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
        }
        
        r_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &r_rlp_object);
        if (r_index < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Append r to Tx RLP object failed.");
            boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
        }

        // Encode s
        result = RlpInitStringObject(&s_rlp_object,
                                     tx_ptr->rawtx_fields.sig.s32B,
                                     tx_ptr->rawtx_fields.sig.s_len);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_CRITICAL, "S RLP object initialize failed.");
            boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
        }
        
        s_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &s_rlp_object);
        if (s_index < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Append s to Tx RLP object failed.");
            boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, EthSendRawtx_cleanup);
        }
    }

    // Encode Tx RLP LIST
    result = RlpEncode(&tx_rlp_object, NULL);

    if (result == BOAT_SUCCESS)
    {

        rlp_stream_storage_ptr = RlpGetEncodedStream(&tx_rlp_object);
		BoatLog_hexdump(BOAT_LOG_VERBOSE, "Encoded RLP stream", 
						rlp_stream_storage_ptr->stream_ptr, rlp_stream_storage_ptr->stream_len);
    }
    else
    {

        BoatLog(BOAT_LOG_CRITICAL, "Encode Tx failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, EthSendRawtx_cleanup);
    }

    /**************************************************************************
    * STEP 2: Calculate SHA3 hash of message                                  *
    **************************************************************************/
    // Hash the message
	result = BoatHash(BOAT_HASH_KECCAK256,
					  rlp_stream_storage_ptr->stream_ptr, 
					  rlp_stream_storage_ptr->stream_len, 
					  message_digest, &message_digestLen, NULL);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Execute BoatHash failed.");
        boat_throw(result, EthSendRawtx_cleanup);
    }

    /**************************************************************************
    * STEP 3: Sign the transaction                                            *
    **************************************************************************/
    BoatSignatureResult signatureResultTmp;

	result = BoatSignature(tx_ptr->wallet_ptr->account_info.prikeyCtx, 
						   message_digest, message_digestLen, &signatureResultTmp, NULL);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Execute BoatSignature failed.");
        boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, EthSendRawtx_cleanup);
    }
    // assign signature value
    if (signatureResultTmp.native_format_used)
    {
        memcpy(tx_ptr->rawtx_fields.sig.r32B, &signatureResultTmp.native_sign[0]  ,32);
        memcpy(tx_ptr->rawtx_fields.sig.s32B, &signatureResultTmp.native_sign[32] ,32);
    }

    if (signatureResultTmp.signPrefix_used)
    {
		sig_parity = signatureResultTmp.signPrefix;
    }
	
    // Trim r
    BUINT8 trimed_r[32];
    tx_ptr->rawtx_fields.sig.r_len = UtilityTrimBin(trimed_r, tx_ptr->rawtx_fields.sig.r32B,
													32,	TRIMBIN_LEFTTRIM, BOAT_TRUE);
    memcpy(tx_ptr->rawtx_fields.sig.r32B, trimed_r, tx_ptr->rawtx_fields.sig.r_len);

    // Trim s
    BUINT8 trimed_s[32];
    tx_ptr->rawtx_fields.sig.s_len = UtilityTrimBin(trimed_s,tx_ptr->rawtx_fields.sig.s32B,
													32, TRIMBIN_LEFTTRIM, BOAT_TRUE);
    memcpy(tx_ptr->rawtx_fields.sig.s32B, trimed_s, tx_ptr->rawtx_fields.sig.s_len);
    
    /**************************************************************************
    * STEP 4: Encode full RAW transaction with updated v/r/s                  *
    *         (See above description for details)                             *
    **************************************************************************/
    // Re-encode v
    if (tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE)
    {
        // v = Chain ID * 2 + parity + 35
        v = tx_ptr->wallet_ptr->network_info.chain_id * 2 + sig_parity + 35;
    }
    else
    {
        if (tx_ptr->is_private)
        {
            v = sig_parity + 37;
        }
        else
        {
            v = sig_parity + 27;
        }
    }
        
    chain_id_len = UtilityUint32ToBigend(tx_ptr->rawtx_fields.v.field,
                                         v, TRIMBIN_LEFTTRIM);
    tx_ptr->rawtx_fields.v.field_len = chain_id_len;


    result = RlpInitStringObject(&v_rlp_object,
                                 tx_ptr->rawtx_fields.v.field,
                                 tx_ptr->rawtx_fields.v.field_len);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Re-initialize v RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
    }

    if (tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE)
    {
        v_index = RlpEncoderReplaceObjectInList(&tx_rlp_object, v_index, &v_rlp_object);
    }
    else
    {
        v_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &v_rlp_object);
    }
    
    if (v_index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Replace v in Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_REPLACE_FAIL, EthSendRawtx_cleanup);
    }

    // Re-encode r
    result = RlpInitStringObject(&r_rlp_object,
                                 tx_ptr->rawtx_fields.sig.r32B,
                                 tx_ptr->rawtx_fields.sig.r_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Re-initialize r RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, EthSendRawtx_cleanup);
    }
    
    if (tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE)
    {
        r_index = RlpEncoderReplaceObjectInList(&tx_rlp_object, r_index, &r_rlp_object);
    }
    else
    {
        r_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &r_rlp_object);
    }

    if (r_index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Replace r in Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, EthSendRawtx_cleanup);
    }

    // Re-encode s
    result = RlpInitStringObject(&s_rlp_object,
                                 tx_ptr->rawtx_fields.sig.s32B,
                                 tx_ptr->rawtx_fields.sig.s_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Re-initialize s RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, EthSendRawtx_cleanup);
    }

    if (tx_ptr->wallet_ptr->network_info.eip155_compatibility == BOAT_TRUE)
    {
        s_index = RlpEncoderReplaceObjectInList(&tx_rlp_object, s_index, &s_rlp_object);
    }
    else
    {
        s_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &s_rlp_object);
    }
    
    if (s_index < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Replace s in Tx RLP object failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, EthSendRawtx_cleanup);
    }

    // Re-encode Tx RLP LIST
    result = RlpReEncode(&tx_rlp_object, NULL);
    if (result == BOAT_SUCCESS)
    {   
        rlp_stream_storage_ptr = RlpGetEncodedStream(&tx_rlp_object);
		BoatLog_hexdump(BOAT_LOG_VERBOSE, "Re-Encoded RLP stream", 
						rlp_stream_storage_ptr->stream_ptr, rlp_stream_storage_ptr->stream_len);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Re-encode Tx failed.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, EthSendRawtx_cleanup);
    }
    

    // Allocate memory for RLP stream HEX string
    // It's a storage for HEX string converted from RLP stream binary. The
    // HEX string is used as input for web3. It's in a form of "0x1234ABCD".
    // Where *2 for binary to HEX conversion, +2 for "0x" prefix, + 1 for null terminator.
    rlp_stream_hex_str = BoatMalloc(rlp_stream_storage_ptr->stream_len * 2 + 2 + 1 + 80);
    if (rlp_stream_hex_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to dynamically allocate memory to store RLP HEX string.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, EthSendRawtx_cleanup);
    }

    // To save memory, re-use rlp_stream_hex_str to print debug information
    // As rlp_stream_hex_str's size is enough to hold the entire RLP hex string,
    // thus it's far more than enough to hold any field of the RLP hex string,
    // which is printed.
    
    // Print transaction recipient to log
    if (0 == UtilityBinToHex(rlp_stream_hex_str, tx_ptr->rawtx_fields.recipient, 20, 
							 BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE))
    {
        strcpy(rlp_stream_hex_str, "NULL");
    }

    BoatLog(BOAT_LOG_NORMAL, "Transaction to: %s", rlp_stream_hex_str);

	/* print ethereum transaction message */
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Nonce     )", 
					tx_ptr->rawtx_fields.nonce.field, tx_ptr->rawtx_fields.nonce.field_len);
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Sender    )", 
					tx_ptr->wallet_ptr->account_info.address, 20);
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Recipient )", 
					tx_ptr->rawtx_fields.recipient, 20);
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Value     )", 
					tx_ptr->rawtx_fields.value.field, tx_ptr->rawtx_fields.value.field_len);
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Data      )", 
					tx_ptr->rawtx_fields.data.field_ptr, tx_ptr->rawtx_fields.data.field_len);
    BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(privatefor)", 
                    tx_ptr->rawtx_fields.privatefor, 44);


    UtilityBinToHex(rlp_stream_hex_str, rlp_stream_storage_ptr->stream_ptr, rlp_stream_storage_ptr->stream_len,
				    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);

    if (tx_ptr->is_private)
    {
        param_quorum_sendRawPrivateTransaction.method_name_str = "eth_sendRawPrivateTransaction";
        param_quorum_sendRawPrivateTransaction.signedtx_str    =  rlp_stream_hex_str; 
        param_quorum_sendRawPrivateTransaction.privatefor_str  = "ROAZBWtSacxXQrOe3FGAqJDyJjFePR5ce4TSIzmJ0Bc=";
    
        tx_hash_str = web3_sendRawPrivateTransaction(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                          tx_ptr->wallet_ptr->network_info.node_url_str,
                                          &param_quorum_sendRawPrivateTransaction,&result);
    }
    else
    {
        param_quorum_sendRawTransaction.method_name_str = "eth_sendRawTransaction";
        param_quorum_sendRawTransaction.signedtx_str = rlp_stream_hex_str; 
        tx_hash_str = web3_sendRawTransaction(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                          tx_ptr->wallet_ptr->network_info.node_url_str,
                                          &param_quorum_sendRawTransaction,&result);
    }
  
	if (tx_hash_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to send raw transaction to network.");
		boat_throw(result, EthSendRawtx_cleanup);
    }
    result = quorum_parse_json_result(tx_hash_str, "tx", "input",
								   &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "Fail to parse RPC response.");
		boat_throw(result, EthSendRawtx_cleanup);
	}

    tx_ptr->tx_hash.field_len = UtilityHexToBin(tx_ptr->tx_hash.field, 32, 
												(BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
												TRIMBIN_TRIM_NO, BOAT_FALSE);

    result = BOAT_SUCCESS;

    // Clean Up
    boat_catch(EthSendRawtx_cleanup)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    // Free RLP Objects
    RlpRecursiveDeleteObject(&tx_rlp_object);
    
    // Free RLP hex string buffer
    if (rlp_stream_hex_str != NULL)
    {
        BoatFree(rlp_stream_hex_str);
    }
    
    return result;
}

BOAT_RESULT QuorumSendFilltx(BOAT_INOUT BoatQuorumTx *tx_ptr)
{

    Param_quorun_fillTransaction param_quorum_fillTransaction;
    BCHAR *data_hex = NULL;// Storage for RLP stream HEX string for use with web3 interface
    BCHAR *recipient_hex = NULL;// Storage for RLP stream HEX string for use with web3 interface
    BCHAR *tx_hash_str;

    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Transaction and wallet pointer cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, EthSendRawtx_cleanup);
    }

    data_hex = BoatMalloc(tx_ptr->rawtx_fields.data.field_len * 2 + 2 + 1);
    if (data_hex == NULL)
    {

    }

    recipient_hex = BoatMalloc(BOAT_QUORUM_ADDRESS_SIZE * 2 + 2 +1);
    if (recipient_hex == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to dynamically allocate memory to store RLP HEX string.");
        //boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, EthSendRawtx_cleanup);
    }

    UtilityBinToHex(data_hex, tx_ptr->rawtx_fields.data.field_ptr, tx_ptr->rawtx_fields.data.field_len,
        BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);

    UtilityBinToHex(recipient_hex, tx_ptr->rawtx_fields.recipient, BOAT_QUORUM_ADDRESS_SIZE,
        BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);

  
    param_quorum_fillTransaction.method_name_str = "eth_fillTransaction";
    param_quorum_fillTransaction.data            = data_hex;
    param_quorum_fillTransaction.to              = recipient_hex;
    param_quorum_fillTransaction.privateFor      = &(tx_ptr->rawtx_fields.privatefor[0]);
 
    tx_hash_str = web3_fillTransaction(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                          tx_ptr->wallet_ptr->network_info.node_url_str,
                                          &param_quorum_fillTransaction,&result);
    if (tx_hash_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to send raw transaction to network.");
        boat_throw(result, EthSendRawtx_cleanup);
    }
    result = quorum_parse_json_result(tx_hash_str, "tx", "input",
                                   &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to parse RPC response.");
        boat_throw(result, EthSendRawtx_cleanup);
    }

    // tx_ptr->tx_hash.field_len = UtilityHexToBin(tx_ptr->tx_hash.field, 32, 
    //                                             (BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
    //                                             TRIMBIN_TRIM_NO, BOAT_FALSE);

    result = QuorumSendRawtx(tx_ptr);

    // result = BOAT_SUCCESS;

    // Clean Up
    boat_catch(EthSendRawtx_cleanup)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }
    
    return result;
}


BOAT_RESULT quorum_parse_json_result(const BCHAR *json_string, 
                                  const BCHAR *child_name, 
                                  const BCHAR *child_child_name,
                                  BoatFieldVariable *result_out)
{
    cJSON  *cjson_string_ptr     = NULL;
    cJSON  *cjson_result_ptr     = NULL;
    cJSON  *cjson_child_name_ptr = NULL;
    cJSON  *cjson_child_child_name_ptr = NULL;
    BCHAR  *parse_result_str     = NULL;
    BUINT32 parse_result_str_len;
    const char *cjson_error_ptr;
    
    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
    
    if ((json_string == NULL) || (child_name == NULL) || (result_out == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "parameter should not be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    
    // Convert string to cJSON
    cjson_string_ptr = cJSON_Parse(json_string);
    if (cjson_string_ptr == NULL)
    {
        cjson_error_ptr = cJSON_GetErrorPtr();
        if (cjson_error_ptr != NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Parsing RESPONSE as JSON fails before: %s.", cjson_error_ptr);
        }
        boat_throw(BOAT_ERROR_WEB3_JSON_PARSE_FAIL, quorum_parse_json_result_cleanup);
    }
    
    // Obtain result object
    cjson_result_ptr = cJSON_GetObjectItemCaseSensitive(cjson_string_ptr, "result");
    if (cjson_result_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Cannot find \"result\" item in RESPONSE.");
        boat_throw(BOAT_ERROR_WEB3_JSON_GETOBJ_FAIL, quorum_parse_json_result_cleanup);
    }

    if (cJSON_IsObject(cjson_result_ptr))
    {
        // the "result" object is json item
        cjson_child_name_ptr = cJSON_GetObjectItemCaseSensitive(cjson_result_ptr, child_name);
        if (cjson_child_name_ptr == NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Cannot find \"%s\" item in RESPONSE.", child_name);
            boat_throw(BOAT_ERROR_WEB3_JSON_GETOBJ_FAIL, quorum_parse_json_result_cleanup);
        }

        if (cJSON_IsObject(cjson_child_name_ptr))
        {
            // the "result" object is json item
            cjson_child_child_name_ptr = cJSON_GetObjectItemCaseSensitive(cjson_child_name_ptr, child_child_name);
            if (cjson_child_child_name_ptr == NULL)
            {
                BoatLog(BOAT_LOG_NORMAL, "Cannot find \"%s\" item in RESPONSE.", child_child_name);
                boat_throw(BOAT_ERROR_WEB3_JSON_GETOBJ_FAIL, quorum_parse_json_result_cleanup);
            }

             //parse child_child_name object
            if (cJSON_IsString(cjson_child_child_name_ptr))
            {
                parse_result_str = cJSON_GetStringValue(cjson_child_child_name_ptr);
                if (parse_result_str != NULL)
                {
                    BoatLog(BOAT_LOG_VERBOSE, "result = %s", parse_result_str);

                    parse_result_str_len = strlen(parse_result_str);

                    while(parse_result_str_len >= result_out->field_len)
                    {
                        BoatLog(BOAT_LOG_VERBOSE, "Expand result_out memory...");
                        result = BoatFieldVariable_malloc_size_expand(result_out, WEB3_STRING_BUF_STEP_SIZE);
                        if (result != BOAT_SUCCESS)
                        {
                            BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, quorum_parse_json_result_cleanup);
                        }
                    }
                    strcpy((BCHAR*)result_out->field_ptr, parse_result_str);
                }
             }

        }
         //parse child_name object
        else if (cJSON_IsString(cjson_child_name_ptr))
        {
            parse_result_str = cJSON_GetStringValue(cjson_child_name_ptr);
            if (parse_result_str != NULL)
            {
                BoatLog(BOAT_LOG_VERBOSE, "result = %s", parse_result_str);

                parse_result_str_len = strlen(parse_result_str);

                while(parse_result_str_len >= result_out->field_len)
                {
                    BoatLog(BOAT_LOG_VERBOSE, "Expand result_out memory...");
                    result = BoatFieldVariable_malloc_size_expand(result_out, WEB3_STRING_BUF_STEP_SIZE);
                    if (result != BOAT_SUCCESS)
                    {
                        BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, quorum_parse_json_result_cleanup);
                    }
                }
                strcpy((BCHAR*)result_out->field_ptr, parse_result_str);
            }
        }

    }
       
    
    else if (cJSON_IsString(cjson_result_ptr))
    {
        parse_result_str = cJSON_GetStringValue(cjson_result_ptr);
        
        if (parse_result_str != NULL)
        {
            BoatLog(BOAT_LOG_VERBOSE, "result = %s", parse_result_str);

            parse_result_str_len = strlen(parse_result_str);
            while(parse_result_str_len >= result_out->field_len)
            {
                BoatLog(BOAT_LOG_VERBOSE, "Expand result_out memory...");
                result = BoatFieldVariable_malloc_size_expand(result_out, WEB3_STRING_BUF_STEP_SIZE);
                if (result != BOAT_SUCCESS)
                {
                    BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                    boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, quorum_parse_json_result_cleanup);
                }
            }
            strcpy((BCHAR*)result_out->field_ptr, parse_result_str);
        }
    }
    else if (cJSON_IsNull(cjson_result_ptr))//cjson_result_ptr:null
    {
        BoatLog(BOAT_LOG_VERBOSE, "Result is NULL.");
        boat_throw(BOAT_ERROR_JSON_OBJ_IS_NULL, quorum_parse_json_result_cleanup);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Un-expect object type.");
        boat_throw(BOAT_ERROR_WEB3_JSON_PARSE_FAIL, quorum_parse_json_result_cleanup);
    }
    if (cjson_string_ptr != NULL)
    {
        cJSON_Delete(cjson_string_ptr);
    }
    
    // Exceptional Clean Up
    boat_catch(quorum_parse_json_result_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);

        if (cjson_string_ptr != NULL)
        {
            cJSON_Delete(cjson_string_ptr);
        }

        result = boat_exception;
    }
    
    return result;
}                        


