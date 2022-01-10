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

/*!@brief http2 interface

@file http2intf.c contains http2 interface functions.
*/

/* self header include */
#include "boatconfig.h"

#include "boatplatform_internal.h"
#include "http2intf.h"
/* protos header include */

//! xx
#define MAKE_NV(K, V)                                                                          \
	{                                                                                          \
		(BUINT8 *)K, (BUINT8 *)V, strlen((BCHAR *)K), strlen((BCHAR *)V), NGHTTP2_NV_FLAG_NONE \
	}

/*!****************************************************************************
 * @brief 
 * 
 * @param session 
 * @param data 
 * @param length 
 * @param flags 
 * @param user_data 
 * @return  
 ******************************************************************************/
__BOATSTATIC ssize_t send_callback(nghttp2_session *session, const uint8_t *data,
								   size_t length, int flags, void *user_data)
{
	BSINT32 sendLen;
	http2IntfContext *http2Context = (http2IntfContext *)user_data;

#if (BOAT_TLS_SUPPORT == 1)
	sendLen = BoatSend(http2Context->sockfd, http2Context->tlsContext, data, length, NULL);
#else
	sendLen = BoatSend(http2Context->sockfd, NULL, data, length, NULL);
#endif
	return sendLen;
}

/*!****************************************************************************
 * @brief 
 * 
 * @param session 
 * @param buf 
 * @param length 
 * @param flags 
 * @param user_data 
 * @return  
 ******************************************************************************/
__BOATSTATIC ssize_t recv_callback(nghttp2_session *session, uint8_t *buf,
								   size_t length, int flags, void *user_data)
{
	BSINT32 recvLen;
	http2IntfContext *http2Context = (http2IntfContext *)user_data;

#if (BOAT_TLS_SUPPORT == 1)
	recvLen = BoatRecv(http2Context->sockfd, http2Context->tlsContext, buf, length, NULL);
#else
	recvLen = BoatRecv(http2Context->sockfd, NULL, buf, length, NULL);
#endif
	if (recvLen > 0)
	{
		return recvLen;
	}

	return 0;
}

/*!****************************************************************************
 * @brief 
 * 
 * @param session 
 * @param frame 
 * @param user_data 
 * @return  
 ******************************************************************************/
__BOATSTATIC int on_frame_recv_callback(nghttp2_session *session,
										const nghttp2_frame *frame, void *user_data)
{

	switch (frame->hd.type)
	{
	case NGHTTP2_HEADERS:

		BoatLog(BOAT_LOG_VERBOSE, "[http2] NGHTTP2_HEADERS received. ");
		break;
	case NGHTTP2_DATA:
		BoatLog(BOAT_LOG_VERBOSE, "[http2] NGHTTP2_DATA received.");
		break;
	case NGHTTP2_SETTINGS:
		BoatLog(BOAT_LOG_VERBOSE, "[http2] NGHTTP2_SETTINGS received.");
		break;
	case NGHTTP2_RST_STREAM:
		BoatLog(BOAT_LOG_VERBOSE, "[http2] NGHTTP2_RST_STREAM received.");
		break;
	case NGHTTP2_GOAWAY:
		BoatLog(BOAT_LOG_VERBOSE, "[http2] NGHTTP2_GOAWAY received.");
		break;
	default:
		break;
	}

	return 0;
}

/*!****************************************************************************
 * @brief 
 * 
 * @param session 
 * @param flags 
 * @param stream_id 
 * @param data 
 * @param len 
 * @param user_data 
 * @return  
 ******************************************************************************/
