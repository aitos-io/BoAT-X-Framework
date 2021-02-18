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

/*!@brief Header file for performing RAW transaction

@file
boatplatone.h is header file for RAW transaction construction and performing.
*/


#ifndef __BOATPLATONE_H__
#define __BOATPLATONE_H__


#include "boatinternal.h"

#if PROTOCOL_USE_PLATONE == 1


#ifdef __cplusplus
extern "C" {
#endif



BOAT_RESULT PlatoneSendRawtx(BOAT_INOUT BoatPlatoneTx *tx_ptr);
BOAT_RESULT PlatoneSendRawtxWithReceipt(BOAT_INOUT BoatPlatoneTx *tx_ptr);



#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of PROTOCOL_USE_PLATONE */

#endif

