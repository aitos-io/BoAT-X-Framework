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

/*!@brief cita wallet API for BoAT IoT SDK

@file
api_cita.c defines the cita wallet API for BoAT IoT SDK.
*/

#include "boatinternal.h"
#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"
#include "cJSON.h"

BoatCitaWallet *BoatCitaWalletInit(const BoatCitaWalletConfig *config_ptr, BUINT32 config_size)
{
    BoatCitaWallet *wallet_ptr;
    BOAT_RESULT result;
    BUINT8 pubkeyHash[32];
    BUINT8 hashLenDummy;

    if (config_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return NULL;
    }

    if (sizeof(BoatCitaWalletConfig) != config_size)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Incorrect configuration size.");
        return NULL;
    }
    
    wallet_ptr = BoatMalloc(sizeof(BoatCitaWallet));
    if (wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet memory malloc falied.");
        return NULL;
    }
        
    /* Init Web3 interface */
    wallet_ptr->web3intf_context_ptr = web3_init();

    if (wallet_ptr->web3intf_context_ptr == NULL)
    {
        BoatFree(wallet_ptr);
        BoatLog(BOAT_LOG_CRITICAL, "web3 interface initialization falied.");
        return NULL;
    }
    //Configure priKey context information
    if (BOAT_SUCCESS != BoatPort_keyCreate(&config_ptr->prikeyCtx_config, &wallet_ptr->account_info.prikeyCtx))
    {
        web3_deinit(wallet_ptr->web3intf_context_ptr);
        BoatFree(wallet_ptr);
        BoatLog(BOAT_LOG_CRITICAL, "Failed to exec BoatPort_keyCreate.");
        return NULL;
    }

    // Configure account address    
    BoatHash(BOAT_HASH_SHA256, wallet_ptr->account_info.prikeyCtx.pubkey_content, 
             64, pubkeyHash, &hashLenDummy, NULL);
    memcpy(wallet_ptr->account_info.address, &pubkeyHash[32 - BOAT_CITA_ADDRESS_SIZE], BOAT_CITA_ADDRESS_SIZE);
    // Configure node URL string
    wallet_ptr->network_info.node_url_ptr = NULL;
    result = BoatCitaWalletSetNodeUrl(wallet_ptr, config_ptr->node_url_str);
    if (result != BOAT_SUCCESS)
    {
        web3_deinit(wallet_ptr->web3intf_context_ptr);
        BoatFree(wallet_ptr);
        BoatLog(BOAT_LOG_CRITICAL, "wallet set nodeURL failed.");
        return NULL;
    }
    return wallet_ptr;
}

