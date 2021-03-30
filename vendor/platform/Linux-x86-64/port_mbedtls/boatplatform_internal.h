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

/**
@brief platform interface
@file boatPlatform_intarnal.h contains boat platform port interface functions.
*/

#ifndef __BOATPLATFORM_INTERNAL_H__
#define __BOATPLATFORM_INTERNAL_H__

/* self header include */
#include "boatinternal.h"

//!@brief boat SDK signature result
typedef struct TBoatSignatureResult
{
	BBOOL   native_format_used;  //!< ture: used, false:unused
	BUINT8  native_sign[64];
	
	BBOOL   pkcs_format_used;
	BUINT8  pkcs_sign[256];
	
	BBOOL   signPrefix_used;
	BUINT8  signPrefix;
}BoatSignatureResult;


#ifdef __cplusplus
extern "C" {
#endif


/*! @defgroup platform boat platform
 * @{
 */

/*!****************************************************************************
 * @brief 
 *   random number generate function.
 * 
 * @details
 *   random number generate function.
 *
 * @param[out] output
 *   buffer of storage randrom. Caller should provide enough space to store it.
 *
 * @param[in] outputLen 
 *   expect random number length. it cannot greater than <b>output</b> size.
 *   usually, it equal with <b>output</b> size.
 *
 * @param rsvd 
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT
 *   return BOAT_SUCCESS if generate success; otherwise return a negative error code.
 ******************************************************************************/
BOAT_RESULT  BoatRandom(BUINT8* output, BUINT32 outputLen, void* rsvd);


#if 0
/*!****************************************************************************
 * @brief 
 *   elliptic curve signature function.
 * 
 * @details
 *   elliptic curve signature function.
 *
 * @param type 
 *	 the signature type.
 *   \n - #BOAT_SIGNATURE_SECP256K1.
 *   \n - #BOAT_SIGNATURE_SECP256R1.
 *
 * @param[in] priKeyId 
 * 	 priKeyId can be the full path of a private key, also the content of private key.
 *   If priKeyId contains both the following:
 *      1. "-----BEGIN "
 *      2. "-----END "
 *      3. " PRIVATE KEY-----"
 *   priKeyId is considered the content of private key; otherwise the full path of 
 *   a private key.
 *
 * @param[in] digest
 *   pointer to disgest message.
 *
 * @param[in] digestLen 
 *   the length of digiest message.
 *
 * @param[out] signature 
 *   ASN.1 format signature.The signature size maximum 139 bytes, caller needs to 
 *   make sure that there is enough space to store it.
 *
 * @param[out] signatureLen 
 *   ASN.1 format signature length.
 *
 * @param[out] r 
 *   the r field of the native signature, the space of this filed is fixed 32 bytes.
 *   caller needs to make sure that there is enough space to store it.
 *
 * @param[out] s 
 *   the s field of the native signature, the space of this filed is fixed 32 bytes.
 *   caller needs to make sure that there is enough space to store it.
 *
 * @param[out] signaturePrefix 
 *   the generate signature Prefix.
 *
 * @param rsvd 
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if generate success; otherwise return a negative error code
 ******************************************************************************/
BOAT_RESULT  BoatSignature( const BoatSignatureAlgType type, const BUINT8* priKeyId,
							const BUINT8* digest, BUINT32 digestLen, BUINT8* signature,
						    size_t* signatureLen, BUINT8* r, BUINT8* s,
							BUINT8* signaturePrefix, void* rsvd );
#endif

BOAT_RESULT BoatSignature( BoatWalletPriKeyCtx prikeyCtx, 
						   const BUINT8* digest, BUINT32 digestLen, 
						   BoatSignatureResult* signatureResult, void* rsvd );


#if 0
/*!****************************************************************************
 * @brief 
 *   generate one-time ECP private key.
 *
 * @details
 *   generate one-time ECP private key.
 *
 * @param type 
 *	 the signature type.
 *   \n - #BOAT_SIGNATURE_SECP256K1.
 *   \n - #BOAT_SIGNATURE_SECP256R1.
 *
 * @param[out] outbuf 
 *   pointer to store the generate private key.
 *
 * @param[in] maxLen 
 *   param <b>outbuf</b> size.
 *
 * @param[out] pubX 
 *   X point of Corresponding public key.
 *
 * @param[out] pubY 
 *   Y point of Corresponding public key.
 *
 * @param rsvd
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if gennerate success; otherwise return a negative error code.
 ******************************************************************************/
BOAT_RESULT BoatGenOnetimeSignPrikey( const BoatSignatureAlgType type, BUINT8 *outbuf, 
								      BUINT32 maxLen, BUINT8 *pubX, BUINT8 *pubY, void* rsvd );


/*!****************************************************************************
 * @brief 
 *   check the privete is exists or not.
 * 
 * @details
 *   check the privete is exists or not.
 *
 * @param prikeyId 
 *   the full path of a private key.
 *
 * @param pubX 
 *   X point of Corresponding public key.
 *
 * @param pubY 
 *   Y point of Corresponding public key.
 *
 * @param rsvd
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if gennerate success; otherwise return a negative error code.
 ******************************************************************************/
BOAT_RESULT  BoatChkPrikeyExist( const BUINT8 * prikeyId, 
								 BUINT8 *pubX, BUINT8 *pubY, void* rsvd );
#endif

/*!****************************************************************************
 * @brief 
 *   get file size
 *
 * @details
 *   This function both supports file system and no file system operations.
 *   \n For file-system operations, parameter 'fileName' is the file name to 
 *    be operationed; 
 *   \n For no-file-system operations, parameter 'fileName' will be ignored,
 *   re-writer should give a physical address that to be accessed at internal
 *   of this function.
 *   \n Default, only with-file-system operation be implemented, other situation
 *   be implemented by user.
 *
 * @param fileName 
 *    File name of to be get size. 
 *
 * @param[out] size 
 *   file size. this field only valid when return BOAT_SUCCESS.
 *
 * @param rsvd
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if read success, otherwise return a negative error code
 ******************************************************************************/
BOAT_RESULT  BoatGetFileSize( const BCHAR *fileName, BUINT32 *size, void* rsvd );


/*!****************************************************************************
 * @brief 
 *   write data to a file
 *
 * @details
 *   This function both supports file system and no file system operations.
 *   \n For file-system operations, parameter 'fileName' is the file name to 
 *    be operationed; 
 *   \n For no-file-system operations, parameter 'fileName' will be ignored,
 *   re-writer should give a physical address that to be accessed at internal
 *   of this function.
 *   \n Default, only with-file-system operation be implemented, other situation
 *   be implemented by user.
 *
 * @param fileName 
 *   File name of to be write. 
 *
 * @param writeBuf 
 *   to be write data pointer.
 *
 * @param writeLen 
 *   to be write data length.
 *
 * @param rsvd
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if write success, otherwise return a negative error code.
 *
 * @see BoatReadFile
 ******************************************************************************/
BOAT_RESULT  BoatWriteFile( const BCHAR *fileName,
						    BUINT8* writeBuf, BUINT32 writeLen, void* rsvd );


/*!****************************************************************************
 * @brief 
 *   read data from a file
 *
 * @details
 *   This function both supports file system and no file system operations.
 *   \n For file-system operations, parameter 'fileName' is the file name to 
 *    be operationed; 
 *   \n For no-file-system operations, parameter 'fileName' will be ignored,
 *   re-writer should give a physical address that to be accessed at internal
 *   of this function.
 *   \n Default, only with-file-system operation be implemented, other situation
 *   be implemented by user.
 *
 * @param fileName 
 *   File name of to be read. 
 *
 * @param readBuf 
 *   to be read data store buffer pointer.
 *
 * @param readLen 
 *   to be read data length.
 *   \n NOTE: if parameter 'fileName' is not NULL, and parameter 'readLen' is 
 *   greater than file size, the parameter 'readLen' will be considered to 
 *   have no exception, and read entire file. So, caller can assign a large
 *    value to this parameter if want read entire file. 
 *
 * @param rsvd
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if read success, otherwise return a negative error code
 *
 * @see BoatWriteFile
 ******************************************************************************/
BOAT_RESULT  BoatReadFile( const BCHAR *fileName,
						   BUINT8* readBuf, BUINT32 readLen, void* rsvd );


/*!****************************************************************************
 * @brief 
 *   remove a file 
 *
 * @details
 *   This function both supports file system and no file system operations.
 *   \n For file-system operations, parameter 'fileName' is the file name to 
 *    be operationed; 
 *   \n For no-file-system operations, parameter 'fileName' will be ignored,
 *   re-writer should give a physical address that to be accessed at internal
 *   of this function.
 *   \n Default, only with-file-system operation be implemented, other situation
 *   be implemented by user.
 *
 * @param fileName 
 *   File name of to be removed. 
 *
 * @param rsvd
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if read success, otherwise return a negative error code
 ******************************************************************************/
BOAT_RESULT  BoatRemoveFile( const BCHAR *fileName, void* rsvd );


/*!****************************************************************************
 * @brief 
 *   connect to network
 *
 * @details
 *   connect to network
 *
 * @param address 
 *   network address, include URL/IP adress and IP port. e.g. a.b.com:7051
 *
 * @param rsvd
 *   reserved for futrue. 
 *
 * @return BSINT32 
 *   return a socket descriptor if commect success; otherwise return -1.
 ******************************************************************************/
BSINT32 BoatConnect(const BCHAR *address, void* rsvd);


#if (HLFABRIC_TLS_SUPPORT == 1)
/*!****************************************************************************
 * @brief initinal TLS connection
 * 
 * @details
 *   initinal TLS connection
 *
 * @param hostName
 *   host name. It's the N field of server certificate.
 *
 * @param caChain 
 *   rootCA certificate content list address.
 *
 * @param socketfd 
 *   the raw socket connection file descriptor.
 *
 * @param tlsContext 
 *   TLS context. it's defined at http2intf.h.
 *
 * @param rsvd 
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if read success, otherwise return a negative error code
 ******************************************************************************/
BOAT_RESULT BoatTlsInit( const BCHAR *hostName, const BoatFieldVariable *caChain,
						 BSINT32 socketfd, void* tlsContext, void* rsvd );
#endif


/*!****************************************************************************
 * @brief 
 *   send data to remote
 *
 * @details
 *   send data to remote
 *
 * @param sockfd 
 *   socket descriptor
 *
 * @param buf 
 *   send data buffer
 *
 * @param len 
 *   send data length
 *
 * @param rsvd 
 *        reserved for futrue. 
 *
 * @return BSINT32 
 *   return send length.
 ******************************************************************************/
BSINT32 BoatSend(BSINT32 sockfd, void* tlsContext, const void *buf, size_t len, void* rsvd);


/*!****************************************************************************
 * @brief 
 *   received data from remote net
 * 
 * @details
 *   received data from remote net
 *
 * @param sockfd 
 *   socket descriptor
 *
 * @param buf 
 *   receive data buffer
 *
 * @param len 
 *   receive buffer size
 *
 *@param rsvd 
 *        reserved for futrue. 
 *
 *@return BSINT32 
  *   return received length.
 ******************************************************************************/
BSINT32 BoatRecv(BSINT32 sockfd, void* tlsContext, void *buf, size_t len, void* rsvd);


/*!****************************************************************************
 * @brief 
 *   colose a socket commection
 *
 * @details
 *   colose a socket commection
 *
 * @param sockfd 
 *   socket descriptor
 *
 * @param rsvd 
 *        reserved for futrue. 
 ******************************************************************************/
void    BoatClose(BSINT32 sockfd, void* tlsContext, void* rsvd);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of __BOATPLATFORM_INTERNAL_H__ */
