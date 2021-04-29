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

/*!@brief Boatwallet options

@file
boatconfig.h defines options for compiling.
*/

#ifndef __BOATCONFIG_H__
#define __BOATCONFIG_H__

// // BOAT LOG LEVEL DEFINITION
// // Log level is used to control the detail of log output.
// // 3 types of detail level can be specified in BoatLog():
// // "CRITICAL" is for critical exceptions that may halt the wallet from going on.
// // "NORMAL" is for warnings that may imply some error but could be held.
// // "VERBOSE" is for detail information that is only useful for debug.
// #define BOAT_LOG_NONE                            0
// #define BOAT_LOG_CRITICAL                        1
// #define BOAT_LOG_NORMAL                          2
// #define BOAT_LOG_VERBOSE                         3

// // BOAT_LOG_LEVEL is a macro that limits the log detail up to that level.
// // Seting it to BOAT_LOG_NONE means outputing nothing.
// #define BOAT_LOG_LEVEL   BOAT_LOG_VERBOSE

// RPC USE OPTION: One and only one RPC_USE option shall be set to 1
#define RPC_USE_LIBCURL                          0
#define RPC_USE_NWYHTTPPORT                      1
#define RPC_USE_NOTHING                          0

#define RPC_USE_COUNT                            (RPC_USE_NWYHTTPPORT + RPC_USE_LIBCURL + RPC_USE_NOTHING)

#if (RPC_USE_COUNT != 1)
#error "One and only one RPC_USE option shall be set to 1"
#endif
#undef RPC_USE_COUNT


#define BOAT_USE_NWY_FILESYSTEM                  1

// Blockchain Protocol OPTION
#define PROTOCOL_USE_ETHEREUM   	             1
#define PROTOCOL_USE_PLATONE    	             1
#define PROTOCOL_USE_HLFABRIC                    0
#define PROTOCOL_USE_FISCOBCOS    	             1

// Protocol dependency
#if PROTOCOL_USE_PLATONE != 0
#undef PROTOCOL_USE_ETHEREUM
#define PROTOCOL_USE_ETHEREUM                    1
#endif

#if (PROTOCOL_USE_HLFABRIC != 0)
#undef PROTOCOL_USE_ETHEREUM
#define PROTOCOL_USE_ETHEREUM                    1
#endif

#if (PROTOCOL_USE_FISCOBCOS != 0)
#undef PROTOCOL_USE_ETHEREUM
#define PROTOCOL_USE_ETHEREUM                    1
#endif

#define PROTOCOL_USE_COUNT                       (PROTOCOL_USE_ETHEREUM + \
												  PROTOCOL_USE_HLFABRIC + \
												  PROTOCOL_USE_PLATONE  + \
												  PROTOCOL_USE_FISCOBCOS)

#if PROTOCOL_USE_COUNT == 0
#error "At least ONE PROTOCOL_USE option shall be set to 1"
#endif
#undef PROTOCOL_USE_COUNT

//! Mining interval and Pending transaction timeout
//#define BOAT_MINE_INTERVAL                       3  //!< Mining Interval of the blockchain, in seconds
//#define BOAT_WAIT_PENDING_TX_TIMEOUT             30 //!< Timeout waiting for a transaction being mined, in seconds


//! Maxmum length for node's URL
//#define BOAT_NODE_URL_MAX_LEN                    127
//#define BOAT_FILENAME_MAX_LEN                    127

#if PROTOCOL_USE_HLFABRIC == 1
//! hyperledger fabric configuration items
// #define BOAT_HLFABRIC_ARGS_MAX_NUM                    4   //!< arguments max number in fabric command
// #define BOAT_HLFABRIC_ENDORSER_MAX_NUM                10  //!< support endorser max number
// #define BOAT_HLFABRIC_ORDERER_MAX_NUM                 4   //!< support orderer max number

// #define BOAT_HLFABRIC_TLS_SUPPORT                     1 //!< If need client support TLS, set it to 1.
// #define BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT             0 //!< If server need identify client, set it to 1.
//                                                    //!< this macro valid only BOAT_HLFABRIC_TLS_SUPPORT has
//                                                    //!< set to 1.
// #define BOAT_HLFABRIC_ROOTCA_MAX_NUM                  3 //!< support ROOT CA certificate max number

// #define BOAT_HLFABRIC_HTTP2_SEND_BUF_MAX_LEN                   4096 //!< The maximum length of HTTP2 send buffer



#endif /* end of PROTOCOL_USE_HLFABRIC */

//#define BOAT_STRING_MAX_LEN					     4096 //!< The maximum allowed string length

#endif /* end of __BOATCONFIG_H__ */
