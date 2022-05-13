/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stdarg.h"
#include "boattypes.h"
#include "boatutility.h"
#include "boatlog.h"


#include "http_client.h"
#include "qapi_diag.h"
#include "qapi_socket.h"
#include "qapi_dnsc.h"
#include "qapi_dss.h"
#include "qapi_timer.h"
#include "qapi_uart.h"
#include "qapi_fs.h"
#include "qapi_status.h"
#include "qapi_netservices.h"
#include "qapi_httpc.h"
#include "qapi_device_info.h"


http_session_policy_t http_session_policy;
static qapi_Net_HTTPc_handle_t  http_hadle;

extern TX_BYTE_POOL *byte_pool_boat;

TX_SEMAPHORE *g_tx_semaphore_returndata_ptr;

qapi_DSS_Hndl_t http_dss_handle = NULL;  
DSS_Lib_Status_e http_netctl_lib_status = DSS_LIB_STAT_INVALID_E;
TX_EVENT_FLAGS_GROUP *http_signal_handle;
TX_EVENT_FLAGS_GROUP *http_release_handle;

uint8 http_datacall_status = DSS_EVT_INVALID_E;

#define FIBO_APN_LEN            (QAPI_DSS_CALL_INFO_APN_MAX_LEN + 1)
#define FIBO_APN_USER_LEN       (QAPI_DSS_CALL_INFO_USERNAME_MAX_LEN + 1)
#define FIBO_APN_PASS_LEN       (QAPI_DSS_CALL_INFO_PASSWORD_MAX_LEN + 1)
#define FIBO_DEV_NAME_LEN       (QAPI_DSS_CALL_INFO_DEVICE_NAME_MAX_LEN + 1)

#define FIBO_DEF_APN          "CMNET"
static char apn[FIBO_APN_LEN];                  /* APN */
static char apn_username[FIBO_APN_USER_LEN];    /* APN username */
static char apn_passwd[FIBO_APN_PASS_LEN];      /* APN password */

#define DSS_ADDR_INFO_SIZE  5
#define DSS_ADDR_SIZE       16

#define GET_ADDR_INFO_MIN(a, b) ((a) > (b) ? (b) : (a))

#define DBG(x, arg...)  odm_ght_printf(debug_uart_context_D.uart_handle,"[DBG_http:]"x,##arg)
#define WARN(x, arg...) odm_ght_printf(debug_uart_context_D.uart_handle,"[WARN_http:]"x,##arg)
#define ERR(x, arg...)  odm_ght_printf(debug_uart_context_D.uart_handle,"[ERR_http:]"x,##arg)

int fibo_atoi(const char *str)
{
    int value = 0;
    int flag = 1;

    while (*str == ' ')
    {
        str++;
    }

    if (*str == '-')
    {
        flag = 0;
        str++;
    }
    else if (*str == '+')
    {
        flag = 1;
        str++;
    }
    else if (*str >= '9' || *str <= '0')
    {
        return 0;
    }

    while (*str != '\0' && *str <= '9' && *str >= '0')
    {
        value = value * 10 + *str - '0';
        str++;
    }

    if (flag == 0)
    {
        value = -value;
    }

    return value;
}



int32 http_strlcpy
(
char *dst,
const char *src,
int32  siz
)
{
    char  *d = dst;
    const char *s = src;
    int32 n = siz;

    /* Copy as many bytes as will fit */
    if (n != 0)
    {
        while (--n != 0)
        {
            if ((*d++ = *s++) == '\0')
            break;
        }
    }

    /* Not enough room in dst, add NUL and traverse rest of src */
    if (n == 0)
    {
        if (siz != 0)
        *d = '\0';  /* NUL-terminate dst */
        while (*s++)
        {
            ;
        }
    }

    return(s - src - 1);    /* count does not include NUL */
}



