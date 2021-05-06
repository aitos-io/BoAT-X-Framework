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
#ifndef __BOATVERSION_H__
#define __BOATVERSION_H__
/*! @addtogroup wallet
 * @{
 */

#include <stdbool.h>
#include "boattypes.h"

/**
 * The version number x.y.z is split into three parts.
 * Major, Minor, Patchlevel
 */
#define BOAT_VERSION_MAJOR                       2
#define BOAT_VERSION_MINOR                       0
#define BOAT_VERSION_PATCH                       0

/**
 * The single version number has the following structure:
 *    MMNNPP00
 *    Major version | Minor version | Patch version
 */
#define BOAT_VERSION_STRING         "2.0.0"
#define BOAT_VERSION_NUMBER         ( (BOAT_VERSION_MAJOR << 24) | \
                                      (BOAT_VERSION_MINOR << 16) | \
                                      (BOAT_VERSION_PATCH <<  8) )

#ifdef __cplusplus
extern "C" {
#endif

/*!****************************************************************************
 * @brief   Get the version number.
 *
 * @return   
 *    The constructed version number in the format MMNNPP00 (Major, Minor, Patch).       
 ******************************************************************************/
BUINT32 BoatIotSdkVersion( void );


/*!****************************************************************************
 * @brief   Get the version string.
 *
 * @return   
 *   The constructed version string.
 ******************************************************************************/
BCHAR * BoatIotSdkVersionString( void );

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif