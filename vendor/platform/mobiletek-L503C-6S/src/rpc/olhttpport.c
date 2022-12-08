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

#if RPC_USE_OLHTTPPORT == 1
#include "rpcport.h"
#include "olhttpport.h"
#include "http_api.h"

__BOATSTATIC void response_cb( char *data_ptr, int size, 
											     int nmemb, void *userdata )
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
    BoatLog(BOAT_LOG_NORMAL, "response_cb data_size : %d.", data_size);
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
        expand_steps = (expand_size - 1) / OLHTTPPORT_RECV_BUF_SIZE_STEP + 1;
        expanded_to_space = expand_steps * OLHTTPPORT_RECV_BUF_SIZE_STEP + mem->string_space;
    
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

}


/*
data: the data is received http context data
len: the length of http context data 
num: the http response header "Content-Length" value
cbdata: user private data
*/

// static void response_cb(char * data, int len, int num, void *cbdata)
// {
// 	char * temp;
// 	char *private_data=(char *)cbdata;

//     user_app_printf("[http_client_test]Get private_data %s", data);

// 	user_app_printf("[http_client_test]Get data %d", len);

// 	temp = (void*)ol_malloc(response_len + len);
// 	if (!temp) {
//         user_app_printf("[http_client_test]Malloc failed...");
// 		return;
// 	}
// 	memset(temp, 0, response_len + len);
// 	if (response_data) {
// 		memcpy(temp, response_data, response_len);
// 		ol_free(response_data);
// 	}
// 	memcpy(&temp[response_len], data, len);
// 	response_data = temp;
// 	response_len += len;
// }

http_client * OlHttpPortInit(void)
{
    OlHttpPortContext *olport_context_ptr;

    olport_context_ptr = BoatMalloc(sizeof(OlHttpPortContext));

    if( olport_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate OlHttp Context.");
    }
    else
    {
    
        olport_context_ptr->olhttpport_response.string_space = OLHTTPPORT_RECV_BUF_SIZE_STEP;
        olport_context_ptr->olhttpport_response.string_len = 0;


        olport_context_ptr->olhttpport_response.string_ptr = BoatMalloc(OLHTTPPORT_RECV_BUF_SIZE_STEP);
        
        if( olport_context_ptr->olhttpport_response.string_ptr == NULL )
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate OlHttp RESPONSE buffer.");
            BoatFree(olport_context_ptr);
            olport_context_ptr = NULL;
        }
    }
    
    return olport_context_ptr;
}


void OlHttpPortDeinit(OlHttpPortContext * olport_context_ptr)
{
    if( olport_context_ptr == NULL )
    {
        return;
    }
    
    olport_context_ptr->olhttpport_response.string_space = 0;
    olport_context_ptr->olhttpport_response.string_len = 0;

    if( olport_context_ptr->olhttpport_response.string_ptr != NULL )
    {
        BoatFree(olport_context_ptr->olhttpport_response.string_ptr);
    }

    olport_context_ptr->olhttpport_response.string_ptr = NULL;

    BoatFree(olport_context_ptr);

    return;
}


