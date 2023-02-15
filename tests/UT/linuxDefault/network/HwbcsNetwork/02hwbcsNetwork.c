/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-04 11:28:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-13 14:37:50
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
#include "tcase_network.h"
#include "boatiotsdk.h"
#include "boatlog.h"

const BCHAR *hw_bcs_client_demokey = "-----BEGIN PRIVATE KEY-----\n"
                                     "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg9d6xlIhBpXL1jyGg\n"
                                     "1cIqhPXvAzf/U/y1qfJAPnC1uiKhRANCAAQEiZePJltVa2rl8ew2lFkE0qMVhYm5\n"
                                     "EzaEs67tFKPDe421v2datMggAo0gJop9NrRO1P2NN5WkvaSfCG8kwyMK\n"
                                     "-----END PRIVATE KEY-----\n";

const BCHAR *hw_bcs_client_democert = "-----BEGIN CERTIFICATE-----\n"
                                      "MIIClDCCAjqgAwIBAgIQUoHlTcUu5HjLbv2HSUilBzAKBggqhkjOPQQDAjCBhjEO\n"
                                      "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
                                      "bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
                                      "Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
                                      "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYoxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
                                      "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
                                      "DQYDVQQLEwZIdWF3ZWkxMTAvBgNVBAMTKHVzZXItMS5vcmcxLmJjcy1lcG1tbGku\n"
                                      "c3ZjLmNsdXN0ZXIubG9jYWwwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQEiZeP\n"
                                      "JltVa2rl8ew2lFkE0qMVhYm5EzaEs67tFKPDe421v2datMggAo0gJop9NrRO1P2N\n"
                                      "N5WkvaSfCG8kwyMKo4GDMIGAMA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAA\n"
                                      "MCsGA1UdIwQkMCKAIK1MsN0yPniDh8jtrSxAvxwR2XpL8dCm0lTvqN4tNlWVMDMG\n"
                                      "A1UdEQQsMCqCKHVzZXItMS5vcmcxLmJjcy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9j\n"
                                      "YWwwCgYIKoZIzj0EAwIDSAAwRQIhAPECJCuqWtLi9cfnxG31IlBGfD4OKII8svV3\n"
                                      "URYn+CQKAiBzKCZk1g+5GH/DvqqJhspp6zB1GsoUXwN/o641s3l4bg==\n"
                                      "-----END CERTIFICATE-----\n";

const BCHAR *hw_bcs_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
                                   "MIICrDCCAlKgAwIBAgIQdvsQdvTNG+rVbXHhnOS9mzAKBggqhkjOPQQDAjCBhjEO\n"
                                   "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
                                   "bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
                                   "Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
                                   "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYYxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
                                   "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
                                   "DQYDVQQLEwZIdWF3ZWkxLTArBgNVBAMTJGNhLm9yZzEuYmNzLWVwbW1saS5zdmMu\n"
                                   "Y2x1c3Rlci5sb2NhbDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABID+pcLWFgiw\n"
                                   "4nlEQNma0QFtlLrDV0YXfyKB1EwmyGynOosbf1mz9qzbzCjtUyZhDJnOhTSbq8uf\n"
                                   "D7OOf9m5JeijgZ8wgZwwDgYDVR0PAQH/BAQDAgGmMB0GA1UdJQQWMBQGCCsGAQUF\n"
                                   "BwMBBggrBgEFBQcDAjAPBgNVHRMBAf8EBTADAQH/MCkGA1UdDgQiBCC+pd8v/ch2\n"
                                   "igg6x8bgJHiBMFB/ABiul6O58eNruCn+IzAvBgNVHREEKDAmgiRjYS5vcmcxLmJj\n"
                                   "cy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9jYWwwCgYIKoZIzj0EAwIDSAAwRQIgIR4F\n"
                                   "YQDnjxHgoOha5N1Ec1aee+8R+n+un6Vu/q5G7RcCIQD0h5Ylb4ytyFxo21QhtFJR\n"
                                   "Jq7sq/hJ+yziR0ZrVnKXeA==\n"
                                   // "MIIClDCCAjqgAwIBAgIQUfKiHXReppD/PuCucebvDTAKBggqhkjOPQQDAjCBhjEO\n"
                                   // "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
                                   // "bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
                                   // "Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
                                   // "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYoxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
                                   // "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
                                   // "DQYDVQQLEwZIdWF3ZWkxMTAvBgNVBAMTKHVzZXItMS5vcmcxLmJjcy1lcG1tbGku\n"
                                   // "c3ZjLmNsdXN0ZXIubG9jYWwwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQ8JbFm\n"
                                   // "u20E7yHZWMkadZJ4QUBL96kWE97bmGukJs0nc6DS34OXmZHIiDetQkNkCX5bcsOw\n"
                                   // "m0hVXQnrkGQdFEIFo4GDMIGAMA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAA\n"
                                   // "MCsGA1UdIwQkMCKAIL6l3y/9yHaKCDrHxuAkeIEwUH8AGK6Xo7nx42u4Kf4jMDMG\n"
                                   // "A1UdEQQsMCqCKHVzZXItMS5vcmcxLmJjcy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9j\n"
                                   // "YWwwCgYIKoZIzj0EAwIDSAAwRQIhAMGaCGD1uKTW4B8PNHoMfTrBgJmjmSbOMV26\n"
                                   // "JDK8ZAySAiBMMQGhCdMJlVCOLphkHYiExVQJa1nMaos0xzAOX9UBUQ==\n"

                                   "-----END CERTIFICATE-----\n";

