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

/*!@brief The wrapper header file for 3rd application to include.

@file
boatiotsdk.h is the wrapper header file for 3rd application to include.
*/

#ifndef __BOATIOTSDK_H__
#define __BOATIOTSDK_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "boatwallet.h"
#include "boaterrcode.h"
#include "boattypes.h"
#include "boatexception.h"
#include "boatutility.h"
#include "boatrlp.h"
#include "boatplatform.h"
#include "boatconfig.h"

#if PROTOCOL_USE_ETHEREUM == 1
#include "protocolapi/api_ethereum.h"
#endif

#if PROTOCOL_USE_PLATON == 1
#include "protocolapi/api_platon.h"
#endif

#if PROTOCOL_USE_HLFABRIC == 1
#include "protocolapi/api_hlfabric.h"
#if DISCOVERY_PEER_QUERY == 1
#include "protocolapi/api_hlfabric_discovery.h"
#endif
#endif

#if PROTOCOL_USE_PLATONE == 1
#include "protocolapi/api_platone.h"
#endif

#if PROTOCOL_USE_FISCOBCOS == 1
#include "protocolapi/api_fiscobcos.h"
#endif

#if PROTOCOL_USE_HWBCS == 1
#include "protocolapi/api_hw_bcs.h"
#endif

#if PROTOCOL_USE_CHAINMAKER == 1
#include "protocolapi/api_chainmaker.h"
#endif

#endif