boolean   http_decode_url
(
uint8            *url,
uint32           *url_length,
char             *host,
uint16           *port,
uint8            *file_name
)
{
    uint8      hstr[9];
    uint32    i;
    uint8    *phostnamehead;
    uint8    *phostnameTail;
    boolean      ret = FALSE;
    uint8     url_type;
    uint8   *puri= url;
    uint32   datalen = *url_length;
    *port = HTTP_DEFAULT_PORT;
    do
    {
        memset(hstr,0,9);

        if((*url_length) < 8)
        {
            break;
        }

        memcpy(hstr,url,8);

        for(i=0;i<8;i++)
        {
            hstr[i] = lower((int32)hstr[i]);
        }
        if(strncmp((char *)hstr,(char*)"https://",8) == 0)
        {
            puri = url + 8;
            datalen -= 8;
            url_type = 1;
#ifdef FIBOCOM_HTTPS_SUPPORT
            DBG("http: get http head\n");
            ssl_ctx_id = 1;
#endif
        }
        else if(strncmp((char *)hstr,(char*)"http://",7) == 0)
        {
            puri = url+7;
            datalen -= 7;
            url_type = 0;
#ifdef FIBOCOM_HTTPS_SUPPORT
            ssl_ctx_id = 0;
#endif

        }
        else
        {
            break;
        }

        if(url_type == 1)
        {
            DBG("http: get port 443\n");
            *port = HTTPS_DEFAULT_PORT;
        }
        else
        {
             *port = HTTP_DEFAULT_PORT;
        }
        i=0;

        if(puri[i] != '[') //chris: normal url  http://220.180.239.201:80/index.html  or http://www.baidu.com
        {
            /* parse host name */
            phostnamehead = puri;
            phostnameTail = puri;

            while(i<datalen && puri[i] != '/' && puri[i] != ':')
            {
                i++;
                phostnameTail++;
            }
        }
        else //ipv6 url:http://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]:80/index.html  or http://[1080:0:0:0:8:800:200C:417A]/index.html
        {
            datalen -= 1;
            puri += 1;

            phostnamehead = puri;
            phostnameTail = puri;
            while(i < datalen && puri[i] != ']')
            {
                i++;
                phostnameTail++;
            }
            i++;
        }
        hstr[0] = *phostnameTail;
        *phostnameTail = '\0';
        if(strlen((char*)phostnamehead)  == 0 || strlen((char*)phostnamehead) >= HTTP_DOMAIN_NAME_LENGTH)
        {
             //HTTP_UART_DBG("%s", __FUNCTION__);
             break;
        }
        http_strlcpy(host,  (char*)phostnamehead, (strlen((char*)phostnamehead)+1));

        *phostnameTail = hstr[0];

        /* IP address URL without port */
        if(datalen >= i)
            datalen -= i;
        else
            datalen = 0;
        if(datalen == 0)
        {
            url[0] = '/';
            *url_length = 1;
            ret = TRUE;
            break;
        }

        /* get port */
        puri+=i;
        i = 0;

        if(*puri==':')
        {
            datalen -= 1;
            puri++;
            phostnamehead = puri;
            phostnameTail = puri;

            while(*puri !='/' && i<datalen ){
                puri++;
                i++;
            }

            phostnameTail = puri;
            hstr[0] = *phostnameTail;
            *phostnameTail = '\0';

            *port = fibo_atoi((char *)phostnamehead);
            *phostnameTail = hstr[0];
        }

        if(datalen >= i)
            datalen -= i;
        else
            datalen = 0;
        if(datalen == 0)
        {
             url[0] = '/';
            *url_length = 1;
            ret = TRUE;
            break;
        }

        /* get resouce URI */
        if(*puri=='/')
        {
            i=0;
            while(i<datalen)
            {
                url[i]=puri[i];
                i++;
            }
            *url_length= datalen;
            *(url+datalen)='\0';
            http_strlcpy((char*)file_name, (char*)url, strlen((char*)url)+1);
        }
        else
        {
            url[0] = '/';
            *url_length= 1;
        }
        ret = TRUE;

    }while(FALSE);
    if(ret == FALSE)
    {
        url[0]= '\0';
        *url_length = 0;
        memset((void *)host, 0, HTTP_DOMAIN_NAME_LENGTH);
    }

    return ret;

}

