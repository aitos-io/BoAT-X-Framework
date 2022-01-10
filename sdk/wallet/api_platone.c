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
#if PROTOCOL_USE_PLATONE == 1
#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "cJSON.h"

// As Platone's wallet structure is mostly the same as Ethereum, thus it
// re-use a lot of Ethereum's data structure and API. APIs not listed 
// here are compatible with Ethereum.

BOAT_RESULT BoatPlatoneTxInit(BoatPlatoneWallet *wallet_ptr,
							  BoatPlatoneTx *tx_ptr,
							  BBOOL is_sync_tx,
							  BCHAR *gasprice_str,
							  BCHAR *gaslimit_str,
							  BCHAR *recipient_str,
							  BoatPlatoneTxtype txtype)
{
    BOAT_RESULT result;

    if ((wallet_ptr == NULL) || (tx_ptr == NULL) || (recipient_str == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;
    memset(&tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields));

    // Re-use Ethereum transaction initialization
    result = BoatEthTxInit((BoatEthWallet *)wallet_ptr, (BoatEthTx *)tx_ptr,
                           is_sync_tx, gasprice_str, gaslimit_str, recipient_str);
    if (result != BOAT_SUCCESS)
    {
		BoatLog(BOAT_LOG_CRITICAL, "platone Tx init failed.");
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
        if(result_out->nodeInfo[i].IP != NULL)
        {
            BoatFree(result_out->nodeInfo[i].IP);
        }
    }
    result_out->num = 0;
    
}

BCHAR *web3_eth_call_getNodesManagerAddr(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_call *param_ptr,
                                    nodesResult *result_out)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    BCHAR *nodeManagerAddr = NULL;
    BoatFieldVariable prase_result = {NULL, 0};
    
    boat_try_declare;
    
    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_eth_call_cleanup);
    }

    web3intf_context_ptr->web3_message_id++;
    
    if( node_url_str == NULL || param_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_eth_call_cleanup);
    }
    
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;


                expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":"
				 "[{\"from\":\"%s\",\"to\":\"0x0000000000000000000000000000000000000011\",\"gas\":\"0x0\",\"gasPrice\":\"0x0\",\"value\":\"\",\"data\":\"%s\",\"txType\":9},\"%s\"],\"id\":%u}",
				  param_ptr->to,
                 param_ptr->data,
				 param_ptr->block_num_str,
				 web3intf_context_ptr->web3_message_id
				);

		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			// result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            	BoatFree(web3intf_context_ptr->web3_json_string_buf.field_ptr);
	
	            web3intf_context_ptr->web3_json_string_buf.field_len += WEB3_STRING_BUF_STEP_SIZE;
	            web3intf_context_ptr->web3_json_string_buf.field_ptr  = BoatMalloc(web3intf_context_ptr->web3_json_string_buf.field_len);
			if (web3intf_context_ptr->web3_json_string_buf.field_ptr ==  NULL)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_eth_call_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );
    // POST the REQUEST through curl

    result = RpcRequestSet( web3intf_context_ptr->rpc_context_ptr, node_url_str );
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_eth_call_cleanup);
    }
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, 
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_call_cleanup);
    }

    
    // return entire RESPONSE content	
	// return_value_ptr = rpc_response_str;

    web3_parse_json_result(rpc_response_str, "result", &prase_result);
    nodeManagerAddr = BoatMalloc(strlen((BCHAR*)(prase_result.field_ptr))/2);
    memset(nodeManagerAddr,0x00,strlen((BCHAR*)(prase_result.field_ptr))/2);
    // hex2array(prase_result.field_ptr+2,strlen((BCHAR*)(prase_result.field_ptr))-2,(BUINT8*)nodeManagerAddr);
    UtilityHexToBin(nodeManagerAddr,strlen((BCHAR*)(prase_result.field_ptr))/2,prase_result.field_ptr,TRIMBIN_TRIM_NO,BOAT_FALSE);
 // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;


                expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":"
				 "[{\"from\":\"%s\",\"to\":\"%s\",\"gas\":\"0x0\",\"gasPrice\":\"0x0\",\"value\":\"\",\"data\":\"0xd58800000000000000098b676574416c6c4e6f646573\",\"txType\":9},\"%s\"],\"id\":%u}",
				  param_ptr->to,
                    nodeManagerAddr,
				 param_ptr->block_num_str,
				 web3intf_context_ptr->web3_message_id
				);
                BoatFree(nodeManagerAddr);

		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			// result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
                BoatFree(web3intf_context_ptr->web3_json_string_buf.field_ptr);
	
	            web3intf_context_ptr->web3_json_string_buf.field_len += WEB3_STRING_BUF_STEP_SIZE;
	            web3intf_context_ptr->web3_json_string_buf.field_ptr  = BoatMalloc(web3intf_context_ptr->web3_json_string_buf.field_len);
			if (web3intf_context_ptr->web3_json_string_buf.field_ptr ==  NULL)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_eth_call_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );

    // POST the REQUEST through curl
    result = RpcRequestSet( web3intf_context_ptr->rpc_context_ptr, node_url_str );
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_eth_call_cleanup);
    }
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, 
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_call_cleanup);
    }
    
    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    web3_parse_json_result(rpc_response_str, "data", &prase_result);

    nodeManagerAddr = BoatMalloc(strlen((BCHAR*)(prase_result.field_ptr))/2);
    memset(nodeManagerAddr,0x00,strlen((BCHAR*)(prase_result.field_ptr))/2);
    // hex2array(prase_result.field_ptr+2,strlen((BCHAR*)(prase_result.field_ptr))-2,(BUINT8*)nodeManagerAddr);
    UtilityHexToBin(nodeManagerAddr,strlen((BCHAR*)(prase_result.field_ptr))/2,prase_result.field_ptr,TRIMBIN_TRIM_NO,BOAT_FALSE);
 // Construct the REQUEST
    
    // web3_parse_fatherNamejson_result(nodeManagerAddr,"data", "externalIP", &prase_result);
    nodeResFree(result_out);
    Platone_get_Nodeinfo(nodeManagerAddr,result_out);



    // Exceptional Clean Up
    boat_catch(web3_eth_call_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }
    if(nodeManagerAddr != NULL){
        BoatFree(nodeManagerAddr);
    }
    if(prase_result.field_ptr != NULL){
        BoatFree(prase_result.field_ptr);
        prase_result.field_len = 0;
    }
    
    return return_value_ptr;
}

