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

/*!@brief Web3 interface

@file web3intf.c contains web3 interface functions for RPC.
*/

#include "boatinternal.h"

#include "rpcintf.h"
#include "curlport.h"

#include "cJSON.h"

#include "web3intf.h"
#include "randgenerator.h"


/******************************************************************************
@brief Expand the memory 

@param[in] mem
	 The memory to be expanded

@param[in] step_size
	 The expanded memory size in one call
@return
    This function returns BOAT_SUCCESS if expand successed. Otherwise
    it returns an error code.
*******************************************************************************/
static BOAT_RESULT web3_malloc_size_expand(BoatFieldVariable * mem, BUINT32 step_size)
{	
	//free before malloc to avoid allocate more memory at same time
	BoatFree(mem->field_ptr);
	
	mem->field_len += step_size;
	mem->field_ptr  = BoatMalloc(mem->field_len);
	if( mem->field_ptr == NULL )
	{
		mem->field_len = 0;
		BoatLog(BOAT_LOG_NORMAL, "web3_malloc_size_expand failed.");
		return BOAT_ERROR;
	}	

	BoatLog(BOAT_LOG_NORMAL, "Excuting %s:[%d].", __func__ , mem->field_len);
	return BOAT_SUCCESS;
}


/*!*****************************************************************************
@brief Prase RPC method RESPONSE

   This function Prase "result" segment.
   If "result" object is string, this function will returns contents of "result" . 
   If "result" object is still json object, the parameter named "child_name" will actived,
   if "child_name" object is string, this function will returns contents of "child_name"; 
   if "child_name" object is other types, his function will prompt "un-implemention yet".
   For other types of "result" this function is not support yet.

@param[in] json_string
	 The json to be parsed.

@param[in] child_name
	 if "result" item is json object, this param will actived.

@param[out] result_out
	 The buffer to store prase result.
	 Caller can allocate memory for this param, or can initial it with {NULL, 0},
	 this function will expand the memory if it too small to store prase result.
	 
@return
    This function returns BOAT_SUCCESS if prase successed. Otherwise
    it returns an error code.
*******************************************************************************/
BOAT_RESULT web3_parse_json_result(const BCHAR * json_string, 
								   const BCHAR * child_name, 
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
	
	if( (json_string == NULL) || (child_name == NULL) || (result_out == NULL) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter should not be NULL.");
		return BOAT_ERROR;
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
        boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_parse_json_result_cleanup);
    }
	
	// Obtain result object
	cjson_result_ptr = cJSON_GetObjectItemCaseSensitive(cjson_string_ptr, "result");
	if (cjson_result_ptr == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "Cannot find \"result\" item in RESPONSE.");
		boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_parse_json_result_cleanup);
	}

	if( cJSON_IsObject(cjson_result_ptr) )
	{
		// the "result" object is json item
		cjson_child_name_ptr = cJSON_GetObjectItemCaseSensitive(cjson_result_ptr, child_name);
		if (cjson_child_name_ptr == NULL)
		{
			BoatLog(BOAT_LOG_NORMAL, "Cannot find \"%s\" item in RESPONSE.", child_name);
			boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_parse_json_result_cleanup);
		}
	
		//prase child_name object
		if( cJSON_IsString(cjson_child_name_ptr) )
		{
			parse_result_str = cJSON_GetStringValue(cjson_child_name_ptr);
			if( parse_result_str != NULL )
			{
				BoatLog(BOAT_LOG_VERBOSE, "result = %s", parse_result_str);

				parse_result_str_len = strlen(parse_result_str);

				while(parse_result_str_len >= result_out->field_len)
				{
					BoatLog(BOAT_LOG_VERBOSE, "Expand result_out memory...");
					result = web3_malloc_size_expand(result_out, WEB3_STRING_BUF_STEP_SIZE);
					if (result != BOAT_SUCCESS)
					{
						BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
						boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_parse_json_result_cleanup);
					}
				}
				strcpy((BCHAR*)result_out->field_ptr, parse_result_str);
			}
		}
		else
		{
			BoatLog(BOAT_LOG_NORMAL, "un-implemention yet.");
		}
	}
	else if( cJSON_IsString(cjson_result_ptr) )
	{
		parse_result_str = cJSON_GetStringValue(cjson_result_ptr);
		
		if( parse_result_str != NULL )
		{
			BoatLog(BOAT_LOG_VERBOSE, "result = %s", parse_result_str);

			parse_result_str_len = strlen(parse_result_str);
			while(parse_result_str_len >= result_out->field_len)
			{
				BoatLog(BOAT_LOG_VERBOSE, "Expand result_out memory...");
				result = web3_malloc_size_expand(result_out, WEB3_STRING_BUF_STEP_SIZE);
				if (result != BOAT_SUCCESS)
				{
					BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
					boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_parse_json_result_cleanup);
				}
			}
			strcpy((BCHAR*)result_out->field_ptr, parse_result_str);
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Un-expect object type.");
		boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_parse_json_result_cleanup);
	}
	
	
	// Exceptional Clean Up
    boat_catch(web3_parse_json_result_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);

        if( cjson_string_ptr != NULL )
        {
            cJSON_Delete(cjson_string_ptr);
        }

        result = boat_exception;
    }
	
	return result;
}