//The http-session requests a callback
void http_client_cb_t(void* arg, BSINT32 state, void* http_resp)
{
	StringWithLen *http_resp_data = arg;
    qapi_Net_HTTPc_Response_t * response = (qapi_Net_HTTPc_Response_t *)http_resp;

	char *rsp_len_str=NULL;
	char *rsp_offset_str=NULL;
	char *rsp_len_start=NULL;

	int rsp_offset = 0;
	static int rsp_data_len = 0;
	int i=0;
	//Response buffer cache
	static BCHAR http_response_string[FIBOCOMHTTPPORT_RECV_BUF_SIZE];
	static BUINT32 http_response_length=0;

	BoatLog(BOAT_LOG_NORMAL, "response->length: %d",response->length);
	BoatLog(BOAT_LOG_NORMAL, "response->data: %s",response->data);

    if( response->resp_Code >= 200 && response->resp_Code < 300)
    {	
		if (response->rsp_hdr_len != 0 && response->rsp_hdr != NULL)
		{
			rsp_len_str = strstr(response->rsp_hdr,"Content-Length:");
			rsp_offset_str = rsp_len_str+strlen("Content-Length:");
			do
			{
				i++;
				if ( *(rsp_offset_str+i) >= '0' && *(rsp_offset_str+i) <= '9')
				{
					rsp_offset++;
					if ( rsp_offset == 1)
					{
						rsp_len_start = (rsp_offset_str+i);
					}
			
				}
			} while ( *(rsp_offset_str+i) != 0x0d);
			BoatLog(BOAT_LOG_VERBOSE, "rsp_offset: %d",rsp_offset);

			for ( i = 0; i < rsp_offset; i++)
			{
				rsp_data_len += (rsp_len_start[i] - 0x30) * (BUINT32)pow(10,(rsp_offset-i-1));
				
			}
			BoatLog(BOAT_LOG_VERBOSE, "rsp_data_len: %d",rsp_data_len);
		}

        if (  response->data != NULL && state >= 0  )
        {
			//Buffer cache
			memcpy(http_response_string+http_response_length,response->data,response->length);
			http_response_length += response->length;
			
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "Http callback function is error State: %d", state);
        }       
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL," Http callback function is error Response->resp_Code: %d", response->resp_Code);
    }

	if (http_response_length >= rsp_data_len )
	{
		memset(http_resp_data->string_ptr,0,response->length);
		//Read the data in the cache
		BoatLog(BOAT_LOG_VERBOSE, " http_response_string: %s", http_response_string);
		memcpy(http_resp_data->string_ptr,http_response_string,http_response_length);			
		BoatLog(BOAT_LOG_VERBOSE, " http_response_length: %d",http_response_length);			
        http_resp_data->string_len = http_response_length;
		rsp_data_len = 0;

		/* actively free the data buffer after processing relevant data*/
		tx_byte_release(response->data);
		tx_byte_release(response->rsp_hdr);
		tx_byte_release(http_resp);
		response = NULL;

		memset(http_response_string,0,http_response_length);
		http_response_length = 0;
	
		tx_semaphore_put(g_tx_semaphore_returndata_ptr);


	}
	
	return;
}



