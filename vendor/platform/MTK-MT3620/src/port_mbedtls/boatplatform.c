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

#include "keccak.h"

/* mbedTLS header include */
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/asn1.h"

#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/bignum.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/error.h"

/* net releated include */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

//#include <sys/time.h>


BOAT_RESULT  BoatHash( const BoatHashAlgType type, const BUINT8* input, BUINT32 inputLen,
				       BUINT8* hashed, BUINT8* hashedLen, void* rsvd )
{
	BOAT_RESULT result = BOAT_SUCCESS;

	/* input param check */
	if(  hashed == NULL  )
	{
		BoatLog( BOAT_LOG_CRITICAL, "param which 'hashed' can't be NULL." );
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	if( type == BOAT_HASH_KECCAK256 )
	{
		keccak_256( input, inputLen, hashed );
		if( hashedLen != NULL )
		{
			*hashedLen = 32;
		}
	}
	else if( type == BOAT_HASH_SHA256 )
	{
		mbedtls_sha256( input, inputLen, hashed, 0 );
		if( hashedLen != NULL )
		{
			*hashedLen = 32;
		}
	}
	else
	{
		BoatLog( BOAT_LOG_CRITICAL, "unknown boat hash algorithm type." );
		result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	return result;
}


void *BoatMalloc(size_t size)
{
    return(malloc(size));
}


void *BoatCalloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}


void BoatFree(void *mem_ptr)
{
    free(mem_ptr);
}


void BoatSleep(BUINT32 second)
{
    sleep(second);
}
