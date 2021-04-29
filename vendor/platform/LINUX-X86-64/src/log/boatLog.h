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

/*!@brief Boatwallet options

@file
boatLogConfig.h defines options for compiling.
*/

#ifndef __BOATCONFIG_H__
#define __BOATCONFIG_H__

//! BOAT LOG LEVEL DEFINITION
//! Log level is used to control the detail of log output.
//! 3 types of detail level can be specified in BoatLog():
//! "CRITICAL" is for critical exceptions that may halt the wallet from going on.
//! "NORMAL" is for warnings that may imply some error but could be held.
//! "VERBOSE" is for detail information that is only useful for debug.
#define BOAT_LOG_NONE                            0
#define BOAT_LOG_CRITICAL                        1
#define BOAT_LOG_NORMAL                          2
#define BOAT_LOG_VERBOSE                         3

//! BOAT_LOG_LEVEL is a macro that limits the log detail up to that level.
//! Seting it to BOAT_LOG_NONE means outputing nothing.
#define BOAT_LOG_LEVEL   BOAT_LOG_VERBOSE



#define BoatPrintf                               printf

#endif