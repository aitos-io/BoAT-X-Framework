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
boatcita.c contains functions to construct a raw transaction, serialize it with protobuf,
perform it and wait for its receipt.
*/

#include "boatinternal.h"
#include "web3intf.h"
#include "boatcita.h"
#include "cJSON.h"
#include "blockchain.pb-c.h"


BOAT_RESULT CitaSendRawtx(BOAT_INOUT BoatCitaTx *tx_ptr)
{
    BCHAR *tx_hash_str;

    BUINT8 sig_parity = 0;
    BUINT8 sign_result[65] = {0};
    BUINT8 message_digest[32];
    BUINT8 message_digestLen;
    BCHAR *probuf_hex_str = NULL;
    BCHAR *probuf_nonce_str = NULL;
    BUINT8  *packed_trans_unverifiled_buf = NULL;
    BUINT32 packed_trans_unverifiled_length;
    BUINT32 packed_trans_length;
    Param_cita_sendRawTransaction param_cita_sendRawTransaction;

    Transaction transaction = TRANSACTION__INIT;
    UnverifiedTransaction unverified_transaction = UNVERIFIED_TRANSACTION__INIT;
    BoatFieldVariable hash_data = {NULL, 0};

    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Transaction and wallet pointer cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, CitaSendRawtx_cleanup);
    }

    /**************************************************************************
    * STEP 1: Construct RAW transaction without real v/r/s                 *
    *         (See above description for details)                             *
    **************************************************************************/
    // Encode nonce
    probuf_nonce_str = BoatMalloc(16 * 2 + 1);
    if (probuf_nonce_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to dynamically allocate memory to store protobuf HEX string.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, CitaSendRawtx_cleanup);
    }
    UtilityBinToHex(probuf_nonce_str,
                (char*)tx_ptr->rawtx_fields.nonce.field, 16,
                BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE);
    
    transaction.nonce = probuf_nonce_str;
    transaction.quota = tx_ptr->rawtx_fields.quota;
    transaction.valid_until_block = tx_ptr->rawtx_fields.valid_until_block;

    //data
    transaction.data.data = tx_ptr->rawtx_fields.data.field_ptr;
    transaction.data.len  = tx_ptr->rawtx_fields.data.field_len;

    //value
    transaction.value.data = tx_ptr->rawtx_fields.value.field;
    transaction.value.len = tx_ptr->rawtx_fields.value.field_len;

    //version
    transaction.version = tx_ptr->wallet_ptr->network_info.version;

    //to_v1 
    transaction.to_v1.data = tx_ptr->rawtx_fields.recipient;
    transaction.to_v1.len  = BOAT_CITA_ADDRESS_SIZE;

    //chainid
    transaction.chain_id_v1.data = tx_ptr->wallet_ptr->network_info.chain_id_buf;
    transaction.chain_id_v1.len  = 32;

    packed_trans_length = transaction__get_packed_size(&transaction);
    hash_data.field_ptr = BoatMalloc(packed_trans_length);
    if (hash_data.field_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Failed to allocate memory.");
        boat_throw(result, CitaSendRawtx_cleanup);;
    }

    hash_data.field_len = packed_trans_length;
    transaction__pack(&transaction, hash_data.field_ptr);

    /**************************************************************************
    * STEP 2: Calculate SHA3 hash of message                                  *
    **************************************************************************/
     // Hash the message
    result = BoatHash(BOAT_HASH_KECCAK256,hash_data.field_ptr, 
                       hash_data.field_len, message_digest, &message_digestLen, NULL);

    if (result != BOAT_SUCCESS) 
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
        boat_throw(result, CitaSendRawtx_cleanup);

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
        boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, CitaSendRawtx_cleanup);
    }

    /**************************************************************************
    * STEP 4: Encode full RAW transaction with updated /r/s/v                  *
    **************************************************************************/

    // Trim r
    BUINT8 trimed_r[32] = {0};
    BUINT8 trimed_r_len = UtilityTrimBin(trimed_r, &signatureResultTmp.native_sign[0],
                                                    32, TRIMBIN_LEFTTRIM, BOAT_TRUE);
    memcpy(sign_result, trimed_r, trimed_r_len);
    // Trim s
    BUINT8 trimed_s[32] = {0};
    BUINT8 trimed_s_len = UtilityTrimBin(trimed_s, &signatureResultTmp.native_sign[32],
                                                    32, TRIMBIN_LEFTTRIM, BOAT_TRUE);
    memcpy(&sign_result[32], trimed_s, trimed_s_len);

    // v
    BUINT8 v = signatureResultTmp.signPrefix;
    sign_result[64] = v;
    
    /**************************************************************************
    * SETP 4: pack the envelope                                               *
    **************************************************************************/
    unverified_transaction.transaction    = &transaction;
    unverified_transaction.signature.data = sign_result;
    unverified_transaction.signature.len  = 65;
    unverified_transaction.crypto         = 0;
    packed_trans_unverifiled_length       = unverified_transaction__get_packed_size(&unverified_transaction);
    packed_trans_unverifiled_buf          = BoatMalloc(packed_trans_unverifiled_length);
    if (packed_trans_unverifiled_buf == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Failed to allocate memory.");
        boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, CitaSendRawtx_cleanup);;
    }

    unverified_transaction__pack(&unverified_transaction, packed_trans_unverifiled_buf);
    probuf_hex_str = BoatMalloc(packed_trans_unverifiled_length * 2 + 2 + 1);
    if (probuf_hex_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to dynamically allocate memory to store protobuf HEX string.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, CitaSendRawtx_cleanup);
    }
    
    UtilityBinToHex(probuf_hex_str,
                packed_trans_unverifiled_buf, packed_trans_unverifiled_length,
                BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);

    /* print cita transaction message */
    BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(sign     )", 
                    unverified_transaction.signature.data, unverified_transaction.signature.len);
    /* print cita transaction message */
    BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Nonce    )", 
                    tx_ptr->rawtx_fields.nonce.field, tx_ptr->rawtx_fields.nonce.field_len);
    BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Sender   )", 
                    tx_ptr->wallet_ptr->account_info.address, BOAT_CITA_ADDRESS_SIZE);
    BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Recipient)", 
                    tx_ptr->rawtx_fields.recipient, BOAT_CITA_ADDRESS_SIZE);
    BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Value    )", 
                    tx_ptr->rawtx_fields.value.field, tx_ptr->rawtx_fields.value.field_len);
    BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(Data     )", 
                    tx_ptr->rawtx_fields.data.field_ptr, tx_ptr->rawtx_fields.data.field_len);
    BoatLog_hexdump(BOAT_LOG_VERBOSE, "Transaction Message(chain_id )", 
                     transaction.chain_id_v1.data,  transaction.chain_id_v1.len);
    
    param_cita_sendRawTransaction.signedtx_str = probuf_hex_str;
    tx_hash_str = web3_cita_sendRawTransaction(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                                    tx_ptr->wallet_ptr->network_info.node_url_buf,
                                                    &param_cita_sendRawTransaction,&result);
    if (tx_hash_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to send raw transaction to network.");
        boat_throw(result, CitaSendRawtx_cleanup);
    }

    result = BoatCitaParseRpcResponseResult(tx_hash_str, "hash",
                                                       &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to parse RPC response.");
        boat_throw(result, CitaSendRawtx_cleanup);
    }                                        

    tx_ptr->tx_hash.field_len = UtilityHexToBin(tx_ptr->tx_hash.field, 32, 
                                                (BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr, 
                                                TRIMBIN_TRIM_NO, BOAT_FALSE);

    result = BOAT_SUCCESS;

    // Clean Up
    boat_catch(CitaSendRawtx_cleanup)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    /* free malloc */
    if (packed_trans_unverifiled_buf != NULL) 
    {
        BoatFree(packed_trans_unverifiled_buf);
    }

    if (hash_data.field_ptr != NULL) 
    {
        BoatFree(hash_data.field_ptr);
    }

    if (probuf_hex_str != NULL) 
    {
        BoatFree(probuf_hex_str);
    }

    if (probuf_nonce_str != NULL) 
    {
        BoatFree(probuf_nonce_str);
    }

    return result;
}


