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

/*! @defgroup persiststore boat persiststore
 * @{
 */

/*!****************************************************************************
 * @brief Persistently store data in an encrypted file
 *
 * @details
 *   This function persistently stores data in an encrypted file with hash check.
 *
 * @note
 *   This is a default implementation for persistent storage with AES-256
 *   encryption. It assumes filesystem is supported in the system.
 *   \n In case filesystem is not supported, or some type of secure enclave is equipped,
 *   re-implement this function according to the system configuration.
 *   \n This function encrypts the data and saves them into the specified file in
 *   following format.   
 *   @verbatim
    File format:\n
     --------------------------------------------------------------------------
     | 16 byte salt | 32 byte hash | AES(plain_data) | 0~31 byte AES padding |
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

 *   Fields Explanation
 *   
 *   \n <b>[SALT]</b>
 *   The beginning of the file is a 16-byte random salt field. The salt is used as
 *   the Initial Vector (IV) of the AES256_CBC encryption algorithm. The randomness
 *   of the IV is a security factor. With identical IV, identical plain text
 *   always generates identical encrypted text against identical crypto key. Thus
 *   in turn the attacker may infer some information of the plain text from the
 *   encrypted text. Random IV generates different encrypted text even if the
 *   plain text is identical.
 *
 *   \n <b>[HASH]</b>
 *   The second field is a 32-byte keccak256 hash (the same keccak256 as the one
 *   used by Ethereum). To verify the data in decryption phase, the hash of the
 *   plain data is saved in the file. If any of the encrypted data, IV or the AES
 *   key is corrupted, the hash of the decrypted data definitely diffs from the
 *   original one.
 *
 *   \n <b>[AES256 ENCRYPTED DATA]</b>
 *   The third field is the encrypted data by AES-256 CBC.
 *   \n AES is a block cipher algorithm with a block size of 16 bytes. To encrypt
 *   plain text of any size, some block cipher mode of operation is performed.
 *   AES-CBC is one of the most popular modes. It XORs every plain text block
 *   with the previous encrypted text block and encrypts the XORed block. For
 *   the first plain text block, an extra 16-byte Initial Vector (IV) is XORed.
 *   \n To decrypt an encrypted block, first decrypt the block with the same AES
 *   key and then XOR the decrypted text with the encrypted text of the previous
 *   block. This recovers the plain text block.
 *   \n To decrypt the first encrypted block (whose corresponding plain text block
 *   was XORed with IV before being encrypted), the same IV as the one at
 *   encryption time must be specified.
 *
 *   \n <b>[PKCS PADDING]</b>
 *
 *   The fourth field is padding as per OpenSSL rules.
 *   
 *   \n OpenSSL uses PKCS#7 padding rule for AES by default. As AES's block size is
 *   16 bytes, the rule adds 1-to-16-byte padding with every byte's value equal
 *   to how many bytes the padding is. For example, if 6 bytes are added as
 *   padding, the value of every byte is 0x06. Especally if no padding is required,
 *   this rule always adds a 16-byte padding. When decrypting, OpenSSL will check
 *   the padding value against PKCS#7 rule as a simple validition and automatically
 *   removes PKCS#7 padding by default.
 *   \n Besides the PKCS#7 padding, there is an extra 0-to-15-byte padding for data
 *   alignment.
 *   \n Thus the total padding size varies between 1~31 bytes. The output buffer
 *   must have enough room to hold the encrypted data.
 *
 * @param[in] storage_name_str
 *   The file name to store the data.
 *
 * @param[in] data_ptr
 *   The plain data to save into file.
 *
 * @param[in] data_len
 *   Length (in byte) of <data_ptr>.\n
 *   If <data_len> is 0, no file will be created.
 *
 * @return
 *   This function returns BOAT_SUCCESS if it successfully stores the data.\n
 *   Otherwise it returns one of the error codes.
 *
 * @see BoatPersistRead() 
 ******************************************************************************/
BOAT_RESULT BoatPersistStore( const BCHAR *storage_name_str, const void *data_ptr, 
							  BUINT32 data_len );


/*!****************************************************************************
 * @brief Read and decrypt data in an encrypted persistent file
 *
 * @details
 *   This function reads and decrypts data from an encrypted file (written by
 *   BoatPersistStore()). It also checks the data hash to verify if the decrypted
 *   data is correct.
 *
 * @note
 *   This is a default implementation for persistent storage with AES-256
 *   encryption. It assumes filesystem is supported in the system.
 *   \n In case filesystem is not supported, or some type of secure enclave is equipped,
 *   re-implement this function according to the system configuration.
 *   See BoatPersistStore() for the detailed file format.
 *  
 * @param[in] storage_name_str
 *   The file name to store the data.
 *
 * @param[out] data_ptr
 *   Pointer to hold the decrypted data.
 *
 * @param[in] len_to_read
 *   Length (in byte) of the original plain data. It must be the same as the\n
 *   <data_len> in corresponding BoatPersistStore() call.
 *  
 * @return
 *   This function returns BOAT_SUCCESS if it successfully read the data.\n
 *   Otherwise it returns one of the error codes.
 *
 * @see BoatPersistStore()
 ******************************************************************************/
BOAT_RESULT BoatPersistRead( const BCHAR *storage_name_str, BOAT_OUT void *data_ptr, 
							 BUINT32 len_to_read );


/*!****************************************************************************
 * @brief Delete a persistent storage (file)
 *
 * @details
 *   This function deletes a persistent storage (file) with given name.
 *
 * @param[in] storage_name_str
 *   The storage (file) name to delete.
 *
 * @return 
 *   This function returns BOAT_SUCCESS if successful.
 *   Otherwise it returns one of the error codes.
 ******************************************************************************/
BOAT_RESULT BoatPersistDelete( const BCHAR * storage_name_str );


/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
