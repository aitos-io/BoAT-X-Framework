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
api_venachain.c defines the Venachain wallet API for BoAT IoT SDK.
*/

#include "boatinternal.h"
#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"
#include "cJSON.h"

/**
 * @description:
 *  This function create BoatVenachainWallet by walletindex and networkIndex.
 * @param {BUINT8} walletIndex
 * @param {BUINT8} networkIndex
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BoatVenachainWallet *BoatVenachainWalletInit(BUINT8 walletIndex, BUINT8 networkIndex)
{
    BoatVenachainWallet *wallet_ptr;
    BOAT_RESULT result;
    BUINT8 pubkeyHash[32];
    BUINT8 hashLenDummy;

    wallet_ptr = BoatMalloc(sizeof(BoatVenachainWallet));
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
    result = BoATVenachain_GetNetworkByIndex(&(wallet_ptr->network_info), networkIndex);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get network by index fail");
        return NULL;
    }
    // if(wallet_ptr->network_info.protocolType != BOAT_PROTOCOL_ETHEREUM){
    //     BoatLog(BOAT_LOG_CRITICAL, "this network is not for eth ");
    //     return NULL;
    // }
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
    memcpy(wallet_ptr->account_info.address, &pubkeyHash[32 - BOAT_VENACHAIN_ADDRESS_SIZE], BOAT_VENACHAIN_ADDRESS_SIZE);

    return wallet_ptr;
}

void BoatVenachainWalletDeInit(BoatVenachainWallet *wallet_ptr)
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

        wallet_ptr = NULL;
    }
}

BCHAR *BoatVenachainWalletGetBalance(BoatVenachainWallet *wallet_ptr, BCHAR *alt_address_str)
{
    BUINT8 alt_address[BOAT_VENACHAIN_ADDRESS_SIZE]; // Binary altered address converted from alt_address_str
    BUINT8 *address_ptr;                             // Point to an address in binary format, either wallet
                                                     // owner's or the one converted from alt_address_str
    BCHAR address_str[43];                           // Address in string format, converted from address_ptr

    Param_web3_getBalance param_web3_getBalance;
    BCHAR *tx_balance_str;
    BOAT_RESULT result = BOAT_SUCCESS;

    if (wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return NULL;
    }

    if (alt_address_str == NULL)
    {
        // Query Wallet's Owner address
        // PRIVATE KEY MUST BE SET BEFORE GETTING BALANCE, BECAUSE GETTING BALANCE FROM
        // NETWORK NEEDS ETHEREUM ADDRESS, WHICH IS COMPUTED FROM KEY
        address_ptr = wallet_ptr->account_info.address;
    }
    else
    {
        // Query specified altered address
        UtilityHexToBin(alt_address, BOAT_VENACHAIN_ADDRESS_SIZE, alt_address_str,
                        TRIMBIN_TRIM_NO, BOAT_TRUE);

        address_ptr = alt_address;
    }

    // Get balance from network
    // Return value of web3_getBalance() is balance in wei
    UtilityBinToHex(address_str, address_ptr, BOAT_VENACHAIN_ADDRESS_SIZE,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_web3_getBalance.method_name_str = "eth_getBalance";
    param_web3_getBalance.address_str = address_str;
    param_web3_getBalance.block_num_str = "latest";
    tx_balance_str = web3_getBalance(wallet_ptr->web3intf_context_ptr,
                                     wallet_ptr->network_info.node_url_str,
                                     &param_web3_getBalance, &result);

    if (tx_balance_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to get balance from network, result = %d ", result);
        return NULL;
    }

    return tx_balance_str;
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

BOAT_RESULT BoatVenachainTxInit(BoatVenachainWallet *wallet_ptr,
                                BoatVenachainTx *tx_ptr,
                                BBOOL is_sync_tx,
                                BCHAR *gasprice_str,
                                BCHAR *gaslimit_str,
                                BCHAR *recipient_str)
{
    BOAT_RESULT result;

    if (wallet_ptr == NULL || tx_ptr == NULL || recipient_str == NULL)
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

    if (!UtilityStringIsHex(recipient_str))
    {
        BoatLog(BOAT_LOG_CRITICAL, "The format of recipient is incorrect");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (UtilityStringLenCheck(recipient_str) != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "The length of string recipient_str is incorrect");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (strlen(recipient_str) != 42)
    {
        BoatLog(BOAT_LOG_CRITICAL, "The length of string recipient_str is incorrect");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;
    memset(&tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields));

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
        result = BoatVenachainTxSetGasPrice(tx_ptr, &gasprice);
    }
    else
    {
        // To use the price obtained from network, simply call BoatTxSetGasPrice(NULL)
        result = BoatVenachainTxSetGasPrice(tx_ptr, NULL);
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
    result = BoatVenachainTxSetGasLimit(tx_ptr, &gaslimit);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatVenachainTxSetGasLimit failed.");
        return result;
    }

    // Initialize recipient
    BUINT8 recipient[BOAT_VENACHAIN_ADDRESS_SIZE];
    BUINT32 converted_len;
    converted_len = UtilityHexToBin(recipient, BOAT_VENACHAIN_ADDRESS_SIZE, recipient_str,
                                    TRIMBIN_TRIM_NO, BOAT_TRUE);
    if (converted_len == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "recipient Initialize failed.");
        return BOAT_ERROR_COMMON_UTILITY;
    }

    result = BoatVenachainTxSetRecipient(tx_ptr, recipient);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatVenachainTxSetRecipient failed.");
        return result;
    }

    // Initialize value = 0
    result = BoatVenachainTxSetValue(tx_ptr, NULL);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatVenachainTxSetValue failed.");
        return result;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatVenachainTxSetNonce(BoatVenachainTx *tx_ptr, BUINT64 nonce)
{
    BCHAR account_address_str[43];
    Param_web3_getTransactionCount param_web3_getTransactionCount;
    BCHAR *tx_count_str;
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (nonce == BOAT_VENACHAIN_NONCE_AUTO)
    {
        /* Get transaction count from network
           Return value of web3_getTransactionCount() is transaction count */
        UtilityBinToHex(account_address_str, tx_ptr->wallet_ptr->account_info.address,
                        BOAT_VENACHAIN_ADDRESS_SIZE, BIN2HEX_LEFTTRIM_UNFMTDATA,
                        BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
        param_web3_getTransactionCount.method_name_str = "eth_getTransactionCount";
        param_web3_getTransactionCount.address_str = account_address_str;
        param_web3_getTransactionCount.block_num_str = "latest";
        tx_count_str = web3_getTransactionCount(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                                tx_ptr->wallet_ptr->network_info.node_url_str,
                                                &param_web3_getTransactionCount, &result);
        if (tx_count_str == NULL)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to get transaction count from network.");
            return result;
        }
        result = BoatVenachainParseRpcResponseStringResult(tx_count_str,
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

BOAT_RESULT BoatVenachainTxSetGasPrice(BoatVenachainTx *tx_ptr, BoatFieldMax32B *gas_price_ptr)
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
        // Return value of web3_gasPrice is in wei
        gas_price_from_net_str = web3_gasPrice(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                               tx_ptr->wallet_ptr->network_info.node_url_str,
                                               "eth_gasPrice", &result);
        if (gas_price_from_net_str == NULL)
        {
            BoatLog(BOAT_LOG_CRITICAL, "get gas price fail, result = %d.", result);
            return BOAT_ERROR_WEB3_GET_GASPRICE_FAIL;
        }

        result = BoatVenachainParseRpcResponseStringResult(gas_price_from_net_str,
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

            BoatLog(BOAT_LOG_VERBOSE, "Use gasPrice from network: %s wei.", gas_price_from_net_str);
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get gasPrice from network.");
        }
    }

    return result;
}

