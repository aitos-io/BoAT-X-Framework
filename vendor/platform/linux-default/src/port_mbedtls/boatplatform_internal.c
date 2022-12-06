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
#include "boatplatform_internal.h"
#include "boatiotsdk.h"
#include "boattypes.h"
#include "boatkeypair.h"

#include "keccak.h"
#include "persiststore.h"
#include "boatkeystore.h"

/* mbedTLS header include */
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/asn1.h"

#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/bignum.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/error.h"

/* net releated include */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#include <sys/time.h>

#if ((PROTOCOL_USE_HLFABRIC == 1) || (PROTOCOL_USE_CHAINMAKER_V1 == 1) || (PROTOCOL_USE_CHAINMAKER_V2 == 1) || (PROTOCOL_USE_HWBCS == 1))
// for TTLSContext structure
#include "http2intf.h"
#endif

BOAT_RESULT BoatRandom(BUINT8 *output, BUINT32 outputLen, void *rsvd)
{
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_entropy_context entropy;

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);
	result = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
	if (result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec mbedtls_ctr_drbg_seed.");
		boat_throw(result, hlfabricGenNonce_exception);
	}

	result = mbedtls_ctr_drbg_random(&ctr_drbg, output, outputLen);
	if (result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec mbedtls_ctr_drbg_random(nonce).");
		boat_throw(result, hlfabricGenNonce_exception);
	}

	/* boat catch handle */
	boat_catch(hlfabricGenNonce_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}

	/* free */
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

	return result;
}

BOAT_RESULT BoatSignature(BoatKeypairPriKeyCtx prikeyCtx,
						  const BUINT8 *digest, BUINT32 digestLen,
						  BoatSignatureResult *signatureResult, void *rsvd)
{
	BUINT8 signature[64] = {0};
	BUINT8 signatureTmp[139];
	BUINT32 signaturelen = 0;
	BUINT32 signatureTmpLen = 0;

	BUINT8 ecdsPrefix = 0;

	BOAT_RESULT result;

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
	// result = BoAT_sign(prikeyCtx.prikey_type,prikeyCtx.prikey_format,prikey.value,prikey.value_len,digest,digestLen,signature,&signaturelen,&ecdsPrefix);
	result = BoAT_Keystore_Sign(prikeyCtx.prikey_type, prikeyCtx.keypair_index, digest, digestLen, signature, &signaturelen, &ecdsPrefix);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "gen signature fail.");
		return BOAT_ERROR;
	}
	/* convert r,s to asn.1 */
	result = utility_signature_to_asn1(signature, signaturelen, signatureTmp, &signatureTmpLen);
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