int http_request_send(char* host, uint16 port, uint8* file_name,const char *request_data,uint32 request_data_len, StringWithLen *http_resp)
{
	qapi_Status_t ret = -1;
	uint8 reconnect_count = 0;
	char range_buffer[32] = {0};
#ifdef FIBOCOM_HTTPS_SUPPORT
	SSL_INST http_ssl;
#endif


		/* code */
		/* Start HTTPc service */
		qapi_Net_HTTPc_Start();
		memset(&http_session_policy, 0, sizeof(http_session_policy_t));
#ifdef FIBOCOM_HTTPS_SUPPORT
		if (ssl_ctx_id)
		{
			/* config object and certificate */
			if (0 != http_ssl_config(&http_ssl))
			{
				BoatLog(BOAT_LOG_CRITICAL,"ERROR: Config ssl object and certificates error\n");
			}
		/* @Justice_20190413 qapi_Net_HTTPc_Connect" will internally take care of creating SSL connection 
		* corresponding to HTTP connection. And creating SSL connection using "qapi_Net_SSL_Con_New" which 
		* is not necessary. (refer to TX3.0.1 P8101-35 Doc) */

			http_hadle = qapi_Net_HTTPc_New_sess(20000, http_ssl.sslCtx, http_client_cb_t,http_resp, 2048, 1024);
			ret = qapi_Net_HTTPc_Configure_SSL(http_hadle, &http_ssl.config);
			if(ret != QAPI_OK)
			{
				BoatLog(BOAT_LOG_CRITICAL,"Start HTTPS connection, SSL config ret[%d]\n", ret);
				return -2;
			}
		}
		else
		{
			http_hadle = qapi_Net_HTTPc_New_sess(20000, 0, http_client_cb_t, http_resp, 2048, 1024);
		}
#else
		http_hadle = qapi_Net_HTTPc_New_sess(20000, 0, http_client_cb_t,http_resp, 2048, 1024);
#endif /* FIBOCOM_HTTPS_SUPPORT */


	http_session_policy.session_state = HTTP_SESSION_INIT;

	if ( http_hadle == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL,"qapi_Net_HTTPc_New_sess ERROR\n");
		return -3;
	}
	qapi_Net_HTTPc_Pass_Pool_Ptr(http_hadle, byte_pool_boat);

	do
	{
		//qapi_Status_t qapi_Net_HTTPc_Configure(qapi_Net_HTTPc_handle_t handle, qapi_Net_HTTPc_Config_t *httpc_Cfg);

		memset(range_buffer, 0, sizeof(range_buffer));
			
		ret = qapi_Net_HTTPc_Connect(http_hadle, host, port);
		if (ret != QAPI_OK)
		{
			reconnect_count++;
			BoatLog(BOAT_LOG_CRITICAL,"qapi_Net_HTTPc_Connect ERROR ret:%d, re-connect times:%d\n",ret,reconnect_count);
			if(reconnect_count >= HTPP_MAX_CONNECT_RETRY)
			{
				return -4;
			}
			qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
		}
		else
		{
			BoatLog(BOAT_LOG_CRITICAL,"qapi_Net_HTTPc_Connect success :%d\n", ret);
			break;
		}
	} while (1);
	
	ret = qapi_Net_HTTPc_Add_Header_Field(http_hadle, "Content-Type", "application/json");
	if (ret != QAPI_OK)
	{
		BoatLog(BOAT_LOG_CRITICAL,"qapi_Net_HTTPc_Add_Header_Field content-type ERROR :%d\n", ret);
        return -2;
	}

	http_session_policy.session_state = HTTP_SESSION_CONN;
	
	ret = qapi_Net_HTTPc_Set_Body(http_hadle, request_data, request_data_len);
	if (ret != QAPI_OK)
	{
		BoatLog(BOAT_LOG_CRITICAL,"qapi_Net_HTTPc_Set_Body ERROR :%d\n", ret);
        return -2;
	}
	/* get response content */
	ret = qapi_Net_HTTPc_Request(http_hadle, QAPI_NET_HTTP_CLIENT_POST_E, (char*)file_name); 
	
	if (ret != QAPI_OK)
	{
		BoatLog(BOAT_LOG_CRITICAL,"qapi_Net_HTTPc_Request ERROR :%d\n",ret);
		return -5;
	}
	
	return 0;
}

static void http_session_release(void)
{

    /* stop http client first */
    qapi_Net_HTTPc_Stop();

    if (NULL != http_hadle)
    {
        qapi_Net_HTTPc_Disconnect(http_hadle);
        qapi_Net_HTTPc_Free_sess(http_hadle);
    }
    #ifdef FIBOCOM_HTTPS_SUPPORT
    if (0 != ssl_obj_hdl)
    {
        qapi_Net_SSL_Obj_Free(ssl_obj_hdl);
    }
    #endif  /* FIBOCOM_HTTPS_SUPPORT */
}

