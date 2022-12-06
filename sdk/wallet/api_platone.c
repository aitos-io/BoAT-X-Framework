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
#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "cJSON.h"

/**
 * @description:
 *  This function create BoatPlatoneWallet by walletindex and networkIndex.
 * @param {BUINT8} walletIndex
 * @param {BUINT8} networkIndex
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BoatPlatoneWallet *BoatPlatoneWalletInit(BUINT8 walletIndex, BUINT8 networkIndex)
{
    BoatPlatoneWallet *wallet_ptr;
    BOAT_RESULT result;
    BUINT8 pubkeyHash[32];
    BUINT8 hashLenDummy;

    wallet_ptr = BoatMalloc(sizeof(BoatPlatoneWallet));
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
    result = BoATPlatone_GetNetworkByIndex(&(wallet_ptr->network_info), networkIndex);
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
    memcpy(wallet_ptr->account_info.address, &pubkeyHash[32 - BOAT_PLATONE_ADDRESS_SIZE], BOAT_PLATONE_ADDRESS_SIZE);

    return wallet_ptr;
}

void BoatPlatoneWalletDeInit(BoatPlatoneWallet *wallet_ptr)
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

BOAT_RESULT BoatPlatoneTxInit(BoatPlatoneWallet *wallet_ptr,
							  BoatPlatoneTx *tx_ptr,
							  BBOOL is_sync_tx,
							  BCHAR *gasprice_str,
							  BCHAR *gaslimit_str,
							  BCHAR *recipient_str,
							  BoatPlatoneTxtype txtype)
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
        result = BoatPlatoneTxSetGasPrice(tx_ptr, &gasprice);
    }
    else
    {
        // To use the price obtained from network, simply call BoatTxSetGasPrice(NULL)
        result = BoatPlatoneTxSetGasPrice(tx_ptr, NULL);
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
    result = BoatPlatoneTxSetGasLimit(tx_ptr, &gaslimit);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatoneTxSetGasLimit failed.");
        return result;
    }

    // Initialize recipient
    BUINT8 recipient[BOAT_PLATONE_ADDRESS_SIZE];
    BUINT32 converted_len;
    converted_len = UtilityHexToBin(recipient, BOAT_PLATONE_ADDRESS_SIZE, recipient_str,
                                    TRIMBIN_TRIM_NO, BOAT_TRUE);
    if (converted_len == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "recipient Initialize failed.");
        return BOAT_ERROR_COMMON_UTILITY;
    }

    result = BoatPlatoneTxSetRecipient(tx_ptr, recipient);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatoneTxSetRecipient failed.");
        return result;
    }

    // Initialize value = 0
    result = BoatPlatoneTxSetValue(tx_ptr, NULL);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatPlatoneTxSetValue failed.");
        return result;
    }
    // Set transaction type
    result = BoatPlatoneTxSetTxtype(tx_ptr, txtype);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "platone set Tx type failed.");
        return result;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatPlatoneTxSetTxtype(BoatPlatoneTx *tx_ptr, BoatPlatoneTxtype txtype)
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->rawtx_fields.txtype = txtype;

    return BOAT_SUCCESS;
}

void nodeResFree(nodesResult *result_out)
{
    for (size_t i = 0; i < result_out->num; i++)
    {
        /* code */
        if (result_out->nodeInfo[i].IP != NULL)
        {
            BoatFree(result_out->nodeInfo[i].IP);
        }
    }
    BoatFree(result_out->nodeInfo);
    result_out->nodeInfo = NULL;
    result_out->num = 0;
}

