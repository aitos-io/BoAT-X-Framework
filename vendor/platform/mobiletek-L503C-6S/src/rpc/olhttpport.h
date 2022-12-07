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

#ifndef __OLHTTPPORT_H__
#define __OLHTTPPORT_H__

#include "boatconfig.h"

#if RPC_USE_OLHTTPPORT == 1

#include "boatinternal.h"
#include "http_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OLHTTPPORT_RECV_BUF_SIZE_STEP 1024

typedef struct TOlHttpPortContext
{
    BCHAR *remote_url_str;                 //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
    StringWithLen olhttpport_response;    //!<  Store response from remote peer
}OlHttpPortContext;

http_client * OlHttpPortInit(void);

void OlHttpPortDeinit(OlHttpPortContext * olport_context_ptr);

BOAT_RESULT OlHttpSetOpt(OlHttpPortContext * olport_context_ptr, BCHAR *remote_url_str);

BOAT_RESULT OlHttpPortRequestSync( OlHttpPortContext * olport_context_ptr,
                                 const BCHAR *request_str,
                                 BUINT32 request_len,
                                 BOAT_OUT BCHAR **response_str_ptr,
                                 BOAT_OUT BUINT32 *response_len_ptr );

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of #if olhttpport.h == 1 */
#endif /* end of #ifndef __OLHTTPPORT_H__ */