BOAT_RESULT OlHttpSetOpt(OlHttpPortContext * olport_context_ptr, BCHAR *remote_url_str)
{
    if( olport_context_ptr == NULL || remote_url_str == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    olport_context_ptr->remote_url_str = remote_url_str;

    return BOAT_SUCCESS;
}


BOAT_RESULT OlHttpPortRequestSync( OlHttpPortContext * olport_context_ptr,
                                 const BCHAR *request_str,
                                 BUINT32 request_len,
                                 BOAT_OUT BCHAR **response_str_ptr,
                                 BOAT_OUT BUINT32 *response_len_ptr )
{
	http_client * client = NULL;
	struct http_client_list * header = NULL;
    
    long info;
    BOAT_RESULT result = BOAT_ERROR;
    int olhttp_result = 0;
    boat_try_declare;


    if(   olport_context_ptr == NULL
       || request_str == NULL
       || response_str_ptr == NULL
       || response_len_ptr == NULL )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, OlHttpPortRequestSync_cleanup);
    }

    client = ol_http_client_init();
    
    if(client == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "ol_http_client_init() fails.");
        boat_throw(BOAT_ERROR_CURL_INIT_FAIL, OlHttpPortRequestSync_cleanup);
    }
    BoatLog(BOAT_LOG_CRITICAL, "olhttp url = %s .",olport_context_ptr->remote_url_str);
    // Set RPC URL in format "<protocol>://<target name or IP>:<port>". e.g. "http://192.168.56.1:7545"
    olhttp_result = ol_http_client_setopt(client, HTTPCLIENT_OPT_URL, olport_context_ptr->remote_url_str);
    if( olhttp_result != HTTP_CLIENT_OK )
    {
        BoatLog(BOAT_LOG_NORMAL, "Unknown URL: %s", olport_context_ptr->remote_url_str);
        boat_throw(BOAT_ERROR_CURL_SETOPT_FAIL, OlHttpPortRequestSync_cleanup);
    }

	ol_http_client_setopt(client, HTTPCLIENT_OPT_HTTP1_0, 0);				/*"0" is HTTP 1.1, "1" is HTTP 1.0*/

    //ol_http_client_setopt(client, HTTPCLIENT_OPT_AUTH_TYPE, HTTP_AUTH_TYPE_BASE);	/*auth type support base and digest*/
    //ol_http_client_setopt(client, HTTPCLIENT_OPT_AUTH_USERNAME, "admin");			 /*username*/
	//ol_http_client_setopt(client, HTTPCLIENT_OPT_AUTH_PASSWORD, "admin");			 /*password*/

    // Set HTTP Type: POST
	ol_http_client_setopt(client, HTTPCLIENT_OPT_METHOD, HTTPCLIENT_REQUEST_POST);	/*set method,support GET/POST/PUT*/

    // Set HTTP HEADER Options
	header = ol_http_client_list_append(header, "Content-Type:application/json;charset=UTF-8\r\n");
	// header = ol_http_client_list_append(header, "Accept:application/json, text/javascript, */*;q=0.01\r\n");
	// header = ol_http_client_list_append(header, "Accept-Language:zh-CN,zh;q=0.8\r\n");

	ol_http_client_setopt(client, HTTPCLIENT_OPT_HTTPHEADER, header);

    // Set callback and receive buffer for RESPONSE
    // Clean up response buffer
    olport_context_ptr->olhttpport_response.string_ptr[0] = '\0';
    olport_context_ptr->olhttpport_response.string_len = 0;
    ol_http_client_setopt(client, HTTPCLIENT_OPT_RESPONSECB_DATA, &olport_context_ptr->olhttpport_response.string_ptr);
	ol_http_client_setopt(client, HTTPCLIENT_OPT_RESPONSECB, response_cb);

    // Set content to POST    
	ol_http_client_setopt(client, HTTPCLIENT_OPT_POSTDATA, request_str);
	ol_http_client_setopt(client, HTTPCLIENT_OPT_POSTLENGTH, request_len);

    ol_http_client_getinfo(client, HTTPCLIENT_GETINFO_TCP_STATE, &olhttp_result);

    if( olhttp_result != HTTP_TCP_ESTABLISHED )
    {
        BoatLog(BOAT_LOG_NORMAL, "http tcp not connecting: %d.", olhttp_result);
        boat_throw(BOAT_ERROR_CURL_CODE_FAIL - olhttp_result, OlHttpPortRequestSync_cleanup);
    }

    // Perform the RPC request
    olhttp_result = ol_http_client_perform(client);

    if( olhttp_result != HTTP_CLIENT_OK )
    {
        BoatLog(BOAT_LOG_NORMAL, "ol_http_client_perform fails: %d.", olhttp_result);
        boat_throw(BOAT_ERROR_CURL_CODE_FAIL - olhttp_result, OlHttpPortRequestSync_cleanup);
    }
    
    olhttp_result = ol_http_client_getinfo(client, HTTPCLIENT_GETINFO_RESPONSE_CODE, &info);

    if(( olhttp_result == HTTP_CLIENT_OK ) && (info != HTTP_TCP_ESTABLISHED))
    {
        *response_str_ptr = olport_context_ptr->olhttpport_response.string_ptr;
        *response_len_ptr = olport_context_ptr->olhttpport_response.string_len;
        
        BoatLog(BOAT_LOG_VERBOSE, "Post: %s", request_str);
        BoatLog(BOAT_LOG_VERBOSE, "Result Code: %ld", info);
        BoatLog(BOAT_LOG_VERBOSE, "Response: %s", *response_str_ptr);
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "ol_http_client_getinfo fails with: %d, HTTP response code %ld.", olhttp_result, info);
        boat_throw(BOAT_ERROR_CURL_INFO_FAIL - info, OlHttpPortRequestSync_cleanup);
    }    

    // Clean Up
    if (header) 
    {
		ol_http_client_list_destroy(header);
		header = NULL;
	}
    ol_http_client_shutdown(client);
    
    result = BOAT_SUCCESS;

    // Exceptional Clean Up
    boat_catch(OlHttpPortRequestSync_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        
        if( header != NULL )
        {
		    ol_http_client_list_destroy(header);
            header = NULL;
        }

        if( client != NULL )
        {
            ol_http_client_shutdown(client);
        }
        result = boat_exception;
    }
    
    return result;
}

#endif // end of #if RPC_USE_LIBCURL == 1
