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

/*!@brief fibocom HTTP RPC porting header file

@file
fibocomhttpport.h is the header file of fibocom's HTTP porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use fibocom HTTP RPC porting, RPC_USE_FIBOCOMHTTPPORT in boatoptions.h must set to 1.
*/

#ifndef __FIBOCOMHTTPPORT_H__
#define __FIBOCOMHTTPPORT_H__

#if RPC_USE_FIBOCOMHTTPPORT == 1


typedef struct TFibocomHttpPortContext
{
    BCHAR *remote_url_str;        //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
    StringWithLen http_response;  //!< Store HTTP response from remote peer
}FibocomHttpPortContext;

#ifdef __cplusplus
extern "C" {
#endif

FibocomHttpPortContext *FibocomHttpPortInit(void);

void FibocomHttpPortDeinit(FibocomHttpPortContext *fibocomhttpport_context_ptr);

BOAT_RESULT FibocomHttpPortSetOpt(FibocomHttpPortContext *fibocomhttpport_context_ptr, BCHAR *remote_url_str);

BOAT_RESULT FibocomHttpPortRequestSync(FibocomHttpPortContext *fibocomhttpport_context_ptr,
                                    const BCHAR *request_str,
                                    BUINT32 request_len,
                                    BOAT_OUT BCHAR **response_str_ptr,
                                    BOAT_OUT BUINT32 *response_len_ptr);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif // end of #if RPC_USE_FIBOCOMHTTPPORT == 1

#endif

