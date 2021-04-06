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
#include "boattypes.h"

#include "rand.h"
#include "aes/aes.h"
#include "sha3.h"
#include "secp256k1.h"
#include "nist256p1.h"
#include "bignum.h"

/* net releated include */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#include <sys/time.h>

#if (HLFABRIC_TLS_SUPPORT == 1)
// for TTLSContext structure
#include "http2intf.h"
#endif

uint32_t random32(void)
{
	static uint32_t seed = 0;
	// Linear congruential generator from Numerical Recipes
	// https://en.wikipedia.org/wiki/Linear_congruential_generator
	seed = 1664525 * seed + 1013904223;

	return seed;
}


BOAT_RESULT  BoatRandom( BUINT8* output, BUINT32 outputLen, void* rsvd )
{	
	BOAT_RESULT result = BOAT_SUCCESS;

	(void)rsvd;

	random_buffer(output, outputLen);
	
	return result;
}



BOAT_RESULT BoatSignature( BoatWalletPriKeyCtx prikeyCtx, 
						   const BUINT8* digest, BUINT32 digestLen, 
						   BoatSignatureResult* signatureResult, void* rsvd )
{
	BUINT8 signatureTmp[64];
	BUINT8 ecdsPrefix = 0;
	
	BOAT_RESULT result = BOAT_SUCCESS;
	
	(void)rsvd;
	
	/* param check */
	if( (digest == NULL) || (signatureResult == NULL) )
	{
		BoatLog( BOAT_LOG_CRITICAL, "parameter can't be NULL." );
		return BOAT_ERROR_NULL_POINTER;
	}

	if( prikeyCtx.prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256K1 )
	{
		result = ecdsa_sign_digest( &secp256k1,      // const ecdsa_curve *curve
									prikeyCtx.extra_data.value,        // const uint8_t *priv_key
									digest,          // const uint8_t *digest
									signatureTmp,    // uint8_t *sig,
									&ecdsPrefix,     // uint8_t *pby,
									NULL             // int (*is_canonical)(uint8_t by, uint8_t sig[64]))
									);
	}
	else if( prikeyCtx.prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256R1 )
	{
		result = ecdsa_sign_digest( &nist256p1,      // const ecdsa_curve *curve
									prikeyCtx.extra_data.value,        // const uint8_t *priv_key
									digest,          // const uint8_t *digest
									signatureTmp,    // uint8_t *sig,
									&ecdsPrefix,     // uint8_t *pby,
									NULL             // int (*is_canonical)(uint8_t by, uint8_t sig[64]))
									);
	}
	else
	{
		BoatLog( BOAT_LOG_CRITICAL, "Unkown private key type." );
		return  BOAT_ERROR;
	}

	// signature result assign
	memset(signatureResult, 0, sizeof(BoatSignatureResult));
	
	signatureResult->native_format_used = true;
	memcpy(	signatureResult->native_sign,  signatureTmp, 64 );

	signatureResult->signPrefix_used = true;
	signatureResult->signPrefix      = ecdsPrefix;

	return result;
}


/******************************************************************************
                              BOAT FILE OPERATION WARPPER
*******************************************************************************/
BOAT_RESULT  BoatGetFileSize( const BCHAR *fileName, BUINT32 *size, void* rsvd )
{
	FILE   *file_ptr;
	
	(void)rsvd;
	
	if( (fileName == NULL) || (size == NULL) )
	{
		BoatLog( BOAT_LOG_CRITICAL, "param which 'fileName' or 'size' can't be NULL." );
		return BOAT_ERROR_NULL_POINTER;
	}
	
	file_ptr = fopen( fileName, "rb" );
	if( file_ptr == NULL )
	{
		BoatLog( BOAT_LOG_CRITICAL, "Failed to open file: %s.", fileName );
		return BOAT_ERROR_BAD_FILE_DESCRIPTOR;
	}
	
	fseek( file_ptr, 0, SEEK_END );
	*size   = ftell( file_ptr );
	fclose( file_ptr );
	
	return BOAT_SUCCESS;
}


