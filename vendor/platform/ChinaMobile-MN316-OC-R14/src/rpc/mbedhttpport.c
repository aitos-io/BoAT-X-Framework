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

/*!@brief mbed HTTP porting for RPC

@file
mbedhttpport.c is the AT command porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use mbed HTTP RPC porting, RPC_USE_MBEDHTTPPORT in boatoptions.h must set to 1.
*/

#include "boatinternal.h"

#if RPC_USE_MBEDHTTPPORT == 1

//#include "rpcport.h"
#include "mbedhttpport.h"

#include "cm_http.h"
#include "cm_eloop.h"

#define HTTP_PATH   "/"
static cm_eloop_handle_t s_main_eloop = NULL;
/*!*****************************************************************************
@brief Initialize mbed HTTP RPC context.

Function: MbedHttpPortInit()

    This function initializes mbed HTTP RPC context. It also dynamically allocates
    storage to receive response from the peer.
    
@see MbedHttpPortDeinit()

@return
    This function returns a pointer to mbed's HTTP RPC context.\n
    It returns NULL if initialization fails.
    

@param This function doesn't take any argument.

*******************************************************************************/
MbedHttpPortContext *MbedHttpPortInit(void)
{
    MbedHttpPortContext *mbedhttpport_context_ptr;
    BOAT_RESULT result = BOAT_SUCCESS;

    mbedhttpport_context_ptr = BoatMalloc(sizeof(MbedHttpPortContext));

    if (mbedhttpport_context_ptr != NULL)
    {
        result = BOAT_SUCCESS;
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate mbed HTTP RPC Context.");
        result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    
    if (result == BOAT_SUCCESS)
    {
        mbedhttpport_context_ptr->http_response_head.string_ptr = BoatMalloc(MBEDHTTPPORT_RECV_BUF_SIZE);

        if (mbedhttpport_context_ptr->http_response_head.string_ptr != NULL)
        {
            mbedhttpport_context_ptr->http_response_head.string_space = MBEDHTTPPORT_RECV_BUF_SIZE;
            mbedhttpport_context_ptr->http_response_head.string_len = 0;
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate mbed HTTP RESPONSE head buffer.");
            result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
    }
    
    if (result == BOAT_SUCCESS)
    {
        mbedhttpport_context_ptr->http_response_body.string_ptr = BoatMalloc(MBEDHTTPPORT_RECV_BUF_SIZE);

        if (mbedhttpport_context_ptr->http_response_body.string_ptr != NULL)
        {
            mbedhttpport_context_ptr->http_response_body.string_space = MBEDHTTPPORT_RECV_BUF_SIZE;
            mbedhttpport_context_ptr->http_response_body.string_len = 0;
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate mbed HTTP RESPONSE body buffer.");
            result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
    }
    
    if (result != BOAT_SUCCESS)
    {
        if (mbedhttpport_context_ptr != NULL)
        {
            if (mbedhttpport_context_ptr->http_response_head.string_ptr != NULL)
            {
                BoatFree(mbedhttpport_context_ptr->http_response_head.string_ptr);
                mbedhttpport_context_ptr->http_response_head.string_ptr = NULL;
                mbedhttpport_context_ptr->http_response_head.string_space = 0;
                mbedhttpport_context_ptr->http_response_head.string_len = 0;
            }

            if (mbedhttpport_context_ptr->http_response_body.string_ptr != NULL)
            {
                BoatFree(mbedhttpport_context_ptr->http_response_body.string_ptr);
                mbedhttpport_context_ptr->http_response_body.string_ptr = NULL;
                mbedhttpport_context_ptr->http_response_body.string_space = 0;
                mbedhttpport_context_ptr->http_response_body.string_len = 0;
            }
            
            BoatFree(mbedhttpport_context_ptr);
            mbedhttpport_context_ptr = NULL;
        }
    }
    
    return mbedhttpport_context_ptr;

}


/*!*****************************************************************************
@brief Deinitialize mbed HTTP RPC context.

Function: MbedHttpPortDeinit()

    This function de-initializes mbed HTTP RPC context. It also frees the dynamically
    allocated storage to receive response from the peer.

@see MbedHttpPortInit()    

@return
    This function doesn't return any value.
    

@param[in] mbedhttpport_context_ptr
    Pointer to the context of MbedHttpPortContext, which is returned by MbedHttpPortInit()

*******************************************************************************/
void MbedHttpPortDeinit(MbedHttpPortContext *mbedhttpport_context_ptr)
{
    if (mbedhttpport_context_ptr == NULL)
    {
        return;
    }
    
    mbedhttpport_context_ptr->http_response_head.string_space = 0;
    mbedhttpport_context_ptr->http_response_head.string_len = 0;

    mbedhttpport_context_ptr->http_response_body.string_space = 0;
    mbedhttpport_context_ptr->http_response_body.string_len = 0;

    if (mbedhttpport_context_ptr->http_response_head.string_ptr != NULL)
    {
        BoatFree(mbedhttpport_context_ptr->http_response_head.string_ptr);
        mbedhttpport_context_ptr->http_response_head.string_ptr = NULL;
    }

    if (mbedhttpport_context_ptr->http_response_body.string_ptr != NULL)
    {
        BoatFree(mbedhttpport_context_ptr->http_response_body.string_ptr);
        mbedhttpport_context_ptr->http_response_body.string_ptr = NULL;
    }

    BoatFree(mbedhttpport_context_ptr);

    return;
}


/*!*****************************************************************************
@brief Set options for use with mbed HTTP RPC.

Function: MbedHttpPortSetOpt()

    This function set options for mbed HTTP.
    

@return
    This function returns BOAT_SUCCESS if succeeds.
    Otherwise it returns one of the error codes.
    

@param[in] mbedhttpport_context_ptr
    A pointer to the mbed HTTP RPC context
    
@param[in] remote_url_str
    The URL of the remote server.

*******************************************************************************/
BOAT_RESULT MbedHttpPortSetOpt(MbedHttpPortContext *mbedhttpport_context_ptr, BCHAR *remote_url_str)
{
    if (mbedhttpport_context_ptr == NULL || remote_url_str == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    mbedhttpport_context_ptr->remote_url_str = remote_url_str;

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Perform a synchronous HTTP POST and wait for its response.

Function: MbedHttpPortRequestSync()

    This function performs a synchronous mbed HTTP POST and waits for its response.

@return
    This function returns BOAT_SUCCESS if succeeds.
    Otherwise it returns one of the error codes.
    
@param[in] mbedhttpport_context_ptr
    A pointer to the mbed HTTP RPC context.
    
@param[in] request_str
    A pointer to the request string to POST.

@param[in] request_len
    The length of <request_str> excluding NULL terminator. This function is
    wrapped by RpcRequestSync() and thus takes this argument for compatibility
    with the wrapper function. Typically it equals to strlen(request_str).

@param[out] response_str_ptr
    The address of a BCHAR* pointer (i.e. a double pointer) to hold the address
    of the receiving buffer.\n
    The receiving buffer is internally maintained by curlport and the caller
    shall only read from the buffer. DO NOT modify the buffer or save the address
    for later use.

@param[out] response_len_ptr
    The address of a BUINT32 integer to hold the effective length of
    <response_str_ptr> excluding NULL terminator. This function is wrapped by
    RpcRequestSync() and thus takes this argument for compatibility with the
    wrapper function. Typically it equals to strlen(response_str_ptr).

*******************************************************************************/
static  cm_httpclient_handle_t http_handle = NULL;
BOAT_RESULT MbedHttpPortRequestSync(MbedHttpPortContext *mbedhttpport_context_ptr,
                                    const BCHAR *request_str,
                                    BUINT32 request_len,
                                    BOAT_OUT BCHAR **response_str_ptr,
                                    BOAT_OUT BUINT32 *response_len_ptr)
{
    cm_httpclient_cfg_t client_cfg;
    //client = {0};
    BSINT32 response_code;
    BSINT32 ret;

    BOAT_RESULT result = BOAT_ERROR;
    boat_try_declare;


    if (mbedhttpport_context_ptr == NULL
       || request_str == NULL
       || response_str_ptr == NULL
       || response_len_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, cleanup);
    }

    if (NULL == http_handle)
    {   
        ret = cm_httpclient_create(mbedhttpport_context_ptr->remote_url_str, NULL, &http_handle);

        if (CM_HTTP_RET_CODE_OK != ret || NULL == http_handle)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Http client creat error.");
            result = BOAT_ERROR_HTTP_CONNECT_FAIL;
            boat_throw(BOAT_ERROR_HTTP_CONNECT_FAIL, cleanup);
        }

        client_cfg.ssl_enable = false;
        client_cfg.ssl_id = 0;
        client_cfg.cid = 0;
        client_cfg.conn_timeout = HTTPCLIENT_CONNECT_TIMEOUT_DEFAULT;
        client_cfg.rsp_timeout = HTTPCLIENT_WAITRSP_TIMEOUT_DEFAULT;
        client_cfg.dns_priority = 0;
        ret = cm_httpclient_set_cfg(http_handle, client_cfg);

        if (CM_HTTP_RET_CODE_OK != ret || NULL == http_handle)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Http client set config error.");
            result = BOAT_ERROR_HTTP_CONNECT_FAIL;
            boat_throw(BOAT_ERROR_HTTP_CONNECT_FAIL, cleanup);
        }

        uint8_t *header_str = "Content-Type: application/json";
        ret = cm_httpclient_custom_header_set(http_handle, header_str, strlen(header_str));
       
        if (CM_HTTP_RET_CODE_OK != ret || NULL == http_handle)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Http header set config error.");
            result = BOAT_ERROR_HTTP_CONNECT_FAIL;
            boat_throw(BOAT_ERROR_HTTP_CONNECT_FAIL, cleanup);
        }
    }

    cm_httpclient_sync_response_t response = {};
    cm_httpclient_sync_param_t param = {HTTPCLIENT_REQUEST_POST, HTTP_PATH, request_len, request_str};

    ret = cm_httpclient_sync_request(http_handle, param, &response);

    if (CM_HTTP_RET_CODE_OK != ret || NULL == http_handle)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Http client request error.");
        result = BOAT_ERROR_HTTP_POST_FAIL;
        boat_throw(BOAT_ERROR_HTTP_POST_FAIL, cleanup);
    }

    BoatLog(BOAT_LOG_VERBOSE, "response_code is %d\r\n", response.response_code);
    BoatLog(BOAT_LOG_VERBOSE, "response_header_len is %d\r\n", response.response_header_len);
    BoatLog(BOAT_LOG_VERBOSE, "response_content_len is %d\r\n", response.response_content_len);
    
    response_code = response.response_code;


    if (response_code == 200 || response_code == 201)
    {
        if (mbedhttpport_context_ptr->http_response_body.string_ptr == NULL)
        {
            cm_httpclient_sync_free_data(http_handle);
            BoatLog(BOAT_LOG_CRITICAL, "Memory malloc failed.");
            result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, cleanup);
        }
        mbedhttpport_context_ptr->http_response_body.string_len = response.response_content_len;
        memcpy(mbedhttpport_context_ptr->http_response_body.string_ptr, response.response_content, response.response_content_len);
        
        *response_str_ptr = mbedhttpport_context_ptr->http_response_body.string_ptr;
        *response_len_ptr = mbedhttpport_context_ptr->http_response_body.string_len;
            
        BoatLog(BOAT_LOG_VERBOSE, "Post: %s", request_str);
        BoatLog(BOAT_LOG_VERBOSE, "Result Code: %d", response_code);
        BoatLog(BOAT_LOG_VERBOSE, "Response: %s", *response_str_ptr);

        result = BOAT_SUCCESS;
    }
    else
    {
        result = BOAT_ERROR_HTTP_POST_FAIL - response_code;
        BoatLog(BOAT_LOG_NORMAL, "HTTP POST fails with response code %d.", response_code);
    }  

    cm_httpclient_sync_free_data(http_handle);

    // Exceptional Clean Up
    boat_catch(cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }
    
    return result;
}


 
#endif // end of #if RPC_USE_MBEDHTTPPORT == 1