/*!*****************************************************************************
@brief Initialize web3 interface

Function: web3_init()

    This function initialize resources for web3 interface.


@return
    This function always returns a pointer to the Web3 Interface context.\n
    It returns NULL if initialization fails.
    

@param This function doesn't take any argument.

*******************************************************************************/
Web3IntfContext * web3_init(void)
{
    Web3IntfContext *web3intf_context_ptr;
    boat_try_declare;

    web3intf_context_ptr = BoatMalloc(sizeof(Web3IntfContext));

    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to allocate Web3 Interface context.");
        boat_throw(BOAT_ERROR_OUT_OF_MEMORY, web3_init_cleanup);
    }

	web3intf_context_ptr->web3_json_string_buf.field_len = WEB3_STRING_BUF_STEP_SIZE;
    web3intf_context_ptr->web3_json_string_buf.field_ptr = \
			BoatMalloc(web3intf_context_ptr->web3_json_string_buf.field_len);
    if( web3intf_context_ptr->web3_json_string_buf.field_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to allocate Web3 JSON string buffer.");
        boat_throw(BOAT_ERROR_OUT_OF_MEMORY, web3_init_cleanup);
    }


	web3intf_context_ptr->web3_result_string_buf.field_len = WEB3_STRING_BUF_STEP_SIZE;
    web3intf_context_ptr->web3_result_string_buf.field_ptr = \
			BoatMalloc(web3intf_context_ptr->web3_result_string_buf.field_len);
    if( web3intf_context_ptr->web3_result_string_buf.field_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to allocate Web3 result string buffer.");
        boat_throw(BOAT_ERROR_OUT_OF_MEMORY, web3_init_cleanup);
    }
    

    web3intf_context_ptr->web3_message_id = random32();

    web3intf_context_ptr->rpc_context_ptr = RpcInit();

    if( web3intf_context_ptr->rpc_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to inintialize RPC.");
        boat_throw(BOAT_ERROR_EXT_MODULE_OPERATION_FAIL, web3_init_cleanup);
    }

    // Clean Up

    boat_catch(web3_init_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        if( web3intf_context_ptr != NULL )
        {
            if( web3intf_context_ptr->web3_json_string_buf.field_ptr   != NULL )  BoatFree(web3intf_context_ptr->web3_json_string_buf.field_ptr);
            if( web3intf_context_ptr->web3_result_string_buf.field_ptr != NULL )  BoatFree(web3intf_context_ptr->web3_result_string_buf.field_ptr);
            if( web3intf_context_ptr->rpc_context_ptr != NULL )   RpcDeinit(web3intf_context_ptr->rpc_context_ptr);

            BoatFree(web3intf_context_ptr);
            web3intf_context_ptr = NULL;
        }
    }
    
    return web3intf_context_ptr;
}


