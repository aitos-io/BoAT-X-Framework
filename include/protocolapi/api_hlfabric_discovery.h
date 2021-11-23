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

/*!@brief Header file for performing fabric transaction

@file
api_hlfabric.h is header file for fabric transaction construction and performing.
*/

#ifndef __API_HLFABRIC_DISCOVER_H__
#define __API_HLFABRIC_DISCOVER_H__

#if( DISCOVERY_PEER_QUERY == 1)
#include "boatiotsdk.h"
#include "boattypes.h"


typedef struct PeerInfo
{
    BCHAR    *url;
    BCHAR    *cert;
}PeerInfo;

typedef struct OrgInfo
{
    BCHAR *orgName;
    BUINT8 peerNum;
    PeerInfo *peers;
    BCHAR   *tlsCert;
}OrgInfo;
typedef struct DiscoverPeers
{
    BUINT8 orgNum;
    OrgInfo *orgs;
}DiscoverPeers;


typedef struct EndorsersByGroups
{
    BCHAR *MSPID;
    BCHAR *Endpoint;
}Endorsers;


typedef struct groupInfo
{
    BCHAR *key;
    BUINT8 value;
    BUINT8 numEndorsers;
    Endorsers *endorsers;
}groupInfo;

typedef struct T_layoutInfo
{
    BUINT8 num;
    groupInfo * groups;
}layoutInfo;

typedef struct T_cc_query_res
{
   BUINT8 num;
   layoutInfo *layouts;
}cc_query_res;

typedef struct mspsInfo
{
    BCHAR *name;
    BCHAR *tlsCert;
    BUINT16 tlsCertLen;
}mspsInfo;

typedef struct msps
{
    BUINT8 num;
    mspsInfo *discoverMspInfo;
}msps;

typedef struct orderInfo
{
    BCHAR *host;
    BCHAR port[6];
    BCHAR *name;
}orderInfo;

typedef struct orders
{
    BUINT8 num;
    orderInfo *discoverOrderinfo;
}orders;


typedef struct discoverConfig
{
    msps discoverMsps;
    orders discoverOrders;
}discoverConfig;




typedef struct T_DiscoverRes
{
    // DiscoverPeers Peership;
    cc_query_res  cc_res;
    discoverConfig discoverConfig;
}DiscoverRes;



/*!****************************************************************************
 * @brief 
 *   Submit transaction.
 *
 * @details
 *   This function should be invoked after BoatHlfabricTxSetTimestamp() and
 *   BoatHlfabricTxSetArgs() had excuted.When you need to set states to
 *   Hlfabric,use this function.
 * @param tx_ptr 
 *   Fabric transaction structure pointer.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if submit success, otherwise return a error code.
 ******************************************************************************/
BOAT_RESULT BoatHlfabricDiscoverySubmit( BoatHlfabricTx *tx_ptr,const BoatHlfabricNodesCfg endorserInfo_ptr );
/*! @}*/
void DiscoveryResInit(DiscoverRes *discoverResult);
void DiscoveryResFree(DiscoverRes discoverResult);

#endif
/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
