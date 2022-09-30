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

/*!@brief RPC wrapper header file internally used by RPC porting

@file
rpcport.h is header file to be included by RPC Porting such as curlport.
Upper layer calling RPC wrapper functions should include rpcintf.h instead.\n
The exact implementation of the actual RPC mechanism is controlled by
RPC_USE_XXX macros.
*/

#ifndef __RPCPORT_H__
#define __RPCPORT_H__

#include "boatinternal.h"

#include "rpcintf.h"

#if RPC_USE_LIBCURL == 1
#include "curlport.h"
#endif

#if RPC_USE_MBEDHTTPPORT == 1
#include "mbedhttpport.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
