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
#include "boatwallet.h"

#include "keccak.h"

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

#if ((PROTOCOL_USE_HLFABRIC == 1) || (PROTOCOL_USE_CHAINMAKER == 1))
// for TTLSContext structure
#include "http2intf.h"
#endif


static int ecdsa_signature_to_asn1(const mbedtls_mpi *r, const mbedtls_mpi *s,
								   unsigned char *sig, size_t *slen)
{
    unsigned char buf[MBEDTLS_ECDSA_MAX_LEN];
    unsigned char *p = buf + sizeof(buf);
    size_t len = 0;
	int ret;

    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_mpi(&p, buf, s));
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_mpi(&p, buf, r));
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_len(&p, buf, len));
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_tag(&p, buf,
						  MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE));

    memcpy(sig, p, len);
    *slen = len;

    return(0);
}

static int derive_mpi(const mbedtls_ecp_group *grp, mbedtls_mpi *x,
                      const unsigned char *buf, size_t blen)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t n_size = (grp->nbits + 7) / 8;
    size_t use_size = blen > n_size ? n_size : blen;

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(x, buf, use_size));
    if (use_size * 8 > grp->nbits)
        MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(x, use_size * 8 - grp->nbits));

    /* While at it, reduce modulo N */
    if (mbedtls_mpi_cmp_mpi(x, &grp->N) >= 0)
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(x, x, &grp->N));

cleanup:
    return(ret);
}

/******************************************************************************
 * @brief ECDSA with prefix
 *
 * ECDSA with prefix. s of signature is guaranteed to be less than N/2. This function
 * both generates ASN.1 format signatures and native signatures consisting of R and S.
 * 
 * @param[in] ctx 
 *   ECDSA context.
 *
 * @param[in] digest
 *   A digest message. 
 *
 * @param[in] digestLen 
 *   The length of digiest message.
 *
 * @param[out] signature 
 *   ASN.1 format signature.The signature size maximum 139 bytes, caller needs to 
 *   make sure that there is enough space to store it.
 *
 * @param[out] signatureLen 
 *   ASN.1 format signature length.
 * @param[out] raw_r
 *   The r field of the native signature, the space of this filed is fixed 32 bytes.
 *   Caller needs to make sure that there is enough space to store it.
 *
 * @param[out] raw_s 
 *   The s field of the native signature, the space of this filed is fixed 32 bytes.
 *   Caller needs to make sure that there is enough space to store it.
 *
 * @param[in] f_rng
 *   The function of random number generation. This function is used to generate a 
 *   random number to generate r filed of signature.
 *
 * @param[in] p_rng 
 *   The parameter f_rng.
 *
 * @param[in] f_rng_blind 
 *   The function of random number generation. This function is used to generate a 
 *   random number to counter side-channel attacks.
 *
 * @param[in] p_rng_blind 
 *   The parameter f_rng_blind.
 *
 * @param[out] ecdsPrefix
 *   The generate signature Prefix.
 *
 * @return 
  *   Return \c BOAT_SUCCESS if generate success; otherwise return a negative error code.
 ******************************************************************************/
