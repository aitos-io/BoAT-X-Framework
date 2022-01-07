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

#if 0

#include <stdio.h>
#include <string.h>
#include "boattypes.h"
#include "boatutility.h"
#include "boatlog.h"

#include "qapi_socket.h"
#include "include/netdb.h"
#include "http_client.h"
#include "qapi.h"
#include "qflog_utils.h"
#include "qapi_timer.h"
//#include "odm_ght_log.h"


#define DBG(x, arg...)  odm_ght_printf(debug_uart_context_D.uart_handle,"[DBG_http:]"x,##arg)
#define WARN(x, arg...) odm_ght_printf(debug_uart_context_D.uart_handle,"[WARN_http:]"x,##arg)
#define ERR(x, arg...)  odm_ght_printf(debug_uart_context_D.uart_handle,"[ERR_http:]"x,##arg)

#define boat_sys_log DBG

//#define strncasecmp lwip_strnicmp

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))

#define HTTPCLIENT_AUTHB_SIZE     128

#define HTTPCLIENT_CHUNK_SIZE     512
#define HTTPCLIENT_SEND_BUF_SIZE  512

#define HTTPCLIENT_MAX_HOST_LEN   64
#define HTTPCLIENT_MAX_URL_LEN    256

#define HTTPCLIENT_MAX_SOC_TIMEOUT    (10 * 60) // 10 min

#define FIBOCOMHTTPPORT_RECV_BUF_SIZE (4096)
TX_SEMAPHORE *g_tx_semaphore_returndata_ptr;

// static int httpclient_parse_host(char *url, char *host, size_t maxhost_len);
static int httpclient_parse_url(const char *url, char *scheme, size_t max_scheme_len, char *host, size_t maxhost_len, int *port, char *path, size_t max_path_len);
static int httpclient_tcp_send_all(int sock_fd, char *data, int length);
static int httpclient_conn(httpclient_t *client, char *host);
static int httpclient_recv(httpclient_t *client, char *buf, int min_len, int max_len, int *p_read_len);
static int httpclient_retrieve_content(httpclient_t *client, char *data, int len, httpclient_data_t *client_data);
static int httpclient_response_parse(httpclient_t *client, char *data, int len, httpclient_data_t *client_data);
#ifdef BOAT_HTTPCLIENT_SSL_ENABLE
static int httpclient_ssl_conn(httpclient_t *client, char *host);
static int httpclient_ssl_send_all( const char *data, size_t length);

static int httpclient_ssl_close(httpclient_t *client);
#endif

#ifdef BOAT_HTTPCLIENT_SSL_ENABLE
ssl_ctx_t boat_ssl_ctx;
WOLFSSL *boat_ssl;
#endif
static void httpclient_base64enc(char *out, const char *in)
{
    const char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" ;
    int i = 0, x = 0, l = 0;

    for (; *in; in++) 
    {
        x = x << 8 | *in;
        for (l += 8; l >= 6; l -= 6) 
        {
            out[i++] = code[(x >> (l - 6)) & 0x3f];
        }
    }
    if (l > 0) 
    {
        x <<= 6 - l;
        out[i++] = code[x & 0x3f];
    }
    for (; i % 4;) 
    {
        out[i++] = '=';
    }
    out[i] = '\0' ;
}

int strncasecmp(const char *str1, const char *str2, size_t len)
{
    int str1_len;
    int str2_len;
    int compare_len;
    int i;
    int c1;
    int c2;

    if (str1 == NULL && str2 == NULL) 
    {
        return 0;
    }
    
    if ((str1 == NULL) || (str2 == NULL))
    {
        return -1;
    }
    
    str1_len = strlen(str1);
    str2_len = (int)strlen(str2);
    
    compare_len = str1_len < str2_len ? str1_len : str2_len;
    compare_len = len < compare_len ? len : compare_len;

    for (i = 0; i < compare_len; i++) 
    {
        c1 = str1[i];
        c2 = str2[i];
        if (c1 != c2
           && c1 + ('a' - 'A') != c2
           && c1 - ('a' - 'A') != c2)
        {
            break;
        } 
    }

    if (i == compare_len) 
    {
        return 0;
    }else 
    {
        return (c1 - c2);
    }
    
}


