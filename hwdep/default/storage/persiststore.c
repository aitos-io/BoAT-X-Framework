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

/*!@brief Default Persistent Storage

@file
persiststore.c contains APIs for default persistent storage as a file.
*/

#include "boatinternal.h"
#include "randgenerator.h"
#include "sha3.h"


#if BOAT_USE_OPENSSL != 0
#include <openssl/evp.h>
#include <openssl/aes.h>
#endif

//!@brief Salt size for keystore
#define BOAT_STORAGE_SALT_SIZE 16

// AES KEY FOR DEVELOPMENT ONLY. DO NOT USE IT FOR PRODUCTION.
// Either replace it with a production key or replace the persitent storage
// mechanism with a secure one.
__BOATSTATIC const BUINT8 g_aes_key[32] = { 0x7F ,0x78, 0xBC, 0xEC, 0xD8, 0xBA, 0x96, 0xF7,
                                            0x8E, 0x66, 0xFD, 0x98, 0xEA, 0x4A, 0x4E, 0x88,
                                            0x0C, 0xB7, 0x34, 0xD3, 0x11, 0x9F, 0x72, 0xE0,
                                            0x81, 0xD2, 0x5E, 0xC6, 0x16, 0xAC, 0x08, 0xC4};



/******************************************************************************
@brief AES encryption wrapper

Function: KeystoreEncrypt()

    This function encrypts the data with AES-256 CBC.

    In case OpenSSL is not availble, it simply encodes the data in a compatible
    format without actual encryption.

    It's for internally use only.


@see KeystoreDecrypt()

@return
    This function returns BOAT_SUCCESS if it successfully encrypts the data.\n
    Otherwise it returns one of the error codes.

@param[out] encrypted_ptr
    Output buffer of the encrypted data.

@param[inout] encrypted_len_ptr
    In:  The size of <encrypted_ptr>.\n
    Out: The size of the padded encrypted data. It varies between <plain_len> + (1~31).

@param[in] iv_ptr
    16-byte initial vector.

@param[in] data_ptr
    Plain data to encrypt.
    
@param[in] data_len
    Length (in byte) of <data_ptr>.\n
    If <data_len> is 0, nothing is encrypted.

*******************************************************************************/
__BOATSTATIC BOAT_RESULT KeystoreEncrypt(BOAT_OUT BUINT8 *encrypted_ptr, BOAT_INOUT BUINT32 *encrypted_len_ptr, const BUINT8 *iv_ptr, const BUINT8 *plain_ptr, BUINT32 plain_len)
{
#if BOAT_USE_OPENSSL != 0
    BUINT32 encrypted_total_len = 0;

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX *ctx_ptr = &ctx;
#else
    EVP_CIPHER_CTX *ctx_ptr = NULL;
#endif

    int openssl_ret;
    int openssl_len = 0;
#endif

    BOAT_RESULT result = BOAT_ERROR;


    if( ( encrypted_ptr == NULL || encrypted_len_ptr == NULL || iv_ptr == NULL || plain_ptr == NULL ) && plain_len != 0 )
    {
        return BOAT_ERROR_INVALID_ARGUMENT;
    }


    if( plain_len == 0 )
    {
        *encrypted_len_ptr = 0;
        return BOAT_SUCCESS;
    }


#if BOAT_USE_OPENSSL != 0
    if( *encrypted_len_ptr < plain_len + AES_BLOCK_SIZE * 2 - 1 )
    {
        // Output buffer size must reserve at least AES_BLOCK_SIZE * 2 - 1 bytes for PKCS padding
        return BOAT_ERROR;
    }


    // Initialize OpenSSL EVP context for cipher/decipher
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_CIPHER_CTX_init(ctx_ptr);
#else
    ctx_ptr = EVP_CIPHER_CTX_new();
#endif

    if( ctx_ptr != NULL )
    {
        // Specify AES256-CBC Algorithm, AES key and initial vector of CBC
        openssl_ret = EVP_EncryptInit_ex(ctx_ptr, EVP_aes_256_cbc(), NULL, g_aes_key, iv_ptr);
        if( openssl_ret == 1 )
        {
            encrypted_total_len = 0;

            // Encryption update
            openssl_ret = EVP_EncryptUpdate(ctx_ptr, encrypted_ptr, &openssl_len, plain_ptr, plain_len);
            if( openssl_ret == 1 )
            {
                encrypted_total_len += openssl_len;
                
                // Finalize encryption
                openssl_ret = EVP_EncryptFinal_ex(ctx_ptr, encrypted_ptr + encrypted_total_len, &openssl_len);
                if( openssl_ret == 1 )
                {
                    encrypted_total_len += openssl_len;
                    *encrypted_len_ptr = encrypted_total_len;
                    result = BOAT_SUCCESS;
                }
            }
        }

    }


#if OPENSSL_VERSION_NUMBER < 0x10100000L
        EVP_CIPHER_CTX_cleanup(&ctx);
#else
        if( ctx_ptr != NULL )
        {
            EVP_CIPHER_CTX_free(ctx_ptr);
        }
#endif

#else   // BOAT_USE_OPENSSL != 0

    BSINT32 i;
    
    if( *encrypted_len_ptr < plain_len )
    {
        return BOAT_ERROR;
    }
    
    for( i = 0; i < plain_len; i++ )
    {
        encrypted_ptr[i] = plain_ptr[i] ^ g_aes_key[i%sizeof(g_aes_key)];
        *encrypted_len_ptr = plain_len;
    }

    result = BOAT_SUCCESS;

#endif  // BOAT_USE_OPENSSL != 0

    return result;
}



