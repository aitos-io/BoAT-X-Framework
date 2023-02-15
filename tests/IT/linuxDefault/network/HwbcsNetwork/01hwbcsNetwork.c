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

const BCHAR *hw_bcs_demo_endorser_peer0Org1_url = "49.4.115.203:30616";
const BCHAR *hw_bcs_demo_endorser_peer0Org1_hostName = "node-0.org1.bcs-epmmli.svc.cluster.local";

const BCHAR *hw_bcs_demo_endorser_peer0Org2_url = "49.4.115.203:30606";
const BCHAR *hw_bcs_demo_endorser_peer0Org2_hostName = "node-0.org2.bcs-epmmli.svc.cluster.local";

const BCHAR *hw_bcs_demo_order_url = "49.4.115.203:30616";
const BCHAR *hw_bcs_demo_order_hostName = "node-0.org1.bcs-epmmli.svc.cluster.local";

const BCHAR *hw_bcs_demo_endorser_peer0Org1_url2 = "49.4.115.203:30616";
const BCHAR *hw_bcs_demo_endorser_peer0Org1_hostName2 = "node-0.org1.bcs-epmmli.svc.cluster.local";

const BCHAR *hw_bcs_demo_endorser_peer0Org2_url2 = "49.4.115.203:30606";
const BCHAR *hw_bcs_demo_endorser_peer0Org2_hostName2 = "node-0.org2.bcs-epmmli.svc.cluster.local";

const BCHAR *hw_bcs_demo_order_url2 = "49.4.115.203:30616";
const BCHAR *hw_bcs_demo_order_hostName2 = "node-0.org1.bcs-epmmli.svc.cluster.local";

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
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg(BoatHwbcsNetworkConfig *networkConfig)
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
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_hostName) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, hw_bcs_demo_endorser_peer0Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, hw_bcs_demo_endorser_peer0Org1_hostName);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, hw_bcs_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org2_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org2_hostName) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, hw_bcs_demo_endorser_peer0Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, hw_bcs_demo_endorser_peer0Org2_hostName);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(hw_bcs_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, hw_bcs_org2_tlsCert);

    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, hw_bcs_org1_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_order_hostName) + 1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_order_url) + 1);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, hw_bcs_demo_order_url);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, hw_bcs_demo_order_hostName);
    return result;
}

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
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg2(BoatHwbcsNetworkConfig *networkConfig)
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
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_url2) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_hostName2) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, hw_bcs_demo_endorser_peer0Org1_url2);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, hw_bcs_demo_endorser_peer0Org1_hostName2);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, hw_bcs_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org2_url2) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org2_hostName2) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, hw_bcs_demo_endorser_peer0Org2_url2);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, hw_bcs_demo_endorser_peer0Org2_hostName2);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(hw_bcs_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, hw_bcs_org2_tlsCert);

    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, hw_bcs_org1_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_order_hostName2) + 1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_order_url2) + 1);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, hw_bcs_demo_order_url2);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, hw_bcs_demo_order_hostName2);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of hwbcs;
