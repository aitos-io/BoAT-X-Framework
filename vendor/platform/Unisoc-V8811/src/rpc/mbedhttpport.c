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
nHttp_info *MbedHttpPortInit(void)
{
    return Init_Http();
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
void MbedHttpPortDeinit(nHttp_info *mbedhttpport_context_ptr)
{
    Term_Http(rpc_context_ptr);

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
BOAT_RESULT MbedHttpPortSetOpt(nHttp_info *mbedhttpport_context_ptr, BCHAR *remote_url_str)
{
    if (mbedhttpport_context_ptr == NULL || remote_url_str == NULL)
    {
        return BOAT_ERROR;
    }

    if (Http_para(rpc_context_ptr, "URL", remote_url_str))
    {
        return BOAT_SUCCESS;
    }
    return BOAT_ERROR
}



/*!*****************************************************************************
@brief Perform a synchronous HTTP POST and wait for its response.

Function: MbedHttpPortRequestSync()

    This function initiates a curl session, performs a synchronous HTTP POST
    and waits for its response.

    is a callback function as per libcurl CURLOPT_WRITEFUNCTION option.
    libcurl will call this function (possibly) multiple times to write received
    data from peer to the buffer specified by this function. The received data
    are typically some RESPONSE from the HTTP server.

@see https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
    

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
BOAT_RESULT MbedHttpPortRequestSync(nHttp_info *mbedhttpport_context_ptr,
                                    const BCHAR *request_str,
                                    BUINT32 request_len,
                                    BOAT_OUT BCHAR **response_str_ptr,
                                    BOAT_OUT BUINT32 *response_len_ptr)
{
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
        result = BOAT_ERROR;
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, cleanup);
    }
    
    Http_para(mbedhttpport_context_ptr, "TIMEOUT", "30");
    //client.timeout_in_sec = 30;
    
    //client_data.is_more = false;
    
    /*client_data.header_buf = mbedhttpport_context_ptr->http_response_head.string_ptr;
    client_data.header_buf_len = mbedhttpport_context_ptr->http_response_head.string_space;
    memset(client_data.header_buf, 0, client_data.header_buf_len);

    client_data.response_buf = mbedhttpport_context_ptr->http_response_body.string_ptr;
    client_data.response_buf_len = mbedhttpport_context_ptr->http_response_body.string_space;
    memset(client_data.response_buf, 0, client_data.response_buf_len);*/

    http_setUserdata(mbedhttpport_context_ptr, request_str, request_len);
    //Http_para(mbedhttpport_context_ptr, "USERDATA", request_str);
    Http_para(mbedhttpport_context_ptr, "CONTENT", "application/json");
     
    BoatLog(BOAT_LOG_VERBOSE, "[HTTP]POST Start\n");
    if (!Http_postn(mbedhttpport_context_ptr))
    {
        BoatLog(BOAT_LOG_VERBOSE, "[HTTP]POST Failed:%d\n", ret);
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE, "[HTTP]POST:\n%s\n", mbedhttpport_context_ptr->user_data);

        response_code = mbedhttpport_context_ptr->status_code;
        
        if (response_code == 200 || response_code == 201)
        {
            *response_len_ptr = mbedhttpport_context_ptr->content_length;
            *response_str_ptr = mbedhttpport_context_ptr->user_data;
            
            BoatLog(BOAT_LOG_VERBOSE, "Post: %s", request_str);
            BoatLog(BOAT_LOG_VERBOSE, "Result Code: %d", response_code);
            BoatLog(BOAT_LOG_VERBOSE, "Response: %s", *response_str_ptr);

            result = BOAT_SUCCESS;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "HTTP POST fails with response code %d.", response_code);
        }  
        
    }


    // Exceptional Clean Up
    boat_catch(cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }
    
    return result;
}


 
#endif // end of #if RPC_USE_MBEDHTTPPORT == 1
