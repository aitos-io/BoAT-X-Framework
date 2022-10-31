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

static char * response_data = NULL;
static int response_len = 0;

static void response_cb(char * data, int len, int num, void *cbdata)
{
	char * temp;
	char *private_data=(char *)cbdata;

	temp = (void*)BoatMalloc(response_len + len);
	if (!temp) {
		return;
	}
	memset(temp, 0, response_len + len);
	if (response_data) {
		memcpy(temp, response_data, response_len);
		BoatFree(response_data);
	}
	memcpy(&temp[response_len], data, len);
	response_data = temp;
	response_len += len;
}

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
    
        olport_context_ptr->curlport_response.string_space = OLHTTPPORT_RECV_BUF_SIZE_STEP;
        olport_context_ptr->curlport_response.string_len = 0;


        olport_context_ptr->curlport_response.string_ptr = BoatMalloc(OLHTTPPORT_RECV_BUF_SIZE_STEP);
        
        if( olport_context_ptr->curlport_response.string_ptr == NULL )
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
    
    olport_context_ptr->curlport_response.string_space = 0;
    olport_context_ptr->curlport_response.string_len = 0;

    if( olport_context_ptr->curlport_response.string_ptr != NULL )
    {
        BoatFree(olport_context_ptr->curlport_response.string_ptr);
    }

    olport_context_ptr->curlport_response.string_ptr = NULL;

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

    // Set HTTP Type: POST
	ol_http_client_setopt(client, HTTPCLIENT_OPT_METHOD, HTTPCLIENT_REQUEST_POST);	/*set method,support GET/POST/PUT*/

    // Set HTTP HEADER Options
	header = ol_http_client_list_append(header, "Content-Type: text/xml;charset=UTF-8\r\n");
	header = ol_http_client_list_append(header, "SOAPAction:\r\n");

	ol_http_client_setopt(client, HTTPCLIENT_OPT_HTTPHEADER, header);

    // Set callback and receive buffer for RESPONSE
    // Clean up response buffer
    olport_context_ptr->curlport_response.string_ptr[0] = '\0';
    olport_context_ptr->curlport_response.string_len = 0;
    ol_http_client_setopt(client, HTTPCLIENT_OPT_RESPONSECB_DATA, &olport_context_ptr->curlport_response.string_ptr);
	ol_http_client_setopt(client, HTTPCLIENT_OPT_RESPONSECB, response_cb);

    // Set content to POST    
	ol_http_client_setopt(client, HTTPCLIENT_OPT_POSTDATA, request_str);
	ol_http_client_setopt(client, HTTPCLIENT_OPT_POSTLENGTH, request_len);

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
        *response_str_ptr = olport_context_ptr->curlport_response.string_ptr;
        *response_len_ptr = olport_context_ptr->curlport_response.string_len;
        
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