/******************************************************************************
@brief AES decryption wrapper

Function: KeystoreDecrypt()

    This function decrypts the data with AES-256 CBC.

    In case OpenSSL is not availble, it simply decodes the data in a compatible
    format without actual decryption.

    It's for internally use only.


@see KeystoreEncrypt()

@return
    This function returns BOAT_SUCCESS if it successfully decrypts the data.\n
    Otherwise it returns one of the error codes.

@param[out] plain_ptr
    Output buffer of the decrypted plain data.

@param[inout] plain_len_ptr
    In:  The size of <plain_ptr>.\n
    Out: The actual size of plain data after removing the padding.

@param[in] iv_ptr
    16-byte initial vector.

@param[in] encrypted_ptr
    Encrypted data to decrypt.
    
@param[in] encrypted_len
    Length (in byte) of <encrypted_ptr>.\n
    If <encrypted_len> is 0, nothing is decrypted.

*******************************************************************************/
__BOATSTATIC BOAT_RESULT KeystoreDecrypt(BOAT_OUT BUINT8 *plain_ptr, BOAT_INOUT BUINT32 *plain_len_ptr, const BUINT8 *iv_ptr, const BUINT8 *encrypted_ptr, BUINT32 encrypted_len)
{
#if BOAT_USE_OPENSSL != 0
    BUINT32 plain_total_len = 0;
    BUINT8 temp_plain_array[encrypted_len];

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX *ctx_ptr = &ctx;
#else
    EVP_CIPHER_CTX *ctx_ptr = NULL;
#endif

    int openssl_ret;
    int openssl_len;
#endif

    BOAT_RESULT result = BOAT_ERROR;

    if( ( plain_ptr == NULL || plain_len_ptr == NULL || iv_ptr == NULL || encrypted_ptr == NULL ) && encrypted_len != 0 )
    {
        return BOAT_ERROR_INVALID_ARGUMENT;
    }


    if( encrypted_len == 0 )
    {
        *plain_len_ptr = 0;
        return BOAT_SUCCESS;
    }


#if BOAT_USE_OPENSSL != 0

    // Initialize OpenSSL EVP context for cipher/decipher
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_CIPHER_CTX_init(ctx_ptr);
#else
    ctx_ptr = EVP_CIPHER_CTX_new();
#endif


    if( ctx_ptr != NULL )
    {
        // Specify AES256-CBC Algorithm, AES key and initial vector of CBC
        openssl_ret = EVP_DecryptInit_ex(ctx_ptr, EVP_aes_256_cbc(), NULL, g_aes_key, iv_ptr);
        if( openssl_ret == 1 )
        {
            plain_total_len = 0;

            // Decryption update
            openssl_ret = EVP_DecryptUpdate(ctx_ptr, temp_plain_array, &openssl_len, encrypted_ptr, encrypted_len);
            if( openssl_ret == 1 )
            {
                plain_total_len += openssl_len;
                
                // Finalize encryption
                openssl_ret = EVP_DecryptFinal_ex(ctx_ptr, temp_plain_array + plain_total_len, &openssl_len);
                if( openssl_ret == 1 )
                {
                    plain_total_len += openssl_len;

                    // Check output buffer size
                    if( *plain_len_ptr >= plain_total_len )
                    {
                        *plain_len_ptr = plain_total_len;
                        memcpy(plain_ptr, temp_plain_array, plain_total_len);
                        
                        result = BOAT_SUCCESS;
                    }
                }
            }
        }

    }



#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_CIPHER_CTX_cleanup(&ctx);
#else
    if( ctx_ptr != NULL )
    {
        EVP_CIPHER_CTX_free(ctx_ptr);
    }
#endif

#else   // BOAT_USE_OPENSSL != 0

    BSINT32 i;
    
    if( *plain_len_ptr < encrypted_len )
    {
        return BOAT_ERROR;
    }
    
    for( i = 0; i < encrypted_len; i++ )
    {
        plain_ptr[i] = encrypted_ptr[i] ^ g_aes_key[i%sizeof(g_aes_key)];
        *plain_len_ptr = encrypted_len;
    }

    result = BOAT_SUCCESS;

#endif  // BOAT_USE_OPENSSL != 0

    return result;
}


