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

/*!@brief nwy HTTP porting for RPC

@file
nwyhttpport.c is the AT command porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use nwy HTTP RPC porting, RPC_USE_NWYHTTPPORT in boatconfig.h must set to 1.
*/

#include "boatinternal.h"

#if RPC_USE_NWYHTTPPORT == 1
#include "nwyhttpport.h"

#include "nwy_http.h"
#include "nwy_ftp.h"
#include "nwy_osi_api.h"

BBOOL g_http_cb_falg =  BOAT_FALSE;
#define BOAT_NWY_BUFF_MAX 2*1024
static char boat_nwy_recv_buff[BOAT_NWY_BUFF_MAX] = {0};
BUINT32 recv_data_len = 0;

/*!*****************************************************************************
@brief Initialize nwy HTTP RPC context.

Function: NwyHttpPortInit()

    This function initializes nwy HTTP RPC context. It also dynamically allocates
    storage to receive response from the peer.
    
@see NwyHttpPortDeinit()

@return
    This function returns a pointer to nwy's HTTP RPC context.\n
    It returns NULL if initialization fails.
    

@param This function doesn't take any argument.

*******************************************************************************/
NwyHttpPortContext * NwyHttpPortInit(void)
{
    NwyHttpPortContext *nwyhttpport_context_ptr;

    nwyhttpport_context_ptr = BoatMalloc(sizeof(NwyHttpPortContext));

    if( nwyhttpport_context_ptr == NULL )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate nwy HTTP RPC Context.");
    }
    else
    {
    
        nwyhttpport_context_ptr->http_response.string_space = NWYHTTPPORT_RECV_BUF_SIZE;
        nwyhttpport_context_ptr->http_response.string_len = 0;


        nwyhttpport_context_ptr->http_response.string_ptr = BoatMalloc(NWYHTTPPORT_RECV_BUF_SIZE);
        
        if( nwyhttpport_context_ptr->http_response.string_ptr == NULL )
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate nwy HTTP RESPONSE buffer.");
            BoatFree(nwyhttpport_context_ptr);
            nwyhttpport_context_ptr = NULL;
        }
    }
    
    return nwyhttpport_context_ptr;

}


/*!*****************************************************************************
@brief Deinitialize nwy HTTP RPC context.

Function: NwyHttpPortDeinit()

    This function de-initializes nwy HTTP RPC context. It also frees the dynamically
    allocated storage to receive response from the peer.

@see NwyHttpPortInit()    

@return
    This function doesn't return any value.
    

@param[in] nwyhttpport_context_ptr
    Pointer to the context of NwyHttpPortContext, which is returned by NwyHttpPortInit()

*******************************************************************************/
void NwyHttpPortDeinit(NwyHttpPortContext * nwyhttpport_context_ptr)
{
    if( nwyhttpport_context_ptr == NULL )
    {
        return;
    }
    
    nwyhttpport_context_ptr->http_response.string_space = 0;
    nwyhttpport_context_ptr->http_response.string_len = 0;


    if( nwyhttpport_context_ptr->http_response.string_ptr != NULL )
    {
        BoatFree(nwyhttpport_context_ptr->http_response.string_ptr);
    }


    nwyhttpport_context_ptr->http_response.string_ptr = NULL;

    BoatFree(nwyhttpport_context_ptr);

    return;
}


