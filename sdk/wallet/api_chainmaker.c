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

/*!@brief chainmaker wallet API for BoAT IoT SDK

@file
api_chainmaker.c defines the chainmaker wallet API for BoAT IoT SDK.
*/

/* self-header include */
#include "boatinternal.h"
#include "boatiotsdk.h"
#include "http2intf.h"
#include "common/result.pb-c.h"
#include "common/transaction.pb-c.h"
#include "boatchainmaker_v2/boatchainmaker.h"

/**
 * @description:
 *  This function init chainmaker wallet by keypair index and network index;
 * @param {BUINT8} keypairIndex
 * @param {BUINT8} networkIndex
 * @return {*}
 *  This function returns fabricwallet if successfully executed.
 *  Otherwise it returns NULL.
 * @author: aitos
 */
BoatChainmakerWallet *BoatChainmakerWalletInit(BUINT8 keypairIndex, BUINT8 networkIndex)
{
    BoatChainmakerWallet *wallet_ptr = NULL;
    BOAT_RESULT result = BOAT_SUCCESS;

    /* allocate wallet memory */
    wallet_ptr = BoatMalloc(sizeof(BoatChainmakerWallet));
    if (wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to malloc wallet memory.");
        return NULL;
    }
    result = BoATKeypair_GetKeypairByIndex(&(wallet_ptr->account_info.prikeyCtx), keypairIndex);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get keypair by index fail");
        return NULL;
    }

    result = BoATChainmaker_GetNetworkByIndex(&(wallet_ptr->network_info), networkIndex);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get network by index fail");
        return NULL;
    }

    wallet_ptr->http2Context_ptr = NULL;

    /* http2Context_ptr assignment */
    wallet_ptr->http2Context_ptr = http2Init();

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to set accountInfo|TlsUInfo|networkInfo.");
        return NULL;
    }

    return wallet_ptr;
}

void BoatChainmakerWalletDeInit(BoatChainmakerWallet *wallet_ptr)
{
    if (NULL == wallet_ptr)
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr needn't DeInit: wallet_ptr is NULL.");
        return;
    }

    /* account_info DeInit */
    if (wallet_ptr->account_info.prikeyCtx.keypair_name != NULL)
    {
        BoatFree(wallet_ptr->account_info.prikeyCtx.keypair_name);
        wallet_ptr->account_info.prikeyCtx.keypair_name = NULL;
    }

    /* network_info DeInit */
    if (wallet_ptr->network_info.node_url != NULL)
    {
        BoatFree(wallet_ptr->network_info.node_url);
        wallet_ptr->network_info.node_url = NULL;
    }

    if (wallet_ptr->network_info.host_name != NULL)
    {
        BoatFree(wallet_ptr->network_info.host_name);
        wallet_ptr->network_info.host_name = NULL;
    }

    if (wallet_ptr->network_info.chain_id != NULL)
    {
        BoatFree(wallet_ptr->network_info.chain_id);
        wallet_ptr->network_info.chain_id = NULL;
    }

    if (wallet_ptr->network_info.org_id != NULL)
    {
        BoatFree(wallet_ptr->network_info.org_id);
        wallet_ptr->network_info.org_id = NULL;
    }

    /* http2Context DeInit */
    http2DeInit(wallet_ptr->http2Context_ptr);

    /* wallet_ptr DeInit */
    BoatFree(wallet_ptr);

    /* set NULL after free completed */
    wallet_ptr = NULL;
}

void hex_to_str(BUINT8 *hex_data, BUINT32 hex_len, BCHAR *str_data)
{
    BUINT32 num;
    BUINT32 pos_index = 0;

    for (num = 0; num < hex_len; num++)
    {
        sprintf(str_data + pos_index, "%02x", hex_data[num]);
        pos_index += 2;
    }
}