BCHAR *web3_platone_call_getNodesManagerAddr(Web3IntfContext *web3intf_context_ptr,
                                             BCHAR *node_url_str,
                                             const Param_web3_call *param_ptr,
                                             nodesResult *result_out)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
    BBOOL malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    BCHAR *nodeManagerAddr = NULL;
    BoatFieldVariable parse_result = {NULL, 0};

    boat_try_declare;

    if (web3intf_context_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_platone_call_cleanup);
    }

    web3intf_context_ptr->web3_message_id++;

    if (node_url_str == NULL || param_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_platone_call_cleanup);
    }

    // Construct the REQUEST
    do
    {
        malloc_size_expand_flag = false;

        expected_string_size = snprintf(
            (BCHAR *)web3intf_context_ptr->web3_json_string_buf.field_ptr,
            web3intf_context_ptr->web3_json_string_buf.field_len,
            "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":"
            "[{\"from\":\"%s\",\"to\":\"0x0000000000000000000000000000000000000011\",\"gas\":\"0x0\",\"gasPrice\":\"0x0\",\"value\":\"\",\"data\":\"%s\",\"txType\":9},\"%s\"],\"id\":%u}",
            param_ptr->to,
            param_ptr->data,
            param_ptr->block_num_str,
            web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
        {
            // result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            BoatFree(web3intf_context_ptr->web3_json_string_buf.field_ptr);

            web3intf_context_ptr->web3_json_string_buf.field_len += WEB3_STRING_BUF_STEP_SIZE;
            web3intf_context_ptr->web3_json_string_buf.field_ptr = BoatMalloc(web3intf_context_ptr->web3_json_string_buf.field_len);
            if (web3intf_context_ptr->web3_json_string_buf.field_ptr == NULL)
            {
                BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_platone_call_cleanup);
            }
            malloc_size_expand_flag = true;
        }
    } while (malloc_size_expand_flag);
    // POST the REQUEST through curl

    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_platone_call_cleanup);
    }

    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                            (BUINT8 *)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                            expected_string_size,
                            (BOAT_OUT BUINT8 **)&rpc_response_str,
                            &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_platone_call_cleanup);
    }

    // return entire RESPONSE content
    // return_value_ptr = rpc_response_str;

    BoatPlatoneParseRpcResponseResult(rpc_response_str, "result", &parse_result);
    nodeManagerAddr = BoatMalloc(strlen((BCHAR *)(parse_result.field_ptr)) / 2);
    memset(nodeManagerAddr, 0x00, strlen((BCHAR *)(parse_result.field_ptr)) / 2);
    // hex2array(parse_result.field_ptr+2,strlen((BCHAR*)(parse_result.field_ptr))-2,(BUINT8*)nodeManagerAddr);
    UtilityHexToBin((BUINT8 *)nodeManagerAddr, strlen((BCHAR *)(parse_result.field_ptr)) / 2, (BCHAR *)parse_result.field_ptr, TRIMBIN_TRIM_NO, BOAT_FALSE);
    // Construct the REQUEST
    do
    {
        malloc_size_expand_flag = false;

        expected_string_size = snprintf(
            (BCHAR *)web3intf_context_ptr->web3_json_string_buf.field_ptr,
            web3intf_context_ptr->web3_json_string_buf.field_len,
            "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":"
            "[{\"from\":\"%s\",\"to\":\"%s\",\"gas\":\"0x0\",\"gasPrice\":\"0x0\",\"value\":\"\",\"data\":\"0xd58800000000000000098b676574416c6c4e6f646573\",\"txType\":9},\"%s\"],\"id\":%u}",
            param_ptr->to,
            nodeManagerAddr,
            param_ptr->block_num_str,
            web3intf_context_ptr->web3_message_id);
        BoatFree(nodeManagerAddr);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
        {
            // result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            BoatFree(web3intf_context_ptr->web3_json_string_buf.field_ptr);

            web3intf_context_ptr->web3_json_string_buf.field_len += WEB3_STRING_BUF_STEP_SIZE;
            web3intf_context_ptr->web3_json_string_buf.field_ptr = BoatMalloc(web3intf_context_ptr->web3_json_string_buf.field_len);
            if (web3intf_context_ptr->web3_json_string_buf.field_ptr == NULL)
            {
                BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_platone_call_cleanup);
            }
            malloc_size_expand_flag = true;
        }
    } while (malloc_size_expand_flag);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_platone_call_cleanup);
    }

    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                            (BUINT8 *)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                            expected_string_size,
                            (BOAT_OUT BUINT8 **)&rpc_response_str,
                            &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_platone_call_cleanup);
    }

    // return entire RESPONSE content
    return_value_ptr = rpc_response_str;

    BoatPlatoneParseRpcResponseResult(rpc_response_str, "data", &parse_result);

    nodeManagerAddr = BoatMalloc(strlen((BCHAR *)(parse_result.field_ptr)) / 2);
    memset(nodeManagerAddr, 0x00, strlen((BCHAR *)(parse_result.field_ptr)) / 2);
    // hex2array(parse_result.field_ptr+2,strlen((BCHAR*)(parse_result.field_ptr))-2,(BUINT8*)nodeManagerAddr);
    UtilityHexToBin((BUINT8 *)nodeManagerAddr, strlen((BCHAR *)(parse_result.field_ptr)) / 2, (BCHAR *)parse_result.field_ptr, TRIMBIN_TRIM_NO, BOAT_FALSE);
    // Construct the REQUEST

    // web3_parse_fatherNamejson_result(nodeManagerAddr,"data", "externalIP", &parse_result);
    nodeResFree(result_out);
    Platone_get_Nodeinfo(nodeManagerAddr, result_out);

    // Exceptional Clean Up
    boat_catch(web3_platone_call_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }
    if (nodeManagerAddr != NULL)
    {
        BoatFree(nodeManagerAddr);
    }
    if (parse_result.field_ptr != NULL)
    {
        BoatFree(parse_result.field_ptr);
        parse_result.field_len = 0;
    }

    return return_value_ptr;
}

