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

/*!@brief platform internal interface

@file boatplatform_internal.c contains platform internal interface functions.
*/

//! self header include
#include "boatconfig.h"
#include "boatplatform_internal.h"
#include "boattypes.h"
#include "persiststore.h"
#include "boatkeystore.h"

#include "rand.h"
#include "aes/aes.h"
#include "sha3.h"
#include "secp256k1.h"
#include "nist256p1.h"
#include "bignum.h"
#include <string.h>

/* net releated include */
#if (PROTOCOL_USE_HLFABRIC == 1)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#endif
#include <sys/time.h>


// fibocom include
#include "qapi_fibocom.h"
#include "qapi_fs.h"
#include "locale.h"

#if (PROTOCOL_USE_HLFABRIC == 1)
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
// for TTLSContext structure
#include "http2intf.h"
#endif
#endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */



#define GENERATE_KEY_REPEAT_TIMES	100

#define BoAT_DIR_PATH_ROOT		"/datatx/dir_boat"



uint32_t random32(void)
{
	
	qapi_Status_t status;
	uint16_t randBuf[2]={0};
	uint32_t rand32;
	status = qapi_fibo_random_data_get(2,randBuf);
	if(status != QAPI_OK)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to get random data.");
		return 0;
	}

	memcpy(&rand32,randBuf,4);
	
	return rand32;
}

