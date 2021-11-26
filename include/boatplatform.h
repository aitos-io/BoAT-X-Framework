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

/**
@brief platform interface
@file boatPlatform.h contains boat platform port interface functions.
*/

#ifndef __BOATPLATFORM_H__
#define __BOATPLATFORM_H__

/* self header include */
//#include "boatinternal.h"
#include <stdbool.h>
#include <stddef.h>
#include "boattypes.h"
#include "boaterrcode.h"

#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief
 *   hash generate function.
 *
 * @details
 *   hash generate function.
 *
 * @param type
 *	 the compute hash type.
 *   \n - #BOAT_HASH_SHA256 compute SHA256.
 *   \n - #BOAT_HASH_KECCAK256 compute KECCAK256.
 *
 * @param[in] input
 *   the origin data of hash computation.
 *
 * @param[in] inputLen
 *   the origin data length of hash computation.
 *
 * @param[out] hashed
 *   compute hash result. Caller should provide enough space to store it.
 *
 * @param[out] hashedLen
 *   computed hash length. If Don't need this param, set it NULL.
 *
 * @param rsvd
 *   reserved for futrue.
 *
 * @return
 *   Return \c BOAT_SUCCESS if generate success; otherwise return a negative error code
 ******************************************************************************/
BOAT_RESULT BoatHash(const BoatHashAlgType type, const BUINT8* input, BUINT32 inputLen, 
				     BUINT8* hashed, BUINT8* hashedLen, void* rsvd);


/*!****************************************************************************
 * @brief  Wrapper function for memory allocation
 *
 * @details
 *   This function is a wrapper for dynamic memory allocation.
 *   \n It typically wraps malloc() in a linux or Windows system.
 *   \n For RTOS it depends on the specification of the RTOS.
 * @param[in] size
 *   How many bytes to allocate.
 * @return
 *   This function returns the address of the allocated memory. If allocation\n
 *  fails, it returns NULL.
 ******************************************************************************/
void *BoatMalloc(size_t size);


/*!****************************************************************************
 * @brief  Wrapper function for memory allocation
 *
 * @details
 *   This function is a wrapper for dynamic memory allocation.
 *   \n It typically wraps calloc() in a linux or Windows system.
 *   \n For RTOS it depends on the specification of the RTOS.
 * @param[in] nmemb
 *   How many bytes of nmemb elements to allocate.
 * @param[in] size
 *   How many nmemb elements to allocate.
 * @return
 *   This function returns the address of the allocated memory. If allocation\n
 *  fails, it returns NULL.
 ******************************************************************************/
void *BoatCalloc(size_t nmemb, size_t size);


/*!****************************************************************************
 * @brief Wrapper function for memory allocation
 *
 * @details
 *   This function is a wrapper for dynamic memory de-allocation.
 *   \n It typically wraps free() in a linux or Windows system.
 *   \n For RTOS it depends on the specification of the RTOS.
 * @param[in] mem_ptr
 *   The address to free. The address must be the one returned by BoatMalloc().
 ******************************************************************************/
void BoatFree(void *mem_ptr);


/*!****************************************************************************
 * @brief Wrapper function for sleep (thread suspension)
 *
 * @details
 *   This function is a wrapper for sleep (thread suspension).
 *   \n It typically wraps sleep() or usleep() in a linux or Windows system.
 *   \n For RTOS it depends on the specification of the RTOS.
 *
 * @param[in] second
 *   How many seconds to sleep.
 ******************************************************************************/
void BoatSleep(BUINT32 second);

/*!****************************************************************************
 * @brief Wrapper function for gettimes 
 *
 * @details
 *   This function is a wrapper for gettimes
 *   \n It typically wraps time() in a linux or Windows system.
 *   \n For RTOS it depends on the specification of the RTOS.
 *
 ******************************************************************************/
BUINT64 BoatGetTimes();

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of __BOATPLATFORM_H__ */
