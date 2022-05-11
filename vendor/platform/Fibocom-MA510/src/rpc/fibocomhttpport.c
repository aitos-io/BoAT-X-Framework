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

/*!@brief fibocom HTTP porting for RPC

@file
fibocomhttpport.c is the AT command porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use fibocom HTTP RPC porting, RPC_USE_FIBOCOMHTTPPORT in boatoptions.h must set to 1.
*/

#include "boatinternal.h"

#if RPC_USE_FIBOCOMHTTPPORT == 1

//#include "rpcport.h"
#include "fibocomhttpport.h"

#include "http_client.h"

//#include "qapi_netservices.h"
#include "qapi_httpc.h"
#include "qapi_timer.h"
#include "com_dtypes.h"
#include "qapi_socket.h"
#include "qapi_dnsc.h"

extern TX_BYTE_POOL *byte_pool_boat;
extern http_session_policy_t http_session_policy;          /* http session policy */

extern TX_SEMAPHORE *g_tx_semaphore_returndata_ptr;

extern TX_EVENT_FLAGS_GROUP *http_signal_handle;

extern TX_EVENT_FLAGS_GROUP *http_release_handle;
/*!*****************************************************************************
@brief Initialize fibocom HTTP RPC context.

Function: FibocomHttpPortInit()

    This function initializes fibocom HTTP RPC context. It also dynamically allocates
    storage to receive response from the peer.
    
@see FibocomHttpPortDeinit()

@return
    This function returns a pointer to fibocom's HTTP RPC context.\n
    It returns NULL if initialization fails.
    

@param This function doesn't take any argument.

*******************************************************************************/
FibocomHttpPortContext *FibocomHttpPortInit(void)
{
    FibocomHttpPortContext *fibocomhttpport_context_ptr;
    BOAT_RESULT result = BOAT_SUCCESS;

    fibocomhttpport_context_ptr = BoatMalloc(sizeof(FibocomHttpPortContext));
    if (fibocomhttpport_context_ptr != NULL)
    {
        result = BOAT_SUCCESS;
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate fibocom HTTP RPC Context.");
        result = BOAT_ERROR;
    }
    
    if (result == BOAT_SUCCESS)
    {
        fibocomhttpport_context_ptr->http_response.string_ptr = BoatMalloc(FIBOCOMHTTPPORT_RECV_BUF_SIZE);

        if (fibocomhttpport_context_ptr->http_response.string_ptr != NULL)
        {
            fibocomhttpport_context_ptr->http_response.string_space = FIBOCOMHTTPPORT_RECV_BUF_SIZE;
            fibocomhttpport_context_ptr->http_response.string_len = 0;
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate mbed HTTP RESPONSE buffer.");
            result = BOAT_ERROR;
        }
    }
    
    if (result != BOAT_SUCCESS)
    {
        if (fibocomhttpport_context_ptr != NULL)
        {
            if (fibocomhttpport_context_ptr->http_response.string_ptr != NULL)
            {
                BoatFree(fibocomhttpport_context_ptr->http_response.string_ptr);
                fibocomhttpport_context_ptr->http_response.string_ptr = NULL;
                fibocomhttpport_context_ptr->http_response.string_space = 0;
                fibocomhttpport_context_ptr->http_response.string_len = 0;
            }

            BoatFree(fibocomhttpport_context_ptr);
            fibocomhttpport_context_ptr = NULL;
        }
    }

    	/* Create event signal handle and clear signals */
    	uint32 dss_event = 0;
	txm_module_object_allocate(&http_signal_handle, sizeof(TX_EVENT_FLAGS_GROUP));
	tx_event_flags_create(http_signal_handle, "dss_signal_event");
	tx_event_flags_set(http_signal_handle, 0x0, TX_AND);

    	txm_module_object_allocate(&http_release_handle, sizeof(TX_EVENT_FLAGS_GROUP));
    	tx_event_flags_create(http_release_handle, "http_release_event");
    	tx_event_flags_set(http_release_handle, 0x0, TX_AND);

	int ret = http_netctrl_start();
	if (ret != 0)
	{
		BoatLog(BOAT_LOG_VERBOSE,"http_netctrl_start,result: %d",ret);
	}
    
	tx_event_flags_get(http_signal_handle, DSS_SIG_EVT_CONN_E|DSS_SIG_EVT_DIS_E|DSS_SIG_EVT_EXIT_E|DSS_SIG_EVT_NO_CONN_E, TX_OR_CLEAR, &dss_event, TX_WAIT_FOREVER);
	tx_event_flags_delete(http_signal_handle);
	if(dss_event&DSS_SIG_EVT_CONN_E)
	{
		BoatLog(BOAT_LOG_VERBOSE,"dss_event:%x",dss_event);
        /*
		if(qapi_Net_DNSc_Is_Started() == 0)
		{
			qapi_Net_DNSc_Command(QAPI_NET_DNS_START_E);
		}
        */
	}
	else if(dss_event&DSS_SIG_EVT_EXIT_E)
	{
		BoatLog(BOAT_LOG_VERBOSE,"dss_event: %x",dss_event);
        	http_netctrl_stop();
        	tx_event_flags_delete(http_release_handle);
       		// return NULL;
	}
	else
	{
        	tx_event_flags_delete(http_release_handle);
		BoatLog(BOAT_LOG_VERBOSE,"ret: %d",ret);
       		// return NULL;
	}

	ret = txm_module_object_allocate(&g_tx_semaphore_returndata_ptr, sizeof(TX_SEMAPHORE));
    	if(ret != TX_SUCCESS)
	{
		BoatLog(BOAT_LOG_VERBOSE,"txm_module_object_allocate [boat_malloc_buff] failed,ret = %d", ret);
	}
	ret = tx_semaphore_create(g_tx_semaphore_returndata_ptr,"callbacksemaphore",0);
	if(ret != TX_SUCCESS)
	{
		BoatLog(BOAT_LOG_VERBOSE,"tx_semaphore_create [boat_malloc_buff] failed,ret = %d", ret);
	}

    return fibocomhttpport_context_ptr;

}