BOAT_RESULT BoatRandom(BUINT8 *output, BUINT32 outputLen, void *rsvd)
{	
	BOAT_RESULT result = BOAT_SUCCESS;
	qapi_Status_t status;
	
	(void)rsvd;

	if(outputLen <= 20u)
	{
		uint16 randBuf[20]={0};
		status = qapi_fibo_random_data_get((uint16)outputLen,randBuf);
		if(status != QAPI_OK)
		{
			BoatLog(BOAT_LOG_CRITICAL, "Fail to get random data.");
			result=BOAT_ERROR;
			goto end;
		}
		memcpy(output,randBuf,outputLen);
	}
	else
	{
		uint16 *randHeapBuf = NULL;
		randHeapBuf=BoatMalloc(outputLen);
		if(randHeapBuf == NULL)
		{
			BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate random buffer.");
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		status = qapi_fibo_random_data_get((uint16)outputLen,randHeapBuf);
		if(status != QAPI_OK)
		{
			BoatLog(BOAT_LOG_CRITICAL, "Fail to get random data.");
			result=BOAT_ERROR;
			BoatFree(randHeapBuf);
			goto end;
		}
		memcpy(output,randHeapBuf,outputLen);
		BoatFree(randHeapBuf);
	}	

end:	
	return result;
}

BOAT_RESULT BoatSignature(BoatKeypairPriKeyCtx prikeyCtx,
						  const BUINT8 *digest, BUINT32 digestLen,
						  BoatSignatureResult *signatureResult, void *rsvd)
{
	BUINT8 signature[64] = {0};
	BUINT8 signatureTmp[139];
	BUINT32 signatureTmpLen = 0;
	BUINT8 ecdsPrefix = 0;
	BUINT32 signatureLen = 0;
	BOAT_RESULT result = BOAT_SUCCESS;

	(void)rsvd;

	/* param check */
	if ((digest == NULL) || (signatureResult == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	// result = BoAT_GetPirkeyByIndex(prikeyCtx.keypair_index,&prikey);
	// if(result != BOAT_SUCCESS){
	// 	BoatLog(BOAT_LOG_CRITICAL, "get keypair prikey fail.");
	// 	return BOAT_ERROR;
	// }

	// result = BoAT_sign(prikeyCtx.prikey_type,prikeyCtx.prikey_format, prikey.value,prikey.value_len,digest,digestLen,signatureTmp,&signatureLen,&ecdsPrefix);

	result = BoAT_Keystore_Sign(prikeyCtx.prikey_type, prikeyCtx.keypair_index, digest, digestLen, signature, &signatureLen, &ecdsPrefix);
	if (result != BOAT_SUCCESS)
	{
		return result;
	}

	// // signature result assign
	// memset(signatureResult, 0, sizeof(BoatSignatureResult));

	// signatureResult->native_format_used = true;
	// memcpy(signatureResult->native_sign, signatureTmp, 64);

	// signatureResult->signPrefix_used = true;
	// signatureResult->signPrefix      = ecdsPrefix;

	/* convert r,s to asn.1 */
	result = utility_signature_to_asn1(signature, signatureLen, signatureTmp, &signatureTmpLen);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "signature to asn.1  fail.");
		return BOAT_ERROR;
	}
	// signature result assign
	memset(signatureResult, 0, sizeof(BoatSignatureResult));

	signatureResult->pkcs_format_used = true;
	signatureResult->pkcs_sign_length = signatureTmpLen;
	memcpy(signatureResult->pkcs_sign, signatureTmp, signatureResult->pkcs_sign_length);

	signatureResult->native_format_used = true;
	memcpy(&signatureResult->native_sign[0], signature, 64);

	signatureResult->signPrefix_used = true;
	signatureResult->signPrefix = ecdsPrefix;

	return result;
}


/******************************************************************************
                              BOAT FILE OPERATION WARPPER
*******************************************************************************/
#define S_IFDIR        	0040000 	/**< Directory */
#define S_IFMT          0170000		/**< Mask of all values */
#define S_ISDIR(m)     	(((m) & S_IFMT) == S_IFDIR)

/*
@func
  fibocom_efs_is_dir_exists
@brief
  find the path is a directory or directory is exist 
*/
int fibocom_efs_is_dir_exists(const char *path)
{
     int result, ret_val = 0;
     struct qapi_FS_Stat_Type_s sbuf;

     memset (&sbuf, 0, sizeof (sbuf));

     result = qapi_FS_Stat(path, &sbuf);

     if (result != 0)
          goto End;

     if (S_ISDIR (sbuf.st_Mode) == 1)
          ret_val = 1;

     End:
          return ret_val;
}


BOAT_RESULT BoatGetFileSize(const BCHAR *fileName, BUINT32 *size, void *rsvd)
{
	qapi_FS_Status_t status = QAPI_OK;
  	int    fd = -1;
	qapi_FS_Offset_t seek_status = 0;

	int fNameLen = 0;
	int fullPathNameLen = 0;
	BCHAR *fullFilePath = NULL;

	fNameLen = strlen(fileName);
	fullPathNameLen = strlen((char *)BoAT_DIR_PATH_ROOT)+1+fNameLen;

	/// MUST SETTING
	setlocale(LC_ALL, "C");	/// <locale.h>

	if(fullPathNameLen < 200)
	{
		BCHAR fullFileName[200]={0};

		strcat(fullFileName,BoAT_DIR_PATH_ROOT);
		strcat(fullFileName,"/");
		strcat(fullFileName,fileName);

		status = qapi_FS_Open(fullFileName, QAPI_FS_O_RDONLY_E, &fd);
	}
	else
	{
		fullFilePath=BoatMalloc(fullPathNameLen+1);
		if(fullFilePath == NULL)
		{
			BoatLog(BOAT_LOG_CRITICAL,"BoatMalloc ERROR!");
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}

		strcat(fullFilePath,BoAT_DIR_PATH_ROOT);
		strcat(fullFilePath,"/");
		strcat(fullFilePath,fileName);

		status = qapi_FS_Open(fullFilePath, QAPI_FS_O_RDONLY_E, &fd);
	}

	if(fullFilePath)
	{
		BoatFree(fullFilePath);
	}

	if((status != QAPI_OK) && (-1 == fd))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", fileName);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	status = qapi_FS_Seek(fd, 0, QAPI_FS_SEEK_END_E, &seek_status);
	if(status != QAPI_OK)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to qapi_FS_Seek.");
		qapi_FS_Close(fd);
		return BOAT_ERROR_PERSISTER_READ_FAIL;
	}

	*size = (BUINT32)seek_status;

	qapi_FS_Close(fd);
	return BOAT_SUCCESS;
}