static BOAT_RESULT Boat_private_ecdsa_sign(mbedtls_ecdsa_context *ctx, 
										   const BUINT8 *digest, size_t digestLen,
									       BUINT8 *signature, size_t *signatureLen, BUINT8 *raw_r, BUINT8 *raw_s,
									       int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
									       int (*f_rng_blind)(void *, unsigned char *, size_t),
									       void *p_rng_blind, unsigned char *ecdsPrefix)
{
    int ret, key_tries, sign_tries;
    mbedtls_ecp_point R;
	mbedtls_mpi r, s;
    mbedtls_mpi k, e, t; //k: temporary random number of signature
	                     //e: convert from hash message
	                     //t: temporary random number to counter side-channel attacks
	int boat_ecdsPrefix;

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if (ctx == NULL || !mbedtls_ecdsa_can_do(ctx->grp.id) || ctx->grp.N.p == NULL)
        return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    /* Make sure d is in range 1..n-1 */
    if (mbedtls_mpi_cmp_int(&ctx->d, 1) < 0 || mbedtls_mpi_cmp_mpi(&ctx->d, &ctx->grp.N) >= 0)
        return(MBEDTLS_ERR_ECP_INVALID_KEY);

    mbedtls_ecp_point_init(&R);
    mbedtls_mpi_init(&k); 
	mbedtls_mpi_init(&e); 
	mbedtls_mpi_init(&t);
	mbedtls_mpi_init(&r); 
	mbedtls_mpi_init(&s);

    sign_tries = 0;
    do
    {
        if ((sign_tries)++ > 10)
        {
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto cleanup;
        }

        /*
         * Steps 1-3: generate a suitable ephemeral keypair
         * and set r = xR mod n
         */
        key_tries = 0;
        do
        {
            if ((key_tries)++ > 10)
            {
                ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
                goto cleanup;
            }

            MBEDTLS_MPI_CHK(mbedtls_ecp_gen_privkey(&ctx->grp, &k, f_rng, p_rng));

            MBEDTLS_MPI_CHK(mbedtls_ecp_mul_restartable(&ctx->grp, &R, &k, &ctx->grp.G,
							f_rng_blind,
							p_rng_blind,
							NULL));
            MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&r, &R.X, &ctx->grp.N));
			
			//-----------------------------------------------------------
			// -boat:boat_ecdsPrefix update 
			boat_ecdsPrefix = *(R.Y.p) & 1;
            if (1 == mbedtls_mpi_cmp_mpi(&R.X, &ctx->grp.N)){// R.X > N
                boat_ecdsPrefix |= 2;
            }
			//-----------------------------------------------------------
        }
        while(mbedtls_mpi_cmp_int(&r, 0) == 0);

        /*
         * Step 5: derive MPI from hashed message
         */
        MBEDTLS_MPI_CHK(derive_mpi(&ctx->grp, &e, digest, digestLen));

        /*
         * Generate a random value to blind inv_mod in next step,
         * avoiding a potential timing leak.
         */
        MBEDTLS_MPI_CHK(mbedtls_ecp_gen_privkey(&ctx->grp, &t, f_rng_blind,
						p_rng_blind));

        /*
         * Step 6: compute s = (e + r * d) / k = t (e + rd) / (kt) mod n
         */
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&s, &r, &ctx->d));
        MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&e, &e, &s));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&e, &e, &t));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&k, &k, &t));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&k, &k, &ctx->grp.N));
        MBEDTLS_MPI_CHK(mbedtls_mpi_inv_mod(&s, &k, &ctx->grp.N));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&s, &s, &e));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&s, &s, &ctx->grp.N));
    }
    while(mbedtls_mpi_cmp_int(&s, 0) == 0);

	//-----------------------------------------------------------
	// -boat: boat_ecdsPrefix update
	mbedtls_mpi x;
    mbedtls_mpi_init(&x);
    mbedtls_mpi_mul_int(&x, &s, 2);
	// compare 2S with N
	if (0 < mbedtls_mpi_cmp_mpi(&x, &ctx->grp.N)){
		mbedtls_mpi_sub_mpi(&s, &ctx->grp.N, &s); // -s should less than N/2
		boat_ecdsPrefix ^= 1;
    }
	mbedtls_mpi_free(&x);
	*ecdsPrefix = boat_ecdsPrefix;
	//-----------------------------------------------------------
	
	/* convert asn1 signature to raw r & s */
	mbedtls_mpi_write_binary(&r, raw_r, 32);
	mbedtls_mpi_write_binary(&s, raw_s, 32);
	/* convert r,s to asn.1 */
	ecdsa_signature_to_asn1(&r, &s, signature, signatureLen);

cleanup:
    mbedtls_ecp_point_free(&R);
    mbedtls_mpi_free(&k); 
	mbedtls_mpi_free(&e); 
	mbedtls_mpi_free(&t);
	mbedtls_mpi_free(&r); 
	mbedtls_mpi_free(&s);

    return(ret);
}