BCHAR *BoatPlatoneCallContractFunc(BoatPlatoneTx *tx_ptr, BUINT8 *rlp_param_ptr,
								   BUINT32 rlp_param_len)
{
    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len*2 + 3]; // Compiler MUST support C99 to allow variable-size local array
	
    Param_eth_call param_eth_call;
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

    if (BOAT_SUCCESS != UtilityStringLenCheck((BCHAR*)rlp_param_ptr))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return NULL;
    }

    BCHAR recipient_hexstr[BOAT_PLATONE_ADDRESS_SIZE*2+3];
    
    UtilityBinToHex(recipient_hexstr, tx_ptr->rawtx_fields.recipient, BOAT_PLATONE_ADDRESS_SIZE,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_eth_call.to = recipient_hexstr;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_eth_call.gas      = "0x1fffff";
    param_eth_call.gasPrice = "0x8250de00";

    // Set function parameters
    UtilityBinToHex(data_str, rlp_param_ptr, rlp_param_len,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_eth_call.method_name_str = "eth_call";
    param_eth_call.data = data_str;
    param_eth_call.block_num_str = "latest";
    retval_str = web3_call(tx_ptr->wallet_ptr->web3intf_context_ptr,
                           tx_ptr->wallet_ptr->network_info.node_url_ptr,
                           &param_eth_call,&result);
    if(retval_str == NULL){
        BoatLog(BOAT_LOG_CRITICAL, "web3 call fail, result = %d ",result);
    }
    return retval_str;
}

BCHAR *BoatPlatoneCallContractGetNodesInfoFunc( BoatPlatoneTx *tx_ptr, BUINT8 *rlp_param_ptr,
									 BUINT32 rlp_param_len ,
                                     nodesResult *result_out)
{
    // *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[rlp_param_len*2 + 3]; // Compiler MUST support C99 to allow variable-size local array
	
    Param_eth_call param_eth_call;
    BCHAR *retval_str;

    if( rlp_param_ptr == NULL && rlp_param_len != 0 )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return NULL;
    }

	if (rlp_param_len > BOAT_STRING_MAX_LEN)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return NULL;
	}

    if (BOAT_SUCCESS != UtilityStringLenCheck((BCHAR*)rlp_param_ptr))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return NULL;
    }

    BCHAR recipient_hexstr[BOAT_PLATONE_ADDRESS_SIZE*2+3];
    
    UtilityBinToHex( recipient_hexstr, tx_ptr->wallet_ptr->account_info.address, BOAT_PLATONE_ADDRESS_SIZE,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE );
    param_eth_call.to = recipient_hexstr;
    
    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_eth_call.gas      = "0x0";
    param_eth_call.gasPrice = "0x0";

    // Set function parameters
    UtilityBinToHex( data_str, rlp_param_ptr, rlp_param_len,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE );
    param_eth_call.data = data_str;
    param_eth_call.block_num_str = "latest";
    retval_str = web3_eth_call_getNodesManagerAddr( tx_ptr->wallet_ptr->web3intf_context_ptr,
                                tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                &param_eth_call,
                                result_out);

    return retval_str;

}