BOAT_RESULT BoatVenachainTxSetGasLimit(BoatVenachainTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
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

BOAT_RESULT BoatVenachainTxSetRecipient(BoatVenachainTx *tx_ptr, BUINT8 address[BOAT_VENACHAIN_ADDRESS_SIZE])
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set recipient's address
    memcpy(&tx_ptr->rawtx_fields.recipient, address, BOAT_VENACHAIN_ADDRESS_SIZE);

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatVenachainSendRawtxWithReceipt(BOAT_INOUT BoatVenachainTx *tx_ptr)
{
    BOAT_RESULT result = BOAT_ERROR;

    result = VenachainSendRawtx(tx_ptr);

    if (result == BOAT_SUCCESS)
    {
        result = BoatVenachainGetTransactionReceipt(tx_ptr);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "VenachainSendRawtx failed.");
    }

    return result;
}

BOAT_RESULT BoatVenachainTxSetValue(BoatVenachainTx *tx_ptr, BoatFieldMax32B *value_ptr)
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

BOAT_RESULT BoatVenachainTxSetData(BoatVenachainTx *tx_ptr, BoatFieldVariable *data_ptr)
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

BOAT_RESULT BoatVenachainTxSend(BoatVenachainTx *tx_ptr)
{
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
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

BCHAR *BoatVenachainCallContractFunc(BoatVenachainTx *tx_ptr, BUINT8 *rlp_param_ptr,
                                     BUINT32 rlp_param_len)
{
    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len * 2 + 3]; // Compiler MUST support C99 to allow variable-size local array

    Param_web3_call param_venachain_call;
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

    BCHAR recipient_hexstr[BOAT_VENACHAIN_ADDRESS_SIZE * 2 + 3];

    UtilityBinToHex(recipient_hexstr, tx_ptr->rawtx_fields.recipient, BOAT_VENACHAIN_ADDRESS_SIZE,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_venachain_call.to = recipient_hexstr;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_venachain_call.gas = "0x1fffff";
    param_venachain_call.gasPrice = "0x8250de00";

    // Set function parameters
    UtilityBinToHex(data_str, rlp_param_ptr, rlp_param_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_venachain_call.method_name_str = "eth_call";
    param_venachain_call.data = data_str;
    param_venachain_call.block_num_str = "latest";
    retval_str = web3_call(tx_ptr->wallet_ptr->web3intf_context_ptr,
                           tx_ptr->wallet_ptr->network_info.node_url_str,
                           &param_venachain_call, &result);
    if (retval_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "web3 call fail, result = %d ", result);
    }
    return retval_str;
}

BOAT_RESULT BoatVenachainTransfer(BoatVenachainTx *tx_ptr, BCHAR *value_hex_str)
{
    BoatFieldMax32B value;
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL || value_hex_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set nonce
    result = BoatVenachainTxSetNonce(tx_ptr, BOAT_VENACHAIN_NONCE_AUTO);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "nonce set failed.");
        return result;
    }

    // Set value

    value.field_len = UtilityHexToBin(value.field, 32, value_hex_str,
                                      TRIMBIN_LEFTTRIM, BOAT_TRUE);
    result = BoatVenachainTxSetValue(tx_ptr, &value);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "value set failed.");
        return result;
    }

    // Set data
    result = BoatVenachainTxSetData(tx_ptr, NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "data set failed.");
        return result;
    }

    // Perform the transaction
    // NOTE: Field v,r,s are calculated automatically
    result = BoatVenachainTxSend(tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "transaction send failed.");
        return result;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatVenachainGetTransactionReceipt(BoatVenachainTx *tx_ptr)
{
    BCHAR tx_hash_str[67];
    BCHAR *tx_status_str;
    Param_web3_getTransactionReceipt param_web3_getTransactionReceipt;
    BSINT32 tx_mined_timeout;

    BOAT_RESULT result = BOAT_SUCCESS;

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    tx_mined_timeout = BOAT_VENACHAIN_WAIT_PENDING_TX_TIMEOUT;
    param_web3_getTransactionReceipt.method_name_str = "eth_getTransactionReceipt";
    param_web3_getTransactionReceipt.tx_hash_str = tx_hash_str;

    do
    {
        BoatSleep(BOAT_VENACHAIN_MINE_INTERVAL); // Sleep waiting for the block being mined
        tx_status_str = web3_getTransactionReceiptStatus(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                                         tx_ptr->wallet_ptr->network_info.node_url_str,
                                                         &param_web3_getTransactionReceipt, &result);
        if (tx_status_str == NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            break;
        }
        result = BoatVenachainParseRpcResponseResult(tx_status_str, "status",
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
                    BoatLog(BOAT_LOG_NORMAL, "Transaction fails.");
                    break;
                }
            }
            else
            {
                BoatLog(BOAT_LOG_NORMAL, "Transaction has not got mined, requery after %d seconds.", BOAT_VENACHAIN_MINE_INTERVAL);
            }

            tx_mined_timeout -= BOAT_VENACHAIN_MINE_INTERVAL;
        }

    } while (tx_mined_timeout > 0);

    if (tx_mined_timeout <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "Wait for pending transaction timeout. This does not mean the transaction fails.");
        result = BOAT_ERROR_COMMON_TX_PENDING;
    }

    return result;
}

