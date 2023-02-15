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
#include "hwbcs_network.pb-c.h"

__BOATSTATIC BOAT_RESULT BoATHwbcs_checkNodefig(BoatHwbcsNodesCfg nodeCfg)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check endorserLayoutNum , if endorserLayoutNum equal 0, have no nodes information*/
    if (nodeCfg.endorserLayoutNum == 0)
    {
        return BOAT_ERROR_NETWORK_NODECFG_ERR;
    }
    if (NULL == nodeCfg.layoutCfg)
    {
        return BOAT_ERROR_NETWORK_NODECFG_ERR;
    }
    /******************  check layoutCfg ****************************/
    for (size_t i = 0; i < nodeCfg.endorserLayoutNum; i++)
    {
        if (nodeCfg.layoutCfg[i].endorserGroupNum == 0)
        {
            return BOAT_ERROR_NETWORK_NODECFG_ERR;
        }
        if (NULL == nodeCfg.layoutCfg[i].groupCfg)
        {
            return BOAT_ERROR_NETWORK_NODECFG_ERR;
        }
        for (size_t j = 0; j < nodeCfg.layoutCfg[i].endorserGroupNum; j++)
        {
            if (nodeCfg.layoutCfg[i].groupCfg[j].endorserNumber == 0)
            {
                return BOAT_ERROR_NETWORK_NODECFG_ERR;
            }
            if (NULL == nodeCfg.layoutCfg[i].groupCfg[j].endorser)
            {
                return BOAT_ERROR_NETWORK_NODECFG_ERR;
            }
            /*    if quantities is bigger than endorserNumber ,the transaction cannot be executed successfully*/
            if (nodeCfg.layoutCfg[i].groupCfg[j].quantities > nodeCfg.layoutCfg[i].groupCfg[j].endorserNumber)
            {
                return BOAT_ERROR_NETWORK_NODECFG_ERR;
            }
            for (size_t k = 0; k < nodeCfg.layoutCfg[i].groupCfg[j].endorserNumber; k++)
            {
                if (NULL == nodeCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName)
                {
                    return BOAT_ERROR_NETWORK_NODECFG_ERR;
                }
                if (NULL == nodeCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl)
                {
                    return BOAT_ERROR_NETWORK_NODECFG_ERR;
                }
            }
        }
    }
    /**********************check orderCfg **********************************/
    if (nodeCfg.orderCfg.endorserNumber == 0)
    {
        return BOAT_ERROR_NETWORK_NODECFG_ERR;
    }
    if (NULL == nodeCfg.orderCfg.endorser)
    {
        return BOAT_ERROR_NETWORK_NODECFG_ERR;
    }
    for (size_t i = 0; i < nodeCfg.orderCfg.endorserNumber; i++)
    {
        if (NULL == nodeCfg.orderCfg.endorser[i].hostName)
        {
            return BOAT_ERROR_NETWORK_NODECFG_ERR;
        }
        if (NULL == nodeCfg.orderCfg.endorser[i].nodeUrl)
        {
            return BOAT_ERROR_NETWORK_NODECFG_ERR;
        }
    }

    return result;
}

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
BOAT_RESULT BoATHwbcs_FreeNetworkData(BoatHwbcsNetworkData networkData)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    networkData.index = 0;
    networkData.accountCertContent.length = 0;
#if (BOAT_HWBCS_TLS_SUPPORT == 1) && (BOAT_HWBCS_TLS_IDENTIFY_CLIENT == 1)
    networkData.accountClientTlsPrikey.value_len = 0;
    networkData.accountClientTlsCert.length = 0;
