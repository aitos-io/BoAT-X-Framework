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

/*!@brief Default Random Number Generator

@file
randgenerate.c is the default random number generator.
*/

#include "boatinternal.h"

#if BOAT_USE_OPENSSL == 1

#include <stdio.h>
#include <openssl/rand.h>
#include "randgenerator.h"

#else

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "randgenerator.h"
#include "sha3.h"

#endif


#if BOAT_USE_OPENSSL == 1

// If OpenSSL is used, seed is automatically initialized by OpenSSL
void rand_seed_init(void)
{
    return;
}

BOAT_RESULT random_stream(BUINT8 *rand_buf, BUINT16 len)
{
    int rand_status;
    BOAT_RESULT result = BOAT_ERROR;
    
    rand_status = RAND_status();
    
    if( rand_status == 1 )
    {
        rand_status = RAND_bytes(rand_buf, len);
        
        if( rand_status == 1 )
        {
            result = BOAT_SUCCESS;
        }
    }
    
    
    if( result == BOAT_ERROR )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to generate random number.");
    }
    else
    {
        #ifdef DEBUG_LOG
        do
        {
            BUINT32 i;
            printf("Rand: ");
            for( i = 0; i < len; i++ )
            {
                printf("%02x", rand_buf[i]);
            }
            putchar('\n');
        }while(0);
        #endif
    }
    
    return result;
}

BUINT32 random32(void)
{
    BOAT_RESULT result;
    BUINT32 rand_bytes;
    
    result = random_stream((BUINT8 *)&rand_bytes, 4);
    
    if( result != BOAT_SUCCESS )
    {
        rand_bytes = 0;
    }
    
    return rand_bytes;
}


BUINT64 random64(void)
{
    BOAT_RESULT result;
    BUINT64 rand_bytes;
    
    result = random_stream((BUINT8 *)&rand_bytes, 8);
    
    if( result != BOAT_SUCCESS )
    {
        rand_bytes = 0;
    }
    
    return rand_bytes;
}


#else // else of #if BOAT_USE_OPENSSL == 1


BUINT8 * read_random_sdram(BUINT32 len)
{
#ifdef DEBUG_LOG
    BUINT32 i;
#endif
    size_t current_time;
    BUINT8 *random_sdram_ptr;
    BUINT8 *ret_val;
    BUINT8 time_digest[32];
    
    current_time = time(NULL);
    keccak_256((unsigned char *)&current_time, sizeof(size_t), time_digest);
    
    // Allocate extra 256 bytes so an random offset can be given by current time
    random_sdram_ptr = malloc(len+256);

    if( random_sdram_ptr != NULL )
    {
        // Return a freed buffer increase the randomness
        free(random_sdram_ptr);
        
        // Give an offset related to current time to improve randomness
        ret_val = random_sdram_ptr + time_digest[0];
    }
    else
    {
        // If malloc() fails (possibly due to out of memory), return an address
        // in the stack. The content of stack is typically random.
        ret_val = (BUINT8*)&ret_val  + time_digest[0];
    }
    
#ifdef DEBUG_LOG
        printf("read_random_sdram() returns:\n");
        for( i = 0; i < len; i++ )
        {
            printf("%02x", ret_val[i]);
        }
        putchar('\n');
#endif

        return(ret_val);

}


void rand_seed_init(void)
{
    size_t current_time;
    BUINT8 time_digest[32];
    BUINT8 sdram_digest[32];
    volatile BUINT32 seed;
    
    // Random Seed Source 1: Current Time
    // NOTE: return type of time() is 64-bit or 32-bit depending on the system
    current_time = time(NULL);
    keccak_256((BUINT8 *)&current_time, sizeof(size_t), time_digest);
    
    // Random Seed Source 2: Content of random address in SDRAM
    keccak_256(read_random_sdram(256),
               256,
               sdram_digest);
    
    // Seed = uninitialized "seed" xor first 4 bytes of time_digest xor first 4 bytes of sdram_digest
    seed ^= *(BUINT32 *)time_digest ^ *(BUINT32 *)sdram_digest;
    
    #ifdef DEBUG_LOG
        printf("rand_seed_init(): Seed is: %08x.\n", seed);
    #endif
    
	srand(seed);
	
    return;
}


BUINT32 random32(void)
{
	return ((rand() & 0xFF) | ((rand() & 0xFF) << 8) | ((rand() & 0xFF) << 16) | ((BUINT32) (rand() & 0xFF) << 24));
}


BUINT64 random64(void)
{
    BUINT64 rand64;
    
    *(BUINT32 *)&rand64 = random32();
    *(((BUINT32 *)&rand64) + 1) = random32();
    
	return (rand64);
}


BOAT_RESULT random_stream(BUINT8 *buf, BUINT16 len)
{
	BUINT32 r = 0;
	for (BUINT32 i = 0; i < len; i++)
	{
		if (i % 4 == 0)
		{
			r = random32();
		}
		buf[i] = (r >> ((i % 4) * 8)) & 0xFF;
	}

	return BOAT_SUCCESS;
}


#endif // else of #if BOAT_USE_OPENSSL == 1
