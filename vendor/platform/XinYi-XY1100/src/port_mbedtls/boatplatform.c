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
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <arpa/inet.h>
#include <string.h>

//#include <sys/time.h>

/*xinyi1100 header include */
#include "softap_api.h"



BUINT32 UtilityHex2Bin(BOAT_OUT BUINT8 *to_ptr,
                        BUINT32 to_size,
                        const BCHAR *from_str,
                        TRIMBIN_TRIM_MODE trim_mode,
                        BBOOL zero_as_null)
{
    BUINT32 from_offset;
    BUINT32 from_len;
    BUINT32 to_offset;
    BUINT32 odd_flag;

    unsigned char octet;
    unsigned char halfbyte;
    char halfbytechar;
    BBOOL bool_trim_done;
     
    if(to_ptr == NULL || to_size == 0 || from_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "<to_ptr>, <to_size> and <from_str> cannot be 0 or NULL.");
        return 0;
    }

    octet = 0;

    from_len = strlen(from_str);

    from_offset = 0;
    to_offset = 0;

    // Skip leading "0x" or "0X" if there be.
    // Note: If strlen(from_ptr) <= 2 or <from_up_to_len> <= 2, no "0x" prefix
    //       is allowed in HEX string.
    if(from_len > 2)
    {
        if(from_str[0] == '0' 
            && (from_str[1] == 'x' || from_str[1] == 'X'))
        {
            from_offset += 2;
        }
    }

            
    // if HEX length is odd, treat as if it were left filled with one more '0'
    if((from_len&0x01) != 0)
    {
        // length is odd 
        odd_flag = 1;
    }
    else
    {
        // length is even
        odd_flag = 0;
    }

    if(trim_mode == TRIMBIN_TRIM_NO)
    {
        bool_trim_done = BOAT_TRUE;
    }
    else
    {
        bool_trim_done = BOAT_FALSE;
    }
    

    while(from_offset < from_len)
    {
        halfbytechar = from_str[from_offset];
        
        if(halfbytechar >= '0' && halfbytechar <= '9')
        {
            halfbyte = halfbytechar - '0';
        }
        else if(halfbytechar >= 'A' && halfbytechar <= 'F')
        {
            halfbyte = halfbytechar - 'A' + 0x0A;
        }
        else if(halfbytechar >= 'a' && halfbytechar <= 'f')
        {
            halfbyte = halfbytechar - 'a' + 0x0A;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "<from_str> contains non-HEX character 0x%02x (%c) at Position %d of \"%s\".\n", halfbytechar, halfbytechar, from_offset, from_str);
            if(halfbytechar == ' ' || halfbytechar == '\t')
            {
                BoatLog(BOAT_LOG_NORMAL, "There should be no space between HEX codes.");
            }
            return 0;
        }

        // If from_len is even, pack 2 half bytes to a byte when from_offset is odd.
        // For example, "0x012345" is packed when from_offset points to '1', '3' and '5'.
        //
        // If from_len is odd, pack 2 half bytes to a byte when from_offset is even.
        // For example, "0x12345" is packed when from_offset points to '1', '3' and '5'.
        if( (from_offset&0x01) == odd_flag)
        {
            octet = halfbyte << 4;
            from_offset++;
            continue;
        }
        else
        {
            octet |= halfbyte;
        
            if(bool_trim_done == BOAT_FALSE && octet == 0x00)
            {
                from_offset++;
                continue;  // Trim leading zeros.
            }
            else
            {
                bool_trim_done = BOAT_TRUE;
            }
            
            to_ptr[to_offset++] = octet;

            from_offset++;

            // Check capacity of output buffer
            if(to_offset >= to_size)
            {
                break;
            }
         }
    }

    // Special process for trimed all zero HEX string
    if(to_offset == 0 && zero_as_null == BOAT_FALSE)
    {
        to_ptr[0] = 0x00;
        to_offset = 1;
    }

    return to_offset;
    
}






BOAT_RESULT  BoatHash(const BoatHashAlgType type, const BUINT8* input, BUINT32 inputLen,
				       BUINT8* hashed, BUINT8* hashedLen, void* rsvd)
{
	BOAT_RESULT result = BOAT_SUCCESS;

	/* input param check */
	if((hashed == NULL))
	{
		BoatLog( BOAT_LOG_CRITICAL, "param which 'hashed' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	if(type == BOAT_HASH_KECCAK256)
	{
		keccak_256(input, inputLen, hashed);
		if(hashedLen != NULL)
		{
			*hashedLen = 32;
		}
	}
	else if(type == BOAT_HASH_SHA256)
	{
		mbedtls_sha256(input, inputLen, hashed, 0);
		if(hashedLen != NULL)
		{
			*hashedLen = 32;
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "unknown boat hash algorithm type.");
		result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	return result;
}


void *BoatMalloc(size_t size)
{
    return(xy_malloc(size));
}


void BoatFree(void *mem_ptr)
{
    xy_free(mem_ptr);
}


void BoatSleep(BUINT32 second)
{
    xy_sleep(second);
}
