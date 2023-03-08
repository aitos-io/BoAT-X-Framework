/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-06 14:38:19
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-16 16:01:55
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
#include "cita_network.pb-c.h"

/**
 * @description:
 *  This function get BoatCitaNetworkData struct from Common__CitaNetworkData;
 *  move data from Common__CitaNetworkData to BoatCitaNetworkData;
 * @param {BoatCitaNetworkData} *Networkdata
 * @param {Common__CitaNetworkData} *network_proto
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATCita_getNetworkFromProto(BoatCitaNetworkData *Networkdata, Common__CitaNetworkData *network_proto)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if (Networkdata == NULL || network_proto == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    /* index */
    Networkdata->networkIndex = network_proto->index;
    Networkdata->version = network_proto->version;

    /*node info*/
    if (strlen(network_proto->node_url_str) > BOAT_CITA_NODE_URL_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    if (network_proto->chain_id_str.len > BOAT_CITA_CHAIN_ID_V1_SIZE)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    strcpy(Networkdata->node_url_buf, network_proto->node_url_str);
    memcpy(Networkdata->chain_id_buf, network_proto->chain_id_str.data, network_proto->chain_id_str.len);

    return result;
}

/**
 * @description:
 *  This function get the network data of cita from Nvram;
 *  after reading data from Nvram , unpacked the data to cita struct;
 * @param {BoatCitaNetworkData} *Networkdata
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
__BOATSTATIC BOAT_RESULT BoATCita_GetNetworkFromNvram(BoatCitaNetworkData *Networkdata, BUINT32 startAddr, BoatStoreType storeType, BUINT32 *outlen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 lengthbytes[3] = {0};
    BUINT32 offset = startAddr;
    BUINT32 networkDataLength = 0, networkDataLengthLen = 0;
    BUINT8 *networkcontent = NULL;
    Common__CitaNetworkData *protobuf_network = NULL;
    boat_try_declare;

    if (Networkdata == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "param is NULL");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, citaGetnetworkdata_exception);
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthbytes, sizeof(lengthbytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network length fail , errorcode = %d ", result);
        boat_throw(result, citaGetnetworkdata_exception);
    }
    networkDataLength = UtilityGetLVData_L(lengthbytes);
    if (networkDataLength < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "network data length err ");
        boat_throw(BOAT_ERROR, citaGetnetworkdata_exception);
    }
    networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
    networkcontent = BoatMalloc(networkDataLength);
    if (networkcontent == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, citaGetnetworkdata_exception);
    }
    offset += networkDataLengthLen;
    /* read network data */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networkcontent, networkDataLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network data fail , errorcode = %d ", result);
        boat_throw(result, citaGetnetworkdata_exception);
    }

    /* unpacket network data */
    protobuf_network = common__cita_network_data__unpack(NULL, networkDataLength, networkcontent);
    if (protobuf_network == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "unpack networkdata fail ");
        boat_throw(BOAT_ERROR, citaGetnetworkdata_exception);
    }

    result = BoATCita_getNetworkFromProto(Networkdata, protobuf_network);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to get network from protobuf struct");
        boat_throw(BOAT_ERROR, citaGetnetworkdata_exception);
    }
    *outlen = (networkDataLength + networkDataLengthLen);

    /* boat catch handle */
    boat_catch(citaGetnetworkdata_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    if (networkcontent != NULL)
    {
        BoatFree(networkcontent);
    }

    if (protobuf_network != NULL)
    {
        common__cita_network_data__free_unpacked(protobuf_network, NULL);
    }

    return result;
}