BCHAR *BoatPlatoneCallContractFunc(BoatPlatoneTx *tx_ptr, BUINT8 *rlp_param_ptr,
                                   BUINT32 rlp_param_len)
{
    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len * 2 + 3]; // Compiler MUST support C99 to allow variable-size local array

    Param_web3_call param_platone_call;
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

    BCHAR recipient_hexstr[BOAT_PLATONE_ADDRESS_SIZE * 2 + 3];

    UtilityBinToHex(recipient_hexstr, tx_ptr->rawtx_fields.recipient, BOAT_PLATONE_ADDRESS_SIZE,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_platone_call.to = recipient_hexstr;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_platone_call.gas = "0x1fffff";
    param_platone_call.gasPrice = "0x8250de00";

    // Set function parameters
    UtilityBinToHex(data_str, rlp_param_ptr, rlp_param_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_platone_call.method_name_str = "eth_call";
    param_platone_call.data = data_str;
    param_platone_call.block_num_str = "latest";
    retval_str = web3_call(tx_ptr->wallet_ptr->web3intf_context_ptr,
                           tx_ptr->wallet_ptr->network_info.node_url_str,
                           &param_platone_call, &result);
    if (retval_str == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "web3 call fail, result = %d ", result);
    }
    return retval_str;
}

BCHAR *BoatPlatoneCallContractGetNodesInfoFunc(BoatPlatoneTx *tx_ptr, BUINT8 *rlp_param_ptr,
                                               BUINT32 rlp_param_len,
                                               nodesResult *result_out)
{
    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len * 2 + 3]; // Compiler MUST support C99 to allow variable-size local array

    Param_web3_call param_platone_call;
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

    BCHAR recipient_hexstr[BOAT_PLATONE_ADDRESS_SIZE * 2 + 3];

    UtilityBinToHex(recipient_hexstr, tx_ptr->wallet_ptr->account_info.address, BOAT_PLATONE_ADDRESS_SIZE,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_platone_call.to = recipient_hexstr;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_platone_call.gas = "0x0";
    param_platone_call.gasPrice = "0x0";

    // Set function parameters
    UtilityBinToHex(data_str, rlp_param_ptr, rlp_param_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_platone_call.data = data_str;
    param_platone_call.block_num_str = "latest";
    retval_str = web3_platone_call_getNodesManagerAddr(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                                       tx_ptr->wallet_ptr->network_info.node_url_str,
                                                       &param_platone_call,
                                                       result_out);

    return retval_str;
}

BCHAR *BoatPlatoneGetNodesInfo(BoatPlatoneTx *tx_ptr, nodesResult *result_out)
{
    BCHAR *call_result_str = NULL;
    RlpEncodedStreamObject *rlp_stream_ptr;
    RlpObject rlp_object_list;
    RlpObject rlp_object_txtype;
    BUINT64 txtype;
    RlpObject rlp_object_string_func_name;
    RlpObject rlp_object_string_parm_name;
    RlpObject rlp_object_string_type_name;
    boat_try_declare;

    boat_try(RlpInitListObject(&rlp_object_list));

    txtype = tx_ptr->rawtx_fields.txtype;
    boat_try(RlpInitStringObject(&rlp_object_txtype, UtilityChangeEndian(&txtype, sizeof(txtype)), sizeof(txtype)));
    boat_try(0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_txtype));

    boat_try(RlpInitStringObject(&rlp_object_string_func_name, (BUINT8 *)"getContractAddress", strlen("getContractAddress")));
    boat_try(0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_string_func_name));
    boat_try(RlpInitStringObject(&rlp_object_string_parm_name, (BUINT8 *)"__sys_NodeManager", strlen("__sys_NodeManager")));
    boat_try(0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_string_parm_name));
    boat_try(RlpInitStringObject(&rlp_object_string_type_name, (BUINT8 *)"latest", strlen("latest")));
    boat_try(0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_string_type_name));

    boat_try(RlpEncode(&rlp_object_list, NULL));

    rlp_stream_ptr = RlpGetEncodedStream(&rlp_object_list);

    call_result_str = BoatPlatoneCallContractGetNodesInfoFunc(tx_ptr, rlp_stream_ptr->stream_ptr, rlp_stream_ptr->stream_len, result_out);

    BoatLog(BOAT_LOG_CRITICAL, " BoatPlatoneCallContractGetNodesInfoFunc ok. num = %d", result_out->num);
    boat_catch(cleanup)
    {
        RlpRecursiveDeleteObject(&rlp_object_list);
        return (NULL);
    }

    RlpRecursiveDeleteObject(&rlp_object_list);
    return (call_result_str);
}

