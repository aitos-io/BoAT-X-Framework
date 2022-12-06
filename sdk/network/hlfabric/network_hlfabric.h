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

#ifndef __NETWORK_HLFABRIC_H__
#define __NETWORK_HLFABRIC_H__

#include "boatiotsdk.h"
#include "boattypes.h"

#define BOAT_HLFABRIC_TLS_SUPPORT BOAT_TLS_SUPPORT				   //!< If need client support TLS, set it to 1.
#define BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT BOAT_TLS_IDENTIFY_CLIENT //!< If server need identify client, set it to 1.

//!< This macro valid only BOAT_HLFABRIC_TLS_SUPPORT has
//!< set to 1.
//!< @note This macro is reserved for future.

#define BOAT_HLFABRIC_NODE_URL_MAX_LEN 127 //!< Maxmum length of node's URL
#define BOAT_HLFABRIC_HOSTNAME_MAX_LEN 127 //!< Maxmum length of hostname
#define BOAT_HLFABRIC_CERT_MAX_LEN 1024	   //!< Maxmum length of certificate

//!@brief fabric certificate information config structure
//! fabric certificate information config structure
typedef struct TBoatHlfabricCertInfoCfg
{
	BUINT32 length;							   //!< length of certificate content, this length contains the terminator '\0'.
	BCHAR content[BOAT_HLFABRIC_CERT_MAX_LEN]; //!< content of certificate.
} BoatHlfabricCertInfoCfg;

//!@brief fabric node information config structure
//! fabric node information config structure
typedef struct TBoatHlfabricNodeInfoCfg
{
	BCHAR *nodeUrl;	 //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
	BCHAR *hostName; //!< TLS server hostname, it's 'CN' field in server certificate
} BoatHlfabricNodeInfoCfg;

//!@brief fabric node group config structure
//! fabric node group config structure
typedef struct TBoatHlfabricNodeGroupCfg
{
	BUINT8 endorserNumber; //!< The number of endorser to be set
	BUINT8 quantities;
	BoatHlfabricNodeInfoCfg *endorser;
	BoatHlfabricCertInfoCfg tlsOrgCertContent;
} BoatHlfabricNodeGroupCfg;

//!@brief fabric node layout config structure
//! fabric node layout config structure
typedef struct TBoatHlfabricNodeLayoutCfg
{
	BUINT8 endorserGroupNum;
	BoatHlfabricNodeGroupCfg *groupCfg;
} BoatHlfabricNodeLayoutCfg;

//!@brief fabric node  config structure
//! fabric node layout config structure
typedef struct TBoatHlfabricNodesCfg
{
	BUINT8 endorserLayoutNum;
	BoatHlfabricNodeLayoutCfg *layoutCfg;
	BoatHlfabricNodeGroupCfg orderCfg;
} BoatHlfabricNodesCfg;

//!@brief fabric wallet config structure
//! fabric wallet config structure
typedef struct TBoatHlfabricNetworkConfig
{
	BoatHlfabricCertInfoCfg accountCertContent; //!< certificate content of account
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	BoatKeypairExtraData accountClientTlsPrikey;
	BoatHlfabricCertInfoCfg accountClientTlsCert;
#endif
	BoatHlfabricNodesCfg nodesCfg;
} BoatHlfabricNetworkConfig;

//!@brief fabric network  structure
//! fabric network  structure
typedef struct TBoatHlfabricNetworkData
{
	BUINT8 index;
	BoatHlfabricCertInfoCfg accountCertContent; //!< certificate content of account
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	BoatKeypairExtraData accountClientTlsPrikey;
	BoatHlfabricCertInfoCfg accountClientTlsCert;
#endif
	BoatHlfabricNodesCfg nodesCfg;
} BoatHlfabricNetworkData;

typedef struct TBoatHlfabricNetworkContext
{
	/* data */
	BUINT8 networkNum;
	BoatHlfabricNetworkData networks[BOAT_MAX_NETWORK_NUM + 1]; // fabirc networks
} BoatHlfabricNetworkContext;

/**
 * @description:
 *  This function get network list ,include persistent networks and onetime network
 * @param[out] {BoatHlfabricNetworkContext} *networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHlfabric_GetNetworkList(BoatHlfabricNetworkContext *networkList);

/**
 * @description:
 *  This function reset every param in BoatHlfabricNetworkData;
 *  if someone have malloced memory , free the memory;
 * @param {BoatHlfabricNetworkData} networkData
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHlfabric_FreeNetworkData(BoatHlfabricNetworkData networkData);

/**
 * @description:
 *  This function use to free BoatHlfabricNetworkContext param.
 * @param[in] {BoatHlfabricNetworkContext} networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHlfabric_FreeNetworkContext(BoatHlfabricNetworkContext networkList);

/**
 * @description:
 * This function create fabric network
 * @param[in] {BoatHlfabricNetworkConfig} networkConfig
 * @param[in] {BoatStoreType} storeType
 *  For onetime network or test , select store in ram .
 *  For persistent network ,slect store in flash.
 * @return
 *   This function returns network index if create network successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoatHlfabricNetworkCreate(BoatHlfabricNetworkConfig *networkConfig, BoatStoreType storeType);

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
BOAT_RESULT BoATHlfabricNetworkDelete(BUINT8 index);

/**
 * @description:
 *  This function read network data from Nvram by index.
 * @param {BoatHlfabricNetworkData} *networkData
 * @param {BUINT8} index
 *  the network index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHlfabric_GetNetworkByIndex(BoatHlfabricNetworkData *networkData, BUINT8 index);

#endif
