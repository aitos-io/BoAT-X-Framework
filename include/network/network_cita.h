/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-06 14:49:41
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-22 13:41:47
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

/*!@brief Boatwallet SDK header file

@file
boatwallet.h is the SDK header file.
*/

#ifndef __NETWORK_CITA_H__
#define __NETWORK_CITA_H__

/*! @defgroup wallet boat wallet API
 * @{
 */

#include "boattypes.h"

#define BOAT_CITA_NODE_URL_MAX_LEN 127 //!< Maxmum length for node's URL
#define BOAT_CITA_CHAIN_ID_V1_SIZE 32

//!@brief cita network configuration

//! CIta network configuration is used in network creation.
typedef struct TBoatCitaNetworkConfig
{
    BUINT32 version;
    BUINT8 chain_id_buf[BOAT_CITA_CHAIN_ID_V1_SIZE];
    BCHAR node_url_buf[BOAT_CITA_NODE_URL_MAX_LEN]; //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
} BoatCitaNetworkConfig;

/*!
 * @brief Blockchain network information
 * chain ID of the network being part of the transaction before it's signed.
 * <node_url_ptr> must include the protocol descriptor, IP address or URL name and
 * port. For example, http://a.b.com:8545
 */
typedef struct TBoatCitaNetworData
{
    BUINT8 networkIndex; // index of network
    BUINT32 version;
    BUINT8 chain_id_buf[BOAT_CITA_CHAIN_ID_V1_SIZE];
    BCHAR node_url_buf[BOAT_CITA_NODE_URL_MAX_LEN]; //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
} BoatCitaNetworkData;

//!@brief BoAT IoT SDK Context
typedef struct TBoatCitaNetworkContext
{
    BUINT8 networkNum;
    // Protocol specific properties are defined in protocol specific WalletInfo structure
    BoatCitaNetworkData networks[BOAT_MAX_NETWORK_NUM]; //!< Wallet Info List
} BoatCitaNetworkContext;

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
BOAT_RESULT BoATCita_GetNetworkList(BoatCitaNetworkContext *networkList);

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
BOAT_RESULT BoATCita_FreeNetworkContext(BoatCitaNetworkContext networkList);

/**
 * @description:
 * This function creat cita network
 * @param[in] {BoatCitaNetworkConfig} networkConfig
 * @param[in] {BoatStoreType} storeType
 *  For onetime network or test , select store in ram .
 *  For persistent network ,slect store in flash.
 * @return
 *   This function returns network index if creat network successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATCitaNetworkCreate(BoatCitaNetworkConfig *networkConfig, BoatStoreType storeType);

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
BOAT_RESULT BoATCitaNetworkDelete(BUINT8 index);

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
BOAT_RESULT BoATCita_GetNetworkByIndex(BoatCitaNetworkData *networkData, BUINT8 index);

#endif