BOAT_RESULT BoatWriteStorage(BUINT32 offset, BUINT8 *writeBuf, BUINT32 writeLen, void *rsvd)
{

	qapi_FS_Status_t status = QAPI_OK;
  	int    fd = -1;
	int   wr_bytes = 0;
	(void)rsvd;
	int fNameLen=0;
	int fullPathNamelen=0;
	BCHAR *fullFilePath=NULL;

	BSINT32 count = 0;
	BUINT32 size = 0;
	BUINT8 *buf_zero = NULL;

	qapi_FS_Offset_t seek_status = 0;

	/// MUST SETTING
	setlocale(LC_ALL, "C");	/// <locale.h>

	///create boat dir;
	if(fibocom_efs_is_dir_exists(BoAT_DIR_PATH_ROOT) != 1)
	{
		status = qapi_FS_Mk_Dir(BoAT_DIR_PATH_ROOT, 0677);
		BoatLog(BOAT_LOG_CRITICAL,"qapi_FS_Mk_Dir %d", status);
	}

	fNameLen = strlen(fileName);
	fullPathNamelen = strlen((char *)BoAT_DIR_PATH_ROOT)+1+fNameLen;
	
	if(fullPathNamelen < 200)
	{
		BCHAR fullFileName[200]={0};

		strcat(fullFileName,BoAT_DIR_PATH_ROOT);
		strcat(fullFileName,"/");
		strcat(fullFileName,BOAT_FILE_STOREDATA);

	}
	else
	{
		fullFilePath=BoatMalloc(fullPathNamelen+1);
		if(fullFilePath == NULL)
		{
			BoatLog(BOAT_LOG_CRITICAL,"BoatMalloc ERROR!");
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}

		strcat(fullFilePath,BoAT_DIR_PATH_ROOT);
		strcat(fullFilePath,"/");
		strcat(fullFilePath,BOAT_FILE_STOREDATA);

		
	}

	BoatGetFileSize(fullFilePath,&size,NULL);

	status = qapi_FS_Open(fullFilePath, QAPI_FS_O_RDWR_E | QAPI_FS_O_CREAT_E, &fd);

	if(fullFilePath)
	{
		BoatFree(fullFilePath);
	}
	if((status != QAPI_OK) && (-1 == fd))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", fileName);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}


	/*move to the end of the file*/
	BoatLog(BOAT_LOG_NORMAL, "size = %d , offset = %d ", size, offset);
	if (size < offset)
	{
		buf_zero = BoatMalloc(offset - size);
		if (NULL == buf_zero)
		{
			qapi_FS_Close(fd);
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		memset(buf_zero, 0x00, offset - size);
		//count = fwrite(buf_zero, 1, offset - size, file_ptr);
		qapi_FS_Write(fd,buf_zero,offset - size,&count);
		BoatFree(buf_zero);
		if (count != (offset - size))
		{
			//fclose(file_ptr);
			qapi_FS_Close(fd);
			BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
			return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
		}
		//fseek(file_ptr, 0, SEEK_END);
		qapi_FS_Seek(fd,0,QAPI_FS_SEEK_END_E, &seek_status);
	}
	else
	{
		//fseek(file_ptr, offset, SEEK_SET);
		qapi_FS_Seek(fd,offset,QAPI_FS_SEEK_SET_E, &seek_status);
	}

	status = qapi_FS_Write(fd, writeBuf, writeLen, &wr_bytes);
	if(status != QAPI_OK)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to qapi_FS_Write.");
		qapi_FS_Close(fd);
		return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
	}
	qapi_FS_Close(fd);

	if (wr_bytes != writeLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", fileName);
		return BOAT_ERROR_PERSISTER_STORE_FAIL;
	}

	return BOAT_SUCCESS;
}


BOAT_RESULT BoatReadStorage(BUINT32 offset, BUINT8 *readBuf, BUINT32 readLen, void *rsvd)
{
	qapi_FS_Status_t status = QAPI_OK;
  	int    fd = -1;
	int   rd_bytes = 0;
	(void)rsvd;
	int fNameLen = 0;
	int fullPathNameLen = 0;
	BCHAR *fullFilePath = NULL;

	BSINT32 count = 0;
	BUINT32 size = 0;
	BUINT8 *buf_zero = NULL;

	fNameLen = strlen(fileName);
	fullPathNameLen = strlen((char *)BoAT_DIR_PATH_ROOT)+1+fNameLen;
	
	/// MUST SETTING
	setlocale(LC_ALL, "C");	/// <locale.h>

	if(fullPathNameLen < 200)
	{
		BCHAR fullFileName[200]={0};

		strcat(fullFileName,BoAT_DIR_PATH_ROOT);
		strcat(fullFileName,"/");
		strcat(fullFileName,BOAT_FILE_STOREDATA);

	}
	else
	{
		fullFilePath=BoatMalloc(fullPathNameLen+1);
		if(fullFilePath == NULL)
		{
			BoatLog(BOAT_LOG_CRITICAL,"BoatMalloc ERROR!");
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}

		strcat(fullFilePath,BoAT_DIR_PATH_ROOT);
		strcat(fullFilePath,"/");
		strcat(fullFilePath,BOAT_FILE_STOREDATA);


	}

	BoatGetFileSize(fullFilePath,&size,NULL);

	status = qapi_FS_Open(fullFilePath, QAPI_FS_O_RDONLY_E, &fd);

	if(fullFilePath)
	{
		BoatFree(fullFilePath);
	}

	if((status != QAPI_OK) && (-1 == fd))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", fileName);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	if (readLen + offset > size)
	{
		qapi_FS_Close(fd);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}

	qapi_FS_Seek(fd,offset,QAPI_FS_SEEK_SET_E, &seek_status);

	status = qapi_FS_Read(fd, readBuf, readLen, &rd_bytes);
	if(status != QAPI_OK)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to qapi_FS_Read.");
		qapi_FS_Close(fd);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	qapi_FS_Close(fd);

	if (rd_bytes != readLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", fileName);
		return BOAT_ERROR_PERSISTER_READ_FAIL;
	}

	return BOAT_SUCCESS;
}


