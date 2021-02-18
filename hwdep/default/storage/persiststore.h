/******************************************************************************
 * Copyright (C) 2018-2020 aitos.io
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

/*!@brief Default Persistent Storage Header File

@file
persiststore.h contains APIs declaration for default persistent storage as a file.
*/

#ifndef __PERSISTSTORE_H__
#define __PERSISTSTORE_H__

#include "boatinternal.h"

#ifdef __cplusplus
extern "C" {
#endif

BOAT_RESULT BoatPersistStore(const BCHAR *storage_name_str, const void *data_ptr, BUINT32 data_len);
BOAT_RESULT BoatPersistRead(const BCHAR *storage_name_str, BOAT_OUT void *data_ptr, BUINT32 len_to_read);
BOAT_RESULT BoatPersistDelete(const BCHAR * storage_name_str);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