BOAT_RESULT BoatGetFileSize(BUINT32 *size, void *rsvd)
{
	FILE *file_ptr;

	(void)rsvd;

	if (size == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'size' can't be NULL.");
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
		BoatLog(BOAT_LOG_CRITICAL, "param which 'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* write to file-system */
	// file_ptr = fopen(fileName, "wb");
	file_ptr = fopen(BOAT_FILE_STOREDATA, "rb+");
	if (file_ptr == NULL)
	{
		file_ptr = fopen(BOAT_FILE_STOREDATA, "wb");
		fclose(file_ptr);
		file_ptr = fopen(BOAT_FILE_STOREDATA, "rb+");
	}
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to create file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	fseek(file_ptr, 0, SEEK_END);
	size = ftell(file_ptr);
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
		BoatLog(BOAT_LOG_CRITICAL, "param which  'readBuf' can't be NULL.");
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

#if ((PROTOCOL_USE_HLFABRIC == 1) || (PROTOCOL_USE_CHAINMAKER_V1 == 1) || (PROTOCOL_USE_CHAINMAKER_V2 == 1) || (PROTOCOL_USE_HWBCS == 1))

BSINT32 BoatConnect(const BCHAR *address, void *rsvd)
{
	int connectfd = -1;
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
	if (0 != getsockname(connectfd, &localaddr, &addrlen))
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
BOAT_RESULT BoatTlsInit(const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
						const BoatFieldVariable clientCert, BSINT32 socketfd, void *tlsContext, void *rsvd)
{
	TTLSContext *tlsContext_ptr = (TTLSContext *)tlsContext;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
#if (BOAT_TLS_SUPPORT == 1) && (BOAT_TLS_IDENTIFY_CLIENT == 1)
	mbedtls_x509_crt clientTlscert;
	mbedtls_pk_context clientTlspkey;
#endif

	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	tlsContext_ptr->ssl = BoatMalloc(sizeof(mbedtls_ssl_context));
	if (tlsContext_ptr->ssl == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate ssl_context.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	tlsContext_ptr->ssl_cfg = BoatMalloc(sizeof(mbedtls_ssl_config));
	if (tlsContext_ptr->ssl_cfg == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate ssl_config.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	tlsContext_ptr->ssl_crt = BoatMalloc(sizeof(mbedtls_x509_crt));
	if (tlsContext_ptr->ssl_crt == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate x509_crt.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	tlsContext_ptr->ssl_net = BoatMalloc(sizeof(mbedtls_net_context));
	if (tlsContext_ptr->ssl_net == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate net_context.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}

	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_net_init(tlsContext_ptr->ssl_net);
	mbedtls_ssl_init(tlsContext_ptr->ssl);
	mbedtls_ssl_config_init(tlsContext_ptr->ssl_cfg);
	mbedtls_x509_crt_init(tlsContext_ptr->ssl_crt);
#if (BOAT_TLS_SUPPORT == 1) && (BOAT_TLS_IDENTIFY_CLIENT == 1)
	mbedtls_x509_crt_init(&clientTlscert);
	mbedtls_pk_init(&clientTlspkey);
#endif

	result = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ctr_drbg_seed.");
		boat_throw(result, BoatTlsInit_exception);
	}

	result = mbedtls_ssl_config_defaults(tlsContext_ptr->ssl_cfg, MBEDTLS_SSL_IS_CLIENT,
										 MBEDTLS_SSL_TRANSPORT_STREAM,
										 MBEDTLS_SSL_PRESET_DEFAULT);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_config_defaults.\n");
		boat_throw(result, BoatTlsInit_exception);
	}

	mbedtls_ssl_conf_rng(tlsContext_ptr->ssl_cfg, mbedtls_ctr_drbg_random, &ctr_drbg);
	result += mbedtls_x509_crt_parse(tlsContext_ptr->ssl_crt, caChain.field_ptr, caChain.field_len);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to execute x509_crt_parse: -%x\n", -result);
		boat_throw(result, BoatTlsInit_exception);
	}

	mbedtls_ssl_conf_ca_chain(tlsContext_ptr->ssl_cfg, tlsContext_ptr->ssl_crt, NULL);
#if (BOAT_TLS_SUPPORT == 1) && (BOAT_TLS_IDENTIFY_CLIENT == 1)
	result = mbedtls_x509_crt_parse(&clientTlscert, (const unsigned char *)clientCert.field_ptr, clientCert.field_len);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, " failed\n  !  mbedtls_x509_crt_parse returned %x\n\n", -result);
		boat_throw(result, BoatTlsInit_exception);
	}

	result = mbedtls_pk_parse_key(&clientTlspkey, (const unsigned char *)clientPrikey.field_ptr,
								  clientPrikey.field_len, NULL, 0);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, " failed\n  !  mbedtls_pk_parse_key returned %x\n\n", -result);
		boat_throw(result, BoatTlsInit_exception);
	}
	if ((result = mbedtls_ssl_conf_own_cert(tlsContext_ptr->ssl_cfg, &clientTlscert, &clientTlspkey)) != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, " failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", result);
		boat_throw(result, BoatTlsInit_exception);
	}
#endif

	mbedtls_ssl_conf_authmode(tlsContext_ptr->ssl_cfg, MBEDTLS_SSL_VERIFY_REQUIRED);
	result = mbedtls_ssl_setup(tlsContext_ptr->ssl, tlsContext_ptr->ssl_cfg);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_setup.\n");
		boat_throw(result, BoatTlsInit_exception);
	}

	result = mbedtls_ssl_set_hostname(tlsContext_ptr->ssl, hostName);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to execute hostname_set.\n");
		boat_throw(result, BoatTlsInit_exception);
	}

	((mbedtls_net_context *)(tlsContext_ptr->ssl_net))->fd = socketfd;
	mbedtls_ssl_set_bio(tlsContext_ptr->ssl, tlsContext_ptr->ssl_net, mbedtls_net_send, mbedtls_net_recv, NULL);
	result = mbedtls_ssl_handshake(tlsContext_ptr->ssl);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_handshake: -%x\n", -result);
		boat_throw(result, BoatTlsInit_exception);
	}

	/* In real life, we would have used MBEDTLS_SSL_VERIFY_REQUIRED so that the
	 * handshake would not succeed if the peer's cert is bad.  Even if we used
	 * MBEDTLS_SSL_VERIFY_OPTIONAL, we would bail out here if ret != 0 */
	if ((result = mbedtls_ssl_get_verify_result(tlsContext_ptr->ssl)) != 0)
	{
		char vrfy_buf[512];

		BoatLog(BOAT_LOG_CRITICAL, " failed\n");

		mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", result);

		BoatLog(BOAT_LOG_CRITICAL, "%s\n", vrfy_buf);
	}
	else
		BoatLog(BOAT_LOG_CRITICAL, " ok\n");

	BoatLog(BOAT_LOG_NORMAL, "ret = ssl_handshake SUCCESSED!");

	/* boat catch handle */
	boat_catch(BoatTlsInit_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		mbedtls_ssl_free(tlsContext_ptr->ssl);
		mbedtls_net_free(tlsContext_ptr->ssl_net);
		mbedtls_ssl_config_free(tlsContext_ptr->ssl_cfg);
		mbedtls_x509_crt_free(tlsContext_ptr->ssl_crt);
		BoatFree(tlsContext_ptr->ssl);
		BoatFree(tlsContext_ptr->ssl_net);
		BoatFree(tlsContext_ptr->ssl_cfg);
		BoatFree(tlsContext_ptr->ssl_crt);
	}

	mbedtls_entropy_free(&entropy);
	mbedtls_ctr_drbg_free(&ctr_drbg);

	return result;
}
#endif

BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
	if ((tlsContext == NULL) || (((TTLSContext *)tlsContext)->ssl == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext or tlsContext->ssl can't be NULL.");
		return -1;
	}
	return mbedtls_ssl_write(((TTLSContext *)tlsContext)->ssl, buf, len);
#else
	return send(sockfd, buf, len, 0);
#endif
}

BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
	if ((tlsContext == NULL) || (((TTLSContext *)tlsContext)->ssl == NULL))
	{
		//! @todo still receive a unknown data after Boatclose(...)
		return -1;
	}
	return mbedtls_ssl_read(((TTLSContext *)tlsContext)->ssl, buf, len);
#else
	return recv(sockfd, buf, len, 0);
#endif
}

void BoatClose(BSINT32 sockfd, void *tlsContext, void *rsvd)
{
	close(sockfd);
#if (BOAT_TLS_SUPPORT == 1)
	// free tls releated
	if (tlsContext != NULL)
	{
		mbedtls_ssl_free(((TTLSContext *)tlsContext)->ssl);
		BoatFree(((TTLSContext *)tlsContext)->ssl);
		((TTLSContext *)tlsContext)->ssl = NULL;

		mbedtls_ssl_config_free(((TTLSContext *)tlsContext)->ssl_cfg);
		BoatFree(((TTLSContext *)tlsContext)->ssl_cfg);
		((TTLSContext *)tlsContext)->ssl_cfg = NULL;

		mbedtls_net_free(((TTLSContext *)tlsContext)->ssl_net);
		BoatFree(((TTLSContext *)tlsContext)->ssl_net);
		((TTLSContext *)tlsContext)->ssl_net = NULL;

		mbedtls_x509_crt_free(((TTLSContext *)tlsContext)->ssl_crt);
		BoatFree(((TTLSContext *)tlsContext)->ssl_crt);
		((TTLSContext *)tlsContext)->ssl_crt = NULL;
	}
#endif
}

#endif

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

