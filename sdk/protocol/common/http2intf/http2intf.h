/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
/*!@brief http2 interface header file

@file
http2intf.h is the header file for http2 interface.
*/

#ifndef __HTTP2INTF_H__
#define __HTTP2INTF_H__

/* self header include */
#include "boatinternal.h"
/* nghttp2 header include */
#include <nghttp2/nghttp2.h>

/*! @defgroup http2 communication-http2 
 * @{
 */

#define BOAT_TLS_SUPPORT                1 //!< If need client support TLS, set it to 1.
#define BOAT_TLS_IDENTIFY_CLIENT        0 //!< If server need identify client, set it to 1.

#define BOAT_HTTP2_SEND_BUF_MAX_LEN     8192 //!< The maximum length of HTTP2 send buffer

typedef struct Thttp2Response
{
	BUINT32								httpResLen;
	BUINT8 								*http2Res;
}Http2Response;


#if (BOAT_TLS_SUPPORT == 1) 	
//!@brief TLS releated structure 
//!@note this structure only suitable for mbedtls, if use other TLS libiary,
//! please modify this specific structure
	typedef struct TTLSContext{	
		void*       ssl;     //!< mbedtls_ssl_context if mbedTLS
		void*       ssl_cfg; //!< mbedtls_ssl_config if mbedTLS
		void*       ssl_net; //!< mbedtls_net_context if mbedTLS
		void*       ssl_crt; //!< mbedtls_x509_crt if mbedTLS
	}TTLSContext;
#endif
	
//!@brief http2 releated structure
typedef struct Thttp2IntfContext{
	nghttp2_session     *session;     //!< nghttp2 session
	BCHAR               *nodeUrl;     //!< example: orderer.example.com:7050
#if (BOAT_TLS_SUPPORT == 1)
	BCHAR               *hostName;    //!< CN field in remote certificate. example: orderer.example.com
	BoatFieldVariable   *tlsCAchain;  //!< rootCA certificate content list
	
	void*               tlsContext;   //!< TLS releated context, it has a strong correlation
                                      //!< with specific TLS libraries
#endif	
	BoatFieldVariable   sendBuf;      //!< http2 send buffer.NOTE:there is no receive buffer here,
	                                  //!< cause the receive buffer be maintained by nghttp2 lib.
	
	BSINT32             sockfd;       //!< network connection file desc

	BUINT8                type;   //!< this field will determine http2 head content
	BCHAR               *pathTmp; 
	void*               parseDataPtr; //!< the http2 parsed data structure
}http2IntfContext;


#ifdef __cplusplus
extern "C" {
#endif

/*!****************************************************************************
 * @brief http2context initialization
 * 
 * @details
 *   Allocate memory for http2Context structure and it's member, set pointer as NULL
 *   and integer as 0.
 *
 * @return 
 *   Return the address of http2IntfContext structure if initialization success, 
 *   otherwise return NULL.
 ******************************************************************************/
http2IntfContext *http2Init(void);


/*!****************************************************************************
 * @brief http2context de-initialization
 * 
 * @details
 *   free memory of http2Context structure's member and itself, set pointer as NULL
 *   and integer as 0.
 *
 * @param http2Context 
 *   http2Context structure to be de-initialization.
 ******************************************************************************/
void http2DeInit(http2IntfContext *http2Context);


/*!****************************************************************************
 * @brief send http2 request frame and handle respond frame.
 * 
 * @param context 
 *   http2 context of submit request
 *
 * @return 
 *   Return \c BOAT_SUCCESS if submit success, otherwise return failed code.
 ******************************************************************************/
BOAT_RESULT http2SubmitRequest(http2IntfContext *context);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