int httpclient_conn(httpclient_t *client, char *host)
{

    struct addrinfo hints, *addr_list, *cur;
    struct timeval timeout;
    int ret = 0;
    char port[10] = {0};

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
  //  hints.ai_protocol = IPPROTO_TCP;

    timeout.tv_sec  = client->timeout_in_sec > HTTPCLIENT_MAX_SOC_TIMEOUT ? HTTPCLIENT_MAX_SOC_TIMEOUT : client->timeout_in_sec;
    timeout.tv_usec = 0;
    snprintf(port, sizeof(port), "%d", client->remote_port) ;
    if (getaddrinfo(host, port , &hints, &addr_list) != 0) 
   // if (getaddrinfo(host, NULL , &hints, &addr_list) != 0) 
    {
        DBG("getaddrinfo != 0, return HTTPCLIENT_UNRESOLVED_DNS");
        return HTTPCLIENT_UNRESOLVED_DNS;
    }

    /* Try the sockaddrs until a connection succeeds */
    ret = HTTPCLIENT_UNRESOLVED_DNS;
    for (cur = addr_list; cur != NULL; cur = cur->ai_next) 
    {
        client->handle = qapi_socket(cur->ai_family, cur->ai_socktype,
                                     0);
        if (client->handle == -1) 
        {
            ret = HTTPCLIENT_ERROR_CONN;
            continue;
        }
        /* set timeout if user need */
        if (client->timeout_in_sec > 0) 
        {
            qapi_setsockopt(client->handle, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            qapi_setsockopt(client->handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        }
        if (qapi_connect(client->handle, cur->ai_addr, (int)cur->ai_addrlen) == 0) 
        {
            ret = 0;
            break;
        }

        qapi_socketclose(client->handle);
        ret = HTTPCLIENT_ERROR_CONN;
    }

    freeaddrinfo(addr_list);
    return ret;
}


int httpclient_parse_url(const char *url, char *scheme, size_t max_scheme_len, char *host, size_t maxhost_len, int *port, char *path, size_t max_path_len)
{
    char *scheme_ptr = (char *)url;
    char *host_ptr = (char *)strstr(url, "://");
    size_t host_len = 0;
    size_t path_len;
    char *port_ptr;
    char *path_ptr;
    char *fragment_ptr;

    if (host_ptr == NULL) 
    {
        //WARN("Could not find host");
        //return HTTPCLIENT_ERROR_PARSE; /* URL is invalid */
        WARN("Could not find http char");
        memcpy(scheme, "http", 4);
        scheme[4] = '\0';
        host_ptr = scheme_ptr;
    }
    else
    {
        if (max_scheme_len < host_ptr - scheme_ptr + 1 ) 
        { /* including NULL-terminating char */
            //WARN("Scheme str is too small (%d >= %d)", max_scheme_len, host_ptr - scheme_ptr + 1);
            return HTTPCLIENT_ERROR_PARSE;
        }
        memcpy(scheme, scheme_ptr, host_ptr - scheme_ptr);
        scheme[host_ptr - scheme_ptr] = '\0';

        host_ptr += 3;
    }


    port_ptr = strchr(host_ptr, ':');
    if (port_ptr != NULL) 
    {
        uint16_t tport;
        host_len = port_ptr - host_ptr;
        port_ptr++;
        if (sscanf(port_ptr, "%hu", &tport) != 1)
        {
            WARN("Could not find port");
            return HTTPCLIENT_ERROR_PARSE;
        }
        *port = (int)tport;
    } 
    else 
    {
        *port = 0;
    }
    path_ptr = strchr(host_ptr, '/');
    //if (path_ptr == NULL) 
    //{
    //    return HTTPCLIENT_ERROR_PARSE;
    //}
    if (host_len == 0) 
    {
        host_len = path_ptr - host_ptr;
    }

    if (maxhost_len < host_len + 1) 
    { /* including NULL-terminating char */
        WARN("Host str is too small (%d >= %d)", maxhost_len, host_len + 1);
        return HTTPCLIENT_ERROR_PARSE;
    }
    memcpy(host, host_ptr, host_len);
    host[host_len] = '\0';

    fragment_ptr = strchr(host_ptr, '#');
    if (fragment_ptr != NULL) 
    {
        path_len = fragment_ptr - path_ptr;
    } 
    else 
    {
        if (path_ptr != NULL) 
        {
            path_len = strlen(path_ptr);
        } 
        else 
        {
            path_len = 0;
        }
    }

    if ( max_path_len < path_len + 1 ) 
    { /* including NULL-terminating char */
        WARN("Path str is too small (%d >= %d)", max_path_len, path_len + 1);
        return HTTPCLIENT_ERROR_PARSE;
    }

    if (path_ptr != NULL && path_len > 0) 
    {
        memcpy(path, path_ptr, path_len);
    }
    path[path_len] = '\0';

    return HTTPCLIENT_OK;
}

#if 0
int httpclient_parse_host(char *url, char *host, size_t maxhost_len)
{
    char *host_ptr = (char *) strstr(url, "://");
    size_t host_len = 0;
    char *port_ptr;
    char *path_ptr;

    if (host_ptr == NULL) 
    {
        WARN("Could not find host");
        return HTTPCLIENT_ERROR_PARSE; /* URL is invalid */
    }
    host_ptr += 3;

    port_ptr = strchr(host_ptr, ':');
    if (port_ptr != NULL) 
    {
        uint16_t tport;
        host_len = port_ptr - host_ptr;
        port_ptr++;
        if (sscanf(port_ptr, "%hu", &tport) != 1)
        {
            WARN("Could not find port");
            return HTTPCLIENT_ERROR_PARSE;
        }
    }

    path_ptr = strchr(host_ptr, '/');
    if (host_len == 0) 
    {
        host_len = path_ptr - host_ptr;
    }

    if (maxhost_len < host_len + 1 ) 
    { /* including NULL-terminating char */
        WARN("Host str is too small (%d >= %d)", maxhost_len, host_len + 1);
        return HTTPCLIENT_ERROR_PARSE;
    }
    memcpy(host, host_ptr, host_len);
    host[host_len] = '\0';

    return HTTPCLIENT_OK;
}
#endif

int httpclient_get_info(httpclient_t *client, char *send_buf, int *send_idx, char *buf, size_t len)   /* 0 on success, err code on failure */
{
    int ret ;
    int cp_len ;
    int idx = *send_idx;

    if (len == 0) 
    {
        len = strlen(buf);
    }

    do {
        if ((HTTPCLIENT_SEND_BUF_SIZE - idx) >= len)
        {
            cp_len = len ;
        } 
        else 
        {
            cp_len = HTTPCLIENT_SEND_BUF_SIZE - idx ;
        }

        memcpy(send_buf + idx, buf, cp_len) ;
        idx += cp_len ;
        len -= cp_len ;

        if (idx == HTTPCLIENT_SEND_BUF_SIZE) 
        {
            if (client->is_http == false) 
            {
                ERR("send buffer overflow");
                return HTTPCLIENT_ERROR ;
            }
            ret = httpclient_tcp_send_all(client->handle, send_buf, HTTPCLIENT_SEND_BUF_SIZE) ;
            if (ret) 
            {
                return (ret) ;
            }
        }
    } while (len) ;

    *send_idx = idx;
    return HTTPCLIENT_OK ;
}

void httpclient_set_custom_header(httpclient_t *client, char *header)
{
    client->header = header;
}

int httpclient_basic_auth(httpclient_t *client, char *user, char *password)
{
    if ((strlen(user) + strlen(password)) >= HTTPCLIENT_AUTHB_SIZE) 
    {
        return HTTPCLIENT_ERROR ;
    }
    client->auth_user = user;
    client->auth_password = password;
    return HTTPCLIENT_OK;
}

int httpclient_send_auth(httpclient_t *client, char *send_buf, int *send_idx)
{
    char b_auth[(int)((HTTPCLIENT_AUTHB_SIZE + 3) * 4 / 3 + 3)] ;
    char base64buff[HTTPCLIENT_AUTHB_SIZE + 3];

    httpclient_get_info(client, send_buf, send_idx, "Authorization: Basic ", 0);
    sprintf(base64buff, "%s:%s", client->auth_user, client->auth_password);
    DBG("bAuth: %s", base64buff) ;
    httpclient_base64enc(b_auth, base64buff);
    b_auth[strlen(b_auth) + 2] = '\0';
    b_auth[strlen(b_auth) + 1] = '\n';
    b_auth[strlen(b_auth)] = '\r';
    DBG("b_auth:%s", b_auth);
    httpclient_get_info(client, send_buf, send_idx, b_auth, 0);
    return HTTPCLIENT_OK;
}


int httpclient_tcp_send_all(int sock_handle, char *data, int length)
{
    int written_len = 0;
    int ret = 0;

    while (written_len < length)
    {
        //ret = send(sock_fd, data + written_len, length - written_len, 0);
        ret = qapi_send(sock_handle,data + written_len, length - written_len,0);
        if (ret > 0) 
        {
            written_len += ret;
            continue;
        } 
        else if (ret == 0) 
        {
            return written_len;
        } 
        else 
        {
            return -1; /* Connnection error */
        }
    }

    return written_len;
}

int httpclient_send_header(httpclient_t *client, char *url, int method, httpclient_data_t *client_data)
{
    char scheme[8] = {0};
    char host[HTTPCLIENT_MAX_HOST_LEN] = {0};
    char path[HTTPCLIENT_MAX_URL_LEN] = {0};
    int len;
    char send_buf[HTTPCLIENT_SEND_BUF_SIZE] = {0};
    char buf[HTTPCLIENT_SEND_BUF_SIZE] = {0};
    char *meth = (method == HTTPCLIENT_GET) ? "GET" : (method == HTTPCLIENT_POST) ? "POST" : (method == HTTPCLIENT_PUT) ? "PUT" : (method == HTTPCLIENT_DELETE) ? "DELETE" : (method == HTTPCLIENT_HEAD) ? "HEAD" : "";
    int ret, port;

    /* First we need to parse the url (http[s]://host[:port][/[path]]) */
    int res = httpclient_parse_url(url, scheme, sizeof(scheme), host, sizeof(host), &(port), path, sizeof(path));
    if (res != HTTPCLIENT_OK)
    {
        ERR("httpclient_parse_url returned %d", res);
        return res;
    }

    /* Send request */
    memset(send_buf, 0, HTTPCLIENT_SEND_BUF_SIZE);
    len = 0; /* Reset send buffer */
    client->method = method;
    
    if( strlen(path) == 0 ) 
    {
        snprintf(buf, sizeof(buf), "%s /%s HTTP/1.1\r\nHost: %s\r\n", meth, path, host); /* Write request */
    }
    else 
    {
        snprintf(buf, sizeof(buf), "%s %s HTTP/1.1\r\nHost: %s\r\n", meth, path, host); /* Write request */
    }

    ret = httpclient_get_info(client, send_buf, &len, buf, strlen(buf));
    if (ret) 
    {
        ERR("Could not write request");
        return HTTPCLIENT_ERROR_CONN;
    }

    /* Send all headers */
    if (client->auth_user) 
    {
        httpclient_send_auth(client, send_buf, &len); /* send out Basic Auth header */
    }

    /* Add user header information */
    if (client->header) 
    {
        httpclient_get_info(client, send_buf, &len, (char *)client->header, strlen(client->header));
    }

    if ( client_data->post_buf != NULL ) 
    {
        snprintf(buf, sizeof(buf), "Content-Length: %d\r\n", client_data->post_buf_len);
        httpclient_get_info(client, send_buf, &len, buf, strlen(buf));

        if (client_data->post_content_type != NULL)  
        {
            snprintf(buf, sizeof(buf), "Content-Type: %s\r\n", client_data->post_content_type);
            httpclient_get_info(client, send_buf, &len, buf, strlen(buf));
        }
    }

    /* Close headers */
    httpclient_get_info(client, send_buf, &len, "\r\n", 0);

    DBG("Trying to write %d bytes http header:%s", len, send_buf);

#ifdef BOAT_HTTPCLIENT_SSL_ENABLE
    if (client->is_http == false) 
    {
        DBG("Enter PolarSSL_write");        
    
        if (httpclient_ssl_send_all( send_buf, len) != len) 
        {
            ERR("SSL_write failed");
            return HTTPCLIENT_ERROR;
        }
        return HTTPCLIENT_OK;
    }
#endif

    ret = httpclient_tcp_send_all(client->handle, send_buf, len);
    if (ret > 0) 
    {
        DBG("Written %d bytes, socket_handle = %d", ret, client->handle);
    } 
    else if (ret == 0) 
    {
        WARN("ret == 0,Connection was closed by server");
        return HTTPCLIENT_CLOSED; /* Connection was closed by server */
    } 
    else 
    {
        ERR("Connection error (send returned %d)", ret);
        return HTTPCLIENT_ERROR_CONN;
    }

    return HTTPCLIENT_OK;
}

int httpclient_send_userdata(httpclient_t *client, httpclient_data_t *client_data)
{
    int ret = 0;

    if (client_data->post_buf && client_data->post_buf_len) 
    {
        DBG("client_data->post_buf:%s", client_data->post_buf);
#ifdef BOAT_HTTPCLIENT_SSL_ENABLE
        if (client->is_http == false) 
        {
            if (httpclient_ssl_send_all(client_data->post_buf, client_data->post_buf_len) != client_data->post_buf_len) 
            {
                ERR("SSL_write failed");
                return HTTPCLIENT_ERROR;
            }
        } else
#endif
        {
            ret = httpclient_tcp_send_all(client->handle, client_data->post_buf, client_data->post_buf_len);
            if (ret > 0) 
            {
                DBG("Written %d bytes", ret);
            } else if (ret == 0) 
            {
                WARN("ret == 0,Connection was closed by server");
                return HTTPCLIENT_CLOSED; /* Connection was closed by server */
            } 
            else 
            {
                ERR("Connection error (send returned %d)", ret);
                return HTTPCLIENT_ERROR_CONN;
            }
        }
    }

    return HTTPCLIENT_OK;
}

int httpclient_recv(httpclient_t *client, char *buf, int min_len, int max_len, int *p_read_len)   /* 0 on success, err code on failure */
{
    int ret = 0;
    size_t readLen = 0;

    while (readLen < max_len) 
    {
        buf[readLen] = '\0';
        if (client->is_http) 
        {
        #if 0
            if (readLen < min_len) 
            {
                //ret = recv(client->socket, buf + readLen, min_len - readLen, 0);
                ret = fibo_sock_recv(client->socket, buf + readLen, min_len - readLen);
                DBG("recv [not blocking] return:%d", ret);
                if (ret == 0) 
                {
                    DBG("recv [blocking] return 0 may disconnected");
                    ret = HTTPCLIENT_CLOSED;
                }
            } else 
            {
                //ret = recv(client->socket, buf + readLen, max_len - readLen, MSG_DONTWAIT);
                ret = fibo_sock_recv(client->socket, buf + readLen, max_len - readLen);
                DBG("recv [not blocking] return:%d", ret);
                if (ret == -1 && errno == EWOULDBLOCK) 
                {
                    DBG("recv [not blocking] EWOULDBLOCK");
                    break;
                }
            }
        #else
            //ret = recv(client->socket, buf + readLen, max_len - readLen, 0);
            //ret = fibo_sock_recv(client->socket, (unsigned char*)buf + readLen, max_len - readLen);
            ret = qapi_recv(client->handle,buf + readLen, max_len - readLen,0);
        #endif
        }
#ifdef BOAT_HTTPCLIENT_SSL_ENABLE
        else 
        {       
            int nb_flag;
            if (readLen < min_len) 
            {   
                // nb_flag = 1;
                // lwip_ioctl(client->socket, FIONBIO, &nb_flag);
                boat_sys_log("httpread:readlen:%d,min_len:%d",readLen,min_len);
                ret = wolfSSL_read(boat_ssl, (unsigned char *)buf + readLen, max_len - readLen);
                boat_sys_log("httpread:ret:%d",ret);
               
            } 
            else 
            {
                // nb_flag = 0;
                // lwip_ioctl(client->socket, FIONBIO, &nb_flag);
                boat_sys_log("httpread1:readlen:%d,max_len:%d",readLen,max_len);
                ret = wolfSSL_read(boat_ssl, (unsigned char *)buf + readLen, max_len - readLen);  
                boat_sys_log("httpread2:ret:%d",ret);                
            }
        }
#endif

        if (ret > 0) 
        {
            readLen += ret;
        } 
        else if (ret == 0) 
        {
            break;
        } 
        else if (ret == HTTPCLIENT_CLOSED) 
        {
            return HTTPCLIENT_CLOSED;
        } 
        else 
        {
            ERR("Connection error (recv returned %d)", ret);
            *p_read_len = readLen;
            return HTTPCLIENT_ERROR_CONN;
        }
    }

    DBG("Read %d bytes", readLen);
    *p_read_len = readLen;
    buf[readLen] = '\0';

    return HTTPCLIENT_OK;
}

int httpclient_retrieve_content(httpclient_t *client, char *data, int len, httpclient_data_t *client_data)
{
    int count = 0;
    int templen = 0;
    int crlf_pos;
    //?a��?��?�㨹???��     
	static char remain_buf[512] = {0};
	static int remain_len = 0;
	if (remain_len > 0 && remain_len < 512)
	{
		DBG("last retrieve remain_len:%d",remain_len);
		memcpy(data,remain_buf,remain_len);
		len = remain_len;
		memset(remain_buf,0,sizeof(remain_buf));
		remain_len = 0;
	}

	/* Receive data */
    DBG("Receiving data:%s", data);
    client_data->is_more = true;

    if (client_data->response_content_len == -1 && client_data->is_chunked == false) 
    {
        while(true)
        {
            int ret, max_len;
            if (count + len < client_data->response_buf_len - 1) 
            {
                memcpy(client_data->response_buf + count, data, len);
                count += len;
                client_data->response_buf[count] = '\0';
            } 
            else 
            {
                memcpy(client_data->response_buf + count, data, client_data->response_buf_len - 1 - count);
                client_data->response_buf[client_data->response_buf_len - 1] = '\0';
                client_data->content_block_len = client_data->response_buf_len - 1;
                return HTTPCLIENT_RETRIEVE_MORE_DATA;
            }

            max_len = MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count);
            ret = httpclient_recv(client, data, 1, max_len, &len);

            /* Receive data */
            DBG("data len: %d %d", len, count);

            if (ret <= HTTPCLIENT_ERROR_CONN) 
            {
                DBG("ret == HTTPCLIENT_ERROR_CONN");
                client_data->content_block_len = count;
                client_data->is_more = false;
                return ret;
            }

            if (len == 0) 
            {/* read no more data */
                DBG("no more len == 0");
                client_data->is_more = false;
                return HTTPCLIENT_OK;
            }
        }
    }

    while (true) 
    {
        size_t readLen = 0;

        if (client_data->is_chunked && client_data->retrieve_len <= 0) 
        {
            /* Read chunk header */
            bool foundCrlf;
            int n;
            do {               
                DBG("len: %d", len);
                foundCrlf = false;
                crlf_pos = 0;
                data[len] = 0;
                if (len >= 2) 
                {
                    for (; crlf_pos < len - 2; crlf_pos++) 
                    {
                        if (data[crlf_pos] == '\r' && data[crlf_pos + 1] == '\n') 
                        {
                            foundCrlf = true;
                            break;
                        }
                    }
                }
                if (!foundCrlf) 
                { /* Try to read more */
                    int max_len = MIN(HTTPCLIENT_CHUNK_SIZE - len - 1, client_data->response_buf_len - 1 - count);
                    if (len < max_len) 
                    {
                        int new_trf_len, ret;
                        ret = httpclient_recv(client, data + len, 0, max_len, &new_trf_len);
                        len += new_trf_len;
                        if (ret <= HTTPCLIENT_ERROR_CONN) 
                        {
                            return ret;
                        } 
                        else 
                        {
                            //osiDelayUS(1000);
                            qapi_Timer_Sleep(1000,QAPI_TIMER_UNIT_USEC,true);
                            continue;
                        }
                    } 
                    else 
                    {
                        return HTTPCLIENT_ERROR;
                    }
                }
            } while (!foundCrlf);
            data[crlf_pos] = '\0';
            n = sscanf(data, "%x", &readLen);/* chunk length */
            DBG("chunked size: %d", readLen);
            client_data->retrieve_len = readLen;
            client_data->response_content_len += client_data->retrieve_len;
            if (n != 1) 
            {
                boat_sys_log("Could not read chunk length");
                return HTTPCLIENT_ERROR_PRTCL;
            }

            memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2)); /* Not need to move NULL-terminating char any more */
            len -= (crlf_pos + 2);

            if ( readLen == 0 ) 
            {
                /* Last chunk */
                client_data->is_more = false;
                DBG("no more (last chunk)");
                break;
            }
        } 
        else 
        {
            readLen = client_data->retrieve_len;
        }

        DBG("Retrieving %d bytes, len:%d", readLen, len);

        do {            
            DBG("readLen %d, len:%d", readLen, len);
            templen = MIN(len, readLen);
            if (count + templen < client_data->response_buf_len - 1) 
            {
                memcpy(client_data->response_buf + count, data, templen);
                count += templen;
                client_data->response_buf[count] = '\0';
                client_data->retrieve_len -= templen;
            } 
            else 
            {
                memcpy(client_data->response_buf + count, data, client_data->response_buf_len - 1 - count);
                client_data->response_buf[client_data->response_buf_len - 1] = '\0';
                client_data->retrieve_len -= (client_data->response_buf_len - 1 - count);
				//?a��?��?�㨹???��
				remain_len = templen - (client_data->response_buf_len - 1 - count);
				if(remain_len)
				{
					memcpy(remain_buf,data + client_data->response_buf_len - 1 - count,remain_len);
					DBG("data is larger than response_buf_len,remain_len:%d",remain_len);
				}
				//?a��?��?�㨹???��
                if (readLen > len || remain_len) 
                {
                    client_data->content_block_len = client_data->response_buf_len - 1;
                    return HTTPCLIENT_RETRIEVE_MORE_DATA;
                } 
                else 
                {
                    count += templen;
                }
            }

            // if ( len > readLen ) {            
            if (len >= readLen) {
                DBG("memmove %d %d %d", readLen, len, client_data->retrieve_len);
                memmove(data, &data[readLen], len - readLen); /* chunk case, read between two chunks */
                len -= readLen;
                readLen = 0;
                client_data->retrieve_len = 0;
            } 
            else 
            {
                readLen -= len;
            }
            
            if (readLen) 
            {
                int ret;
                // int max_len = MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count);                
                int max_len = MIN(MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count), readLen);
                ret = httpclient_recv(client, data, 1, max_len, &len);
                if (ret <= HTTPCLIENT_ERROR_CONN) 
                {
                    return ret;
                }
            }
        } while (readLen);

        if (client_data->is_chunked) 
        {
            if (len < 2) 
            {
                int new_trf_len, ret;
                int max_len = MIN(HTTPCLIENT_CHUNK_SIZE - len - 1, client_data->response_buf_len - 1 - count);
                /* Read missing chars to find end of chunk */
                ret = httpclient_recv(client, data + len, 2 - len, max_len, &new_trf_len);
                if (ret <= HTTPCLIENT_ERROR_CONN) 
                {
                    return ret;
                }
                len += new_trf_len;
            }
            if ((data[0] != '\r') || (data[1] != '\n')) 
            {
                boat_sys_log("Format error, %s", data); /* after memmove, the beginning of next chunk */
                return HTTPCLIENT_ERROR_PRTCL;
            }
            memmove(data, &data[2], len - 2); /* remove the \r\n */
            len -= 2;
        } 
        else 
        {
            DBG("no more(content-length)");
            client_data->is_more = false;
            break;
        }

    }
    client_data->content_block_len = count;

    return HTTPCLIENT_OK;
}