BCHAR * BoatPlatoneGetNodesInfo(BoatPlatoneTx *tx_ptr,nodesResult *result_out)
{
    BCHAR *call_result_str = NULL;
    RlpEncodedStreamObject * rlp_stream_ptr;
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

    boat_try(RlpInitStringObject(&rlp_object_string_func_name, (BUINT8*)"getContractAddress", strlen("getContractAddress")));
    boat_try(0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_string_func_name));
    boat_try(RlpInitStringObject(&rlp_object_string_parm_name, (BUINT8*)"__sys_NodeManager", strlen("__sys_NodeManager")));
    boat_try(0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_string_parm_name));
    boat_try(RlpInitStringObject(&rlp_object_string_type_name, (BUINT8*)"latest", strlen("latest")));
    boat_try(0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_string_type_name));

    boat_try(RlpEncode(&rlp_object_list, NULL));

    rlp_stream_ptr = RlpGetEncodedStream(&rlp_object_list);

    call_result_str = BoatPlatoneCallContractGetNodesInfoFunc(tx_ptr, rlp_stream_ptr->stream_ptr, rlp_stream_ptr->stream_len,result_out);

    BoatLog(BOAT_LOG_CRITICAL, " BoatPlatoneCallContractGetNodesInfoFunc ok.");
    boat_catch(cleanup)
    {
        RlpRecursiveDeleteObject(&rlp_object_list);
        return(NULL);
    }
        
    RlpRecursiveDeleteObject(&rlp_object_list);
    return(call_result_str);

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
    BoatFieldMax32B   value;
    BoatFieldVariable data;
    BUINT64           tx_type_big;
    BOAT_RESULT       result;
   
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
    UtilityUint64ToBigend((BUINT8*)&tx_type_big, 0,  TRIMBIN_TRIM_NO);
    data.field_ptr = (BUINT8*)&tx_type_big;
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

BOAT_RESULT BoatPlatonePraseRpcResponseStringResult(const BCHAR *json_string, BoatFieldVariable *result_out)
{
    return web3_parse_json_result(json_string, "", result_out);
}

BOAT_RESULT BoatPlatonePraseRpcResponseResult(const BCHAR *json_string, 
										  	  const BCHAR *child_name, 
										  	  BoatFieldVariable *result_out)
{
    if (child_name == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
	return web3_parse_json_result(json_string, child_name, result_out);
}
#endif /* end of PROTOCOL_USE_PLATONE */