/**
 * @description:
 *  This function get network list ,include persistent networks and onetime network
 * @param[out] {BoatCitaNetworkContext} *networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATCita_GetNetworkList(BoatCitaNetworkContext *networkList)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0;
    BUINT8 index = 0;
    BUINT32 networkDataLength = 0, networkDataLengthLen = 0;
    BUINT32 offset = 0;
    BUINT8 networknumBytes[4] = {0};
    BUINT8 lengthBytes[3] = {0};
    BoatProtocolType protocolType = BOAT_PROTOCOL_UNKNOWN;
    /* check param*/
    if (NULL == networkList)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    /* persistent network  */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_FLASH);
    /* if read Nvram failed , no network */
    if (result != BOAT_SUCCESS)
    {
        networkNum = 0;
        // return BOAT_SUCCESS;
    }
    else
    {
        result = utility_check_NumBytes(networknumBytes, &networkNum);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "check network num fail");
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
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&protocolType, sizeof(protocolType), BOAT_STORE_TYPE_FLASH);
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        offset += sizeof(protocolType); // protocol type
        offset += sizeof(BUINT8);       // index
        if (protocolType == BOAT_PROTOCOL_CITA)
        { // cita
            result = BoATCita_GetNetworkFromNvram(&(networkList->networks[index++]), offset, BOAT_STORE_TYPE_FLASH, &networkDataLength);
            if (result != BOAT_SUCCESS)
            {
                return result;
            }
            offset += networkDataLength;
        }
        else
        {
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                return result;
            }
            networkDataLength = UtilityGetLVData_L(lengthBytes);
            if (networkDataLength < 0)
            {
                BoatLog(BOAT_LOG_NORMAL, "network data  length err ");
                return BOAT_ERROR;
            }
            networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
            offset += (networkDataLength + networkDataLengthLen);
        }
    }
    /* onetime network  */
    offset = 0;
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_RAM);
    if (result != BOAT_SUCCESS)
    {
        networkNum = 0;
        // return BOAT_SUCCESS;
    }
    else
    {
        result = utility_check_NumBytes(networknumBytes, &networkNum);
        if (result != BOAT_SUCCESS)
        {
            networkNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    networkList->networkNum += networkNum;
    offset += sizeof(networknumBytes);
    for (int i = 0; i < networkNum; i++)
    {
        /* protocol type */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&protocolType, sizeof(protocolType), BOAT_STORE_TYPE_RAM);
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        offset += sizeof(protocolType); // protocol type
        offset += sizeof(BUINT8);       // index
        if (protocolType == BOAT_PROTOCOL_CITA)
        { // cita
            result = BoATCita_GetNetworkFromNvram(&(networkList->networks[index++]), offset, BOAT_STORE_TYPE_RAM, &networkDataLength);
            if (result != BOAT_SUCCESS)
            {
                return result;
            }
            offset += networkDataLength;
        }
        else
        {
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_RAM);
            if (result != BOAT_SUCCESS)
            {
                return result;
            }
            networkDataLength = UtilityGetLVData_L(lengthBytes);
            if (networkDataLength < 0)
            {
                BoatLog(BOAT_LOG_NORMAL, "network data  length err ");
                return BOAT_ERROR;
            }
            networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
            offset += (networkDataLength + networkDataLengthLen);
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
__BOATSTATIC BOAT_RESULT BoATCita_GetFreeNetworkIndex_From_Persistent(void)
{
    BOAT_RESULT result = BOAT_ERROR;
    BUINT8 networkNum = 0;
    BUINT8 i = 0, j = 0;
    BUINT32 networkDataLength = 0, networkDataLengthLen = 0;
    BUINT32 offset = 0;
    BUINT8 networknumBytes[4] = {0};
    BUINT8 lengthBytes[3] = {0};
    BUINT8 indexBytes[BOAT_MAX_NETWORK_NUM] = {0};
    /* persistent network num */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_FLASH);
    /* if read Nvram failed , no network */
    if (result != BOAT_SUCCESS)
    {
        networkNum = 0;
        // return BOAT_SUCCESS;
    }
    else
    {
        result = utility_check_NumBytes(networknumBytes, &networkNum);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "check network num fail");
            networkNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    if (networkNum >= BOAT_MAX_NETWORK_NUM)
    {
        return BOAT_ERROR_NETWORK_INDEX_EXCEED;
    }
    offset += sizeof(networknumBytes);
    for (size_t i = 0; i < networkNum; i++)
    {
        /* protocol type */
        offset += sizeof(BoatProtocolType);
        /* index */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, &indexBytes[i], sizeof(BUINT8), BOAT_STORE_TYPE_FLASH);
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        offset += sizeof(BUINT8);
        /* network length */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        networkDataLength = UtilityGetLVData_L(lengthBytes);
        if (networkDataLength < 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "network data  length err ");
            return BOAT_ERROR;
        }
        networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
        offset += (networkDataLength + networkDataLengthLen);
    }
    for (i = 0; i < BOAT_MAX_NETWORK_NUM; i++)
    {
        /* code */
        for (j = 0; j < networkNum; j++)
        {
            /* code */
            if (indexBytes[j] == (i + 1))
            {
                break;
            }
        }
        if (j == networkNum)
        {
            return i + 1;
        }
    }
    return BOAT_ERROR;
}

