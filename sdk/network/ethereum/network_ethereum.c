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

/**
 * @description:
 *  This function get one network data from Nvram include persistent space and non-persistent space
 * @param {BoatEthNetworkData} *Networkdata
 * @param {BUINT32} startAddr
 *  from this addr to read one network data
 * @param {BoatStoreType} storeType
 *  read from persistent space or non-persistent space
 * @param {BUINT32} *outlen
 *  length of each network data
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATEth_GetNetworkFromNvram(BoatEthNetworkData *Networkdata, BUINT32 startAddr, BoatStoreType storeType, BUINT32 *outlen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 networkDataLength = 0, networkDataLengthLen = 0, paramDataLength = 0, paramDataLengthLen = 0;
    BUINT32 offset = 0, offset_obj = 0;
    BUINT8 lengthbytes[3] = {0};

    offset = startAddr;
    /*read the data length of each network */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthbytes, sizeof(lengthbytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network length fail , errorcode = %d ", result);
        return result;
    }
    networkDataLength = UtilityGetLVData_L(lengthbytes);
    if (networkDataLength < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "network data length err ");
        return BOAT_ERROR;
    }
    networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
    offset += networkDataLengthLen;
    offset_obj = offset;
    /* begin to read network index */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthbytes, sizeof(lengthbytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network length fail , errorcode = %d ", result);
        return result;
    }
    paramDataLength = UtilityGetLVData_L(lengthbytes);
    if (paramDataLength < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "network data length err ");
        return BOAT_ERROR;
    }
    if (paramDataLength != 1)
    {
        BoatLog(BOAT_LOG_NORMAL, "network index data err ");
        return BOAT_ERROR;
    }
    paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
    offset += paramDataLengthLen;
    if (offset - offset_obj + paramDataLength > networkDataLength)
    {
        BoatLog(BOAT_LOG_NORMAL, "network index data err ");
        return BOAT_ERROR;
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&(Networkdata->networkIndex), paramDataLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network index fail , errorcode = %d ", result);
        return result;
    }
    offset += paramDataLength;
    /* begin to read network chainID */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthbytes, sizeof(lengthbytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network length fail , errorcode = %d ", result);
        return result;
    }
    paramDataLength = UtilityGetLVData_L(lengthbytes);
    if (paramDataLength < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "network data length err ");
        return BOAT_ERROR;
    }
    paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
    offset += paramDataLengthLen;
    if (offset - offset_obj + paramDataLength > networkDataLength)
    {
        BoatLog(BOAT_LOG_NORMAL, "network chainID data err ");
        return BOAT_ERROR;
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&(Networkdata->chain_id), paramDataLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network chainID fail , errorcode = %d ", result);
        return result;
    }
    offset += paramDataLength;
    /* read network eip155_compatibility */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthbytes, sizeof(lengthbytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network eip155 length fail , errorcode = %d ", result);
        return result;
    }
    paramDataLength = UtilityGetLVData_L(lengthbytes);
    if (paramDataLength < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "network eip155 data length err ");
        return BOAT_ERROR;
    }
    paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
    offset += paramDataLengthLen;
    if (offset - offset_obj + paramDataLength > networkDataLength)
    {
        BoatLog(BOAT_LOG_NORMAL, "network eip155 len err ");
        return BOAT_ERROR;
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&(Networkdata->eip155_compatibility), paramDataLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network eip155  fail , errorcode = %d ", result);
        return result;
    }
    offset += paramDataLength;
    /* read network url */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthbytes, sizeof(lengthbytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network url length fail , errorcode = %d ", result);
        return result;
    }
    paramDataLength = UtilityGetLVData_L(lengthbytes);
    if (paramDataLength < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "network url  length err ");
        return BOAT_ERROR;
    }
    paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
    offset += paramDataLengthLen;
    if (offset - offset_obj + paramDataLength > networkDataLength)
    {
        BoatLog(BOAT_LOG_NORMAL, "network url data err ");
        return BOAT_ERROR;
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)Networkdata->node_url_str, paramDataLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network url fail , errorcode = %d ", result);
        return result;
    }
    offset += paramDataLength;

    /* end */

    *outlen = (networkDataLength + networkDataLengthLen);

    return result;
}