/*!*****************************************************************************
@brief Deinitialize web3 interface.

Function: web3_deinit()

    This function de-initializes web3 interface.

@see web3_init()    

@return
    This function doesn't return any value.
    

@param[in] web3intf_context_ptr
    Pointer to the web3 interface context.

*******************************************************************************/
void web3_deinit(Web3IntfContext *web3intf_context_ptr)
{
    if( web3intf_context_ptr == NULL )
    {
        return;
    }

    if( web3intf_context_ptr->web3_json_string_buf.field_ptr != NULL )
    {
        BoatFree(web3intf_context_ptr->web3_json_string_buf.field_ptr);
    }

    if( web3intf_context_ptr->web3_result_string_buf.field_ptr != NULL )
    {
        BoatFree(web3intf_context_ptr->web3_result_string_buf.field_ptr);
    }
    
    if( web3intf_context_ptr->rpc_context_ptr != NULL )
    {
        RpcDeinit(web3intf_context_ptr->rpc_context_ptr);
    }

    BoatFree(web3intf_context_ptr);
    
    return;
}

/*!*****************************************************************************
@brief Perform eth_getTransactionCount RPC method and get the transaction count
       of the specified account

Function: web3_eth_getTransactionCount()

    This function calls RPC method eth_getTransactionCount and returns the complete 
	message of the RPC method response.

    The typical RPC REQUEST is similar to:
    {"jsonrpc":"2.0","method":"eth_getTransactionCount","params":["0xc94770007dda54cF92009BFF0dE90c06F603a09f","latest"],"id":1}
    
    The typical RPC RESPONSE from blockchain node is similar to:
    {"id":1,"jsonrpc": "2.0","result": "0x1"}


    This function takes 2 arguments. The first is the URL to the blockchian
    node. Protocol and port must be explicitly specified in URL, such as
    "http://127.0.0.1:7545". The second argument is a struct containing all
    RPC call parameters as specified by ethereum.

    See following wiki for details about RPC parameters and return value:
    https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api

    This function returns the complete json message of the RPC method response. 
	The buffer storing the message is maintained by web3intf and the caller 
	shall NOT modify it, free it or save the address for later use.

@return
    This function returns the complete json message of the RPC method response.\n
    The transaction count is typically used as the "nonce" in a new transaction.\n
    If any error occurs or RPC call timeouts, it returns NULL.
    
@param web3intf_context_ptr
        A pointer to Web3 Interface context

@param node_url_str
        A string indicating the URL of blockchain node.

@param param_ptr
        The parameters of the eth_getTransactionCount RPC method.\n
        address_str:\n
            DATA, 20 Bytes - address, a string such as "0x123456..."\n
        block_num_str:\n
            QUANTITY|TAG - a string of integer block number, or "latest", "earliest" or "pending"
        
*******************************************************************************/
BCHAR *web3_eth_getTransactionCount(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_getTransactionCount *param_ptr)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_getTransactionCount_cleanup);
    }
    
    web3intf_context_ptr->web3_message_id++;
    
    if( node_url_str == NULL || param_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_getTransactionCount_cleanup);
    }
    
   
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getTransactionCount\",\"params\":"
				 "[\"%s\",\"%s\"],\"id\":%u}",
				 param_ptr->address_str,
				 param_ptr->block_num_str,
				 web3intf_context_ptr->web3_message_id
				);
		
		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_eth_getTransactionCount_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl

#if RPC_USE_LIBCURL == 1
    result = CurlPortSetOpt((CurlPortContext *)web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, web3_eth_getTransactionCount_cleanup);
    }
