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

/*!@brief PlatON wallet API for BoAT IoT SDK

@file
api_platon.c defines the PlatON wallet API for BoAT IoT SDK.
*/

#include "boatconfig.h"
#include "boatinternal.h"
#if PROTOCOL_USE_PLATON == 1
#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"
#include "cJSON.h"

BCHAR *BoatPlatONWalletGetBalance(BoatPlatONWallet *wallet_ptr, const BCHAR *hrp_str)
{
    Param_web3_getBalance param_platon_getBalance;
    BCHAR *tx_balance_str;
    BCHAR *address_ptr;
    BOAT_RESULT result = BOAT_SUCCESS;

    if (wallet_ptr == NULL || hrp_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return NULL;
    }

    address_ptr = BoatMalloc(50);
    if (address_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Malloc memory failed.");
        return NULL;
    }
    BoatPlatONBech32Encode(wallet_ptr->account_info.address, BOAT_PLATON_ADDRESS_SIZE,
                           address_ptr, hrp_str, strlen(hrp_str));

    // Get balance from network
    // Return value of web3_getBalance() is balance in von
    param_platon_getBalance.method_name_str = "platon_getBalance";
    param_platon_getBalance.address_str = address_ptr;
    param_platon_getBalance.block_num_str = "latest";
    tx_balance_str = web3_getBalance(wallet_ptr->web3intf_context_ptr,
                                     wallet_ptr->network_info.node_url_str,
                                     &param_platon_getBalance, &result);

    if (tx_balance_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to get balance from network, result = %d ", result);
        return NULL;
    }

    BoatFree(address_ptr);

    return tx_balance_str;
}