int httpclient_response_parse(httpclient_t *client, char *data, int len, httpclient_data_t *client_data)
{
    int crlf_pos;
    int header_buf_len = client_data->header_buf_len;
    char *header_buf = client_data->header_buf;

    // reset the header buffer
    memset(header_buf, 0, header_buf_len);
    
    client_data->response_content_len = -1;

    char *crlf_ptr = strstr(data, "\r\n");
    if (crlf_ptr == NULL) 
    {
        boat_sys_log("\r\n not found");
        return HTTPCLIENT_ERROR_PRTCL;
    }

    crlf_pos = crlf_ptr - data;
    data[crlf_pos] = '\0';

    /* Parse HTTP response */
    if (sscanf(data, "HTTP/%*d.%*d %d %*[^\r\n]", &(client->response_code)) != 1) 
    {
        /* Cannot match string, error */
        boat_sys_log("Not a correct HTTP answer : %s", data);
        return HTTPCLIENT_ERROR_PRTCL;
    }

    if ((client->response_code < 200) || (client->response_code >= 400)) 
    {
        /* Did not return a 2xx code; TODO fetch headers/(&data?) anyway and implement a mean of writing/reading headers */
        WARN("Response code %d", client->response_code);
    }

    DBG("Reading headers%s", data);

    memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2) + 1); /* Be sure to move NULL-terminating char as well */
    len -= (crlf_pos + 2);

    client_data->is_chunked = false;

    /* Now get headers */
    while (true) 
    {
        char *colon_ptr, *key_ptr, *value_ptr;
        int key_len, value_len;
        
        crlf_ptr = strstr(data, "\r\n");
        if (crlf_ptr == NULL) 
        {
            if (len < HTTPCLIENT_CHUNK_SIZE - 1) 
            {
                int new_trf_len, ret;
                ret = httpclient_recv(client, data + len, 1, HTTPCLIENT_CHUNK_SIZE - len - 1, &new_trf_len);
                len += new_trf_len;
                data[len] = '\0';
                DBG("Read %d chars; In buf: [%s]", new_trf_len, data);
                if (ret <= HTTPCLIENT_ERROR_CONN) 
                {
                    return ret;
                } 
                else 
                {
                    continue;
                }
            } 
            else 
            {
                DBG("header len > chunksize");
                return HTTPCLIENT_ERROR;
            }
        }

        crlf_pos = crlf_ptr - data;        
        if (crlf_pos == 0) 
        { /* End of headers */
            memmove(data, &data[2], len - 2 + 1); /* Be sure to move NULL-terminating char as well */
            len -= 2;
            break;
        }
        
        colon_ptr = strstr(data, ": ");        
        if (colon_ptr) 
        {             
            if (header_buf_len >= crlf_pos + 2) 
            {
                /* copy response header to caller buffer */
                memcpy(header_buf, data, crlf_pos + 2);                                
                header_buf += crlf_pos + 2;
                header_buf_len -= crlf_pos + 2;
            }
            
            key_len = colon_ptr - data;
            value_len = crlf_ptr - colon_ptr - strlen(": ");            
            key_ptr = data;
            value_ptr = colon_ptr + strlen(": ");

            DBG("Read header : %.*s: %.*s", key_len, key_ptr, value_len, value_ptr); 
            if (0 == strncasecmp(key_ptr, "Content-Length", key_len)) 
            {
                sscanf(value_ptr, "%d[^\r]", &(client_data->response_content_len));                
                client_data->retrieve_len = client_data->response_content_len;
            } 
            else if (0 == strncasecmp(key_ptr, "Transfer-Encoding", key_len)) 
            {
                if (0 == strncasecmp(value_ptr, "Chunked", value_len)) 
                {
                    client_data->is_chunked = true;
                    client_data->response_content_len = 0;
                    client_data->retrieve_len = 0;
                }
            } 
           
            memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2) + 1); /* Be sure to move NULL-terminating char as well */
            len -= (crlf_pos + 2);
        } 
        else 
        {
            ERR("Could not parse header");
            return HTTPCLIENT_ERROR;
        }
    }

    if (client->method == HTTPCLIENT_HEAD) 
    {
        /* no content if it's HEAD method. */
        return HTTPCLIENT_OK;
    } 
    else 
    {
        return httpclient_retrieve_content(client, data, len, client_data);
    }
}


