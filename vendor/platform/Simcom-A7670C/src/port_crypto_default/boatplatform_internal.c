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

@file platform.c contains platform interface functions.
*/

//! self header include
#include "boatiotsdk.h"
#include "boatplatform_internal.h"

#include "boattypes.h"

#include "rand.h"
#include "aes/aes.h"
#include "sha3.h"
#include "secp256k1.h"
#include "nist256p1.h"
#include "bignum.h"
#include "persiststore.h"
#include "boatkeystore.h"
#include "simcom_os.h"
#include "simcom_file.h"

/* net releated include */
#if (PROTOCOL_USE_HLFABRIC == 1)
#include <sys/types.h>
#include <string.h>
#include "http2intf.h"
#include "simcom_tcpip_old.h"
#include "simcom_ssl.h"
#endif

// #if (PROTOCOL_USE_HLFABRIC == 1)
// // for TTLSContext structure
// #include "http2intf.h"
// #endif

uint32_t random32(void)
{
	static uint32_t seed = 0;
	if (seed == 0)
	{
		seed = sAPI_Time(NULL);
	}
	// Linear congruential generator from Numerical Recipes
	// https://en.wikipedia.org/wiki/Linear_congruential_generator
	seed = 1664525 * seed + 1013904223;

	return seed;
}