void http_release(void)
{
	http_session_release();
	memset(&http_session_policy, 0, sizeof(http_session_policy_t));
}
 

/*
@func
    http_inet_ntoa
@brief
    utility interface to translate ip from "int" to x.x.x.x format.
*/
int32 http_inet_ntoa
(
    const qapi_DSS_Addr_t    inaddr, /* IPv4 address to be converted         */
    uint8                   *buf,    /* Buffer to hold the converted address */
    int32                    buflen  /* Length of buffer                     */
)
{
    uint8 *paddr  = (uint8 *)&inaddr.addr.v4;
    int32  rc = 0;

    if ((NULL == buf) || (0 >= buflen))
    {
        rc = -1;
    }
    else
    {
        if (-1 == snprintf((char*)buf, (unsigned int)buflen, "%d.%d.%d.%d",
                            paddr[0],
                            paddr[1],
                            paddr[2],
                            paddr[3]))
        {
            rc = -1;
        }
    }

    return rc;
} 

void http_show_sysinfo(void)
{
    int i   = 0;
    int j   = 0;
    unsigned int len = 0;
    uint8 buff[DSS_ADDR_SIZE] = {0};
    qapi_Status_t status;
    qapi_DSS_Addr_Info_t info_ptr[DSS_ADDR_INFO_SIZE];

    status = qapi_DSS_Get_IP_Addr_Count(http_dss_handle, &len);
    if (QAPI_ERROR == status)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Get IP address count error\n");
        return;
    }

    status = qapi_DSS_Get_IP_Addr(http_dss_handle, info_ptr, len);
    if (QAPI_ERROR == status)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Get IP address error\n");
        return;
    }

    j = GET_ADDR_INFO_MIN(len, DSS_ADDR_INFO_SIZE);

    for (i = 0; i < j; i++)
    {
        BoatLog(BOAT_LOG_CRITICAL,"<--- static IP address information --->\n");
        http_inet_ntoa(info_ptr[i].iface_addr_s, buff, DSS_ADDR_SIZE);
        BoatLog(BOAT_LOG_CRITICAL,"static IP: %s\n", buff);

        memset(buff, 0, sizeof(buff));
        http_inet_ntoa(info_ptr[i].gtwy_addr_s, buff, DSS_ADDR_SIZE);
        BoatLog(BOAT_LOG_CRITICAL,"Gateway IP: %s\n", buff);

        memset(buff, 0, sizeof(buff));
        http_inet_ntoa(info_ptr[i].dnsp_addr_s, buff, DSS_ADDR_SIZE);
        BoatLog(BOAT_LOG_CRITICAL,"Primary DNS IP: %s\n", buff);

        memset(buff, 0, sizeof(buff));
        http_inet_ntoa(info_ptr[i].dnss_addr_s, buff, DSS_ADDR_SIZE);
        BoatLog(BOAT_LOG_CRITICAL,"Second DNS IP: %s\n", buff);
    }

    BoatLog(BOAT_LOG_CRITICAL,"<--- End of system info --->\n");
}


