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

/*!@brief RPC wrapper

@file
rpcintf.c contains wrapper functions for RPC.
The exact implementation of the actual RPC mechanism is controlled by
RPC_USE_XXX macros.
*/

#include "boatconfig.h"
#include "boatinternal.h"
#include "mbedhttpport.h"

void *RpcInit(void)
{
    void *rpc_context_ptr = NULL;

#if RPC_USE_LIBCURL == 1
    rpc_context_ptr = CurlPortInit();
#elif RPC_USE_MBEDHTTPPORT == 1
    rpc_context_ptr = MbedHttpPortInit();
#endif

    return rpc_context_ptr;
}


void RpcDeinit(void *rpc_context_ptr)
{
    if (rpc_context_ptr == NULL)
    {
        return;
    }
    
#if RPC_USE_LIBCURL == 1
    CurlPortDeinit(rpc_context_ptr);
#elif RPC_USE_MBEDHTTPPORT == 1
    MbedHttpPortDeinit(rpc_context_ptr);
#endif
}


BOAT_RESULT RpcRequestSet(void *rpc_context_ptr, BCHAR *remote_url_str)
{
    BOAT_RESULT result = BOAT_ERROR;

#if RPC_USE_LIBCURL == 1    
    return CurlPortSetOpt((CurlPortContext*)rpc_context_ptr, remote_url_str);
#elif RPC_USE_MBEDHTTPPORT == 1
	return MbedHttpPortSetOpt((MbedHttpPortContext*)rpc_context_ptr, remote_url_str);
#endif
    return result;
}


BOAT_RESULT RpcRequestSync(void *rpc_context_ptr,
                           BUINT8 *request_ptr,
                           BUINT32 request_len,
                           BOAT_OUT BUINT8 **response_pptr,
                           BOAT_OUT BUINT32 *response_len_ptr )
{
    BOAT_RESULT result = BOAT_ERROR;
    
#if RPC_USE_LIBCURL == 1
    result = CurlPortRequestSync(rpc_context_ptr, (const BCHAR *)request_ptr, request_len, 
								 (BOAT_OUT BCHAR **)response_pptr, response_len_ptr);
#elif RPC_USE_MBEDHTTPPORT == 1
    result = MbedHttpPortRequestSync(rpc_context_ptr, (const BCHAR *)request_ptr, request_len, (BOAT_OUT BCHAR **)response_pptr, response_len_ptr);
#endif

    return result;
}