BOAT_RESULT BoatCitaTxInit(BoatCitaWallet *wallet_ptr,
                                BoatCitaTx *tx_ptr,
                                BBOOL is_sync_tx,
                                BCHAR *recipient_str,
                                BCHAR *chainid_str,
                                BUINT64 quota)
{
    BCHAR   *retval_str = NULL;
    BOAT_RESULT result;

    if ((wallet_ptr == NULL) || (tx_ptr == NULL) || (recipient_str == NULL) || (chainid_str == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;
    memset(&tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields));

    // Set synchronous transaction flag
    tx_ptr->is_sync_tx = is_sync_tx;

    // Initialize recipient
    BUINT32 converted_len;
    BUINT8  recipient[BOAT_CITA_ADDRESS_SIZE];
    converted_len = UtilityHexToBin(recipient, BOAT_CITA_ADDRESS_SIZE, 
                                    recipient_str, TRIMBIN_TRIM_NO, BOAT_TRUE);
    if (converted_len == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "recipient Initialize failed.");
        return BOAT_ERROR_COMMON_UTILITY;
    }

    result = BoatCitaTxSetRecipient(tx_ptr, recipient);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatFiscobcosTxSetRecipient failed.");
        return result;
    }
    
    //chain_id_v1
    tx_ptr->rawtx_fields.chain_id_v1.field_len    = UtilityHexToBin(tx_ptr->rawtx_fields.chain_id_v1.field, 32, 
                                                                chainid_str, TRIMBIN_LEFTTRIM, BOAT_TRUE);
    if (tx_ptr->rawtx_fields.chain_id_v1.field_len == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "chain_id_v1 Initialize failed.");
        return BOAT_ERROR_COMMON_UTILITY;
    }
    
    //quota 
    tx_ptr->rawtx_fields.quota = quota;

    // less than current blocknumber plus 100.
    retval_str = BoatCitaGetBlockNumber(tx_ptr);
    result     = BoatCitaParseRpcResponseStringResult(retval_str, 
                                                           &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatCitaGetBlockNumber failed.");
        return result;
    }
    BUINT8 Block_bumber[8] = {0};

    UtilityHexToBin(Block_bumber, 32, 
                    (BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
                    TRIMBIN_TRIM_NO, BOAT_TRUE);



      
    BoatLog(BOAT_LOG_CRITICAL, "1111111111 = %s\n", (BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr);

    
    tx_ptr->rawtx_fields.valid_until_block = 0x258c;
    BoatLog(BOAT_LOG_CRITICAL, "3333333333333 = %x\n",  tx_ptr->rawtx_fields.valid_until_block);

    // Initialize value = 0
    //CITA DOES NOT SET VALUE, IT'S DE-COINIZED
    result = BoatCitaTxSetValue(tx_ptr, NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatCitaTxSetValue failed.");
        return result;
    }

    return BOAT_SUCCESS;
}



void BoatCitaWalletDeInit(BoatCitaWallet *wallet_ptr)
{
    if (wallet_ptr != NULL)
    {
        if (wallet_ptr->network_info.node_url_ptr != NULL)
        {
            BoatFree(wallet_ptr->network_info.node_url_ptr);
            wallet_ptr->network_info.node_url_ptr = NULL;
        }

        web3_deinit(wallet_ptr->web3intf_context_ptr);

        BoatFree(wallet_ptr);
    }
}

BOAT_RESULT BoatCitaTxSetQuotaLimit(BoatCitaTx *tx_ptr, BUINT64 quota_limit_value)
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set quotaLimit
    tx_ptr->rawtx_fields.quota = quota_limit_value;
}