/**
 * @description:
 *  This function get all the data of network.
 *  data format: protobuf
 * @param {BoatCitaNetworkData} *networkData
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
__BOATSTATIC BOAT_RESULT BoATCita_Get_Network_Data(BoatCitaNetworkData *networkData, BUINT8 **data, BUINT32 *datalen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 networkLength = 0, networkLengthLen = 0, offset = 0;
    BUINT8 *networkbuf = NULL;
    Common__CitaNetworkData protobuf_network = COMMON__CITA_NETWORK_DATA__INIT;

    boat_try_declare;
    if (datalen == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "networkData is NULL");
        return BOAT_ERROR;
    }

    protobuf_network.index = networkData->networkIndex;
    protobuf_network.version = networkData->version;
    protobuf_network.node_url_str = networkData->node_url_buf;
    protobuf_network.chain_id_str.data = networkData->chain_id_buf;

    // memcpy(protobuf_network.chain_id_str.data, networkData->chain_id_buf, BOAT_CITA_CHAIN_ID_V1_SIZE);
    protobuf_network.chain_id_str.len = BOAT_CITA_CHAIN_ID_V1_SIZE;

    networkLength = common__cita_network_data__get_packed_size(&protobuf_network);
    if (networkLength <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to get packed size of network");
        boat_throw(BOAT_ERROR, cita_exception);
    }

    networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
    networkbuf = BoatMalloc(networkLength + networkLengthLen);
    if (networkbuf == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory for network buf");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, cita_exception);
    }
    networkLength = common__cita_network_data__pack(&protobuf_network, networkbuf);
    if (networkLength <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to packed  network");
        boat_throw(BOAT_ERROR, cita_exception);
    }
    offset = 0;
    result = add_L_withOffset(networkbuf, &offset, networkLength);
    *data = networkbuf;
    *datalen = offset;
    /* boat catch handle */
    boat_catch(cita_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
        if (networkbuf != NULL)
        {
            BoatFree(networkbuf);
        }
        return result;
    }

    return BOAT_SUCCESS;
}