/*
@func
    dss_net_event_cb
@brief
    Initializes the DSS netctrl library for the specified operating mode.
*/
static void http_net_event_cb
(
    qapi_DSS_Hndl_t         hndl,
    void                   *user_data,
    qapi_DSS_Net_Evt_t      evt,
    qapi_DSS_Evt_Payload_t *payload_ptr
)
{

    BoatLog(BOAT_LOG_CRITICAL,"Data test event callback, event: %d\n", evt);

    switch (evt)
    {
        case QAPI_DSS_EVT_NET_NEWADDR_E:
        case QAPI_DSS_EVT_NET_IS_CONN_E:
        {
            BoatLog(BOAT_LOG_CRITICAL,"Data Call Connected.\n");
            http_show_sysinfo();
            /* Signal main task */
            tx_event_flags_set(http_signal_handle, DSS_SIG_EVT_CONN_E, TX_OR);
            http_datacall_status = DSS_EVT_NET_IS_CONN_E;

            break;
        }
        case QAPI_DSS_EVT_NET_DELADDR_E: 
        case QAPI_DSS_EVT_NET_NO_NET_E:
        {
            BoatLog(BOAT_LOG_CRITICAL,"Data Call Disconnected.\n");

            if (DSS_EVT_NET_IS_CONN_E == http_datacall_status)
            {
                http_datacall_status = DSS_EVT_NET_NO_NET_E;
                tx_event_flags_set(http_signal_handle, DSS_SIG_EVT_EXIT_E, TX_OR);
            }
            else
            {
                tx_event_flags_set(http_signal_handle, DSS_SIG_EVT_NO_CONN_E, TX_OR);
            }
            break;
        }

        default:
        {
            BoatLog(BOAT_LOG_CRITICAL,"Data Call status is invalid.\n");
            tx_event_flags_set(http_signal_handle, DSS_SIG_EVT_INV_E, TX_OR);
            http_datacall_status = DSS_EVT_INVALID_E;
            break;
        }
    }
}


/*
@func
    http_netctrl_init
@brief
    Initializes the DSS netctrl library for the specified operating mode.
*/
static int http_netctrl_init(void)
{
    int ret_val = 0;
    qapi_Status_t status = QAPI_OK;

    BoatLog(BOAT_LOG_CRITICAL,"Initializes the DSS netctrl library\n");

    /* Initializes the DSS netctrl library */
    if (QAPI_OK == qapi_DSS_Init(QAPI_DSS_MODE_GENERAL))
    {
        http_netctl_lib_status = DSS_LIB_STAT_SUCCESS_E;
        BoatLog(BOAT_LOG_CRITICAL,"qapi_DSS_Init success\n");
    }
    else
    {
        /* @Note: netctrl library has been initialized */
        http_netctl_lib_status = DSS_LIB_STAT_FAIL_E;
        BoatLog(BOAT_LOG_CRITICAL,"DSS netctrl library has been initialized.\n");
    }

    /* Registering callback http_dss_handleR */
    do
    {
        BoatLog(BOAT_LOG_CRITICAL,"Registering Callback http_dss_handle\n");

        /* Obtain data service handle */
        status = qapi_DSS_Get_Data_Srvc_Hndl(http_net_event_cb, NULL, &http_dss_handle);
        BoatLog(BOAT_LOG_CRITICAL,"http_dss_handle %d, status %d\n", http_dss_handle, status);

        if (NULL != http_dss_handle)
        {
        BoatLog(BOAT_LOG_CRITICAL,"Registed http_dss_handler success\n");
        break;
        }

        /* Obtain data service handle failure, try again after 10ms */
        qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_MSEC, true);
    } while(1);

    return ret_val;
}