/**
 * @description:
 *  This function get network list ,include persistent networks and onetime network
 * @param[out] {BoatEthNetworkContext} *networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATEth_GetNetworkList(BoatEthNetworkContext *networkList)
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
        if (protocolType == BOAT_PROTOCOL_ETHEREUM)
        { // eth
            result = BoATEth_GetNetworkFromNvram(&(networkList->networks[index++]), offset, BOAT_STORE_TYPE_FLASH, &networkDataLength);
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
        result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&protocolType, sizeof(protocolType), BOAT_STORE_TYPE_RAM);
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        offset += sizeof(protocolType); // protocol type
        offset += sizeof(BUINT8);       // index
        if (protocolType == BOAT_PROTOCOL_ETHEREUM)
        { // eth
            result = BoATEth_GetNetworkFromNvram(&(networkList->networks[index++]), offset, BOAT_STORE_TYPE_RAM, &networkDataLength);
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
 *  This function use to free BoatEthNetworkContext param.
 * @param[in] {BoatEthNetworkContext} networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATEth_FreeNetworkContext(BoatEthNetworkContext networkList)
{
    if (networkList.networkNum == 0)
    {
        return BOAT_SUCCESS;
    }
    for (int i = 0; i < networkList.networkNum; i++)
    {
        /* code */
        networkList.networks[i].networkIndex = 0;
        networkList.networks[i].chain_id = 0;
        networkList.networks[i].eip155_compatibility = BOAT_FALSE;
        memset(networkList.networks[i].node_url_str, 0x00, sizeof(networkList.networks[i].node_url_str));
    }
    networkList.networkNum = 0;
    return BOAT_SUCCESS;
}

// /**
//  * @description:
//  *  This function initialize network list
//  * @param[inout] {BoatEthNetworkContext} *mNetworkList
//  * @return
//  *   This function returns BOAT_SUCCESS if initialization is successful.\n
//  *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
//  *   for details.
//  * @author: aitos
//  */
// __BOATSTATIC BOAT_RESULT BoATEth_Init_BoatEthNetworkContext(BoatEthNetworkContext *mNetworkList){
//     mNetworkList->networkNum = 0;
//     for (int i = 0; i < BOAT_MAX_NETWORK_NUM; i++)
//     {
//         /* code */
//         mNetworkList->networks[i].networkIndex = 0;
//         mNetworkList->networks[i].chain_id = 0;
//         mNetworkList->networks[i].eip155_compatibility = BOAT_FALSE;
//         memset(mNetworkList->networks[i].node_url_str,0x00,sizeof(mNetworkList->networks[i].node_url_str));
//     }
//     return BOAT_SUCCESS;
// }

/**
 * @description:
 *  This function get unused network index from persistent networks.
 * @return {*}
 *  This function returns unused network index if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATEth_GetFreeNetworkIndex_From_Persistent(void)
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
            // return result;
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
 *  This function get the length of network data.
 *  data like this:
 *  L  LV LV LV
 * @param[in] {BoatEthNetworkData*} networkData
 * @param[out] {BUINT32} *len
 *  length of all the network data
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATEth_Get_NetworkData_Len(BoatEthNetworkData *networkData, BUINT32 *len)
{
    BUINT32 paramLength = 0, paramLengthLen = 0;
    BUINT32 networkLength = 0;
    if (networkData == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "networkData is NULL");
        return BOAT_ERROR;
    }
    /* network index*/
    paramLength = sizeof(networkData->networkIndex);
    paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
    networkLength += (paramLength + paramLengthLen);
    /*network chain_id*/
    paramLength = sizeof(networkData->chain_id);
    paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
    networkLength += (paramLength + paramLengthLen);
    /* network eip155_compatibility */
    paramLength = sizeof(networkData->eip155_compatibility);
    paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
    networkLength += (paramLength + paramLengthLen);
    /* acount url*/
    paramLength = strlen(networkData->node_url_str) + 1;
    paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
    networkLength += (paramLength + paramLengthLen);
    /* all the data*/
    paramLengthLen = UtilityGetTLV_LL_from_len(networkLength);
    networkLength += paramLengthLen;
    *len = networkLength;
    return BOAT_SUCCESS;
}