HTTPCLIENT_RESULT httpclient_connect(httpclient_t *client, char *url)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    char host[HTTPCLIENT_MAX_HOST_LEN] = {0};
    char scheme[8] = {0};
    char path[HTTPCLIENT_MAX_URL_LEN] = {0};
    
    /* First we need to parse the url (http[s]://host[:port][/[path]]) */
    int res = httpclient_parse_url(url, scheme, sizeof(scheme), host, sizeof(host), &(client->remote_port), path, sizeof(path));
    if (res != HTTPCLIENT_OK) 
    {
        ERR("httpclient_parse_url returned %d", res);
        return (HTTPCLIENT_RESULT)res;
    }

    // http or https
    if (strcmp(scheme, "https") == 0) 
        client->is_http = false;
    else if (strcmp(scheme, "http") == 0)
        client->is_http = true;

    // default http 80 port, https 443 port
    if (client->remote_port == 0) 
    {
        if (client->is_http) 
        { 
            client->remote_port = HTTP_PORT;
        } else {
            client->remote_port = HTTPS_PORT;
        }
    }
        
    DBG("http?:%d, port:%d, host:%s", client->is_http, client->remote_port, host);

    client->handle = -1;
    if (client->is_http) 
        ret = httpclient_conn(client, host);
#ifdef BOAT_HTTPCLIENT_SSL_ENABLE
    else {
        ret = httpclient_ssl_conn(client, host);
    }
