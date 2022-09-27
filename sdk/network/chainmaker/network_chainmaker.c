/*
 * @Description: 
 * @Author: aitos
 * @Date: 2022-09-09 11:24:55
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-21 15:24:42
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

#include "boatconfig.h"
#include "boatinternal.h"

#include "persiststore.h"
#include "network_chainmaker.h"
#include "chainmaker_network.pb-c.h"

/**
 * @description: 
 *  This function get BoatChainmakerNetworkData struct from Common__ChainmakerNetworkData;
 *  move data from Common__ChainmakerNetworkData to BoatChainmakerNetworkData;
 * @param {BoatChainmakerNetworkData} *Networkdata
 * @param {Common__ChainmakerNetworkData} *network_proto
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATChainmaker_getNetworkFromProto(BoatChainmakerNetworkData *Networkdata, Common__ChainmakerNetworkData *network_proto )
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if (Networkdata == NULL || network_proto == NULL){
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    /* index */
    Networkdata->index = network_proto->index;

    /* client sign Cert */
    if(strlen(network_proto->client_sign_cert_content) > sizeof(Networkdata->client_sign_cert_content.content))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    Networkdata->client_sign_cert_content.length = strlen(network_proto->client_sign_cert_content);
    strcpy(Networkdata->client_sign_cert_content.content, network_proto->client_sign_cert_content);

    /*node info*/
    if (strlen(network_proto->node_url)  > BOAT_CHAINMAKER_NODE_INFO_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    if (strlen(network_proto->host_name) > BOAT_CHAINMAKER_NODE_INFO_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    if (strlen(network_proto->chain_id)  > BOAT_CHAINMAKER_NODE_INFO_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    if (strlen(network_proto->org_id)    > BOAT_CHAINMAKER_NODE_INFO_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    strcpy(Networkdata->node_url,  network_proto->node_url);
    strcpy(Networkdata->host_name, network_proto->host_name);
    strcpy(Networkdata->chain_id,  network_proto->chain_id);
    strcpy(Networkdata->org_id,    network_proto->org_id);

   
#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1) 
    {
        if (strlen(network_proto->client_tls_cert_content) -1 > sizeof(Networkdata->client_tls_cert_content.content))
        {
            BoatLog(BOAT_LOG_NORMAL,"client tls cert len exceed");
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }

        Networkdata->client_tls_cert_content.length = strlen(network_proto->client_tls_cert_content);
        strcpy((BCHAR*)Networkdata->client_tls_cert_content.content, network_proto->client_tls_cert_content);
    }
#endif    

#if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
    {
        if(strlen(network_proto->client_tls_privkey_vlaue) -1 > sizeof(Networkdata->client_tls_privkey_vlaue.value)){
            BoatLog(BOAT_LOG_NORMAL,"client tls prikey len exceed");
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        Networkdata->client_tls_privkey_vlaue.value_len = strlen(network_proto->client_tls_privkey_vlaue);
        strcpy((BCHAR*)Networkdata->client_tls_privkey_vlaue.value, network_proto->client_tls_privkey_vlaue);
    }
#endif
    return result;
}

/**
 * @description: 
 *  This function get the network data of chainmaker from Nvram;
 *  after reading data from Nvram , unpacked the data to fabric struct;
 * @param {BoatChainmakerNetworkData} *Networkdata
 * @param {BUINT32} startAddr
 * @param {BoatStoreType} storeType
 *  from flash or ram
 * @param {BUINT32} *outlen
 *  the length of Networkdata
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATChainmaker_GetNetworkFromNvram(BoatChainmakerNetworkData *Networkdata , BUINT32 startAddr, BoatStoreType storeType,BUINT32 *outlen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 lengthbytes[3] = {0};
    BUINT32 offset = startAddr;
    BUINT32 networkDataLength = 0 , networkDataLengthLen = 0;
    BUINT8 *networkcontent = NULL;
    Common__ChainmakerNetworkData *protobuf_network = NULL;
    boat_try_declare;

    if(Networkdata == NULL){
        BoatLog(BOAT_LOG_NORMAL,"param is NULL");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, chainmakerGetnetworkdata_exception);
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,lengthbytes,sizeof(lengthbytes),storeType);
    if(result != BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"Read network length fail , errorcode = %d ",result);
        boat_throw(result, chainmakerGetnetworkdata_exception);
    }
    networkDataLength = UtilityGetLVData_L(lengthbytes);
    if(networkDataLength < 0){
        BoatLog(BOAT_LOG_NORMAL,"network data length err ");
        boat_throw(BOAT_ERROR, chainmakerGetnetworkdata_exception);
    }
    networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
    networkcontent = BoatMalloc(networkDataLength);
    if(networkcontent == NULL){
        BoatLog(BOAT_LOG_NORMAL,"fail to malloc memory");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, chainmakerGetnetworkdata_exception);
    }
    offset += networkDataLengthLen;
    /* read network data */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,networkcontent,networkDataLength,storeType);
    if(result != BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"Read network data fail , errorcode = %d ",result);
        boat_throw(result, chainmakerGetnetworkdata_exception);
    }

    /* unpacket network data */
    protobuf_network = common__chainmaker_network_data__unpack(NULL,networkDataLength,networkcontent);
    if(protobuf_network == NULL){
        BoatLog(BOAT_LOG_NORMAL,"unpack networkdata fail ");
        boat_throw(BOAT_ERROR, chainmakerGetnetworkdata_exception);
    }

    result = BoATChainmaker_getNetworkFromProto(Networkdata, protobuf_network);
    if(result != BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"fail to get network from protobuf struct");
        boat_throw(BOAT_ERROR,chainmakerGetnetworkdata_exception);
    }
    *outlen = (networkDataLength + networkDataLengthLen);
    
	/* boat catch handle */
	boat_catch(chainmakerGetnetworkdata_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    if (networkcontent != NULL){
        BoatFree(networkcontent);
    }

    if (protobuf_network != NULL){
        common__chainmaker_network_data__free_unpacked(protobuf_network,NULL);
    }

    return result;
}

/**
 * @description: 
 *  This function get network list ,include persistent networks and onetime network
 * @param[out] {BoatChainmakerNetworkContext} *networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATChainmaker_GetNetworkList(BoatChainmakerNetworkContext *networkList)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0;
    BUINT8 index = 0;
    BUINT32 networkDataLength = 0 ,networkDataLengthLen = 0 ;
    BUINT32 offset = 0 ;
    BUINT8 networknumBytes[4] = {0};
    BUINT8 lengthBytes[3] = {0};
    BoatProtocolType protocolType = BOAT_PROTOCOL_UNKNOWN;
    /* persistent network  */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,networknumBytes,sizeof(networknumBytes),BOAT_STORE_TYPE_FLASH);
    /* if read Nvram failed , no network */
    if(result != BOAT_SUCCESS){
        networkNum = 0;
        // return BOAT_SUCCESS;
    }else{
        result = utility_check_NumBytes(networknumBytes,&networkNum);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"check network num fail");
            // return result;
            networkNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    (*networkList).networkNum = networkNum;
    offset += sizeof(networknumBytes);
    for (int i = 0; i < networkNum; i++)
    {
        /* protocol type */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,(BUINT8 *)&protocolType,sizeof(protocolType),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS){
            return result;
        }
        offset += sizeof(protocolType);   //protocol type
        offset += sizeof(BUINT8);         // index
        if(protocolType == BOAT_PROTOCOL_HLFABRIC){  // fabric
            result = BoATChainmaker_GetNetworkFromNvram(&(networkList->networks[index++]),offset,BOAT_STORE_TYPE_FLASH,&networkDataLength);
            if(result != BOAT_SUCCESS){
                return result;
            }
            offset += networkDataLength;
        }else{
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH);
            if(result != BOAT_SUCCESS){
                return result;
            }
            networkDataLength = UtilityGetLVData_L(lengthBytes);
            if(networkDataLength < 0){
                BoatLog(BOAT_LOG_NORMAL,"network data  length err ");
                return BOAT_ERROR;
            }
            networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
            offset += (networkDataLength+networkDataLengthLen);
        }
    }
    /* onetime network  */
    offset = 0;
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,networknumBytes,sizeof(networknumBytes),BOAT_STORE_TYPE_RAM);
    if(result != BOAT_SUCCESS){
        networkNum = 0;
        // return BOAT_SUCCESS;
    }else{
        result = utility_check_NumBytes(networknumBytes,&networkNum);
        if(result != BOAT_SUCCESS){
            // BoatLog(BOAT_LOG_NORMAL,"check network num fail");
            // return result;
            networkNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    networkList->networkNum += networkNum;
    offset += sizeof(networknumBytes);
    for (int i = 0; i < networkNum; i++)
    {
        /* protocol type */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,(BUINT8 *)&protocolType,sizeof(protocolType),BOAT_STORE_TYPE_RAM);
        if(result != BOAT_SUCCESS){
            return result;
        }
        offset += sizeof(protocolType);   //protocol type
        offset += sizeof(BUINT8);         // index
        if(protocolType == BOAT_PROTOCOL_HLFABRIC){  // fabric
            result = BoATChainmaker_GetNetworkFromNvram(&(networkList->networks[index++]),offset,BOAT_STORE_TYPE_RAM,&networkDataLength);
            if(result != BOAT_SUCCESS){
                return result;
            }
            offset += networkDataLength;
        }else{
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_RAM);
            if(result != BOAT_SUCCESS){
                return result;
            }
            networkDataLength = UtilityGetLVData_L(lengthBytes);
            if(networkDataLength < 0){
                BoatLog(BOAT_LOG_NORMAL,"network data  length err ");
                return BOAT_ERROR;
            }
            networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
            offset += (networkDataLength+networkDataLengthLen);
            
        }
    }
    return result;
}



