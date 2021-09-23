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
#include "lwip/sockets.h"

// #define MTK_HTTPCLIENT_SSL_ENABLE
#ifdef MTK_HTTPCLIENT_SSL_ENABLE
//#include "mbedtls/compat-1.3.h"
#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/certs.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup HttpClient
 * @{
 * HttpClient API implements the client-side of HTTP/1.1. It provides base interfaces to execute an HTTP request on a given URL. It also supports HTTPS (HTTP over TLS) to provide secure communication.\n
 * @section HttpClient_Usage_Chapter How to use this module
 * In this release, MediaTek provides two types of APIs: high level APIs and low level APIs.\n
 * - \b The \b high \b level \b APIs
 *  - Enables to execute a single HTTP request on a given URL.
 *  - Call #httpclient_get(), #httpclient_post(), #httpclient_put() or #httpclient_delete() to get, post, put or delete and HTTP request.\n
 * - \b The \b low \b level \b APIs
 *  - Enables to execute more than one HTTP requests during a Keep-Alive connection. Keep-alive is the idea of using a single TCP connection to send and receive multiple HTTP requests/responses, as opposed to opening a new connection for every single request/response pair.
 *  - Step1: Call #httpclient_connect() to connect to a remote server.
 *  - Step2: Call #httpclient_send_request() to send an HTTP request to the server.
 *  - Step3: Call #httpclient_recv_response() to receive an HTTP response from the server.
 *  - Step4: Repeat Steps 2 and 3 to execute more requests.
 *  - Step5: Call #httpclient_close() to close the connection.
 */

/** @defgroup httpclient_define Define
  * @{
  */
/** @brief   This macro defines the deault HTTP port.  */
#define HTTP_PORT   80

/** @brief   This macro defines the deault HTTPS port.  */
#define HTTPS_PORT 443

/**
 * @}
 */

/** @defgroup httpclient_enum Enum
 *  @{
 */
/** @brief   This enumeration defines the HTTP request type.  */
typedef enum {
    HTTPCLIENT_GET,
    HTTPCLIENT_POST,
    HTTPCLIENT_PUT,
    HTTPCLIENT_DELETE,
    HTTPCLIENT_HEAD
} HTTPCLIENT_REQUEST_TYPE;

/** @brief   This enumeration defines the API return type.  */
typedef enum {
    HTTPCLIENT_ERROR_PARSE = -6,           /**< A URL parse error occurred. */
    HTTPCLIENT_UNRESOLVED_DNS = -5,        /**< Could not resolve the hostname. */
    HTTPCLIENT_ERROR_PRTCL = -4,           /**< A protocol error occurred. */
    HTTPCLIENT_ERROR = -3,                 /**< An unknown error occurred. */
    HTTPCLIENT_CLOSED = -2,                /**< Connection was closed by a remote host. */
    HTTPCLIENT_ERROR_CONN = -1,            /**< Connection failed. */
    HTTPCLIENT_OK = 0,                     /**< The operation was successful. */
    HTTPCLIENT_RETRIEVE_MORE_DATA = 1      /**< More data needs to be retrieved. */
} HTTPCLIENT_RESULT;
/**
 * @}
 */

/** @defgroup httpclient_struct Struct
  * @{
  */
/** @brief   This structure defines the httpclient_t structure.  */
typedef struct {
    int socket;                     /**< Socket ID. */
    int remote_port;                /**< HTTP or HTTPS port. */
    int response_code;              /**< Response code. */
    char *header;                   /**< Request custom header. */
    char *auth_user;                /**< Username for basic authentication. */
    char *auth_password;            /**< Password for basic authentication. */
    bool is_http;                   /**< Http connection? if 1, http; if 0, https. */
    int method;                     /**< HTTP method, 0 ~ 4.  */
    unsigned int timeout_in_sec;    /**< HTTP socket recv timeout. */
#ifdef MTK_HTTPCLIENT_SSL_ENABLE
    const char *server_cert;        /**< Server certification. */
    const char *client_cert;        /**< Client certification. */
    const char *client_pk;          /**< Client private key. */
    int server_cert_len;            /**< Server certification lenght, server_cert buffer size. */
    int client_cert_len;            /**< Client certification lenght, client_cert buffer size. */
    int client_pk_len;              /**< Client private key lenght, client_pk buffer size. */
    void *ssl;                      /**< Ssl content. */
#endif
} httpclient_t;

/** @brief   This structure defines the HTTP data structure.  */
typedef struct {
    bool is_more;                /**< Indicates if more data needs to be retrieved. */
    bool is_chunked;             /**< Response data is encoded in portions/chunks.*/
    int retrieve_len;            /**< Content length to be retrieved. */
    int response_content_len;    /**< Response content length. */
    int content_block_len;       /**< The content length of one block. */
    int post_buf_len;            /**< Post data length. */
    int response_buf_len;        /**< Response body buffer length. */
    int header_buf_len;          /**< Response head buffer lehgth. */
    char *post_content_type;     /**< Content type of the post data. */
    char *post_buf;              /**< User data to be posted. */
    char *response_buf;          /**< Buffer to store the response body data. */
    char *header_buf;            /**< Buffer to store the response head data. */
    char *url;
    int method;
} httpclient_data_t;

/**
 * @}
 */

/**
 * @brief            This function executes a GET request on a given URL. It blocks until completion.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        url is the URL to run the request.
 * @param[in, out]   client_data is a pointer to the #httpclient_data_t instance to collect the data returned by the request.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 */
HTTPCLIENT_RESULT oc_httpclient_get(httpclient_t *client, char *url, httpclient_data_t *client_data);


/**
 * @brief            This function executes a POST request on a given URL. It blocks until completion.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        url is the URL to run the request.
 * @param[in, out]   client_data is a pointer to the #httpclient_data_t instance to collect the data returned by the request. It also contains the data to be posted.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 */
HTTPCLIENT_RESULT oc_httpclient_post(httpclient_t *client, char *url, httpclient_data_t *client_data);

/**
 * @brief            This function executes a PUT request on a given URL. It blocks until completion.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        url is the URL to run the request.
 * @param[in, out]   client_data is a pointer to the #httpclient_data_t instance to collect the data returned by the request. It also contains the data to be put.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 */
HTTPCLIENT_RESULT oc_httpclient_put(httpclient_t *client, char *url, httpclient_data_t *client_data);

/**
 * @brief            This function executes a DELETE request on a given URL. It blocks until completion.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        url is the URL to run the request.
 * @param[in, out]   client_data is a pointer to the #httpclient_data_t instance to collect the data returned by the request.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 */
HTTPCLIENT_RESULT oc_httpclient_delete(httpclient_t *client, char *url, httpclient_data_t *client_data);

/**
 * @brief            This function establishes an HTTP connection with the remote server by given URL.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        url is the URL to run the request.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 */
HTTPCLIENT_RESULT oc_httpclient_connect(httpclient_t *client, char *url);

/**
 * @brief            This function sends an HTTP(GET or POST) request to the given URL.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        url is the URL to run the request.
 * @param[in]        method is #HTTPCLIENT_REQUEST_TYPE.
 * @param[in]        client_data is a pointer to the #httpclient_data_t instance to collect the data to be posted.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 */
HTTPCLIENT_RESULT oc_httpclient_send_request(httpclient_t *client, char *url, int method, httpclient_data_t *client_data);

/**
 * @brief            This function receives the response from a server for the last request.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[out]       client_data is a pointer to the #httpclient_data_t instance to collect the data returned by the request.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 */
HTTPCLIENT_RESULT oc_httpclient_recv_response(httpclient_t *client, httpclient_data_t *client_data);

/**
 * @brief            This function closes the HTTP connection.
 * @param[in]        client is a pointer to the #httpclient_t.
 */
void oc_httpclient_close(httpclient_t *client);

/**
 * @brief            This function gets the HTTP response code assigned to the last request.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @return           The HTTP response code of the last request.
 */
int oc_httpclient_get_response_code(httpclient_t *client);

/**
 * @brief            This function sets a custom header.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        header is a custom header string.
 * @return           None.
 */
void oc_httpclient_set_custom_header(httpclient_t *client, char *header);

/**
 * @brief            This function get specified response header value.
 * @param[in]        header_buf is the response header buffer.
 * @param[in]        name is the specified http response header name.
 * @param[in, out]    val_pos is the position of header value in header_buf.
 * @param[in, out]    val_len is header value length.
 * @return           0, if value is got. Others, if errors occurred.
 */
int oc_httpclient_get_response_header_value(char *header_buf, char *name, int *val_pos, int *val_len);

#ifdef MTK_HTTPCLIENT_SSL_ENABLE
typedef struct {
    mbedtls_ssl_context ssl_ctx;        /* mbedtls ssl context */
    mbedtls_net_context net_ctx;        /* Fill in socket id */
    mbedtls_ssl_config ssl_conf;        /* SSL configuration */
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_x509_crt_profile profile;
    mbedtls_x509_crt cacert;
    mbedtls_x509_crt clicert;
    mbedtls_pk_context pkey;
} httpclient_ssl_t;
#endif

#ifdef __cplusplus
}
#endif

#endif /* __HTTPCLIENT_H__ */


