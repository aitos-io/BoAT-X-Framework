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

/*!@brief Default Persistent Storage

@file
persiststore.c contains APIs for default persistent storage as a file.
*/

#include "boatinternal.h"
#include "persiststore.h"
// #include "keccak.h"
// #include "sha3.h"
#include "boatlog.h"

/* mbedTLS header include */
// #include "mbedtls/aes.h"

//!@brief Salt size for keystore
#define BOAT_STORAGE_SALT_SIZE 16
// #define BOAT_NETWORK_FILE_STOREDATA	"boat_network_file"
BUINT8 RAM_BOAT_DATA[1024] = {0};

#if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_HWBCS == 1 || PROTOCOL_USE_CHAINMAKER_V1 == 1 || PROTOCOL_USE_CHAINMAKER_V2 == 1)
BUINT8 RAM_BOAT_NETWORK_DATA[5120] = {0};
#else
BUINT8 RAM_BOAT_NETWORK_DATA[1024] = {0};
#endif

BUINT8 RAM_BOAT_PRIKEY_DATA[256] = {0};

// AES KEY FOR DEVELOPMENT ONLY. DO NOT USE IT FOR PRODUCTION.
// Either replace it with a production key or replace the persitent storage
// mechanism with a secure one.
__BOATSTATIC const BUINT8 g_aes_key[32] = {0x7F, 0x78, 0xBC, 0xEC, 0xD8, 0xBA, 0x96, 0xF7,
										   0x8E, 0x66, 0xFD, 0x98, 0xEA, 0x4A, 0x4E, 0x88,
										   0x0C, 0xB7, 0x34, 0xD3, 0x11, 0x9F, 0x72, 0xE0,
										   0x81, 0xD2, 0x5E, 0xC6, 0x16, 0xAC, 0x08, 0xC4};

/**
 * @description:
 * 	This function encrypts data that want to store into Nvram with ciphertext.
 * @param[in] {BUINT8} *data_ptr
 * 	data that want to srore into Nvram.
 * @param {BUINT32} data_len
 * 	data length
 * @param {BUINT8} *out
 * 	encrypt result
 * @param {BUINT32} *outlen
 * 	length of encrypt result
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */

BOAT_RESULT BoATSoftRotNvramEnc(BUINT8 const *data_ptr, BUINT32 data_len, BUINT8 *out, BUINT32 *outlen)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT8 len = 0;
	BUINT32 encrypted_len = 0, writeDataLen = 0;
	BUINT8 rawData[data_len + 31]; // 31 for AES padding
	BUINT8 data_hash_array[32] = {0};
	BUINT8 salt_array[BOAT_STORAGE_SALT_SIZE] = {0};
	BUINT8 encrypted_array[data_len + 31]; // 31 for AES padding

	if ((data_ptr == NULL) || (data_len == 0))
	{
		BoatLog(BOAT_LOG_CRITICAL, "storage_name_str or data_ptr cannot be NULL, data_len cannot be zero.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* prepare encrypt raw data */
	memset(rawData, 0, sizeof(rawData));
	memcpy(rawData, data_ptr, data_len);
	encrypted_len = BOAT_ROUNDUP(data_len, 16);
	if (encrypted_len == data_len)
	{
		encrypted_len += 16;
	}
	memset(rawData + data_len, encrypted_len - data_len, encrypted_len - data_len);

	/* Calculate data hash */
	// keccak_256(rawData, data_len, data_hash_array);
	BoatHash(BOAT_HASH_KECCAK256, rawData, data_len, data_hash_array, &len, NULL);

	/* Generate 16-byte salt as AES initial vector */
	BoatRandom(salt_array, BOAT_STORAGE_SALT_SIZE, NULL);

	/* Encrypt the data	*/
	result = BoatAesEncrypt(salt_array, g_aes_key, rawData, encrypted_len, encrypted_array);
	if (result == BOAT_SUCCESS)
	{
		writeDataLen = 0;
		/* Write salt */
		memcpy(&out[writeDataLen], salt_array, sizeof(salt_array));
		writeDataLen += sizeof(salt_array);
		/* Write hash */
		memcpy(&out[writeDataLen], data_hash_array, sizeof(data_hash_array));
		writeDataLen += sizeof(data_hash_array);
		/* Write encrypted data */
		memcpy(&out[writeDataLen], encrypted_array, encrypted_len);
		writeDataLen += encrypted_len;
	}
	else
	{
		BoatLog(BOAT_LOG_NORMAL, "BoATSoftRotNvramEnc fail");
		return result;
	}

	*outlen = writeDataLen;
	return BOAT_SUCCESS;
}

/**
 * @description:
 * 	This function decrypt ciphertext , use for wallet prikey data.
 * @param[in] {BUINT8} *data_ptr
 * 	ciphertext of wallet prikey
 * @param[in] {BUINT32} data_len
 * 	length of ciphertext
 * @param[out] {BUINT8} *out
 * 	decrypt result
 * @param[out] {BUINT32} *outlen
 * 	length of decrypt result
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATSoftRotNvramDec(BUINT8 const *data_ptr, BUINT32 data_len, BUINT8 *out, BUINT32 *outlen)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT8 len = 0;
	BUINT32 ciphertextLen = 0;
	// Storage format: | 16 byte salt | 32 byte hash | AES(data) | 1~31 byte AES padding |
	// Where, hash = keccak256(data)
	BUINT8 salt_array[BOAT_STORAGE_SALT_SIZE];
	BUINT8 original_data_hash_array[32];
	BUINT8 data_hash_array[32];
	BUINT8 encrypted_array[data_len + 31]; // 31 for AES padding
	BUINT8 padding = 0;
	BUINT32 encrypted_readLen;
	// Plain buffer
	BUINT8 plain_array[data_len + 31];
	// BUINT32 plain_len = sizeof(plain_array);
	BUINT32 readDataIndex;
	if ((data_ptr == NULL) || (out == NULL) || (data_len == 0))
	{
		BoatLog(BOAT_LOG_CRITICAL, "storage_name_str or data_ptr cannot be NULL, data_len cannot be zero.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	readDataIndex = 0;
	ciphertextLen = data_len;
	/* Read salt */
	memcpy(salt_array, &data_ptr[readDataIndex], BOAT_STORAGE_SALT_SIZE);
	readDataIndex += BOAT_STORAGE_SALT_SIZE;
	/* Read original data hash */
	memcpy(original_data_hash_array, &data_ptr[readDataIndex], sizeof(original_data_hash_array));
	readDataIndex += sizeof(original_data_hash_array);
	/* Read rest of the file (encrypted data)  */
	encrypted_readLen = BOAT_MIN(sizeof(encrypted_array),
								 ciphertextLen - BOAT_STORAGE_SALT_SIZE - sizeof(original_data_hash_array));
	memcpy(encrypted_array, &data_ptr[readDataIndex], encrypted_readLen);
	/* decrypt data */
	result = BoatAesDecrypt(salt_array, g_aes_key, encrypted_array, encrypted_readLen, plain_array);
	// Check size of the decrypted data matches the length to read
	if (result == BOAT_SUCCESS)
	{
		/* check padding */
		padding = plain_array[encrypted_readLen - 1];
		if (padding > 16)
		{
			BoatLog(BOAT_LOG_NORMAL, "padding err , pad = %d ", padding);
			result = BOAT_ERROR;
		}
		else
		{
			for (size_t i = 0; i < padding; i++)
			{
				/* code */
				if (plain_array[encrypted_readLen - 1 - i] != padding)
				{
					result = BOAT_ERROR;
					BoatLog(BOAT_LOG_NORMAL, "padding err");
					break;
				}
			}
			if (result == BOAT_SUCCESS)
			{
				/* Calculate data hash from the decrypted data */
				// keccak_256(plain_array, encrypted_readLen - padding, data_hash_array);
				BoatHash(BOAT_HASH_KECCAK256, plain_array, encrypted_readLen - padding, data_hash_array, &len, NULL);
				/* Check if decrypted hash is the same as the original one */
				if (0 == memcmp(original_data_hash_array, data_hash_array, sizeof(data_hash_array)))
				{
					memcpy(out, plain_array, encrypted_readLen - padding);
					result = BOAT_SUCCESS;
					*outlen = encrypted_readLen - padding;
				}
				else
				{
					BoatLog(BOAT_LOG_CRITICAL, "Decrypted hash different with original.");
					result = BOAT_ERROR;
				}
			}
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Decrypted failed or the length doesn't match.");
		result = BOAT_ERROR;
	}
	return result;
}

/*!****************************************************************************
@brief Store SoftRoT context to NVRAM

@details
	This function stores any SoftRoT implementation data (e.g. activation code) to
	NVRAM. The NVRAM size varies for different hardware
	configurations, which could be obtained by calling BoatGetSoftRotNvramSize().


	Note that a writing buffer may be set up to cache the data. Flash has a limited
	endurance for erasure and writing operations. A buffer is necessary to
	aggregrate multiple writing operations into much less erasure and writing
	operations. The data in the buffer may be lost if the device is powered off
	before the data could be flushed.

@param [in] storeFile
	Select for store for keypair or network

@param [in] offset
	The offset byte in the NVRAM to write to. For example, assuming offset = 4,\n
	then the data_ptr[0] will be written to the 4th byte of the NVRAM.\n
	offset + data_len should not exceed the NVRAM size.

@param[in] data_ptr
	The data buffer to write into the NVRAM.

@param[inout] data_len
	The data length (in byte) to write into the NVRAM.
	output the data length really written into the NVRAM

@return
	This function returns 0 in a successful case.

******************************************************************************/
BOAT_RESULT BoATStoreSoftRotNvram(BoatStoreFile storeFile, BUINT32 offset, BUINT8 const *data_ptr, BUINT32 data_len, BoatStoreType storeType)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT32 offset_base = 0;
	BUINT32 rambufLen = 0;
	BUINT8 *rambuf;
	if (storeFile == BOAT_STORE_KEYPAIR)
	{
		rambuf = RAM_BOAT_DATA;
		rambufLen = sizeof(RAM_BOAT_DATA);
		offset_base = BOAT_STORAGE_KEYPAIR_OFFSET;
		if (offset + data_len > BOAT_STORAGE_KEYPAIR_MAXLEN)
		{
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
	}
	else if (storeFile == BOAT_STORE_NETWORK)
	{
		rambuf = RAM_BOAT_NETWORK_DATA;
		rambufLen = sizeof(RAM_BOAT_NETWORK_DATA);
		offset_base = BOAT_STORAGE_NETWORK_OFFSET;
	}
	else
	{
		rambuf = RAM_BOAT_PRIKEY_DATA;
		rambufLen = sizeof(RAM_BOAT_PRIKEY_DATA);
		offset_base = BOAT_STORAGE_PRIKEY_OFFSET;
		if (offset + data_len > BOAT_STORAGE_PRIKEY_MAXLEN)
		{
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
	}

	if ((data_ptr == NULL) || (data_len == 0) || (storeType == BOAT_STORE_TYPE_UNKNOWN))
	{
		BoatLog(BOAT_LOG_CRITICAL, "storage_name_str or data_ptr cannot be NULL, data_len cannot be zero.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	if (storeType == BOAT_STORE_TYPE_FLASH)
	{
		result = BoatWriteStorage(offset_base + offset, (BUINT8 *)data_ptr, data_len, NULL);
	}
	else if (storeType == BOAT_STORE_TYPE_RAM)
	{
		if (offset + data_len > rambufLen)
		{
			result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		else
		{
			memcpy(rambuf + offset, data_ptr, data_len);
			result = BOAT_SUCCESS;
		}
	}

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "write data fail .");
		return result;
	}
	return BOAT_SUCCESS;
}

/*!****************************************************************************
@brief Read from SoftRoT NVRAM

@details
	This function reads any SoftRoT implementation data (e.g. activation code) from
	the persistent NVRAM. The SoftRoT implementation must take care of how many data
	are stored in the NVRAM and read no more than that.

@param [in] storeFile
	Select for reading from file of keypair or network

@param [in] offset
	The offset byte in the NVRAM to read from. For example, assuming offset = 4,\n
	then the 4th byte of the NVRAM will be written to data_ptr[0].

@param[out] data_ptr
	The data buffer to copy the NVRAM content to.

@param[in] data_len
	The data length (in byte) to read from the NVRAM.


@return
	This function returns 0 in a successful case.

******************************************************************************/
BOAT_RESULT BoatReadSoftRotNvram(BoatStoreFile storeFile, BUINT32 offset, BUINT8 *data_ptr, BUINT32 len_to_read, BoatStoreType storeType)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT32 offset_base = 0;
	BUINT32 rambufLen = 0;
	BUINT8 *rambuf;
	if (storeFile == BOAT_STORE_KEYPAIR)
	{
		rambuf = RAM_BOAT_DATA;
		rambufLen = sizeof(RAM_BOAT_DATA);
		offset_base = BOAT_STORAGE_KEYPAIR_OFFSET;
		if (offset + len_to_read > BOAT_STORAGE_KEYPAIR_MAXLEN)
		{
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
	}
	else if (storeFile == BOAT_STORE_NETWORK)
	{
		rambuf = RAM_BOAT_NETWORK_DATA;
		rambufLen = sizeof(RAM_BOAT_NETWORK_DATA);
		offset_base = BOAT_STORAGE_NETWORK_OFFSET;
	}
	else
	{
		rambuf = RAM_BOAT_PRIKEY_DATA;
		rambufLen = sizeof(RAM_BOAT_PRIKEY_DATA);
		offset_base = BOAT_STORAGE_PRIKEY_OFFSET;
		if (offset + len_to_read > BOAT_STORAGE_PRIKEY_MAXLEN)
		{
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
	}

	if ((data_ptr == NULL) || (len_to_read == 0) || storeType == BOAT_STORE_TYPE_UNKNOWN)
	{
		BoatLog(BOAT_LOG_CRITICAL, "storage_name or data cannot be NULL, data_len cannot be zero.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	if (storeType == BOAT_STORE_TYPE_FLASH)
	{
		result = BoatReadStorage(offset_base + offset, data_ptr, len_to_read, NULL);
	}
	else
	{
		if (offset + len_to_read > rambufLen)
		{
			result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		else
		{
			memcpy(data_ptr, rambuf + offset, len_to_read);
			result = BOAT_SUCCESS;
		}
	}

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, " read file  fail ,ret = %d \r\n", result);
		return result;
	}

	return BOAT_SUCCESS;
}