__BOATSTATIC int on_data_chunk_recv_callback(nghttp2_session *session, uint8_t flags,
											 int32_t stream_id, const uint8_t *data,
											 size_t len, void *user_data)
{
	http2IntfContext *http2Context = (http2IntfContext *)user_data;
	// Protos__ProposalResponse *proposalResponse = NULL;
	// Orderer__SubmitResponse *submitResponse = NULL;
	Http2Response *parsePtr = NULL;
	uint8_t *temp = NULL;
	parsePtr = (Http2Response *)http2Context->parseDataPtr;
	if (parsePtr == NULL)
	{
		parsePtr = BoatMalloc(sizeof(Http2Response));
		parsePtr->httpResLen = 0;
	}
	else
	{
		if (parsePtr->httpResLen != 0)
		{
			temp = BoatMalloc(parsePtr->httpResLen);
			memcpy(temp, parsePtr->http2Res, parsePtr->httpResLen);
			if(parsePtr->http2Res != NULL){
			BoatFree(parsePtr->http2Res);
			}

		}
	}
	parsePtr->http2Res = BoatMalloc(parsePtr->httpResLen + len);
	if (parsePtr->httpResLen != 0)
	{
		memcpy(parsePtr->http2Res, temp, parsePtr->httpResLen);
		BoatFree(temp);
	}
	memcpy(parsePtr->http2Res + parsePtr->httpResLen, data, len);
	parsePtr->httpResLen += len;
	return 0;
}

/*!****************************************************************************
 * @brief 
 * 
 * @param session 
 * @param stream_id 
 * @param buf 
 * @param length 
 * @param data_flags 
 * @param source 
 * @param user_data 
 * @return  
 ******************************************************************************/
__BOATSTATIC ssize_t data_source_read_callback(nghttp2_session *session,
											   int32_t stream_id, uint8_t *buf, size_t length,
											   uint32_t *data_flags, nghttp2_data_source *source, void *user_data)
{
	http2IntfContext *http2Context = (http2IntfContext *)user_data;

	*data_flags |= NGHTTP2_DATA_FLAG_EOF;
	memcpy(buf, http2Context->sendBuf.field_ptr, http2Context->sendBuf.field_len);

	return http2Context->sendBuf.field_len;
}

/*!****************************************************************************
 * @brief 
 * 
 * @param session 
 * @param stream_id 
 * @param error_code 
 * @param user_data 
 * @return  
 ******************************************************************************/
__BOATSTATIC int on_stream_close_callback(nghttp2_session *session, int32_t stream_id,
										  uint32_t error_code, void *user_data)
{
	http2IntfContext *http2Context = (http2IntfContext *)user_data;
	nghttp2_session_terminate_session(session, 0);
#if (BOAT_TLS_SUPPORT == 1)
	BoatClose(http2Context->sockfd, http2Context->tlsContext, NULL);
#else
	BoatClose(http2Context->sockfd, NULL, NULL);
#endif
	return 0;
}

http2IntfContext *http2Init(void)
{
	http2IntfContext *http2Context = NULL;

	boat_try_declare;

	/* allocate memory for http2 structure */
	http2Context = BoatMalloc(sizeof(http2IntfContext));
	if (NULL == http2Context)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, http2Init_exception);
	}
	http2Context->session = NULL;
	http2Context->nodeUrl = NULL;
#if (BOAT_TLS_SUPPORT == 1)
	http2Context->hostName = NULL;
	http2Context->tlsCAchain = NULL;
	http2Context->tlsContext = BoatMalloc(sizeof(TTLSContext));
	if (http2Context->tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate TTLSContext.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, http2Init_exception);
	}
#endif
	http2Context->sendBuf.field_len = 0;
	http2Context->sendBuf.field_ptr = NULL;
	http2Context->sockfd = 0;
	http2Context->type = 0;
	http2Context->parseDataPtr = NULL;

	/* http2Context->nodeUrl initial */
	//DO NOTHING

	/* http2Context->sendBuf initial */
	http2Context->sendBuf.field_ptr = BoatMalloc(BOAT_HTTP2_SEND_BUF_MAX_LEN);
	if (NULL == http2Context->sendBuf.field_ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
		boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, http2Init_exception);
	}

	/* boat catch handle */
	boat_catch(http2Init_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		http2DeInit(http2Context);
	}

	return http2Context;
}

