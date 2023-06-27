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

#ifdef CM_HTTPCLIENT_SSL_ENABLE
#include "wolfssl/openssl/crypto.h"
#include "wolfssl/openssl/ssl.h"
#include "wolfssl/openssl/err.h"
#include "wolfssl/openssl/rand.h"
#include "wolfssl/internal.h"
#endif



//#define CM_HTTPCLIENT_SSL_ENABLE
/*
#ifdef CM_HTTPCLIENT_SSL_ENABLE

#endif
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
* @}
*/
 /**

 * @defgroup HTTP_FUNCTIONS
 * 
   HTTP API，请参照SDK中示例文件使用
 *@{
 */

 
 /**
 *  \brief 执行Get方法，阻塞直到完成
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] client_data 接收数据的对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_get(httpclient_t *client, char *url, httpclient_data_t *client_data);

 /**
 *  \brief 执行POST方法，阻塞直到完成
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] client_data 接收数据的对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_post(httpclient_t *client, char *url, httpclient_data_t *client_data);

 /**
 *  \brief 执行PUT方法，阻塞直到完成
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] client_data 接收数据对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_put(httpclient_t *client, char *url, httpclient_data_t *client_data);
 
 /**
 *  \brief 执行Delete方法，阻塞直到完成
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] client_data 客户端对象数据指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_delete(httpclient_t *client, char *url, httpclient_data_t *client_data);
 /**
 *  \brief HTTP连接
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_connect(httpclient_t *client, char *url);

 /**
 *  \brief 发送请求(GET或POST)
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] method 方法
 *  \param [in] client_data 要发送的数据对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_send_request(httpclient_t *client, char *url, int method, httpclient_data_t *client_data);

 /**
 *  \brief 接收服务器返回
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] client_data 客户端数据对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_recv_response(httpclient_t *client, httpclient_data_t *client_data);
 /**
 *  \brief 关闭HTTP连接
 *  
 *  \param [in] client 客户端对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
void httpclient_close(httpclient_t *client);
 /**
 *  \brief 获取返回码
 *  
 *  \param [in] client 客户端对象指针
 *  \return 返回码
 *  
 *  \details More details
 */
int httpclient_get_response_code(httpclient_t *client);
 /**
 *  \brief 设置头部
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] header 头部字符串
 *  \return 空
 *  
 *  \details More details
 */
void httpclient_set_custom_header(httpclient_t *client, char *header);
 /**
 *  \brief 获取返回头部值
 *  
 *  \param [in] header_buf 返回头部指针
 *  \param [in] name 返回头部名称
 *  \param [in] val_pos 返回头部中头部值的位置
 *  \param [in] val_len 头部值长度
 *  \return 0：正确 其他：错误 
 *  
 *  \details More details
 */
int httpclient_get_response_header_value(char *header_buf, char *name, int *val_pos, int *val_len);
/** @} */ 
#define HTTPCLIENT_DEBUG 0



#endif /* __HTTPCLIENT_H__ */