BOAT_RESULT get_tx_id(BCHAR *tx_id_ptr)
{
    // 32 byte randrom generate
    BoatFieldMax32B random_data;
    random_data.field_len = 32;
    BOAT_RESULT result = BOAT_SUCCESS;

    result = BoatRandom(random_data.field, random_data.field_len, NULL);
    if (result != BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }

    hex_to_str(random_data.field, random_data.field_len, tx_id_ptr);
    return result;
}

BOAT_RESULT BoatChainmakerTxInit(const BoatChainmakerWallet *wallet_ptr, BoatChainmakerTx *tx_ptr, BUINT64 gas_limit)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if ((tx_ptr == NULL) || (wallet_ptr == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerTxInit paramters cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = (BoatChainmakerTx *)wallet_ptr;
    tx_ptr->gas_limit = gas_limit;
    return result;
}

__BOATSTATIC BOAT_RESULT BoatChainmakerTxRequest(BoatChainmakerTx *tx_ptr, Common__TxResponse **tx_response, BBOOL sync_result)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    Common__TxResponse *tx_response_ptr = NULL;
    Http2Response http2_response = {0, NULL};

    if (tx_ptr == NULL || tx_ptr->wallet_ptr->http2Context_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->nodeUrl = tx_ptr->wallet_ptr->network_info.node_url;

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->hostName = tx_ptr->wallet_ptr->network_info.host_name;
    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_len = tx_ptr->wallet_ptr->network_info.ca_tls_cert_content.length + 1;
    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_ptr = BoatMalloc(((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_len);
    memset(((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_ptr, 0x00, ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_len);
    memcpy(((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_ptr, tx_ptr->wallet_ptr->network_info.ca_tls_cert_content.content, tx_ptr->wallet_ptr->network_info.ca_tls_cert_content.length);

#if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
    if (((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr != NULL)
    {
        BoatFree(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr);
    }
    ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_len = tx_ptr->wallet_ptr->network_info.client_tls_privkey_data.value_len + 1;
    ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr = BoatMalloc(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_len);
    strcpy((BCHAR *)((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsPrikey.field_ptr, (BCHAR *)tx_ptr->wallet_ptr->network_info.client_tls_privkey_data.value);
    if (((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr != NULL)
    {
        BoatFree(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr);
    }
    ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_len = tx_ptr->wallet_ptr->network_info.client_tls_cert_content.length + 1;
    ((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr = BoatMalloc(((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_len);
    strcpy((BCHAR *)((http2IntfContext *)(tx_ptr->wallet_ptr->http2Context_ptr))->tlsCert.field_ptr, tx_ptr->wallet_ptr->network_info.client_tls_cert_content.content);
#endif
#endif
    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->pathTmp = "/api.RpcNode/SendRequest";
    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->parseDataPtr = &http2_response;

    result = http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
    if (result == BOAT_SUCCESS)
    {
        if (http2_response.http2Res != NULL)
        {
            tx_response_ptr = common__tx_response__unpack(NULL, http2_response.httpResLen - 5, http2_response.http2Res + 5);
            if (tx_response_ptr != NULL)
            {
                BoatLog(BOAT_LOG_NORMAL, "[http2] respond received.");
                *tx_response = tx_response_ptr;
            }
            else
            {
                BoatLog(BOAT_LOG_NORMAL, "[http2] respond unpack NULL");
                *tx_response = NULL;
                result = BOAT_ERROR_RLP_ENCODER_FAIL;
            }
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "[http2] respond NULL");
            result = BOAT_ERROR_HTTP2_RECEIVE_FAIL;
        }
    }

    if (http2_response.http2Res != NULL)
    {
        BoatFree(http2_response.http2Res);
        http2_response.http2Res = NULL;
        http2_response.httpResLen = 0;
    }

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    if (((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_ptr != NULL)
    {
        BoatFree(((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_ptr);
        ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_ptr = NULL;
    }
    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCAchain.field_len = 0;

#if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
    if (((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsPrikey.field_ptr != NULL)
    {
        BoatFree(((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsPrikey.field_ptr);
        ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsPrikey.field_ptr = NULL;
    }
    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsPrikey.field_len = 0;

    if (((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCert.field_ptr != NULL)
    {
        BoatFree(((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCert.field_ptr);
        ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCert.field_ptr = NULL;
    }
    ((http2IntfContext *)tx_ptr->wallet_ptr->http2Context_ptr)->tlsCert.field_len = 0;
#endif

#endif
    return result;
}

BOAT_RESULT BoatChainmakerAddTxParam(BoatChainmakerTx *tx_ptr, BUINT8 length, const BCHAR *arg, ...)
{
    va_list ap;

    BCHAR *args = NULL;
    BOAT_RESULT result = BOAT_SUCCESS;

    if ((tx_ptr == NULL) || (length > BOAT_HLCHAINMAKER_ARGS_MAX_NUM) || ((length & 0x01) == 1))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->trans_para.n_parameters = 0;
    va_start(ap, arg);
    if (arg != NULL)
    {
        tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].key = (BCHAR *)arg;
    }

    for (int i = 1; i < length; i++)
    {
        args = va_arg(ap, BCHAR *);
        if (args == NULL)
        {
            BoatLog(BOAT_LOG_CRITICAL, "variable parameter cannot be NULL.");
            return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
        }
        if (i & 0x01)
        {
            tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].value.field_ptr = (BCHAR *)args;
            tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].value.field_len = strlen(args);
            tx_ptr->trans_para.n_parameters++;
        }
        else
        {
            tx_ptr->trans_para.parameters[tx_ptr->trans_para.n_parameters].key = (BCHAR *)args;
        }
    }

    va_end(ap);
    return result;
}

BOAT_RESULT BoatChainmakerContractInvoke(BoatChainmakerTx *tx_ptr, char *method, char *contract_name, bool sync_result, BoatResponseData *response_data)
{
    BUINT32 i;
    BUINT32 len;
    Common__TxResponse *tx_response = NULL;
    Common__ContractResult *contract_result = NULL;
    BCHAR invoke_tx_id[BOAT_TXID_LEN] = {0};
    BCHAR query_systm_tx_id[BOAT_TXID_LEN] = {0};

    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;

    TxType tx_type = TXTYPE_INVOKE_USER_CONTRACT;
    if ((tx_ptr == NULL) || (method == NULL) || (contract_name == NULL) || (response_data == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, BoatHlchainmakerContractInvoke);
    }

    result = get_tx_id(invoke_tx_id);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get_tx_id failed");
        boat_throw(result, BoatHlchainmakerContractInvoke);
    }

    invoke_tx_id[BOAT_TXID_LEN - 1] = 0;
    result = hlchainmakerTransactionPacked(tx_ptr, method, contract_name, tx_type, invoke_tx_id);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "hlchainmakerTransactionPacked failed");
        boat_throw(result, BoatHlchainmakerContractInvoke);
    }

    result = BoatChainmakerTxRequest(tx_ptr, &tx_response, BOAT_FALSE);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerTxRequest failed");
        boat_throw(result, BoatHlchainmakerContractInvoke);
    }

    response_data->code = tx_response->code;
    if (strlen(tx_response->message) < BOAT_RESPONSE_MESSAGE_MAX_LEN)
    {
        memset(response_data->message, 0, BOAT_RESPONSE_MESSAGE_MAX_LEN);
        strcpy(response_data->message, tx_response->message);
    }
    strcpy(response_data->contract_result, "NULL");

    if (tx_response != NULL)
    {
        common__tx_response__free_unpacked(tx_response, NULL);
    }

    if (response_data->code != BOAT_SUCCESS)
    {
        strcpy(response_data->contract_result, tx_response->contract_result);
        BoatLog(BOAT_LOG_CRITICAL, "invoke without sync failed");
        boat_throw(result, BoatHlchainmakerContractInvoke);
    }

    if (sync_result)
    {
        for (i = 0; i < BOAT_RETRY_CNT; i++)
        {
            result = get_tx_id(query_systm_tx_id);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_CRITICAL, "get_tx_id failed");
                boat_throw(result, BoatHlchainmakerContractInvoke);
            }

            BoatChainmakerAddTxParam(tx_ptr, 2, "txId", invoke_tx_id);
#ifdef CHAINMAKER_V1
            result = hlchainmakerTransactionPacked(tx_ptr, "GET_TX_BY_TX_ID", "SYSTEM_CONTRACT_QUERY", TXTYPE_QUERY_SYSTEM_CONTRACT, query_systm_tx_id);
#endif

#ifdef CHAINMAKER_V2
            result = hlchainmakerTransactionPacked(tx_ptr, "GET_TX_BY_TX_ID", "CHAIN_QUERY", TXTYPE_QUERY_USER_CONTRACT, query_systm_tx_id);
#endif
            if (result == BOAT_SUCCESS)
            {
                result = BoatChainmakerTxRequest(tx_ptr, &tx_response, BOAT_TRUE);
                if (result != BOAT_SUCCESS)
                {
                    BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerTxRequest sync failed");
                }

                response_data->code = tx_response->contract_result->code;
                memset(response_data->message, 0, BOAT_RESPONSE_MESSAGE_MAX_LEN);
                strcpy(response_data->message, tx_response->contract_result->message);

                if (response_data->code == BOAT_SUCCESS)
                {
#ifdef CHAINMAKER_V1
                    Common__TransactionInfo *transactation_info = common__transaction_info__unpack(NULL, tx_response->contract_result->result.len, tx_response->contract_result->result.data);
                    if (tx_response != NULL)
                    {
                        common__tx_response__free_unpacked(tx_response, NULL);
                    }
                    if (transactation_info != NULL)
                    {
                        if (transactation_info->transaction->result->code == BOAT_SUCCESS)
                        {
                            len = sprintf(response_data->contract_result, "gas_used:%lld ", transactation_info->transaction->result->contract_result->gas_used);
                            common__transaction_info__free_unpacked(transactation_info, NULL);
                            break;
                        }
                        common__transaction_info__free_unpacked(transactation_info, NULL);
                    }

#endif
#ifdef CHAINMAKER_V2
                    Common__TransactionInfoWithRWSet *transaction_info_with_rwset = common__transaction_info_with_rwset__unpack(NULL, tx_response->contract_result->result.len, tx_response->contract_result->result.data);
                    if (tx_response != NULL)
                    {
                        common__tx_response__free_unpacked(tx_response, NULL);
                    }
                    if (transaction_info_with_rwset != NULL)
                    {
                        if (transaction_info_with_rwset->transaction->result->code == BOAT_SUCCESS)
                        {
                            len = sprintf(response_data->contract_result, "gas_used:%lld ", transaction_info_with_rwset->transaction->result->contract_result->gas_used);

                            if (transaction_info_with_rwset->transaction->result->contract_result->n_contract_event  != 0)
                            {
                                len += sprintf(response_data->contract_result + len, "contract_event:<");
                                for (i = 0; i < transaction_info_with_rwset->transaction->result->contract_result->n_contract_event; i++)
                                {
                                    len += sprintf(response_data->contract_result + len, "topic:\"%s\"", transaction_info_with_rwset->transaction->result->contract_result->contract_event[i]->topic);
                                    len += sprintf(response_data->contract_result + len, " tx_id:\"%s\"", transaction_info_with_rwset->transaction->result->contract_result->contract_event[i]->tx_id);
                                    len += sprintf(response_data->contract_result + len, " contract_name:\"%s\"", transaction_info_with_rwset->transaction->result->contract_result->contract_event[i]->contract_name);
                                    len += sprintf(response_data->contract_result + len, " contract_version:\"%s\"", transaction_info_with_rwset->transaction->result->contract_result->contract_event[i]->contract_version);

                                    for (int n = 0; n < transaction_info_with_rwset->transaction->result->contract_result->contract_event[i]->n_event_data; n++)
                                    {
                                        len += sprintf(response_data->contract_result + len, " event_data:\"%s\"", transaction_info_with_rwset->transaction->result->contract_result->contract_event[i]->event_data[n]);
                                    }
                                    len += sprintf(response_data->contract_result + len, ">");
                                }
                            }
                          
                            common__transaction_info_with_rwset__free_unpacked(transaction_info_with_rwset, NULL);
                            break;
                        }
                        common__transaction_info_with_rwset__free_unpacked(transaction_info_with_rwset, NULL);
                    }
#endif
                }
            }
            BoatSleep(BOAT_CHAINMAKER_MINE_INTERVAL);
        }
    }

    /* boat catch handle */
    boat_catch(BoatHlchainmakerContractInvoke)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    return result;
}

BOAT_RESULT BoatChainmakerContractQuery(BoatChainmakerTx *tx_ptr, char *method, char *contract_name, BoatResponseData *response_data)
{
    BUINT32 len = 0;
    TxType tx_type = TXTYPE_QUERY_USER_CONTRACT;
    Common__TxResponse *tx_response = NULL;
    BCHAR query_tx_id[BOAT_TXID_LEN] = {0};

    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;

    if ((tx_ptr == NULL) || (method == NULL) || (contract_name == NULL) || (response_data == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, BoatHlchainmakerContractQuery_exception);
    }
    BoatLog(BOAT_LOG_NORMAL, "Submit will execute...");

    get_tx_id(query_tx_id);
    query_tx_id[BOAT_TXID_LEN - 1] = 0;

    result = hlchainmakerTransactionPacked(tx_ptr, method, contract_name, tx_type, query_tx_id);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "hlchainmakerTransactionPacked failed");
        boat_throw(result, BoatHlchainmakerContractQuery_exception);
    }

    result = BoatChainmakerTxRequest(tx_ptr, &tx_response, BOAT_FALSE);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerTxRequest failed");
        boat_throw(result, BoatHlchainmakerContractQuery_exception);
    }

    if (tx_response == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "tx_response is NULL");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, BoatHlchainmakerContractQuery_exception);
    }

    response_data->code = tx_response->code;
    if (strlen(tx_response->message) > BOAT_RESPONSE_MESSAGE_MAX_LEN)
    {
        BoatLog(BOAT_LOG_CRITICAL, "tx_response->message is too long");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlchainmakerContractQuery_exception);
    }

    memset(response_data->message, 0, BOAT_RESPONSE_MESSAGE_MAX_LEN);
    memcpy(response_data->message, tx_response->message, strlen(tx_response->message));

    memset(response_data->contract_result, 0, BOAT_RESPONSE_CONTRACT_RESULT_MAX_LEN);

    if (response_data->code == BOAT_SUCCESS)
    {
        if (tx_response->contract_result->code == BOAT_SUCCESS)
        {
            if (tx_response->contract_result->result.len != 0)
            {
                if (tx_response->contract_result->result.len > BOAT_RESPONSE_CONTRACT_RESULT_MAX_LEN)
                {
                    BoatLog(BOAT_LOG_CRITICAL, "tx_response->contract_result->result.datais too long");
                    boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, BoatHlchainmakerContractQuery_exception);
                }
                memcpy(response_data->contract_result, tx_response->contract_result->result.data, tx_response->contract_result->result.len);
            }
        }
        len = tx_response->contract_result->result.len;
        len += sprintf(response_data->contract_result + len, " gas_used: %lld", tx_response->contract_result->gas_used);
#ifdef CHAINMAKER_V2
        sprintf(response_data->contract_result + len, " tx_id:\"%s\"", tx_response->tx_id);
#endif
    }
    else
    {
        memcpy(response_data->contract_result, "NULL", strlen("NULL"));
    }

    /* boat catch handle */
    boat_catch(BoatHlchainmakerContractQuery_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    if (tx_response != NULL)
    {
        common__tx_response__free_unpacked(tx_response, NULL);
    }
    return result;
}