const BCHAR *hw_bcs_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
                                   "MIICqzCCAlKgAwIBAgIQQgfD2Vp1Gr+aAo4w73iXbzAKBggqhkjOPQQDAjCBhjEO\n"
                                   "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
                                   "bnpoZW4xDTALBgNVBAoTBG9yZzIxDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
                                   "Y2Eub3JnMi5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
                                   "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYYxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
                                   "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcyMQ8w\n"
                                   "DQYDVQQLEwZIdWF3ZWkxLTArBgNVBAMTJGNhLm9yZzIuYmNzLWVwbW1saS5zdmMu\n"
                                   "Y2x1c3Rlci5sb2NhbDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABI1P9XWwX1Ub\n"
                                   "c3kGY6OOfrh/jGVEM2Aci8+tZozrpxASfj3O5/YNPFjXJAbCDHs+/cqFBNTXxL0Y\n"
                                   "zQ8G2mEzW/2jgZ8wgZwwDgYDVR0PAQH/BAQDAgGmMB0GA1UdJQQWMBQGCCsGAQUF\n"
                                   "BwMBBggrBgEFBQcDAjAPBgNVHRMBAf8EBTADAQH/MCkGA1UdDgQiBCAWIw2jwEOU\n"
                                   "TqzkfhdBOMtJmHphhAD0FLxlSCPeP8UGYzAvBgNVHREEKDAmgiRjYS5vcmcyLmJj\n"
                                   "cy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9jYWwwCgYIKoZIzj0EAwIDRwAwRAIgKSMJ\n"
                                   "YDSTPdX0BsiLKu0f0IFauU1E/pgi3jMU3SuhPxgCIFUuVfU/pfTRxK6UGFYH5uxc\n"
                                   "AtrZMPym0MX872awTHPL\n"
                                   "-----END CERTIFICATE-----\n";

const BCHAR *hw_bcs_demo_endorser_peer0Org1_url[6] = {"127.0.0.0:30616",
                                                      "127.0.0.1:30616",
                                                      "127.0.0.2:30616",
                                                      "127.0.0.3:30616",
                                                      "127.0.0.4:30616",
                                                      "127.0.0.5:30616"};
const BCHAR *hw_bcs_demo_endorser_peer0Org1_hostName[6] = {"node-0.org1.bcs-epmmli.svc.cluster.local0",
                                                           "node-0.org1.bcs-epmmli.svc.cluster.local1",
                                                           "node-0.org1.bcs-epmmli.svc.cluster.local2",
                                                           "node-0.org1.bcs-epmmli.svc.cluster.local3",
                                                           "node-0.org1.bcs-epmmli.svc.cluster.local4",
                                                           "node-0.org1.bcs-epmmli.svc.cluster.local5"};
const BCHAR *hw_bcs_demo_endorser_peer0Org2_url[6] = {
    "127.0.0.0:30606",
    "127.0.0.1:30606",
    "127.0.0.2:30606",
    "127.0.0.3:30606",
    "127.0.0.4:30606",
    "127.0.0.5:30606"};

