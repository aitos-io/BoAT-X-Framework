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




#ifdef __cplusplus
extern "C" {
#endif

void* RpcInit(void);

void RpcDeinit(void *rpc_context_ptr);


BOAT_RESULT RpcRequestSync(void *rpc_context_ptr,
                          BUINT8 *request_ptr,
                          BUINT32 request_len,
                          BOAT_OUT BUINT8 **response_pptr,
                          BOAT_OUT BUINT32 *response_len_ptr);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
