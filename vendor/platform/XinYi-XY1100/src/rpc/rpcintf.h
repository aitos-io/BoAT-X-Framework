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

/*!@brief RPC wrapper header file

@file
rpcintf.h is header file for RPC wrapper functions.
The exact implementation of the actual RPC mechanism is controlled by
RPC_USE_XXX macros.
*/
#ifndef __RPCINTF_H__
#define __RPCINTF_H__

#include "boatinternal.h"


#if RPC_USE_LIBCURL == 1
#include "curl/curl.h"
#endif
#if RPC_USE_MBEDHTTPPORT == 1
#include "mbedhttpport.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void* RpcInit(void);

void RpcDeinit(void *rpc_context_ptr);


BOAT_RESULT RpcRequestSet(void *rpc_context_ptr, BCHAR *remote_url_str);

BOAT_RESULT RpcRequestSync(void *rpc_context_ptr,
                           BUINT8 *request_ptr,
                           BUINT32 request_len,
                           BOAT_OUT BUINT8 **response_pptr,
                           BOAT_OUT BUINT32 *response_len_ptr);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