/**
 * @description:
 *  This function get all the data of network.
 *  like this:
 *  L  LV LV LV
 * @param {BoatEthNetworkData} *mNetworkDataCtx
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
__BOATSTATIC BOAT_RESULT BoATEth_Get_Network_Data(BoatEthNetworkData *mNetworkDataCtx, BUINT8 *data, BUINT32 *datalen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 networkLength = 0, offset = 0;
    if (mNetworkDataCtx == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "network data is NULL");
        return BOAT_ERROR;
    }
    if (data == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "out data  buf is NULL");
        return BOAT_ERROR;
    }

    /* network index*/
    data[offset] = mNetworkDataCtx->networkIndex;
    result = add_L_withOffset(data, &offset, sizeof(mNetworkDataCtx->networkIndex));
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "get network index LV fail");
        return BOAT_ERROR;
    }
    /*network chainID*/
    memcpy(data + offset, &(mNetworkDataCtx->chain_id), sizeof(mNetworkDataCtx->chain_id));
    result = add_L_withOffset(data, &offset, sizeof(mNetworkDataCtx->chain_id));
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "get network chainID LV fail");
        return BOAT_ERROR;
    }
    /*network eip155*/
    memcpy(data + offset, &(mNetworkDataCtx->eip155_compatibility), sizeof(mNetworkDataCtx->eip155_compatibility));
    result = add_L_withOffset(data, &offset, sizeof(mNetworkDataCtx->eip155_compatibility));
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "get network eip155 LV fail");
        return BOAT_ERROR;
    }
    /* network url */
    strcpy((BCHAR *)(data + offset), mNetworkDataCtx->node_url_str);
    result = add_L_withOffset(data, &offset, strlen(mNetworkDataCtx->node_url_str) + 1);
    if (result < BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "get wallet pubkey LV fail");
        return BOAT_ERROR;
    }

    /* all the data*/
    networkLength = offset;
    offset = 0;
    result = add_L_withOffset(data, &offset, networkLength);
    if (result < BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }
    *datalen = offset;
    return BOAT_SUCCESS;
}