#endif
   
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, // web3intf_context_ptr->web3_json_string_buf stores REQUEST
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_getTransactionCount_cleanup);
    }

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_eth_getTransactionCount_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }

    return return_value_ptr;
}


/*!*****************************************************************************
@brief Perform eth_gasPrice RPC method and get the current price per gas in wei
       of the specified network.

Function: web3_eth_gasPrice()

    This function calls RPC method eth_gasPrice and returns the complete json 
	message of the RPC method response.

    The typical RPC REQUEST is similar to:
    {"jsonrpc":"2.0","method":"eth_gasPrice","params":[],"id":73}
    
    The typical RPC RESPONSE from blockchain node is similar to:
    {"id":73,"jsonrpc": "2.0","result": "0x09184e72a000"}


    This function takes 1 arguments, which is the URL to the blockchian
    node. Protocol and port must be explicitly specified in URL, such as
    "http://127.0.0.1:7545". The eth_gasPrice RPC method itself doesn't take
    any parameter.

    See following wiki for details about RPC parameters and return value:
    https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api

    This function returns the complete json message of the RPC method response. 
	The buffer storing the message is maintained by web3intf and the caller 
	shall NOT modify it, free it or save the address for later use.

@return
    This function returns the complete json message of the RPC method response.\n
    The gasPrice returned from network is a reference for use in a transaction.\n
    Specify a higher gasPrice in transaction may increase the posibility that\n
    the transcaction is get mined quicker and vice versa.\n
    If any error occurs or RPC call timeouts, it returns NULL.
    

@param web3intf_context_ptr
        A pointer to Web3 Interface context

@param node_url_str
        A string indicating the URL of blockchain node.

*******************************************************************************/
BCHAR *web3_eth_gasPrice(Web3IntfContext *web3intf_context_ptr, BCHAR *node_url_str)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_gasPrice_cleanup);
    }

    web3intf_context_ptr->web3_message_id++;
    
    if( node_url_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_gasPrice_cleanup);
    }
    
   
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_gasPrice\",\"params\":"
				 "[],\"id\":%u}",
				 web3intf_context_ptr->web3_message_id
				);
		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_eth_gasPrice_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl

#if RPC_USE_LIBCURL == 1
    result = CurlPortSetOpt((CurlPortContext *)web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, web3_eth_gasPrice_cleanup);
    }
#endif
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,   // web3intf_context_ptr->web3_json_string_buf stores REQUEST
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_gasPrice_cleanup);
    }

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;
	
	// Exceptional Clean Up
    boat_catch(web3_eth_gasPrice_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }

    return return_value_ptr;
}


/*!*****************************************************************************
@brief Perform eth_getBalance RPC method and get the balance of the specified account

Function: web3_eth_getBalance()

    This function calls RPC method eth_getBalance and returns the complete json message 
	of the RPC method response.

    The typical RPC REQUEST is similar to:
    {"jsonrpc":"2.0","method":"eth_getBalance","params":["0xc94770007dda54cF92009BFF0dE90c06F603a09f", "latest"],"id":1}
    
    The typical RPC RESPONSE from blockchain node is similar to:
    {"id":1,"jsonrpc": "2.0","result": "0x0234c8a3397aab58"}


    This function takes 2 arguments. The first is the URL to the blockchian
    node. Protocol and port must be explicitly specified in URL, such as
    "http://127.0.0.1:7545". The second argument is a struct containing all
    RPC call parameters as specified by ethereum.

    See following wiki for details about RPC parameters and return value:
    https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api

    This function returns the complete json message of the RPC method response. 
	The buffer storing the string is maintained by web3intf and the caller 
	shall NOT modify it, free it or save the address for later use.

@return
    This function returns the complete json message of the RPC method response.\n
    If any error occurs or RPC call timeouts, it returns NULL.
    

@param web3intf_context_ptr
        A pointer to Web3 Interface context

@param node_url_str
        A string indicating the URL of blockchain node.

@param param_ptr
        The parameters of the eth_getTransactionCount RPC method.\n
        address_str:\n
            DATA, 20 Bytes - address to check for balance.\n
        block_num_str:\n
            QUANTITY|TAG - a string of integer block number, or "latest", "earliest" or "pending"
        
*******************************************************************************/
BCHAR *web3_eth_getBalance(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_getBalance *param_ptr)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_getBalance_cleanup);
    }

    web3intf_context_ptr->web3_message_id++;
    
    if( node_url_str == NULL || param_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_getBalance_cleanup);
    }
    
   
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getBalance\",\"params\":"
				 "[\"%s\",\"%s\"],\"id\":%u}",
				 param_ptr->address_str,
				 param_ptr->block_num_str,
				 web3intf_context_ptr->web3_message_id
				);
		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_eth_getBalance_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl

