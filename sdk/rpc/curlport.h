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

/*!@brief libcurl porting header file

@file
curlport.h is the header file of libcurl porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use libcurl porting, RPC_USE_LIBCURL in boatoptions.h must set to 1.
*/

#ifndef __CURLPORT_H__
#define __CURLPORT_H__

#if RPC_USE_LIBCURL == 1

#include "boatinternal.h"


//!The step to dynamically expand the receiving buffer.
#define CURLPORT_RECV_BUF_SIZE_STEP 1024



typedef struct TCurlPortContext
{
    BCHAR *remote_url_str;                 //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
    StringWithLen curlport_response;    //!<  Store response from remote peer
}CurlPortContext;

#ifdef __cplusplus
extern "C" {
#endif

CurlPortContext * CurlPortInit(void);

void CurlPortDeinit(CurlPortContext * curlport_context_ptr);

BOAT_RESULT CurlPortSetOpt(CurlPortContext * curlport_context_ptr, BCHAR *remote_url_str);

BOAT_RESULT CurlPortRequestSync(CurlPortContext * curlport_context_ptr,
                               const BCHAR *request_str,
                               BUINT32 request_len,
                               BOAT_OUT BCHAR **response_str_ptr,
                               BOAT_OUT BUINT32 *response_len_ptr);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif // end of #if RPC_USE_LIBCURL == 1

#endif

