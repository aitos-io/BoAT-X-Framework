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
#include "boatconfig.h"
#include "boatplatform_internal.h"

#include "boattypes.h"

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

#include <sys/time.h>
#endif

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
// for TTLSContext structure
#include "http2intf.h"
#endif

#define GENERATE_KEY_REPEAT_TIMES	100

/*
uint32_t random32(void)
{
	static uint32_t seed = 0;
	if(seed == 0)
	{
		seed = time(NULL);
	}
	// Linear congruential generator from Numerical Recipes
	// https://en.wikipedia.org/wiki/Linear_congruential_generator
	seed = 1664525 * seed + 1013904223;

	return seed;
}
*/


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


BOAT_RESULT BoatSignature(BoatWalletPriKeyCtx prikeyCtx, 
						  const BUINT8 *digest, BUINT32 digestLen, 
						  BoatSignatureResult *signatureResult, void *rsvd)
{
	BUINT8 signatureTmp[64];
	BUINT8 ecdsPrefix = 0;
	
	BOAT_RESULT result = BOAT_SUCCESS;
	
	(void)rsvd;
	
	/* param check */
	if ((digest == NULL) || (signatureResult == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	if (prikeyCtx.prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256K1)
	{
		result = ecdsa_sign_digest(&secp256k1,      // const ecdsa_curve *curve
								   prikeyCtx.extra_data.value,        // const uint8_t *priv_key
								   digest,          // const uint8_t *digest
								   signatureTmp,    // uint8_t *sig,
								   &ecdsPrefix,     // uint8_t *pby,
								   NULL);           // int (*is_canonical)(uint8_t by, uint8_t sig[64]))		
	}
	else if (prikeyCtx.prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256R1)
	{
		result = ecdsa_sign_digest(&nist256p1,      // const ecdsa_curve *curve
								   prikeyCtx.extra_data.value,        // const uint8_t *priv_key
								   digest,          // const uint8_t *digest
								   signatureTmp,    // uint8_t *sig,
								   &ecdsPrefix,     // uint8_t *pby,
								   NULL);           // int (*is_canonical)(uint8_t by, uint8_t sig[64]))
									
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Unkown private key type.");
		return  BOAT_ERROR_WALLET_KEY_TYPE_ERR;
	}

	// signature result assign
	memset(signatureResult, 0, sizeof(BoatSignatureResult));
	
	signatureResult->native_format_used = true;
	memcpy(	signatureResult->native_sign,  signatureTmp, 64);

	signatureResult->signPrefix_used = true;
	signatureResult->signPrefix      = ecdsPrefix;

	return result;
}


/******************************************************************************
                              BOAT FILE OPERATION WARPPER
*******************************************************************************/
BOAT_RESULT  BoatGetFileSize(const BCHAR *fileName, BUINT32 *size, void *rsvd)
{
	FILE   *file_ptr;
	
	(void)rsvd;
	
	if ((fileName == NULL) || (size == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'fileName' or 'size' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	
	file_ptr = fopen(fileName, "rb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", fileName);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	
	fseek(file_ptr, 0, SEEK_END);
	*size   = ftell(file_ptr);
	fclose(file_ptr);
	
	return BOAT_SUCCESS;
}


BOAT_RESULT  BoatWriteFile(const BCHAR *fileName, 
						   BUINT8 *writeBuf, BUINT32 writeLen, void *rsvd)
{
	FILE         *file_ptr;
	BSINT32      count = 0;
	
	(void)rsvd;
	
	if ((fileName == NULL) || (writeBuf == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'fileName' or 'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* write to file-system */
	file_ptr = fopen(fileName, "wb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to create file: %s.", fileName);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	
	count = fwrite(writeBuf, 1, writeLen, file_ptr);
	fclose(file_ptr);
	if (count != writeLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", fileName);
		return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
	}
	
	return BOAT_SUCCESS;
}


BOAT_RESULT  BoatReadFile(const BCHAR *fileName, 
						  BUINT8 *readBuf, BUINT32 readLen, void *rsvd)
{
	FILE         *file_ptr;
	BSINT32      count = 0;
	
	(void)rsvd;
	
	if ((fileName == NULL) || (readBuf == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'fileName' or 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* read from file-system */
	file_ptr = fopen(fileName, "rb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", fileName);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	count = fread(readBuf, 1, readLen, file_ptr);
	fclose(file_ptr);
	if (count != readLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", fileName);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	
	return BOAT_SUCCESS;
}


BOAT_RESULT  BoatRemoveFile(const BCHAR *fileName, void *rsvd)
{
	(void)rsvd;
		
	if (fileName == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'fileName' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	
	if (0 != remove(fileName))
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
    int                 connectfd;
    char                ip[64];
    char                port[8];
    char                *ptr = NULL;
    struct hostent      *he; 
    struct sockaddr_in  server;
    struct sockaddr     localaddr;
    struct sockaddr_in  *localaddr_ptr;
    socklen_t           addrlen = sizeof(struct sockaddr);

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

    struct timeval timeout = {0, 500*1000};
    setsockopt(connectfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
    setsockopt(connectfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(port));
    server.sin_addr = *((struct in_addr *)(he->h_addr_list[0])); 

    if(connect(connectfd, (struct sockaddr *)&server,sizeof(struct sockaddr)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error");
        close(connectfd);
        return -1;
    }
    if(getsockname(connectfd, &localaddr, &addrlen) < 0)
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
static BOAT_RESULT sBoatPort_keyCreate_internal_generation(const BoatWalletPriKeyCtx_config *config, 
													       BoatWalletPriKeyCtx *pkCtx)
{
	/* Valid private key value (as a UINT256) for Ethereum is [1, n-1], where n is
       0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141 */
    const BUINT32 priv_key_max_u256[8] = { 0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6,
                                           0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
	bignum256   priv_key_bn256;
    bignum256   priv_key_max_bn256;
	BUINT8      prikeyTmp[32];
	BUINT8      pubKey65[65] = {0};
	BUINT32     key_try_count;
    BOAT_RESULT result = BOAT_SUCCESS;

	/* Convert priv_key_max_u256 from UINT256 to Bignum256 format */
    bn_read_le((const uint8_t *)priv_key_max_u256, &priv_key_max_bn256);

	// 1- update private key
	/* generate native private key loop */
	for (key_try_count = 0; key_try_count < GENERATE_KEY_REPEAT_TIMES; key_try_count++)
    {
		/* generate native private key */
        result = BoatRandom(prikeyTmp, 32, NULL);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to generate private key.");
            break;
        }

		/* Convert private key from UINT256 to Bignum256 format */
    	bn_read_le(prikeyTmp, &priv_key_bn256);

		/* check the generated private key is valid or not */
		if ((bn_is_zero(&priv_key_bn256) == 0) && \
		    (bn_is_less(&priv_key_bn256, &priv_key_max_bn256) != 0))
		{
			/* key is valid */
			memcpy(pkCtx->extra_data.value, prikeyTmp, 32);
			pkCtx->extra_data.value_len = 32;
			result = BOAT_SUCCESS;
			break;
		}
		else if (key_try_count == GENERATE_KEY_REPEAT_TIMES - 1)
		{
			result = BOAT_ERROR;
		}
    }

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "generate private key failed.");
		return result;
	}

	// 2- update private key format
	pkCtx->prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
	
	// 3- update private key type
	pkCtx->prikey_type   = config->prikey_type;

	// 4- update private key index
	// This field should update by 'key secure storage'(such as TE/SE).
	// When algorithms are implemented by software, this field is default to 0, means
	// that ignore this field.
	pkCtx->prikey_index  = 0; 

	// 5- update public key
	pkCtx->pubkey_format = BOAT_WALLET_PUBKEY_FORMAT_NATIVE;

	if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256K1)
	{
		ecdsa_get_public_key65(&secp256k1, pkCtx->extra_data.value, pubKey65);
		memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
	}
	else if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256R1)
	{
		ecdsa_get_public_key65(&nist256p1, pkCtx->extra_data.value, pubKey65);
		memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
	}
	else 
	{
		BoatLog(BOAT_LOG_CRITICAL, "Invalid private key type.");
		result = BOAT_ERROR_WALLET_KEY_TYPE_ERR;
	}

	return result;
}

static BOAT_RESULT sBoatPort_keyCreate_external_injection_native(const BoatWalletPriKeyCtx_config *config, 
													             BoatWalletPriKeyCtx *pkCtx)
{
	BUINT8       pubKey65[65] = {0};
	BOAT_RESULT  result = BOAT_SUCCESS;

	// 0- check input parameter
	if ((config == NULL) || (config->prikey_content.field_ptr == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "input parameter can not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// 1- update private key
	if (config->prikey_content.field_len > sizeof(pkCtx->extra_data.value))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Error: length of injection key is too long.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}

	memcpy(pkCtx->extra_data.value, config->prikey_content.field_ptr, config->prikey_content.field_len);
	pkCtx->extra_data.value_len = config->prikey_content.field_len;

	// 2- update private key format
	pkCtx->prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
	
	// 3- update private key type
	pkCtx->prikey_type   = config->prikey_type;

	// 4- update private key index
	// This field should update by 'key secure storage'(such as TE/SE).
	// When algorithms are implemented by software, this field is default to 0, means
	// that ignore this field.
	pkCtx->prikey_index  = 0; 

	// 5- update public key
	pkCtx->pubkey_format = BOAT_WALLET_PUBKEY_FORMAT_NATIVE;

	if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256K1)
	{
		ecdsa_get_public_key65(&secp256k1, pkCtx->extra_data.value, pubKey65);
		memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
	}
	else if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256R1)
	{
		ecdsa_get_public_key65(&nist256p1, pkCtx->extra_data.value, pubKey65);
		memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
	}
	else 
	{
		BoatLog(BOAT_LOG_CRITICAL, "Invalid private key type.");
		result = BOAT_ERROR_WALLET_KEY_TYPE_ERR;
	}
	
    return result;
}

static BOAT_RESULT sBoatPort_keyCreate_external_injection_pkcs(const BoatWalletPriKeyCtx_config *config, 
													             BoatWalletPriKeyCtx *pkCtx)
{
	BUINT8       pubKey65[65] = {0};
	BOAT_RESULT  result = BOAT_SUCCESS;
	KeypairNative keypair;

	// 0- check input parameter
	if ((config == NULL) || (config->prikey_content.field_ptr == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "input parameter can not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	// 1- update private key
	if (config->prikey_content.field_len > sizeof(pkCtx->extra_data.value))
	{
		BoatLog(BOAT_LOG_CRITICAL, "Error: length of injection key is too long.");
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}

    result = UtilityPKCS2Native(config->prikey_content.field_ptr,&keypair);
    if(result != BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> UtilityPKCS2Native err.");
				UtilityFreeKeypair(keypair);
        return result;
    }

	// memcpy(pkCtx->extra_data.value, config->prikey_content.field_ptr, config->prikey_content.field_len);
	// pkCtx->extra_data.value_len = config->prikey_content.field_len;
	memcpy(pkCtx->extra_data.value,keypair.prikey,keypair.prikeylen);
	pkCtx->extra_data.value_len = keypair.prikeylen;

	// 2- update private key format
	pkCtx->prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
	
	// 3- update private key type
	pkCtx->prikey_type   = config->prikey_type;

	// 4- update private key index
	// This field should update by 'key secure storage'(such as TE/SE).
	// When algorithms are implemented by software, this field is default to 0, means
	// that ignore this field.
	pkCtx->prikey_index  = 0; 

	// 5- update public key
	pkCtx->pubkey_format = BOAT_WALLET_PUBKEY_FORMAT_NATIVE;

	// if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256K1)
	// {
	// 	ecdsa_get_public_key65(&secp256k1, pkCtx->extra_data.value, pubKey65);
	// 	memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
	// }
	// else if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256R1)
	// {
	// 	ecdsa_get_public_key65(&nist256p1, pkCtx->extra_data.value, pubKey65);
	// 	memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
	// }
	// else 
	// {
	// 	BoatLog( BOAT_LOG_CRITICAL, "Invalid private key type." );
	// 	result = BOAT_ERROR_WALLET_KEY_TYPE_ERR;
	// }
	memcpy(pkCtx->pubkey_content, keypair.pubkey, keypair.pubkeylen);
	UtilityFreeKeypair(keypair);
  return result;
}


BOAT_RESULT  BoatPort_keyCreate(const BoatWalletPriKeyCtx_config *config, BoatWalletPriKeyCtx *pkCtx)
{
	BOAT_RESULT               result = BOAT_SUCCESS;
	
	if ((config == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	
	if(config->prikey_genMode == BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION)
	{
		BoatLog(BOAT_LOG_VERBOSE, "The private key is generated internally...");
		result = sBoatPort_keyCreate_internal_generation(config, pkCtx);
	}
	else if(config->prikey_genMode == BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION)
	{
		switch (config->prikey_format)
		{
			case BOAT_WALLET_PRIKEY_FORMAT_PKCS:
				BoatLog(BOAT_LOG_NORMAL, "wallet private key[pkcs] set...");
				result = sBoatPort_keyCreate_external_injection_pkcs(config, pkCtx);
				break;
			case BOAT_WALLET_PRIKEY_FORMAT_NATIVE:
				BoatLog(BOAT_LOG_VERBOSE, "wallet private key[native] set...");
				result = sBoatPort_keyCreate_external_injection_native(config, pkCtx);
				break;
			case BOAT_WALLET_PRIKEY_FORMAT_MNEMONIC:
				BoatLog(BOAT_LOG_NORMAL, "NOT SUPPORT FORMAT YET.");
				result = BOAT_ERROR_WALLET_KEY_FORMAT_ERR;
				break;
			default:
				BoatLog(BOAT_LOG_CRITICAL, "Invalid private key format.");
				result = BOAT_ERROR_WALLET_KEY_FORMAT_ERR;
				break;
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "Invalid private key format.");
		result = BOAT_ERROR_WALLET_KEY_GENMODE_ERR;
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
	BUINT8  saltArrayTmp[16];
	BOAT_RESULT result = BOAT_SUCCESS;

	/* aes init */
	result += aes_encrypt_key128(key, &ctxe);

	/* use saltArrayTmp because function aes_cbc_encrypt(...) will modify this field */
	memcpy(saltArrayTmp, iv, 16);

	/* encrypt process */
	result += aes_cbc_encrypt(input, output, length, saltArrayTmp, &ctxe);

	return result;
}

BOAT_RESULT  BoatAesDecrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
	aes_decrypt_ctx ctxd;
	BOAT_RESULT result = BOAT_SUCCESS;

	/* aes init */
	result += aes_decrypt_key128(key, &ctxd);

	/* decrypt process */
	result += aes_cbc_decrypt(input, output, length, iv, &ctxd);

	return result;
}