#if RPC_USE_LIBCURL == 1
    result = CurlPortSetOpt((CurlPortContext *)web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, web3_eth_getBalance_cleanup);
    }
#endif
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,   // web3intf_context_ptr->web3_json_string_buf stores REQUEST
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_getBalance_cleanup);
    }

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_eth_getBalance_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }

    return return_value_ptr;
}

/*!*****************************************************************************
@brief Perform eth_sendRawTransaction RPC method.

Function: web3_eth_sendRawTransaction()

    This function calls RPC method eth_sendRawTransaction and returns the complete 
	json message of the RPC method response.

    The typical RPC REQUEST is similar to:
    {"jsonrpc":"2.0","method":"eth_sendRawTransaction","params":["0xd46e8dd67c5d32be8d46e8dd67c5d32be8058bb8eb970870f072445675058bb8eb970870f072445675"],"id":1}
    
    The typical RPC RESPONSE from blockchain node is similar to:
    {"id":1,"jsonrpc": "2.0","result": "0xe670ec64341771606e55d6b4ca35a1a6b75ee3d5145a99d05921026d1527331"}

    This function takes 2 arguments. The first is the URL to the blockchian
    node. Protocol and port must be explicitly specified in URL, such as
    "http://127.0.0.1:7545". The second argument is a struct containing all
    RPC call parameters as specified by ethereum.

    See following wiki for details about RPC parameters and return value:
    https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api

    This function returns the complete json message of the RPC method response. 
	The buffer storing the message is maintained by web3intf and the caller 
	shall NOT modify it, free it or save the address for later use.

@return
    This function returns the complete json message of the RPC method response.\n
    If this transaction is not yet avaialble, the "result" of the message is "0x0".\n    
    If the blockchain node returns error or RPC call timeouts, it returns NULL.\n\n
    Note: A successful return from eth_sendRawTransaction DOES NOT mean the\n
    transaction is confirmed. The caller shall priodically polling the receipt\n
    using eth_getTransactionReceipt method with the transaction hash returned\n
    by eth_sendRawTransaction.


@param web3intf_context_ptr
        A pointer to Web3 Interface context

@param node_url_str
        A string indicating the URL of blockchain node.

@param param_ptr
        The parameters of the eth_sendRawTransaction RPC method.\n
        signedtx_str:\n
            DATA, The signed transaction data as a HEX string, with "0x" prefix.

*******************************************************************************/
BCHAR *web3_eth_sendRawTransaction(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_sendRawTransaction *param_ptr)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_sendRawTransaction_cleanup);
    }

    web3intf_context_ptr->web3_message_id++;
    
    if( node_url_str == NULL || param_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_sendRawTransaction_cleanup);
    }
    

    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_sendRawTransaction\",\"params\":"
				 "[\"%s\"],\"id\":%u}",
				 param_ptr->signedtx_str,
				 web3intf_context_ptr->web3_message_id
				);
		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_eth_sendRawTransaction_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl

#if RPC_USE_LIBCURL == 1
    result = CurlPortSetOpt((CurlPortContext *)web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, web3_eth_sendRawTransaction_cleanup);
    }
