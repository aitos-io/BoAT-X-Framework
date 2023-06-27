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

/* net releated include */
#if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_HWBCS == 1 || PROTOCOL_USE_CHAINMAKER_V1 == 1 || PROTOCOL_USE_CHAINMAKER_V2 == 1)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
// #include "http2intf.h"

#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>

#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/x509v3.h>

static X509 *buffer2x509(const uint8_t *cert, size_t len)
{
	/*read the cert and decode it*/
	BIO *b = BIO_new_mem_buf((void *)cert, len);
	if (NULL == b)
	{
		BoatLog(BOAT_LOG_NORMAL, "read cert data fail");
		return NULL;
	}
	X509 *x509 = PEM_read_bio_X509(b, NULL, NULL, NULL);
	if (NULL == x509)
	{
		BoatLog(BOAT_LOG_NORMAL, "PEM_read_bio_X509 fail");
		BIO_free(b);
		return NULL;
	}
	BIO_free(b);
	return x509;
}

static EVP_PKEY *buffer2evpkey(const uint8_t *key, size_t key_len)
{
	BIO *b = BIO_new_mem_buf((void *)key, key_len);
	if (NULL == b)
	{
		return NULL;
	}

	// see openssl's PEM_read_bio_PrivateKey interface
	EVP_PKEY *evpkey = PEM_read_bio_PrivateKey(b, NULL, NULL, NULL);
	if (NULL == evpkey)
	{
		BIO_free(b);
		return NULL;
	}
	BIO_free(b);
	return evpkey;
}

BOAT_RESULT boat_find_subject_common_name(const BCHAR *cert, const BUINT32 certlen, BCHAR *value, size_t value_length)
{
	BOAT_RESULT retval = BOAT_SUCCESS;
	X509 *x509Cert = NULL;
	X509_NAME *subject = NULL;
	// retval = boat_get_x509cert(cert, certlen, &x509Cert);
	x509Cert = buffer2x509(cert, certlen);
	if (x509Cert == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "read x509cert fail");
		return BOAT_ERROR;
	}
	// Version = X509_get_version(x509Cert);
	// BoatLog(BOAT_LOG_NORMAL, "X509 Version:%d\n", Version);
	subject = X509_get_subject_name(x509Cert);
	retval = X509_NAME_get_text_by_NID(subject, NID_commonName, value, value_length);
	X509_free(x509Cert);

	return retval;
}

#endif