BOAT_RESULT  BoatWriteFile( const BCHAR *fileName, 
						    BUINT8* writeBuf, BUINT32 writeLen, void* rsvd )
{
	FILE         *file_ptr;
	BSINT32      count = 0;
	BOAT_RESULT  result = BOAT_SUCCESS;
	
	(void)rsvd;
	result = result;
	
	if( (fileName == NULL) || (writeBuf == NULL) )
	{
		BoatLog( BOAT_LOG_CRITICAL, "param which 'fileName' or 'writeBuf' can't be NULL." );
		return BOAT_ERROR_NULL_POINTER;
	}

	/* write to file-system */
	file_ptr = fopen( fileName, "wb" );
	if( file_ptr == NULL )
	{
		BoatLog( BOAT_LOG_CRITICAL, "Failed to create file: %s.", fileName );
		return BOAT_ERROR_BAD_FILE_DESCRIPTOR;
	}
	
	count = fwrite( writeBuf, 1, writeLen, file_ptr );
	fclose( file_ptr );
	if( count != writeLen )
	{
		BoatLog( BOAT_LOG_CRITICAL, "Failed to write file: %s.", fileName );
		return BOAT_ERROR;
	}
	
	return BOAT_SUCCESS;
}


BOAT_RESULT  BoatReadFile( const BCHAR *fileName, 
						   BUINT8 *readBuf, BUINT32 readLen, void* rsvd )
{
	FILE         *file_ptr;
	BSINT32      count = 0;
	BOAT_RESULT  result = BOAT_SUCCESS;
	
	(void)rsvd;
	result = result;
	
	if( (fileName == NULL) || (readBuf == NULL) )
	{
		BoatLog( BOAT_LOG_CRITICAL, "param which 'fileName' or 'readBuf' can't be NULL." );
		return BOAT_ERROR_NULL_POINTER;
	}

	/* read from file-system */
	file_ptr = fopen(fileName, "rb");
	if( file_ptr == NULL )
	{
		BoatLog( BOAT_LOG_CRITICAL, "Failed to open file: %s.", fileName );
		return BOAT_ERROR_BAD_FILE_DESCRIPTOR;
	}
	count = fread( readBuf, 1, readLen, file_ptr );
	fclose( file_ptr );
	if( count != readLen )
	{
		BoatLog( BOAT_LOG_CRITICAL, "Failed to read file: %s.", fileName );
		return BOAT_ERROR;
	}
	
	return BOAT_SUCCESS;
}