#endif
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,   // web3intf_context_ptr->web3_json_string_buf stores REQUEST
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_sendRawTransaction_cleanup);
    }

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;
	
    // Exceptional Clean Up
    boat_catch(web3_eth_sendRawTransaction_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }

    return return_value_ptr;
}



/*!*****************************************************************************
@brief Perform eth_getStorageAt RPC method.

Function: web3_eth_getStorageAt()

    This function calls RPC method eth_getStorageAt and returns the complete 
	json message of the RPC method response.

    The typical RPC REQUEST is similar to:
    {"jsonrpc":"2.0", "method": "eth_getStorageAt", "params": ["0x295a70b2de5e3953354a6a8344e616ed314d7251", "0x0", "latest"], "id": 1}
    
    The typical RPC RESPONSE from blockchain node is similar to:
    {"jsonrpc":"2.0","id":1,"result":"0x00000000000000000000000000000000000000000000000000000000000004d2"}

    This function takes 2 arguments. The first is the URL to the blockchian
    node. Protocol and port must be explicitly specified in URL, such as
    "http://127.0.0.1:7545". The second argument is a struct containing all
    RPC call parameters as specified by ethereum.

    See following wiki for details about RPC parameters and return value:
    https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api

    The way to calculate <position_str> is quite complicated if the datum is
    not a simple element type. The position conforms to the storage slot as
    specified by Solidity memory layout.

    Solidity Memory Layout:
    https://solidity.readthedocs.io/en/latest/miscellaneous.html#layout-of-state-variables-in-storage

    A easy-to-understand explanation can be found at:
    https://medium.com/aigang-network/how-to-read-ethereum-contract-storage-44252c8af925
    

    This function returns the complete json message of the RPC method response.
    The buffer storing the message is maintained by web3intf and the caller 
	shall NOT modify it, free it or save the address for later use.

@return
    This function returns the complete json message of the RPC method response.\n
    Note that multiple data may be packed in one storage slot.\n
	See Solidity Memory Layout to understand how they are packed.\n
    If the blockchain node returns error or RPC call timeouts, it returns NULL.


@param web3intf_context_ptr
        A pointer to Web3 Interface context

@param node_url_str
        A string indicating the URL of blockchain node.

@param param_ptr
        The parameters of the eth_getStorageAt RPC method.\n        
        address_str:\n
            DATA, 20 Bytes - address, a string such as "0x123456..."\n
        position_str:\n
            QUANTITY - integer of the position in the storage.\n        
        block_num_str:\n
            QUANTITY|TAG - a string of integer block number, or "latest", "earliest" or "pending"

*******************************************************************************/
BCHAR *web3_eth_getStorageAt(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_getStorageAt *param_ptr)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_getStorageAt_cleanup);
    }

    web3intf_context_ptr->web3_message_id++;
    
    if( node_url_str == NULL || param_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_getStorageAt_cleanup);
    }
    

    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getStorageAt\",\"params\":"
				 "[\"%s\",\"%s\",\"%s\"],\"id\":%u}",
				 param_ptr->address_str,
				 param_ptr->position_str,
				 param_ptr->block_num_str,
				 web3intf_context_ptr->web3_message_id
				);
		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_eth_getStorageAt_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);
    
    // POST the REQUEST through curl

#if RPC_USE_LIBCURL == 1
    result = CurlPortSetOpt((CurlPortContext *)web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, web3_eth_getStorageAt_cleanup);
    }
#endif
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,   // web3intf_context_ptr->web3_json_string_buf stores REQUEST
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_getStorageAt_cleanup);
    }

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_eth_getStorageAt_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }

    return return_value_ptr;
}


