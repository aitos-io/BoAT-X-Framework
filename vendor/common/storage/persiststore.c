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
#include "keccak.h"
#include "boatlog.h"

/* mbedTLS header include */
//#include "mbedtls/aes.h"

//!@brief Salt size for keystore
#define BOAT_STORAGE_SALT_SIZE 16

// AES KEY FOR DEVELOPMENT ONLY. DO NOT USE IT FOR PRODUCTION.
// Either replace it with a production key or replace the persitent storage
// mechanism with a secure one.
__BOATSTATIC const BUINT8 g_aes_key[32] = { 0x7F ,0x78, 0xBC, 0xEC, 0xD8, 0xBA, 0x96, 0xF7,
                                            0x8E, 0x66, 0xFD, 0x98, 0xEA, 0x4A, 0x4E, 0x88,
                                            0x0C, 0xB7, 0x34, 0xD3, 0x11, 0x9F, 0x72, 0xE0,
                                            0x81, 0xD2, 0x5E, 0xC6, 0x16, 0xAC, 0x08, 0xC4 };


BOAT_RESULT BoatPersistStore( const BCHAR *storage_name_str, const void *data_ptr, 
							  BUINT32 data_len )
{
    // Storage format: | 16 byte salt | 32 byte hash | AES(data) | 1~31 byte AES paddiing |
    // Where, hash = keccak256(data)

    BUINT8  salt_array[BOAT_STORAGE_SALT_SIZE];
    BUINT8  data_hash_array[32];
    BUINT8  rawData[data_len + 31]; // 31 for AES padding
	BUINT8  encrypted_array[data_len + 31]; // 31 for AES padding
    BUINT32 encrypted_len;
	BUINT8  writeDataTmp[data_len + 31 + 32 + 16 + 16];
    BUINT32 writeDataLen;

    BOAT_RESULT result = BOAT_SUCCESS;

    if( (storage_name_str == NULL) || (data_ptr == NULL) || (data_len == 0) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "storage_name or data cannot be NULL, data_len cannot be zero.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if ( (BOAT_SUCCESS != UtilityStringLenCheck(storage_name_str)) )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
	
	/* prepare encrypt raw data */
	memset(rawData, 0, sizeof(rawData));
	memcpy(rawData, data_ptr, data_len);	
    encrypted_len = BOAT_ROUNDUP(data_len, 16);
	
    /* Calculate data hash */
    keccak_256(rawData, encrypted_len, data_hash_array);
    
    /* Generate 16-byte salt as AES initial vector */
    BoatRandom(salt_array, BOAT_STORAGE_SALT_SIZE, NULL);
	
    /* Encrypt the data	*/
	result = BoatAesEncrypt(salt_array, g_aes_key, rawData, encrypted_len, encrypted_array);
    if( result == BOAT_SUCCESS )
    {
		writeDataLen = 0;
		/* Write salt */
		memcpy( &writeDataTmp[writeDataLen], salt_array, sizeof(salt_array) );
		writeDataLen += sizeof(salt_array);
		/* Write hash */
		memcpy( &writeDataTmp[writeDataLen], data_hash_array, sizeof(data_hash_array) );
		writeDataLen += sizeof(data_hash_array);
		/* Write encrypted data */
		memcpy( &writeDataTmp[writeDataLen], encrypted_array, encrypted_len );
		writeDataLen += encrypted_len;
		result = BoatWriteFile( storage_name_str, writeDataTmp, writeDataLen, NULL );
    }
	
    return result;
}


BOAT_RESULT BoatPersistRead( const BCHAR *storage_name_str, BOAT_OUT void *data_ptr, 
							 BUINT32 len_to_read )
{
    // Storage format: | 16 byte salt | 32 byte hash | AES(data) | 1~31 byte AES paddiing |
    // Where, hash = keccak256(data)
    BUINT8  salt_array[BOAT_STORAGE_SALT_SIZE];
    BUINT8  original_data_hash_array[32];
    BUINT8  data_hash_array[32];
    BUINT8  encrypted_array[len_to_read + 31]; // 31 for AES padding
	BUINT32 encrypted_readLen;
    // Plain buffer
    BUINT8  plain_array[len_to_read];
    BUINT32 plain_len = sizeof(plain_array);
	
	BUINT8  readDataTmp[len_to_read + 31 + 32 + 16 + 16];
    BUINT32 readDataIndex;
	BUINT32 fileSize;
    
    BOAT_RESULT result = BOAT_SUCCESS;

    if( (storage_name_str == NULL) || (data_ptr == NULL) || (len_to_read == 0) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "storage_name or data cannot be NULL, data_len cannot be zero.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if ( (BOAT_SUCCESS != UtilityStringLenCheck(storage_name_str)) )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Arguments check error.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

	result += BoatGetFileSize( storage_name_str, &fileSize, NULL );
	result += BoatReadFile( storage_name_str, readDataTmp, fileSize, NULL );

	if( ( result == BOAT_SUCCESS ) && \
		( fileSize >= BOAT_STORAGE_SALT_SIZE + sizeof(data_hash_array) + len_to_read ) )
	{
		readDataIndex = 0;
		/* Read salt */
		memcpy(salt_array, &readDataTmp[readDataIndex], BOAT_STORAGE_SALT_SIZE );
		readDataIndex += BOAT_STORAGE_SALT_SIZE;
		/* Read original data hash */
		memcpy(original_data_hash_array, &readDataTmp[readDataIndex], sizeof(original_data_hash_array) );
		readDataIndex += sizeof(original_data_hash_array);
		/* Read rest of the file (encrypted data)  */
		encrypted_readLen = BOAT_MIN(sizeof(encrypted_array), 
									 fileSize - BOAT_STORAGE_SALT_SIZE - sizeof(original_data_hash_array));
		memcpy(encrypted_array, &readDataTmp[readDataIndex], encrypted_readLen);
		/* decrypt data */
		result = BoatAesDecrypt(salt_array, g_aes_key, encrypted_array, encrypted_readLen, plain_array);
		// Check size of the decrypted data matches the length to read
		if( result == BOAT_SUCCESS && plain_len == len_to_read )
		{
			/* Calculate data hash from the decrypted data */
			keccak_256(plain_array, BOAT_ROUNDUP(len_to_read, 16), data_hash_array);
			/* Check if decrypted hash is the same as the original one */
			if( 0 == memcmp(original_data_hash_array, data_hash_array, sizeof(data_hash_array)) )
			{
				memcpy(data_ptr, plain_array, len_to_read);
				result = BOAT_SUCCESS;
			}
			else
			{
		        BoatLog(BOAT_LOG_CRITICAL, "Decrypted hash different with original.");
				result = BOAT_ERROR;
			}
		}
		else
		{
			BoatLog(BOAT_LOG_CRITICAL, "Decrypted failed or the length is don't match.");
			result = BOAT_ERROR;
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file or the file size error.");
		result = BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	
    return result;
}


BOAT_RESULT BoatPersistDelete(const BCHAR * storage_name_str)
{
    // Delete file
	return BoatRemoveFile( storage_name_str, NULL );
}
