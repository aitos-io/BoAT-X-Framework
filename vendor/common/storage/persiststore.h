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

#define BOAT_KEYSTORE_PRIKEY_NUM_LEN  4  // AA 55 NUM CRC
#define BOAT_KEYSTORE_PRIKEY_LEN  1+16+32+32+16  // L : 1byte , 16: 16bytes random , 32 : hash , 32 : prikey ,16 : padding
#define BOAT_KEYSTORE_PRIKEYINDEX_LEN 2 // 01+INDEX

#define BOAT_FILE_STOREDATA	"boat_wallet_file"
#define BOAT_STORAGE_KEYPAIR_OFFSET 0
#define BOAT_STORAGE_KEYPAIR_MAXLEN BOAT_KEYPAIR_NUM_LEN+BOAT_KEYPAIR_ALL_LEN_MAX+ \
                                    BOAT_MAX_KEYPAIR_NUM* \
                                    (BOAT_KEYPAIR_INDEX_LEN+ \
                                    BOAT_KEYPAIR_NAME_LEN_MAX+ \
                                    BOAT_KEYPAIR_FORMAT_LEN+ \
                                    BOAT_KEYPAIR_TYPE_LEN + \
                                    BOAT_KEYPAIR_PUBKEY_LEN  \
                                    )

#define BOAT_STORAGE_PRIKEY_OFFSET BOAT_STORAGE_KEYPAIR_OFFSET+BOAT_STORAGE_KEYPAIR_MAXLEN + 128  // reserved 128 bytes
#define BOAT_STORAGE_PRIKEY_MAXLEN BOAT_KEYSTORE_PRIKEY_NUM_LEN + BOAT_MAX_KEYPAIR_NUM*(BOAT_KEYSTORE_PRIKEYINDEX_LEN+BOAT_KEYSTORE_PRIKEY_LEN)

#define BOAT_STORAGE_NETWORK_OFFSET BOAT_STORAGE_PRIKEY_OFFSET+BOAT_STORAGE_PRIKEY_MAXLEN + 128  // reserved 128 bytes


/*! @defgroup persiststore boat persiststore
 * @{
 */

//! @brief The store file for keypair or network
typedef enum
{
    BOAT_STORE_KEYPAIR,  //!< to store keypair data
    BOAT_STORE_NETWORK, //!< to store network data
    BOAT_STORE_PRIKEY,  //!< to store prikey data
}BoatStoreFile;

/*!****************************************************************************
@brief Store SoftRoT context to NVRAM

@details
    This function stores any SoftRoT implementation data (e.g. activation code) to
    persistent NVRAM. The NVRAM size varies for different hardware
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
BOAT_RESULT BoATStoreSoftRotNvram(BoatStoreFile storeFile ,BUINT32 offset,BUINT8 const *data_ptr,BUINT32 data_len,BoatStoreType storeType);


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
BOAT_RESULT BoatReadSoftRotNvram(BoatStoreFile storeFile ,BUINT32 offset, BUINT8 *data_ptr, BUINT32 len_to_read ,BoatStoreType storeType);


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

BOAT_RESULT BoATSoftRotNvramEnc(BUINT8 const *data_ptr,BUINT32 data_len,BUINT8 *out, BUINT32 *outlen);


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
BOAT_RESULT BoATSoftRotNvramDec(BUINT8 const *data_ptr,BUINT32 data_len,BUINT8 *out, BUINT32 *outlen);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