/*!*****************************************************************************
@brief Perform eth_getTransactionReceipt RPC method.

Function: web3_eth_getTransactionReceipt()

    This function calls RPC method eth_getTransactionReceipt and returns the complete 
	json message of the RPC method response.

    The typical RPC REQUEST is similar to:
    {"jsonrpc":"2.0","method":"eth_getTransactionReceipt","params":["0xb903239f8543d04b5dc1ba6579132b143087c68db1b2168786408fcbce568238"],"id":1}
    
    The typical RPC RESPONSE from blockchain node is similar to:
    @verbatim
    {
    "id":1,
    "jsonrpc":"2.0",
    "result": {
         transactionHash: '0xb903239f8543d04b5dc1ba6579132b143087c68db1b2168786408fcbce568238',
         transactionIndex:  '0x1', // 1
         blockNumber: '0xb', // 11
         blockHash: '0xc6ef2fc5426d6ad6fd9e2a26abeab0aa2411b7ab17f30a99d3cb96aed1d1055b',
         cumulativeGasUsed: '0x33bc', // 13244
         gasUsed: '0x4dc', // 1244
         contractAddress: '0xb60e8dd61c5d32be8058bb8eb970870f07233155', // or null, if none was created
         logs: [{
             // logs as returned by getFilterLogs, etc.
         }, ...],
         logsBloom: "0x00...0", // 256 byte bloom filter
         status: '0x1'
      }
    }
    @endverbatim

    This function takes 2 arguments. The first is the URL to the blockchian
    node. Protocol and port must be explicitly specified in URL, such as
    "http://127.0.0.1:7545". The second argument is a struct containing all
    RPC call parameters as specified by ethereum.

    See following wiki for details about RPC parameters and return value:
    https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api

    This function returns the complete json message of the RPC method response. 
	The buffer storing the message is maintained by web3intf and the caller shall 
	NOT modify it, free it or save the address for later use.

@return
    This function returns the complete json message of the RPC method response.\n
    For the value of "status" in message, "0x1" for success and "0x0" for failure.\n
    If the transaction ispending, it returns a null string i.e. a string containing only '\0'\n
    instead of a NULL pointer.\n
    If any error occurs or RPC call timeouts, it returns NULL.
    

@param web3intf_context_ptr
        A pointer to Web3 Interface context

@param node_url_str
        A string indicating the URL of blockchain node.

@param param_ptr
        The parameters of the eth_getTransactionReceipt RPC method.\n        
        tx_hash_str:\n
            DATA, 32 Bytes - hash of a transaction
        
*******************************************************************************/
BCHAR *web3_eth_getTransactionReceiptStatus(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_getTransactionReceipt *param_ptr)
{
    BCHAR  *rpc_response_str;
    BUINT32 rpc_response_len;
    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BCHAR  *return_value_ptr = NULL;
    
	BOAT_RESULT result;
    boat_try_declare;
    
    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_getTransactionReceiptStatus_cleanup);
    }

    web3intf_context_ptr->web3_message_id++;
    
    if( node_url_str == NULL || param_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_getTransactionReceiptStatus_cleanup);
    }
    
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getTransactionReceipt\",\"params\":"
				 "[\"%s\"],\"id\":%u}",
				 param_ptr->tx_hash_str,
				 web3intf_context_ptr->web3_message_id
				);
		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_eth_getTransactionReceiptStatus_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl

#if RPC_USE_LIBCURL == 1
    result = CurlPortSetOpt((CurlPortContext *)web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, web3_eth_getTransactionReceiptStatus_cleanup);
    }
#endif
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,   // web3intf_context_ptr->web3_json_string_buf stores REQUEST
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_getTransactionReceiptStatus_cleanup);
    }

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_eth_getTransactionReceiptStatus_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }

    return return_value_ptr;
}





