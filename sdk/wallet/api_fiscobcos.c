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

/*!@brief FISCOBCOS wallet API for BoAT IoT SDK

@file
boatFiscobcoswallet.c defines the FISCOBCOS wallet API for BoAT IoT SDK.
*/

#include "boatconfig.h"
#include "boatinternal.h"
#if PROTOCOL_USE_FISCOBCOS == 1
#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"
#include "cJSON.h"

// As FISCOBCOS's wallet structure is mostly the same as Ethereum, thus it
// re-use a lot of Ethereum's data structure and API.
// APIs not listed here are compatible with Ethereum.


BOAT_RESULT BoatFiscobcosTxInit(BoatFiscobcosWallet *wallet_ptr,
							    BoatFiscobcosTx *tx_ptr,
							    BBOOL is_sync_tx,
							    BCHAR *gasprice_str,
							    BCHAR *gaslimit_str,
							    BCHAR *recipient_str,
								BCHAR *chainid_str,
								BCHAR *groupid_str)
{
	BCHAR   *retval_str = NULL;
    BOAT_RESULT result;
    BUINT16     i = 0;

    if ((wallet_ptr == NULL) || (tx_ptr == NULL) || (gasprice_str == NULL) || (recipient_str == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;
    memset(&tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields));

    // Set synchronous transaction flag
    tx_ptr->is_sync_tx = is_sync_tx;
    
    // Initialize gasprice
	BoatFieldMax32B gasprice;
	gasprice.field_len = UtilityHexToBin(gasprice.field, 32, gasprice_str, TRIMBIN_LEFTTRIM, BOAT_TRUE);
	result = BoatFiscobcosTxSetGasPrice(tx_ptr, &gasprice);
	if (result != BOAT_SUCCESS)
    {
		BoatLog(BOAT_LOG_CRITICAL, "BoatFiscobcosTxSetGasPrice failed.");
        return result;
    }
	
    // Initialize gaslimit
    BoatFieldMax32B gaslimit;
    gaslimit.field_len = UtilityHexToBin(gaslimit.field, 32, gaslimit_str, TRIMBIN_LEFTTRIM, BOAT_TRUE);
    result = BoatFiscobcosTxSetGasLimit(tx_ptr, &gaslimit);
    if (result != BOAT_SUCCESS)
    {
		BoatLog(BOAT_LOG_CRITICAL, "BoatFiscobcosTxSetGasLimit failed.");
        return result;
    }

    // Initialize recipient
	BUINT32 converted_len;
    BUINT8  recipient[BOAT_FISCOBCOS_ADDRESS_SIZE];
    converted_len = UtilityHexToBin(recipient, BOAT_FISCOBCOS_ADDRESS_SIZE, 
									recipient_str, TRIMBIN_TRIM_NO, BOAT_TRUE);
    if (converted_len == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "recipient Initialize failed.");
		return BOAT_ERROR_COMMON_UTILITY;
    }

    result = BoatFiscobcosTxSetRecipient(tx_ptr, recipient);
    if (result != BOAT_SUCCESS)
    {
		BoatLog(BOAT_LOG_CRITICAL, "BoatFiscobcosTxSetRecipient failed.");
        return result;
    }
	
	//chainid
	tx_ptr->rawtx_fields.chainid.field_len    = UtilityHexToBin(tx_ptr->rawtx_fields.chainid.field, 32, 
															    chainid_str, TRIMBIN_LEFTTRIM, BOAT_TRUE);
	if (tx_ptr->rawtx_fields.chainid.field_len == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "chainid Initialize failed.");
		return BOAT_ERROR_COMMON_UTILITY;
    }
	
	//groupid
	tx_ptr->rawtx_fields.groupid.field_len    = UtilityHexToBin(tx_ptr->rawtx_fields.groupid.field, 32, 
																groupid_str, TRIMBIN_LEFTTRIM, BOAT_TRUE);
	if (tx_ptr->rawtx_fields.groupid.field_len == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "groupid Initialize failed.");
		return BOAT_ERROR_COMMON_UTILITY;
    }
	
	// Initialize blocklimit
	// blocklimit should be greater than current blocknumber, 
	// and less than current blocknumber plus 1000.
	retval_str = BoatFiscobcosGetBlockNumber(tx_ptr);
	result     = BoatFiscobcosPraseRpcResponseStringResult(retval_str, 
											    	 	   &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatFiscobcosGetBlockNumber failed.");
        return result;
	}

	tx_ptr->rawtx_fields.blocklimit.field_len = \
							UtilityHexToBin(tx_ptr->rawtx_fields.blocklimit.field, 32, 
											(BCHAR *)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
											TRIMBIN_LEFTTRIM, BOAT_TRUE);
	BoatLog(BOAT_LOG_CRITICAL, "%s", tx_ptr->rawtx_fields.blocklimit.field);
	if (tx_ptr->rawtx_fields.blocklimit.field_len == 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "blocklimit Initialize failed.");
        return BOAT_ERROR_COMMON_UTILITY;
	}

	//convert to bigendian uint256
	BoatFieldMax32B blocklimitTmp;
	memset(&blocklimitTmp, 0, sizeof(BoatFieldMax32B));
	for (i = 0; i < tx_ptr->rawtx_fields.blocklimit.field_len; i++)
	{
		blocklimitTmp.field[31 - i] = tx_ptr->rawtx_fields.blocklimit.field[tx_ptr->rawtx_fields.blocklimit.field_len - i - 1];
	}
	BoatLog_hexdump(BOAT_LOG_NORMAL, "blocklimitTmp1:", blocklimitTmp.field, 32);
	
	//convert bigendian uint256 to bignumber
	utilityBignum256 convertTmp;
	BUINT32 blockLimitOffset = 500; //value should rangle of 1 ~ 1000
	UtilityReadBigendToBignum(&blocklimitTmp.field[0], &convertTmp);
	
	//execute bignumber plus uint
	for (i = 0; i < 9; i++)
	{
		blockLimitOffset += convertTmp.val[i];
		convertTmp.val[i] = blockLimitOffset & 0x3FFFFFFF;
		blockLimitOffset >>= 30;
	}
	
	//convert bignumber to bigendian uint256
	UtilityWriteBignumToBigend(&convertTmp, &blocklimitTmp.field[0]);	
	BoatLog_hexdump(BOAT_LOG_NORMAL, "blocklimitTmp2:", blocklimitTmp.field, 32);
	memset(tx_ptr->rawtx_fields.blocklimit.field, 0, 32);
	
	//convert bigendian uint256 to foregone
	for (i = 0; i < 32; i++)
	{
		if (blocklimitTmp.field[i] != 0x00)
		{
			blocklimitTmp.field_len = 32 - i; //compute valid data length
			break;
		}
	}
	tx_ptr->rawtx_fields.blocklimit.field_len = blocklimitTmp.field_len;//update data length
	for (i = 0; i < tx_ptr->rawtx_fields.blocklimit.field_len; i++)
	{
		tx_ptr->rawtx_fields.blocklimit.field[i] = blocklimitTmp.field[32 - tx_ptr->rawtx_fields.blocklimit.field_len + i];
	}
	
	// Initialize value = 0
	//FISCO BCOS DOES NOT SET VALUE, IT'S DE-COINIZED
    result = BoatFiscobcosTxSetValue(tx_ptr, NULL);
    if (result != BOAT_SUCCESS)
    {
		BoatLog(BOAT_LOG_CRITICAL, "BoatFiscobcosTxSetValue failed.");
        return result;
    }

    return BOAT_SUCCESS;
}


