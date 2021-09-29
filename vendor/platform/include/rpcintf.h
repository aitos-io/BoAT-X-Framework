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

/*!@brief RPC wrapper header file

@file
rpcintf.h is header file for RPC wrapper functions.
The exact implementation of the actual RPC mechanism is controlled by
RPC_USE_XXX macros.
*/
#ifndef __RPCINTF_H__
#define __RPCINTF_H__

#include "boatconfig.h"
#include "boatinternal.h"

#if RPC_USE_LIBCURL == 1
//#include "curl/curl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!****************************************************************************
 * @brief Wrapper function to initialize RPC mechanism.
 *
 * @details
 *   This function wrapper to initialize RPC mechanism.
 *   The exact implementation of the actual RPC mechanism is controlled by
 *   RPC_USE_XXX macros.
 *	  
 * @param This function doesn't take any argument.
 *
 * @return
 *   This function returns pointer to the RPC context.\n
 *   It returns NULL if initialization fails.
 *	 
 * @see RpcInit()
 ******************************************************************************/
void* RpcInit(void);


/*!****************************************************************************
 * @brief Wrapper function to de-initialize RPC mechanism.
 *
 * @details
 *   This function de-initializes RPC mechanism.
 *   The exact implementation of the actual RPC mechanism is controlled by
 *   RPC_USE_XXX macros.
 *	  
 * @param[in] rpc_context_ptr
 *   Pointer to RPC context returned by RpcInit().
 * 
 * @return
 *   This function doesn't return any value.
 *	 
 * @see RpcInit()
 ******************************************************************************/
void RpcDeinit(void *rpc_context_ptr);


/*!****************************************************************************
 * @brief Set options for rpc context.
 *
 * @details
 *   This function is to set rpc parameter.	 
 *
 * @param[in] rpc_context_ptr
 *   A pointer to the rpc context context
 *	 
 * @param[in] remote_url_str
 *   The URL of the remote server.
 *
 * @return
 *   This function always returns BOAT_SUCCESS.
 ******************************************************************************/
BOAT_RESULT RpcRequestSet(void *rpc_context_ptr, BCHAR *remote_url_str);


/*!****************************************************************************
 * @brief Wrapper function to perform RPC request and receive its response synchronously.
 *
 * @details
 *   This function is a wrapper for performing RPC calls synchronously.
 *   \n This function takes the REQUEST to transmit as input argument and outputs
 *   the address of the buffer containing received RPC RESPONSE. The function
 *   will be suspended until it receives RESPONSE or timeouts.
 *   \n The exact format and meaning of the request and response is defined by the
 *   wrapped function.
 *   \n The caller could only read from the response buffer and copy to its own
 *   buffer. The caller MUST NOT modify, free the response buffer or save the
 *   address of the response buffer for later use.
 *	 
 * @param[in] rpc_context_ptr
 *   A pointer to the RPC context. The exact type depends on the RPC method.
 *
 * @param[in] request_ptr
 *   A pointer to the buffer containing RPC REQUEST.
 *
 * @param[in] request_len
 *   The length of the RPC REQUEST in bytes.
 *
 * @param[out] response_pptr
 *   The address of a (BUINT8 *) pointer to hold the address of the RESPONSE
 *   buffer. Note that whether the content in the buffer is binary stream or
 *   string is defined by the wrapped function. DO NOT assume it's NULL
 *   terminated even if it were string.\n
 *   If any error occurs or RPC REQUEST timeouts, the address is NULL.
 *
 * @param[out] response_len_ptr
 *   The address of a BUINT32 to hold the length of the received RESPONSE.
 *   Note that this doesn't imply a NULL terminator being counted even if
 *   the response is a string.\n
 *   If any error occurs or RPC REQUEST timeouts, the length is 0.
 *
 * @return
 *   This function returns BOAT_SUCCESS if the RPC call is successful.\n
 *   If any error occurs or RPC REQUEST timeouts, it transfers the error code
 *   returned by the wrapped function.
 ******************************************************************************/
BOAT_RESULT RpcRequestSync( void *rpc_context_ptr,
                            BUINT8 *request_ptr,
                            BUINT32 request_len,
                            BOAT_OUT BUINT8 **response_pptr,
                            BOAT_OUT BUINT32 *response_len_ptr );

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
