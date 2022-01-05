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

/*!@brief Error codes

@file
boaterrcode.h defines error codes.
*/

#ifndef __BOATERRCODE_H__
#define __BOATERRCODE_H__


#define BOAT_SUCCESS	                    (0)
#define BOAT_ERROR	                        (-1)
#define BOAT_ERROR_MQTT_INIT_FAIL	        (-10900001)
#define BOAT_ERROR_MQTT_PUB_FAIL	        (-10900002)
#define BOAT_ERROR_HTTP_POST_FAIL	        (-10801000)
#define BOAT_ERROR_HTTP_INIT_FAIL	        (-10800001)
#define BOAT_ERROR_HTTP_UNRESOLVED_DNS	    (-10800002)
#define BOAT_ERROR_HTTP_CONNECT_FAIL	    (-10800003)
#define BOAT_ERROR_HTTP_CLOSED	            (-10800004)
#define BOAT_ERROR_HTTP_PARSE_FAIL	        (-10800005)
#define BOAT_ERROR_HTTP_GENERAL	            (-10800006)
#define BOAT_ERROR_HTTP_PRTCL	            (-10800007)
	
	
	
#define BOAT_ERROR_WEB3_JSON_PARSE_FAIL	    (-10700000)
#define BOAT_ERROR_WEB3_JSON_GETOBJ_FAIL	(-10700001)
#define BOAT_ERROR_WEB3_RPC_INIT_FAIL	    (-10700002)
#define BOAT_ERROR_WEB3_SEND_FAIL	        (-10700003)
#define BOAT_ERROR_WEB3_GET_GASPRICE_FAIL	(-10700004)
#define BOAT_ERROR_HTTP2_CONNECT_FAIL	    (-10600000)
#define BOAT_ERROR_HTTP2_INIT_FAIL	        (-10600001)
#define BOAT_ERROR_HTTP2_TLS_INIT_FAIL	    (-10600002)
#define BOAT_ERROR_HTTP2_REQUEST_FAIL	    (-10600003)
#define BOAT_ERROR_HTTP2_RECEIVE_FAIL	    (-10600004)
#define BOAT_ERROR_HTTP2_SEND_FAIL	        (-10600005)
#define BOAT_ERROR_HTTP2_GENERAL	        (-10600006)
#define BOAT_ERROR_HTTP2_INTERNAL	        (-10601000)
#define BOAT_ERROR_RLP_STRING_INIT_FAIL	    (-10500000)
#define BOAT_ERROR_RLP_LIST_INIT_FAIL	    (-10500001)
#define BOAT_ERROR_RLP_ENCODER_APPEND_FAIL	(-10500002)
#define BOAT_ERROR_RLP_ENCODER_REPLACE_FAIL	(-10500003)
#define BOAT_ERROR_RLP_ENCODER_FAIL	        (-10500004)
#define BOAT_ERROR_STORAGE_FILE_OPEN_FAIL	(-10400000)
#define BOAT_ERROR_STORAGE_FILE_WRITE_FAIL	(-10400001)
#define BOAT_ERROR_STORAGE_FILE_READ_FAIL	(-10400002)
#define BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL	(-10400003)
#define BOAT_ERROR_PERSISTER_STORE_FAIL	    (-10410000)
#define BOAT_ERROR_PERSISTER_READ_FAIL	    (-10410001)
#define BOAT_ERROR_PERSISTER_DELETE_FAIL	(-10410002)
#define BOAT_ERROR_CURL_INIT_FAIL	        (-10300000)
#define BOAT_ERROR_CURL_SETOPT_FAIL	        (-10300001)
#define BOAT_ERROR_CURL_SLIST_APPEND_FAIL	(-10300002)
#define BOAT_ERROR_CURL_PERFORM_FAIL	    (-10300003)
#define BOAT_ERROR_CURL_CODE_FAIL	        (-10301000)
#define BOAT_ERROR_CURL_INFO_FAIL	        (-10302000)
#define BOAT_ERROR_WALLET_INIT_FAIL	        (-10200000)
#define BOAT_ERROR_WALLET_NUM_EXCEED	    (-10200001)
#define BOAT_ERROR_WALLET_WEB3_INIT_FAIL	(-10200002)
#define BOAT_ERROR_WALLET_RESULT_PRASE_FAIL	(-10200003)
#define BOAT_ERROR_WALLET_CREATE_FAIL	    (-10200004)
#define BOAT_ERROR_WALLET_KEY_FORMAT_ERR	(-10210000)
#define BOAT_ERROR_WALLET_KEY_CREAT_FAIL	(-10210001)
#define BOAT_ERROR_WALLET_KEY_TYPE_ERR	    (-10210002)
#define BOAT_ERROR_WALLET_KEY_GENMODE_ERR	(-10210003)
#define BOAT_ERROR_COMMON_INVALID_ARGUMENT	(-10100000)
#define BOAT_ERROR_COMMON_OUT_OF_MEMORY	    (-10100001)
#define BOAT_ERROR_COMMON_PROTO_PACKET_FAIL	(-10100002)
#define BOAT_ERROR_COMMON_TX_PENDING	    (-10100003)
#define BOAT_ERROR_COMMON_UTILITY	        (-10100004)
#define BOAT_ERROR_COMMON_GEN_RAND_FAIL	    (-10110000)
#define BOAT_ERROR_COMMON_GEN_HASH_FAIL	    (-10110001)
#define BOAT_ERROR_COMMON_GEN_SIGN_FAIL	    (-10110002)


#endif
