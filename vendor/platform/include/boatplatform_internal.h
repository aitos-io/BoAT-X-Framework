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

#include "boatconfig.h"
#include "boatinternal.h"
#include "boatlog.h"

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
 * @return 
 *   Return \c BOAT_SUCCESS if generate success; otherwise return a negative error code.
 ******************************************************************************/
BOAT_RESULT  BoatRandom(BUINT8* output, BUINT32 outputLen, void* rsvd);


/*!****************************************************************************
 * @brief 
 *   elliptic curve signature function.
 * 
 * @details
 *   elliptic curve signature function.
 *
 * @param[in] prikeyCtx 
 *   Private key context.
 *
 * @param[in] digest
 *   pointer to disgest message.
 *
 * @param[in] digestLen 
 *   the length of digiest message.
 *
 * @param[out] signatureResult 
 *   The buffer to stored signature result.
 *
 * @param rsvd 
 *   Reserved for futrue. 
 *
 * @return 
 *   Return \c BOAT_SUCCESS if generate success; otherwise return a negative error code.
 ******************************************************************************/
BOAT_RESULT BoatSignature( BoatWalletPriKeyCtx prikeyCtx, 
						   const BUINT8* digest, BUINT32 digestLen, 
						   BoatSignatureResult* signatureResult, void* rsvd );


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
 *   File size. this field only valid when return \c BOAT_SUCCESS.
 *
 * @param rsvd
 *   Reserved for futrue. 
 *
 * @return 
 *   Return \c BOAT_SUCCESS if read success, otherwise return a negative error code.
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
 * @return  
 *   Return \c BOAT_SUCCESS if write success, otherwise return a negative error code.
 *
 * @see BoatReadFile
 ******************************************************************************/
BOAT_RESULT  BoatWriteFile( const BCHAR *fileName,
						    BUINT8* writeBuf, BUINT32 writeLen, void* rsvd );


/*!****************************************************************************
 * @brief 
 *   Read data from a file
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
 *   To be read data store buffer pointer.
 *
 * @param readLen 
 *   To be read data length.
 *   \n NOTE: If parameter 'fileName' is not NULL, and parameter 'readLen' is 
 *   greater than file size, the parameter 'readLen' will be considered to 
 *   have no exception, and read entire file. So, caller can assign a large
 *    value to this parameter if want read entire file. 
 *
 * @param rsvd
 *   Reserved for futrue. 
 *
 * @return 
 *   return \c BOAT_SUCCESS if read success, otherwise return a negative error code.
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
 *   Reserved for futrue. 
 *
 * @return  
 *   Return \c BOAT_SUCCESS if read success, otherwise return a negative error code.
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
 *   Reserved for futrue. 
 *
 * @return  
 *   Return a socket descriptor if commect success; otherwise return -1.
 ******************************************************************************/
BSINT32 BoatConnect(const BCHAR *address, void* rsvd);


#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
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
 *   The raw socket connection file descriptor.
 *
 * @param tlsContext 
 *   TLS context. it's defined at http2intf.h.
 *
 * @param rsvd 
 *   Reserved for futrue. 
 *
 * @return 
 *   Return \c BOAT_SUCCESS if read success, otherwise return a negative error code
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
 *   Reserved for futrue. 
 *
 * @return  
 *   Return send length.
 ******************************************************************************/
BSINT32 BoatSend(BSINT32 sockfd, void* tlsContext, const void *buf, size_t len, void* rsvd);


/*!****************************************************************************
 * @brief 
 *   Received data from remote net.
 * 
 * @details
 *   Received data from remote net.
 *
 * @param sockfd 
 *   Socket descriptor.
 *
 * @param buf 
 *   Receive data buffer.
 *
 * @param len 
 *   Receive buffer size.
 *
 *@param rsvd 
 *   Reserved for futrue. 
 *
 *@return  
  *   Return received length.
 ******************************************************************************/
BSINT32 BoatRecv(BSINT32 sockfd, void* tlsContext, void *buf, size_t len, void* rsvd);


/*!****************************************************************************
 * @brief 
 *   Colose a socket commection.
 *
 * @details
 *   Colose a socket commection.
 *
 * @param sockfd 
 *   Socket descriptor.
 *
 * @param rsvd 
 *   Reserved for futrue. 
 ******************************************************************************/
void    BoatClose(BSINT32 sockfd, void* tlsContext, void* rsvd);


BOAT_RESULT  BoatPort_keyCreate( const BoatWalletPriKeyCtx_config* config, BoatWalletPriKeyCtx* pkCtx );
BOAT_RESULT  BoatPort_keyQuery( const BoatWalletPriKeyCtx_config* config, BoatWalletPriKeyCtx* pkCtx );
BOAT_RESULT  BoatPort_keyDelete( BoatWalletPriKeyCtx* pkCtx );

BOAT_RESULT  BoatAesEncrypt(BUINT8 iv[16], const BUINT8 * key, const BUINT8 * input, size_t length, BUINT8 * output);
BOAT_RESULT  BoatAesDecrypt(BUINT8 iv[16], const BUINT8 * key, const BUINT8 * input, size_t length, BUINT8 * output);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of __BOATPLATFORM_INTERNAL_H__ */