const BCHAR *hw_bcs_demo_endorser_peer0Org2_hostName[6] = {
    "node-0.org2.bcs-epmmli.svc.cluster.local0",
    "node-0.org2.bcs-epmmli.svc.cluster.local1",
    "node-0.org2.bcs-epmmli.svc.cluster.local2",
    "node-0.org2.bcs-epmmli.svc.cluster.local3",
    "node-0.org2.bcs-epmmli.svc.cluster.local4",
    "node-0.org2.bcs-epmmli.svc.cluster.local5"};
const BCHAR *hw_bcs_demo_order_url[6] = {
    "127.0.0.0:30616",
    "127.0.0.1:30616",
    "127.0.0.2:30616",
    "127.0.0.3:30616",
    "127.0.0.4:30616",
    "127.0.0.5:30616"};
const BCHAR *hw_bcs_demo_order_hostName[6] = {
    "peer1.org1.example0.com0",
    "peer1.org1.example1.com1",
    "peer1.org1.example2.com2",
    "peer1.org1.example3.com3",
    "peer1.org1.example4.com4",
    "peer1.org1.example5.com5"};

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of hwbcs
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/

/**
****************************************************************************************
* @brief:
* @param[in] *networkConfig
* @param[in] index
* @return
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg(BoatHwbcsNetworkConfig *networkConfig, BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(hw_bcs_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, hw_bcs_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHwbcsNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHwbcsNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_url[index]) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_hostName[index]) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, hw_bcs_demo_endorser_peer0Org1_url[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, hw_bcs_demo_endorser_peer0Org1_hostName[index]);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, hw_bcs_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org2_url[index]) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org2_hostName[index]) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, hw_bcs_demo_endorser_peer0Org2_url[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, hw_bcs_demo_endorser_peer0Org2_hostName[index]);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(hw_bcs_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, hw_bcs_org2_tlsCert);

    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, hw_bcs_org1_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_order_hostName[index]) + 1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_order_url[index]) + 1);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, hw_bcs_demo_order_url[index]);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, hw_bcs_demo_order_hostName[index]);

    return result;
}

/**
****************************************************************************************
* @brief:
*  This function check nodes configuration, the first nodes configuration must be same to another one;
* @param[in] nodesCfg1
*  the first nodes configuration
* @param[in] nodesCfg2
*  the second nodes configuration
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT hwbcs_checkNetworkNodesConfg(BoatHwbcsNodesCfg nodesCfg1, BoatHwbcsNodesCfg nodesCfg2)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check two nodes configuration's endorserLayoutNum*/
    if (nodesCfg1.endorserLayoutNum != nodesCfg2.endorserLayoutNum)
    {
        BoatLog(BOAT_LOG_NORMAL, " endorserLayoutNum ERR ");
        return BOAT_ERROR;
    }
    /****************** check the content ***************************/
    /* check every layout */
    for (size_t i = 0; i < nodesCfg1.endorserLayoutNum; i++)
    {
        /* check layoutcfg is NULL or not */
        if ((NULL == nodesCfg1.layoutCfg) || (NULL == nodesCfg2.layoutCfg))
        {
            BoatLog(BOAT_LOG_NORMAL, " layoutCfg NULL ");
            return BOAT_ERROR;
        }
        /* check every endorserGroupNum*/
        if (nodesCfg1.layoutCfg[i].endorserGroupNum != nodesCfg2.layoutCfg[i].endorserGroupNum)
        {
            BoatLog(BOAT_LOG_NORMAL, " endorserGroupNum err ");
            return BOAT_ERROR;
        }
        for (size_t j = 0; j < nodesCfg1.layoutCfg[i].endorserGroupNum; j++)
        {
            /* check every groupCfg is NULL or not */
            if ((NULL == nodesCfg1.layoutCfg[i].groupCfg) || (NULL == nodesCfg2.layoutCfg[i].groupCfg))
            {
                BoatLog(BOAT_LOG_NORMAL, " groupCfg NULL ");
                return BOAT_ERROR;
            }
            /* check every endorserNumber */
            if (nodesCfg1.layoutCfg[i].groupCfg[j].endorserNumber != nodesCfg2.layoutCfg[i].groupCfg[j].endorserNumber)
            {
                BoatLog(BOAT_LOG_NORMAL, " endorserNumber err ");
                return BOAT_ERROR;
            }
            /* check every endorser is NULL or not */
            if ((NULL == nodesCfg1.layoutCfg[i].groupCfg[j].endorser) || (NULL == nodesCfg2.layoutCfg[i].groupCfg[j].endorser))
            {
                BoatLog(BOAT_LOG_NORMAL, " endorser NULL ");
                return BOAT_ERROR;
            }
            /* check every quantities is same or not */
            if (nodesCfg1.layoutCfg[i].groupCfg[j].quantities != nodesCfg2.layoutCfg[i].groupCfg[j].quantities)
            {
                BoatLog(BOAT_LOG_NORMAL, " quantities err ");
                return BOAT_ERROR;
            }
            /* check every tlsOrgCertContent*/
            if (!strcmp(nodesCfg1.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, nodesCfg2.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content), 0)
            {
                BoatLog(BOAT_LOG_NORMAL, " tlsOrgCertContent content err ");
                return BOAT_ERROR;
            }
            /* check endorser content */
            for (size_t k = 0; k < nodesCfg1.layoutCfg[i].groupCfg[j].endorserNumber; k++)
            {
                /* check every hostname */
                if (strcmp(nodesCfg1.layoutCfg[i].groupCfg[j].endorser[k].hostName, nodesCfg2.layoutCfg[i].groupCfg[j].endorser[k].hostName) != 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, " hostName err ");
                    return BOAT_ERROR;
                }
                /* check every url */
                if (strcmp(nodesCfg1.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl, nodesCfg2.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl) != 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, " nodeurl err ");
                    return BOAT_ERROR;
                }
            }
        }
    }
    /****************** check every order **********************/
    /* check endorserNumber of orderCfg */
    if (nodesCfg1.orderCfg.endorserNumber != nodesCfg2.orderCfg.endorserNumber)
    {
        BoatLog(BOAT_LOG_NORMAL, " ordercfg endorserNumber err ");
        return BOAT_ERROR;
    }
    /* check endorser of orderCfg is NULL or not */
    if ((NULL == nodesCfg1.orderCfg.endorser) || (NULL == nodesCfg2.orderCfg.endorser))
    {
        BoatLog(BOAT_LOG_NORMAL, " ordercfg endorser NULL ");
        return BOAT_ERROR;
    }
    /* check tlsOrgCertContent of orderCfg */
    if (strcmp(nodesCfg1.orderCfg.tlsOrgCertContent.content, nodesCfg2.orderCfg.tlsOrgCertContent.content) != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, " ordercfg tlsOrgCertContent content err ");
        return BOAT_ERROR;
    }

    for (size_t i = 0; i < nodesCfg1.orderCfg.endorserNumber; i++)
    {
        /* check every hostname of orderCfg*/
        if (strcmp(nodesCfg1.orderCfg.endorser[i].hostName, nodesCfg2.orderCfg.endorser[i].hostName) != 0)
        {
            BoatLog(BOAT_LOG_NORMAL, " ordercfg hostname err ");
            return BOAT_ERROR;
        }
        /* check every url of orderCfg */
        if (strcmp(nodesCfg1.orderCfg.endorser[i].nodeUrl, nodesCfg2.orderCfg.endorser[i].nodeUrl) != 0)
        {
            BoatLog(BOAT_LOG_NORMAL, " ordercfg nodeurl err ");
            return BOAT_ERROR;
        }
    }

    return result;
}
/**
****************************************************************************************
* @brief:
*  This function check network data reading by index or list.
* @param[in] networkData
*  network data read from memory
* @param[in] networkCfg
*  network configuration to creat the network data
* @param[in] expectedIndex
*  index of created network
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT
hwbcs_checkNetworkData(BoatHwbcsNetworkData networkData, BoatHwbcsNetworkConfig networkCfg, BUINT8 expectedIndex)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* index of networkData must be same to expectedIndex */
    if (networkData.index != expectedIndex)
    {
        BoatLog(BOAT_LOG_NORMAL, " index err");
        return BOAT_ERROR;
    }
    /******************* check account cert ***************************/
    /* check account cert content */
    if (strcmp(networkData.accountCertContent.content, networkCfg.accountCertContent.content) != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, " accountCertContent err");
        return BOAT_ERROR;
    }
    /* check nodesCfg */
    result = hwbcs_checkNetworkNodesConfg(networkData.nodesCfg, networkCfg.nodesCfg);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function test case: creat onetime network success
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0001CreateOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg, 0);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, 0);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = hwbcs_checkNetworkData(networkData, networkCfg, 0);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHwbcsNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat two onetime network success
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0002CreateMoreOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHwbcsNetworkConfig networkCfg[2];
    BoatHwbcsNetworkData networkData;
    /************************** create the first onetime network *******************/
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg[index], index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg[index], BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /************************* create the second onetime network ******************/
    index = 1;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg[index], index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg[index], BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* get network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, 0);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = hwbcs_checkNetworkData(networkData, networkCfg[index], 0);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg[0]);
    hwbcsWalletConfigFree(networkCfg[1]);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHwbcsNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network success
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0003CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 1;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg, index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network , index of persist network is from 1 to BOAT_MAX_NETWORK_NUM ;
        this is the first persist network, index muse be 1*/
    ck_assert_int_eq(rtnVal, index);
    /* get network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, index);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = hwbcs_checkNetworkData(networkData, networkCfg, index);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat more persist network success;
*  have created a persist network in above case, go ahead and create BOAT_MAX_NETWORK_NUM-1 persistent networks
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0004CreateMorePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 1;
    BoatHwbcsNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM];
    BoatHwbcsNetworkData networkData;
    for (index = 2; index <= BOAT_MAX_NETWORK_NUM; index++)
    {
        /* get network configuration to creat hwbcs network */
        rtnVal = hwbcs_getNetworkCfg(&networkCfg[index - 1], index);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* creat persist network , store type must be FLASH */
        rtnVal = BoATHwbcsNetworkCreate(&networkCfg[index - 1], BOAT_STORE_TYPE_FLASH);
        /* check result of create network , index of persist network is from 1 to BOAT_MAX_NETWORK_NUM */
        ck_assert_int_eq(rtnVal, index);
        /* get network by index */
        rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, index);
        /* check result of getting network by index */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = hwbcs_checkNetworkData(networkData, networkCfg[index - 1], index);
        /* free configuration struct */
        hwbcsWalletConfigFree(networkCfg[index - 1]);
        /* free network data*/
        BoATHwbcs_FreeNetworkData(networkData);
        /* check result */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat more than BOAT_MAX_NETWORK_NUM persist networks fail;