BOAT_RESULT  BoatRandom(BUINT8 *output, BUINT32 outputLen, void *rsvd)
{
	mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context  entropy;
	
	BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
	
	mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
	result = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
	if (result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec  mbedtls_ctr_drbg_seed.");
        boat_throw(result, hlfabricGenNonce_exception);
	}
	
	result = mbedtls_ctr_drbg_random(&ctr_drbg, output, outputLen);
	if (result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec  mbedtls_ctr_drbg_random(nonce).");
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


BOAT_RESULT BoatSignature(BoatWalletPriKeyCtx prikeyCtx, 
						  const BUINT8 *digest, BUINT32 digestLen, 
						  BoatSignatureResult *signatureResult, void *rsvd)
{
	mbedtls_entropy_context   entropy;
    mbedtls_ctr_drbg_context  ctr_drbg;
	mbedtls_pk_context        mbedtls_pkCtx;
    mbedtls_ecdsa_context *ecPrikey = NULL;
	BUINT8 signatureTmp[139];
	size_t signatureTmpLen = 0;
	BUINT8 raw_r[32];
	BUINT8 raw_s[32];
	BUINT8 ecdsPrefix = 0;
	
	BOAT_RESULT result;
	boat_try_declare;
	
	(void)rsvd;
	
	/* param check */
	if ((digest == NULL) || (signatureResult == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_pk_init(&mbedtls_pkCtx);
	
	result = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
	if(result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec mbedtls_ctr_drbg_seed.");
        boat_throw(result, BoatSignature_exception);
    }
	
	if (prikeyCtx.prikey_format == BOAT_WALLET_PRIKEY_FORMAT_PKCS)
	{
		/* get prikey content according to prikey index */
		//sBoat_get_prikey_content(prikeyCtx,);
		//! @todo function todo
		
		/* parse is prikey content */
		result = mbedtls_pk_parse_key(&mbedtls_pkCtx, prikeyCtx.extra_data.value,
									  strlen((const char *)prikeyCtx.extra_data.value) + 1, NULL, 0);
		ecPrikey = mbedtls_pk_ec(mbedtls_pkCtx);
	}
	else
	{
		// BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
		// boat_throw(BOAT_ERROR, BoatSignature_exception);
		if (prikeyCtx.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256K1)
		{
			BoatLog(BOAT_LOG_CRITICAL, "begin mbedtls_ecp_read_key ");
			ecPrikey = BoatCalloc(1, sizeof(mbedtls_ecp_keypair));
			if (ecPrikey != NULL)
				mbedtls_ecp_keypair_init(ecPrikey);
			result = mbedtls_ecp_read_key(MBEDTLS_ECP_DP_SECP256K1, ecPrikey, prikeyCtx.extra_data.value, prikeyCtx.extra_data.value_len);
			BoatLog(BOAT_LOG_CRITICAL, "mbedtls_ecp_read_key result = %x ", result);
		}
		else if (prikeyCtx.prikey_type = BOAT_WALLET_PRIKEY_TYPE_SECP256R1)
		{
			ecPrikey = BoatCalloc(1, sizeof(mbedtls_ecp_keypair));
			if (ecPrikey != NULL)
				mbedtls_ecp_keypair_init(ecPrikey);
			result = mbedtls_ecp_read_key(MBEDTLS_ECP_DP_SECP256R1, ecPrikey, prikeyCtx.extra_data.value, prikeyCtx.extra_data.value_len);
			BoatLog(BOAT_LOG_CRITICAL, "mbedtls_ecp_read_key result = %x ", result);
		}
		else
		{
			BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
			boat_throw(BOAT_ERROR_WALLET_KEY_TYPE_ERR, BoatSignature_exception);
		}
	}
    if(result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec mbedtls_pk_parse.");
        boat_throw(result, BoatSignature_exception);
    }
	//ecPrikey = mbedtls_pk_ec(mbedtls_pkCtx);

	/* signature process */
	result = Boat_private_ecdsa_sign(ecPrikey, digest, digestLen, signatureTmp, &signatureTmpLen, raw_r, raw_s, 
							 		 mbedtls_ctr_drbg_random, &ctr_drbg,
							  		 mbedtls_ctr_drbg_random, &ctr_drbg, &ecdsPrefix);
	if(result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec mbedtls_ecdsa_write_signature.");
		boat_throw(result, BoatSignature_exception);
	}
	
	// signature result assign
	memset(signatureResult, 0, sizeof(BoatSignatureResult));

	signatureResult->pkcs_format_used = true;
	signatureResult->pkcs_sign_length = signatureTmpLen;
	memcpy(signatureResult->pkcs_sign, signatureTmp, signatureResult->pkcs_sign_length);
	
	signatureResult->native_format_used = true;
	memcpy(	&signatureResult->native_sign[0],  raw_r, 32);
	memcpy(	&signatureResult->native_sign[32], raw_s, 32);

	signatureResult->signPrefix_used = true;
	signatureResult->signPrefix      = ecdsPrefix;

	/* boat catch handle */
	boat_catch(BoatSignature_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

	/* free */
	mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_pk_free(&mbedtls_pkCtx);
	if (prikeyCtx.prikey_format != BOAT_WALLET_PRIKEY_FORMAT_PKCS){
		mbedtls_ecp_keypair_free(ecPrikey);
		if(ecPrikey != NULL){
			BoatFree(ecPrikey);
		}
	}

	return result;
}


BOAT_RESULT BoatGetFileSize(const BCHAR *fileName, BUINT32 *size, void *rsvd)
{
	FILE *file_ptr;
	
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
	*size = ftell(file_ptr);
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

#if ((PROTOCOL_USE_HLFABRIC == 1) || (PROTOCOL_USE_CHAINMAKER == 1))

BSINT32 BoatConnect(const BCHAR *address, void *rsvd)
{
    int                 connectfd = -1;
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
    if(0 != getsockname(connectfd, &localaddr, &addrlen))
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


#if (BOAT_TLS_SUPPORT == 1)	
BOAT_RESULT BoatTlsInit(const BCHAR *hostName, const BoatFieldVariable *caChain,
						BSINT32 socketfd, void *tlsContext, void *rsvd)
{
	TTLSContext  *tlsContext_ptr = (TTLSContext *)tlsContext;
	mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
	
	BOAT_RESULT   result = BOAT_SUCCESS;
	boat_try_declare;

	tlsContext_ptr->ssl     = BoatMalloc(sizeof(mbedtls_ssl_context));
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

	// for(int i = 0; i < BOAT_HLFABRIC_ROOTCA_MAX_NUM; i++)
	// {
	// 	if (((caChain + i) != NULL) && ((caChain + i)->field_ptr != NULL)) 
	// 	{
	// 		result += mbedtls_x509_crt_parse(tlsContext_ptr->ssl_crt, (caChain + i)->field_ptr, (caChain + i)->field_len);
	// 	}
	// }

	// BoatLog_hexasciidump(BOAT_LOG_NORMAL, "tlsCAchain  :",
	// 	caChain[0].field_ptr,
	// 	caChain[0].field_len);

	result += mbedtls_x509_crt_parse(tlsContext_ptr->ssl_crt, caChain[0].field_ptr, caChain[0].field_len);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute x509_crt_parse: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }

	mbedtls_ssl_conf_ca_chain(tlsContext_ptr->ssl_cfg, tlsContext_ptr->ssl_crt, NULL);
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

	((mbedtls_net_context*)(tlsContext_ptr->ssl_net))->fd = socketfd;
	mbedtls_ssl_set_bio(tlsContext_ptr->ssl, tlsContext_ptr->ssl_net, mbedtls_net_send, mbedtls_net_recv, NULL);
	result = mbedtls_ssl_handshake(tlsContext_ptr->ssl);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_handshake: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }
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
	if ((tlsContext == NULL) || (((TTLSContext*)tlsContext)->ssl == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext or tlsContext->ssl can't be NULL.");
		return -1;
	}
	return mbedtls_ssl_write(((TTLSContext*)tlsContext)->ssl, buf, len);
#else
	return send(sockfd, buf, len, 0);	
#endif	
}


BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1) 
	if ((tlsContext == NULL) || (((TTLSContext*)tlsContext)->ssl == NULL))
	{
		//! @todo still receive a unknown data after Boatclose(...)
		return -1;
	}
	return mbedtls_ssl_read(((TTLSContext*)tlsContext)->ssl, buf, len);
#else
	return recv(sockfd, buf, len, 0);
#endif	
}


void BoatClose(BSINT32 sockfd, void *tlsContext, void *rsvd)
{
	close(sockfd);
#if (BOAT_TLS_SUPPORT == 1)
	// free tls releated
	if(tlsContext != NULL)
	{
		mbedtls_ssl_free(((TTLSContext*)tlsContext)->ssl);
		BoatFree(((TTLSContext*)tlsContext)->ssl);
		((TTLSContext*)tlsContext)->ssl     = NULL;
		
		mbedtls_ssl_config_free(((TTLSContext*)tlsContext)->ssl_cfg);
		BoatFree(((TTLSContext*)tlsContext)->ssl_cfg);
		((TTLSContext*)tlsContext)->ssl_cfg = NULL;
		
		mbedtls_net_free(((TTLSContext*)tlsContext)->ssl_net);
		BoatFree(((TTLSContext*)tlsContext)->ssl_net);
		((TTLSContext*)tlsContext)->ssl_net = NULL;
		
		mbedtls_x509_crt_free(((TTLSContext*)tlsContext)->ssl_crt);
		BoatFree(((TTLSContext*)tlsContext)->ssl_crt);
		((TTLSContext*)tlsContext)->ssl_crt = NULL;
	}
#endif
}

#endif


/******************************************************************************
                              BOAT KEY PROCESS WARPPER
*******************************************************************************/
static BOAT_RESULT sBoatPort_keyCreate_internal_generation(const BoatWalletPriKeyCtx_config *config, 
													       BoatWalletPriKeyCtx *pkCtx)
{
	mbedtls_entropy_context   entropy;
    mbedtls_ctr_drbg_context  ctr_drbg;
	mbedtls_pk_context        key;
	BOAT_RESULT               result = BOAT_SUCCESS;
	
	if ((config == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_pk_init(&key);

	result += mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
    result += mbedtls_pk_setup(&key, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));
	
	if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256K1)
	{
		result += mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256K1, mbedtls_pk_ec(key),
									  mbedtls_ctr_drbg_random, &ctr_drbg);
	}
	else if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256R1)
	{
		result += mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(key),
									  mbedtls_ctr_drbg_random, &ctr_drbg);
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "unknown private keytype...");
		result = BOAT_ERROR_WALLET_KEY_TYPE_ERR;
	}

	if (config->prikey_format == BOAT_WALLET_PRIKEY_FORMAT_PKCS)
	{
		// 1- update private key
		memset(pkCtx->extra_data.value, 0, sizeof(pkCtx->extra_data.value));
		result += mbedtls_pk_write_key_pem(&key, pkCtx->extra_data.value, sizeof(pkCtx->extra_data.value));

		// 2- update private key format
		pkCtx->prikey_format = BOAT_WALLET_PRIKEY_FORMAT_PKCS;

		// 3- update private key type
		pkCtx->prikey_type = config->prikey_type;
	}
	else
	{

		BoatLog(BOAT_LOG_CRITICAL, "sBoatPort_keyCreate_internal_generation 000");
		// 1- update private key
		memset(pkCtx->extra_data.value, 0, sizeof(pkCtx->extra_data.value));
		// result += mbedtls_pk_write_key_pem(&key, pkCtx->extra_data.value, sizeof(pkCtx->extra_data.value));
		// mbedtls_pk_ec(key)->d.

		// mbedtls_mpi_write_binary(&(mbedtls_pk_ec(key)->d),pkCtx->extra_data.value,sizeof(pkCtx->extra_data.value));
		// memcpy(pkCtx->extra_data.value, mbedtls_pk_ec(key)->d.p, (mbedtls_pk_ec(key)->d.n) * (sizeof(mbedtls_mpi_uint)));
		mbedtls_mpi_write_binary(&mbedtls_pk_ec(key)->d, pkCtx->extra_data.value, 32);
		BoatLog(BOAT_LOG_CRITICAL, "sBoatPort_keyCreate_internal_generation 111");
		// 2- update private key format
		pkCtx->prikey_format = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;

		// 3- update private key type
		pkCtx->prikey_type = config->prikey_type;
		pkCtx->extra_data.value_len = (mbedtls_pk_ec(key)->d.n) * (sizeof(mbedtls_mpi_uint));
	}

	// 4- update private key index
	// This field should update by 'key secure storage'(such as TE/SE).
	// When algorithms are implemented by software, this field is default to 0, means
	// that ignore this field.
	pkCtx->prikey_index  = 0;

	// 5- update public key
	pkCtx->pubkey_format = BOAT_WALLET_PUBKEY_FORMAT_NATIVE;
	mbedtls_mpi_write_binary(&mbedtls_pk_ec(key)->Q.X, &pkCtx->pubkey_content[0],  32);
    mbedtls_mpi_write_binary(&mbedtls_pk_ec(key)->Q.Y, &pkCtx->pubkey_content[32], 32);

	// clear
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_pk_free(&key);
	
    return result;
}

