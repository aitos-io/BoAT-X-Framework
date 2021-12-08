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

/*!@brief nwy HTTP RPC porting header file

@file
nwyhttpport.h is the header file of nwy's HTTP porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use nwy HTTP RPC porting, RPC_USE_NWYHTTPPORT in boatconfig.h must set to 1.
*/

#ifndef __NWYHTTPPORT_H__
#define __NWYHTTPPORT_H__

#include "boatinternal.h"
#include "nwy_osi_api.h"

//!Receiving buffer size
#define NWYHTTPPORT_RECV_BUF_SIZE 4096

typedef struct NwyHttpPortContext
{
    BCHAR *remote_url_str;        //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
    StringWithLen http_response;    //!<  Store response from remote peer
}NwyHttpPortContext;

#ifdef __cplusplus
extern "C" {
#endif

NwyHttpPortContext * NwyHttpPortInit(void);

void NwyHttpPortDeinit(NwyHttpPortContext * nwyhttpport_context_ptr);

BOAT_RESULT NwyHttpPortSetOpt(NwyHttpPortContext * nwyhttpport_context_ptr, BCHAR *remote_url_str);

BOAT_RESULT NwyHttpPortRequestSync(NwyHttpPortContext * nwyhttpport_context_ptr,
                               const BCHAR *request_str,
                               BUINT32 request_len,
                               BOAT_OUT BCHAR **response_str_ptr,
                               BOAT_OUT BUINT32 *response_len_ptr);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif

