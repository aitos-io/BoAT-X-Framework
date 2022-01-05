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

/*!@brief libcurl porting for RPC

@file
curlport.c is the libcurl porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use libcurl porting, RPC_USE_LIBCURL in boatconfig.h must set to 1.
*/
#include "boatconfig.h"
#include "boatinternal.h"

#if RPC_USE_LIBCURL == 1
#include "rpcport.h"
#include "curlport.h"
#include "curl/curl.h"


/*!****************************************************************************
 * @brief Callback function to write received data from the peer to the user specified buffer.
 *
 * @details
 *   This function is a callback function as per libcurl CURLOPT_WRITEFUNCTION option.
 *   libcurl will call this function (possibly) multiple times to write received
 *   data from peer to the buffer specified by this function. The received data
 *   are typically some RESPONSE from the HTTP server.
 *   \n The receiving buffer is dynamically allocated. If the received data from
 *   the peer exceeds the current buffer size, a new buffer that could hold all
 *   data will be allocated and previously received data will be copied to the
 *   new buffer as well as the newly received data.
 * 
 * @param[in] data_ptr
 *   A pointer given by libcurl, pointing to the received data from peer.
 *
 * @param[in] size
 *   For historic reasons, libcurl will always call with <size> = 1.
 *
 * @param[in] nmemb
 *   <nmemb> is the size of the data chunk to write. It doesn't include null
 *   terminator even if the data were string.\n
 *   For backward compatibility, use <size> * <nmemb> to calculate the size of
 *   the received data.
 *
 * @param[in] userdata
 *   <userdata> is the value previously set by CURLOPT_WRITEDATA option.
 *   Typically it's a pointer to a struct which contains information about the
 *   receiving buffer.
 *  
 * @return
 *   This function returns how many bytes are written into the user buffer.
 *   If the returned size differs from <size>*<nmemb>, libcurl will treat it as
 *   a failure.
 *
 * @see https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 ******************************************************************************/
__BOATSTATIC size_t CurlPortWriteMemoryCallback( void *data_ptr, size_t size, 
											     size_t nmemb, void *userdata )
{
    size_t data_size;
    StringWithLen *mem;
    BUINT32 expand_size;
    BUINT32 expand_steps;
    BCHAR *expanded_str;
    BUINT32 expanded_to_space;
    
    mem = (StringWithLen*)userdata;

    // NOTE: For historic reasons, argument size is always 1 and nmemb is the
    // size of the data chunk to write. And size * nmemb doesn't include null
    // terminator even if the data were string.
    data_size = size * nmemb;
    
    // If response buffer has enough space:
    if( mem->string_space - mem->string_len > data_size ) // 1 more byte reserved for null terminator
    {
        memcpy(mem->string_ptr + mem->string_len, data_ptr, data_size);
        mem->string_len += data_size;
        mem->string_ptr[mem->string_len] = '\0';
    }
    else // If response buffer has no enough space
    {
        // If malloc is supported, expand the response buffer in steps of
        // CURLPORT_RECV_BUF_SIZE_STEP.
        expand_size  = data_size - (mem->string_space - mem->string_len) + 1; // plus 1 for null terminator
        expand_steps = (expand_size - 1) / CURLPORT_RECV_BUF_SIZE_STEP + 1;
        expanded_to_space = expand_steps * CURLPORT_RECV_BUF_SIZE_STEP + mem->string_space;
    
        expanded_str = BoatMalloc(expanded_to_space);

        if( expanded_str != NULL )
        {
            memcpy(expanded_str, mem->string_ptr, mem->string_len);
            memcpy(expanded_str + mem->string_len, data_ptr, data_size);
            BoatFree(mem->string_ptr);
            mem->string_ptr = expanded_str;
            mem->string_space = expanded_to_space;
            mem->string_len += data_size;
            mem->string_ptr[mem->string_len] = '\0';
        }

    }

    return data_size;
}


CurlPortContext * CurlPortInit(void)
{
    CurlPortContext *curlport_context_ptr;

    curlport_context_ptr = BoatMalloc(sizeof(CurlPortContext));

    if( curlport_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate Curl Context.");
    }
    else
    {
    
        curlport_context_ptr->curlport_response.string_space = CURLPORT_RECV_BUF_SIZE_STEP;
        curlport_context_ptr->curlport_response.string_len = 0;


        curlport_context_ptr->curlport_response.string_ptr = BoatMalloc(CURLPORT_RECV_BUF_SIZE_STEP);
        
        if( curlport_context_ptr->curlport_response.string_ptr == NULL )
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate Curl RESPONSE buffer.");
            BoatFree(curlport_context_ptr);
            curlport_context_ptr = NULL;
        }
    }
    
    return curlport_context_ptr;
}


void CurlPortDeinit(CurlPortContext * curlport_context_ptr)
{
    if( curlport_context_ptr == NULL )
    {
        return;
    }
    
    curlport_context_ptr->curlport_response.string_space = 0;
    curlport_context_ptr->curlport_response.string_len = 0;

    if( curlport_context_ptr->curlport_response.string_ptr != NULL )
    {
        BoatFree(curlport_context_ptr->curlport_response.string_ptr);
    }

    curlport_context_ptr->curlport_response.string_ptr = NULL;

    BoatFree(curlport_context_ptr);

    return;
}


BOAT_RESULT CurlPortSetOpt(CurlPortContext * curlport_context_ptr, BCHAR *remote_url_str)
{
    if( curlport_context_ptr == NULL || remote_url_str == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    curlport_context_ptr->remote_url_str = remote_url_str;

    return BOAT_SUCCESS;
}


BOAT_RESULT CurlPortRequestSync( CurlPortContext * curlport_context_ptr,
                                 const BCHAR *request_str,
                                 BUINT32 request_len,
                                 BOAT_OUT BCHAR **response_str_ptr,
                                 BOAT_OUT BUINT32 *response_len_ptr )
{
    CURL *curl_ctx_ptr = NULL;
    struct curl_slist *curl_opt_list_ptr = NULL;
    CURLcode curl_result;
    
    long info;
    BOAT_RESULT result = BOAT_ERROR;
    boat_try_declare;


    if(   curlport_context_ptr == NULL
       || request_str == NULL
       || response_str_ptr == NULL
       || response_len_ptr == NULL )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, CurlPortRequestSync_cleanup);
    }

    curl_ctx_ptr = curl_easy_init();
    
    if(curl_ctx_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "curl_easy_init() fails.");
        boat_throw(BOAT_ERROR_CURL_INIT_FAIL, CurlPortRequestSync_cleanup);
    }
    
    // Set RPC URL in format "<protocol>://<target name or IP>:<port>". e.g. "http://192.168.56.1:7545"
    curl_result = curl_easy_setopt(curl_ctx_ptr, CURLOPT_URL, curlport_context_ptr->remote_url_str);
    if( curl_result != CURLE_OK )
    {
        BoatLog(BOAT_LOG_NORMAL, "Unknown URL: %s", curlport_context_ptr->remote_url_str);
        boat_throw(BOAT_ERROR_CURL_SETOPT_FAIL, CurlPortRequestSync_cleanup);
    }

    // Configure all protocols to be supported
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_PROTOCOLS, CURLPROTO_ALL);
                   
    // Configure SSL Certification Verification
    // If certification file is not available, set them to 0.
    // See: https://curl.haxx.se/libcurl/c/CURLOPT_SSL_VERIFYPEER.html
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_SSL_VERIFYHOST, 0);

    // To specify a certificate file or specify a path containing certification files
    // Only make sense when CURLOPT_SSL_VERIFYPEER is set to non-zero.
    // curl_easy_setopt(curl_ctx_ptr, CURLOPT_CAINFO, "/etc/certs/cabundle.pem");
    // curl_easy_setopt(curl_ctx_ptr, CURLOPT_CAPATH, "/etc/cert-dir");

    // Allow Re-direction
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_FOLLOWLOCATION,1); 

    // Verbose Debug Info.
    // curl_easy_setopt(curl_ctx_ptr, CURLOPT_VERBOSE, 1);

    // Set HTTP Type: POST
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_POST, 1L);

    // Set redirection: No
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_FOLLOWLOCATION, 0);

    // Set entire curl timeout in millisecond. This time includes DNS resloving.
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_TIMEOUT_MS, 30000L);

    // Set Connection timeout in millisecond
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_CONNECTTIMEOUT_MS, 10000L);

    // Set HTTP HEADER Options
    curl_opt_list_ptr = curl_slist_append(curl_opt_list_ptr,"Content-Type:application/json;charset=UTF-8");
    if( curl_opt_list_ptr == NULL ) boat_throw(BOAT_ERROR_CURL_SLIST_APPEND_FAIL, CurlPortRequestSync_cleanup);
    
    curl_opt_list_ptr = curl_slist_append(curl_opt_list_ptr,"Accept:application/json, text/javascript, */*;q=0.01");
    if( curl_opt_list_ptr == NULL ) boat_throw(BOAT_ERROR_CURL_SLIST_APPEND_FAIL, CurlPortRequestSync_cleanup);

    curl_opt_list_ptr = curl_slist_append(curl_opt_list_ptr,"Accept-Language:zh-CN,zh;q=0.8");
    if( curl_opt_list_ptr == NULL ) boat_throw(BOAT_ERROR_CURL_SLIST_APPEND_FAIL, CurlPortRequestSync_cleanup);

    curl_easy_setopt(curl_ctx_ptr, CURLOPT_HTTPHEADER, curl_opt_list_ptr);

    // Set callback and receive buffer for RESPONSE
    // Clean up response buffer
    curlport_context_ptr->curlport_response.string_ptr[0] = '\0';
    curlport_context_ptr->curlport_response.string_len = 0;
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_WRITEDATA, &curlport_context_ptr->curlport_response);
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_WRITEFUNCTION, CurlPortWriteMemoryCallback);

    // Set content to POST    
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_POSTFIELDS, request_str);
    curl_easy_setopt(curl_ctx_ptr, CURLOPT_POSTFIELDSIZE, request_len);

    // Perform the RPC request
    curl_result = curl_easy_perform(curl_ctx_ptr);

    if( curl_result != CURLE_OK )
    {
        BoatLog(BOAT_LOG_NORMAL, "curl_easy_perform fails with CURLcode: %d.", curl_result);
        boat_throw(BOAT_ERROR_CURL_CODE_FAIL - curl_result, CurlPortRequestSync_cleanup);
    }
    
    curl_result = curl_easy_getinfo(curl_ctx_ptr, CURLINFO_RESPONSE_CODE, &info);

    if(( curl_result == CURLE_OK ) && (info == 200 || info == 201))
    {
        *response_str_ptr = curlport_context_ptr->curlport_response.string_ptr;
        *response_len_ptr = curlport_context_ptr->curlport_response.string_len;
        
        BoatLog(BOAT_LOG_VERBOSE, "Post: %s", request_str);
        BoatLog(BOAT_LOG_VERBOSE, "Result Code: %ld", info);
        BoatLog(BOAT_LOG_VERBOSE, "Response: %s", *response_str_ptr);
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "curl_easy_getinfo fails with CURLcode: %d, HTTP response code %ld.", curl_result, info);
        boat_throw(BOAT_ERROR_CURL_INFO_FAIL - info, CurlPortRequestSync_cleanup);
    }    

    // Clean Up
    curl_slist_free_all(curl_opt_list_ptr);
    curl_easy_cleanup(curl_ctx_ptr);
    
    result = BOAT_SUCCESS;

    // Exceptional Clean Up
    boat_catch(CurlPortRequestSync_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        
        if( curl_opt_list_ptr != NULL )
        {
            curl_slist_free_all(curl_opt_list_ptr);
        }

        if( curl_ctx_ptr != NULL )
        {
            curl_easy_cleanup(curl_ctx_ptr);
        }
        result = boat_exception;
    }
    
    return result;
}

#endif // end of #if RPC_USE_LIBCURL == 1