BOAT_RESULT BoatCitaWalletSetNodeUrl(BoatCitaWallet *wallet_ptr, const BCHAR *node_url_ptr)
{
    BOAT_RESULT result;
    
    if ((wallet_ptr == NULL) || (node_url_ptr == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr or node_url_ptr cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // string length check
    if (BOAT_SUCCESS != UtilityStringLenCheck(node_url_ptr))
    {
        BoatLog(BOAT_LOG_CRITICAL, "node URL length out of limit: %s.", node_url_ptr);
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
	
    // Set Node URL
	if (wallet_ptr->network_info.node_url_ptr != NULL)
	{
		BoatFree(wallet_ptr->network_info.node_url_ptr);
	}

	// +1 for NULL Terminator
	wallet_ptr->network_info.node_url_ptr = BoatMalloc(strlen(node_url_ptr) + 1);
	if (wallet_ptr->network_info.node_url_ptr == NULL)
    {
		BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate memory for Node URL string.");
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    else
	{
		strcpy(wallet_ptr->network_info.node_url_ptr, node_url_ptr);
		result = BOAT_SUCCESS;
	}

    return result;
}


BOAT_RESULT BoatCitaTxSetRecipient(BoatCitaTx *tx_ptr, BUINT8 address[BOAT_CITA_ADDRESS_SIZE])
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    
    // Set recipient's address
    memcpy(&tx_ptr->rawtx_fields.recipient, address, BOAT_CITA_ADDRESS_SIZE);

    return BOAT_SUCCESS;    
}

BOAT_RESULT BoatCitaTxSetValue(BoatCitaTx *tx_ptr, BoatFieldMax32B *value_ptr)
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    
    // Set value
    if (value_ptr != NULL)
    {
        memcpy(&tx_ptr->rawtx_fields.value, value_ptr, sizeof(BoatFieldMax32B));
    }
    else
    {
        // If value_ptr is NULL, value is treated as 0.
        // NOTE: value.field_len == 0 has the same effect as
        //       value.field_len == 1 && value.field[0] == 0x00 for RLP encoding
        tx_ptr->rawtx_fields.value.field_len = 0;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatCitaTxSetData(BoatCitaTx *tx_ptr, BoatFieldVariable *data_ptr)
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    
    // Set data
    if (data_ptr != NULL)
    {
        // NOTE: tx_ptr->rawtx_fields.data.field_ptr is a pointer
        //       The caller must make sure the storage it points to is available
        //       until the transaction is sent.
        memcpy(&tx_ptr->rawtx_fields.data, data_ptr, sizeof(BoatFieldVariable));
    }
    else
    {
        // If data_ptr is NULL, value is treated as 0.
        // NOTE: data.field_len == 0 has the same effect as
        //       data.field_len == 1 && data.field_ptr[0] == 0x00 for RLP encoding
        tx_ptr->rawtx_fields.data.field_len = 0;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatCitaTxSetNonce(BoatCitaTx *tx_ptr, BUINT64 nonce)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (nonce == BOAT_CITA_NONCE_AUTO)
    {
        tx_ptr->rawtx_fields.nonce.field_len = 16;
        result = BoatRandom(tx_ptr->rawtx_fields.nonce.field, 
                            tx_ptr->rawtx_fields.nonce.field_len, NULL);
    }
    else
    {
        tx_ptr->rawtx_fields.nonce.field_len = \
               UtilityUint64ToBigend(tx_ptr->rawtx_fields.nonce.field, nonce, TRIMBIN_LEFTTRIM);
    }

    return result;
}


BOAT_RESULT BoatCitaSendRawtxWithReceipt(BOAT_INOUT BoatCitaTx *tx_ptr)
{
    BOAT_RESULT result = BOAT_ERROR;

    result = CitaSendRawtx(tx_ptr);

    if (result == BOAT_SUCCESS)
    {
        result = BoatCitaGetTransactionReceipt(tx_ptr);
    }

    return result;
}

BOAT_RESULT BoatCitaTxSend(BoatCitaTx *tx_ptr)
{
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }


    if (tx_ptr->is_sync_tx == BOAT_FALSE)
    {
        result = CitaSendRawtx(tx_ptr);
    }
    else
    {
        result = BoatCitaSendRawtxWithReceipt(tx_ptr);
    }
    
    return result;
}
BCHAR *BoatCitaCallContractFunc(BoatCitaTx *tx_ptr, BCHAR *func_proto_str,
                                     BUINT8 *func_param_ptr, BUINT32 func_param_len)
{
    Param_cita_call param_cita_call;
    BUINT8 function_selector[32];
    BUINT8 hashLenDummy;

    BCHAR  from_hexstr[BOAT_CITA_ADDRESS_SIZE * 2 + 3];
    BCHAR  to_hexstr[BOAT_CITA_ADDRESS_SIZE * 2 + 3];
    BCHAR  data_hexstr[(func_param_len + 4) * 2 + 3]; /* +4 for function selector, *2 for bin to HEX, 
                                                       +3 for "0x" prefix and NULL terminator Compiler 
                                                       MUST support C99 to allow variable-size local array */
    BOAT_RESULT result = BOAT_SUCCESS;
    BCHAR  *retval_str;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return NULL;
    }
    
    if (func_param_ptr == NULL && func_param_len != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return NULL;
    }

    if (func_param_len > BOAT_STRING_MAX_LEN)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return NULL;
    }
    
    if ((BOAT_SUCCESS != UtilityStringLenCheck(func_proto_str)) && \
        (BOAT_SUCCESS != UtilityStringLenCheck((BCHAR*)func_param_ptr)))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return NULL;
    }

    //set from
    memset(from_hexstr, 0, sizeof(from_hexstr));
    UtilityBinToHex(from_hexstr, tx_ptr->wallet_ptr->account_info.address, BOAT_CITA_ADDRESS_SIZE,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
     param_cita_call.from  = from_hexstr;

    //set to
    memset(to_hexstr, 0, sizeof(to_hexstr));
    UtilityBinToHex(to_hexstr, tx_ptr->rawtx_fields.recipient, BOAT_CITA_ADDRESS_SIZE, 
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_cita_call.to  = to_hexstr;

    //set data
    memset(data_hexstr, 0, sizeof(data_hexstr));
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8*)func_proto_str, 
             strlen(func_proto_str), function_selector, &hashLenDummy, NULL);
    UtilityBinToHex(data_hexstr, function_selector, 4,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES,  BOAT_FALSE);
    UtilityBinToHex(data_hexstr+10, func_param_ptr, func_param_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE);
    param_cita_call.data  = data_hexstr;

    retval_str = web3_cita_call(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                     tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                     &param_cita_call,&result);
    if (retval_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "web3 cita call fail ,result = %d .",result);
    }

    return retval_str;
}