static BOAT_RESULT sBoatPort_keyCreate_external_injection_pkcs(const BoatWalletPriKeyCtx_config *config, 
															   BoatWalletPriKeyCtx *pkCtx)
{
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_pk_context mbedtls_pkCtx;
	// mbedtls_ecdsa_context *ecPrikey = NULL;
	mbedtls_entropy_context entropy;
	BOAT_RESULT result = BOAT_SUCCESS;

	// 0- check input parameter
	if ((config == NULL) || (config->prikey_content.field_ptr == NULL) || (pkCtx == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "input parameter can not be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	mbedtls_pk_init(&mbedtls_pkCtx);
	if (config->prikey_format == BOAT_WALLET_PRIKEY_FORMAT_PKCS)
	{
		result = mbedtls_pk_parse_key(&mbedtls_pkCtx, config->prikey_content.field_ptr,
									  config->prikey_content.field_len, NULL, 0);
		if (result != BOAT_SUCCESS)
		{
			mbedtls_pk_free(&mbedtls_pkCtx);
			BoatLog(BOAT_LOG_CRITICAL, "Error: pkcs key prase failed.");
			return BOAT_ERROR;
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
	pkCtx->prikey_format = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	
	// 3- update private key type
	pkCtx->prikey_type   = config->prikey_type;

	// 4- update private key index
	// This field should update by 'key secure storage'(such as TE/SE).
	// When algorithms are implemented by software, this field is default to 0, means
	// that ignore this field.
	pkCtx->prikey_index  = 0; 

		// 5- update public key
		// Ethereum series need NATIVE public key to calculate account address
		pkCtx->pubkey_format = BOAT_WALLET_PUBKEY_FORMAT_NATIVE;
		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->Q.X, &pkCtx->pubkey_content[0], 32);
		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->Q.Y, &pkCtx->pubkey_content[32], 32);
	}else if (config->prikey_format == BOAT_WALLET_PRIKEY_FORMAT_NATIVE){
		// BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
		// boat_throw(BOAT_ERROR, BoatSignature_exception);
		mbedtls_ctr_drbg_init(&ctr_drbg);
		mbedtls_entropy_init(&entropy);
		result += mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
		result += mbedtls_pk_setup(&mbedtls_pkCtx, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));
		if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256K1)
		{
			BoatLog(BOAT_LOG_CRITICAL, "begin mbedtls_ecp_read_key ");
			// ecPrikey = BoatCalloc(1, sizeof(mbedtls_ecp_keypair));
			// if (ecPrikey != NULL)
			// 	mbedtls_ecp_keypair_init(ecPrikey);
			result = mbedtls_ecp_read_key(MBEDTLS_ECP_DP_SECP256K1,  mbedtls_pk_ec(mbedtls_pkCtx), config->prikey_content.field_ptr, config->prikey_content.field_len);
			BoatLog(BOAT_LOG_CRITICAL, "mbedtls_ecp_read_key result = %x ", result);
		}
		else if (config->prikey_type == BOAT_WALLET_PRIKEY_TYPE_SECP256R1)
		{
			// ecPrikey = BoatCalloc(1, sizeof(mbedtls_ecp_keypair));
			// if (ecPrikey != NULL)
			// 	mbedtls_ecp_keypair_init(ecPrikey);
			result = mbedtls_ecp_read_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(mbedtls_pkCtx), config->prikey_content.field_ptr, config->prikey_content.field_len);
			BoatLog(BOAT_LOG_CRITICAL, "mbedtls_ecp_read_key result = %x ", result);
		}
		else
		{
			BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
			// boat_throw(BOAT_ERROR, BoatSignature_exception);
		}

		mbedtls_ecp_mul( mbedtls_pk_ec(mbedtls_pkCtx), &mbedtls_pk_ec(mbedtls_pkCtx)->Q, &mbedtls_pk_ec(mbedtls_pkCtx)->d, &mbedtls_pk_ec(mbedtls_pkCtx)->grp.G, mbedtls_ctr_drbg_random, &ctr_drbg );
		mbedtls_ctr_drbg_free(&ctr_drbg);
		mbedtls_entropy_free(&entropy);

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
		pkCtx->prikey_type = config->prikey_type;

		// 4- update private key index
		// This field should update by 'key secure storage'(such as TE/SE).
		// When algorithms are implemented by software, this field is default to 0, means
		// that ignore this field.
		pkCtx->prikey_index = 0;


		pkCtx->pubkey_format = BOAT_WALLET_PUBKEY_FORMAT_NATIVE;
		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->Q.X, &pkCtx->pubkey_content[0], 32);
		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->Q.Y, &pkCtx->pubkey_content[32], 32);

	}else{
		BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
		// boat_throw(BOAT_ERROR, BoatSignature_exception);
	}

	/* free */
	mbedtls_pk_free(&mbedtls_pkCtx);

    return result;
}


