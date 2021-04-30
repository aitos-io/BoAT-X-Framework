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

To use libcurl porting, RPC_USE_LIBCURL in boatconfig.h must set to 1.
*/

#ifndef __CURLPORT_H__
#define __CURLPORT_H__

#include "boatconfig.h"

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

/*!****************************************************************************
 * @brief Initialize libcurl.
 *
 * @details
 *   This function initializes libcurl. It also dynamically allocates storage to
 *   receive response from the peer.
 *   
 * @param This function doesn't take any argument.
 * 
 * @return
 *   This function returns a pointer to curlport context.\n
 *   It returns NULL if initialization fails.
 *
 * @see CurlPortDeinit()
 ******************************************************************************/
CurlPortContext * CurlPortInit(void);


/*!****************************************************************************
 * @brief Deinitialize libcurl.
 *
 * @details
 *   This function de-initializes libcurl. It also frees the dynamically
 *   allocated storage to receive response from the peer.
 * 
 * @param[in] curlport_context_ptr
 *   Pointer to the context of curlport, which is returned by CurlPortInit()
 *
 * @return
 *   This function doesn't return any value.
 *
 * @see CurlPortInit()    
 ******************************************************************************/
void CurlPortDeinit( CurlPortContext * curlport_context_ptr );



/*!****************************************************************************
 * @brief Set options for use with libcurl.
 *
 * @details
 *   This function is a dummy function and does nothing.	 
 *
 * @param[in] curlport_context_ptr
 *   A pointer to the curlport context
 *	 
 * @param[in] remote_url_str
 *   The URL of the remote server.
 *
 * @return
 *   This function always returns BOAT_SUCCESS.
 ******************************************************************************/
BOAT_RESULT CurlPortSetOpt(CurlPortContext * curlport_context_ptr, BCHAR *remote_url_str);



/*!****************************************************************************
 * @brief Perform a synchronous HTTP POST and wait for its response.
 *
 * @details
 *   This function initiates a curl session, performs a synchronous HTTP POST
 *   and waits for its response.
 *
 * @param[in] curlport_context_ptr
 *   A pointer to the curlport context. 
 *	 
 * @param[in] request_str
 *   A pointer to the request string to POST.
 *
 * @param[in] request_len
 *   The length of <request_str> excluding NULL terminator. This function is
 *   wrapped by RpcRequestSync() and thus takes this argument for compatibility
 *   with the wrapper function. Typically it equals to strlen(request_str).
 *
 * @param[out] response_str_ptr
 *   The address of a BCHAR* pointer (i.e. a double pointer) to hold the address
 *   of the receiving buffer.\n
 *   The receiving buffer is internally maintained by curlport and the caller
 *   shall only read from the buffer. DO NOT modify the buffer or save the address
 *   for later use.
 *
 * @param[out] response_len_ptr
 *   The address of a BUINT32 integer to hold the effective length of
 *   <response_str_ptr> excluding NULL terminator. This function is wrapped by
 *   RpcRequestSync() and thus takes this argument for compatibility with the
 *   wrapper function. Typically it equals to strlen(response_str_ptr).
 *
 * @return
 *   This function returns BOAT_SUCCESS if succeeds.
 *   Otherwise it returns one of the error codes.
 *
 * @see https://curl.haxx.se/libcurl/c/curl_easy_setopt.html
 * @see https://curl.haxx.se/libcurl/c/curl_easy_perform.html 
 ******************************************************************************/
BOAT_RESULT CurlPortRequestSync( CurlPortContext * curlport_context_ptr,
                                 const BCHAR *request_str,
                                 BUINT32 request_len,
                                 BOAT_OUT BCHAR **response_str_ptr,
                                 BOAT_OUT BUINT32 *response_len_ptr );


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of #if RPC_USE_LIBCURL == 1 */
#endif /* end of #ifndef __CURLPORT_H__ */