BOAT_RESULT BoatPlatoneSendRawtxWithReceipt(BOAT_INOUT BoatPlatoneTx *tx_ptr)
{
    BOAT_RESULT result = BOAT_ERROR;

    result = PlatoneSendRawtx(tx_ptr);

    if (result == BOAT_SUCCESS)
    {
        result = BoatPlatoneGetTransactionReceipt(tx_ptr);
    }

    return result;
}

BOAT_RESULT BoatPlatoneTxSend(BoatPlatoneTx *tx_ptr)
{
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (tx_ptr->is_sync_tx == BOAT_FALSE)
    {
        result = PlatoneSendRawtx(tx_ptr);
    }
    else
    {
        result = BoatPlatoneSendRawtxWithReceipt(tx_ptr);
    }

    return result;
}

BOAT_RESULT BoatPlatoneTransfer(BoatPlatoneTx *tx_ptr, BCHAR *value_hex_str)
{
    BoatFieldMax32B value;
    BoatFieldVariable data;
    BUINT64 tx_type_big;
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL || value_hex_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set nonce
    result = BoatPlatoneTxSetNonce(tx_ptr, BOAT_PLATONE_NONCE_AUTO);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }

    // Set value
    value.field_len = UtilityHexToBin(value.field, 32, value_hex_str,
                                      TRIMBIN_LEFTTRIM, BOAT_TRUE);
    result = BoatPlatoneTxSetValue(tx_ptr, &value);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }

    // Set data (contains txtype only)
    UtilityUint64ToBigend((BUINT8 *)&tx_type_big, 0, TRIMBIN_TRIM_NO);
    data.field_ptr = (BUINT8 *)&tx_type_big;
    data.field_len = sizeof(BUINT64);

    result = BoatPlatoneTxSetData(tx_ptr, &data);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }

    // Perform the transaction
    // NOTE: Field v,r,s are calculated automatically
    result = BoatPlatoneTxSend(tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatPlatoneParseRpcResponseStringResult(const BCHAR *json_string, BoatFieldVariable *result_out)
{
    return platone_parse_json_result(json_string, "", result_out);
}

BOAT_RESULT BoatPlatoneParseRpcResponseResult(const BCHAR *json_string,
                                              const BCHAR *child_name,
                                              BoatFieldVariable *result_out)
{
    if (child_name == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    return platone_parse_json_result(json_string, child_name, result_out);
}

/**
 * @description:
 *  This function changes the URL of platone wallet to new URL;
 * @param {BoatPlatoneWallet} *wallet_ptr
 * @param {BCHAR *} newUrl
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatPlatoneWalletChangeNodeUrl(BoatPlatoneWallet *wallet_ptr, BCHAR *newUrl)
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
 *  This function changes the chainID of platone wallet to new chainID;
 * @param {BoatPlatoneWallet} *wallet_ptr
 * @param {BUINT32} newChainID
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatPlatoneWalletChangeChainID(BoatPlatoneWallet *wallet_ptr, BUINT32 newChainID)
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
 *  This function changes the eip155 compatibility of platone wallet to new compatibility;
 * @param {BoatPlatoneWallet} *wallet_ptr
 * @param {BBOOL} eip155_compatibility
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatPlatoneWalletChangeEIP155Comp(BoatPlatoneWallet *wallet_ptr, BBOOL eip155_compatibility)
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

BCHAR *BoatPlatoneWalletGetBalance(BoatPlatoneWallet *wallet_ptr, BCHAR *alt_address_str)
{
    BUINT8 alt_address[BOAT_PLATONE_ADDRESS_SIZE]; // Binary altered address converted from alt_address_str
    BUINT8 *address_ptr;                           // Point to an address in binary format, either wallet
                                                   // owner's or the one converted from alt_address_str
    BCHAR address_str[43];                         // Address in string format, converted from address_ptr

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
        // NETWORK NEEDS PLATONE ADDRESS, WHICH IS COMPUTED FROM KEY
        address_ptr = wallet_ptr->account_info.address;
    }
    else
    {
        // Query specified altered address
        UtilityHexToBin(alt_address, BOAT_PLATONE_ADDRESS_SIZE, alt_address_str,
                        TRIMBIN_TRIM_NO, BOAT_TRUE);

        address_ptr = alt_address;
    }

    // Get balance from network
    // Return value of web3_getBalance() is balance in wei
    UtilityBinToHex(address_str, address_ptr, BOAT_PLATONE_ADDRESS_SIZE,
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

BOAT_RESULT BoatPlatoneTxSetNonce(BoatPlatoneTx *tx_ptr, BUINT64 nonce)
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

    if (nonce == BOAT_PLATONE_NONCE_AUTO)
    {
        /* Get transaction count from network
           Return value of web3_getTransactionCount() is transaction count */
        UtilityBinToHex(account_address_str, tx_ptr->wallet_ptr->account_info.address,
                        BOAT_PLATONE_ADDRESS_SIZE, BIN2HEX_LEFTTRIM_UNFMTDATA,
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
        result = BoatPlatoneParseRpcResponseStringResult(tx_count_str,
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

BOAT_RESULT BoatPlatoneTxSetGasPrice(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *gas_price_ptr)
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

        result = BoatPlatoneParseRpcResponseStringResult(gas_price_from_net_str,
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

BOAT_RESULT BoatPlatoneTxSetGasLimit(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
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

BOAT_RESULT BoatPlatoneTxSetRecipient(BoatPlatoneTx *tx_ptr, BUINT8 address[BOAT_PLATONE_ADDRESS_SIZE])
{
    if (tx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set recipient's address
    memcpy(&tx_ptr->rawtx_fields.recipient, address, BOAT_PLATONE_ADDRESS_SIZE);

    return BOAT_SUCCESS;
}

BOAT_RESULT BoatPlatoneTxSetValue(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *value_ptr)
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

BOAT_RESULT BoatPlatoneTxSetData(BoatPlatoneTx *tx_ptr, BoatFieldVariable *data_ptr)
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

BOAT_RESULT BoatPlatoneGetTransactionReceipt(BoatPlatoneTx *tx_ptr)
{
    BCHAR tx_hash_str[67];
    BCHAR *tx_status_str;
    Param_web3_getTransactionReceipt param_web3_getTransactionReceipt;
    BSINT32 tx_mined_timeout;

    BOAT_RESULT result = BOAT_SUCCESS;

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len,
                    BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    tx_mined_timeout = BOAT_PLATONE_WAIT_PENDING_TX_TIMEOUT;
    param_web3_getTransactionReceipt.method_name_str = "eth_getTransactionReceipt";
    param_web3_getTransactionReceipt.tx_hash_str = tx_hash_str;

    do
    {
        BoatSleep(BOAT_PLATONE_MINE_INTERVAL); // Sleep waiting for the block being mined
        tx_status_str = web3_getTransactionReceiptStatus(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                                         tx_ptr->wallet_ptr->network_info.node_url_str,
                                                         &param_web3_getTransactionReceipt, &result);
        if (tx_status_str == NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            break;
        }
        result = BoatPlatoneParseRpcResponseResult(tx_status_str, "status",
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
                BoatLog(BOAT_LOG_NORMAL, "Transaction has not got mined, requery after %d seconds.", BOAT_PLATONE_MINE_INTERVAL);
            }

            tx_mined_timeout -= BOAT_PLATONE_MINE_INTERVAL;
        }

    } while (tx_mined_timeout > 0);

    if (tx_mined_timeout <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "Wait for pending transaction timeout. This does not mean the transaction fails.");
        result = BOAT_ERROR_COMMON_TX_PENDING;
    }

    return result;
}
