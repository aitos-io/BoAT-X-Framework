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

/*!@brief platform interface

@file platform.c contains platform interface functions.
*/

//! self header include
#include <stdlib.h>
#include "boatplatform.h"
#include "boattypes.h"
#include "boatutility.h"
#include "boatlog.h"

#include "sha3.h"

/* net releated include */
#include <sys/types.h>
#include <string.h>

#include "txm_module.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "qapi_fibocom.h"


#define TEST_BYTE_POOL_SIZE 30720*8 

uint32 free_memory_test[TEST_BYTE_POOL_SIZE/4];

TX_BYTE_POOL *byte_pool_test;


BOAT_RESULT  BoatHash(const BoatHashAlgType type, const BUINT8 *input, BUINT32 inputLen, 
				      BUINT8 *hashed, BUINT8 *hashedLen, void *rsvd )
{
	BOAT_RESULT result = BOAT_SUCCESS;
	
	/* input param check */
	if (( hashed == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'hashed' can't be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	
	if (type == BOAT_HASH_KECCAK256)
	{
		keccak_256(input, inputLen, hashed);
		if (hashedLen != NULL)
		{
			*hashedLen = 32;
		}
	}
	else if (type == BOAT_HASH_SHA256)
	{
		sha3_256(input, inputLen, hashed);
		if (hashedLen != NULL)
		{
			*hashedLen = 32;
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "unknown boat hash algorithm type.");
		result = BOAT_ERROR_INVALID_ARGUMENT;
	}

	return result;
}


qapi_Status_t dam_byte_pool_init(void)
{
  int ret;

  do
  {
    /* Allocate byte_pool_dam (memory heap) */
    ret = txm_module_object_allocate(&byte_pool_test, sizeof(TX_BYTE_POOL));
    if(ret != TX_SUCCESS)
    {
      LOG_ERROR("DAM_APP:Allocate byte_pool_dam fail \n");
      break;
    }

    /* Create byte_pool_dam */
    ret = tx_byte_pool_create(byte_pool_test, "Test application pool", free_memory_test, TEST_BYTE_POOL_SIZE);
    if(ret != TX_SUCCESS)
    {
      LOG_ERROR("DAM_APP:Create byte_pool_dam fail \n");
      break;
    }
    
  }while(0);

  return ret;
}


void *data_malloc(uint32_t size)
{
  void *data = NULL;
  uint32_t status = 0;

  if (0 == size)
  {
    return NULL;
  }

  status = tx_byte_allocate(byte_pool_test, (VOID **)&data, size, TX_NO_WAIT);

  if (TX_SUCCESS != status)
  {
   	BoatLog(BOAT_LOG_CRITICAL,"DAM_APP:Failed to allocate memory with %d", status); 
    return NULL;
  }

  if(NULL != data)
  {
    memset(data, 0, size);
  }

  return data;
}


void data_free(void *data)
{
  uint32_t status = 0;
  
  if(NULL == data)
  {
    return;
  }

  status = tx_byte_release(data);
  
  if (TX_SUCCESS != status)
  {
    BoatLog(BOAT_LOG_CRITICAL,"DAM_APP:Failed to release memory with %d", status); 
  }

  data = NULL;
}

void *BoatMalloc(size_t size)
{
	return data_malloc(size);
}


void BoatFree(void *mem_ptr)
{
    data_free(mem_ptr);
}


void BoatSleep(BUINT32 second)
{
    qapi_Timer_Sleep(second,QAPI_TIMER_UNIT_SEC,true);
}