/*!*****************************************************************************
@brief Persitently store data in an encrypted file

Function: BoatPersistStore()

    This function persistently stores data in an encrypted file with hash check.

    NOTE:
    This is a default implementation for persistent storage with AES-256
    encryption. It assumes filesystem is supported in the system.

    In case filesystem is not supported, or some type of secure enclave is equipped,
    re-implement this function according to the system configuration.


    This function encrypts the data and saves them into the specified file in
    following format.
    
    @verbatim
    File format:\n
     --------------------------------------------------------------------------
     | 16 byte salt | 32 byte hash | AES(plain_data) | 0~31 byte AES paddiing |
     --------------------------------------------------------------------------
      ^              ^              ^                 ^                        
      |              |              |                 |-> Padding within 1~31  
      |              |              |                 |   bytes                
      |              |              |                                          
      |              |              |-> AES256 encrypted data (the same size as
      |              |                  the plain data), followed by padding   
      |              |                                                         
      |              |-> 32-byte SHA3 hash: keccak256(plain_data)              
      |                                                                        
      |-> 16-byte random salt, used as initial vector of AEC_CBC
    @endverbatim

    Fields Explanation
    
    [SALT]
    
    The beginning of the file is a 16-byte random salt field. The salt is used as
    the Initial Vector (IV) of the AES256_CBC encryption algorithm. The randomness
    of the IV is a security factor. With identical IV, identical plain text
    always generates identical encrypted text against identical crypto key. Thus
    in turn the attacker may infer some information of the plain text from the
    encrypted text. Random IV generates different encrypted text even if the
    plain text is identical.


    [HASH]

    The second field is a 32-byte keccak256 hash (the same keccak256 as the one
    used by Ethereum). To verify the data in decryption phase, the hash of the
    plain data is saved in the file. If any of the encrypted data, IV or the AES
    key is corrupted, the hash of the decrypted data definitely diffs from the
    original one.


    [AES256 ENCRYPTED DATA]

    The third field is the encrypted data by AES-256 CBC.

    AES is a block cipher algorithm with a block size of 16 bytes. To encrypt
    plain text of any size, some block cipher mode of operation is performed.
    AES-CBC is one of the most popular modes. It XORs every plain text block
    with the previous encrypted text block and encrypts the XORed block. For
    the first plain text block, an extra 16-byte Initial Vector (IV) is XORed.

    To decrypt an encrypted block, first decrypt the block with the same AES
    key and then XOR the decrypted text with the encrypted text of the previous
    block. This recovers the plain text block.
    
    To decrypt the first encrypted block (whose corresponding plain text block
    was XORed with IV before being encrypted), the same IV as the one at
    encryption time must be specified.


    [PKCS PADDING]

    The fourth field is padding as per OpenSSL rules.
    
    OpenSSL uses PKCS#7 padding rule for AES by default. As AES's block size is
    16 bytes, the rule adds 1-to-16-byte padding with every byte's value equal
    to how many bytes the padding is. For example, if 6 bytes are added as
    padding, the value of every byte is 0x06. Especally if no padding is required,
    this rule always adds a 16-byte padding. When decrypting, OpenSSL will check
    the padding value against PKCS#7 rule as a simple validition and automatically
    removes PKCS#7 padding by default.

    Besides the PKCS#7 padding, there is an extra 0-to-15-byte padding for data
    alignment.

    Thus the total padding size varies between 1~31 bytes. The output buffer
    must have enough room to hold the encrypted data.

    

@see BoatPersistRead()

@return
    This function returns BOAT_SUCCESS if it successfully stores the data.\n
    Otherwise it returns one of the error codes.

@param[in] storage_name_str
    The file name to store the data.

@param[in] data_ptr
    The plain data to save into file.

@param[in] data_len
    Length (in byte) of <data_ptr>.\n
    If <data_len> is 0, no file will be created.


*******************************************************************************/
BOAT_RESULT BoatPersistStore(const BCHAR *storage_name_str, const void *data_ptr, BUINT32 data_len)
{
    // Storage format: | 16 byte salt | 32 byte hash | AES(data) | 1~31 byte AES paddiing |
    // Where, hash = keccak256(data)

    BUINT8 salt_array[BOAT_STORAGE_SALT_SIZE];
    BUINT8 data_hash_array[32];
    BUINT8 encrypted_array[data_len + 31]; // 31 for AES padding
    BUINT32 encrypted_len = sizeof(encrypted_array);
    
    FILE *file_ptr;
    BSINT32 count = 0;

    BOAT_RESULT result;

    if( storage_name_str == NULL || (data_ptr == NULL && data_len != 0 ) )
    {
        return BOAT_ERROR;
    }

    if( data_len == 0 )
    {
        return BOAT_SUCCESS;
    }

    // Calculate data hash
    keccak_256(data_ptr, data_len, data_hash_array);

    
    // Generate 16-byte salt as AES initial vector
    random_stream(salt_array, BOAT_STORAGE_SALT_SIZE);

    // Encrypt the data
    result = KeystoreEncrypt(encrypted_array, &encrypted_len, salt_array, data_ptr, data_len);

    if( result == BOAT_SUCCESS )
    {
        file_ptr = fopen(storage_name_str, "wb");

        if( file_ptr != NULL )
        {
            // Store 16-byte salt
            count += fwrite(salt_array, 1, sizeof(salt_array), file_ptr);

            // Store 32-byte hash
            count += fwrite(data_hash_array, 1, sizeof(data_hash_array), file_ptr);
            
            // Store AES(data) | 0~15 byte AES paddiing
            count += fwrite(encrypted_array, 1, encrypted_len, file_ptr);

            fclose(file_ptr);
        }

    }

    return result;
}