/**
 * @description:
 *  This function create BoatPlatONWallet by walletindex and networkIndex.
 * @param {BUINT8} walletIndex
 * @param {BUINT8} networkIndex
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BoatPlatONWallet *BoatPlatONWalletInit(BUINT8 walletIndex, BUINT8 networkIndex)
{
    BoatPlatONWallet *wallet_ptr;
    BOAT_RESULT result;
    BUINT8 pubkeyHash[32];
    BUINT8 hashLenDummy;

    wallet_ptr = BoatMalloc(sizeof(BoatPlatONWallet));
    if (wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet memory malloc falied.");
        return NULL;
    }
    result = BoATKeypair_GetKeypairByIndex(&(wallet_ptr->account_info.prikeyCtx), walletIndex);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get wallet by index fail");
        return NULL;
    }
    result = BoATPlatON_GetNetworkByIndex(&(wallet_ptr->network_info), networkIndex);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get network by index fail");
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

    // Configure account address
    BoatHash(BOAT_HASH_KECCAK256, wallet_ptr->account_info.prikeyCtx.pubkey_content,
             64, pubkeyHash, &hashLenDummy, NULL);
    memcpy(wallet_ptr->account_info.address, &pubkeyHash[32 - BOAT_PLATON_ADDRESS_SIZE], BOAT_PLATON_ADDRESS_SIZE);

    return wallet_ptr;
}

void BoatPlatONWalletDeInit(BoatPlatONWallet *wallet_ptr)
{
    if (wallet_ptr != NULL)
    {
        if (wallet_ptr->account_info.prikeyCtx.keypair_name != NULL)
        {
            BoatFree(wallet_ptr->account_info.prikeyCtx.keypair_name);
            wallet_ptr->account_info.prikeyCtx.keypair_name = NULL;
        }

        web3_deinit(wallet_ptr->web3intf_context_ptr);

        BoatFree(wallet_ptr);
    }
}

BOAT_RESULT BoatPlatONTxInit(BoatPlatONWallet *wallet_ptr,
                             BoatPlatONTx *tx_ptr,
                             BBOOL is_sync_tx,
                             const BCHAR *gasprice_str,
                             const BCHAR *gaslimit_str,
                             const BCHAR *recipient_str,
                             const BCHAR *hrp_str)
{
    BOAT_RESULT result;

    if (wallet_ptr == NULL || tx_ptr == NULL || recipient_str == NULL || hrp_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if ((gasprice_str != NULL) && !UtilityStringIsHex(gasprice_str))
    {
        BoatLog(BOAT_LOG_CRITICAL, "The format of gasprice is incorrect");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if ((gaslimit_str != NULL) && !UtilityStringIsHex(gaslimit_str))
    {
        BoatLog(BOAT_LOG_CRITICAL, "The format of gaslimit is incorrect");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (UtilityStringLenCheck(recipient_str) != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "The length of string recipient_str is incorrect");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;
    memset(&tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields));

    // Generate platon's Bech32 address from the public key
    result = BoatPlatONBech32Encode(wallet_ptr->account_info.address , BOAT_PLATON_ADDRESS_SIZE,
                                    tx_ptr->address, hrp_str, strlen(hrp_str));
    if (result == BOAT_ERROR)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONBech32Encode failed");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set synchronous transaction flag
    tx_ptr->is_sync_tx = is_sync_tx;

    // Initialize gasprice
    BoatFieldMax32B gasprice;
    // Either manually set the gas price or get the price from network
    if (gasprice_str != NULL)
    {
        // Manually
        gasprice.field_len = UtilityHexToBin(gasprice.field, 32, gasprice_str,
                                             TRIMBIN_LEFTTRIM, BOAT_TRUE);
        result = BoatPlatONTxSetGasPrice(tx_ptr, &gasprice);
    }
    else
    {
        // To use the price obtained from network, simply call BoatTxSetGasPrice(NULL)
        result = BoatPlatONTxSetGasPrice(tx_ptr, NULL);
    }

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "set gas price failed: %d.", result);
        return result;
    }

    // Initialize gaslimit
    BoatFieldMax32B gaslimit;

    gaslimit.field_len = UtilityHexToBin(gaslimit.field, 32, gaslimit_str,
                                         TRIMBIN_LEFTTRIM, BOAT_TRUE);
    result = BoatPlatONTxSetGasLimit(tx_ptr, &gaslimit);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONTxSetGasLimit failed.");
        return result;
    }

    BUINT8 recipient[BOAT_PLATON_ADDRESS_SIZE];
    BSINT32 converted_len;

    converted_len = BoatPlatONBech32Decode(recipient_str, strlen(recipient_str), recipient);

    // converted_len = UtilityHexToBin(recipient, BOAT_PLATON_ADDRESS_SIZE, recipient_str,
    //								TRIMBIN_TRIM_NO, BOAT_TRUE);
    if (converted_len <= 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "recipient Initialize failed.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    memset(tx_ptr->rawtx_fields.recipientbech32,0U,BOAT_PLATON_BECH32_ADDRESS_SIZE);
    memcpy(tx_ptr->rawtx_fields.recipientbech32,recipient_str,strlen(recipient_str));

    result = BoatPlatONTxSetRecipient(tx_ptr, recipient);

    // result = BoatPlatONTxSetRecipient(tx_ptr, recipient_str);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONTxSetRecipient failed.");
        return result;
    }

    // Initialize value = 0
    result = BoatPlatONTxSetValue(tx_ptr, NULL);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONTxSetValue failed.");
        return result;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatPlatONTxSetNonce(BoatPlatONTx *tx_ptr, BUINT64 nonce)
{
    Param_web3_getTransactionCount param_platon_getTransactionCount;
    BCHAR *tx_count_str;
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (nonce == BOAT_PLATON_NONCE_AUTO)
    {
        /* Get transaction count from network
           Return value of web3_getTransactionCount() is transaction count */
        param_platon_getTransactionCount.method_name_str = "platon_getTransactionCount";
        param_platon_getTransactionCount.address_str = tx_ptr->address;
        param_platon_getTransactionCount.block_num_str = "latest";
        tx_count_str = web3_getTransactionCount(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                                tx_ptr->wallet_ptr->network_info.node_url_str,
                                                &param_platon_getTransactionCount, &result);
        if (tx_count_str == NULL)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to get transaction count from network.");
            return result;
        }
        result = BoatPlatONParseRpcResponseStringResult(tx_count_str,
                                                        &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to get transaction count from network.");
            return result;
        }

        /* Set nonce from transaction count */
        tx_ptr->rawtx_fields.nonce.field_len = UtilityHexToBin(tx_ptr->rawtx_fields.nonce.field, 32,
                                                               (BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
                                                               TRIMBIN_LEFTTRIM, BOAT_TRUE);
    }
    else
    {
        /* Set nonce */
        tx_ptr->rawtx_fields.nonce.field_len = UtilityUint64ToBigend(tx_ptr->rawtx_fields.nonce.field,
                                                                     nonce, TRIMBIN_LEFTTRIM);
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatPlatONTxSetGasPrice(BoatPlatONTx *tx_ptr, BoatFieldMax32B *gas_price_ptr)
{
    BCHAR *gas_price_from_net_str;
    BOAT_RESULT result = BOAT_SUCCESS;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // If gas price is specified, use it
    // Otherwise use gas price obtained from network
    if (gas_price_ptr != NULL)
    {
        memcpy(&tx_ptr->rawtx_fields.gasprice, gas_price_ptr, sizeof(BoatFieldMax32B));
    }
    else
    {
        // Get current gas price from network
        // Return value of platon_gasPrice is in von
        gas_price_from_net_str = web3_gasPrice(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                               tx_ptr->wallet_ptr->network_info.node_url_str,
                                               "platon_gasPrice", &result);
        if (gas_price_from_net_str == NULL)
        {
            BoatLog(BOAT_LOG_CRITICAL, "get gas price fail, result = %d.", result);
            return BOAT_ERROR_WEB3_GET_GASPRICE_FAIL;
        }

        result = BoatPlatONParseRpcResponseStringResult(gas_price_from_net_str,
                                                        &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
        if (result == BOAT_SUCCESS)
        {
            // Set transaction gasPrice with the one got from network
            tx_ptr->rawtx_fields.gasprice.field_len =
                UtilityHexToBin(
                    tx_ptr->rawtx_fields.gasprice.field,
                    32,
                    (BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
                    TRIMBIN_LEFTTRIM,
                    BOAT_TRUE);

            BoatLog(BOAT_LOG_VERBOSE, "Use gasPrice from network: %s wei.", tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr);
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get gasPrice from network.");
        }
    }

    return result;
}

BOAT_RESULT BoatPlatONTxSetRecipient(BoatPlatONTx *tx_ptr, BUINT8 address[BOAT_PLATON_ADDRESS_SIZE])
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set recipient's address
    // memcpy(&tx_ptr->rawtx_fields.recipientbech32, address_str, BOAT_PLATON_BECH32_ADDRESS_SIZE);
    memcpy(&tx_ptr->rawtx_fields.recipient, address, BOAT_PLATON_ADDRESS_SIZE);

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatPlatONTxSetGasLimit(BoatPlatONTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set gasLimit
    if (gas_limit_ptr != NULL)
    {
        memcpy(&tx_ptr->rawtx_fields.gaslimit, gas_limit_ptr, sizeof(BoatFieldMax32B));
        return BOAT_SUCCESS;
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
}

BOAT_RESULT BoatPlatONTxSetValue(BoatPlatONTx *tx_ptr, BoatFieldMax32B *value_ptr)
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

BOAT_RESULT BoatPlatONTxSetData(BoatPlatONTx *tx_ptr, BoatFieldVariable *data_ptr)
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

BOAT_RESULT BoatPlatONSendRawtxWithReceipt(BOAT_INOUT BoatPlatONTx *tx_ptr)
{
    BOAT_RESULT result = BOAT_ERROR;

    result = PlatONSendRawtx(tx_ptr);

    if (result == BOAT_SUCCESS)
    {
        result = BoatPlatONGetTransactionReceipt(tx_ptr);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "PlatONSendRawtx failed.");
    }

    return result;
}

BOAT_RESULT BoatPlatONTxSend(BoatPlatONTx *tx_ptr)
{
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (tx_ptr->is_sync_tx == BOAT_FALSE)
    {
        result = PlatONSendRawtx(tx_ptr);
    }
    else
    {
        result = BoatPlatONSendRawtxWithReceipt(tx_ptr);
    }

    return result;
}



BCHAR *BoatPlatONCallContractFunc(BoatPlatONTx *tx_ptr, BUINT8 *rlp_param_ptr,
                                   BUINT32 rlp_param_len)
{
    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len * 2 + 3]; // Compiler MUST support C99 to allow variable-size local array

    Param_web3_call param_platon_call;
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

    if (BOAT_SUCCESS != UtilityStringLenCheck((BCHAR *)rlp_param_ptr))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return NULL;
    }

    //BCHAR recipient_hexstr[BOAT_PLATON_ADDRESS_SIZE * 2 + 3];

   // UtilityBinToHex(recipient_hexstr, tx_ptr->rawtx_fields.recipient, BOAT_PLATON_ADDRESS_SIZE,
    //                BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);

    param_platon_call.from = tx_ptr->address;
    param_platon_call.to = tx_ptr->rawtx_fields.recipientbech32;;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_platon_call.gas = "0x1fffff";
    param_platon_call.gasPrice = "0x12a05f200";

    // Set function parameters
    UtilityBinToHex(data_str, rlp_param_ptr, rlp_param_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_platon_call.method_name_str = "platon_call";
    param_platon_call.data = data_str;
    param_platon_call.block_num_str = "latest";
    retval_str = platon_web3_call(tx_ptr->wallet_ptr->web3intf_context_ptr,
                           tx_ptr->wallet_ptr->network_info.node_url_str,
                           &param_platon_call, &result);
    if (retval_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "web3 call fail, result = %d ", result);
    }
    return retval_str;
}


BOAT_RESULT BoatPlatONTransfer(BoatPlatONTx *tx_ptr, BCHAR *value_hex_str)
{
    BoatFieldMax32B value;
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL || value_hex_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set nonce
    result = BoatPlatONTxSetNonce(tx_ptr, BOAT_PLATON_NONCE_AUTO);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "nonce set failed.");
        return result;
    }

    // Set value

    value.field_len = UtilityHexToBin(value.field, 32, value_hex_str,
                                      TRIMBIN_LEFTTRIM, BOAT_TRUE);
    result = BoatPlatONTxSetValue(tx_ptr, &value);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONTxSetValue failed.");
        return result;
    }

    // Set data
    result = BoatPlatONTxSetData(tx_ptr, NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatONTxSetData failed.");
        return result;
    }

    // Perform the transaction
    // NOTE: Field v,r,s are calculated automatically
    result = BoatPlatONTxSend(tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "transaction send failed.");
        return result;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatPlatONGetTransactionReceipt(BoatPlatONTx *tx_ptr)
{
    BCHAR tx_hash_str[67];
    BCHAR *tx_status_str;
    Param_web3_getTransactionReceipt param_platon_getTransactionReceipt;
    BSINT32 tx_mined_timeout;

    BOAT_RESULT result = BOAT_SUCCESS;

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    tx_mined_timeout = BOAT_PLATON_WAIT_PENDING_TX_TIMEOUT;
    param_platon_getTransactionReceipt.method_name_str = "platon_getTransactionReceipt";
    param_platon_getTransactionReceipt.tx_hash_str = tx_hash_str;

    do
    {
        BoatSleep(BOAT_PLATON_MINE_INTERVAL); // Sleep waiting for the block being mined
        tx_status_str = web3_getTransactionReceiptStatus(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                                         tx_ptr->wallet_ptr->network_info.node_url_str,
                                                         &param_platon_getTransactionReceipt, &result);
        if (tx_status_str == NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            break;
        }
        result = BoatPlatONParseRpcResponseResult(tx_status_str, "status",
                                                  &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
        if (result != BOAT_SUCCESS && result != BOAT_ERROR_JSON_OBJ_IS_NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            result = BOAT_ERROR_WALLET_RESULT_PARSE_FAIL;
            break;
        }
        else
        {
            // tx_status_str == "": the transaction is pending
            // tx_status_str == "0x1": the transaction is successfully mined
            // tx_status_str == "0x0": the transaction fails
            if (result != BOAT_ERROR_JSON_OBJ_IS_NULL)
            {
                if (strcmp((BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr, "0x1") == 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, "Transaction has got mined.");
                    break;
                }
                else
                {
                    BoatLog(BOAT_LOG_CRITICAL, "Transaction fails.");
                    break;
                }
            }
            else
            {
                BoatLog(BOAT_LOG_NORMAL, "Transaction has not got mined, requery after %d seconds.", BOAT_PLATON_MINE_INTERVAL);
            }

            tx_mined_timeout -= BOAT_PLATON_MINE_INTERVAL;
        }

    } while (tx_mined_timeout > 0);

    if (tx_mined_timeout <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "Wait for pending transaction timeout. This does not mean the transaction fails.");
        result = BOAT_ERROR_COMMON_TX_PENDING;
    }

    return result;
}

BOAT_RESULT BoatPlatONParseRpcResponseStringResult(const BCHAR *json_string, BoatFieldVariable *result_out)
{
    return platon_parse_json_result(json_string, "", result_out);
}

BOAT_RESULT BoatPlatONParseRpcResponseResult(const BCHAR *json_string,
                                             const BCHAR *child_name,
                                             BoatFieldVariable *result_out)
{
    if (child_name == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    return platon_parse_json_result(json_string, child_name, result_out);
}

#endif /* end of PROTOCOL_USE_PLATON */