BOAT_RESULT BoatRemoveFile(const BCHAR *fileName, void *rsvd)
{
	(void)rsvd;
		
	if (fileName == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'fileName' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	int status = 0;
	int fNameLen = 0;
	int fullPathNameLen = 0;
	BCHAR *fullFilePath = NULL;

	fNameLen = strlen(fileName);
	fullPathNameLen = strlen((char *)BoAT_DIR_PATH_ROOT)+1+fNameLen;
	

	/// MUST SETTING
	setlocale(LC_ALL, "C");	/// <locale.h>

	if(fullPathNameLen < 200)
	{
		BCHAR fullFileName[200]={0};

		strcat(fullFileName,BoAT_DIR_PATH_ROOT);
		strcat(fullFileName,"/");
		strcat(fullFileName,fileName);

		status=qapi_FS_Unlink(fullFileName);
	}
	else
	{
		fullFilePath=BoatMalloc(fullPathNameLen+1);
		if(fullFilePath == NULL)
		{
			BoatLog(BOAT_LOG_CRITICAL,"BoatMalloc ERROR!");
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		
		strcat(fullFilePath,BoAT_DIR_PATH_ROOT);
		strcat(fullFilePath,"/");
		strcat(fullFilePath,fileName);

		status=qapi_FS_Unlink(fullFilePath);
	}

	if(fullFilePath)
	{
		BoatFree(fullFilePath);
	
	}
	if(QAPI_OK != status)
    {

        return BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL;
    }
    else
    {	
        return BOAT_SUCCESS;
    }
}

#if (PROTOCOL_USE_HLFABRIC == 1)
/******************************************************************************
                              BOAT SOCKET WARPPER
					        THIS ONLY USED BY FABRIC
*******************************************************************************/
BSINT32 BoatConnect(const BCHAR *address, void *rsvd)
{
    int                connectfd;
    char               ip[64];
    char               port[8];
    char               *ptr = NULL;
    struct hostent     *he; 
    struct sockaddr_in server;
    struct sockaddr    localaddr;
    struct sockaddr_in *localaddr_ptr;
    socklen_t          addrlen = sizeof(struct sockaddr);

    (void)rsvd;

    ptr = strchr(address, ':');
    if (NULL == ptr)
    {
        BoatLog(BOAT_LOG_CRITICAL, "invalid address:%s.", address);
        return -1;
    }

    memset(ip  , 0      , sizeof(ip));
    memset(port, 0      , sizeof(port));
    memcpy(ip  , address, (int)(ptr - address));
    memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));

    if ((he = gethostbyname(ip)) == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "gethostbyname() error");
        return -1;
    }

    if ((connectfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }

    struct timeval timeout = {0, 500 * 1000};
    setsockopt(connectfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
    setsockopt(connectfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(port));
    server.sin_addr = *((struct in_addr *)(he->h_addr_list[0])); 

    if (connect(connectfd, (struct sockaddr *)&server,sizeof(struct sockaddr)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error");
        close(connectfd);
        return -1;
    }
    if (getsockname(connectfd, &localaddr, &addrlen) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "getsockname() error");
        close(connectfd);
        return -1;
    }
    else
    {
        localaddr_ptr = (struct sockaddr_in*)&localaddr;
        BoatLog(BOAT_LOG_VERBOSE, "localIP: %s:%d.", 
        inet_ntoa(localaddr_ptr->sin_addr), htons(localaddr_ptr->sin_port));
    }

    BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, connectfd);

    return connectfd;
}


