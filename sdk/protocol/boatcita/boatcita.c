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
#include "boatethereum.h"
#include "cJSON.h"
#include "blockchain.pb-c.h"


BOAT_RESULT CitaSendRawtx(BOAT_INOUT BoatCitaTx *tx_ptr)
{
    unsigned int chain_id_len;
        
    BCHAR *tx_hash_str;

    BUINT8 message_digest[32];
    BUINT8 message_digestLen;
    BUINT8 hash_result[64];
    BCHAR *probuf_hex_str = NULL;
    BUINT32  packed_trans_unverifiled_length;
    BUINT32  packed_trans_length;
    BoatSignatureResult signatureResult;
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
    transaction.to    = tx_ptr->rawtx_fields.recipient;
    transaction.nonce = tx_ptr->rawtx_fields.nonce.field;
    transaction.quota = tx_ptr->rawtx_fields.quota;
    transaction.valid_until_block = tx_ptr->rawtx_fields.valid_until_block
    transaction.version = tx_ptr->rawtx_fields.version

    transaction.to_v1.data = tx_ptr->rawtx_fields.to_v1.field;
    transaction.to_v1.len  = tx_ptr->rawtx_fields.to_v1.field_len;

    transaction.chain_id_v1.data = tx_ptr->rawtx_fields.chain_id_v1.field;
    transaction.chain_id_v1.len  = tx_ptr->rawtx_fields.chain_id_v1.field_len;

    packed_trans_length = transaction__get_packed_size(&transaction);
    hash_data.field_ptr = BoatMalloc(packed_trans_length);
    if (hash_data.field_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Failed to allocate memory.");
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    hash_data.field_len = packed_trans_length;

    transaction__pack(&transaction, hash_data.field_ptr);

    /**************************************************************************
    * STEP 2: Calculate SHA3 hash of message                                  *
    **************************************************************************/

     // Hash the message
    result = BoatHash(BOAT_HASH_SHA256,hash_data.field_ptr, 
                       hash_data.field_len, hash_result, NULL, NULL);

    if (result != BOAT_SUCCESS) 
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
        boat_throw(result, CitaSendRawtx_cleanup);
    }

     /**************************************************************************
    * STEP 3: Sign the transaction                                            *
    **************************************************************************/
    result = BoatSignature(tx_ptr->wallet_ptr->user_cert_prikey_info.prikeyCtx, hash_result, sizeof(hash), &signatureResult, NULL);

    if (result != BOAT_SUCCESS) 
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
        boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, CitaSendRawtx_cleanup);
    }

    if (!signatureResult.pkcs_format_used) 
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
        boat_throw(BOAT_ERROR_COMMON_GEN_SIGN_FAIL, CitaSendRawtx_cleanup);
    }

    /**************************************************************************
    * SETP 4: pack the envelope                                               *
    **************************************************************************/
    unverified_transaction.transaction    = &transaction;
    unverified_transaction.signature.data = signatureResult.pkcs_sign;
    unverified_transaction.signature.len  = signatureResult.pkcs_sign_length;
    unverified_transaction.crypto         = 0;
    packed_trans_unverifiled_length       = unverified_transaction__get_packed_size(&unverified_transaction);

    probuf_hex_str = BoatMalloc(packed_trans_unverifiled_length);
    if (probuf_hex_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Failed to allocate memory.");
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    unverified_transaction__pack(&unverified_transaction, probuf_hex_str);


    /* print cita transaction message */
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
    
    param_cita_sendRawTransaction.signedtx_str = probuf_hex_str;
    tx_hash_str = web3_cita_sendRawTransaction(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                                    tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                                    &param_cita_sendRawTransaction,&result);
    if (tx_hash_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to send raw transaction to network.");
        boat_throw(result, CitaSendRawtx_cleanup);
    }

    result = BoatCitaParseRpcResponseStringResult(tx_hash_str,
                                                       &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to parse RPC response.");
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

    /* free malloc */
    if (hash_data.field_ptr != NULL) 
    {
        BoatFree(hash_data.field_ptr);
    }

    if (probuf_hex_str != NULL) 
    {
        BoatFree(probuf_hex_str);
    }

    return result;
}


#endif /* end of PROTOCOL_USE_CITA */