#endif

    DBG("httpclient_connect() result:%d, client:%p", ret, client);
    return (HTTPCLIENT_RESULT)ret;
}

HTTPCLIENT_RESULT httpclient_send_request(httpclient_t *client, char *url, int method, httpclient_data_t *client_data)
{
    int ret = HTTPCLIENT_ERROR_CONN;

    if (client->handle == -1) 
    {
        return (HTTPCLIENT_RESULT)ret;
    }

    ret = httpclient_send_header(client, url, method, client_data);
    if (ret != 0) 
    {
        return (HTTPCLIENT_RESULT)ret;
    }

    if (method == HTTPCLIENT_POST || method == HTTPCLIENT_PUT) 
    {
        ret = httpclient_send_userdata(client, client_data);
    }
   
    DBG("httpclient_send_request() result:%d, client:%p", ret, client);
    return (HTTPCLIENT_RESULT)ret;
}

HTTPCLIENT_RESULT httpclient_recv_response(httpclient_t *client, httpclient_data_t *client_data)
{
    int reclen = 0;
    int ret = HTTPCLIENT_ERROR_CONN;
    // TODO: header format:  name + value must not bigger than HTTPCLIENT_CHUNK_SIZE.
    char buf[HTTPCLIENT_CHUNK_SIZE] = {0}; // char buf[HTTPCLIENT_CHUNK_SIZE*2] = {0};
    boat_sys_log("http start recv response");
    if (client->handle == -1) 
    {
        return (HTTPCLIENT_RESULT)ret;
    }

    if (client_data->is_more) 
    {
        boat_sys_log("http start retrieve data");
        client_data->response_buf[0] = '\0';
        ret = httpclient_retrieve_content(client, buf, reclen, client_data);
    } 
    else 
    {
        boat_sys_log("http enter recv");
        ret = httpclient_recv(client, buf, 1, HTTPCLIENT_CHUNK_SIZE - 1, &reclen);
        if (ret != 0) 
        {
            return (HTTPCLIENT_RESULT)ret;
        }

        buf[reclen] = '\0';

        if (reclen) 
        {
            DBG("reclen:%d, buf:%s", reclen, buf);
            ret = httpclient_response_parse(client, buf, reclen, client_data);
        }
    }

    DBG("httpclient_recv_response() result:%d, client:%p", ret, client);
    return (HTTPCLIENT_RESULT)ret;
}

