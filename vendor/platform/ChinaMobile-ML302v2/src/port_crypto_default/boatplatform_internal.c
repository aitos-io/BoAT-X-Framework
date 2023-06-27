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

#include "cm_fs.h"
#if (PROTOCOL_USE_HLFABRIC == 1)
#include "cm_ssl.h"

// #include <sys/types.h>
#include "sockets.h"
// #include <netinet/in.h>
#include "netdb.h"
#include "inet.h"
#include <sys/time.h>
#endif

#define GENERATE_KEY_REPEAT_TIMES 100

uint32_t random32(void)
{
	static uint32_t seed = 0;
	if (seed == 0)
	{
		seed = osiEpochSecond();
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
	struct stat st;
	BSINT32 result = 0;

	(void)rsvd;

	if (size == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param 'size' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	result = cm_fs_stat(BOAT_FILE_STOREDATA, &st);
	if (result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to get file status: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	*size = st.st_size;

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
	BSINT32 file;
	BSINT32 count = 0;
	BSINT32 result = 0;
	BUINT8 *buf_zero = NULL;
	BUINT32 size;
	struct stat st;

	(void)rsvd;

	if (writeBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param 'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* write to file-system */
	result = cm_fs_stat(BOAT_FILE_STOREDATA, &st);
	if (result != 0)
	{
		size = 0;
	}
	else
	{
		size = st.st_size;
	}

	file = cm_fs_fopen(BOAT_FILE_STOREDATA, O_WRONLY | O_CREAT);
	if (file < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	if (size < offset)
	{
		buf_zero = BoatMalloc(offset - size);
		if (buf_zero == NULL)
		{
			cm_fs_fclose(file);
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		memset(buf_zero, 0x00, offset - size);
		cm_fs_fseek(file, 0, SEEK_END);
		count = cm_fs_fwrite(file, buf_zero, offset - size);
		BoatFree(buf_zero);
		if (count != (offset - size))
		{
			cm_fs_fclose(file);
			BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
			return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
		}
		cm_fs_fseek(file, 0, SEEK_END);
	}
	else
	{
		cm_fs_fseek(file, offset, SEEK_SET);
	}

	count = cm_fs_fwrite(file, writeBuf, writeLen);
	if (count != writeLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
	}

	result = cm_fs_fclose(file);
	if (result < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to close file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_CLOSE_FAIL;
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
	BSINT32 file;
	BSINT32 count = 0;
	BSINT32 result = 0;

	(void)rsvd;

	if (readBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* read from file-system */
	file = cm_fs_fopen(BOAT_FILE_STOREDATA, O_RDONLY);
	if (file < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	count = cm_fs_fread(file, readBuf, readLen);
	if (count != readLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}

	result = cm_fs_fclose(file);
	if (result < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to close file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_CLOSE_FAIL;
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

	if (0 != cm_fs_fdelete(fileName))
	{
		return BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL;
	}
	else
	{
		return BOAT_SUCCESS;
	}
}

/******************************************************************************
							  BOAT SOCKET WARPPER
							THIS ONLY USED BY FABRIC
*******************************************************************************/
#if (PROTOCOL_USE_HLFABRIC == 1)
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

// 加密算法数组
static uint8_t global_cipher_list[1024] = {0};

/**
 *  \brief 初始化ssl上下文结构体
 *
 *  \param [in] ssl_ctx 要初始化的ssl_ctx_t结构体指针
 *  \return 空
 *
 *  \details 用户可根据需要修改其中的默认值
 */
void set_ssl_ctx_default(ssl_ctx_t *ssl_ctx)
{
	ssl_ctx->init_flg = 0;
	ssl_ctx->ssl_ver = 4;
	ssl_ctx->cipher_suit = TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384;
	// ssl_ctx->cipher_suit = 0xffff;
	ssl_ctx->sec_lvl = 0;
	ssl_ctx->ca_cert = NULL;
	ssl_ctx->client_cert = NULL;
	ssl_ctx->client_key = NULL;
	ssl_ctx->ign_rtctime = 0;
	ssl_ctx->http_enable = 0;
	ssl_ctx->http_index = -1;
	ssl_ctx->smtp_style = 0;
	ssl_ctx->smtp_index = -1;
	ssl_ctx->wolf_ctx = NULL;

	ssl_ctx->used_cnt = 0;
}

/**
 *  \brief 根据指定的SSL版本号，得到相应的SSL协议功能
 *
 *  \param [in] ssl_ctx ssl上下文结构体指针
 *  \return SSL协议功能
 *
 *  \details 用户无需修改此函数
 */
static wolfSSL_method_func ssl_get_ssl_version_method(ssl_ctx_t *ssl_ctx)
{
	if (ssl_ctx == NULL)
		return (wolfSSL_method_func)wolfTLSv1_1_client_method_ex;

	if (ssl_ctx->ssl_ver == 0)
		return (wolfSSL_method_func)wolfSSLv3_client_method;
	if (ssl_ctx->ssl_ver == 1)
		return (wolfSSL_method_func)wolfTLSv1_client_method;
	if (ssl_ctx->ssl_ver == 2)
		return (wolfSSL_method_func)wolfTLSv1_1_client_method;
	if (ssl_ctx->ssl_ver == 3)
		return (wolfSSL_method_func)wolfTLSv1_2_client_method;
	if (ssl_ctx->ssl_ver == 4)
		return (wolfSSL_method_func)wolfSSLv23_client_method; // use highest possible version

	return (wolfSSL_method_func)NULL;
}
/**
 *  \brief 获取加密套件
 *
 *  \param [in] buf 加密套件保存数组
 *  \return 0：成功 -1：失败
 *
 *  \details 用户无需修改此函数
 */
static int8_t ssl_get_full_cipher_list(uint8_t *buf)
{
	uint8_t *cipher_item = NULL;
	uint16_t len = 0;

	if (buf == NULL)
		return -1;

	cipher_item = (uint8_t *)GetCipherNameInternal(0, TLS_RSA_WITH_AES_256_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, TLS_RSA_WITH_AES_128_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, SSL_RSA_WITH_RC4_128_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, SSL_RSA_WITH_RC4_128_MD5);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, SSL_RSA_WITH_3DES_EDE_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, TLS_RSA_WITH_AES_256_CBC_SHA256);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s", cipher_item);

	if (strlen(buf) == 0)
		return -1;

	return 0;
}

// WOFFSSL初始化标志
uint8_t wolf_ssl_init_flg = 0;

/**
 *  \brief ssl模块初始化函数
 *
 *  \param [in] ssl_ctx ssl上下文结构体
 *  \return 0：成功 -1：失败
 *
 *  \details 用户无需修改此函数
 */
int8_t wolf_ssl_init(ssl_ctx_t *ssl_ctx)
{
	wolfSSL_method_func method = NULL;
	int32_t ret = 0;

	if (ssl_ctx == NULL)
	{
		cm_sys_log("wolf_ssl_init input error");
		return -1;
	}

	if (wolf_ssl_init_flg == 0)
	{
		wolf_ssl_init_flg = 1;
		wolfSSL_Debugging_ON();
		wolfSSL_Init();
	}

	method = ssl_get_ssl_version_method(ssl_ctx);
	if (ssl_ctx->wolf_ctx == NULL)
	{
		ssl_ctx->wolf_ctx = SSL_CTX_new(method(NULL));
		if (ssl_ctx->wolf_ctx == NULL)
		{
			cm_sys_log("wolf_ssl_init ssl ctx new error");
			return -1;
		}
	}
	else if (ssl_ctx->used_cnt > 0)
	{
		cm_sys_log("wolf_ssl_init ssl ctx busy, can't reconfig");
		return 0;
	}

	if (ssl_ctx->cipher_suit != 0xffff)
	{
		ret = wolfSSL_CTX_set_cipher_list(ssl_ctx->wolf_ctx, GetCipherNameInternal(ECC_BYTE, ssl_ctx->cipher_suit));
	}
	else
	{

		ssl_get_full_cipher_list(global_cipher_list);
		ret = wolfSSL_CTX_set_cipher_list(ssl_ctx->wolf_ctx, (const char *)global_cipher_list);
	}

	if (ret != WOLFSSL_SUCCESS)
	{
		cm_sys_log("wolf_ssl_init cipher set error");
		return -1;
	}

	if (ssl_ctx->sec_lvl == 0)
		wolfSSL_CTX_set_verify(ssl_ctx->wolf_ctx, WOLFSSL_VERIFY_NONE, NULL);
	else
	{
		int32_t format = SSL_FILETYPE_PEM;
		int ret = 0;
		// 加载密钥证书
		cm_sys_log("sec level 1\n");
		wolfSSL_CTX_set_verify(ssl_ctx->wolf_ctx, WOLFSSL_VERIFY_PEER, NULL);
		cm_sys_log("sec level 1\n");
		ret = wolfSSL_CTX_load_verify_buffer(ssl_ctx->wolf_ctx, ssl_ctx->ca_cert, strlen(ssl_ctx->ca_cert), format);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_sys_log("[SSL]ca load error\n");
			return -1;
		}
	}

	if (ssl_ctx->sec_lvl == 2)
	{
		cm_sys_log("[SSL]sec level 2\n");
		int ret = 0;
		int32_t format = SSL_FILETYPE_PEM;
		ret = wolfSSL_CTX_use_certificate_chain_buffer_format(ssl_ctx->wolf_ctx, ssl_ctx->client_cert, strlen(ssl_ctx->client_cert), format);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_sys_log("%s local ca file use error", __func__);
			return -1;
		}

		ret = wolfSSL_CTX_use_PrivateKey_buffer(ssl_ctx->wolf_ctx, ssl_ctx->client_key, strlen(ssl_ctx->client_key), format);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_sys_log("%s local key file use error", __func__);
			return -1;
		}
	}

	return 0;
}

BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
						const BoatFieldVariable clientCert, BSINT32 *socketfd, void **tlsContext, void *rsvd)
{
	BCHAR *clientcert;
	BCHAR *clientprikey;
	BCHAR *caphain;
	ssl_ctx_t test_ssl_ctx;
	int ret = 0;
	ret = BoatConnect(address, NULL);
	if (ret == -1)
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
		return BOAT_ERROR;
	}

	*socketfd = ret;
	// 初始化SSL上下文
	set_ssl_ctx_default(&test_ssl_ctx);
	BoatSleep(1);
	test_ssl_ctx.sec_lvl = 1; // 1 校验服务器    2、双向校验     0：不校验
#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
	test_ssl_ctx.client_cert = clientCert.field_ptr;
	test_ssl_ctx.client_key = clientPrikey.field_ptr;
	test_ssl_ctx.sec_lvl = 2; // 1 校验服务器    2、双向校验     0：不校验
#endif
	test_ssl_ctx.ca_cert = caChain.field_ptr; // ca证书

	BoatSleep(1);

	// 初始化SSL模块
	wolf_ssl_init(&test_ssl_ctx);

	BoatSleep(1);

	// 新建WOLFSSL实例
	*tlsContext = wolfSSL_new(test_ssl_ctx.wolf_ctx);

	BoatSleep(1);
	// 绑定socket号到WOLFSSL实例
	SSL_set_fd(*tlsContext, *socketfd);
	// 启动WOLFSSL连接握手
	if ((SSL_connect(*tlsContext)) == WOLFSSL_SUCCESS)
	{
		// 连接握手成功
		BoatLog(BOAT_LOG_NORMAL, "WOLFSSL_CONNECT_SUCCESS");
	}
	else
	{
		// 失败
		BoatLog(BOAT_LOG_NORMAL, "WOLFSSL_CONNECT_ERROR");
		wolfSSL_free(*tlsContext);
	}

	BoatLog(BOAT_LOG_NORMAL, " BoatTlsInit OK");
	return BOAT_SUCCESS;
}
#endif

BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd)
{
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
	size_t written_len = 0;

	while (written_len < len)
	{
		int ret = wolfSSL_write(tlsContext, (unsigned char *)(buf + written_len), (len - written_len));
		if (ret > 0)
		{
			written_len += ret;
			continue;
		}
		else if (ret == 0)
		{
			return written_len;
		}
		else
		{
			return -1; /* Connnection error */
		}
	}

	return written_len;
#else
	return send(sockfd, buf, len, 0);
#endif
}

BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
{
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
	int ret = wolfSSL_read(tlsContext, buf, len);

	if (ret >= 0)
	{
		// BoatLog(BOAT_LOG_VERBOSE, "recv data size:%d", ret);
		return ret;
	}
	else if (ret < 0)
	{
		BoatLog(BOAT_LOG_VERBOSE, "recv data fail");
		return ret;
	}
#else
	return recv(sockfd, buf, len, 0);
#endif
}

void BoatClose(BSINT32 sockfd, void **tlsContext, void *rsvd)
{
	wolfSSL_free(*tlsContext);
	close(sockfd);
	*tlsContext = NULL;
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