#endif
    /* layout */
    for (size_t i = 0; i < networkData.nodesCfg.endorserLayoutNum; i++)
    {
        /* code */
        for (size_t j = 0; j < networkData.nodesCfg.layoutCfg[i].endorserGroupNum; j++)
        {
            /* code */
            networkData.nodesCfg.layoutCfg[i].groupCfg[j].quantities = 0;
            networkData.nodesCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length = 0;
            for (size_t k = 0; k < networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber; k++)
            {
                /* code */
                if (NULL != networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName)
                {
                    BoatFree(networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName);
                    networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName = NULL;
                }
                if (NULL != networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl)
                {
                    BoatFree(networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl);
                    networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl = NULL;
                }
            }
            if (networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber != 0)
            {
                BoatFree(networkData.nodesCfg.layoutCfg[i].groupCfg[j].endorser);
            }
        }
        if (networkData.nodesCfg.layoutCfg[i].endorserGroupNum != 0)
        {
            BoatFree(networkData.nodesCfg.layoutCfg[i].groupCfg);
        }
    }
    if (networkData.nodesCfg.endorserLayoutNum != 0)
    {
        BoatFree(networkData.nodesCfg.layoutCfg);
    }

    /* order */
    networkData.nodesCfg.orderCfg.quantities = 0;
    networkData.nodesCfg.orderCfg.tlsOrgCertContent.length = 0;
    for (size_t i = 0; i < networkData.nodesCfg.orderCfg.endorserNumber; i++)
    {
        /* code */
        if (NULL != networkData.nodesCfg.orderCfg.endorser[i].hostName)
        {
            BoatFree(networkData.nodesCfg.orderCfg.endorser[i].hostName);
            networkData.nodesCfg.orderCfg.endorser[i].hostName = NULL;
        }
        if (NULL != networkData.nodesCfg.orderCfg.endorser[i].nodeUrl)
        {
            BoatFree(networkData.nodesCfg.orderCfg.endorser[i].nodeUrl);
            networkData.nodesCfg.orderCfg.endorser[i].nodeUrl = NULL;
        }
    }
    if (networkData.nodesCfg.orderCfg.endorserNumber != 0)
    {
        BoatFree(networkData.nodesCfg.orderCfg.endorser);
    }

    return result;
}

