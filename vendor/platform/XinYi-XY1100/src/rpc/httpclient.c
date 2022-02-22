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

/*xinyi1100 header include */
#include "softap_api.h"

#include "boatutility.h"
#include "httpclient.h"
#include "lwip/sockets.h"
#include "stdio.h"
#include "errno.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "lwip/err.h"
#include "los_task.h"

#define DBG(x, arg...)  xy_printf("[httpclient]"x,##arg)
#define WARN(x, arg...) xy_printf("[httpclient]"x,##arg)
#define ERR(x, arg...)  xy_printf("[httpclient]"x,##arg)

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))

#define HTTPCLIENT_AUTHB_SIZE     128

#define HTTPCLIENT_CHUNK_SIZE     512
#define HTTPCLIENT_SEND_BUF_SIZE  512

#define HTTPCLIENT_MAX_HOST_LEN   64
#define HTTPCLIENT_MAX_URL_LEN    256

#define HTTPCLIENT_MAX_SOC_TIMEOUT    (10 * 60) // 10 min

/*
struct lwip_timeval {
	time_t		tv_sec;		
	suseconds_t	tv_usec;	
};
*/
// static int httpclient_parse_host(char *url, char *host, size_t maxhost_len);
static int httpclient_parse_url(const char *url, char *scheme, size_t max_scheme_len, char *host, size_t maxhost_len, int *port, char *path, size_t max_path_len);
static int httpclient_tcp_send_all(int sock_fd, char *data, int length);
static int httpclient_conn(httpclient_t *client, char *host);
static int httpclient_recv(httpclient_t *client, char *buf, int min_len, int max_len, int *p_read_len);
static int httpclient_retrieve_content(httpclient_t *client, char *data, int len, httpclient_data_t *client_data);
static int httpclient_response_parse(httpclient_t *client, char *data, int len, httpclient_data_t *client_data);

static void httpclient_base64enc(char *out, const char *in)
{
    const char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" ;
    int i = 0, x = 0, l = 0;

    for (; *in; in++) {
        x = x << 8 | *in;
        for (l += 8; l >= 6; l -= 6) {
            out[i++] = code[(x >> (l - 6)) & 0x3f];
        }
    }
    if (l > 0) {
        x <<= 6 - l;
        out[i++] = code[x & 0x3f];
    }
    for (; i % 4;) {
        out[i++] = '=';
    }
    out[i] = '\0' ;
}