BOAT_RESULT  BoatRemoveFile( const BCHAR *fileName, void* rsvd )
{
	(void)rsvd;
		
	if( fileName == NULL )
	{
		BoatLog( BOAT_LOG_CRITICAL, "param which 'fileName' can't be NULL." );
		return BOAT_ERROR_NULL_POINTER;
	}
	
	if( 0 != remove(fileName) )
    {
        return BOAT_ERROR;
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
BSINT32 BoatConnect(const BCHAR *address, void* rsvd)
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
    if( NULL == ptr )
    {
        BoatLog( BOAT_LOG_CRITICAL, "invalid address:%s.", address );
        return -1;
    }

    memset(ip  , 0      , sizeof(ip));
    memset(port, 0      , sizeof(port));
    memcpy(ip  , address, (int)(ptr - address));
    memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));

    if( (he = gethostbyname(ip)) == NULL )
    {
        BoatLog( BOAT_LOG_CRITICAL, "gethostbyname() error" );
        return -1;
    }

    if( (connectfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        BoatLog( BOAT_LOG_CRITICAL, "socket() error" );
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
        BoatLog( BOAT_LOG_CRITICAL, "connect() error" );
        close(connectfd);
        return -1;
    }
    if(getsockname(connectfd, &localaddr, &addrlen) < 0)
    {
        BoatLog( BOAT_LOG_CRITICAL, "getsockname() error" );
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


#if (HLFABRIC_TLS_SUPPORT == 1)	
BOAT_RESULT BoatTlsInit( const BCHAR *hostName, const BoatFieldVariable *caChain,
						 BSINT32 socketfd, void* tlsContext, void* rsvd )
{
	TTLSContext * tlsContext_ptr = (TTLSContext *)tlsContext;
	mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
	
	BOAT_RESULT   result = BOAT_SUCCESS;
	boat_try_declare;

	tlsContext_ptr->ssl     = BoatMalloc(sizeof(mbedtls_ssl_context));
	if (tlsContext_ptr->ssl == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate ssl_context.");
		return BOAT_ERROR;
	}
	tlsContext_ptr->ssl_cfg = BoatMalloc(sizeof(mbedtls_ssl_config));
	if (tlsContext_ptr->ssl_cfg == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate ssl_config.");
		return BOAT_ERROR;
	}
	tlsContext_ptr->ssl_crt = BoatMalloc(sizeof(mbedtls_x509_crt));
	if (tlsContext_ptr->ssl_crt == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate x509_crt.");
		return BOAT_ERROR;
	}
	tlsContext_ptr->ssl_net = BoatMalloc(sizeof(mbedtls_net_context));
	if (tlsContext_ptr->ssl_net == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate net_context.");
		return BOAT_ERROR;
	}
	
    mbedtls_entropy_init( &entropy );
    mbedtls_ctr_drbg_init( &ctr_drbg );
	mbedtls_net_init( tlsContext_ptr->ssl_net );
	mbedtls_ssl_init( tlsContext_ptr->ssl );
	mbedtls_ssl_config_init( tlsContext_ptr->ssl_cfg );
	mbedtls_x509_crt_init( tlsContext_ptr->ssl_crt );
	result = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0 );
    if( result != BOAT_SUCCESS )
	{
		BoatLog( BOAT_LOG_CRITICAL, "Failed to execute ctr_drbg_seed." );
        boat_throw( result, BoatTlsInit_exception );
    }

	result = mbedtls_ssl_config_defaults( tlsContext_ptr->ssl_cfg, MBEDTLS_SSL_IS_CLIENT,
										  MBEDTLS_SSL_TRANSPORT_STREAM,
										  MBEDTLS_SSL_PRESET_DEFAULT );
	if( result != BOAT_SUCCESS )
    {
        BoatLog( BOAT_LOG_CRITICAL, "Failed to execute ssl_config_defaults.\n" );
        boat_throw( result, BoatTlsInit_exception );
    }

	mbedtls_ssl_conf_rng(tlsContext_ptr->ssl_cfg, mbedtls_ctr_drbg_random, &ctr_drbg);

	for(int i = 0; i < HLFABRIC_ROOTCA_MAX_NUM; i++)
	{
		if( ((caChain + i) != NULL ) && ((caChain + i)->field_ptr != NULL ) ) 
		{
			result += mbedtls_x509_crt_parse(tlsContext_ptr->ssl_crt, (caChain + i)->field_ptr, (caChain + i)->field_len);
		}
	}
	if( result != BOAT_SUCCESS )
    {
        BoatLog( BOAT_LOG_CRITICAL, "Failed to execute x509_crt_parse: -%x\n", -result );
        boat_throw( result, BoatTlsInit_exception );
    }

	mbedtls_ssl_conf_ca_chain(tlsContext_ptr->ssl_cfg, tlsContext_ptr->ssl_crt, NULL);
	mbedtls_ssl_conf_authmode(tlsContext_ptr->ssl_cfg, MBEDTLS_SSL_VERIFY_REQUIRED);
	result = mbedtls_ssl_setup(tlsContext_ptr->ssl, tlsContext_ptr->ssl_cfg);
	if( result != BOAT_SUCCESS )
    {
        BoatLog( BOAT_LOG_CRITICAL, "Failed to execute ssl_setup.\n" );
        boat_throw( result, BoatTlsInit_exception );
    }

	result = mbedtls_ssl_set_hostname(tlsContext_ptr->ssl, hostName);
	if( result != BOAT_SUCCESS )
    {
        BoatLog( BOAT_LOG_CRITICAL, "Failed to execute hostname_set.\n" );
        boat_throw( result, BoatTlsInit_exception );
    }

	((mbedtls_net_context*)(tlsContext_ptr->ssl_net))->fd = socketfd;
	mbedtls_ssl_set_bio(tlsContext_ptr->ssl, tlsContext_ptr->ssl_net, mbedtls_net_send, mbedtls_net_recv, NULL);
	result = mbedtls_ssl_handshake(tlsContext_ptr->ssl);
	if( result != BOAT_SUCCESS )
    {
        BoatLog( BOAT_LOG_CRITICAL, "Failed to execute ssl_handshake: -%x\n", -result );
        boat_throw( result, BoatTlsInit_exception );
    }
	BoatLog( BOAT_LOG_NORMAL, "ret = ssl_handshake SUCCESSED!" );
	
	/* boat catch handle */
	boat_catch(BoatTlsInit_exception)
	{
        BoatLog( BOAT_LOG_CRITICAL, "Exception: %d", boat_exception );
        result = boat_exception;
		mbedtls_ssl_free(tlsContext_ptr->ssl);
		mbedtls_net_free(tlsContext_ptr->ssl_net);
		mbedtls_ssl_config_free(tlsContext_ptr->ssl_cfg);
		mbedtls_x509_crt_free(tlsContext_ptr->ssl_crt);
		BoatFree( tlsContext_ptr->ssl );
		BoatFree( tlsContext_ptr->ssl_net );
		BoatFree( tlsContext_ptr->ssl_cfg );
		BoatFree( tlsContext_ptr->ssl_crt );
	}
	
	mbedtls_entropy_free( &entropy );
    mbedtls_ctr_drbg_free( &ctr_drbg );
	
	return result;
}
#endif


BSINT32 BoatSend(BSINT32 sockfd, void* tlsContext, const void *buf, size_t len, void* rsvd)
{
#if (HLFABRIC_TLS_SUPPORT == 1) 
	if( (tlsContext == NULL) || (((TTLSContext*)tlsContext)->ssl == NULL) )
	{
		BoatLog( BOAT_LOG_CRITICAL, "tlsContext or tlsContext->ssl can't be NULL." );
		return -1;
	}
	return mbedtls_ssl_write( ((TTLSContext*)tlsContext)->ssl, buf, len );
#else
	return send( sockfd, buf, len, 0 );	
#endif	
}


BSINT32 BoatRecv(BSINT32 sockfd, void* tlsContext, void *buf, size_t len, void* rsvd)
{
#if (HLFABRIC_TLS_SUPPORT == 1) 
	if( (tlsContext == NULL) || (((TTLSContext*)tlsContext)->ssl == NULL) )
	{
		//! @todo still receive a unknown data after Boatclose(...)
		return -1;
	}
	return mbedtls_ssl_read( ((TTLSContext*)tlsContext)->ssl, buf, len );
#else
	return recv( sockfd, buf, len, 0 );
#endif	
}


void BoatClose(BSINT32 sockfd, void* tlsContext, void* rsvd)
{
	close(sockfd);
#if (HLFABRIC_TLS_SUPPORT == 1) 
	// free tls releated
	if(tlsContext != NULL)
	{
		mbedtls_ssl_free(((TTLSContext*)tlsContext)->ssl);
		BoatFree( ((TTLSContext*)tlsContext)->ssl );
		((TTLSContext*)tlsContext)->ssl     = NULL;
		
		mbedtls_ssl_config_free(((TTLSContext*)tlsContext)->ssl_cfg);
		BoatFree( ((TTLSContext*)tlsContext)->ssl_cfg );
		((TTLSContext*)tlsContext)->ssl_cfg = NULL;
		
		mbedtls_net_free(((TTLSContext*)tlsContext)->ssl_net);
		BoatFree( ((TTLSContext*)tlsContext)->ssl_net );
		((TTLSContext*)tlsContext)->ssl_net = NULL;
		
		mbedtls_x509_crt_free(((TTLSContext*)tlsContext)->ssl_crt);
		BoatFree( ((TTLSContext*)tlsContext)->ssl_crt );
		((TTLSContext*)tlsContext)->ssl_crt = NULL;
	}
#endif
}


/******************************************************************************
                              BOAT KEY PROCESS WARPPER
*******************************************************************************/
static BOAT_RESULT sBoatPort_keyCreate_intern_generation( const BoatWalletPriKeyCtx_config* config, 
													      BoatWalletPriKeyCtx* pkCtx )
{

}

static BOAT_RESULT sBoatPort_keyCreate_extern_injection_native( const BoatWalletPriKeyCtx_config* config, 
													            BoatWalletPriKeyCtx* pkCtx )
{
	BUINT8       pubKey65[65] = {0};
	BOAT_RESULT  result = BOAT_SUCCESS;

	// 1- update private key
	memcpy(pkCtx->extra_data.value, config->prikey_content, config->prikey_content_length);
	pkCtx->extra_data.value_len = config->prikey_content_length;

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

	if( config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256K1 )
	{
		ecdsa_get_public_key65(&secp256k1, pkCtx->extra_data.value, pubKey65);
		memcpy( pkCtx->pubkey_content, &pubKey65[1], 64 );
	}
	else if( config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256R1 )
	{
		ecdsa_get_public_key65(&nist256p1, pkCtx->extra_data.value, pubKey65);
		memcpy( pkCtx->pubkey_content, &pubKey65[1], 64 );
	}
	else 
	{
		;
	}
	

    return result;
}


BOAT_RESULT  BoatPort_keyCreate( const BoatWalletPriKeyCtx_config* config, BoatWalletPriKeyCtx* pkCtx )
{
	BOAT_RESULT               result = BOAT_SUCCESS;
	
	if( (config == NULL) || (pkCtx == NULL) )
	{
		BoatLog( BOAT_LOG_CRITICAL, "parameter can't be NULL." );
		return BOAT_ERROR_NULL_POINTER;
	}
	
	if(config->prikey_genMode == BOAT_WALLET_PRIKEY_GENMODE_INTERN_GENERATION)
	{
		BoatLog( BOAT_LOG_VERBOSE, "The private key is generated internally..." );
		result = sBoatPort_keyCreate_intern_generation(config, pkCtx);
	}
	else if(config->prikey_genMode == BOAT_WALLET_PRIKEY_GENMODE_EXTERN_INJECTION)
	{
		switch (config->prikey_format)
		{
			case BOAT_WALLET_PRIKEY_FORMAT_PKCS:
				break;
			case BOAT_WALLET_PRIKEY_FORMAT_NATIVE:
				BoatLog( BOAT_LOG_VERBOSE, "wallet private key[native] set..." );
				result = sBoatPort_keyCreate_extern_injection_native(config, pkCtx);
				break;
			case BOAT_WALLET_PRIKEY_FORMAT_MNEMONIC:
				BoatLog( BOAT_LOG_NORMAL, "NOT SUPPORT FORMAT YET." );
				result = BOAT_ERROR;
				break;
			default:
				BoatLog( BOAT_LOG_CRITICAL, "invalid private key format." );
				result = BOAT_ERROR;
				break;
		}
	}
	else
	{
		BoatLog( BOAT_LOG_CRITICAL, "invalid private key format." );
		result = BOAT_ERROR;
	}

    return result;
}

BOAT_RESULT  BoatPort_keyQuery( const BoatWalletPriKeyCtx_config* config, BoatWalletPriKeyCtx* pkCtx )
{
	//! @todo
	return BOAT_SUCCESS;
}

BOAT_RESULT  BoatPort_keyDelete( BoatWalletPriKeyCtx* pkCtx )
{
	//! @todo
	return BOAT_SUCCESS;
}


/******************************************************************************
                              BOAT AES WARPPER
*******************************************************************************/
BOAT_RESULT  BoatAesEncrypt(BUINT8 iv[16], const BUINT8 * key, const BUINT8 * input, size_t length, BUINT8 * output)
{
	aes_encrypt_ctx ctxe;
	BUINT8  saltArrayTmp[16];
	BOAT_RESULT result = BOAT_SUCCESS;

	/* aes init */
	result += aes_encrypt_key128( key, &ctxe );

	/* use saltArrayTmp because function aes_cbc_encrypt(...) will modify this field */
	memcpy(saltArrayTmp, iv, 16);

	/* encrypt process */
	result += aes_cbc_encrypt(input, output, length, saltArrayTmp, &ctxe);

	return result;
}

BOAT_RESULT  BoatAesDecrypt(BUINT8 iv[16], const BUINT8 * key, const BUINT8 * input, size_t length, BUINT8 * output)
{
	aes_encrypt_ctx ctxd;
	BOAT_RESULT result = BOAT_SUCCESS;

	/* aes init */
	result += aes_decrypt_key128( key, &ctxd );

	/* decrypt process */
	result += aes_cbc_decrypt(input, output, length, iv, &ctxd);

	return result;
}
