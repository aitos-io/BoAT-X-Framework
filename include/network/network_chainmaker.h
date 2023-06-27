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



#ifndef __NETWORK_CHAINMAKER_H__
#define __NETWORK_CHAINMAKER_H__


#include "boatiotsdk.h"
#include "boattypes.h"

#define BOAT_CHAINMAKER_TLS_SUPPORT                BOAT_TLS_SUPPORT //!< If need client support TLS, set it to 1.
#define BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT        BOAT_TLS_IDENTIFY_CLIENT //!< If server need identify client, set it to 1.

                                                   //!< This macro valid only BOAT_HLFABRIC_TLS_SUPPORT has
                                                   //!< set to 1. 
                                                   //!< @note This macro is reserved for future.

#define BOAT_CHAINMAKER_CERT_MAX_LEN             1024 //!< Maxmum length of certificate

//!@brief chainmaker certificate information config structure
//! chainmaker certificate information config structure
typedef struct TBoatChainmakerCertInfoCfg{
    BUINT32  length;                              //!< length of certificate content, this length contains the terminator '\0'.
    BCHAR    content[BOAT_CHAINMAKER_CERT_MAX_LEN]; //!< content of certificate.
} BoatChainmakerCertInfoCfg;

typedef enum TBoatChainmakerMemberType {

  BOAT_MEMBER_TYPE_CERT       = 0,       // *X509 cert
  BOAT_MEMBER_TYPE_CERT_HASH  = 1,       // *cert hash
  BOAT_MEMBER_TYPE_PUBLIC_KEY = 2,       // *public key
  BOAT_MEMBER_TYPE_TYPE_DID   = 3,       // *did
  BOAT_MEMBER_TYPE_YPE_ALIAS  = 4,       // *alias
  BOAT_MEMBER_TYPE_YPE_ADDR   = 5        
   // *address

} BoatChainmakerMemberType;
//!@brief chainmaker network  structure
//! chainmaker network  structure
typedef struct TBoatChainmakerNetworkData
{
    BUINT8 networkIndex;

    BoatChainmakerMemberType      client_member_type;
    BoatChainmakerCertInfoCfg     client_sign_cert_content;   //!< certificate content of account

    BCHAR *node_url;
    BCHAR *host_name;
    BCHAR *chain_id;
    BCHAR *org_id;
    
#if(BOAT_CHAINMAKER_TLS_SUPPORT == 1) 

    BoatChainmakerCertInfoCfg     ca_tls_cert_content;
    #if(BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)

        BoatChainmakerCertInfoCfg     client_tls_cert_content;
        BoatKeypairExtraData          client_tls_privkey_data;
    #endif
#endif
    
} BoatChainmakerNetworkData;

typedef struct TBoatChainmakerNetworkContext
{
    /* data */
    BUINT8 networkNum;
    BoatChainmakerNetworkData networks[BOAT_MAX_NETWORK_NUM];  //fabirc networks
} BoatChainmakerNetworkContext;


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
BOAT_RESULT BoATChainmaker_GetNetworkList(BoatChainmakerNetworkContext *networkList);


/**
 * @description: 
 * This function creat fabric network
 * @param[in] {BoatChainmakerNetworkData} networkConfig
 * @param[in] {BoatStoreType} storeType
 *  For onetime network or test , select store in ram .
 *  For persistent network ,slect store in flash.
 * @return 
 *   This function returns network index if creat network successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */

BOAT_RESULT BoATChainmakerNetworkCreate(BoatChainmakerNetworkData *networkConfig, BoatStoreType storeType);

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

BOAT_RESULT BoATChainmakerNetworkDelete(BUINT8 index);


/**
 * @description: 
 *  This function read network data from Nvram by index.
 * @param {BoatChainmakerNetworkData} *networkData
 * @param {BUINT8} index
 *  the network index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */

BOAT_RESULT BoATChainmaker_GetNetworkByIndex(BoatChainmakerNetworkData *networkData ,BUINT8 index);


#endif