void httpclient_close(httpclient_t *client)
{
    if (client->is_http) 
    {
        if (client->handle != -1)
            //close(client->socket);
            qapi_socketclose(client->handle);
    }
#ifdef BOAT_HTTPCLIENT_SSL_ENABLE
    else 
        httpclient_ssl_close(client);
#endif

    client->handle = -1;
    DBG("httpclient_close() client:%p", client);
}

int httpclient_get_response_code(httpclient_t *client)
{
    return client->response_code;
}

static HTTPCLIENT_RESULT httpclient_common(httpclient_t *client, char *url, int method, httpclient_data_t *client_data)
{
    HTTPCLIENT_RESULT ret = httpclient_connect(client, url);
    
    if (!ret) 
    {
        boat_sys_log("http send request");
        ret = httpclient_send_request(client, url, method, client_data);

        if (!ret) 
        {
            boat_sys_log("recv response");
            ret = httpclient_recv_response(client, client_data);
        }
    }

    httpclient_close(client);

    return ret;
}

HTTPCLIENT_RESULT httpclient_get(httpclient_t *client, char *url, httpclient_data_t *client_data)
{
    return httpclient_common(client, url, HTTPCLIENT_GET, client_data);
}

HTTPCLIENT_RESULT httpclient_post(httpclient_t *client, char *url, httpclient_data_t *client_data)
{
    return httpclient_common(client, url, HTTPCLIENT_POST, client_data);
}