/**
 * @description: 
 *  This function get unused network index from persistent networks.
 * @return {*}
 *  This function returns unused network index if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATChainmaker_GetFreeNetworkIndex_From_Persistent(void)
{
    BOAT_RESULT result = BOAT_ERROR;
    BUINT8 networkNum = 0;
    BUINT8 i = 0 , j =0;
    BUINT32 networkDataLength = 0 ,networkDataLengthLen = 0 ;
    BUINT32 offset = 0 ;
    BUINT8 networknumBytes[4] = {0};
    BUINT8 lengthBytes[3] = {0};
    BUINT8 indexBytes[BOAT_MAX_NETWORK_NUM] = {0};
    /* persistent network num */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,networknumBytes,sizeof(networknumBytes),BOAT_STORE_TYPE_FLASH);
    /* if read Nvram failed , no network */
    if(result != BOAT_SUCCESS){
        networkNum = 0;
        // return BOAT_SUCCESS;
    }else{
        result = utility_check_NumBytes(networknumBytes,&networkNum);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"check network num fail");
            // return result;
            networkNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    if(networkNum >= BOAT_MAX_NETWORK_NUM){
        return BOAT_ERROR;
    }
    offset += sizeof(networknumBytes);
    for (size_t i = 0; i < networkNum; i++)
    {
        /* protocol type */
        offset += sizeof(BoatProtocolType);
        /* index */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,&indexBytes[i],sizeof(BUINT8),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS){
            return result;
        }
        offset += sizeof(BUINT8);
        /* network length */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS){
            return result;
        }
        networkDataLength = UtilityGetLVData_L(lengthBytes);
        if(networkDataLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"network data  length err ");
            return BOAT_ERROR;
        }
        networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
        offset += (networkDataLength + networkDataLengthLen);
    }
    for ( i = 0; i < BOAT_MAX_NETWORK_NUM; i++)
    {
        /* code */
        for ( j = 0; j < networkNum; j++)
        {
            /* code */
            if(indexBytes[j] == (i+1))
            {
                break;
            }
        }
        if(j == networkNum){
            return i+1;
        }
    }
    return BOAT_ERROR;
}


