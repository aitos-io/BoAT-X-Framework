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

#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include "qapi_types.h"
#include "com_dtypes.h"

#define lower(c) (('A' <= c && c <= 'Z') ? c + ('a' - 'A') : c)
#define LEFT_SHIFT_OP(N)        (1 << (N))

typedef enum
{
    HTTP_SESSION_IDLE,
    HTTP_SESSION_INIT,
    HTTP_SESSION_CONN,
    HTTP_SESSION_DOWNLOADING,
    HTTP_SESSION_DL_FAIL,
    HTTP_SESSION_DL_FIN,
    HTTP_SESSION_DISCONN
} http_session_status_e;

typedef struct
{
    http_session_status_e session_state;

    uint8   data_retry;
    int32     reason_code;

    uint32    start_pos;
    uint32  last_pos;
    uint32    range_size;
} http_session_policy_t;


typedef enum DSS_SIG_EVENTS
{
    DSS_SIG_EVT_INV_E       = LEFT_SHIFT_OP(0),
    DSS_SIG_EVT_NO_CONN_E   = LEFT_SHIFT_OP(1),
    DSS_SIG_EVT_CONN_E      = LEFT_SHIFT_OP(2),
    DSS_SIG_EVT_DIS_E       = LEFT_SHIFT_OP(3),
    DSS_SIG_EVT_EXIT_E      = LEFT_SHIFT_OP(4),
    DSS_SIG_EVT_MAX_E       = LEFT_SHIFT_OP(5)
} DSS_Signal_Evt_e;


typedef enum DSS_Lib_Status
{
    DSS_LIB_STAT_INVALID_E = -1,
    DSS_LIB_STAT_FAIL_E,
    DSS_LIB_STAT_SUCCESS_E,
    DSS_LIB_STAT_MAX_E
} DSS_Lib_Status_e;


typedef enum DSS_Net_Evt_TYPE
{
    DSS_EVT_INVALID_E = 0x00,   /**< Invalid event. */
    DSS_EVT_NET_IS_CONN_E,      /**< Call connected. */
    DSS_EVT_NET_NO_NET_E,       /**< Call disconnected. */
    DSS_EVT_NET_RECONFIGURED_E, /**< Call reconfigured. */
    DSS_EVT_NET_NEWADDR_E,      /**< New address generated. */
    DSS_EVT_NET_DELADDR_E,      /**< Delete generated. */
    DSS_EVT_NIPD_DL_DATA_E,
    DSS_EVT_MAX_E
} DSS_Net_Evt_Type_e;

#define HTTP_DEFAULT_PORT           80

#define HTTPS_DEFAULT_PORT          443
#define HTTP_DOMAIN_NAME_LENGTH     150
#define HTTP_URL_LENGTH             700
#define HTTP_MAX_PATH_LENGTH        100
#define HTTP_READ_BYTES             1024


#define HTPP_MAX_CONNECT_RETRY (6)

#define FIBOCOMHTTPPORT_RECV_BUF_SIZE (4096)

boolean   http_decode_url(uint8 *url,uint32 *url_length,char *host,uint16 *port,uint8 *file_name);


int http_request_send(char* host, uint16 port, uint8* file_name,const char *request_data,uint32 request_data_len, StringWithLen *http_resp);

void http_release(void);

int http_netctrl_start(void);

void http_netctrl_stop(void);
/**
* @}
*/

/* The following content is used in the HttpClient module. */
#ifndef MTK_DEBUG_LEVEL_NONE
#define HTTPCLIENT_DEBUG 1
#else
#define HTTPCLIENT_DEBUG 0
#endif

#ifdef BOAT_HTTPCLIENT_SSL_ENABLE

#endif

#ifdef __cplusplus
}
#endif

#endif /* __HTTPCLIENT_H__ */


