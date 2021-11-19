/******************************************************************************
Copyright (C) 2018-2020 aitos.io

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

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
void *RpcInit(void)
{
    void *rpc_context_ptr = NULL;

#if RPC_USE_LIBCURL == 1
    rpc_context_ptr = CurlPortInit();
#endif
#if RPC_USE_MBEDHTTPPORT == 1
    rpc_context_ptr = MbedHttpPortInit();
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
    if (rpc_context_ptr == NULL)
    {
        return;
    }
    
#if RPC_USE_LIBCURL == 1
    CurlPortDeinit(rpc_context_ptr);
#endif
#if RPC_USE_MBEDHTTPPORT == 1
    MbedHttpPortDeinit(rpc_context_ptr);
#endif

    return;
}


BOAT_RESULT RpcRequestSet(void *rpc_context_ptr, BCHAR *remote_url_str)
{
    BOAT_RESULT result = BOAT_ERROR;

#if RPC_USE_LIBCURL == 1    
    return CurlPortSetOpt((CurlPortContext*)rpc_context_ptr, remote_url_str);
#elif RPC_USE_MBEDHTTPPORT == 1
    BoatLog(BOAT_LOG_VERBOSE, "RPC_USE_MBEDHTTPPORT,rpc_context_ptr=>%x",rpc_context_ptr);
    BoatLog(BOAT_LOG_VERBOSE, "RPC_USE_MBEDHTTPPORT,remote_url_str=>%s",remote_url_str);
	return MbedHttpPortSetOpt((MbedHttpPortContext*)rpc_context_ptr, remote_url_str);
#endif
    return result;
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
    BOAT_RESULT result = BOAT_ERROR;
    
#if RPC_USE_LIBCURL == 1
    result = CurlPortRequestSync(rpc_context_ptr, (const BCHAR *)request_ptr, request_len, (BOAT_OUT BCHAR **)response_pptr, response_len_ptr);
#endif
#if RPC_USE_MBEDHTTPPORT == 1
    result = MbedHttpPortRequestSync(rpc_context_ptr, (const BCHAR *)request_ptr, request_len, (BOAT_OUT BCHAR **)response_pptr, response_len_ptr);
#endif

    return result;
}