/**
 * @description: 
 *  This function get all the data of network.
 *  data format: protobuf
 * @param {BoatChainmakerNetworkData} *networkData
 * @param {BUINT8} *data
 * network data
 * @param[in/out] {BUINT32} *datalen
 * in: length of data
 * out: real length of wallet data
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATChainmaker_Get_Network_Data(BoatChainmakerNetworkData *networkData,BUINT8 **data,BUINT32 *datalen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    // BUINT32 paramLength = 0,paramLengthLen = 0;
    BUINT32 networkLength = 0,networkLengthLen = 0,offset = 0;
    BUINT8 *networkbuf = NULL;
    Common__ChainmakerNetworkData protobuf_network = COMMON__CHAINMAKER_NETWORK_DATA__INIT;

    boat_try_declare;
    if (datalen == NULL){
        BoatLog(BOAT_LOG_NORMAL,"networkData is NULL");
        return BOAT_ERROR;
    }
    protobuf_network.index = networkData->index;
    protobuf_network.client_sign_cert_content = networkData->client_sign_cert_content.content;

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)     
    protobuf_network.client_tls_cert_content = (BCHAR*)networkData->client_tls_cert_content.content;
#endif

#if (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
    protobuf_network.client_tls_privkey_vlaue = (BCHAR*)networkData->client_tls_privkey_vlaue.value;
#endif

    networkLength = common__chainmaker_network_data__get_packed_size(&protobuf_network);
    if(networkLength <= 0){
        BoatLog(BOAT_LOG_NORMAL,"fail to get packed size of network");
        boat_throw(BOAT_ERROR, chainmaker_exception);          
    }

    networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
    networkbuf = BoatMalloc(networkLength + networkLengthLen);
    if(networkbuf == NULL){
        BoatLog(BOAT_LOG_NORMAL,"fail to malloc memory for network buf");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY,chainmaker_exception);           
    }
    networkLength = common__chainmaker_network_data__pack(&protobuf_network,networkbuf);
    if(networkLength <= 0){
        BoatLog(BOAT_LOG_NORMAL,"fail to packed  network");
        boat_throw(BOAT_ERROR,chainmaker_exception);          
    }
    offset = 0;
    result = add_L_withOffset(networkbuf,&offset,networkLength);
    *data = networkbuf;
    *datalen = offset;
    	/* boat catch handle */
	boat_catch(chainmaker_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
        if(networkbuf != NULL){
            BoatFree(networkbuf);
        }
        return result;
    }

    return BOAT_SUCCESS;
}