/**
 * @description:
 *  This function changes the URL of Venachain wallet to new URL;
 * @param {BoatVenachainWallet} *wallet_ptr
 * @param {BCHAR *} newUrl
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatVenachainWalletChangeNodeUrl(BoatVenachainWallet *wallet_ptr, BCHAR *newUrl)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (wallet_ptr == NULL || newUrl == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (strlen(newUrl) >= sizeof(wallet_ptr->network_info.node_url_str))
    {
        BoatLog(BOAT_LOG_CRITICAL, "node URL length out of limit: %s.", newUrl);
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    strcpy(wallet_ptr->network_info.node_url_str, newUrl);
    return result;
}

/**
 * @description:
 *  This function changes the chainID of Venachain wallet to new chainID;
 * @param {BoatVenachainWallet} *wallet_ptr
 * @param {BUINT32} newChainID
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatVenachainWalletChangeChainID(BoatVenachainWallet *wallet_ptr, BUINT32 newChainID)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    wallet_ptr->network_info.chain_id = newChainID;
    return result;
}

/**
 * @description:
 *  This function changes the eip155 compatibility of Venachain wallet to new compatibility;
 * @param {BoatVenachainWallet} *wallet_ptr
 * @param {BBOOL} eip155_compatibility
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatVenachainWalletChangeEIP155Comp(BoatVenachainWallet *wallet_ptr, BBOOL eip155_compatibility)
{
    if (wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set EIP-155 Compatibility
    wallet_ptr->network_info.eip155_compatibility = eip155_compatibility;

    return BOAT_SUCCESS;
}
