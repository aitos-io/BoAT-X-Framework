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

#include "boatiotsdk.h"

#define BoatDisplayTestResult(is_pass, case_name_str) \
{\
    if( is_pass ) \
    { \
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.\n", (case_name_str)); \
    } \
    else \
    { \
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.\n", (case_name_str)); \
        return BOAT_ERROR; \
    } \
}



#define BoatDisplayCellTestResult(result_num,cell_is_pass,case_cell_name_str) \
{\
    if( cell_is_pass ) \
    { \
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.\n", (case_cell_name_str)); \
    } \
    else \
    { \
        result_num--;\
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.\n", (case_cell_name_str)); \
    } \
}


typedef struct TBoatIotSdkTest{
    BCHAR gasprice_str[32];        //gasprice
    BCHAR gaslimit_str[32];        //gaslimit
    BCHAR contract_addr_str[64];   //Contract Address
    BCHAR recipient_addr_str[64];  //Recipient Address
    BCHAR priv_key_str[64];        //Private Key
    BCHAR node_url_str[127];        //node url
    BCHAR wallet_name_str[127];     //Wallet name
    BoatProtocolType protocol_use;  //platone
} BoatIotSdkTest;