BOAT_RESULT BoatRandom(BUINT8 *output, BUINT32 outputLen, void *rsvd)
{
	/* param check */
	if (output == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	(void)rsvd;

	random_buffer(output, outputLen);

	return BOAT_SUCCESS;
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

BOAT_RESULT BoatGetStorageSize(BUINT32 *size, void *rsvd)
{
	SCFILE *file_hdl = NULL;

	(void)rsvd;

	if (size == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'size' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	file_hdl = sAPI_fopen(BOAT_FILE_STOREDATA, "rb");
	if (file_hdl == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	sAPI_fseek(file_hdl, 0, FS_SEEK_END);
	*size = sAPI_ftell(file_hdl);
	sAPI_fclose(file_hdl);

	return BOAT_SUCCESS;
}

/**
 * @description:
 * 	This function wirte data into file.
 * @param[in] {BUINT32} offset
 * 	write data from offset as the starting point of the file
 * @param[in] {BUINT8} *writeBuf
 * 	the data that will be written into file
 * @param {BUINT32} writeLen
 * 	the length of data that will be written
 * @param {void} *rsvd
 * 	reserved , not used now
 * @return {*}
 * 	This function will return BOAT_SUCCESS if write successfully.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatWriteStorage(BUINT32 offset, BUINT8 *writeBuf, BUINT32 writeLen, void *rsvd)
{
	SCFILE *file_hdl = NULL;

	BSINT32 count = 0;
	BUINT32 size = 0;
	BUINT8 *buf_zero = NULL;

	(void)rsvd;

	if (writeBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	file_hdl = sAPI_fopen(BOAT_FILE_STOREDATA, "rb+");
	if (file_hdl == NULL)
	{
		file_hdl = sAPI_fopen(BOAT_FILE_STOREDATA, "wb");
		sAPI_fclose(file_hdl);
		file_hdl = sAPI_fopen(BOAT_FILE_STOREDATA, "rb+");
	}
	/* write to file-system */
	if (file_hdl == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to create file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	sAPI_fseek(file_hdl, 0, FS_SEEK_END);
	size = sAPI_ftell(file_hdl);
	BoatLog(BOAT_LOG_NORMAL, "size = %d , offset = %d ", size, offset);
	if (size < offset)
	{
		buf_zero = BoatMalloc(offset - size);
		if (NULL == buf_zero)
		{
			sAPI_fclose(file_hdl);
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		memset(buf_zero, 0x00, offset - size);
		count = sAPI_fwrite(buf_zero, 1, offset - size, file_hdl);
		BoatFree(buf_zero);
		if (count != (offset - size))
		{
			sAPI_fclose(file_hdl);
			BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
			return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
		}
		sAPI_fseek(file_hdl, 0, FS_SEEK_END);
	}
	else
	{
		sAPI_fseek(file_hdl, offset, FS_SEEK_BEGIN);
	}
	count = sAPI_fwrite(writeBuf, 1, writeLen, file_hdl);
	sAPI_fclose(file_hdl);
	if (count != writeLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
	}

	return BOAT_SUCCESS;
}

/**
 * @description:
 * 	This function read data from file
 * @param {BUINT32} offset
 * 	read data from offset as the starting point of the file
 * @param {BUINT8} *readBuf
 * 	store data reading form the file
 * @param {BUINT32} readLen
 * 	the length of data that want to read
 * @param {void} *rsvd
 * 	reserved , not used now
 * @return {*}
 * 	This function will return BOAT_SUCCESS if read successfully.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatReadStorage(BUINT32 offset, BUINT8 *readBuf, BUINT32 readLen, void *rsvd)
{
	SCFILE *file_hdl = NULL;

	BSINT32 count = 0;
	BUINT32 size = 0;
	(void)rsvd;

	if (readBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	/* read from file-system */
	file_hdl = sAPI_fopen(BOAT_FILE_STOREDATA, "rb");
	if (file_hdl == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	sAPI_fseek(file_hdl, 0, FS_SEEK_END);
	size = sAPI_ftell(file_hdl);
	if (readLen + offset > size)
	{
		sAPI_fclose(file_hdl);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	sAPI_fseek(file_hdl, offset, FS_SEEK_BEGIN);
	count = sAPI_fread(readBuf, 1, readLen, file_hdl);
	sAPI_fclose(file_hdl);
	if (count != readLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}

	return BOAT_SUCCESS;
}

/******************************************************************************
							  BOAT SOCKET WARPPER
							THIS ONLY USED BY FABRIC
*******************************************************************************/
#if (PROTOCOL_USE_HLFABRIC == 1)
BSINT32 BoatConnect(const BCHAR *address, void *rsvd)
{
	INT32 sockfd = -1;
	SChostent *host_entry = NULL;
	SCsockAddrIn server;

	char ip[64];
	char port[8];
	char *ptr = NULL;

	(void)rsvd;

	ptr = strchr(address, ':');
	if (NULL == ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "invalid address:%s.", address);
		return -1;
	}

	memset(ip, 0, sizeof(ip));
	memset(port, 0, sizeof(port));
	memcpy(ip, address, (int)(ptr - address));
	memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));

	// if (-1 == sAPI_TcpipPdpActive(1, 1))
	// {
	// 	BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpActive() error");
	// 	return -1;
	// }

	sockfd = sAPI_TcpipSocket(SC_AF_INET, SC_SOCK_STREAM, SC_IPPROTO_TCP);
	BoatLog(BOAT_LOG_CRITICAL, "BoatConnect sockfd[%d]", sockfd);
	if (sockfd < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipSocket() error");
		// if (-1 == sAPI_TcpipPdpDeactive(1, 1))
		// {
		// 	BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpDeactive() error");
		// }
		return -1;
	}

	host_entry = sAPI_TcpipGethostbyname(ip);
	// host_entry = sAPI_TcpipGethostbyname((INT8 *)"www.baidu.com");
	if (host_entry == NULL)
	{
		sAPI_SslClose(0);
		sAPI_TcpipClose(sockfd);
		BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipGethostbyname() error");
		// if (-1 == sAPI_TcpipPdpDeactive(1, 1))
		// {
		// 	BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpDeactive() error");
		// }
		return -1;
	}

	server.sin_family = SC_AF_INET;
	server.sin_port = sAPI_TcpipHtons(atoi(port));
	// server.sin_port = sAPI_TcpipHtons(443);
	server.sin_addr.s_addr = *(UINT32 *)host_entry->h_addr_list[0];

	BoatLog(BOAT_LOG_CRITICAL, "start connect!!!");
	if (sAPI_TcpipConnect(sockfd, (SCsockAddr *)&server, sizeof(SCsockAddr)) != 0)
	{
		sAPI_SslClose(0);
		sAPI_TcpipClose(sockfd);
		BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipConnect() error");
		// if (-1 == sAPI_TcpipPdpDeactive(1, 1))
		// {
		// 	BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpDeactive() error");
		// }
		return -1;
	}

	return sockfd;
}

#if (BOAT_TLS_SUPPORT == 1)
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
						const BoatFieldVariable clientCert, BSINT32 *socketfd, void **tlsContext, void *rsvd)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BSINT32 sockfd;
	// SCSslCtx_t *tlsContext_ptr = BoatMalloc(sizeof(SCSslCtx_t));
	boat_try_declare;

	sockfd = BoatConnect(address, NULL);
	if (sockfd < BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
		boat_throw(sockfd, BoatTlsInit_exception);
	}
	else
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect success!!! ");
	}
	BoatLog(BOAT_LOG_NORMAL, "boat hostName = %s ", hostName);
	// memset(tlsContext_ptr,0,sizeof(tlsContext_ptr));
	SCSslCtx_t sslCtx;

	memset(&sslCtx, 0, sizeof(SCSslCtx_t));
	sslCtx.ClientId = 0;
	sslCtx.fd = sockfd;
	sslCtx.ciphersuitesetflg = 0;
	// sslCtx.ciphersuite[0] = 0x2C;
	sslCtx.ssl_version = SC_SSL_CFG_VERSION_TLS12;
	// sslCtx.enable_SNI = 0;
	sslCtx.enable_SNI = 1;
	sslCtx.ipstr = (INT8 *)hostName;
	sslCtx.root_ca = (INT8 *)caChain.field_ptr;
	sslCtx.root_ca_len = caChain.field_len;
	sslCtx.ignore_local_time = 1;
	// sslCtx.auth_mode = SC_SSL_CFG_VERIFY_MODE_REQUIRED;
	// sslCtx.auth_mode = SC_SSL_CFG_VERIFY_MODE_CLIENT;
	sslCtx.auth_mode = SC_SSL_CFG_VERIFY_MODE_REQUIRED;
#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
	sslCtx.auth_mode = SC_SSL_CFG_VERIFY_MODE_SERVER_CLIENT;
	sslCtx.client_cert = (INT8 *)clientCert.field_ptr;
	sslCtx.client_cert_len = clientCert.field_len;
	sslCtx.client_key = (INT8 *)clientPrikey.field_ptr;
	sslCtx.client_key_len = clientPrikey.field_len;
#endif

	result = sAPI_SslHandShake(&sslCtx);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "sAPI_SslHandShake fail[%d]", result);
		boat_throw(result, BoatTlsInit_exception);
	}
	else
	{
		BoatLog(BOAT_LOG_NORMAL, "sAPI_SslHandShake success!!!");
	}

	boat_catch(BoatTlsInit_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		sAPI_SslClose(0);
		sAPI_TcpipClose(sockfd);
		if (-1 == sAPI_TcpipPdpDeactive(1, 1))
		{
			BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpDeactive() error");
		}
		// BoatFree(tlsContext_ptr);
		// tlsContext_ptr = NULL;
	}
	*tlsContext = &sslCtx;
	return result;
}
#endif

BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
	if ((SCSslCtx_t *)tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext can't be NULL.");
		return -1;
	}

	return sAPI_SslSend(0, buf, len);
#else
	return send(sockfd, buf, len, 0);
#endif
}

BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
	if ((SCSslCtx_t *)tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext can't be NULL.");
		return -1;
	}

	return sAPI_SslRead(0, buf, len);
#else
	return recv(sockfd, buf, len, 0);
#endif
}

void BoatClose(BSINT32 sockfd, void **tlsContext, void *rsvd)
{
	sAPI_TcpipClose(sockfd);
#if (BOAT_TLS_SUPPORT == 1)
	sAPI_SslClose(0);
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

BOAT_RESULT BoatPort_keyQuery(const BoatKeypairPriKeyCtx_config *config, BoatKeypairPriKeyCtx *pkCtx)
{
	//! @todo
	return BOAT_ERROR;
}

BOAT_RESULT BoatPort_keyDelete(BoatKeypairPriKeyCtx *pkCtx)
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