*  set layoutCfg to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg_layoutCfg_NULL(BoatHwbcsNetworkConfig *networkConfig)
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
    networkConfig->nodesCfg.layoutCfg = NULL;
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of hwbcs;
*  set group to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg_group_NULL(BoatHwbcsNetworkConfig *networkConfig)
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
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = NULL;
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of hwbcs;
*  set endorser to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg_endorser_NULL(BoatHwbcsNetworkConfig *networkConfig)
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
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    memcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, hw_bcs_org1_tlsCert, strlen(hw_bcs_org1_tlsCert));

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = NULL;
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of hwbcs;
*  set nodeurl to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg_nodeurl_NULL(BoatHwbcsNetworkConfig *networkConfig)
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
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    memcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, hw_bcs_org1_tlsCert, strlen(hw_bcs_org1_tlsCert));

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = NULL;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_hostName) + 1);
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of hwbcs;
*  set hostname to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg_hostname_NULL(BoatHwbcsNetworkConfig *networkConfig)
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
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    memcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, hw_bcs_org1_tlsCert, strlen(hw_bcs_org1_tlsCert));

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = NULL;
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
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
START_TEST(test_001hwbcsNetwork_0001CreateOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
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
*  This function test case: creat persist network success
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0002CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network , index of persist network is from 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network ,index must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, 1);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = hwbcs_checkNetworkData(networkData, networkCfg, 1);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHwbcsNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  endorserLayoutNum in the configuration is 0 ;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0003CreatetNetworkFail_NodeCfg_endorserLayoutNum_ERR)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* change the endorserLayoutNum to 0*/
    networkCfg.nodesCfg.endorserLayoutNum = 0;
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  layoutCfg in the configuration is NULL ;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0004CreatetNetworkFail_NodeCfg_layoutCfg_NULL)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg_layoutCfg_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  endorserGroupNum in the configuration is 0, this param must not be 0 ;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0005CreatetNetworkFail_NodeCfg_endorserGroupNum_ERR)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* change endorserGroupNum to 0*/
    networkCfg.nodesCfg.layoutCfg[0].endorserGroupNum = 0;
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  group in the configuration is NULL;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0006CreatetNetworkFail_NodeCfg_groupCfg_NULL)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg_group_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  endorserNumber = 0
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0007CreatetNetworkFail_NodeCfg_endorserNumber_ERR)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
    /* change endorserNumber to 0 */
    networkCfg.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 0;
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  endorser is NULL
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0008CreatetNetworkFail_NodeCfg_endorser_NULL)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg_endorser_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  nodeUrl is NULL
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0009CreatetNetworkFail_NodeCfg_nodeUrl_NULL)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg_nodeurl_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  hostname is NULL
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0010CreatetNetworkFail_NodeCfg_hostname_NULL)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg_hostname_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  quantities is bigger than endorserNumber;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0011CreatetNetworkFail_NodeCfg_quantities_ERR)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* change quantities to 3 ,is bigger than the endorserNumber in the same group*/
    networkCfg.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 3;
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: delete onetime network;
*  create a onetime network ,then delete it ;
*  get network by index return error because there have no network;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0012DeleteOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_RAM);
    /* check result of create network, index of onetime network must be 0*/
    ck_assert_int_eq(rtnVal, 0);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
    /* delete the onetime network*/
    rtnVal = BoATHwbcsNetworkDelete(0);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get network data by index , index = 0 */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, 0);
    /* check result; There are no network , so return BOAT_ERROR_NETWORK_INEXISTENCE */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: delete persist network;
*  create a persist network ,then delete it ;
*  get network by index return error because there have no network;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0013DeletePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network, index of persist network is from 1 to BOAT_MAX_NETWORK_NUM;
        this is the first persist network, index must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
    /* delete the persist network*/
    rtnVal = BoATHwbcsNetworkDelete(1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get network data by index , index = 1 */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, 1);
    /* check result; There are no network , so return BOAT_ERROR_NETWORK_INEXISTENCE */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: delete network using index from 0 to BOAT_MAX_NETWORK_NUM;