/*!*****************************************************************************
@brief Set options for use with nwy HTTP RPC.

Function: NwyHttpPortSetOpt()

    This function set options for nwy HTTP.
    

@return
    This function returns BOAT_SUCCESS if succeeds.
    Otherwise it returns one of the error codes.
    

@param[in] nwyhttpport_context_ptr
    A pointer to the nwy HTTP RPC context
    
@param[in] remote_url_str
    The URL of the remote server.

*******************************************************************************/
BOAT_RESULT NwyHttpPortSetOpt(NwyHttpPortContext * nwyhttpport_context_ptr, BCHAR *remote_url_str)
{
    if( nwyhttpport_context_ptr == NULL || remote_url_str == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    nwyhttpport_context_ptr->remote_url_str = remote_url_str;

    return BOAT_SUCCESS;
}

void nwy_boat_http_result_cb(nwy_ftp_result_t *param)
{
    char *recv_len_ptr = NULL;
    char *recv_jsonrpc_ptr = NULL;
    int i = 0;
    char curr_char;
    
    if(NULL == param)
    {
        BoatLog(BOAT_LOG_NORMAL, "event is NULL\r\n");
    }

   // OSI_LOGI(0, "event is %d", param->event);
    if(NWY_HTTP_DNS_ERR == param->event)
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\nHTTP dns err\r\n");
    }
    else if(NWY_HTTP_OPEN_FAIL == param->event)
    {
       BoatLog(BOAT_LOG_NORMAL, "\r\nHTTP open fail\r\n");
    }
    else if(NWY_HTTP_OPENED == param->event )
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\nHTTP setup success\r\n");
        g_http_cb_falg = BOAT_FALSE;
    }
    else if(NWY_HTTPS_SSL_CONNECTED == param->event)
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\nHTTPS setup success\r\n");
    }
    else if(NWY_HTTP_CLOSED_PASV == param->event || NWY_HTTP_CLOSED == param->event)
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\nHTTP closed\r\n");
        g_http_cb_falg = BOAT_FALSE;
    }
    else if(NWY_HTTP_DATA_RECVED == param->event)
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\n 0 HTTP recv data len %d\r\n",param->data_len);
        BoatLog(BOAT_LOG_NORMAL, "\r\n 0 %s\r\n", param->data);
        
        if (param->data_len > 50 && strstr((const char *)param->data, (const char *)"HTTP/1.1 200 OK"))
        {
            recv_len_ptr = strstr((const char *)param->data, (const char *)"Content-Length:");
            if (NULL != recv_len_ptr)
            {
                i = strlen("Content-Length:");
                recv_data_len = 0;
                do
                {
                    i++;
                    curr_char = *(recv_len_ptr + i);
                    if (curr_char >= '0' && curr_char <= '9')
                    {
                        recv_data_len = recv_data_len*10 + (curr_char - '0');
                    }
                    else
                    {
                        break;
                    }
                }while (curr_char != 0x0d);
                    
                BoatLog(BOAT_LOG_NORMAL, "recv_data_len=%d\r\n", recv_data_len);
            }

            
            recv_jsonrpc_ptr = strstr((const char *)param->data,(const char *)"jsonrpc");
            
            if (NULL != recv_jsonrpc_ptr && recv_data_len > 0 && recv_data_len < BOAT_NWY_BUFF_MAX)
            {
                memset(boat_nwy_recv_buff, 0x00, sizeof(boat_nwy_recv_buff));
                strncpy(boat_nwy_recv_buff, (recv_jsonrpc_ptr - 2), recv_data_len);
            }
            
            g_http_cb_falg = BOAT_FALSE;
        }
        
        //nwy_ext_echo("\r\nrecv_data_len = %d\n", recv_data_len);
       // nwy_ext_echo("\r\nboat_nwy_recv_buff = %s\r\n", boat_nwy_recv_buff);
    }
    else if(NWY_HTTP_DATA_SEND_ERR == param->event)
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\nfile send error");
    }
    else if(NWY_HTTPS_SSL_INIT_ERROR == param->event)
    {
       BoatLog(BOAT_LOG_NORMAL, "\r\nHTTPS SSL init fail");
    }
    else if(NWY_HTTPS_SSL_HANDSHAKE_ERROR == param->event)
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\nHTTPS SSL handshare fail");
    }
    else if(NWY_HTTPS_SSL_AUTH_FAIL == param->event)
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\nHTTPS SSL Authentication fail");
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "\r\n unkown error");
    }

    return;
}

int boat_get_port_from_url(char *url_ptr)
{
    int i = 0;
    int port = 0;
    char *port_ptr = NULL;
    char port_char = 0;
        
    port_ptr = strstr(url_ptr, ":");
    if (NULL != port_ptr && strstr((port_ptr + 1), ":"))
    {
        port_ptr = strstr((port_ptr + 1), ":");
    }
    
    if (NULL != port_ptr)
    {
        i = 0;
        do
        {
            i++;
            port_char = *(port_ptr + i);
            if (port_char >= '0' && port_char <= '9')
            {
                port = (port * 10) + (port_char - '0');
            }
            else
            {
                break;
            }
            
        } while (port_char >= '0' && port_char <= '9');
    }
    
    if (port <= 0)
    {
        port = 7545;
    }
    
    return port;
}