#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)	
BOAT_RESULT BoatTlsInit(const BCHAR *hostName, const BoatFieldVariable *caChain,
						BSINT32 socketfd, void *tlsContext, void *rsvd)
{
	
	//! @todo BoatTlsInit implementation in crypto default.
	return BOAT_ERROR;
}
#endif


BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd)
{
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) 
	//! @todo BOAT_HLFABRIC_TLS_SUPPORT implementation in crypto default.
	return -1;
#else
	return send(sockfd, buf, len, 0);	
#endif	
}


BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
{
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) 
	//! @todo BOAT_HLFABRIC_TLS_SUPPORT implementation in crypto default.
	return -1;
#else
	return recv(sockfd, buf, len, 0);
#endif	
}


void BoatClose(BSINT32 sockfd, void *tlsContext, void *rsvd)
{
	close(sockfd);
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) 
	// free tls releated
	//! @todo BOAT_HLFABRIC_TLS_SUPPORT implementation in crypto default.
#endif
}
#endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */

/******************************************************************************
                              BOAT KEY PROCESS WARPPER
*******************************************************************************/
static BOAT_RESULT sBoatPort_keyCreate_internal_generation(const BoatKeypairPriKeyCtx_config *config,
														   BoatKeypairDataCtx *pkCtx)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatKeypairKeypair keypair;
	// result = BoAT_Keypair_generation(config->prikey_type,config->prikey_format,&keypair);
	result = BoAT_Keystore_Gen_Keypair(config->prikey_type, &keypair);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "generate private key failed.");
		return result;
	}

	// 2- update private key format
	pkCtx->prikeyCtx.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;

	// 3- update private key type
	pkCtx->prikeyCtx.prikey_type = config->prikey_type;
	pkCtx->extraData.value_len = keypair.prikey.value_len;
	memcpy(pkCtx->extraData.value, keypair.prikey.value, keypair.prikey.value_len);
	memcpy(pkCtx->prikeyCtx.pubkey_content, keypair.pubkey.value, 64);

	return result;
}

