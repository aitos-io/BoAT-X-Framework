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
boatfiscobcos.c contains functions to construct a raw transaction, serialize it with RLP,
perform it and wait for its receipt.
*/

#include "boatconfig.h"
#include "boatinternal.h"

#if PROTOCOL_USE_FISCOBCOS == 1
#include "web3intf.h"
#include "boatethereum.h"
#include "boatfiscobcos.h"


BOAT_RESULT FiscobcosSendRawtx(BOAT_INOUT BoatFiscobcosTx *tx_ptr)
{   
    BCHAR *tx_hash_str;

    RlpObject tx_rlp_object;
    RlpObject nonce_rlp_object;
    RlpObject gasprice_rlp_object;
    RlpObject gaslimit_rlp_object;
	RlpObject blocklimit_rlp_object;
    RlpObject recipient_rlp_object;
    RlpObject value_rlp_object;
    RlpObject data_rlp_object;
	RlpObject chainid_rlp_object;
	RlpObject groupid_rlp_object;
	RlpObject extraData_rlp_object;

    RlpObject v_rlp_object;
    RlpObject r_rlp_object;
    RlpObject s_rlp_object;
    
    BSINT32 v_index = 0;
    BSINT32 r_index = 0;
    BSINT32 s_index = 0;
    
    RlpEncodedStreamObject *rlp_stream_storage_ptr;
    
	/* Storage for RLP stream HEX string for use with web3 interface */
    BCHAR *rlp_stream_hex_str = NULL;
    
    BUINT8 message_digest[32];
	BUINT8 message_digestLen;
    BUINT8 sig_parity = 0;
    BUINT32 v;

    Param_fiscobcos_sendRawTransaction param_fiscobcos_sendRawTransaction;

    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
   
    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "<tx_ptr> cannot be null.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, FiscobcosSendRawtx_cleanup);
    }

    // In case the transaction should fail, tx_hash.field_len is initialized to 0
    tx_ptr->tx_hash.field_len = 0;
 
    result = RlpInitListObject(&tx_rlp_object);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize Tx RLP objecte.");
        boat_throw(BOAT_ERROR_RLP_LIST_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }

    /**************************************************************************
    * STEP 1: Construction RAW transaction without real v/r/s                 *
    **************************************************************************/
    // Encode nonce
    result = RlpInitStringObject(&nonce_rlp_object,
                                 tx_ptr->rawtx_fields.nonce.field,
                                 tx_ptr->rawtx_fields.nonce.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize nonce RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &nonce_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append nonce to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }

    // Encode gasprice
    result = RlpInitStringObject(&gasprice_rlp_object,
                                 tx_ptr->rawtx_fields.gasprice.field,
                                 tx_ptr->rawtx_fields.gasprice.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize gasprice RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &gasprice_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append gasprice to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    // Encode gaslimit
    result = RlpInitStringObject(&gaslimit_rlp_object,
                                 tx_ptr->rawtx_fields.gaslimit.field,
                                 tx_ptr->rawtx_fields.gaslimit.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize gaslimit RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &gaslimit_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append gaslimit to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }
	
	// Encode blocklimit
    result = RlpInitStringObject(&blocklimit_rlp_object,
                                 tx_ptr->rawtx_fields.blocklimit.field,
                                 tx_ptr->rawtx_fields.blocklimit.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize blocklimit RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &blocklimit_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append blocklimit to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    // Encode recipient
    result = RlpInitStringObject(&recipient_rlp_object,
                                 tx_ptr->rawtx_fields.recipient, 
								 BOAT_FISCOBCOS_ADDRESS_SIZE);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize recipient RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &recipient_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append recipient to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }


    // Encode value
    result = RlpInitStringObject(&value_rlp_object,
                                 tx_ptr->rawtx_fields.value.field,
                                 tx_ptr->rawtx_fields.value.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize value RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &value_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append value to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }

    	BoatLog_hexdump( BOAT_LOG_VERBOSE, "tx_ptr->rawtx_fields.data.field :", 
					 tx_ptr->rawtx_fields.data.field_ptr, tx_ptr->rawtx_fields.data.field_len);
    // Encode data
    result = RlpInitStringObject(&data_rlp_object,
                                 tx_ptr->rawtx_fields.data.field_ptr,
                                 tx_ptr->rawtx_fields.data.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize data RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &data_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append data to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }
	
	// Encode chainid
    result = RlpInitStringObject(&chainid_rlp_object,
                                 tx_ptr->rawtx_fields.chainid.field,
                                 tx_ptr->rawtx_fields.chainid.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize chainid RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &chainid_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append chainid to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }
	
	// Encode groupid
    result = RlpInitStringObject(&groupid_rlp_object,
                                 tx_ptr->rawtx_fields.groupid.field,
                                 tx_ptr->rawtx_fields.groupid.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize groupid RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &groupid_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append groupid to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }
	
	// Encode extraData
	tx_ptr->rawtx_fields.extraData.field_len = 0;
    result = RlpInitStringObject(&extraData_rlp_object,
                                 tx_ptr->rawtx_fields.extraData.field_ptr,
                                 tx_ptr->rawtx_fields.extraData.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to initialize extraData RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    result = RlpEncoderAppendObjectToList(&tx_rlp_object, &extraData_rlp_object);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to append extraData to Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
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
        BoatLog(BOAT_LOG_NORMAL, "Fail to encode Tx.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, FiscobcosSendRawtx_cleanup);
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
        BoatLog(BOAT_LOG_CRITICAL, "Fail to BoatHash signature.");
        boat_throw(result, FiscobcosSendRawtx_cleanup);
    }

    /**************************************************************************
    * STEP 3: Sign the transaction                                            *
    **************************************************************************/
    BoatSignatureResult signatureResultTmp;

	result = BoatSignature(tx_ptr->wallet_ptr->account_info.prikeyCtx, 
						   message_digest, message_digestLen, &signatureResultTmp, NULL);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to BoatSignature signature.");
        boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, FiscobcosSendRawtx_cleanup);
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
												    32, TRIMBIN_LEFTTRIM, BOAT_TRUE);
    memcpy(tx_ptr->rawtx_fields.sig.r32B, trimed_r, tx_ptr->rawtx_fields.sig.r_len);

    // Trim s
    BUINT8 trimed_s[32];
    tx_ptr->rawtx_fields.sig.s_len = UtilityTrimBin(trimed_s, tx_ptr->rawtx_fields.sig.s32B,
													32, TRIMBIN_LEFTTRIM, BOAT_TRUE);
    memcpy(tx_ptr->rawtx_fields.sig.s32B, trimed_s, tx_ptr->rawtx_fields.sig.s_len);
    
    /**************************************************************************
    * STEP 4: Encode full RAW transaction with updated v/r/s                  *
    **************************************************************************/
    // Re-encode v
	v = sig_parity + 27;    
    tx_ptr->rawtx_fields.v.field_len = UtilityUint32ToBigend(tx_ptr->rawtx_fields.v.field,
															 v, TRIMBIN_LEFTTRIM);
    result = RlpInitStringObject(&v_rlp_object,
                                 tx_ptr->rawtx_fields.v.field,
                                 tx_ptr->rawtx_fields.v.field_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to re-initialize v RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }

	v_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &v_rlp_object);  
    if (v_index < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to replace v in Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }

    // Re-encode r
    result = RlpInitStringObject(&r_rlp_object,
                                 tx_ptr->rawtx_fields.sig.r32B,
                                 tx_ptr->rawtx_fields.sig.r_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to re-initialize r RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }

	r_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &r_rlp_object);
    if (r_index < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to replace r in Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
    }

    // Re-encode s
    result = RlpInitStringObject(&s_rlp_object,
                                 tx_ptr->rawtx_fields.sig.s32B,
                                 tx_ptr->rawtx_fields.sig.s_len);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to re-initialize s RLP object.");
        boat_throw(BOAT_ERROR_RLP_STRING_INIT_FAIL, FiscobcosSendRawtx_cleanup);
    }

	s_index = RlpEncoderAppendObjectToList(&tx_rlp_object, &s_rlp_object); 
    if (s_index < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to replace s in Tx RLP object.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_APPEND_FAIL, FiscobcosSendRawtx_cleanup);
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
        BoatLog(BOAT_LOG_CRITICAL, "Fail to re-encode Tx.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, FiscobcosSendRawtx_cleanup);
    }
    
    // Allocate memory for RLP stream HEX string
    // It's a storage for HEX string converted from RLP stream binary. The
    // HEX string is used as input for web3. It's in a form of "0x1234ABCD".
    // Where *2 for binary to HEX conversion, +2 for "0x" prefix, + 1 for null terminator.
    rlp_stream_hex_str = BoatMalloc(rlp_stream_storage_ptr->stream_len * 2 + 2 + 1);
    if (rlp_stream_hex_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to dynamically allocate memory to store RLP HEX string.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, FiscobcosSendRawtx_cleanup);
    }

    // To save memory, re-use rlp_stream_hex_str to print debug information
    // As rlp_stream_hex_str's size is enough to hold the entire RLP hex string,
    // thus it's far more than enough to hold any field of the RLP hex string,
    // which is printed.
    
    // Print transaction recipient to log
    if (0 == UtilityBinToHex(rlp_stream_hex_str, tx_ptr->rawtx_fields.recipient, BOAT_FISCOBCOS_ADDRESS_SIZE,
							 BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE))
    {
        strcpy(rlp_stream_hex_str, "NULL");
    }

    BoatLog(BOAT_LOG_NORMAL, "Transaction to: %s", rlp_stream_hex_str);

	/* print fiscobcos transaction message */
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Nonce    )", 
					tx_ptr->rawtx_fields.nonce.field, tx_ptr->rawtx_fields.nonce.field_len);
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Sender   )", 
					tx_ptr->wallet_ptr->account_info.address, BOAT_FISCOBCOS_ADDRESS_SIZE);
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Recipient)", 
					tx_ptr->rawtx_fields.recipient, BOAT_FISCOBCOS_ADDRESS_SIZE);
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Value    )", 
					tx_ptr->rawtx_fields.value.field, tx_ptr->rawtx_fields.value.field_len);
	BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Data     )", 
					tx_ptr->rawtx_fields.data.field_ptr, tx_ptr->rawtx_fields.data.field_len);

	//set signedtx
    UtilityBinToHex(rlp_stream_hex_str,
					rlp_stream_storage_ptr->stream_ptr, rlp_stream_storage_ptr->stream_len,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_fiscobcos_sendRawTransaction.signedtx_str = rlp_stream_hex_str;
	
	//set groupid
	BCHAR groupid_hexstr[67];
	memset(groupid_hexstr, 0, sizeof(groupid_hexstr));
	UtilityBinToHex(groupid_hexstr, tx_ptr->rawtx_fields.groupid.field, tx_ptr->rawtx_fields.groupid.field_len,
					BIN2HEX_LEFTTRIM_QUANTITY, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE);
	param_fiscobcos_sendRawTransaction.groupid = groupid_hexstr;
	
    tx_hash_str = web3_fiscobcos_sendRawTransaction(tx_ptr->wallet_ptr->web3intf_context_ptr,
												    tx_ptr->wallet_ptr->network_info.node_url_ptr,
												    &param_fiscobcos_sendRawTransaction,&result);
    if (tx_hash_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to send raw transaction to network.");
		boat_throw(result, FiscobcosSendRawtx_cleanup);
    }

    result = BoatFiscobcosPraseRpcResponseStringResult(tx_hash_str,
											           &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
    if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "Fail to prase RPC response.");
		boat_throw(result, FiscobcosSendRawtx_cleanup);
	}                                        

    tx_ptr->tx_hash.field_len = UtilityHexToBin(tx_ptr->tx_hash.field, 32, 
								                (BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr, 
                                                TRIMBIN_TRIM_NO, BOAT_FALSE);

    result = BOAT_SUCCESS;

    // Clean Up
    boat_catch(FiscobcosSendRawtx_cleanup)
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

#endif /* end of PROTOCOL_USE_FISCOBCOS */