/*!*****************************************************************************
@brief Perform eth_call RPC method.

Function: web3_eth_call()

    This function calls RPC method eth_call and returns the complete json message 
	of the RPC method RESPONSE.

    This function can only call contract functions that don't change the block
    STATE. To call contract functions that may change block STATE, use
    eth_sendRawTransaction instead.

    The typical RPC REQUEST is similar to:
    @verbatim
    {"jsonrpc":"2.0","method":"eth_call","params":[{
      "from": "0xb60e8dd61c5d32be8058bb8eb970870f07233155", // Optional
      "to": "0xd46e8dd67c5d32be8058bb8eb970870f07244567",
      "gas": "0x76c0", // Optional, 30400
      "gasPrice": "0x9184e72a000", // Optional, 10000000000000
      "value": "0x0", // Optional
      "data": "0xd46e8dd67c5d32be8d46e8dd67c5d32be8058bb8eb970870f072445675058bb8eb970870" //Optional
    }],"id":1}
    @endverbatim
    
    The typical RPC RESPONSE from blockchain node is similar to:
    {"id":1,"jsonrpc": "2.0","result": "0x0"}

    This function takes 2 arguments. The first is the URL to the blockchian
    node. Protocol and port must be explicitly specified in URL, such as
    "http://127.0.0.1:7545". The second argument is a struct containing all
    RPC call parameters as specified by ethereum.

    See following wiki for details about RPC parameters and return value:
    https://github.com/ethereum/wiki/wiki/JSON-RPC#json-rpc-api

    This function could call a contract function without creating a transaction
    on the block chain. The only mandatory parameter of eth_call is "to", which
    is the address of the contract. Typically parameter "data" is also mandatory,
    which is composed of 4-byte function selector followed by 0 or more parameters
    regarding the contract function being called. See Ethereum Contract ABI for
    the details about how to compose "data" field:
    https://github.com/ethereum/wiki/wiki/Ethereum-Contract-ABI

    An eth_call doesn't consume gas, but it's a good practice to specifiy the
    "gas" parameter for better compatibility.
    
    This function returns the complete json message of the RPC method response. 
	The buffer storing the message is maintained by web3intf and the caller 
	shall NOT modify it, free it or save the address for later use.

@return
    This function returns the complete json message of the RPC method response.\n

@param web3intf_context_ptr
        A pointer to Web3 Interface context

@param node_url_str
        A string indicating the URL of blockchain node.

@param param_ptr
        The parameters of the eth_call RPC method. Some optional parameters are omitted.\n
        to: The address of the contract.\n
        gas: The gasLimit.\n
        gasPrice: The gasPrice in wei.\n
        data: The function selector followed by parameters.\n
        block_num_str:\n
            QUANTITY|TAG - a string of integer block number, or "latest", "earliest" or "pending"

*******************************************************************************/
BCHAR *web3_eth_call(Web3IntfContext *web3intf_context_ptr,
                                    BCHAR *node_url_str,
                                    const Param_eth_call *param_ptr)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if( web3intf_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_call_cleanup);
    }

    web3intf_context_ptr->web3_message_id++;
    
    if( node_url_str == NULL || param_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_NULL_POINTER, web3_eth_call_cleanup);
    }
    

    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":"
				 "[{\"to\":\"%s\",\"gas\":\"%s\",\"gasPrice\":\"%s\",\"data\":\"%s\"},\"%s\"],\"id\":%u}",
				 param_ptr->to,
				 param_ptr->gas,
				 param_ptr->gasPrice,
				 param_ptr->data,
				 param_ptr->block_num_str,
				 web3intf_context_ptr->web3_message_id
				);
		if(expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = web3_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute web3_malloc_size_expand.");
				boat_throw(BOAT_ERROR_JSON_PARSE_FAIL, web3_eth_call_cleanup);
			}
			malloc_size_expand_flag = true;
		}
	}while( malloc_size_expand_flag );

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl

#if RPC_USE_LIBCURL == 1
    result = CurlPortSetOpt((CurlPortContext *)web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if( result != BOAT_SUCCESS )
    {
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, web3_eth_call_cleanup);
    }
#endif
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,   // web3intf_context_ptr->web3_json_string_buf stores REQUEST
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_eth_call_cleanup);
    }

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_eth_call_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
    }

    return return_value_ptr;
}