BOAT_RESULT cita_parse_json_result(const BCHAR *json_string, 
                                  const BCHAR *child_name, 
                                  BoatFieldVariable *result_out)
{
    cJSON  *cjson_string_ptr     = NULL;
    cJSON  *cjson_result_ptr     = NULL;
    cJSON  *cjson_child_name_ptr = NULL;
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
        boat_throw(BOAT_ERROR_WEB3_JSON_PARSE_FAIL, cita_parse_json_result_cleanup);
    }
    
    // Obtain result object
    cjson_result_ptr = cJSON_GetObjectItemCaseSensitive(cjson_string_ptr, "result");
    if (cjson_result_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Cannot find \"result\" item in RESPONSE.");
        boat_throw(BOAT_ERROR_WEB3_JSON_GETOBJ_FAIL, cita_parse_json_result_cleanup);
    }

    if (cJSON_IsObject(cjson_result_ptr))
    {
        // the "result" object is json item
        cjson_child_name_ptr = cJSON_GetObjectItemCaseSensitive(cjson_result_ptr, child_name);
        if (cjson_child_name_ptr == NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Cannot find \"%s\" item in RESPONSE.", child_name);
            boat_throw(BOAT_ERROR_WEB3_JSON_GETOBJ_FAIL, cita_parse_json_result_cleanup);
        }
    
        //parse child_name object
        if (cJSON_IsString(cjson_child_name_ptr))
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
                        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, cita_parse_json_result_cleanup);
                    }
                }
                strcpy((BCHAR*)result_out->field_ptr, parse_result_str);
            }
        }
        else
        {
            strcpy((BCHAR*)result_out->field_ptr, "null");
            BoatLog(BOAT_LOG_NORMAL, "error info is null");
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
                    boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, cita_parse_json_result_cleanup);
                }
            }
            strcpy((BCHAR*)result_out->field_ptr, parse_result_str);
        }
    }
    else if (cJSON_IsNull(cjson_result_ptr))//cjson_result_ptr:null
    {
        BoatLog(BOAT_LOG_VERBOSE, "Result is NULL.");
        boat_throw(BOAT_ERROR_JSON_OBJ_IS_NULL, cita_parse_json_result_cleanup);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Un-expect object type.");
        boat_throw(BOAT_ERROR_WEB3_JSON_PARSE_FAIL, cita_parse_json_result_cleanup);
    }
    if (cjson_string_ptr != NULL)
    {
        cJSON_Delete(cjson_string_ptr);
    }
    
    // Exceptional Clean Up
    boat_catch(cita_parse_json_result_cleanup)
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
