/*
 * @Description: 
 * @Author: aitos
 * @Date: 2022-09-13 16:39:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-13 18:03:45
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
#ifndef __BOAT_KEYSTORE_SOFT_H__
#define __BOAT_KEYSTORE_SOFT_H__

#include "boattypes.h"

#define BOAT_KEYSTORE_SOFT_OFFSET  


//!@brief Boat keypair  key context
typedef struct TBoatKeystoreSoft
{
    BUINT8                  keypair_index;       //!< Index of keypair
    BoatFieldVariable       prikey_content; //!< Content of prikey key, native format .
    
}BoatKeystoreSoft;


#endif