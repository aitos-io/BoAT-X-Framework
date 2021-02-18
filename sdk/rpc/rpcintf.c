/******************************************************************************
 * Copyright (C) 2018-2020 aitos.io
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

/*!@brief RPC wrapper

@file
rpcintf.c contains wrapper functions for RPC.
The exact implementation of the actual RPC mechanism is controlled by
RPC_USE_XXX macros.
*/

#include "boatinternal.h"
#include "rpcport.h"



/*!*****************************************************************************
@brief Wrapper function to initialize RPC mechanism.

Function: RpcInit()

    This function wrapper to initialize RPC mechanism.
    The exact implementation of the actual RPC mechanism is controlled by
    RPC_USE_XXX macros.
    
@see RpcInit()

@return
    This function returns pointer to the RPC context.\n
    It returns NULL if initialization fails.
    

@param This function doesn't take any argument.

*******************************************************************************/
void * RpcInit(void)
{
    void * rpc_context_ptr;

#if RPC_USE_LIBCURL == 1
    rpc_context_ptr = CurlPortInit();
#endif

    return rpc_context_ptr;

}


/*!*****************************************************************************
@brief Wrapper function to de-initialize RPC mechanism.

Function: RpcDeinit()

    This function de-initializes RPC mechanism.
    The exact implementation of the actual RPC mechanism is controlled by
    RPC_USE_XXX macros.
    
@see RpcInit()

@return
    This function doesn't return any value.
    

@param[in] rpc_context_ptr
    Pointer to RPC context returned by RpcInit().

*******************************************************************************/
void RpcDeinit(void *rpc_context_ptr)
{
    if( rpc_context_ptr == NULL )
    {
        return;
    }
    
#if RPC_USE_LIBCURL == 1
    CurlPortDeinit(rpc_context_ptr);
#endif

    return;
}



/*!******************************************************************************
@brief Wrapper function to perform RPC request and receive its response synchronously.

Function: RpcRequestSync()

    This function is a wrapper for performing RPC calls synchronously.
    
    This function takes the REQUEST to transmit as input argument and outputs
    the address of the buffer containing received RPC RESPONSE. The function
    will be suspended until it receives RESPONSE or timeouts.

    The exact format and meaning of the request and response is defined by the
    wrapped function.

    The caller could only read from the response buffer and copy to its own
    buffer. The caller MUST NOT modify, free the response buffer or save the
    address of the response buffer for later use.


@return
    This function returns BOAT_SUCCESS if the RPC call is successful.\n
    If any error occurs or RPC REQUEST timeouts, it transfers the error code
    returned by the wrapped function.
    

@param[in] rpc_context_ptr
        A pointer to the RPC context. The exact type depends on the RPC method.

@param[in] request_ptr
        A pointer to the buffer containing RPC REQUEST.

@param[in] request_len
        The length of the RPC REQUEST in bytes.

@param[out] response_pptr
        The address of a (BUINT8 *) pointer to hold the address of the RESPONSE
        buffer. Note that whether the content in the buffer is binary stream or
        string is defined by the wrapped function. DO NOT assume it's NULL
        terminated even if it were string.\n
        If any error occurs or RPC REQUEST timeouts, the address is NULL.

@param[out] response_len_ptr
        The address of a BUINT32 to hold the length of the received RESPONSE.
        Note that this doesn't imply a NULL terminator being counted even if
        the response is a string.\n
        If any error occurs or RPC REQUEST timeouts, the length is 0.
        
*******************************************************************************/
BOAT_RESULT RpcRequestSync(void *rpc_context_ptr,
                          BUINT8 *request_ptr,
                          BUINT32 request_len,
                          BOAT_OUT BUINT8 **response_pptr,
                          BOAT_OUT BUINT32 *response_len_ptr)
{
    BOAT_RESULT result;
    
#if RPC_USE_LIBCURL == 1
    result = CurlPortRequestSync(rpc_context_ptr, (const BCHAR *)request_ptr, request_len, (BOAT_OUT BCHAR **)response_pptr, response_len_ptr);
#endif

    return result;
}