HTTPCLIENT_RESULT httpclient_put(httpclient_t *client, char *url, httpclient_data_t *client_data)
{
    return httpclient_common(client, url, HTTPCLIENT_PUT, client_data);
}

HTTPCLIENT_RESULT httpclient_delete(httpclient_t *client, char *url, httpclient_data_t *client_data)
{
    return httpclient_common(client, url, HTTPCLIENT_DELETE, client_data);
}

int httpclient_get_response_header_value(char *header_buf, char *name, int *val_pos, int *val_len)
{    
    char *data = header_buf;
    char *crlf_ptr, *colon_ptr, *key_ptr, *value_ptr;
    int key_len, value_len;

    if (header_buf == NULL || name == NULL || val_pos == NULL  || val_len == NULL )
        return -1;
    while (true) 
    {
        crlf_ptr = strstr(data, "\r\n");
        colon_ptr = strstr(data, ": ");        
        if (colon_ptr) 
        {                         
            key_len = colon_ptr - data;
            value_len = crlf_ptr - colon_ptr - strlen(": ");            
            key_ptr = data;
            value_ptr = colon_ptr + strlen(": ");
            
            DBG("Response header: %.*s: %.*s", key_len, key_ptr, value_len, value_ptr);
            if (0 == strncasecmp(key_ptr, name, key_len)) 
            {
                *val_pos = value_ptr - header_buf;
                *val_len = value_len;
                return 0;
            } 
            else 
            {                 
                data = crlf_ptr + 2;
                continue;
            }
        } 
        else 
        {
            
            return -1;
        }   
    }
}