*  have created BOAT_MAX_NETWORK_NUM persist networks, couldn't create another one;
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0005Create6PersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg, index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network list ;
*  have created BOAT_MAX_NETWORK_NUM persist networks,
*  there will be BOAT_MAX_NETWORK_NUM network in the list ;
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0006GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHwbcsNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM];
    BoatHwbcsNetworkContext networkList;

    /* get network list */
    rtnVal = BoATHwbcs_GetNetworkList(&networkList);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have created BOAT_MAX_NETWORK_NUM persist networks,
     num of network in the list must be BOAT_MAX_NETWORK_NUM */
    ck_assert_int_eq(networkList.networkNum, BOAT_MAX_NETWORK_NUM);
    /****************check content of list **************************/
    for (index = 0; index < BOAT_MAX_NETWORK_NUM; index++)
    {
        /* get all the configurations of persist networks*/
        rtnVal = hwbcs_getNetworkCfg(&networkCfg[index], index + 1);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = hwbcs_checkNetworkData(networkList.networks[index], networkCfg[index], index + 1);
        /* free configuration struct */
        hwbcsWalletConfigFree(networkCfg[index]);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
    /* free memory of list*/
    BoATHwbcs_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete hwbcs network which index equal 3 ;
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0007DeletePersistNetworkSuccess_index3)
{
    BSINT32 rtnVal;
    /* delete network*/
    rtnVal = BoATHwbcsNetworkDelete(3);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get hwbcs network list ;
*  created BOAT_MAX_NETWORK_NUM persist netsorks and deleted the network which index equal 3,
*  there are BOAT_MAX_NETWORK_NUM-1 networks in the list.
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0008GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHwbcsNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM];
    BoatHwbcsNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATHwbcs_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have created BOAT_MAX_NETWORK_NUM persist networks,
        and deleted one which index equal 3 , num of network in the list must be BOAT_MAX_NETWORK_NUM - 1*/
    ck_assert_int_eq(networkList.networkNum, BOAT_MAX_NETWORK_NUM - 1);
    /* check data of every network in the list */
    for (size_t i = 0; i < BOAT_MAX_NETWORK_NUM - 1; i++)
    {
        index = i + 1;
        if (index > 2)
        {
            index++;
        }
        /* get all the configurations of persist networks*/
        rtnVal = hwbcs_getNetworkCfg(&networkCfg[i], index);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = hwbcs_checkNetworkData(networkList.networks[i], networkCfg[i], index);
        /* free configuration struct */
        hwbcsWalletConfigFree(networkCfg[i]);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
    /* free memory of list*/
    BoATHwbcs_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist hwbcs network success;
*  created BOAT_MAX_NETWORK_NUM persist netsorks and deleted the network which index equal 3,
*   if create new persist network, index of the new persist network must be 3.
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0009CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 3;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg, index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, index);
    /* get network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, index);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = hwbcs_checkNetworkData(networkData, networkCfg, index);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case :  create onetime hwbcs network success
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0010CreateOnetimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg, index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_RAM);
    /* check result of create network, index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, index);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = hwbcs_checkNetworkData(networkData, networkCfg, index);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get persist networks by index;
*   have creted BOAT_MAX_NETWORK_NUM persist networks in above case and
*   get network data by index from 1 to BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0011GetNetworkByIndex)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHwbcsNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM];
    BoatHwbcsNetworkData networkData;
    /****************check network data  **************************/
    for (index = 1; index <= BOAT_MAX_NETWORK_NUM; index++)
    {
        /*get network by index */
        rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, index);
        /* check result*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* get all the configurations of persist networks*/
        rtnVal = hwbcs_getNetworkCfg(&networkCfg[index - 1], index);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = hwbcs_checkNetworkData(networkData, networkCfg[index - 1], index);
        /* free configuration struct */
        hwbcsWalletConfigFree(networkCfg[index - 1]);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* free network data*/
        BoATHwbcs_FreeNetworkData(networkData);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete all the network;