/**
 * @description:
 *  This function initialize network data context
 * @param[inout] {BoatCitaNetworkData} *mNetworkDataCtx
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATCitaNetworkDataInit(BoatCitaNetworkData *mNetworkDataCtx)
{
    mNetworkDataCtx->networkIndex = 0;
    mNetworkDataCtx->version = 0;

    memset(mNetworkDataCtx->chain_id_buf, 0x00, BOAT_CITA_CHAIN_ID_V1_SIZE);
    memset(mNetworkDataCtx->node_url_buf, 0x00, BOAT_CITA_NODE_URL_MAX_LEN);
    return BOAT_SUCCESS;
}

/**
 * @description:
 *   This function store network data into Nvram
 * @param {BoatCitaNetworkData} *mNetworkDataCtx
 * @param {BoatStoreType} storeType
 *  store in persistent or non-persistent space
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATCita_NetworkDataCtx_Store(BoatCitaNetworkData *mNetworkDataCtx, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0;
    BUINT32 networkLength = 0, networkLengthLen = 0, offset = 0;
    BoatProtocolType protocol = BOAT_PROTOCOL_CITA;
    BUINT8 lengthbytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    BUINT8 *networkData = NULL;
    boat_try_declare;
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), storeType);

    /* if read Nvram failed , no network */
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "no network exist");
        networkNum = 0;
    }
    else
    {
        result = utility_check_NumBytes(networknumBytes, &networkNum);
        if (result != BOAT_SUCCESS)
        {
            networkNum = 0;
            result = BOAT_SUCCESS;
        }
        BoatLog(BOAT_LOG_NORMAL, "read networkNum = %d\n", networkNum);
    }
    if (networkNum >= BOAT_MAX_NETWORK_NUM)
    {
        boat_throw(BOAT_ERROR_NETWORK_INDEX_EXCEED, cita_exception);
    }
    if (storeType == BOAT_STORE_TYPE_RAM)
    {
        networkNum = 0;
    }
    offset += sizeof(networknumBytes);
    for (int i = 0; i < networkNum; i++)
    {
        /* protocol type */
        offset += sizeof(protocol);
        /* network index */
        offset += sizeof(BUINT8);
        /* network length */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthbytes, sizeof(lengthbytes), storeType);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NONE, "read network length fail");
            boat_throw(result, cita_exception);
        }
        networkLength = UtilityGetLVData_L(lengthbytes);
        if (networkLength < 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "network data length err ");
            boat_throw(BOAT_ERROR, cita_exception);
        }
        networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
        offset += (networkLength + networkLengthLen);
    }

    /* get network data */
    result = BoATCita_Get_Network_Data(mNetworkDataCtx, &networkData, &networkLength);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NONE, " get network data fail ");
        boat_throw(result, cita_exception);
    }
    /* protocol type */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&protocol, sizeof(protocol), storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, cita_exception);
    }
    offset += sizeof(protocol);
    /* network index */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&mNetworkDataCtx->networkIndex, sizeof(mNetworkDataCtx->networkIndex), storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, cita_exception);
    }
    offset += sizeof(mNetworkDataCtx->networkIndex);
    BoatLog(BOAT_LOG_NORMAL, "store cita network, networkLength = %d ", networkLength);
    /* store network data */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, networkData, networkLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, cita_exception);
    }
    networkNum++;
    utility_get_NumBytes(networkNum, networknumBytes);
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, 0, networknumBytes, sizeof(networknumBytes), storeType);
    boat_catch(cita_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }
    if (networkData != NULL)
    {
        BoatFree(networkData);
    }
    return result;
}