BOAT_RESULT BoatCitaGetTransactionReceipt(BoatCitaTx *tx_ptr)
{
    Param_cita_getTransactionReceipt param_cita_getTransactionReceipt;

    BCHAR   txhash_hexstr[67];
    BCHAR   *tx_status_str;
    BSINT32 tx_mined_timeout;
    BOAT_RESULT result = BOAT_SUCCESS;

    tx_mined_timeout = BOAT_CITA_WAIT_PENDING_TX_TIMEOUT;
    
    //set txHash
    memset(txhash_hexstr, 0, sizeof(txhash_hexstr));
    UtilityBinToHex(txhash_hexstr, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_cita_getTransactionReceipt.tx_hash_str = txhash_hexstr;

    do{
        BoatSleep(BOAT_CITA_MINE_INTERVAL); // Sleep waiting for the block being mined 
        tx_status_str = web3_cita_getTransactionReceiptStatus(tx_ptr->wallet_ptr->web3intf_context_ptr,
                        tx_ptr->wallet_ptr->network_info.node_url_ptr,
                        &param_cita_getTransactionReceipt, &result);
        if (tx_status_str == NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            break;
        }
        // "status" == null : the transaction is pending, the result is BOAT_ERROR
        // todo: need to change fiscobcos_parse_json_result() 
        result = BoatCitaParseRpcResponseResult(tx_status_str, "errorMessage", 
                                                     &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
        
        if (result != BOAT_SUCCESS && result != BOAT_ERROR_JSON_OBJ_IS_NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            result = BOAT_ERROR_WALLET_RESULT_PARSE_FAIL;
            break;
        }

        else
        {
            // tx_status_str == "0x0": the transaction is successfully mined
            // tx_status_str is ohters: error number, for detail, see https://fisco-bcos-documentation.readthedocs.io/zh_CN/latest/docs/api.html#
            if (result != BOAT_ERROR_JSON_OBJ_IS_NULL)
            {
                if (strcmp((BCHAR*)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr, "null") == 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, "Transaction has got mined.");
                    break;
                }
                else
                {
                    BoatLog(BOAT_LOG_VERBOSE, "Transaction failed, error number is %s.", tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr);
                    break;
                }
            }
            else
            {
                BoatLog(BOAT_LOG_NORMAL, "Transaction has not got mined, requery after %d seconds.", BOAT_CITA_MINE_INTERVAL);
            }

            tx_mined_timeout -= BOAT_CITA_MINE_INTERVAL;
        }
        
    } while(tx_mined_timeout > 0);

    if (tx_mined_timeout <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "Wait for pending transaction timeout. This does not mean the transaction fails.");
        result = BOAT_ERROR_COMMON_TX_PENDING;
    }

    return result;
}


BCHAR *BoatCitaGetBlockNumber(BoatCitaTx *tx_ptr)
{
    BUINT64 result = BOAT_SUCCESS;
    BCHAR *retval_str;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return NULL;
    }
    retval_str = web3_cita_getBlockNumber(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                               tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                               &result);
    BoatLog(BOAT_LOG_CRITICAL, "web3 cita get blocknumber 1111 ,result = %d\n .",result);
    BoatLog(BOAT_LOG_CRITICAL, "web3 cita get blocknumber 2222 ,retval_str = %s\n .",retval_str);

    if (retval_str == NULL)
    {
        
    }
    return retval_str;
}


BOAT_RESULT BoatCitaParseRpcResponseStringResult(const BCHAR *json_string, BoatFieldVariable *result_out)
{
    return cita_parse_json_result(json_string, "", result_out);
}

BOAT_RESULT BoatCitaParseRpcResponseResult(const BCHAR *json_string, 
                                                const BCHAR *child_name, 
                                                BoatFieldVariable *result_out)
{
    if (child_name == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    return cita_parse_json_result(json_string, child_name, result_out);
}