/**
 * @description:
 *  This function get BoatHwbcsNetworkData struct from Common__HwbcsNetworkData;
 *  move data from Common__HwbcsNetworkData to BoatHwbcsNetworkData;
 * @param {BoatHwbcsNetworkData} *Networkdata
 * @param {Common__HwbcsNetworkData} *network_proto
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATHwbcs_getNetworkFromProto(BoatHwbcsNetworkData *Networkdata, Common__HwbcsNetworkData *network_proto)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
    if (Networkdata == NULL || network_proto == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    /* index */
    Networkdata->index = network_proto->index;
    /* account Cert */
    if (strlen(network_proto->accountcertcontent) > sizeof(Networkdata->accountCertContent.content))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    Networkdata->accountCertContent.length = strlen(network_proto->accountcertcontent);
    strcpy(Networkdata->accountCertContent.content, network_proto->accountcertcontent);
    /* layout */
    Networkdata->nodesCfg.endorserLayoutNum = network_proto->n_layoutcfg;
    Networkdata->nodesCfg.layoutCfg = BoatMalloc(Networkdata->nodesCfg.endorserLayoutNum * sizeof(BoatHwbcsNodeLayoutCfg));
    if (NULL == Networkdata->nodesCfg.layoutCfg)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory for layoutCfg");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
    }
    for (size_t i = 0; i < Networkdata->nodesCfg.endorserLayoutNum; i++)
    {
        /* group */
        Networkdata->nodesCfg.layoutCfg[i].endorserGroupNum = network_proto->layoutcfg[i]->n_groupcfg;
        Networkdata->nodesCfg.layoutCfg[i].groupCfg = BoatMalloc(Networkdata->nodesCfg.layoutCfg[i].endorserGroupNum * sizeof(BoatHwbcsNodeGroupCfg));
        if (NULL == Networkdata->nodesCfg.layoutCfg[i].groupCfg)
        {
            BoatLog(BOAT_LOG_NORMAL, "failt to malloc memory for groupCfg");
            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
        }
        for (size_t j = 0; j < Networkdata->nodesCfg.layoutCfg[i].endorserGroupNum; j++)
        {
            Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber = network_proto->layoutcfg[i]->groupcfg[j]->n_endorser;
            Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].quantities = network_proto->layoutcfg[i]->groupcfg[j]->quantities;
            if (strlen(network_proto->layoutcfg[i]->groupcfg[j]->tlsorgcertcontent) > sizeof(Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content))
            {
                boat_throw(BOAT_ERROR, hwbcs_exception);
            }
            Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.length = strlen(network_proto->layoutcfg[i]->groupcfg[j]->tlsorgcertcontent);
            strcpy(Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, network_proto->layoutcfg[i]->groupcfg[j]->tlsorgcertcontent);
            Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorser =
                BoatMalloc(Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
            if (NULL == Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorser)
            {
                BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory for endorser");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
            }
            for (size_t k = 0; k < Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber; k++)
            {
                /* code */
                Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName =
                    BoatMalloc(strlen(network_proto->layoutcfg[i]->groupcfg[j]->endorser[k]->hostname) + 1);
                if (NULL == Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName)
                {
                    BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory for hostname");
                    boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
                }
                strcpy(Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName, network_proto->layoutcfg[i]->groupcfg[j]->endorser[k]->hostname);
                Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl =
                    BoatMalloc(strlen(network_proto->layoutcfg[i]->groupcfg[j]->endorser[k]->nodeurl) + 1);
                if (NULL == Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl)
                {
                    BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory for url");
                    boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
                }
                strcpy(Networkdata->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl, network_proto->layoutcfg[i]->groupcfg[j]->endorser[k]->nodeurl);
            }
        }
    }
    /* order */
    Networkdata->nodesCfg.orderCfg.endorserNumber = network_proto->ordercfg->n_endorser;
    Networkdata->nodesCfg.orderCfg.quantities = network_proto->ordercfg->quantities;
    if (strlen(network_proto->ordercfg->tlsorgcertcontent) > sizeof(Networkdata->nodesCfg.orderCfg.tlsOrgCertContent.content))
    {
        BoatLog(BOAT_LOG_NORMAL, "fail tomalloc memory for order's tls cert");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
    }
    strcpy(Networkdata->nodesCfg.orderCfg.tlsOrgCertContent.content, network_proto->ordercfg->tlsorgcertcontent);
    Networkdata->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(network_proto->ordercfg->tlsorgcertcontent);
    Networkdata->nodesCfg.orderCfg.endorser =
        BoatMalloc(Networkdata->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    for (size_t i = 0; i < Networkdata->nodesCfg.orderCfg.endorserNumber; i++)
    {
        /* code */
        Networkdata->nodesCfg.orderCfg.endorser[i].hostName =
            BoatMalloc(strlen(network_proto->ordercfg->endorser[i]->hostname) + 1);
        if (NULL == Networkdata->nodesCfg.orderCfg.endorser[i].hostName)
        {
            BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory for order's hostname");
            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
        }
        strcpy(Networkdata->nodesCfg.orderCfg.endorser[i].hostName, network_proto->ordercfg->endorser[i]->hostname);
        Networkdata->nodesCfg.orderCfg.endorser[i].nodeUrl =
            BoatMalloc(strlen(network_proto->ordercfg->endorser[i]->nodeurl) + 1);
        if (NULL == Networkdata->nodesCfg.orderCfg.endorser[i].nodeUrl)
        {
            BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory for order's url");
            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
        }
        strcpy(Networkdata->nodesCfg.orderCfg.endorser[i].nodeUrl, network_proto->ordercfg->endorser[i]->nodeurl);
    }

#if (BOAT_HWBCS_TLS_SUPPORT == 1) && (BOAT_HWBCS_TLS_IDENTIFY_CLIENT == 1)
    if (strlen(network_proto->accountclienttlsprikey) - 1 > sizeof(Networkdata->accountClientTlsPrikey.value))
    {
        BoatLog(BOAT_LOG_NORMAL, "account client tls prikey len exceed");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
    }
    Networkdata->accountClientTlsPrikey.value_len = strlen(network_proto->accountclienttlsprikey);
    strcpy((BCHAR *)Networkdata->accountClientTlsPrikey.value, network_proto->accountclienttlsprikey);

    if (strlen(network_proto->accountclienttlscert) - 1 > sizeof(Networkdata->accountClientTlsCert.content))
    {
        BoatLog(BOAT_LOG_NORMAL, "account client tls cert len exceed");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
    }
    Networkdata->accountClientTlsCert.length = strlen(network_proto->accountclienttlscert);
    strcpy((BCHAR *)Networkdata->accountClientTlsCert.content, network_proto->accountclienttlscert);
#endif

    /* boat catch handle */
    boat_catch(hwbcs_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    return result;
}

/**
 * @description:
 *  This function get the network data of hwbcs from Nvram;
 *  after reading data from Nvram , unpacked the data to hwbcs struct;
 * @param {BoatHwbcsNetworkData} *Networkdata
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
__BOATSTATIC BOAT_RESULT BoATHwbcs_GetNetworkFromNvram(BoatHwbcsNetworkData *Networkdata, BUINT32 startAddr, BoatStoreType storeType, BUINT32 *outlen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 lengthbytes[3] = {0};
    BUINT32 offset = startAddr;
    BUINT32 networkDataLength = 0, networkDataLengthLen = 0;
    BUINT8 *networkcontent = NULL;
    Common__HwbcsNetworkData *protobuf_network = NULL;
    boat_try_declare;
    if (Networkdata == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "param is NULL");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, hwbcsGetnetworkdata_exception);
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, lengthbytes, sizeof(lengthbytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network length fail , errorcode = %d ", result);
        boat_throw(result, hwbcsGetnetworkdata_exception);
    }
    networkDataLength = UtilityGetLVData_L(lengthbytes);
    if (networkDataLength < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "network data length err ");
        boat_throw(BOAT_ERROR, hwbcsGetnetworkdata_exception);
    }
    networkDataLengthLen = UtilityGetTLV_LL_from_len(networkDataLength);
    networkcontent = BoatMalloc(networkDataLength);
    if (networkcontent == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcsGetnetworkdata_exception);
    }
    offset += networkDataLengthLen;
    /* read network data */
    result = BoatReadSoftRotNvram(BOAT_STORE_NETWORK, offset, networkcontent, networkDataLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "Read network data fail , errorcode = %d ", result);
        boat_throw(result, hwbcsGetnetworkdata_exception);
    }

    /* unpacket network data */
    protobuf_network = common__hwbcs_network_data__unpack(NULL, networkDataLength, networkcontent);
    if (protobuf_network == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "unpack networkdata fail ");
        boat_throw(BOAT_ERROR, hwbcsGetnetworkdata_exception);
    }
    result = BoATHwbcs_getNetworkFromProto(Networkdata, protobuf_network);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to get network from protobuf struct");
        boat_throw(BOAT_ERROR, hwbcsGetnetworkdata_exception);
    }
    *outlen = (networkDataLength + networkDataLengthLen);

    /* boat catch handle */
    boat_catch(hwbcsGetnetworkdata_exception)
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
        common__hwbcs_network_data__free_unpacked(protobuf_network, NULL);
    }

    return result;
}

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
BOAT_RESULT BoATHwbcs_GetNetworkList(BoatHwbcsNetworkContext *networkList)
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
        if (protocolType == BOAT_PROTOCOL_HWBCS)
        { // hwbcs
            result = BoATHwbcs_GetNetworkFromNvram(&(networkList->networks[index++]), offset, BOAT_STORE_TYPE_FLASH, &networkDataLength);
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
        if (protocolType == BOAT_PROTOCOL_HWBCS)
        { // hwbcs
            result = BoATHwbcs_GetNetworkFromNvram(&(networkList->networks[index++]), offset, BOAT_STORE_TYPE_RAM, &networkDataLength);
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
 *  This function use to free BoatHwbcsNetworkContext param.
 * @param[in] {BoatHwbcsNetworkContext} networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcs_FreeNetworkContext(BoatHwbcsNetworkContext networkList)
{
    if (networkList.networkNum == 0)
    {
        return BOAT_SUCCESS;
    }
    for (int i = 0; i < networkList.networkNum; i++)
    {
        /* code */
        BoATHwbcs_FreeNetworkData(networkList.networks[i]);
    }
    networkList.networkNum = 0;
    return BOAT_SUCCESS;
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
__BOATSTATIC BOAT_RESULT BoATHwbcs_GetFreeNetworkIndex_From_Persistent(void)
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
 *  This function free memory of Common__HwbcsNetworkData;
 * @param {Common__HwbcsNetworkData} protobuf
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoAT_Free_HwbcsNetworkData(Common__HwbcsNetworkData hwbcs_network_protobuf)
{
    BOAT_RESULT result = BOAT_ERROR;

    for (size_t i = 0; i < hwbcs_network_protobuf.n_layoutcfg; i++)
    {
        /* code */
        for (size_t j = 0; j < hwbcs_network_protobuf.layoutcfg[i]->n_groupcfg; j++)
        {
            /* code */
            for (size_t k = 0; k < hwbcs_network_protobuf.layoutcfg[i]->groupcfg[j]->n_endorser; k++)
            {
                /* code */
                if (hwbcs_network_protobuf.layoutcfg[i]->groupcfg[j]->endorser[k] != NULL)
                {
                    BoatFree(hwbcs_network_protobuf.layoutcfg[i]->groupcfg[j]->endorser[k]);
                }
            }
            if (hwbcs_network_protobuf.layoutcfg[i]->groupcfg[j]->endorser != NULL)
            {
                BoatFree(hwbcs_network_protobuf.layoutcfg[i]->groupcfg[j]->endorser);
            }
            if (hwbcs_network_protobuf.layoutcfg[i]->groupcfg[j] != NULL)
            {
                BoatFree(hwbcs_network_protobuf.layoutcfg[i]->groupcfg[j]);
            }
        }
        if (hwbcs_network_protobuf.layoutcfg[i]->groupcfg != NULL)
        {
            BoatFree(hwbcs_network_protobuf.layoutcfg[i]->groupcfg);
        }
        if (hwbcs_network_protobuf.layoutcfg[i] != NULL)
        {
            BoatFree(hwbcs_network_protobuf.layoutcfg[i]);
        }
    }
    if (hwbcs_network_protobuf.layoutcfg != NULL)
    {
        BoatFree(hwbcs_network_protobuf.layoutcfg);
    }

    for (size_t i = 0; i < hwbcs_network_protobuf.ordercfg->n_endorser; i++)
    {
        /* code */
        if (hwbcs_network_protobuf.ordercfg->endorser[i] != NULL)
        {
            BoatFree(hwbcs_network_protobuf.ordercfg->endorser[i]);
        }
    }
    if (hwbcs_network_protobuf.ordercfg != NULL)
    {
        BoatFree(hwbcs_network_protobuf.ordercfg);
    }

    return result;
}

/**
 * @description:
 *  This function get all the data of network.
 *  data format: protobuf
 * @param {BoatHwbcsNetworkData} *networkData
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
__BOATSTATIC BOAT_RESULT BoATHwbcs_Get_Network_Data(BoatHwbcsNetworkData *networkData, BUINT8 **data, BUINT32 *datalen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    // BUINT32 paramLength = 0,paramLengthLen = 0;
    BUINT32 networkLength = 0, networkLengthLen = 0, offset = 0;
    BUINT8 *networkbuf = NULL;
    Common__HwbcsNetworkData protobuf_network = COMMON__HWBCS_NETWORK_DATA__INIT;
    // Common__HwbcsNodesCfg nodecfg = COMMON__HWBCS_NODES_CFG__INIT;
    Common__HwbcsNodeLayoutCfg **nodelayoutcfg;
    // Common__HwbcsNodeInfo *endorserCfg = COMMON__HWBCS_NODE_INFO__INIT;
    boat_try_declare;
    if (datalen == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "networkData is NULL");
        return BOAT_ERROR;
    }
    protobuf_network.index = networkData->index;
    protobuf_network.accountcertcontent = networkData->accountCertContent.content;
    protobuf_network.n_layoutcfg = networkData->nodesCfg.endorserLayoutNum;
    nodelayoutcfg = BoatMalloc(protobuf_network.n_layoutcfg * sizeof(Common__HwbcsNodeLayoutCfg *));
    if (NULL == nodelayoutcfg)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to malloc layoutcfg");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
    }
    for (size_t i = 0; i < protobuf_network.n_layoutcfg; i++)
    {
        /* code */
        nodelayoutcfg[i] = BoatMalloc(sizeof(Common__HwbcsNodeLayoutCfg));
        common__hwbcs_node_layout_cfg__init(nodelayoutcfg[i]);
        nodelayoutcfg[i]->n_groupcfg = networkData->nodesCfg.layoutCfg[i].endorserGroupNum;
        Common__HwbcsNodeGroupCfg **groupCfg;
        groupCfg = BoatMalloc(nodelayoutcfg[i]->n_groupcfg * sizeof(Common__HwbcsNodeGroupCfg *));
        if (groupCfg == NULL)
        {
            BoatLog(BOAT_LOG_NORMAL, "fail to malloc groupCfg");
            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
        }
        for (size_t j = 0; j < nodelayoutcfg[i]->n_groupcfg; j++)
        {
            /* code */
            groupCfg[j] = BoatMalloc(sizeof(Common__HwbcsNodeGroupCfg));
            if (NULL == groupCfg[j])
            {
                BoatLog(BOAT_LOG_NORMAL, "fail to malloc groupCfg");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
            }
            common__hwbcs_node_group_cfg__init(groupCfg[j]);
            groupCfg[j]->quantities = networkData->nodesCfg.layoutCfg[i].groupCfg[j].quantities;
            groupCfg[j]->tlsorgcertcontent = networkData->nodesCfg.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content;
            groupCfg[j]->n_endorser = networkData->nodesCfg.layoutCfg[i].groupCfg[j].endorserNumber;
            Common__HwbcsNodeInfo **endorserCfg;
            endorserCfg = BoatMalloc(groupCfg[j]->n_endorser * sizeof(Common__HwbcsNodeInfo *));
            if (endorserCfg == NULL)
            {
                BoatLog(BOAT_LOG_NORMAL, "fail to malloc endorserCfg");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
            }
            for (size_t k = 0; k < groupCfg[j]->n_endorser; k++)
            {
                /* code */
                endorserCfg[k] = BoatMalloc(sizeof(Common__HwbcsNodeInfo));
                if (NULL == endorserCfg[k])
                {
                    BoatLog(BOAT_LOG_NORMAL, "fail to malloc endorserCfg");
                    boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
                }
                common__hwbcs_node_info__init(endorserCfg[k]);
                endorserCfg[k]->hostname = networkData->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].hostName;
                endorserCfg[k]->nodeurl = networkData->nodesCfg.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl;
            }
            groupCfg[j]->endorser = endorserCfg;
        }
        nodelayoutcfg[i]->groupcfg = groupCfg;
    }
    protobuf_network.layoutcfg = nodelayoutcfg;
    protobuf_network.ordercfg = BoatMalloc(sizeof(Common__HwbcsNodeGroupCfg));
    if (protobuf_network.ordercfg == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to malloc ordercfg");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
    }
    common__hwbcs_node_group_cfg__init(protobuf_network.ordercfg);
    protobuf_network.ordercfg->quantities = networkData->nodesCfg.orderCfg.quantities;
    protobuf_network.ordercfg->tlsorgcertcontent = networkData->nodesCfg.orderCfg.tlsOrgCertContent.content;
    protobuf_network.ordercfg->n_endorser = networkData->nodesCfg.orderCfg.endorserNumber;
    Common__HwbcsNodeInfo **endorserCfg;
    endorserCfg = BoatMalloc(protobuf_network.ordercfg->n_endorser * sizeof(Common__HwbcsNodeInfo *));
    if (endorserCfg == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to malloc ordercfg endorser");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
    }
    for (size_t i = 0; i < protobuf_network.ordercfg->n_endorser; i++)
    {
        /* code */
        endorserCfg[i] = BoatMalloc(sizeof(Common__HwbcsNodeInfo));
        if (NULL == endorserCfg[i])
        {
            BoatLog(BOAT_LOG_NORMAL, "fail to malloc endorserCfg");
            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
        }
        common__hwbcs_node_info__init(endorserCfg[i]);
        endorserCfg[i]->hostname = networkData->nodesCfg.orderCfg.endorser[i].hostName;
        endorserCfg[i]->nodeurl = networkData->nodesCfg.orderCfg.endorser[i].nodeUrl;
    }
    protobuf_network.ordercfg->endorser = endorserCfg;
#if (BOAT_HWBCS_TLS_SUPPORT == 1) && (BOAT_HWBCS_TLS_IDENTIFY_CLIENT == 1)
    protobuf_network.accountclienttlsprikey = (BCHAR *)networkData->accountClientTlsPrikey.value;
    protobuf_network.accountclienttlscert = (BCHAR *)networkData->accountClientTlsCert.content;

#endif

    networkLength = common__hwbcs_network_data__get_packed_size(&protobuf_network);
    if (networkLength <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to get packed size of network");
        boat_throw(BOAT_ERROR, hwbcs_exception);
    }
    networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
    networkbuf = BoatMalloc(networkLength + networkLengthLen);
    if (networkbuf == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory for network buf");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, hwbcs_exception);
    }
    networkLength = common__hwbcs_network_data__pack(&protobuf_network, networkbuf);
    if (networkLength <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "fail to packed  network");
        boat_throw(BOAT_ERROR, hwbcs_exception);
    }
    offset = 0;
    add_L_withOffset(networkbuf, &offset, networkLength);
    *data = networkbuf;
    *datalen = offset;
    /* boat catch handle */
    boat_catch(hwbcs_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
        if (networkbuf != NULL)
        {
            BoatFree(networkbuf);
        }
    }
    // common__hwbcs_network_data__free_unpacked(&protobuf_network,NULL);
    BoAT_Free_HwbcsNetworkData(protobuf_network);
    return result;
}

