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

/*!@brief mbed HTTP RPC porting header file

@file
mbedhttpport.h is the header file of mbed's HTTP porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use mbed HTTP RPC porting, RPC_USE_MBEDHTTPPORT in boatoptions.h must set to 1.
*/

#ifndef __MBEDHTTPPORT_H__
#define __MBEDHTTPPORT_H__

#if RPC_USE_MBEDHTTPPORT == 1

#include "boatinternal.h"


//!Receiving buffer size
#define MBEDHTTPPORT_RECV_BUF_SIZE 4096



typedef struct TMbedHttpPortContext
{
    BCHAR *remote_url_str;        //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
    StringWithLen http_response_head;  //!< Store HTTP response head from remote peer
    StringWithLen http_response_body;  //!< Store HTTP response body from remote peer
}MbedHttpPortContext;

#ifdef __cplusplus
extern "C" {
#endif

MbedHttpPortContext *MbedHttpPortInit(void);

void MbedHttpPortDeinit(MbedHttpPortContext *mbedhttpport_context_ptr);

BOAT_RESULT MbedHttpPortSetOpt(MbedHttpPortContext *mbedhttpport_context_ptr, BCHAR *remote_url_str);

BOAT_RESULT MbedHttpPortRequestSync(MbedHttpPortContext *mbedhttpport_context_ptr,
                                    const BCHAR *request_str,
                                    BUINT32 request_len,
                                    BOAT_OUT BCHAR **response_str_ptr,
                                    BOAT_OUT BUINT32 *response_len_ptr);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif // end of #if RPC_USE_MBEDHTTPPORT == 1

#endif