#ifdef BOAT_HTTPCLIENT_SSL_ENABLE



static void httpclient_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    // printf("%s\n", str);    
    DBG("%s", str);
}

static int httpclient_ssl_send_all(const char *data, size_t length)
{
    size_t written_len = 0;

    while (written_len < length) 
    {
        int ret = wolfSSL_write(boat_ssl, (unsigned char *)(data + written_len), (length - written_len));
        if (ret > 0) 
        {
            written_len += ret;
            continue;
        } 
        else if (ret == 0) 
        {
            return written_len;
        } 
        else 
        {
            return -1; /* Connnection error */
        }
    }

    return written_len;
}



static int httpclient_ssl_conn(httpclient_t *client, char *host)
{
    const char *pers = "https";
    int value, ret = 0; 
    uint32_t flags;
    char port[10] = {0};
    struct addrinfo hints, *addr_list, *cur;
    struct timeval timeout;
    timeout.tv_sec  = client->timeout_in_sec > HTTPCLIENT_MAX_SOC_TIMEOUT ? HTTPCLIENT_MAX_SOC_TIMEOUT : client->timeout_in_sec;
    timeout.tv_usec = 0;
    
    boat_sys_log("ssl conn starts");
    /* Do name resolution with both IPv6 and IPv4 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
     snprintf(port, sizeof(port), "%d", client->remote_port);
    if (getaddrinfo(host, port, &hints, &addr_list) != 0)
    {
        boat_sys_log("ssl dns failed");
        return -1;
    }
        

    for(cur = addr_list; cur != NULL; cur = cur->ai_next)
    {
        client->socket = (int)socket(cur->ai_family, cur->ai_socktype,
                                     cur->ai_protocol);
        if (client->socket < 0)
        {
            ret = -1;
            continue;
        }
        if (client->timeout_in_sec > 0) 
        {
            lwip_setsockopt(client->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            lwip_setsockopt(client->socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        }
        if (connect(client->socket, cur->ai_addr,  cur->ai_addrlen) == 0)
        {
            ret = 0;
            DBG("ssl tcp connect ok\n");
            break;
        }

        close(client->socket);
        ret = -1;
    }
    freeaddrinfo(addr_list);
    if (ret == -1)
    {
        DBG("ssl tcp connect failed\n");
        return -1;
    }
    set_ssl_ctx_default(&boat_ssl_ctx);
    wolf_ssl_init(&boat_ssl_ctx);  
    
    boat_ssl = wolfSSL_new(boat_ssl_ctx.wolf_ctx);
    
    SSL_set_fd(boat_ssl, client->socket);
    if ((SSL_connect(boat_ssl)) == WOLFSSL_SUCCESS)
    {
        boat_sys_log("WOLFSSL_CONNECT_SUCCESS");     
    }
    else
    {
        boat_sys_log("WOLFSSL_CONNECT_ERROR"); 
        wolfSSL_free(boat_ssl);
    } 
    return ret;
}

static int httpclient_ssl_close(httpclient_t *client)
{
    close(client->socket);
    wolfSSL_free(boat_ssl);
    return 0;
}
#endif

#endif

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
            odm_ght_printf(debug_uart_context_D.uart_handle,"Stop data call success\n");
        }
        stat = qapi_DSS_Rel_Data_Srvc_Hndl(http_dss_handle);
        if (QAPI_OK != stat)
        {
            odm_ght_printf(debug_uart_context_D.uart_handle,"Release data service handle failed:%d\n", stat);
        }
        http_dss_handle = NULL;
    }
    if (DSS_LIB_STAT_SUCCESS_E == http_netctl_lib_status)
    {
        qapi_DSS_Release(QAPI_DSS_MODE_GENERAL);
        http_netctl_lib_status = DSS_LIB_STAT_INVALID_E;
    }

}