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

@file boatplatform.c contains platform interface functions.
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

#define GENERATE_KEY_REPEAT_TIMES 100

#define BoAT_BYTE_POOL_SIZE 30720*8 

uint32 free_memory_boat[BoAT_BYTE_POOL_SIZE/4];

TX_BYTE_POOL *byte_pool_boat;


BOAT_RESULT  BoatHash(const BoatHashAlgType type, const BUINT8 *input, BUINT32 inputLen, 
				      BUINT8 *hashed, BUINT8 *hashedLen, void *rsvd )
{
	BOAT_RESULT result = BOAT_SUCCESS;
	
	/* input param check */
	if (( hashed == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'hashed' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
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
		result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	return result;
}


qapi_Status_t dam_byte_pool_init(void)
{
  int ret;

  do
  {
    /* Allocate byte_pool_dam (memory heap) */
    ret = txm_module_object_allocate(&byte_pool_boat, sizeof(TX_BYTE_POOL));
    if(ret != TX_SUCCESS)
    {
      BoatLog(BOAT_LOG_CRITICAL,"DAM_APP:Allocate byte_pool_dam fail,ret=%d",ret);
      break;
    }

    /* Create byte_pool_dam */
    ret = tx_byte_pool_create(byte_pool_boat, "boat application pool", free_memory_boat, BoAT_BYTE_POOL_SIZE);
    if(ret != TX_SUCCESS)
    {
      BoatLog(BOAT_LOG_CRITICAL,"DAM_APP:Create byte_pool_dam fail,ret=%d",ret);
      break;
    }
    
  }while(0);

  return ret;
}


BOAT_RESULT BoAT_getPubkey(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *prikey, BUINT32 prikeyLen, BUINT8 *pubkey, BUINT32 *pubkeyLen)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT8 pubKey65[65] = {0};
	if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
	{
		ecdsa_get_public_key65(&secp256k1, prikey, pubKey65);
		memcpy(pubkey, &pubKey65[1], 64);
		*pubkeyLen = 64;
	}
	else if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
	{
		ecdsa_get_public_key65(&nist256p1, prikey, pubKey65);
		memcpy(pubkey, &pubKey65[1], 64);
		*pubkeyLen = 64;
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Invalid private key type.");
		result = BOAT_ERROR_KEYPAIR_KEY_TYPE_ERR;
	}
	return result;
}

BOAT_RESULT BoAT_Keypair_generation(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BoatKeypairKeypair *keypair)
{
	/* Valid private key value (as a UINT256) for Ethereum is [1, n-1], where n is
		   0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141 */
	const BUINT32 priv_key_max_u256[8] = {0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6,
										  0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	bignum256 priv_key_bn256;
	bignum256 priv_key_max_bn256;
	BUINT8 prikeyTmp[32];
	BUINT32 key_try_count;
	BOAT_RESULT result = BOAT_SUCCESS;

	/* Convert priv_key_max_u256 from UINT256 to Bignum256 format */
	bn_read_le((const uint8_t *)priv_key_max_u256, &priv_key_max_bn256);

	// 1- update private key
	/* generate native private key loop */
	for (key_try_count = 0; key_try_count < GENERATE_KEY_REPEAT_TIMES; key_try_count++)
	{
		/* generate native private key */
		result = BoatRandom(prikeyTmp, 32, NULL);
		if (result != BOAT_SUCCESS)
		{
			BoatLog(BOAT_LOG_CRITICAL, "Fail to generate private key.");
			break;
		}

		/* Convert private key from UINT256 to Bignum256 format */
		bn_read_le(prikeyTmp, &priv_key_bn256);

		/* check the generated private key is valid or not */
		if ((bn_is_zero(&priv_key_bn256) == 0) &&
			(bn_is_less(&priv_key_bn256, &priv_key_max_bn256) != 0))
		{
			/* key is valid */
			memcpy(keypair->prikey.value, prikeyTmp, 32);
			keypair->prikey.value_len = 32;
			result = BOAT_SUCCESS;
			break;
		}
		else if (key_try_count == GENERATE_KEY_REPEAT_TIMES - 1)
		{
			result = BOAT_ERROR;
		}
	}

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "generate private key failed.");
		return result;
	}
	result = BoAT_getPubkey(type, format, keypair->prikey.value, keypair->prikey.value_len, keypair->pubkey.value, &keypair->pubkey.value_len);

	return result;
}


BOAT_RESULT BoAT_sign(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *prikey, BUINT32 prikeylen, const BUINT8 *digest, BUINT32 digestLen, BUINT8 *signature, BUINT32 *signatureLen, BUINT8 *Prefix)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	if (prikey == NULL || signature == NULL || Prefix == NULL)
	{
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
	{
		result = ecdsa_sign_digest(&secp256k1, // const ecdsa_curve *curve
								   prikey,	   // const uint8_t *priv_key
								   digest,	   // const uint8_t *digest
								   signature,  // uint8_t *sig,
								   Prefix,	   // uint8_t *pby,
								   NULL		   // int (*is_canonical)(uint8_t by, uint8_t sig[64]))
		);
	}
	else if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
	{
		result = ecdsa_sign_digest(&nist256p1, // const ecdsa_curve *curve
								   prikey,	   // const uint8_t *priv_key
								   digest,	   // const uint8_t *digest
								   signature,  // uint8_t *sig,
								   Prefix,	   // uint8_t *pby,
								   NULL		   // int (*is_canonical)(uint8_t by, uint8_t sig[64]))
		);
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Unkown private key type.");
		return BOAT_ERROR_KEYPAIR_KEY_TYPE_ERR;
	}
	if (result == BOAT_SUCCESS)
	{
		*signatureLen = 64;
	}
	return result;
}


void *data_malloc(uint32_t size)
{
  void *data = NULL;
  uint32_t status = 0;

  if (0 == size)
  {
    return NULL;
  }

  status = tx_byte_allocate(byte_pool_boat, (VOID **)&data, size, TX_NO_WAIT);

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
