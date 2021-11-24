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
boatchainamaker.h is header file for RAW transaction construction and performing.
*/

#ifndef __BOATCHAINMAKER_H__
#define __BOATCHAINMAKER_H__

#include "boatconfig.h"
#include "boatinternal.h"
/*!****************************************************************************
 * @brief 
 *   proposal or transaction top level packed
 * 
 * @param tx_ptr 
 *   chainmaker transaction structure pointer
 * 
 * @param method 
 *   chainmaker invoke or save
 * 
 * @param contract_name 
 *   chainmaker contract_name
 * 
 * @param tx_type 
 *   chainmaker user system invoke and query
 * 
 * @param tx_id 
 *   chainmaker 64 bit random
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code.
 ******************************************************************************/
BOAT_RESULT hlchainmakerTransactionPacked(BoatHlchainmakerTx *tx_ptr, BCHAR* method, BCHAR* contract_name, TxType tx_type, char* tx_id);

#ifdef __cplusplus
extern "C" {
#endif

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