/**
 * @description: 
 *  This function initialize network data context
 * @param[inout] {BoatChainmakerNetworkData} *mNetworkDataCtx
 * @return 
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATChainmakerNetworkDataInit(BoatChainmakerNetworkData *mNetworkDataCtx){

    mNetworkDataCtx->index = 0;
    mNetworkDataCtx->client_sign_cert_content.length = 0;
    return BOAT_SUCCESS;
}



/**
 * @description: 
 *   This function store network data into Nvram
 * @param {BoatChainmakerNetworkData} *mNetworkDataCtx
 * @param {BoatStoreType} storeType
 *  store in persistent or non-persistent space
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATChainmaker_NetworkDataCtx_Store(BoatChainmakerNetworkData *mNetworkDataCtx,BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0;
    BUINT32 networkLength = 0 ,networkLengthLen = 0, offset = 0 ;
    BoatProtocolType protocol = BOAT_PROTOCOL_HLFABRIC;
    BUINT8 lengthbytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    // BUINT8 testbuf[1024] = {0};
    BUINT8 *networkData = NULL;
    boat_try_declare;
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,networknumBytes,sizeof(networknumBytes),storeType);
    /* if read Nvram failed , no network */
    if(result != BOAT_SUCCESS){
        networkNum = 0;
    }else{
        result = utility_check_NumBytes(networknumBytes,&networkNum);
        if(result != BOAT_SUCCESS){
            // BoatLog(BOAT_LOG_NORMAL,"check network num fail");
            // return result;
            networkNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    if(networkNum >= BOAT_MAX_NETWORK_NUM){
        boat_throw(BOAT_ERROR,hlfabric_exception);
    }
    offset += sizeof(networknumBytes);
    for (int i = 0; i < networkNum; i++)
    {
        /* protocol type */
        offset += sizeof(protocol);
        /* network index */
        offset += sizeof(BUINT8);
        /* network length */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,lengthbytes,sizeof(lengthbytes),storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NONE,"read network length fail");
            boat_throw(result,hlfabric_exception);
        }
        networkLength = UtilityGetLVData_L(lengthbytes);
        if(networkLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"network data length err ");
            boat_throw(BOAT_ERROR,hlfabric_exception);
        }
        networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
        offset += (networkLength + networkLengthLen);
    }
    /* get network data */
    result = BoATChainmaker_Get_Network_Data(mNetworkDataCtx,&networkData,&networkLength);
    if(result != BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NONE," get network data fail ");
        boat_throw(result,hlfabric_exception);
    }
    /* protocol type */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,offset,(BUINT8*)&protocol,sizeof(protocol),storeType);
    if(result != BOAT_SUCCESS){
        boat_throw(result,hlfabric_exception);
    }
    offset += sizeof(protocol);
    /* network index */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,offset,(BUINT8*)&mNetworkDataCtx->index,sizeof(mNetworkDataCtx->index),storeType);
    if(result != BOAT_SUCCESS){
        boat_throw(result,hlfabric_exception);
    }
    offset += sizeof(mNetworkDataCtx->index);
    BoatLog(BOAT_LOG_NORMAL,"store chainmaker network, networkLength = %d ",networkLength);
    /* store network data */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,offset,networkData,networkLength,storeType);
    if(result != BOAT_SUCCESS){
        boat_throw(result,hlfabric_exception);
    }
    networkNum ++;
    utility_get_NumBytes(networkNum,networknumBytes);
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,0,networknumBytes,sizeof(networknumBytes),storeType);
    boat_catch(hlfabric_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }
    if(networkData != NULL){
        BoatFree(networkData);
    }
    return result;
}

/**
 * @description: 
 * This function creat chainmaker network
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
BOAT_RESULT BoatChainmakerNetworkCreate(BoatChainmakerNetworkData *networkConfig, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatChainmakerNetworkData mNetworkDataCtx ;
    BUINT8 networkIndex = 0;
    result = BoATChainmakerNetworkDataInit(&mNetworkDataCtx);
    if(result != BOAT_SUCCESS){
        return result;
    }
    if(storeType == BOAT_STORE_TYPE_FLASH){
        result = BoATChainmaker_GetFreeNetworkIndex_From_Persistent();
        if(result < BOAT_SUCCESS){
            return result;
        }
        networkIndex = result;  // from 1 to BOAT_MAX_NETWORK_NUM
    }else{
        networkIndex = 0;  // the index of onetimenetwork is always 0
    }

    mNetworkDataCtx.index = networkIndex;
    mNetworkDataCtx.client_sign_cert_content = networkConfig->client_sign_cert_content;

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) 
    mNetworkDataCtx.client_tls_cert_content = networkConfig->client_tls_cert_content;
#endif

#if (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
     mNetworkDataCtx.client_tls_privkey_vlaue = networkConfig->client_tls_privkey_vlaue;
#endif

    result = BoATChainmaker_NetworkDataCtx_Store(&mNetworkDataCtx, storeType);
    if(result != BOAT_SUCCESS){
        return result;
    }
    BoatLog(BOAT_LOG_NORMAL,"network creat success");
    return networkIndex;
}

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
BOAT_RESULT BoATChainmakerNetworkDelete(BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 i = 0;
    BUINT8 networkNum = 0 ,networkNumNew = 0, networkIndex = 0;
    BUINT32 offset = 0, offset_moveFrom = 0 , offset_moveTo = 0;
    BUINT32 networkLength = 0 , networkLengthLen = 0 , paramLength = 0 , paramLengthLen = 0 ;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    BUINT8 *networkData = NULL;
    BoatProtocolType protocolType = BOAT_PROTOCOL_UNKNOWN;
    if(index >= BOAT_MAX_NETWORK_NUM){
        return BOAT_ERROR;
    }
    /* onetime network 
       index of onetime network must be 0
    */
    if(index == 0){
        /* set network_num of onetime network to 0 */
        memset(networknumBytes,0x00,sizeof(networknumBytes));
        result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,0,networknumBytes,sizeof(networknumBytes),BOAT_STORE_TYPE_RAM);
        return result;
    }else{ // persistent network
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,networknumBytes,sizeof(networknumBytes),BOAT_STORE_TYPE_FLASH);
    /* if read Nvram failed , no network */
    if(result != BOAT_SUCCESS){
        return result;
    }
    result = utility_check_NumBytes(networknumBytes,&networkNum);
    if(result != BOAT_SUCCESS || networkNum == 0){
        return result;
    }
    offset += sizeof(networknumBytes);
    for ( i = 0; i < networkNum; i++)
    {
        /* protocol type */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,(BUINT8*)&protocolType,sizeof(protocolType),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS){
            return result;
        }
        /* network index */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset+sizeof(protocolType),(BUINT8*)&networkIndex,sizeof(networkIndex),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS){
            return result;
        }
        /* read each network length */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset+sizeof(protocolType)+sizeof(networkIndex),lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS ){
            return result;
        }
        networkLength = UtilityGetLVData_L(lengthBytes);
        if(networkLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"network data length err ");
            return BOAT_ERROR;
        }
        networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);

        if((networkIndex == index) && (protocolType == BOAT_PROTOCOL_HLFABRIC)){
            break;
        }
        offset += sizeof(protocolType);
        offset += sizeof(networkIndex);
        offset += (networkLengthLen + networkLength);
    }
    BoatLog(BOAT_LOG_NORMAL,"i= %d ",i);
    if(i >= networkNum){
        BoatLog(BOAT_LOG_NORMAL,"not find the network ");
        return BOAT_ERROR;
    }
    networkNumNew = networkNum -1;
    utility_get_NumBytes(networkNumNew,networknumBytes);
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,0,networknumBytes,sizeof(networknumBytes),BOAT_STORE_TYPE_FLASH); // only need to reset network length bytes
    if(result != BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"delete network fail ");
        return result;
    }

    if(i == (networkNum -1)){  /* last one network*/
        memset(lengthBytes,0x00,sizeof(lengthBytes));
        result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,offset,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH); // only need to reset network length bytes
        if(result != BOAT_SUCCESS){
            /* recover networkNum */
            utility_get_NumBytes(networkNum,networknumBytes);
        result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,offset,networknumBytes,sizeof(networknumBytes),BOAT_STORE_TYPE_FLASH); 
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"delete network fail ");
            return result;
        }
            BoatLog(BOAT_LOG_NORMAL,"delete network fail ");
            return result;
        }
    }else{
        offset_moveTo = offset;
        // the next network
        offset += sizeof(protocolType);
        offset += sizeof(networkIndex);
        offset += (networkLengthLen + networkLength);
        offset_moveFrom = offset;
        i++;  
        networkLength = 0;
        for ( ; i < networkNum; i++)
        {
            /* protocol type*/
            offset += sizeof(protocolType);
            /* network index */
            offset += sizeof(networkIndex);
            /* network data */
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH);
            if(result != BOAT_SUCCESS ){
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if(paramLength < 0){
                BoatLog(BOAT_LOG_NORMAL,"network data length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += (paramLengthLen + paramLength);
            networkLength += (sizeof(protocolType)+sizeof(networkIndex));
            networkLength += (paramLengthLen + paramLength);
        }
        // all the networks'length after index network
        networkData = BoatMalloc(networkLength);
        if(NULL == networkData){
            BoatLog(BOAT_LOG_NONE,"fail to malloc memory ");
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset_moveFrom,networkData,networkLength,BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS){
            BoatFree(networkData);
            BoatLog(BOAT_LOG_NONE,"read network data fail ");
            return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
        }
        result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,offset_moveTo,networkData,networkLength,BOAT_STORE_TYPE_FLASH);
        BoatFree(networkData);
        if(result != BOAT_SUCCESS){
            /* recover networkNum */
            utility_get_NumBytes(networkNum,networknumBytes);
            offset = 0;
            result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK,offset,networknumBytes,sizeof(networknumBytes),BOAT_STORE_TYPE_FLASH); 
            if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"delete network fail ");
            return result;
            }
            BoatLog(BOAT_LOG_NONE,"store network data fail ");
            return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
        }
    }
    }
    return result;
}


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
BOAT_RESULT BoATChainmaker_GetNetworkByIndex(BoatChainmakerNetworkData *networkData ,BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
	BUINT8 networkNum = 0 , networkIndex = 0;
	BUINT32 offset = 0 ;
	BUINT32  networkLength = 0 , networkLengthLen = 0 ;
	BUINT8 lengthBytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    BoatProtocolType protocolType = BOAT_PROTOCOL_UNKNOWN;
    BoatStoreType storetype;

    if(index == 0){  // onetime wallet
        storetype = BOAT_STORE_TYPE_RAM;
    }else{
        storetype = BOAT_STORE_TYPE_FLASH;
    }

	result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,networknumBytes,sizeof(networknumBytes),storetype);
    /* if read Nvram failed , no wallet */
    if(result != BOAT_SUCCESS){
        return result;
    }
    result = utility_check_NumBytes(networknumBytes,&networkNum);
    if(result != BOAT_SUCCESS || networkNum == 0){
        return BOAT_ERROR;
    }
	offset += sizeof(networknumBytes) ;
    BoatLog(BOAT_LOG_NORMAL,"network num = %d ",networkNum);
	for (BUINT8 i = 0; i < networkNum; i++)
	{
		/* protocol type */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,(BUINT8*)&protocolType,sizeof(protocolType),storetype);
        if(result != BOAT_SUCCESS){
            return result;
        }
        offset += sizeof(protocolType);
        /* network index */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,(BUINT8*)&networkIndex,sizeof(networkIndex),storetype);
        if(result != BOAT_SUCCESS){
            return result;
        }
        offset += sizeof(networkIndex);
        BoatLog(BOAT_LOG_NORMAL,"networkindex = %d , index = %d ",networkIndex,index);
        BoatLog(BOAT_LOG_NORMAL,"protocol type = %d ",protocolType);

        /* check protocolType and index */
        if((networkIndex == index)&&(protocolType == BOAT_PROTOCOL_CHAINMAKER)){

            BoatLog(BOAT_LOG_NORMAL,"begin to read the network that index = %d ",index);
            result = BoATChainmaker_GetNetworkFromNvram(networkData,offset,storetype,&networkLengthLen);
            if(result != BOAT_SUCCESS){
                BoatLog(BOAT_LOG_NORMAL,"read network data fail");
                return result;
            }
            return BOAT_SUCCESS;
        }else{

            /* network length */
		    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK,offset,lengthBytes,sizeof(lengthBytes),storetype);
		    if(result != BOAT_SUCCESS){
			    BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] length fail " , i);
			    return result;
		    }
		    networkLength = UtilityGetLVData_L(lengthBytes);
            if(networkLength < 0){
                BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
                return BOAT_ERROR;
            }
            networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
            offset += (networkLength + networkLengthLen);
        }
	}
    return BOAT_ERROR;
}