*  get network by index return error because there have no network;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0014DeleteNetwork_NoNetwork)
{
    BSINT32 rtnVal;
    for (size_t i = 0; i <= BOAT_MAX_NETWORK_NUM; i++)
    {
        /* delete the  network*/
        rtnVal = BoATHwbcsNetworkDelete(i);
        /* check result */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: delete network using index that is bigger than BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0015DeleteNetwork_IndexExceed)
{
    BSINT32 rtnVal;

    /* delete the network that index is bigger than BOAT_MAX_NETWORK_NUM;
       index of onetime network must be 0 and index of persist network is from
       1 to BOAT_MAX_NETWORK_NUM, so the index must be from 0 to BOAT_MAX_NETWORK_NUM*/
    rtnVal = BoATHwbcsNetworkDelete(BOAT_MAX_NETWORK_NUM + 1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get keypair list that param is NULL;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0016GetNetworkList_ParamNULL)
{
    BSINT32 rtnVal;
    /* get network list that param is NULL*/
    rtnVal = BoATHwbcs_GetNetworkList(NULL);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get keypair list that have no network;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0017GetNetworkList_NONetwork)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkContext networkList;

    /* get network list that param is NULL*/
    rtnVal = BoATHwbcs_GetNetworkList(&networkList);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check content of list; there is no network, networkNum must be 0*/
    ck_assert_int_eq(networkList.networkNum, 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network list;
*  create only a onetime network and get the list, check the content of list;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0018GetNetworkList_OnlyOnetimeNetwork)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg;
    BoatHwbcsNetworkContext networkList;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg, BOAT_STORE_TYPE_RAM);
    /* check result of create network, index of onetime network must be 0*/
    ck_assert_int_eq(rtnVal, 0);
    /* get network list */
    rtnVal = BoATHwbcs_GetNetworkList(&networkList);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /************* check content of network list******************/
    /* check networkNum in the list ,only a onetime network, networkNum must be 1*/
    ck_assert_int_eq(networkList.networkNum, 1);
    /* check index in the list, only a onetime network, index must be 0*/
    ck_assert_int_eq(networkList.networks[0].index, 0);
    /* check accountCertContent in the list ,must be same to configuration*/
    ck_assert_int_eq(strcmp(networkList.networks[0].accountCertContent.content, networkCfg.accountCertContent.content), 0);
    /* check nodesCfg in the list,must be same to configuration */
    rtnVal = hwbcs_checkNetworkNodesConfg(networkList.networks[0].nodesCfg, networkCfg.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg);
    /* delete the persist network*/
    rtnVal = BoATHwbcsNetworkDelete(0);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* free memory of list*/
    BoATHwbcs_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network list;
*  create a onetime network and a persist network, then get the list, check the content of list;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0019GetNetworkList_OnetimeAndPersistNetwork)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg1;
    BoatHwbcsNetworkConfig networkCfg2;
    BoatHwbcsNetworkContext networkList;
    /*****************create onetime network *********************/
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg1);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg1, BOAT_STORE_TYPE_RAM);
    /* check result of create network, index of onetime network must be 0*/
    ck_assert_int_eq(rtnVal, 0);
    /************** create persist network ***********************/
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg2(&networkCfg2);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg2, BOAT_STORE_TYPE_FLASH);
    /* check result of create network, index of persist network is from 1 to BOAT_MAX_NETWORK_NUM,
       this is the first persist network, so index of persist must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get network list */
    rtnVal = BoATHwbcs_GetNetworkList(&networkList);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /************* check content of network list******************/
    /* there are two network, a onetime network and a persist network;
       read persist network data firstly and then read onetime network*/
    /* check networkNum in the list , networkNum must be 2*/
    ck_assert_int_eq(networkList.networkNum, 2);
    /* check index in the list, networks[0] point to the persist network ,index in this array must be 1*/
    ck_assert_int_eq(networkList.networks[0].index, 1);
    /* check accountCertContent in the list ,must be same to configuration*/
    ck_assert_int_eq(strcmp(networkList.networks[0].accountCertContent.content, networkCfg2.accountCertContent.content), 0);
    /* check nodesCfg in the list,must be same to configuration */
    rtnVal = hwbcs_checkNetworkNodesConfg(networkList.networks[0].nodesCfg, networkCfg2.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the list, networks[1] point to the onetime network ,index in this array must be 0*/
    ck_assert_int_eq(networkList.networks[1].index, 0);
    /* check accountCertContent in the list ,must be same to configuration*/
    ck_assert_int_eq(strcmp(networkList.networks[1].accountCertContent.content, networkCfg1.accountCertContent.content), 0);
    /* check nodesCfg in the list,must be same to configuration */
    rtnVal = hwbcs_checkNetworkNodesConfg(networkList.networks[1].nodesCfg, networkCfg1.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg1);
    hwbcsWalletConfigFree(networkCfg2);
    /* delete the persist network*/
    BoATHwbcsNetworkDelete(0);
    BoATHwbcsNetworkDelete(1);
    /* free memory of list*/
    BoATHwbcs_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network by index that input param is NULL;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0020GetNetworkByIndex_ParamNULL)
{
    BSINT32 rtnVal;
    rtnVal = BoATHwbcs_GetNetworkByIndex(NULL, 1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network by index that exceed to BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0021GetNetworkByIndex_IndexExceed)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkData networkData;
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, BOAT_MAX_NETWORK_NUM + 1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get onetime network data by index ;
*  create a onetime network and a persist network;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0022GetNetworkByIndex_Onetime)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg1;
    BoatHwbcsNetworkConfig networkCfg2;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg1);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg1, BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg2(&networkCfg2);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg2, BOAT_STORE_TYPE_FLASH);
    /* check result of create network , index of onetime network must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    /* get onetime network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, 0);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = hwbcs_checkNetworkData(networkData, networkCfg1, 0);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg1);
    hwbcsWalletConfigFree(networkCfg2);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHwbcsNetworkDelete(0);
    BoATHwbcsNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get persist network data by index ;
*  create a onetime network and a persist network;
* @return
****************************************************************************************
*/
START_TEST(test_001hwbcsNetwork_0023GetNetworkByIndex_Persist)
{
    BSINT32 rtnVal;
    BoatHwbcsNetworkConfig networkCfg1;
    BoatHwbcsNetworkConfig networkCfg2;
    BoatHwbcsNetworkData networkData;
    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg(&networkCfg1);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg1, BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* get network configuration to creat hwbcs network */
    rtnVal = hwbcs_getNetworkCfg2(&networkCfg2);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&networkCfg2, BOAT_STORE_TYPE_FLASH);
    /* check result of create network , index of onetime network must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    /* get onetime network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&networkData, 1);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = hwbcs_checkNetworkData(networkData, networkCfg2, 1);
    /* free configuration struct */
    hwbcsWalletConfigFree(networkCfg1);
    hwbcsWalletConfigFree(networkCfg2);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHwbcsNetworkDelete(0);
    BoATHwbcsNetworkDelete(1);
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
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0001CreateOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0002CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0003CreatetNetworkFail_NodeCfg_endorserLayoutNum_ERR);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0004CreatetNetworkFail_NodeCfg_layoutCfg_NULL);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0005CreatetNetworkFail_NodeCfg_endorserGroupNum_ERR);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0006CreatetNetworkFail_NodeCfg_groupCfg_NULL);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0007CreatetNetworkFail_NodeCfg_endorserNumber_ERR);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0008CreatetNetworkFail_NodeCfg_endorser_NULL);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0009CreatetNetworkFail_NodeCfg_nodeUrl_NULL);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0010CreatetNetworkFail_NodeCfg_hostname_NULL);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0011CreatetNetworkFail_NodeCfg_quantities_ERR);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0012DeleteOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0013DeletePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0014DeleteNetwork_NoNetwork);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0015DeleteNetwork_IndexExceed);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0016GetNetworkList_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0017GetNetworkList_NONetwork);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0018GetNetworkList_OnlyOnetimeNetwork);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0019GetNetworkList_OnetimeAndPersistNetwork);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0020GetNetworkByIndex_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0021GetNetworkByIndex_IndexExceed);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0022GetNetworkByIndex_Onetime);
    tcase_add_test(tc_networkCreat_api, test_001hwbcsNetwork_0023GetNetworkByIndex_Persist);

    return s_networkcreate;
}