*   index from 1 to BOAT_MAX_NETWORK_NUM
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0012DeleteAllNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHwbcsNetworkData networkData;
    /****************check network data  **************************/
    for (index = 1; index <= BOAT_MAX_NETWORK_NUM; index++)
    {
        /* delete network*/
        rtnVal = BoATHwbcsNetworkDelete(index);
        /* check the result */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get hwbcs network list ;
*  there have no network ,so networkNum in the list must be 0
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0013GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATHwbcs_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list */
    ck_assert_int_eq(networkList.networkNum, 0);
    /* free memory of list*/
    BoATHwbcs_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create all the networks ;
*  first, create a onetime network,then creat BOAT_MAX_NETWORK_NUM persist networks;
* @return
****************************************************************************************
*/
START_TEST(test_002hwbcsNetwork_0014CreateAllNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 1;
    BoatHwbcsNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM + 1];
    BoatHwbcsNetworkData networkData;
    for (index = 0; index <= BOAT_MAX_NETWORK_NUM; index++)
    {
        /* get network configuration to creat hwbcs network */
        rtnVal = hwbcs_getNetworkCfg(&networkCfg[index], index);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        if (index == 0)
        {
            /* creat persist network , store type must be RAM */
            rtnVal = BoATHwbcsNetworkCreate(&networkCfg[index], BOAT_STORE_TYPE_RAM);
        }
        else
        {
            /* creat persist network , store type must be FLASH */
            rtnVal = BoATHwbcsNetworkCreate(&networkCfg[index], BOAT_STORE_TYPE_FLASH);
        }

        /* check result of create network , index of persist network is from 1 to BOAT_MAX_NETWORK_NUM */
        ck_assert_int_eq(rtnVal, index);
        /* get network by index */
        rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, index);
        /* check result of getting network by index */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = hwbcs_checkNetworkData(networkData, networkCfg[index], index);
        /* free configuration struct */
        hwbcsWalletConfigFree(networkCfg[index]);
        /* free network data*/
        BoATHwbcs_FreeNetworkData(networkData);
        /* check result */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
}
END_TEST

Suite *make_hwbcsNetworkIntfTest_suite(void)
{
    /* Create Suite */
    Suite *s_networkcreate = suite_create("networkCreat");

    /* Create test cases */
    TCase *tc_networkCreat_api = tcase_create("networkCreat_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_networkcreate, tc_networkCreat_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0001CreateOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0002CreateMoreOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0003CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0004CreateMorePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0005Create6PersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0006GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0007DeletePersistNetworkSuccess_index3);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0008GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0009CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0010CreateOnetimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0011GetNetworkByIndex);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0012DeleteAllNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0013GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0014CreateAllNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002hwbcsNetwork_0012DeleteAllNetworkSuccess);

    return s_networkcreate;
}