BOAT_RESULT BoatFiscobcosTxSetNonce(BoatFiscobcosTx *tx_ptr, BUINT64 nonce)
{
	BOAT_RESULT result = BOAT_SUCCESS;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

	if (nonce == BOAT_FISCOBCOS_NONCE_AUTO)
	{
		tx_ptr->rawtx_fields.nonce.field_len = 32;
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

BOAT_RESULT BoatFiscobcosTxSetGasPrice(BoatFiscobcosTx *tx_ptr, BoatFieldMax32B *gas_price_ptr)
{
    if ((tx_ptr == NULL) || (gas_price_ptr == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // Set gasprice
	memcpy(&tx_ptr->rawtx_fields.gasprice, gas_price_ptr, sizeof(BoatFieldMax32B));
	
	return BOAT_SUCCESS;
}


BOAT_RESULT BoatFiscobcosSendRawtxWithReceipt(BOAT_INOUT BoatFiscobcosTx *tx_ptr)
{
    BOAT_RESULT result = BOAT_ERROR;

    result = FiscobcosSendRawtx(tx_ptr);

    if (result == BOAT_SUCCESS)
    {
        result = BoatFiscobcosGetTransactionReceipt(tx_ptr);
    }

    return result;
}

BOAT_RESULT BoatFiscobcosTxSend(BoatFiscobcosTx *tx_ptr)
{
    BOAT_RESULT result;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }


    if (tx_ptr->is_sync_tx == BOAT_FALSE)
    {
        result = FiscobcosSendRawtx(tx_ptr);
    }
    else
    {
        result = BoatFiscobcosSendRawtxWithReceipt(tx_ptr);
    }
    
    return result;
}


BCHAR *BoatFiscobcosCallContractFunc(BoatFiscobcosTx *tx_ptr, BCHAR *func_proto_str,
									 BUINT8 *func_param_ptr, BUINT32 func_param_len)
{
    Param_fiscobcos_call param_fiscobcos_call;
	BUINT8 function_selector[32];
	BUINT8 hashLenDummy;
	BCHAR  groupid_hexstr[67];
	BCHAR  from_hexstr[BOAT_FISCOBCOS_ADDRESS_SIZE * 2 + 3];
	BCHAR  to_hexstr[BOAT_FISCOBCOS_ADDRESS_SIZE * 2 + 3];
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

	//set groupid
	memset(groupid_hexstr, 0, sizeof(groupid_hexstr));
	UtilityBinToHex(groupid_hexstr, tx_ptr->rawtx_fields.groupid.field, tx_ptr->rawtx_fields.groupid.field_len,
					BIN2HEX_LEFTTRIM_QUANTITY, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE);
	param_fiscobcos_call.groupid = groupid_hexstr;
	//set from
	memset(from_hexstr, 0, sizeof(from_hexstr));
	UtilityBinToHex(from_hexstr, tx_ptr->wallet_ptr->account_info.address, BOAT_FISCOBCOS_ADDRESS_SIZE,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
	 param_fiscobcos_call.from  = from_hexstr;
	//set to
	 memset(to_hexstr, 0, sizeof(to_hexstr));
    UtilityBinToHex(to_hexstr, tx_ptr->rawtx_fields.recipient, BOAT_FISCOBCOS_ADDRESS_SIZE, 
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_fiscobcos_call.to  = to_hexstr;
	//set value
	param_fiscobcos_call.value = "";
	//set data
	memset(data_hexstr, 0, sizeof(data_hexstr));
	BoatHash(BOAT_HASH_KECCAK256, (BUINT8*)func_proto_str, 
			 strlen(func_proto_str), function_selector, &hashLenDummy, NULL);
    UtilityBinToHex(data_hexstr, function_selector, 4,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES,  BOAT_FALSE);
	UtilityBinToHex(data_hexstr+10, func_param_ptr, func_param_len,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE);
	param_fiscobcos_call.data  = data_hexstr;

    retval_str = web3_fiscobcos_call(tx_ptr->wallet_ptr->web3intf_context_ptr,
									 tx_ptr->wallet_ptr->network_info.node_url_ptr,
									 &param_fiscobcos_call,&result);
	if(retval_str == NULL){
		BoatLog(BOAT_LOG_CRITICAL, "web3 fiscobcos call fail ,result = %d .",result);
	}

    return retval_str;
}


BOAT_RESULT BoatFiscobcosGetTransactionReceipt(BoatFiscobcosTx *tx_ptr)
{
    Param_fiscobcos_getTransactionReceipt param_fiscobcos_getTransactionReceipt;
	BCHAR   groupid_hexstr[67];
	BCHAR   txhash_hexstr[67];
	BCHAR   *tx_status_str;
    BSINT32 tx_mined_timeout;
    BOAT_RESULT result = BOAT_SUCCESS;

    tx_mined_timeout = BOAT_FISCOBCOS_WAIT_PENDING_TX_TIMEOUT;
	
	//set groupid
	memset(groupid_hexstr, 0, sizeof(groupid_hexstr));
	UtilityBinToHex(groupid_hexstr, tx_ptr->rawtx_fields.groupid.field, tx_ptr->rawtx_fields.groupid.field_len,
					BIN2HEX_LEFTTRIM_QUANTITY, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE);
	param_fiscobcos_getTransactionReceipt.groupid = groupid_hexstr;
	//set txHash
	memset(txhash_hexstr, 0, sizeof(txhash_hexstr));
	UtilityBinToHex(txhash_hexstr, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len,
					BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);
    param_fiscobcos_getTransactionReceipt.tx_hash_str = txhash_hexstr;

    do{
        BoatSleep(BOAT_FISCOBCOS_MINE_INTERVAL); // Sleep waiting for the block being mined 
        tx_status_str = web3_fiscobcos_getTransactionReceiptStatus(tx_ptr->wallet_ptr->web3intf_context_ptr,
						tx_ptr->wallet_ptr->network_info.node_url_ptr,
						&param_fiscobcos_getTransactionReceipt,&result);
		if(tx_status_str == NULL){
			BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            break;
		}
		// "status" == null : the transaction is pending, the result is BOAT_ERROR
		// todo: need to change web3_parse_json_result() 
		result = BoatFiscobcosPraseRpcResponseResult(tx_status_str, "status", 
													 &tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
		
        if (result != BOAT_SUCCESS)
		{
            BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            result = BOAT_ERROR_WALLET_RESULT_PRASE_FAIL;
            break;
        }
        else
        {
            // tx_status_str == "0x0": the transaction is successfully mined
            // tx_status_str is ohters: error number, for detail, see https://fisco-bcos-documentation.readthedocs.io/zh_CN/latest/docs/api.html#
            if (tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr[0] != '\0')
            {
                if (strcmp((BCHAR*)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr, "0x0") == 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, "Transaction has got mined.");
					break;
                }
                else
                {
                    BoatLog(BOAT_LOG_NORMAL, "Transaction has not got mined, error number is %s.", tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr);
					break;
                }
            }

            tx_mined_timeout -= BOAT_FISCOBCOS_MINE_INTERVAL;
        }
        
    }while(tx_mined_timeout > 0);

    if (tx_mined_timeout <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "Wait for pending transaction timeout. This does not mean the transaction fails.");
        result = BOAT_ERROR_COMMON_TX_PENDING;
    }

    return result;
}


BCHAR *BoatFiscobcosGetBlockNumber(BoatFiscobcosTx *tx_ptr)
{
    Param_fiscobcos_getBlockNumber param_fiscobcos_getBlockNumber;
	BCHAR groupid_hexstr[67];
	BOAT_RESULT result = BOAT_SUCCESS;
    BCHAR *retval_str;

    if (tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
        return NULL;
    }

	//set groupid
	memset(groupid_hexstr, 0, sizeof(groupid_hexstr));
	UtilityBinToHex(groupid_hexstr, tx_ptr->rawtx_fields.groupid.field, tx_ptr->rawtx_fields.groupid.field_len,
					BIN2HEX_LEFTTRIM_QUANTITY, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE);
	param_fiscobcos_getBlockNumber.groupid = groupid_hexstr;

    retval_str = web3_fiscobcos_getBlockNumber(tx_ptr->wallet_ptr->web3intf_context_ptr,
											   tx_ptr->wallet_ptr->network_info.node_url_ptr,
											   &param_fiscobcos_getBlockNumber,&result);
	if(retval_str == NULL){
		BoatLog(BOAT_LOG_CRITICAL, "web3 fiscobcos get blocknumber fail ,result = %d .",result);
	}
    return retval_str;
}

BOAT_RESULT BoatFiscobcosPraseRpcResponseStringResult(const BCHAR *json_string, BoatFieldVariable *result_out)
{
    return web3_parse_json_result(json_string, "", result_out);
}

BOAT_RESULT BoatFiscobcosPraseRpcResponseResult(const BCHAR *json_string, 
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
#endif /* end of PROTOCOL_USE_FISCOBCOS */