/**
 * @description:
 * This function creat cita network
 * @param[in] {BoatCitaNetworkData} networkConfig
 * @param[in] {BoatStoreType} storeType
 *  For onetime network or test , select store in ram .
 *  For persistent network ,slect store in flash.
 * @return
 *   This function returns network index if creat network successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATCitaNetworkCreate(BoatCitaNetworkConfig *networkConfig, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatCitaNetworkData mNetworkDataCtx;
    BUINT8 networkIndex = 0;

    if (networkConfig == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "network create ontime");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    result = BoATCitaNetworkDataInit(&mNetworkDataCtx);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
    if (storeType == BOAT_STORE_TYPE_FLASH)
    {
        result = BoATCita_GetFreeNetworkIndex_From_Persistent();
        if (result < BOAT_SUCCESS)
        {
            return result;
        }
        networkIndex = result; // from 1 to BOAT_MAX_NETWORK_NUM
    }
    else
    {
        networkIndex = 0; // the index of onetimenetwork is always 0
        BoatLog(BOAT_LOG_NORMAL, "network create ontime");
    }

    mNetworkDataCtx.networkIndex = networkIndex;
    mNetworkDataCtx.version = networkConfig->version;

    memcpy(mNetworkDataCtx.chain_id_buf, networkConfig->chain_id_buf, BOAT_CITA_CHAIN_ID_V1_SIZE);
    strcpy(mNetworkDataCtx.node_url_buf, networkConfig->node_url_buf);

    result = BoATCita_NetworkDataCtx_Store(&mNetworkDataCtx, storeType);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
    BoatLog(BOAT_LOG_NORMAL, "network creat success networkIndex = %d\n", networkIndex);
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
BOAT_RESULT BoATCitaNetworkDelete(BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 i = 0;
    BUINT8 networkNum = 0, networkNumNew = 0, networkIndex = 0;
    BUINT32 offset = 0, offset_moveFrom = 0, offset_moveTo = 0;
    BUINT32 networkLength = 0, networkLengthLen = 0, paramLength = 0, paramLengthLen = 0;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    BUINT8 *networkData = NULL;
    BoatProtocolType protocolType = BOAT_PROTOCOL_UNKNOWN;
    if (index > BOAT_MAX_NETWORK_NUM)
    {
        return BOAT_ERROR_NETWORK_INDEX_EXCEED;
    }
    /* onetime network
       index of onetime network must be 0
    */
    if (index == 0)
    {
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_RAM);
        /* if read Nvram failed , no network */
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        result = utility_check_NumBytes(networknumBytes, &networkNum);
        if (result != BOAT_SUCCESS || networkNum == 0)
        {
            return BOAT_ERROR_NETWORK_INEXISTENCE;
        }
        /* set network_num of onetime network to 0 */
        memset(networknumBytes, 0x00, sizeof(networknumBytes));
        result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, 0, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_RAM);
        return result;
    }
    else
    { // persistent network
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_FLASH);
        /* if read Nvram failed , no network */
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        result = utility_check_NumBytes(networknumBytes, &networkNum);
        if (result != BOAT_SUCCESS || networkNum == 0)
        {
            return BOAT_ERROR_NETWORK_INEXISTENCE;
        }
        offset += sizeof(networknumBytes);
        for (i = 0; i < networkNum; i++)
        {
            /* protocol type */
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&protocolType, sizeof(protocolType), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                return result;
            }
            /* network index */
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset + sizeof(protocolType), (BUINT8 *)&networkIndex, sizeof(networkIndex), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                return result;
            }
            /* read each network length */
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset + sizeof(protocolType) + sizeof(networkIndex), lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                return result;
            }
            networkLength = UtilityGetLVData_L(lengthBytes);
            if (networkLength < 0)
            {
                BoatLog(BOAT_LOG_NORMAL, "network data length err ");
                return BOAT_ERROR;
            }
            networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);

            if ((networkIndex == index) && (protocolType == BOAT_PROTOCOL_CITA))
            {
                break;
            }
            offset += sizeof(protocolType);
            offset += sizeof(networkIndex);
            offset += (networkLengthLen + networkLength);
        }
        BoatLog(BOAT_LOG_NORMAL, "i= %d ", i);
        if (i >= networkNum)
        {
            BoatLog(BOAT_LOG_NORMAL, "not find the network ");
            return BOAT_ERROR_NETWORK_INEXISTENCE;
        }
        networkNumNew = networkNum - 1;
        utility_get_NumBytes(networkNumNew, networknumBytes);
        result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, 0, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_FLASH); // only need to reset network length bytes
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "delete network fail ");
            return result;
        }

        if (i == (networkNum - 1))
        { /* last one network*/
            memset(lengthBytes, 0x00, sizeof(lengthBytes));
            result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH); // only need to reset network length bytes
            if (result != BOAT_SUCCESS)
            {
                /* recover networkNum */
                utility_get_NumBytes(networkNum, networknumBytes);
                result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_FLASH);
                if (result != BOAT_SUCCESS)
                {
                    BoatLog(BOAT_LOG_NORMAL, "delete network fail ");
                    return result;
                }
                BoatLog(BOAT_LOG_NORMAL, "delete network fail ");
                return result;
            }
        }
        else
        {
            offset_moveTo = offset;
            // the next network
            offset += sizeof(protocolType);
            offset += sizeof(networkIndex);
            offset += (networkLengthLen + networkLength);
            offset_moveFrom = offset;
            i++;
            networkLength = 0;
            for (; i < networkNum; i++)
            {
                /* protocol type*/
                offset += sizeof(protocolType);
                /* network index */
                offset += sizeof(networkIndex);
                /* network data */
                result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
                if (result != BOAT_SUCCESS)
                {
                    return result;
                }
                paramLength = UtilityGetLVData_L(lengthBytes);
                if (paramLength < 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, "network data length err ");
                    return BOAT_ERROR;
                }
                paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
                offset += (paramLengthLen + paramLength);
                networkLength += (sizeof(protocolType) + sizeof(networkIndex));
                networkLength += (paramLengthLen + paramLength);
            }
            // all the networks'length after index network
            networkData = BoatMalloc(networkLength);
            if (NULL == networkData)
            {
                BoatLog(BOAT_LOG_NONE, "fail to malloc memory ");
                return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            }
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset_moveFrom, networkData, networkLength, BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatFree(networkData);
                BoatLog(BOAT_LOG_NONE, "read network data fail ");
                return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
            }
            result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset_moveTo, networkData, networkLength, BOAT_STORE_TYPE_FLASH);
            BoatFree(networkData);
            if (result != BOAT_SUCCESS)
            {
                /* recover networkNum */
                utility_get_NumBytes(networkNum, networknumBytes);
                offset = 0;
                result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), BOAT_STORE_TYPE_FLASH);
                if (result != BOAT_SUCCESS)
                {
                    BoatLog(BOAT_LOG_NORMAL, "delete network fail ");
                    return result;
                }
                BoatLog(BOAT_LOG_NONE, "store network data fail ");
                return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
            }
        }
    }
    return result;
}