/**
 * @description:
 *  This function initialize network data context
 * @param[inout] {BoatEthNetworkData} *mNetworkDataCtx
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATEthNetworkDataInit(BoatEthNetworkData *mNetworkDataCtx)
{
    mNetworkDataCtx->networkIndex = 0;
    mNetworkDataCtx->chain_id = 0;
    mNetworkDataCtx->eip155_compatibility = BOAT_FALSE;
    memset(mNetworkDataCtx->node_url_str, 0x00, sizeof(mNetworkDataCtx->node_url_str));
    return BOAT_SUCCESS;
}

/**
 * @description:
 *   This function store network data into Nvram
 * @param {BoatEthNetworkData} *mNetworkDataCtx
 * @param {BoatStoreType} storeType
 *  store in persistent or non-persistent space
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATEth_NetworkDataCtx_Store(BoatEthNetworkData *mNetworkDataCtx, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0;
    BUINT32 networkLength = 0, networkLengthLen = 0, offset = 0;
    BoatProtocolType protocol = BOAT_PROTOCOL_ETHEREUM;
    BUINT8 lengthbytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    // BUINT8 testbuf[1024] = {0};
    BUINT8 *networkData = NULL;
    boat_try_declare;
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networknumBytes, sizeof(networknumBytes), storeType);
    /* if read Nvram failed , no network */
    if (result != BOAT_SUCCESS)
    {
        networkNum = 0;
    }
    else
    {
        result = utility_check_NumBytes(networknumBytes, &networkNum);
        if (result != BOAT_SUCCESS)
        {
            // BoatLog(BOAT_LOG_NORMAL,"check network num fail");
            // return result;
            networkNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    if (networkNum >= BOAT_MAX_NETWORK_NUM)
    {
        boat_throw(BOAT_ERROR_NETWORK_INDEX_EXCEED, eth_exception);
    }
    if (BOAT_STORE_TYPE_RAM == storeType)
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
            BoatLog(BOAT_LOG_NORMAL, "read network length fail");
            boat_throw(result, eth_exception);
        }
        networkLength = UtilityGetLVData_L(lengthbytes);
        if (networkLength < 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "network data length err ");
            boat_throw(result, eth_exception);
        }
        networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
        offset += (networkLength + networkLengthLen);
    }
    /* get network data */
    result = BoATEth_Get_NetworkData_Len(mNetworkDataCtx, &networkLength);
    if (result < BOAT_SUCCESS)
    {
        boat_throw(result, eth_exception);
    }
    networkData = BoatMalloc(networkLength);
    if (networkData == NULL)
    {
        boat_throw(BOAT_ERROR, eth_exception);
    }
    result = BoATEth_Get_Network_Data(mNetworkDataCtx, networkData, &networkLength);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, " get network data fail ");
        boat_throw(result, eth_exception);
    }
    /* protocol type */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&protocol, sizeof(protocol), storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, eth_exception);
    }
    offset += sizeof(protocol);
    /* network index */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&mNetworkDataCtx->networkIndex, sizeof(mNetworkDataCtx->networkIndex), storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, eth_exception);
    }
    offset += sizeof(mNetworkDataCtx->networkIndex);

    /* store network data */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, networkData, networkLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, eth_exception);
    }
    networkNum++;
    utility_get_NumBytes(networkNum, networknumBytes);
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, 0, networknumBytes, sizeof(networknumBytes), storeType);
    boat_catch(eth_exception)
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
 * This function create eth network
 * @param[in] {BoatEthNetworkConfig} networkConfig
 * @param[in] {BoatStoreType} storeType
 *  For onetime network or test , select store in ram .
 *  For persistent network ,slect store in flash.
 * @return
 *   This function returns network index if create network successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATEthNetworkCreate(BoatEthNetworkConfig *networkConfig, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatEthNetworkData mNetworkDataCtx;
    BUINT8 networkIndex = 0;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    result = BoATEthNetworkDataInit(&mNetworkDataCtx);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
    if (storeType == BOAT_STORE_TYPE_FLASH)
    {
        result = BoATEth_GetFreeNetworkIndex_From_Persistent();
        if (result < BOAT_SUCCESS)
        {
            return result;
        }
        networkIndex = result; // from 1 to BOAT_MAX_NETWORK_NUM
    }
    else
    {
        networkIndex = 0; // the index of onetimenetwork is always 0
    }

    mNetworkDataCtx.networkIndex = networkIndex;
    mNetworkDataCtx.chain_id = networkConfig->chain_id;
    mNetworkDataCtx.eip155_compatibility = networkConfig->eip155_compatibility;
    strcpy(mNetworkDataCtx.node_url_str, networkConfig->node_url_str);
    result = BoATEth_NetworkDataCtx_Store(&mNetworkDataCtx, storeType);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
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
BOAT_RESULT BoATEthNetworkDelete(BUINT8 index)
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

            BoatLog(BOAT_LOG_NORMAL, "networkindex = %d ,index = %d ", networkIndex, index);
            BoatLog(BOAT_LOG_NORMAL, "protocol type = %d ", protocolType);
            if ((networkIndex == index) && (protocolType == BOAT_PROTOCOL_ETHEREUM))
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
                BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory ");
                return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            }
            result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset_moveFrom, networkData, networkLength, BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatFree(networkData);
                BoatLog(BOAT_LOG_NORMAL, "read network data fail ");
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
                BoatLog(BOAT_LOG_NORMAL, "store network data fail ");
                return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
            }
        }
    }
    return result;
}

/**
 * @description:
 *  This function read network data from Nvram by index.
 * @param {BoatEthNetworkData} *networkData
 * @param {BUINT8} index
 *  the network index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATEth_GetNetworkByIndex(BoatEthNetworkData *networkData, BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0, networkIndex = 0;
    BUINT32 offset = 0;
    BUINT32 networkLength = 0, networkLengthLen = 0;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    BoatProtocolType protocolType = BOAT_PROTOCOL_UNKNOWN;
    BoatStoreType storetype;
    /* check params*/
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
        if ((networkIndex == index) && (protocolType == BOAT_PROTOCOL_ETHEREUM))
        {
            BoatLog(BOAT_LOG_NORMAL, "begin to read the network that index = %d ", index);
            result = BoATEth_GetNetworkFromNvram(networkData, offset, storetype, &networkLengthLen);
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
