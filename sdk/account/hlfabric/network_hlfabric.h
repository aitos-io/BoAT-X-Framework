/*
 * @Description: 
 * @Author: aitos
 * @Date: 2022-09-09 11:24:55
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-09 12:00:16
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



#include "boattypes.h"

#define BOAT_HLFABRIC_NODE_URL_MAX_LEN           127  //!< Maxmum length of node's URL
#define BOAT_HLFABRIC_HOSTNAME_MAX_LEN           127  //!< Maxmum length of hostname
#define BOAT_HLFABRIC_CERT_MAX_LEN               1024 //!< Maxmum length of certificate

//!@brief fabric certificate information config structure
//! fabric certificate information config structure
typedef struct TBoatHlfabricCertInfoCfg{
	BUINT32  length;                              //!< length of certificate content, this length contains the terminator '\0'.
	BCHAR    content[BOAT_HLFABRIC_CERT_MAX_LEN]; //!< content of certificate.
}BoatHlfabricCertInfoCfg;

//!@brief fabric node information config structure
//! fabric node information config structure
typedef struct TBoatHlfabricNodeInfoCfg{
	BCHAR  *nodeUrl;  //!< URL of the blockchain node, e.g. "http://a.b.com:8545"	
	BCHAR  *hostName; //!< TLS server hostname, it's 'CN' field in server certificate
}BoatHlfabricNodeInfoCfg;

//!@brief fabric node group config structure
//! fabric node group config structure
typedef struct TBoatHlfabricNodeGroupCfg{
	BUINT8 endorserNumber;//!< The number of endorser to be set
	BUINT8 quantities;
	BoatHlfabricNodeInfoCfg *endorser;
	BoatHlfabricCertInfoCfg tlsOrgCertContent;
}BoatHlfabricNodeGroupCfg;

//!@brief fabric node layout config structure
//! fabric node layout config structure
typedef struct TBoatHlfabricNodeLayoutCfg{
	BUINT8 endorserGroupNum;
	BoatHlfabricNodeGroupCfg *groupCfg;
}BoatHlfabricNodeLayoutCfg;

//!@brief fabric node  config structure
//! fabric node layout config structure
typedef struct TBoatHlfabricNodesCfg{
	BUINT8 endorserLayoutNum;
	BoatHlfabricNodeLayoutCfg *layoutCfg;
	BoatHlfabricNodeGroupCfg orderCfg;
}BoatHlfabricNodesCfg;


//!@brief fabric wallet config structure
//! fabric wallet config structure
typedef struct TBoatHlfabricNetworkConfig
{
	BoatHlfabricCertInfoCfg     accountCertContent;   //!< certificate content of account
	BoatHlfabricCertInfoCfg     tlsClientCertContent; //!< certificate content of TLS 	
	BoatHlfabricNodesCfg 		nodesCfg;
}BoatHlfabricNetworkConfig;

//!@brief fabric network  structure
//! fabric network  structure
typedef struct TBoatHlfabricNetwork
{
	BoatHlfabricCertInfoCfg     accountCertContent;   //!< certificate content of account
	BoatHlfabricCertInfoCfg     tlsClientCertContent; //!< certificate content of TLS 	
	BoatHlfabricNodesCfg 		nodesCfg;
}BoatHlfabricNetwork;

#endif
