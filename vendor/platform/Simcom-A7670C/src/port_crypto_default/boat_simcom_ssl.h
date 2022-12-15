
#ifndef _BOAT_SIMCOM_SSL_H_
#define _BOAT_SIMCOM_SSL_H_
#include "simcom_os.h"

typedef struct{
    INT32 err;//0:SUCCESS -1:FAIL
    UINT8  ssl_version;
    UINT8  ca_cert_path[64];
    UINT8  client_cert_path[64];
    UINT8  client_key_path[64];
    UINT8  enable_SNI;
    UINT8  auth_mode;
    UINT8  ignore_local_time;
    UINT16 negotiate_time;
}SCsslContent;

SCsslContent sAPI_SslGetContextIdMsg(int sslId);
//int sAPI_SslGetContextIdMsg(int sslId,SCsslContent *sslContext);
int sAPI_SslSetContextIdMsg(char *op,int sslId,char *value);

typedef enum{
    SC_SSL_COMMUNICATE          = 1,
    SC_SSL_TEST                 = 2,
    SC_SSL_DEMO_MAX             = 99
}SC_SSL_DEMO_TYPE;


/**********************************************************************/
#define SC_MAX_SSL_SESSION_NUM 2

typedef struct sc_ssl_ctx_s sc_ssl_ctx_t;

typedef enum
{
  SC_SSL_CFG_VERIFY_MODE_NONE,                      /* not verify the server */
  SC_SSL_CFG_VERIFY_MODE_REQUIRED,                  /* verify server */
  SC_SSL_CFG_VERIFY_MODE_SERVER_CLIENT,             /* Two-way authentication */
  SC_SSL_CFG_VERIFY_MODE_CLIENT                     /* not verify the server and verify the client */
} SC_SSL_CFG_TYPE_VERIFY_MODE;

typedef enum
{
  SC_SSL_CFG_VERSION_SSL30 = 0,                     /* ssl 3.0 */
  SC_SSL_CFG_VERSION_TLS10,                         /* tls 1.0 */
  SC_SSL_CFG_VERSION_TLS11,                         /* tls 1.1 */
  SC_SSL_CFG_VERSION_TLS12,                         /* tls 1.2 */
  SC_SSL_CFG_VERSION_ALL                            /* all */
} SC_SSL_CFG_TYPE_VERSION;


typedef struct SCSslCtx_s {
    UINT32 ClientId;
    UINT32 ciphersuitesetflg;/*0---use default ciphersuite,other use ciphersuite  */
    INT32  ciphersuite[8];
    /* ssl_version: 0---SLL3.0; 1---TLS1.0; 2----TLS1.1; 3----TLS1.2; 4----ALL; */
    UINT8  ssl_version;
    UINT8  enable_SNI;
    /*
    * authmode
    * 0: no authentication;
    * 1: manage server authentication;
    * 2: manage server and client authentication if requested by the remote server
    * 3: client authentication and no server authentication. It needs the cert and key of the client
    */
    UINT8  auth_mode;
    /* 0: care about tiem check for certification; 1:ignore tiem check for certification */
    UINT8  ignore_local_time;
    /* indicates max timeout used in SSL negotiate stage,10-300 sec,default :300 sec */
    INT8 *ipstr;

    INT8 *root_ca;
    UINT32 root_ca_len;
    INT8 *client_cert;
    UINT32 client_cert_len;
    INT8 *client_key;
    UINT32 client_key_len;
    /*Device with PSK*/
    INT8 *psk;
    UINT32 psk_len;
    INT8 *psk_id;
    UINT32 psk_id_len;
    UINT32 fd;
    UINT32 timeout_ms;
    UINT16 negotiate_time;


}SCSslCtx_t;

#define SSL_NON_ASCII_PREFIX                 "{non-ascii}"
#define SSL_MAX_STRING_LEN 128
#define SSL_RESPONSE_BUFFER_LEN 512
#define MAX_MSSL_FILENAME_LEN 121
#define SC_SSL_WANT_READ       -0x6900  /**< No data of requested type currently available on underlying transport. */
#define SC_SSL_WANT_WRITE      -0x6880  /**< Connection requires a write call. */
#define SC_SSL_TIMEOUT         -0x6800  /**< The operation timed out. */
#define SC_SSL_NET_CON_RESET   -0x0050  /**< Connection was reset by peer. */

#define SC_SSL_NET_RECV_FAILED -0x004C  /**< Reading information from the socket failed. */

#define SC_SSL_NET_SEND_FAILED  -0x004E  /**< Sending information through the socket failed. */

INT32 sAPI_SslHandShake(SCSslCtx_t *ctx);
INT32 sAPI_SslRead(UINT32 ClientID,INT8 *buf, INT32 len);
INT32 sAPI_SslSend(UINT32 ClientID,INT8 *buf, INT32 len);
void sAPI_SslClose(UINT32 ClientID);

typedef struct sc_ssl_ctx_s sc_ssl_ctx_t;

#endif