/**
 * @description:
 *  This function initialize network data context
 * @param[inout] {BoatHwbcsNetworkData} *mNetworkDataCtx
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcsNetworkDataInit(BoatHwbcsNetworkData *mNetworkDataCtx)
{
    mNetworkDataCtx->index = 0;
    mNetworkDataCtx->accountCertContent.length = 0;
    mNetworkDataCtx->nodesCfg.endorserLayoutNum = 0;
    mNetworkDataCtx->nodesCfg.layoutCfg = NULL;
    mNetworkDataCtx->nodesCfg.orderCfg.quantities = 0;
    mNetworkDataCtx->nodesCfg.orderCfg.tlsOrgCertContent.length = 0;
    mNetworkDataCtx->nodesCfg.orderCfg.endorserNumber = 0;
    mNetworkDataCtx->nodesCfg.orderCfg.endorser = NULL;
    return BOAT_SUCCESS;
}

/**
 * @description:
 *   This function store network data into Nvram
 * @param {BoatHwbcsNetworkData} *mNetworkDataCtx
 * @param {BoatStoreType} storeType
 *  store in persistent or non-persistent space
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATHwbcs_NetworkDataCtx_Store(BoatHwbcsNetworkData *mNetworkDataCtx, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0;
    BUINT32 networkLength = 0, networkLengthLen = 0, offset = 0;
    BoatProtocolType protocol = BOAT_PROTOCOL_HWBCS;
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
        boat_throw(BOAT_ERROR_NETWORK_INDEX_EXCEED, hwbcs_exception);
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
            BoatLog(BOAT_LOG_NORMAL, "read network length fail");
            boat_throw(result, hwbcs_exception);
        }
        networkLength = UtilityGetLVData_L(lengthbytes);
        if (networkLength < 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "network data length err ");
            boat_throw(BOAT_ERROR, hwbcs_exception);
        }
        networkLengthLen = UtilityGetTLV_LL_from_len(networkLength);
        offset += (networkLength + networkLengthLen);
    }
    /* get network data */
    result = BoATHwbcs_Get_Network_Data(mNetworkDataCtx, &networkData, &networkLength);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, " get network data fail ");
        boat_throw(result, hwbcs_exception);
    }
    /* protocol type */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&protocol, sizeof(protocol), storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, hwbcs_exception);
    }
    offset += sizeof(protocol);
    /* network index */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, (BUINT8 *)&mNetworkDataCtx->index, sizeof(mNetworkDataCtx->index), storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, hwbcs_exception);
    }
    offset += sizeof(mNetworkDataCtx->index);
    BoatLog(BOAT_LOG_NORMAL, "store hwbcs network, networkLength = %d ", networkLength);
    /* store network data */
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, offset, networkData, networkLength, storeType);
    if (result != BOAT_SUCCESS)
    {
        boat_throw(result, hwbcs_exception);
    }
    networkNum++;
    utility_get_NumBytes(networkNum, networknumBytes);
    result = BoATStoreSoftRotNvram(BOAT_STORE_NETWORK, 0, networknumBytes, sizeof(networknumBytes), storeType);
    boat_catch(hwbcs_exception)
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
BOAT_RESULT BoATHwbcsNetworkCreate(BoatHwbcsNetworkConfig *networkConfig, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatHwbcsNetworkData mNetworkDataCtx;
    BUINT8 networkIndex = 0;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    /****************** check params in networkConfig *********************/
    if (BoATHwbcs_checkNodefig(networkConfig->nodesCfg) != BOAT_SUCCESS)
    {
        return BOAT_ERROR_NETWORK_NODECFG_ERR;
    }
    result = BoATHwbcsNetworkDataInit(&mNetworkDataCtx);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
    if (storeType == BOAT_STORE_TYPE_FLASH)
    {
        result = BoATHwbcs_GetFreeNetworkIndex_From_Persistent();
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

    mNetworkDataCtx.index = networkIndex;
    mNetworkDataCtx.accountCertContent = networkConfig->accountCertContent;
#if (BOAT_HWBCS_TLS_SUPPORT == 1) && (BOAT_HWBCS_TLS_IDENTIFY_CLIENT == 1)
    mNetworkDataCtx.accountClientTlsCert = networkConfig->accountClientTlsCert;
    mNetworkDataCtx.accountClientTlsPrikey = networkConfig->accountClientTlsPrikey;
#endif
    mNetworkDataCtx.nodesCfg = networkConfig->nodesCfg;
    result = BoATHwbcs_NetworkDataCtx_Store(&mNetworkDataCtx, storeType);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
    BoatLog(BOAT_LOG_NORMAL, "network create success");
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
BOAT_RESULT BoATHwbcsNetworkDelete(BUINT8 index)
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

            if ((networkIndex == index) && (protocolType == BOAT_PROTOCOL_HWBCS))
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
 * @param {BoatHwbcsNetworkData} *networkData
 * @param {BUINT8} index
 *  the network index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATHwbcs_GetNetworkByIndex(BoatHwbcsNetworkData *networkData, BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 networkNum = 0, networkIndex = 0;
    BUINT32 offset = 0;
    BUINT32 networkLength = 0, networkLengthLen = 0;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 networknumBytes[4] = {0};
    BoatProtocolType protocolType = BOAT_PROTOCOL_UNKNOWN;
    BoatStoreType storetype;
    /* check param */
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
        if ((networkIndex == index) && (protocolType == BOAT_PROTOCOL_HWBCS))
        {
            BoatLog(BOAT_LOG_NORMAL, "begin to read the network that index = %d ", index);
            result = BoATHwbcs_GetNetworkFromNvram(networkData, offset, storetype, &networkLengthLen);
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