/*!*****************************************************************************
@brief Perform a synchronous HTTP POST and wait for its response.

Function: NwyHttpPortRequestSync()

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
    
@param[in] nwyhttpport_context_ptr
    A pointer to the nwy HTTP RPC context.
    
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
BOAT_RESULT NwyHttpPortRequestSync(NwyHttpPortContext * nwyhttpport_context_ptr,
                               const BCHAR *request_str,
                               BUINT32 request_len,
                               BOAT_OUT BCHAR **response_str_ptr,
                               BOAT_OUT BUINT32 *response_len_ptr)
{
    uint8_t channel = 1;
    int result = BOAT_ERROR;
    int port = 0;
    int i = 0; //i is counter, for wait_thread_timeout
    nwy_osiEvent_t event;
    BBOOL thread_ret;
    
    recv_data_len = 0;
    memset(boat_nwy_recv_buff, 0x00, sizeof(boat_nwy_recv_buff));
    
    BoatLog(BOAT_LOG_NORMAL, "NwyHttpPortRequestSync remote_url_str=%s\r\n", nwyhttpport_context_ptr->remote_url_str);
    
    port = boat_get_port_from_url(nwyhttpport_context_ptr->remote_url_str);
    BoatLog(BOAT_LOG_NORMAL, "NwyHttpPortRequestSync port=%d\r\n", port);
    
    g_http_cb_falg = BOAT_TRUE;
    result = nwy_http_setup(channel, nwyhttpport_context_ptr->remote_url_str, port, (void *)nwy_boat_http_result_cb);
    BoatLog(BOAT_LOG_NORMAL, "NwyHttpPortRequestSync nwy_http_setup result=%d\r\n", result);

    i = 0;
    while(1)
    {
        i++;
        memset(&event, 0, sizeof(event));
        thread_ret = nwy_wait_thead_event(boat_get_app_thread(), &event, 1000);
        BoatLog(BOAT_LOG_NORMAL, "RequestSync thread_ret=%d, i=%d\r\n", thread_ret, i);
        if (!g_http_cb_falg || i > 20)
        {
           BoatLog(BOAT_LOG_NORMAL, "RequestSync while g_http_cb_falg==false\r\n");
           break;
        }
    }
   
    g_http_cb_falg = BOAT_TRUE;
    result = nwy_http_post(0, 2, request_str, request_len, BOAT_FALSE);
    BoatLog(BOAT_LOG_NORMAL, "NwyHttpPortRequestSync nwy_http_post result=%d\r\n", result);
    BoatLog(BOAT_LOG_NORMAL, "request_len=%d\r\n", request_len);
    BoatLog(BOAT_LOG_NORMAL, "request_str=%s\r\n", request_str);
    if (-1 == result)
    {
        g_http_cb_falg = BOAT_FALSE;
        nwy_http_close(BOAT_FALSE);
        BoatLog(BOAT_LOG_NORMAL, "RequestSync result -1\r\n");
        return result;
    }
    else
    {
        i = 0;
        while(1)
        {
            i++;
            memset(&event, 0, sizeof(event));
            thread_ret = nwy_wait_thead_event(boat_get_app_thread(), &event, 3*1000);
            BoatLog(BOAT_LOG_NORMAL, " RequestSync thread_ret=%d, i=%d\r\n", thread_ret, i);
            if (!g_http_cb_falg || i > 20)
            {
                BoatLog(BOAT_LOG_NORMAL, "RequestSync while g_http_cb_falg==false\r\n");
                break;
            }
        }
        
        if (recv_data_len > 0 && NULL != boat_nwy_recv_buff[0])
        {
            *response_str_ptr = boat_nwy_recv_buff;
            *response_len_ptr = recv_data_len;
            BoatLog(BOAT_LOG_NORMAL, "RequestSync *response_len_ptr=%d\r\n", *response_len_ptr);
            BoatLog(BOAT_LOG_NORMAL, "RequestSync *response_str_ptr=%s\r\n", *response_str_ptr);
        }
        
        g_http_cb_falg = BOAT_TRUE;
        nwy_http_close(BOAT_FALSE);
        i = 0;
        while(1)
        {
            i++;
            memset(&event, 0, sizeof(event));
            thread_ret = nwy_wait_thead_event(boat_get_app_thread(), &event, 1000);
            BoatLog(BOAT_LOG_NORMAL, "RequestSync thread_ret=%d, event.id=%d, i=%d\r\n", thread_ret, event.id, i);
            if (!g_http_cb_falg || i > 20)
            {
               BoatLog(BOAT_LOG_NORMAL, "RequestSync while g_http_cb_falg==false\r\n");
               break;
            }
        }
        BoatLog(BOAT_LOG_NORMAL, "RequestSync after nwy_http_closed\r\n");
        return BOAT_SUCCESS;
    }
}
 
#endif // end of #if RPC_USE_MBEDHTTPPORT == 1