/**
 * @description:
 *  This function read network data from Nvram by index.
 * @param {BoatCitaNetworkData} *networkData
 * @param {BUINT8} index
 *  the network index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATCita_GetNetworkByIndex(BoatCitaNetworkData *networkData, BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0, networkIndex = 0;
    BUINT32 offset = 0;
    BUINT32 networkLength = 0, networkLengthLen = 0;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    BoatProtocolType protocolType = BOAT_PROTOCOL_UNKNOWN;
    BoatStoreType storetype;
    /*check param*/
    if (NULL == networkData)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (index > BOAT_MAX_NETWORK_NUM)
    {
        return BOAT_ERROR_NETWORK_INDEX_EXCEED;
    }
    if (index == 0)
    { // onetime wallet
        storetype = BOAT_STORE_TYPE_RAM;
    }
    else
    {
        storetype = BOAT_STORE_TYPE_FLASH;
    }

    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), storetype);
    /* if read Nvram failed , no wallet */
    if (result != BOAT_SUCCESS)
    {
        return result;
    }

    result = utility_check_NumBytes(networknumBytes, &networkNum);
    if (result != BOAT_SUCCESS || networkNum == 0)
    {
        return BOAT_ERROR_NETWORK_INEXISTENCE;
    }
    offset += sizeof(networknumBytes);
    BoatLog(BOAT_LOG_NORMAL, "network num = %d ", networkNum);

    for (BUINT8 i = 0; i < networkNum; i++)
    {
        /* protocol type */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&protocolType, sizeof(protocolType), storetype);
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        offset += sizeof(protocolType);
        /* network index */
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&networkIndex, sizeof(networkIndex), storetype);
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        offset += sizeof(networkIndex);
        BoatLog(BOAT_LOG_NORMAL, "networkindex = %d , index = %d ", networkIndex, index);
        BoatLog(BOAT_LOG_NORMAL, "protocol type = %d ", protocolType);

        /* check protocolType and index */
        if ((networkIndex == index) && (protocolType == BOAT_PROTOCOL_CITA))
        {

            BoatLog(BOAT_LOG_NORMAL, "begin to read the network that index = %d ", index);
            result = BoATCita_GetNetworkFromNvram(networkData, offset, storetype, &networkLengthLen);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read network data fail");
                return result;
            }
            return BOAT_SUCCESS;
        }
        else
        {

            /* network length */
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthBytes, sizeof(lengthBytes), storetype);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "get wallet[%d] length fail ", i);
                return result;
            }
            networkLength = UtilityGetLVData_L(lengthBytes);
            if (networkLength < 0)
            {
                BoatLog(BOAT_LOG_NORMAL, "wallet data length err ");
                return BOAT_ERROR;
            }
            networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
            offset += (networkLength + networkLengthLen);
        }
    }
    return BOAT_ERROR_NETWORK_INEXISTENCE;
}

/**
 * @description:
 *  This function use to free BoatCitaNetworkContext param.
 * @param[in] {BoatCitaNetworkContext} networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATCita_FreeNetworkContext(BoatCitaNetworkContext networkList)
{
    if (networkList.networkNum == 0)
    {
        return BOAT_SUCCESS;
    }
    for (int i = 0; i < networkList.networkNum; i++)
    {
        /* code */
        networkList.networks[i].networkIndex = 0;
        networkList.networks[i].version = 0;

        memset(networkList.networks[i].node_url_buf, 0x00, BOAT_CITA_NODE_URL_MAX_LEN);
        memset(networkList.networks[i].chain_id_buf, 0x00, BOAT_CITA_CHAIN_ID_V1_SIZE);
    }
    networkList.networkNum = 0;
    return BOAT_SUCCESS;
}
