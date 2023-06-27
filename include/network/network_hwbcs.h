/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-09 11:24:55
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-21 12:12:19
 */
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

#ifndef __NETWORK_HWBCS_H__
#define __NETWORK_HWBCS_H__

#include "boatiotsdk.h"

#define BOAT_HWBCS_TLS_SUPPORT BOAT_TLS_SUPPORT					//!< If need client support TLS, set it to 1.
#define BOAT_HWBCS_TLS_IDENTIFY_CLIENT BOAT_TLS_IDENTIFY_CLIENT //!< If server need identify client, set it to 1.

//!< This macro valid only BOAT_HWBCS_TLS_SUPPORT has
//!< set to 1.
//!< @note This macro is reserved for future.

#define BOAT_HWBCS_NODE_URL_MAX_LEN 127 //!< Maxmum length of node's URL
#define BOAT_HWBCS_HOSTNAME_MAX_LEN 127 //!< Maxmum length of hostname
#define BOAT_HWBCS_CERT_MAX_LEN 1024	//!< Maxmum length of certificate

//!@brief hwbcs certificate information config structure
//! hwbcs certificate information config structure
typedef struct TBoatHwbcsCertInfoCfg
{
	BUINT32 length;							//!< length of certificate content, this length contains the terminator '\0'.
	BCHAR content[BOAT_HWBCS_CERT_MAX_LEN]; //!< content of certificate.
} BoatHwbcsCertInfoCfg;

//!@brief hwbcs node information config structure
//! hwbcs node information config structure
typedef struct TBoatHwbcsNodeInfoCfg
{
	BCHAR *nodeUrl;	 //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
	BCHAR *hostName; //!< TLS server hostname, it's 'CN' field in server certificate
} BoatHwbcsNodeInfoCfg;

//!@brief hwbcs node group config structure
//! hwbcs node group config structure
typedef struct TBoatHwbcsNodeGroupCfg
{
	BUINT8 endorserNumber; //!< The number of endorser to be set
	BUINT8 quantities;
	BoatHwbcsNodeInfoCfg *endorser;
	BoatHwbcsCertInfoCfg tlsOrgCertContent;
} BoatHwbcsNodeGroupCfg;

//!@brief hwbcs node layout config structure
//! hwbcs node layout config structure
typedef struct TBoatHwbcsNodeLayoutCfg
{
	BUINT8 endorserGroupNum;
	BoatHwbcsNodeGroupCfg *groupCfg;
} BoatHwbcsNodeLayoutCfg;

//!@brief hwbcs node  config structure
//! hwbcs node layout config structure
typedef struct TBoatHwbcsNodesCfg
{
	BUINT8 endorserLayoutNum;
	BoatHwbcsNodeLayoutCfg *layoutCfg;
	BoatHwbcsNodeGroupCfg orderCfg;
} BoatHwbcsNodesCfg;

//!@brief hwbcs wallet config structure
//! hwbcs wallet config structure
typedef struct TBoatHwbcsNetworkConfig
{
	BoatHwbcsCertInfoCfg accountCertContent; //!< certificate content of account
#if (BOAT_HWBCS_TLS_SUPPORT == 1) && (BOAT_HWBCS_TLS_IDENTIFY_CLIENT == 1)
	BoatKeypairExtraData accountClientTlsPrikey;
	BoatHwbcsCertInfoCfg accountClientTlsCert;
#endif
	BoatHwbcsNodesCfg nodesCfg;
} BoatHwbcsNetworkConfig;

//!@brief hwbcs network  structure
//! hwbcs network  structure
typedef struct TBoatHwbcsNetworkData
{
	BUINT8 index;
	BoatHwbcsCertInfoCfg accountCertContent; //!< certificate content of account
#if (BOAT_HWBCS_TLS_SUPPORT == 1) && (BOAT_HWBCS_TLS_IDENTIFY_CLIENT == 1)
	BoatKeypairExtraData accountClientTlsPrikey;
	BoatHwbcsCertInfoCfg accountClientTlsCert;
#endif
	BoatHwbcsNodesCfg nodesCfg;
} BoatHwbcsNetworkData;

typedef struct TBoatHwbcsNetworkContext
{
	/* data */
	BUINT8 networkNum;
	BoatHwbcsNetworkData networks[BOAT_MAX_NETWORK_NUM + 1]; // hwbcs networks
} BoatHwbcsNetworkContext;

/**
 * @description:
 *  This function get network list ,include persistent networks and onetime network
 * @param[out] {BoatHwbcsNetworkContext} *networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcs_GetNetworkList(BoatHwbcsNetworkContext *networkList);

/**
 * @description:
 *  This function reset every param in BoatHwbcsNetworkData;
 *  if someone have malloced memory , free the memory;
 * @param {BoatHwbcsNetworkData} networkData
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcs_FreeNetworkData(BoatHwbcsNetworkData networkData);

/**
 * @description:
 *  This function use to free BoatHwbcsNetworkContext param.
 * @param[in] {BoatHwbcsNetworkContext} networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcs_FreeNetworkContext(BoatHwbcsNetworkContext networkList);

/**
 * @description:
 * This function create hwbcs network
 * @param[in] {BoatHwbcsNetworkConfig} networkConfig
 * @param[in] {BoatStoreType} storeType
 *  For onetime network or test , select store in ram .
 *  For persistent network ,slect store in flash.
 * @return
 *   This function returns network index if create network successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcsNetworkCreate(BoatHwbcsNetworkConfig *networkConfig, BoatStoreType storeType);

/**
 * @description:
 *  This function delete the network which networkIndex equals index.
 *  This function will delete the network data in Nvram.
 * @param {BUINT8} index
 *  the index want to delete
 * @return {*}
 *   This function returns BOAT_SUCCESS if delete successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcsNetworkDelete(BUINT8 index);

/**
 * @description:
 *  This function read network data from Nvram by index.
 * @param {BoatHwbcsNetworkData} *networkData
 * @param {BUINT8} index
 *  the network index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcs_GetNetworkByIndex(BoatHwbcsNetworkData *networkData, BUINT8 index);

#endif