/*!*****************************************************************************
@brief Deinitialize fibocom HTTP RPC context.

Function: MbedHttpPortDeinit()

    This function de-initializes mbed HTTP RPC context. It also frees the dynamically
    allocated storage to receive response from the peer.

@see MbedHttpPortInit()    

@return
    This function doesn't return any value.
    

@param[in] mbedhttpport_context_ptr
    Pointer to the context of MbedHttpPortContext, which is returned by MbedHttpPortInit()

*******************************************************************************/
void FibocomHttpPortDeinit(FibocomHttpPortContext *fibocomhttpport_context_ptr)
{
    if (fibocomhttpport_context_ptr == NULL)
    {
        return;
    }
    
    fibocomhttpport_context_ptr->http_response.string_space = 0;
    fibocomhttpport_context_ptr->http_response.string_len = 0;

    if (fibocomhttpport_context_ptr->http_response.string_ptr != NULL)
    {
        BoatFree(fibocomhttpport_context_ptr->http_response.string_ptr);
        fibocomhttpport_context_ptr->http_response.string_ptr = NULL;
    }

    tx_semaphore_delete(g_tx_semaphore_returndata_ptr);
	txm_module_object_deallocate(g_tx_semaphore_returndata_ptr);

	tx_event_flags_delete(http_signal_handle);

	txm_module_object_deallocate(&http_signal_handle);

    tx_event_flags_delete(http_release_handle);

	txm_module_object_deallocate(&http_release_handle);

	http_netctrl_stop();

    BoatFree(fibocomhttpport_context_ptr);

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
BOAT_RESULT FibocomHttpPortSetOpt(FibocomHttpPortContext *fibocomhttpport_context_ptr, BCHAR *remote_url_str)
{
    if (fibocomhttpport_context_ptr == NULL || remote_url_str == NULL)
    {
        return BOAT_ERROR;
    }

    fibocomhttpport_context_ptr->remote_url_str = remote_url_str;

    return BOAT_SUCCESS;
}



/*!*****************************************************************************
@brief Perform a synchronous HTTP POST and wait for its response.

Function: MbedHttpPortRequestSync()

    This function performs a synchronous mbed HTTP POST and waits for its response.

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
BOAT_RESULT FibocomHttpPortRequestSync(FibocomHttpPortContext *fibocomhttpport_context_ptr,
                                    const BCHAR *request_str,
                                    BUINT32 request_len,
                                    BOAT_OUT BCHAR **response_str_ptr,
                                    BOAT_OUT BUINT32 *response_len_ptr)
{

	BOAT_RESULT result;
	uint8 file_name[HTTP_MAX_PATH_LENGTH+1] = {0};
	char host[HTTP_DOMAIN_NAME_LENGTH+1] = {0};
	char buf[128];
	uint32 len = strlen(fibocomhttpport_context_ptr->remote_url_str);
	uint16 port;

    strcpy(buf,fibocomhttpport_context_ptr->remote_url_str);
	http_decode_url((uint8*)buf, &len, host, &port, file_name);
	BoatLog(BOAT_LOG_VERBOSE,"host:%s,port:%d,source:%s", host, port, file_name);

    result = http_request_send(host, port, file_name,request_str,request_len,&fibocomhttpport_context_ptr->http_response);
	if(result < 0)
	{
		http_release();
		BoatLog(BOAT_LOG_VERBOSE,"dss_event,result: %d",result);
		return result;
	}

	result = tx_semaphore_get(g_tx_semaphore_returndata_ptr, TX_WAIT_FOREVER);
	if(result != TX_SUCCESS)
	{
		BoatLog(BOAT_LOG_VERBOSE,"tx_semaphore_get failed,result = %d", result);

	}
	else
    {
        *response_str_ptr = fibocomhttpport_context_ptr->http_response.string_ptr;
		*response_len_ptr = fibocomhttpport_context_ptr->http_response.string_len;
		    
		BoatLog(BOAT_LOG_VERBOSE, "Result fibocomhttpport_context_ptr->http_response.string_len: %d", fibocomhttpport_context_ptr->http_response.string_len);
		BoatLog(BOAT_LOG_VERBOSE, "Response *response_str_ptr: %s", *response_str_ptr);
   	 
        result = BOAT_SUCCESS;
    }
	http_release();
	
	BoatLog(BOAT_LOG_VERBOSE,"http_release success");
	return result;

}


#endif // end of #if RPC_USE_FIBOCOMHTTPPORT == 1