static BOAT_RESULT sBoatPort_keyCreate_external_injection_pkcs(const BoatKeypairPriKeyCtx_config *config,
															   BoatKeypairDataCtx *pkCtx)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BUINT32 pubkeylen = 0;
	mbedtls_pk_context mbedtls_pkCtx;
	boat_try_declare;
	// 0- check input parameter
	if ((config == NULL) || (config->prikey_content.field_ptr == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "input parameter can not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	if (config->prikey_format == BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE)
	{
		memcpy(pkCtx->extraData.value, config->prikey_content.field_ptr, config->prikey_content.field_len);
		pkCtx->extraData.value_len = config->prikey_content.field_len;
	}
	else if (config->prikey_format == BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS)
	{
		mbedtls_pk_init(&mbedtls_pkCtx);
		result = mbedtls_pk_parse_key(&mbedtls_pkCtx, config->prikey_content.field_ptr,
									  config->prikey_content.field_len, NULL, 0);
		if (result != BOAT_SUCCESS)
		{
			mbedtls_pk_free(&mbedtls_pkCtx);
			BoatLog(BOAT_LOG_CRITICAL, "Error: pkcs key parse failed.");
			boat_throw(BOAT_ERROR, BoatGetkeypair_exception);
		}
		// 1- update private key
		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->d, pkCtx->extraData.value, 32);
		pkCtx->extraData.value_len = (mbedtls_pk_ec(mbedtls_pkCtx)->d.n) * (sizeof(mbedtls_mpi_uint));
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Error: format failed.");
		boat_throw(BOAT_ERROR, BoatGetkeypair_exception);
	}

	// 2- update private key format
	pkCtx->prikeyCtx.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;

	// 3- update private key type
	pkCtx->prikeyCtx.prikey_type = config->prikey_type;

	// 4- updata pubkey
	result = BoAT_getPubkey(config->prikey_type, config->prikey_format, config->prikey_content.field_ptr,
							config->prikey_content.field_len, pkCtx->prikeyCtx.pubkey_content, &pubkeylen);
	/* boat catch handle */
	boat_catch(BoatGetkeypair_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
	}
	if (config->prikey_format == BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS)
	{
		mbedtls_pk_free(&mbedtls_pkCtx);
	}

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
	// 1- update private key
	if (config->prikey_content.field_len > sizeof(pkCtx->extraData.value))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Error: length of injection key is too long.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	if (config->prikey_genMode == BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION)
	{
		BoatLog(BOAT_LOG_VERBOSE, "The private key is generated internally...");
		result = sBoatPort_keyCreate_internal_generation(config, pkCtx);
	}
	else if (config->prikey_genMode == BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION)
	{
		switch (config->prikey_format)
		{
		case BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS:
			BoatLog(BOAT_LOG_VERBOSE, "keypair private key[pkcs] set...");
			result = sBoatPort_keyCreate_external_injection_pkcs(config, pkCtx);
			break;
		case BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE:
		case BOAT_KEYPAIR_PRIKEY_FORMAT_MNEMONIC:
			// BoatLog(BOAT_LOG_NORMAL, "NOT SUPPORT FORMAT YET.");
			// result = BOAT_ERROR;
			BoatLog(BOAT_LOG_VERBOSE, "keypair private key[native] set...");
			result = sBoatPort_keyCreate_external_injection_pkcs(config, pkCtx);
			break;
		default:
			BoatLog(BOAT_LOG_CRITICAL, "invalid private key format.");
			result = BOAT_ERROR_KEYPAIR_KEY_FORMAT_ERR;
			break;
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "invalid private key format.");
		result = BOAT_ERROR_KEYPAIR_KEY_GENMODE_ERR;
	}

	return result;
}

BOAT_RESULT BoatPort_keyQuery(const BoatKeypairPriKeyCtx_config *config, BoatKeypairPriKeyCtx *pkCtx)
{
	//! @todo to be implemented
	return BOAT_ERROR;
}

BOAT_RESULT BoatPort_keyDelete(BoatKeypairPriKeyCtx *pkCtx)
{
	//! @todo to be implemented
	return BOAT_ERROR;
}

/******************************************************************************
							  BOAT AES WARPPER
*******************************************************************************/
BOAT_RESULT BoatAesEncrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
	mbedtls_aes_context ctx;
	BUINT8 saltArrayTmp[16];
	BOAT_RESULT result = BOAT_SUCCESS;

	/* aes init */
	mbedtls_aes_init(&ctx);

	/* set encrypt key */
	result += mbedtls_aes_setkey_enc(&ctx, key, 128);

	/* use saltArrayTmp because function mbedtls_aes_crypt_cbc(...) will modify this field */
	memcpy(saltArrayTmp, iv, 16);

	/* encrypt process */
	result += mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, length, saltArrayTmp, input, output);

	/* aes free */
	mbedtls_aes_free(&ctx);

	return result;
}

BOAT_RESULT BoatAesDecrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
	mbedtls_aes_context ctx;
	BOAT_RESULT result = BOAT_SUCCESS;

	/* aes init */
	mbedtls_aes_init(&ctx);

	/* set encrypt key */
	result += mbedtls_aes_setkey_dec(&ctx, key, 128);

	/* decrypt process */
	result += mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, length, iv, input, output);

	/* aes free */
	mbedtls_aes_free(&ctx);

	return result;
}