BOAT_RESULT  BoatPort_keyCreate(const BoatWalletPriKeyCtx_config *config, BoatWalletPriKeyCtx *pkCtx)
{
	BOAT_RESULT  result = BOAT_SUCCESS;
	
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
				BoatLog(BOAT_LOG_VERBOSE, "wallet private key[pkcs] set...");
				result = sBoatPort_keyCreate_external_injection_pkcs(config, pkCtx);
				break;
			case BOAT_WALLET_PRIKEY_FORMAT_NATIVE:
			case BOAT_WALLET_PRIKEY_FORMAT_MNEMONIC:
				// BoatLog(BOAT_LOG_NORMAL, "NOT SUPPORT FORMAT YET.");
				// result = BOAT_ERROR;
				BoatLog(BOAT_LOG_VERBOSE, "wallet private key[native] set...");
				result = sBoatPort_keyCreate_external_injection_pkcs(config, pkCtx);
				break;
				break;
			default:
				BoatLog(BOAT_LOG_CRITICAL, "invalid private key format.");
				result = BOAT_ERROR_WALLET_KEY_FORMAT_ERR;
				break;
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "invalid private key format.");
		result = BOAT_ERROR_WALLET_KEY_GENMODE_ERR;
	}

    return result;
}

BOAT_RESULT  BoatPort_keyQuery(const BoatWalletPriKeyCtx_config *config, BoatWalletPriKeyCtx *pkCtx)
{	
	//! @todo to be implemented
	return BOAT_ERROR;
}

BOAT_RESULT  BoatPort_keyDelete(BoatWalletPriKeyCtx *pkCtx)
{
	//! @todo to be implemented
	return BOAT_ERROR;
}


/******************************************************************************
                              BOAT AES WARPPER
*******************************************************************************/
BOAT_RESULT  BoatAesEncrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
	mbedtls_aes_context  ctx;
	BUINT8  saltArrayTmp[16];
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

BOAT_RESULT  BoatAesDecrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
	mbedtls_aes_context  ctx;
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