void http2DeInit(http2IntfContext *http2Context)
{
	if (NULL != http2Context)
	{
		BoatFree(http2Context->sendBuf.field_ptr);
		if (NULL != http2Context->session)
		{
			nghttp2_session_del(http2Context->session);
			http2Context->session = NULL;
		}
#if (BOAT_TLS_SUPPORT == 1)
		BoatFree(http2Context->tlsContext);
		http2Context->tlsContext = NULL;
		if(http2Context->tlsCAchain != NULL){
			http2Context->tlsCAchain[0].field_len = 0;
			BoatFree(http2Context->tlsCAchain[0].field_ptr);
			BoatFree(http2Context->tlsCAchain);
			
		}
#endif
		BoatFree(http2Context);
		http2Context = NULL;
	}
}

BOAT_RESULT http2SubmitRequest(http2IntfContext *context)
{
	nghttp2_data_provider data_prd;
	nghttp2_session_callbacks *callbacks;
	// char *pathTmp = NULL;
	// Http2Response *parsePtr = NULL;
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
	nghttp2_nv nva[] = {MAKE_NV(":method", "POST"),
						MAKE_NV(":scheme", "http"),
						MAKE_NV(":path", context->pathTmp),
						MAKE_NV(":authority", context->nodeUrl),
						MAKE_NV("content-type", "application/grpc"),
						MAKE_NV("user-agent", "grpc-go/1.15.0"),
						MAKE_NV("Accept-Encoding", "gzip, deflate"),
						MAKE_NV("te", "trailers")};

	// parsePtr = (Http2Response *)context->parseDataPtr;
	// if ((parsePtr != NULL) && (parsePtr->httpResLen != 0))
	// {
	// 	if(parsePtr->http2Res != NULL){
	// 		BoatFree(parsePtr->http2Res);
	// 	}

	// 	parsePtr->http2Res = NULL;
	// 	parsePtr->httpResLen = 0;
	// }
	/* connection establishment */
	context->sockfd = BoatConnect(context->nodeUrl, NULL);
	if (context->sockfd < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatConnect failed.");
		boat_throw(BOAT_ERROR_HTTP2_CONNECT_FAIL, http2SubmitRequest_exception);
	}
#if (BOAT_TLS_SUPPORT == 1)
	result = BoatTlsInit(context->hostName, context->tlsCAchain, context->sockfd, context->tlsContext, NULL);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatTlsInit failed.");
		boat_throw(BOAT_ERROR_HTTP2_TLS_INIT_FAIL, http2SubmitRequest_exception);
	}
#endif
	if (context->session != NULL)
	{
		nghttp2_session_del(context->session);
		context->session = NULL;
	}
	nghttp2_session_callbacks_new(&callbacks);
	nghttp2_session_callbacks_set_send_callback(callbacks, send_callback);
	nghttp2_session_callbacks_set_recv_callback(callbacks, recv_callback);
	nghttp2_session_callbacks_set_on_frame_recv_callback(callbacks, on_frame_recv_callback);
	nghttp2_session_callbacks_set_on_data_chunk_recv_callback(callbacks, on_data_chunk_recv_callback);
	nghttp2_session_callbacks_set_on_stream_close_callback(callbacks, on_stream_close_callback);
	nghttp2_session_client_new(&context->session, callbacks, context);
	nghttp2_session_callbacks_del(callbacks);

	result = nghttp2_submit_settings(context->session, NGHTTP2_FLAG_NONE, NULL, 0);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "submit settings failed.");
		boat_throw(BOAT_ERROR_HTTP2_INTERNAL + result, http2SubmitRequest_exception);
	}
	data_prd.read_callback = data_source_read_callback;
	result = nghttp2_submit_request(context->session, NULL, nva, sizeof(nva) / sizeof(nva[0]), &data_prd, NULL);
	if (result < BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "submit request failed.");
		boat_throw(BOAT_ERROR_HTTP2_INTERNAL + result, http2SubmitRequest_exception);
	}
	result = nghttp2_session_send(context->session);
	if (result < BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "submit request failed.");
		boat_throw(BOAT_ERROR_HTTP2_INTERNAL + result, http2SubmitRequest_exception);
	}
	result = nghttp2_session_recv(context->session);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "submit request failed.");
		boat_throw(BOAT_ERROR_HTTP2_INTERNAL + result, http2SubmitRequest_exception);
	}
	result = BOAT_SUCCESS;
	boat_catch(http2SubmitRequest_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}
	return result;
}