int httpclient_conn(httpclient_t *client, char *host)
{
    struct addrinfo hints, *addr_list, *cur;
    struct lwip_timeval timeout;
    int ret = 0;
    char port[10] = {0};

    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    timeout.tv_sec  = client->timeout_in_sec > HTTPCLIENT_MAX_SOC_TIMEOUT ? HTTPCLIENT_MAX_SOC_TIMEOUT : client->timeout_in_sec;
    timeout.tv_usec = 0;
    snprintf(port, sizeof(port), "%d", client->remote_port) ;
    if ( getaddrinfo( host, port , &hints, &addr_list ) != 0 ) {
        DBG("getaddrinfo != 0, return HTTPCLIENT_UNRESOLVED_DNS");
        return HTTPCLIENT_UNRESOLVED_DNS;
    }
    DBG("000000");
    /* Try the sockaddrs until a connection succeeds */
    ret = HTTPCLIENT_UNRESOLVED_DNS;
    for ( cur = addr_list; cur != NULL; cur = cur->ai_next ) {
        client->socket = (int) socket( cur->ai_family, cur->ai_socktype,
                                        cur->ai_protocol );
        DBG("11111");
        if ( client->socket < 0 ) {
            DBG("client->socket < 0 ");
            ret = HTTPCLIENT_ERROR_CONN;
            continue;
        }
        /* set timeout if user need */
        if ( client->timeout_in_sec > 0) {
            lwip_setsockopt(client->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            lwip_setsockopt(client->socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        }
        if ( connect( client->socket, cur->ai_addr, (int)cur->ai_addrlen ) == 0 ) {
            DBG("connect( client->socket, cur->ai_addr, (int)cur->ai_addrlen ) == 0 ");
            ret = 0;
            break;
        }

        close( client->socket );
        ret = HTTPCLIENT_ERROR_CONN;
    }

    freeaddrinfo( addr_list );

    return ret;
}


int httpclient_parse_url(const char *url, char *scheme, size_t max_scheme_len, char *host, size_t maxhost_len, int *port, char *path, size_t max_path_len)
{
    char *scheme_ptr = (char *) url;
    char *host_ptr = (char *) strstr(url, "://");
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
        if ( max_scheme_len < host_ptr - scheme_ptr + 1 ) { /* including NULL-terminating char */
            WARN("Scheme str is too small (%d >= %d)", max_scheme_len, host_ptr - scheme_ptr + 1);
            return HTTPCLIENT_ERROR_PARSE;
        }
        memcpy(scheme, scheme_ptr, host_ptr - scheme_ptr);
        scheme[host_ptr - scheme_ptr] = '\0';

        host_ptr += 3;
    }


    port_ptr = strchr(host_ptr, ':');
    if ( port_ptr != NULL ) {
        uint16_t tport;
        host_len = port_ptr - host_ptr;
        port_ptr++;
        if ( sscanf(port_ptr, "%hu", &tport) != 1) {
            WARN("Could not find port");
            return HTTPCLIENT_ERROR_PARSE;
        }
        *port = (int)tport;
    } else {
        *port = 0;
    }
    path_ptr = strchr(host_ptr, '/');
    //if (path_ptr == NULL) {
    //    return HTTPCLIENT_ERROR_PARSE;
    //}
    if ( host_len == 0 ) {
        host_len = path_ptr - host_ptr;
    }

    if ( maxhost_len < host_len + 1 ) { /* including NULL-terminating char */
        WARN("Host str is too small (%d >= %d)", maxhost_len, host_len + 1);
        return HTTPCLIENT_ERROR_PARSE;
    }
    memcpy(host, host_ptr, host_len);
    host[host_len] = '\0';

    fragment_ptr = strchr(host_ptr, '#');
    if (fragment_ptr != NULL) {
        path_len = fragment_ptr - path_ptr;
    } else {
        if (path_ptr != NULL) {
            path_len = strlen(path_ptr);
        } else {
            path_len = 0;
        }
    }

    if ( max_path_len < path_len + 1 ) { /* including NULL-terminating char */
        WARN("Path str is too small (%d >= %d)", max_path_len, path_len + 1);
        return HTTPCLIENT_ERROR_PARSE;
    }

    if (path_ptr != NULL && path_len > 0) {
        memcpy(path, path_ptr, path_len);
    }
    path[path_len] = '\0';

    return HTTPCLIENT_OK;
}


int httpclient_get_info(httpclient_t *client, char *send_buf, int *send_idx, char *buf, size_t len)   /* 0 on success, err code on failure */
{
    int ret ;
    int cp_len ;
    int idx = *send_idx;

    if (len == 0) {
        len = strlen(buf);
    }

    do {
        if ((HTTPCLIENT_SEND_BUF_SIZE - idx) >= len) {
            cp_len = len ;
        } else {
            cp_len = HTTPCLIENT_SEND_BUF_SIZE - idx ;
        }

        memcpy(send_buf + idx, buf, cp_len) ;
        idx += cp_len ;
        len -= cp_len ;

        if (idx == HTTPCLIENT_SEND_BUF_SIZE) {
            if (client->is_http == false) {
                ERR("send buffer overflow");
                return HTTPCLIENT_ERROR ;
            }
            ret = httpclient_tcp_send_all(client->socket, send_buf, HTTPCLIENT_SEND_BUF_SIZE) ;
            if (ret) {
                return (ret) ;
            }
        }
    } while (len) ;

    *send_idx = idx;
    return HTTPCLIENT_OK ;
}

void httpclient_set_custom_header(httpclient_t *client, char *header)
{
    client->header = header ;
}

int httpclient_basic_auth(httpclient_t *client, char *user, char *password)
{
    if ((strlen(user) + strlen(password)) >= HTTPCLIENT_AUTHB_SIZE) {
        return HTTPCLIENT_ERROR ;
    }
    client->auth_user = user;
    client->auth_password = password;
    return HTTPCLIENT_OK ;
}

int httpclient_send_auth(httpclient_t *client, char *send_buf, int *send_idx)
{
    char b_auth[(int)((HTTPCLIENT_AUTHB_SIZE + 3) * 4 / 3 + 3)] ;
    char base64buff[HTTPCLIENT_AUTHB_SIZE + 3] ;

    httpclient_get_info(client, send_buf, send_idx, "Authorization: Basic ", 0) ;
    sprintf(base64buff, "%s:%s", client->auth_user, client->auth_password) ;
    DBG("bAuth: %s", base64buff) ;
    httpclient_base64enc(b_auth, base64buff) ;
    b_auth[strlen(b_auth) + 2] = '\0' ;
    b_auth[strlen(b_auth) + 1] = '\n' ;
    b_auth[strlen(b_auth)] = '\r' ;
    DBG("b_auth:%s", b_auth) ;
    httpclient_get_info(client, send_buf, send_idx, b_auth, 0) ;
    return HTTPCLIENT_OK ;
}


int httpclient_tcp_send_all(int sock_fd, char *data, int length)
{
    int written_len = 0;

    while (written_len < length) {
        int ret = send(sock_fd, data + written_len, length - written_len, 0);
        if (ret > 0) {
            written_len += ret;
            continue;
        } else if (ret == 0) {
            return written_len;
        } else {
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
    if (res != HTTPCLIENT_OK) {
        ERR("httpclient_parse_url returned %d", res);
        return res;
    }

    /* Send request */
    memset(send_buf, 0, HTTPCLIENT_SEND_BUF_SIZE);
    len = 0 ; /* Reset send buffer */
    client->method = method;
    
    if( strlen(path) == 0 ) {
        snprintf(buf, sizeof(buf), "%s /%s HTTP/1.1\r\nHost: %s\r\n", meth, path, host); /* Write request */
    }
    else {
        snprintf(buf, sizeof(buf), "%s %s HTTP/1.1\r\nHost: %s\r\n", meth, path, host); /* Write request */
    }

    ret = httpclient_get_info(client, send_buf, &len, buf, strlen(buf));
    if (ret) {
        ERR("Could not write request");
        return HTTPCLIENT_ERROR_CONN;
    }

    /* Send all headers */
    if (client->auth_user) {
        httpclient_send_auth(client, send_buf, &len) ; /* send out Basic Auth header */
    }

    /* Add user header information */
    if (client->header) {
        httpclient_get_info(client, send_buf, &len, (char *)client->header, strlen(client->header));
    }

    if ( client_data->post_buf != NULL ) {
        snprintf(buf, sizeof(buf), "Content-Length: %d\r\n", client_data->post_buf_len);
        httpclient_get_info(client, send_buf, &len, buf, strlen(buf));

        if (client_data->post_content_type != NULL)  {
            snprintf(buf, sizeof(buf), "Content-Type: %s\r\n", client_data->post_content_type);
            httpclient_get_info(client, send_buf, &len, buf, strlen(buf));
        }
    }

    /* Close headers */
    httpclient_get_info(client, send_buf, &len, "\r\n", 0);

    DBG("Trying to write %d bytes http header:%s", len, send_buf);

    ret = httpclient_tcp_send_all(client->socket, send_buf, len);
    if (ret > 0) {
        DBG("Written %d bytes, socket = %d", ret, client->socket);
    } else if ( ret == 0 ) {
        WARN("ret == 0,Connection was closed by server");
        return HTTPCLIENT_CLOSED; /* Connection was closed by server */
    } else {
        ERR("Connection error (send returned %d)", ret);
        return HTTPCLIENT_ERROR_CONN;
    }

    return HTTPCLIENT_OK;
}

int httpclient_send_userdata(httpclient_t *client, httpclient_data_t *client_data)
{
    int ret = 0;
    

    if (client_data->post_buf && client_data->post_buf_len) {
        DBG("==client_data->post_buf:%s", client_data->post_buf);
        {
            ret = httpclient_tcp_send_all(client->socket, client_data->post_buf, client_data->post_buf_len);
            if (ret > 0) {
                DBG("==Written %d bytes", ret);
            } else if ( ret == 0 ) {
                WARN("ret == 0,Connection was closed by server");
                return HTTPCLIENT_CLOSED; /* Connection was closed by server */
            } else {
                ERR("==Connection error (send returned %d)", ret);
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

    while (readLen < max_len) {
        buf[readLen] = '\0';
        if (client->is_http) {
        #if 0
            if (readLen < min_len) {
                ret = recv(client->socket, buf + readLen, min_len - readLen, 0);
                DBG("recv [blocking] return:%d", ret);
                if (ret == 0) {
                    DBG("recv [blocking] return 0 may disconnected");
                    ret = HTTPCLIENT_CLOSED;
                }
            } else {
                ret = recv(client->socket, buf + readLen, max_len - readLen, MSG_DONTWAIT);
                DBG("recv [not blocking] return:%d", ret);
                if (ret == -1 && errno == EWOULDBLOCK) {
                    DBG("recv [not blocking] EWOULDBLOCK");
                    break;
                }
            }
        #else
            ret = recv(client->socket, buf + readLen, max_len - readLen, 0);
        #endif
        }

        if (ret > 0) {
            readLen += ret;
        } else if (ret == 0) {
            break;
        } else if (ret == HTTPCLIENT_CLOSED) {
            return HTTPCLIENT_CLOSED;
        } else {
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
	if(remain_len > 0 && remain_len < 512)
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

    if (client_data->response_content_len == -1 && client_data->is_chunked == false) {
        while(true)
        {
            int ret, max_len;
            if (count + len < client_data->response_buf_len - 1) {
                memcpy(client_data->response_buf + count, data, len);
                count += len;
                client_data->response_buf[count] = '\0';
            } else {
                memcpy(client_data->response_buf + count, data, client_data->response_buf_len - 1 - count);
                client_data->response_buf[client_data->response_buf_len - 1] = '\0';
                client_data->content_block_len = client_data->response_buf_len - 1;
                return HTTPCLIENT_RETRIEVE_MORE_DATA;
            }

            max_len = MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count);
            ret = httpclient_recv(client, data, 1, max_len, &len);

            /* Receive data */
            DBG("data len: %d %d", len, count);

            if (ret <= HTTPCLIENT_ERROR_CONN) {
                DBG("ret == HTTPCLIENT_ERROR_CONN");
                client_data->content_block_len = count;
                client_data->is_more = false;
                return ret;
            }

            if (len == 0) {/* read no more data */
                DBG("no more len == 0");
                client_data->is_more = false;
                return HTTPCLIENT_OK;
            }
        }
    }

    while (true) {
        size_t readLen = 0;

        if ( client_data->is_chunked && client_data->retrieve_len <= 0) {
            /* Read chunk header */
            bool foundCrlf;
            int n;
            do {               
                DBG("len: %d", len);
                foundCrlf = false;
                crlf_pos = 0;
                data[len] = 0;
                if (len >= 2) {
                    for (; crlf_pos < len - 2; crlf_pos++) {
                        if ( data[crlf_pos] == '\r' && data[crlf_pos + 1] == '\n' ) {
                            foundCrlf = true;
                            break;
                        }
                    }
                }
                if (!foundCrlf) { /* Try to read more */
                    int max_len = MIN(HTTPCLIENT_CHUNK_SIZE - len - 1, client_data->response_buf_len - 1 - count);
                    if ( len < max_len ) {
                        int new_trf_len, ret;
                        ret = httpclient_recv(client, data + len, 0, max_len, &new_trf_len);
                        len += new_trf_len;
                        if (ret <= HTTPCLIENT_ERROR_CONN) {
                            return ret;
                        } else {
                            LOS_TaskDelay(1000);
                            continue;
                        }
                    } else {
                        return HTTPCLIENT_ERROR;
                    }
                }
            } while (!foundCrlf);
            data[crlf_pos] = '\0';
            n = sscanf(data, "%x", &readLen);/* chunk length */
            DBG("chunked size: %d", readLen);
            client_data->retrieve_len = readLen;
            client_data->response_content_len += client_data->retrieve_len;
            if (n != 1) {
                DBG("Could not read chunk length");
                return HTTPCLIENT_ERROR_PRTCL;
            }

            memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2)); /* Not need to move NULL-terminating char any more */
            len -= (crlf_pos + 2);

            if ( readLen == 0 ) {
                /* Last chunk */
                client_data->is_more = false;
                DBG("no more (last chunk)");
                break;
            }
        } else {
            readLen = client_data->retrieve_len;
        }

        DBG("Retrieving %d bytes, len:%d", readLen, len);

        do {            
            DBG("readLen %d, len:%d", readLen, len);
            templen = MIN(len, readLen);
            if (count + templen < client_data->response_buf_len - 1) {
                memcpy(client_data->response_buf + count, data, templen);
                count += templen;
                client_data->response_buf[count] = '\0';
                client_data->retrieve_len -= templen;
            } else {
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
                if (readLen > len || remain_len) {
                    client_data->content_block_len = client_data->response_buf_len - 1;
                    return HTTPCLIENT_RETRIEVE_MORE_DATA;
                } else {
                    count += templen;
                }
            }

            // if ( len > readLen ) {            
            if ( len >= readLen ) {
                DBG("memmove %d %d %d", readLen, len, client_data->retrieve_len);
                memmove(data, &data[readLen], len - readLen); /* chunk case, read between two chunks */
                len -= readLen;
                readLen = 0;
                client_data->retrieve_len = 0;
            } else {
                readLen -= len;
            }
            
            if (readLen) {
                int ret;
                // int max_len = MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count);                
                int max_len = MIN(MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count), readLen);
                ret = httpclient_recv(client, data, 1, max_len, &len);
                if (ret <= HTTPCLIENT_ERROR_CONN) {
                    return ret;
                }
            }
        } while (readLen);

        if ( client_data->is_chunked ) {
            if (len < 2) {
                int new_trf_len, ret;
                int max_len = MIN(HTTPCLIENT_CHUNK_SIZE - len - 1, client_data->response_buf_len - 1 - count);
                /* Read missing chars to find end of chunk */
                ret = httpclient_recv(client, data + len, 2 - len, max_len, &new_trf_len);
                if (ret <= HTTPCLIENT_ERROR_CONN) {
                    return ret;
                }
                len += new_trf_len;
            }
            if ( (data[0] != '\r') || (data[1] != '\n') ) {
                DBG("Format error, %s", data); /* after memmove, the beginning of next chunk */
                return HTTPCLIENT_ERROR_PRTCL;
            }
            memmove(data, &data[2], len - 2); /* remove the \r\n */
            len -= 2;
        } else {
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
    if (crlf_ptr == NULL) {
        DBG("\r\n not found");
        return HTTPCLIENT_ERROR_PRTCL;
    }

    crlf_pos = crlf_ptr - data;
    data[crlf_pos] = '\0';

    /* Parse HTTP response */
    if ( sscanf(data, "HTTP/%*d.%*d %d %*[^\r\n]", &(client->response_code)) != 1 ) {
        /* Cannot match string, error */
        DBG("Not a correct HTTP answer : %s", data);
        return HTTPCLIENT_ERROR_PRTCL;
    }

    if ( (client->response_code < 200) || (client->response_code >= 400) ) {
        /* Did not return a 2xx code; TODO fetch headers/(&data?) anyway and implement a mean of writing/reading headers */
        WARN("Response code %d", client->response_code);
    }

    DBG("Reading headers%s", data);

    memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2) + 1); /* Be sure to move NULL-terminating char as well */
    len -= (crlf_pos + 2);

    client_data->is_chunked = false;

    /* Now get headers */
    while ( true ) {
        char *colon_ptr, *key_ptr, *value_ptr;
        int key_len, value_len;
        
        crlf_ptr = strstr(data, "\r\n");
        if (crlf_ptr == NULL) {
            if ( len < HTTPCLIENT_CHUNK_SIZE - 1 ) {
                int new_trf_len, ret;
                ret = httpclient_recv(client, data + len, 1, HTTPCLIENT_CHUNK_SIZE - len - 1, &new_trf_len);
                len += new_trf_len;
                data[len] = '\0';
                DBG("Read %d chars; In buf: [%s]", new_trf_len, data);
                if (ret <= HTTPCLIENT_ERROR_CONN) {
                    return ret;
                } else {
                    continue;
                }
            } else {
                DBG("header len > chunksize");
                return HTTPCLIENT_ERROR;
            }
        }

        crlf_pos = crlf_ptr - data;        
        if (crlf_pos == 0) { /* End of headers */
            memmove(data, &data[2], len - 2 + 1); /* Be sure to move NULL-terminating char as well */
            len -= 2;
            break;
        }
        
        colon_ptr = strstr(data, ": ");        
        if (colon_ptr) {             
            if (header_buf_len >= crlf_pos + 2) {
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
            if (0 == strncasecmp(key_ptr, "Content-Length", key_len)) {
                sscanf(value_ptr, "%d[^\r]", &(client_data->response_content_len));                
                client_data->retrieve_len = client_data->response_content_len;
            } else if (0 == strncasecmp(key_ptr, "Transfer-Encoding", key_len)) {
                if (0 == strncasecmp(value_ptr, "Chunked", value_len)) {
                    client_data->is_chunked = true;
                    client_data->response_content_len = 0;
                    client_data->retrieve_len = 0;
                }
            } 
           
            memmove(data, &data[crlf_pos + 2], len - (crlf_pos + 2) + 1); /* Be sure to move NULL-terminating char as well */
            len -= (crlf_pos + 2);
        } else {
            ERR("Could not parse header");
            return HTTPCLIENT_ERROR;
        }
    }

    if (client->method == HTTPCLIENT_HEAD) {
        /* no content if it's HEAD method. */
        return HTTPCLIENT_OK;
    } else {
        return httpclient_retrieve_content(client, data, len, client_data);
    }
}


HTTPCLIENT_RESULT httpclient_connect(httpclient_t *client, char *url)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    char host[HTTPCLIENT_MAX_HOST_LEN] = {0};
    char scheme[8] = {0};
    char path[HTTPCLIENT_MAX_URL_LEN] = {0};
    DBG("url==>%s",url);
   #if 1
    /* First we need to parse the url (http[s]://host[:port][/[path]]) */
    int res = httpclient_parse_url(url, scheme, sizeof(scheme), host, sizeof(host), &(client->remote_port), path, sizeof(path));
    if (res != HTTPCLIENT_OK) {
        ERR("httpclient_parse_url returned %d", res);
        return (HTTPCLIENT_RESULT)res;
    }

    // http or https
    if (strcmp(scheme, "https") == 0) 
        client->is_http = false;
    else if (strcmp(scheme, "http") == 0)
        client->is_http = true;

    // default http 80 port, https 443 port
    if (client->remote_port == 0) {
        if (client->is_http) { 
            client->remote_port = HTTP_PORT;
        } else {
            client->remote_port = HTTPS_PORT;
        }
    }
    DBG("http?:%d, port:%d, host:%s", client->is_http, client->remote_port, host);

    client->socket = -1;
    if (client->is_http)    
        ret = httpclient_conn(client, host);
#endif
    DBG("httpclient_connect() result:%d, client:%p", ret, client);
    return (HTTPCLIENT_RESULT)ret;
}

HTTPCLIENT_RESULT httpclient_send_request(httpclient_t *client, char *url, int method, httpclient_data_t *client_data)
{
    int ret = HTTPCLIENT_ERROR_CONN;

    if (client->socket < 0) {
        return (HTTPCLIENT_RESULT)ret;
    }
    ret = httpclient_send_header(client, url, method, client_data);
    if (ret != 0) {
        return (HTTPCLIENT_RESULT)ret;
    }

    if (method == HTTPCLIENT_POST || method == HTTPCLIENT_PUT) {
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
    DBG("http start recv response");
    if (client->socket < 0) {
        return (HTTPCLIENT_RESULT)ret;
    }

    if (client_data->is_more) {
        DBG("==http start retrieve data");
        client_data->response_buf[0] = '\0';
        ret = httpclient_retrieve_content(client, buf, reclen, client_data);
    } else {
        DBG("==http enter recv");
        ret = httpclient_recv(client, buf, 1, HTTPCLIENT_CHUNK_SIZE - 1, &reclen);
        if (ret != 0) {
            return (HTTPCLIENT_RESULT)ret;
        }

        buf[reclen] = '\0';

        if (reclen) {
            DBG("===reclen:%d, buf:%s", reclen, buf);
            ret = httpclient_response_parse(client, buf, reclen, client_data);
        }
    }

    DBG("==httpclient_recv_response() result:%d, client:%p", ret, client);
    return (HTTPCLIENT_RESULT)ret;
}

void httpclient_close(httpclient_t *client)
{
    if (client->is_http) {
        if (client->socket >= 0)
            close(client->socket);
    }

    client->socket = -1;
    DBG("httpclient_close() client:%p", client);
}

int httpclient_get_response_code(httpclient_t *client)
{
    return client->response_code;
}

static HTTPCLIENT_RESULT httpclient_common(httpclient_t *client, char *url, int method, httpclient_data_t *client_data)
{
    HTTPCLIENT_RESULT ret = HTTPCLIENT_ERROR_CONN;

    DBG("http connect");
    ret = httpclient_connect(client, url);
    if (!ret) {
        DBG("http send request");
        ret = httpclient_send_request(client, url, method, client_data);
        DBG("http send request,ret=%d",ret);
        if (!ret) {
            DBG("recv response");
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
    while (true) {
        crlf_ptr = strstr(data, "\r\n");
        colon_ptr = strstr(data, ": ");        
        if (colon_ptr) {                         
            key_len = colon_ptr - data;
            value_len = crlf_ptr - colon_ptr - strlen(": ");            
            key_ptr = data;
            value_ptr = colon_ptr + strlen(": ");
            
            DBG("Response header: %.*s: %.*s", key_len, key_ptr, value_len, value_ptr);
            if (0 == strncasecmp(key_ptr, name, key_len)) {
                *val_pos = value_ptr - header_buf;
                *val_len = value_len;
                return 0;
            } else {                 
                data = crlf_ptr + 2;
                continue;
            }
        } else 
        {
            
            return -1;
        }
            
            
    }
}