static BOAT_RESULT sBoatPort_keyCreate_external_injection_native(const BoatKeypairPriKeyCtx_config *config,
																 BoatKeypairDataCtx *pkCtx)
{
	BUINT32 len = 0;
	BOAT_RESULT result = BOAT_SUCCESS;

	// 0- check input parameter
	if ((config == NULL) || (config->prikey_content.field_ptr == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "input parameter can not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// 1- update private key
	if (config->prikey_content.field_len > sizeof(pkCtx->extraData.value))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Error: length of injection key is too long.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}

	memcpy(pkCtx->extraData.value, config->prikey_content.field_ptr, config->prikey_content.field_len);
	pkCtx->extraData.value_len = config->prikey_content.field_len;

	// 2- update private key format
	pkCtx->prikeyCtx.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;

	// 3- update private key type
	pkCtx->prikeyCtx.prikey_type = config->prikey_type;

	// 4- update private key index

	// 5- update public key
	result = BoAT_getPubkey(config->prikey_type, config->prikey_format, pkCtx->extraData.value, pkCtx->extraData.value_len, pkCtx->prikeyCtx.pubkey_content, &len);

	return result;
}

static BOAT_RESULT sBoatPort_keyCreate_external_injection_pkcs(const BoatKeypairPriKeyCtx_config *config,
															   BoatKeypairDataCtx *pkCtx)
{
	// BUINT8       pubKey65[65] = {0};
	BOAT_RESULT result = BOAT_SUCCESS;
	KeypairNative keypair;

	// 0- check input parameter
	if ((config == NULL) || (config->prikey_content.field_ptr == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "input parameter can not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// 1- update private key
	if (config->prikey_content.field_len > sizeof(pkCtx->extraData.value))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Error: length of injection key is too long.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}

	result = UtilityPKCS2Native((BCHAR *)(config->prikey_content.field_ptr), &keypair);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> UtilityPKCS2Native err.");
		UtilityFreeKeypair(keypair);
		return result;
	}

	// memcpy(pkCtx->extra_data.value, config->prikey_content.field_ptr, config->prikey_content.field_len);
	// pkCtx->extra_data.value_len = config->prikey_content.field_len;
	memcpy(pkCtx->extraData.value, keypair.prikey, keypair.prikeylen);
	pkCtx->extraData.value_len = keypair.prikeylen;

	// 2- update private key format
	pkCtx->prikeyCtx.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;

	// 3- update private key type
	pkCtx->prikeyCtx.prikey_type = config->prikey_type;

	// 4- update private key index
	// This field should update by 'key secure storage'(such as TE/SE).
	// When algorithms are implemented by software, this field is default to 0, means
	// that ignore this field.

	// 5- update public key

	// if (config->prikey_type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
	// {
	// 	ecdsa_get_public_key65(&secp256k1, pkCtx->extra_data.value, pubKey65);
	// 	memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
	// }
	// else if (config->prikey_type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
	// {
	// 	ecdsa_get_public_key65(&nist256p1, pkCtx->extra_data.value, pubKey65);
	// 	memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
	// }
	// else
	// {
	// 	BoatLog( BOAT_LOG_CRITICAL, "Invalid private key type." );
	// 	result = BOAT_ERROR_WALLET_KEY_TYPE_ERR;
	// }
	memcpy(pkCtx->prikeyCtx.pubkey_content, keypair.pubkey, keypair.pubkeylen);
	UtilityFreeKeypair(keypair);
	return result;
}

BOAT_RESULT BoatPort_keyCreate(const BoatKeypairPriKeyCtx_config *config, BoatKeypairDataCtx *pkCtx)
{
	BOAT_RESULT result = BOAT_SUCCESS;

	if ((config == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	BoatLog(BOAT_LOG_NORMAL, "prikey_genMode = %d .", config->prikey_genMode);
	if (config->prikey_genMode == BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION)
	{
		BoatLog(BOAT_LOG_VERBOSE, "The private key is generated internally...");
		result = sBoatPort_keyCreate_internal_generation(config, pkCtx);
	}
	else if (config->prikey_genMode == BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION)
	{
		BoatLog(BOAT_LOG_NORMAL, "prikey_format = %d .", config->prikey_format);
		switch (config->prikey_format)
		{
		case BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS:
			BoatLog(BOAT_LOG_NORMAL, "keypair private key[pkcs] set...");
			// result = BOAT_ERROR_kEYPAIR_KEY_FORMAT_ERR;
			result = sBoatPort_keyCreate_external_injection_pkcs(config, pkCtx);
			break;
		case BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE:
			BoatLog(BOAT_LOG_VERBOSE, "keypair private key[native] set...");
			result = sBoatPort_keyCreate_external_injection_native(config, pkCtx);
			break;
		case BOAT_KEYPAIR_PRIKEY_FORMAT_MNEMONIC:
			BoatLog(BOAT_LOG_NORMAL, "NOT SUPPORT FORMAT YET.");
			result = BOAT_ERROR_KEYPAIR_KEY_FORMAT_ERR;
			break;
		default:
			BoatLog(BOAT_LOG_CRITICAL, "Invalid private key format.");
			result = BOAT_ERROR_KEYPAIR_KEY_FORMAT_ERR;
			break;
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Invalid private key format.");
		result = BOAT_ERROR_KEYPAIR_KEY_GENMODE_ERR;
	}

	return result;
}


BOAT_RESULT BoatPort_keyQuery(const BoatWalletPriKeyCtx_config *config, BoatWalletPriKeyCtx *pkCtx)
{
	//! @todo
	return BOAT_ERROR;
}

BOAT_RESULT BoatPort_keyDelete(BoatWalletPriKeyCtx *pkCtx)
{
	//! @todo
	return BOAT_ERROR;
}


/******************************************************************************
                              BOAT AES WARPPER
*******************************************************************************/
BOAT_RESULT BoatAesEncrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
	aes_encrypt_ctx ctxe;
	BUINT8 saltArrayTmp[16];
	BOAT_RESULT result = BOAT_SUCCESS;

	/* aes init */
	result += aes_encrypt_key128(key, &ctxe);

	/* use saltArrayTmp because function aes_cbc_encrypt(...) will modify this field */
	memcpy(saltArrayTmp, iv, 16);

	/* encrypt process */
	result += aes_cbc_encrypt(input, output, length, saltArrayTmp, &ctxe);

	return result;
}

BOAT_RESULT BoatAesDecrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
	aes_decrypt_ctx ctxd;
	BOAT_RESULT result = BOAT_SUCCESS;

	/* aes init */
	result += aes_decrypt_key128(key, &ctxd);

	/* decrypt process */
	result += aes_cbc_decrypt(input, output, length, iv, &ctxd);

	return result;
}