/*!*****************************************************************************
@brief Read and decrypt data in an encrypted persistent file

Function: BoatPersistRead()

    This function reads and decrypts data from an encrypted file (written by
    BoatPersistRead()). It also checks the data hash to verify if the decrypted
    data is correct.

    NOTE:
    This is a default implementation for persistent storage with AES-256
    encryption. It assumes filesystem is supported in the system.

    In case filesystem is not supported, or some type of secure enclave is equipped,
    re-implement this function according to the system configuration.


    See BoatPersistStore() for the detailed file format.
    

@see BoatPersistStore()

@return
    This function returns BOAT_SUCCESS if it successfully stores the data.\n
    Otherwise it returns one of the error codes.

@param[in] storage_name_str
    The file name to store the data.

@param[out] data_ptr
    Pointer to hold the decrypted data.

@param[in] len_to_read
    Length (in byte) of the original plain data. It must be the same as the\n
    <data_len> in corresponding BoatPersistStore() call.

*******************************************************************************/
BOAT_RESULT BoatPersistRead(const BCHAR *storage_name_str, BOAT_OUT void *data_ptr, BUINT32 len_to_read)
{
    FILE *file_ptr;
    BSINT32 file_size;
    BSINT32 count = 0;

    // Storage format: | 16 byte salt | 32 byte hash | AES(data) | 1~31 byte AES paddiing |
    // Where, hash = keccak256(data)

    BUINT8 salt_array[BOAT_STORAGE_SALT_SIZE];
    BUINT8 original_data_hash_array[32];
    BUINT8 data_hash_array[32];
    BUINT8 encrypted_array[len_to_read + 31]; // 31 for AES padding

    // Plain buffer
    BUINT8 plain_array[len_to_read];
    BUINT32 plain_len = sizeof(plain_array);
    
    BOAT_RESULT result = BOAT_ERROR;

    if( storage_name_str == NULL || (data_ptr == NULL && len_to_read != 0 ) )
    {
        return BOAT_ERROR;
    }

    if( len_to_read == 0 )
    {
        return BOAT_SUCCESS;
    }

    file_ptr = fopen(storage_name_str, "rb");

    if( file_ptr != NULL )
    {
        fseek(file_ptr, 0, SEEK_END);
        file_size = ftell(file_ptr);
        rewind(file_ptr);

        if( file_size >= BOAT_STORAGE_SALT_SIZE + sizeof(data_hash_array) + len_to_read )
        {
            // Read salt
            count = fread(salt_array, 1, BOAT_STORAGE_SALT_SIZE, file_ptr);

            // Read original data hash
            count = fread(original_data_hash_array, 1, sizeof(original_data_hash_array), file_ptr);

            // Read rest of the file (encrypted data)
            count = fread(encrypted_array, 1, BOAT_MIN(sizeof(encrypted_array), file_size - BOAT_STORAGE_SALT_SIZE - sizeof(original_data_hash_array)), file_ptr);
            result = KeystoreDecrypt(plain_array, &plain_len, salt_array, encrypted_array, count);

            // Check size of the decrypted data matches the length to read
            if( result == BOAT_SUCCESS && plain_len == len_to_read)
            {
                // Calculate data hash from the decrypted data
                keccak_256(plain_array, len_to_read, data_hash_array);
                
                // Check if decrypted hash is the same as the original one
                if( 0 == memcmp(original_data_hash_array, data_hash_array, sizeof(data_hash_array)) )
                {
                    memcpy(data_ptr, plain_array, len_to_read);
                    result = BOAT_SUCCESS;
                }
                else
                {
                    result = BOAT_ERROR;
                }
            }
            else
            {
                result = BOAT_ERROR;
            }
        }
        fclose(file_ptr);
    }

    return result;
}


/*!*****************************************************************************
@brief Delete a persistent storage (file)

Function: BoatPersistDelete()

    This function deletes a persistent storage (file) with given name.


@return 
    This function returns BOAT_SUCCESS if successful.
    Otherwise it returns one of the error codes.

@param[in] storage_name_str
    The storage (file) name to delete.

*******************************************************************************/
BOAT_RESULT BoatPersistDelete(const BCHAR * storage_name_str)
{
    // Delete file
    if( 0 != remove(storage_name_str) )
    {
        return BOAT_ERROR;
    }
    else
    {
        return BOAT_SUCCESS;
    }
}