uint32_t random32(void)
{
	static uint32_t seed = 0;
	if (seed == 0)
	{
		seed = time(NULL);
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
	FILE *file_ptr;

	(void)rsvd;

	if (size == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'size' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	file_ptr = fopen(BOAT_FILE_STOREDATA, "rb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	fseek(file_ptr, 0, SEEK_END);
	*size = ftell(file_ptr);
	fclose(file_ptr);

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
	FILE *file_ptr;
	BSINT32 count = 0;
	BUINT32 size = 0;
	BUINT8 *buf_zero = NULL;

	(void)rsvd;

	if (writeBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	file_ptr = fopen(BOAT_FILE_STOREDATA, "rb+");
	if (file_ptr == NULL)
	{
		file_ptr = fopen(BOAT_FILE_STOREDATA, "wb");
		fclose(file_ptr);
		file_ptr = fopen(BOAT_FILE_STOREDATA, "rb+");
	}
	/* write to file-system */
	// file_ptr = fopen(fileName, "wb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to create file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	fseek(file_ptr, 0, SEEK_END);
	size = ftell(file_ptr);
	BoatLog(BOAT_LOG_NORMAL, "size = %d , offset = %d ", size, offset);
	if (size < offset)
	{
		buf_zero = BoatMalloc(offset - size);
		if (NULL == buf_zero)
		{
			fclose(file_ptr);
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		memset(buf_zero, 0x00, offset - size);
		count = fwrite(buf_zero, 1, offset - size, file_ptr);
		BoatFree(buf_zero);
		if (count != (offset - size))
		{
			fclose(file_ptr);
			BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
			return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
		}
		fseek(file_ptr, 0, SEEK_END);
	}
	else
	{
		fseek(file_ptr, offset, SEEK_SET);
	}
	count = fwrite(writeBuf, 1, writeLen, file_ptr);
	fclose(file_ptr);
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
	FILE *file_ptr;
	BSINT32 count = 0;
	BUINT32 size = 0;
	(void)rsvd;

	if (readBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* read from file-system */
	file_ptr = fopen(BOAT_FILE_STOREDATA, "rb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	fseek(file_ptr, 0, SEEK_END);
	size = ftell(file_ptr);
	if (readLen + offset > size)
	{
		fclose(file_ptr);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	fseek(file_ptr, offset, SEEK_SET);
	count = fread(readBuf, 1, readLen, file_ptr);
	fclose(file_ptr);
	if (count != readLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}

	return BOAT_SUCCESS;
}

/******************************************************************************
							  BOAT SOCKET WARPPER
							THIS ONLY USED BY FABRIC/HWBCS/CHAINMAKER
*******************************************************************************/
#if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_HWBCS == 1 || PROTOCOL_USE_CHAINMAKER_V1 == 1 || PROTOCOL_USE_CHAINMAKER_V2 == 1)
BSINT32 BoatConnect(const BCHAR *address, void *rsvd)
{
	int connectfd;
	char ip[64];
	char port[8];
	char *ptr = NULL;
	struct hostent *he;
	struct sockaddr_in server;
	struct sockaddr localaddr;
	struct sockaddr_in *localaddr_ptr;
	socklen_t addrlen = sizeof(struct sockaddr);

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

	if (connect(connectfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) < 0)
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
		localaddr_ptr = (struct sockaddr_in *)&localaddr;
		BoatLog(BOAT_LOG_VERBOSE, "localIP: %s:%d.",
				inet_ntoa(localaddr_ptr->sin_addr), htons(localaddr_ptr->sin_port));
	}

	BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, connectfd);

	return connectfd;
}

#if (BOAT_TLS_SUPPORT == 1)
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
						const BoatFieldVariable clientCert, BSINT32 *socketfd, void **tlsContext, void *rsvd)
{

	//! @todo BoatTlsInit implementation in crypto default.
	// return BOAT_ERROR;
	BOAT_RESULT retval = BOAT_SUCCESS;
	SSL_CTX *ctx;
	X509 *cert = NULL;
	X509 *clientTlsCert = NULL;
	EVP_PKEY *clientTlsprikey = NULL;
	retval = BoatConnect(address, NULL);
	if (retval < BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
		return BOAT_ERROR;
	}
	*socketfd = retval;
	/* SSL library init, ssl-server.c  */
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "creat SSL CTX fail ");
		return BOAT_ERROR;
	}
	// client and server verification
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
	cert = buffer2x509(caChain.field_ptr, caChain.field_len);
	if (cert == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, " get x509 cert fail");
		return BOAT_ERROR;
	}
	X509_STORE *certS = SSL_CTX_get_cert_store(ctx);
	X509_STORE_add_cert(certS, cert);
	X509_free(cert);
	if (BOAT_TLS_IDENTIFY_CLIENT == 1)
	{
		/* load client tls cert ,contain public key */
		clientTlsCert = buffer2x509(clientCert.field_ptr, clientCert.field_len);
		if (clientTlsCert == NULL)
		{
			BoatLog(BOAT_LOG_NORMAL, "read client tls cert fail ");
			SSL_CTX_free(ctx);
			return BOAT_ERROR;
		}
		if (SSL_CTX_use_certificate(ctx, clientTlsCert) <= 0)
		{
			BoatLog(BOAT_LOG_NORMAL, "load client tls cert fail ");
			SSL_CTX_free(ctx);
			X509_free(clientTlsCert);
			return BOAT_ERROR;
		}
		X509_free(clientTlsCert);

		clientTlsprikey = buffer2evpkey(clientPrikey.field_ptr, clientPrikey.field_len);
		if (clientTlsprikey == NULL)
		{
			BoatLog(BOAT_LOG_NORMAL, "read client tls key fail ");
			SSL_CTX_free(ctx);
			return BOAT_ERROR;
		}
		/* load client prikey */
		if (SSL_CTX_use_PrivateKey(ctx, clientTlsprikey) <= 0)
		{
			BoatLog(BOAT_LOG_NORMAL, "load client tls key fail ");
			EVP_PKEY_free(clientTlsprikey);
			SSL_CTX_free(ctx);
			return BOAT_ERROR;
		}
		EVP_PKEY_free(clientTlsprikey);
		/* check client's private key */
		if (!SSL_CTX_check_private_key(ctx))
		{
			BoatLog(BOAT_LOG_NORMAL, "creat SSL CTX fail ");
			SSL_CTX_free(ctx);
			return BOAT_ERROR;
		}
	}
	/* creat new SSL by ctx  */
	*tlsContext = SSL_new(ctx);
	SSL_set_fd(*tlsContext, *socketfd);
	/* creat connection of SSL */
	if (SSL_connect(*tlsContext) == -1)
	{
		BoatLog(BOAT_LOG_NORMAL, "SSL_connect fail ");
		SSL_CTX_free(ctx);
		return BOAT_ERROR;
	}
	SSL_CTX_free(ctx);
	return BOAT_SUCCESS;
}
#endif

BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
	//! @todo BOAT_TLS_SUPPORT implementation in crypto default.
	if (tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext  can't be NULL.");
		return -1;
	}
	return SSL_write((SSL *)tlsContext, buf, len);
#else
	return send(sockfd, buf, len, 0);
#endif
}

BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
	//! @todo BOAT_TLS_SUPPORT implementation in crypto default.
	if (tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext  can't be NULL.");
		return -1;
	}
	return SSL_read((SSL *)tlsContext, buf, len);
#else
	return recv(sockfd, buf, len, 0);
#endif
}

void BoatClose(BSINT32 sockfd, void **tlsContext, void *rsvd)
{
	close(sockfd);
#if (BOAT_TLS_SUPPORT == 1)
	// free tls releated
	//! @todo BOAT_TLS_SUPPORT implementation in crypto default.
	SSL_shutdown((SSL *)*tlsContext);
	SSL_free((SSL *)*tlsContext);
	*tlsContext = NULL;
#endif
}
#endif /* ##if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_HWBCS == 1 || PROTOCOL_USE_CHAINMAKER_V1 == 1 || PROTOCOL_USE_CHAINMAKER_V2 == 1)   \
																																			 \ \
/******************************************************************************                                                                \
							  BOAT KEY PROCESS WARPPER                                                                                         \
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