/*
@func
    http_set_data_param
@brief
    Set the Parameter for Data Call, such as APN and network tech.
*/
static int http_set_data_param(void)
{
    qapi_DSS_Call_Param_Value_t param_info;

    /* Initial Data Call Parameter */
    memset(apn, 0, sizeof(apn));
    memset(apn_username, 0, sizeof(apn_username));
    memset(apn_passwd, 0, sizeof(apn_passwd));
    strlcpy(apn, FIBO_DEF_APN, QAPI_DSS_CALL_INFO_APN_MAX_LEN);

    if (NULL != http_dss_handle)
    {
        /* set data call param */
        param_info.buf_val = NULL;
        param_info.num_val = QAPI_DSS_RADIO_TECH_UNKNOWN;   //Automatic mode(or DSS_RADIO_TECH_LTE)
        BoatLog(BOAT_LOG_CRITICAL,"Setting tech to Automatic\n");
        qapi_DSS_Set_Data_Call_Param(http_dss_handle, QAPI_DSS_CALL_INFO_TECH_PREF_E, &param_info);

        /* set apn */
        param_info.buf_val = apn;
        param_info.num_val = strlen(apn);
        BoatLog(BOAT_LOG_CRITICAL,"Setting APN - %s\n", apn);
        qapi_DSS_Set_Data_Call_Param(http_dss_handle, QAPI_DSS_CALL_INFO_APN_NAME_E, &param_info);
#ifdef FIBO_CUSTOM_APN
        /* set apn username */
        param_info.buf_val = apn_username;
        param_info.num_val = strlen(apn_username);
        odm_ght_printf(debug_uart_context_D.uart_handle,"Setting APN USER - %s\n", apn_username);
        qapi_DSS_Set_Data_Call_Param(http_dss_handle, QAPI_DSS_CALL_INFO_USERNAME_E, &param_info);

        /* set apn password */
        param_info.buf_val = apn_passwd;
        param_info.num_val = strlen(apn_passwd);
        odm_ght_printf(debug_uart_context_D.uart_handle,"Setting APN PASSWORD - %s\n", apn_passwd);
        qapi_DSS_Set_Data_Call_Param(http_dss_handle, QAPI_DSS_CALL_INFO_PASSWORD_E, &param_info);

        /* set auth type */
        param_info.buf_val = NULL;
        param_info.num_val = QAPI_DSS_AUTH_PREF_PAP_CHAP_BOTH_ALLOWED_E; //PAP and CHAP auth
        odm_ght_printf(debug_uart_context_D.uart_handle,"Setting AUTH - %d\n", param_info.num_val);
        qapi_DSS_Set_Data_Call_Param(handle, QAPI_DSS_CALL_INFO_AUTH_PREF_E, &param_info);
#endif
        /* set IP version(IPv4 or IPv6) */
        param_info.buf_val = NULL;
        param_info.num_val = QAPI_DSS_IP_VERSION_4;
        BoatLog(BOAT_LOG_CRITICAL,"Setting family to IPv%d\n", param_info.num_val);
        qapi_DSS_Set_Data_Call_Param(http_dss_handle, QAPI_DSS_CALL_INFO_IP_VERSION_E, &param_info);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL,"Dss handler is NULL!!!\n");
        return -1;
    }

    return 0;
}
 /*
@func
    http_netctrl_start
@brief
    Start the DSS netctrl library, and startup data call.
*/
int http_netctrl_start(void)
{
    int rc = 0;
    qapi_Status_t status = QAPI_OK;

    rc = http_netctrl_init();
    if (0 == rc)
    {
        /* Get valid DSS handler and set the data call parameter */
        http_set_data_param();
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL,"_dss_init fail.\n");
        return -1;
    }

    BoatLog(BOAT_LOG_CRITICAL,"qapi_DSS_Start_Data_Call start!!!.\n");
    status = qapi_DSS_Start_Data_Call(http_dss_handle);
    if (QAPI_OK == status)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Start Data service success.\n");
        return 0;
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL,"Start Data service failed.\n");
        return -1;
    }
}


/*
@func
    http_netctrl_release
@brief
    Cleans up the DSS netctrl library and close data service.
*/
void http_netctrl_stop(void)
{
    qapi_Status_t stat = QAPI_OK;

    if (http_dss_handle)
    {
        stat = qapi_DSS_Stop_Data_Call(http_dss_handle);
        if (QAPI_OK == stat)
        {
            BoatLog(BOAT_LOG_CRITICAL,"Stop data call success\n");
        }
        stat = qapi_DSS_Rel_Data_Srvc_Hndl(http_dss_handle);
        if (QAPI_OK != stat)
        {
            BoatLog(BOAT_LOG_CRITICAL,"Release data service handle failed:%d\n", stat);
        }
        http_dss_handle = NULL;
    }
    if (DSS_LIB_STAT_SUCCESS_E == http_netctl_lib_status)
    {
        qapi_DSS_Release(QAPI_DSS_MODE_GENERAL);
        http_netctl_lib_status = DSS_LIB_STAT_INVALID_E;
    }

}