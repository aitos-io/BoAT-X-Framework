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
#include <stdlib.h>
#include <time.h>
#include "boatplatform.h"
#include "boattypes.h"
#include "boatutility.h"
#include "boatlog.h"

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

//#include <sys/time.h>


BOAT_RESULT BoatHash(const BoatHashAlgType type, const BUINT8* input, BUINT32 inputLen,
				     BUINT8* hashed, BUINT8* hashedLen, void* rsvd)
{
	BOAT_RESULT result = BOAT_SUCCESS;

	/* input param check */
	if (hashed == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'hashed' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	if (type == BOAT_HASH_KECCAK256)
	{
		keccak_256(input, inputLen, hashed);
		if (hashedLen != NULL)
		{
			*hashedLen = 32;
		}
	}
	else if (type == BOAT_HASH_SHA256)
	{
		mbedtls_sha256(input, inputLen, hashed, 0);
		if (hashedLen != NULL)
		{
			*hashedLen = 32;
		}
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "unknown boat hash algorithm type.");
		result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	return result;
}



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
	{
        MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(x, use_size * 8 - grp->nbits));
	}

    /* While at it, reduce modulo N */
    if (mbedtls_mpi_cmp_mpi(x, &grp->N) >= 0)
	{
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(x, x, &grp->N));
	}

cleanup:
    return(ret);
}


/*
 * Get the type of a curve
 */
mbedtls_ecp_curve_type boat_mbedtls_ecp_get_type(const mbedtls_ecp_group *grp)
{
    if (grp->G.X.p == NULL)
	{
        return(MBEDTLS_ECP_TYPE_NONE);
	}

    if (grp->G.Y.p == NULL)
	{
        return(MBEDTLS_ECP_TYPE_MONTGOMERY);
	}
    else
	{
        return(MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS);
	}
}

/*
 * Read a private key.
 */
int boat_mbedtls_ecp_read_key(mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair *key,
                          	  const unsigned char *buf, size_t buflen)
{
    int ret = 0;

    if (key == NULL)
	{
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}
    if (buf == NULL)
	{
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}

    if ((ret = mbedtls_ecp_group_load(&key->grp, grp_id)) != 0)
	{
        return(ret);
	}

    ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;

#if defined(ECP_MONTGOMERY)
    if (mbedtls_ecp_get_type(&key->grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
    {
        /*
         * If it is Curve25519 curve then mask the key as mandated by RFC7748
         */
        if (grp_id == MBEDTLS_ECP_DP_CURVE25519)
        {
            if (buflen != ECP_CURVE25519_KEY_SIZE)
			{
                return MBEDTLS_ERR_ECP_INVALID_KEY;
			}

            MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary_le(&key->d, buf, buflen));

            /* Set the three least significant bits to 0 */
            MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(&key->d, 0, 0));
            MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(&key->d, 1, 0));
            MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(&key->d, 2, 0));

            /* Set the most significant bit to 0 */
            MBEDTLS_MPI_CHK(
                    mbedtls_mpi_set_bit(&key->d,
                                        ECP_CURVE25519_KEY_SIZE * 8 - 1, 0)
                   );

            /* Set the second most significant bit to 1 */
            MBEDTLS_MPI_CHK(
                    mbedtls_mpi_set_bit(&key->d,
                                        ECP_CURVE25519_KEY_SIZE * 8 - 2, 1)
                   );
        }
        else
		{
            ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
		}
    }

#endif
// #if defined(ECP_SHORTWEIERSTRASS)
    if (boat_mbedtls_ecp_get_type(&key->grp) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
    {
        MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&key->d, buf, buflen));

        MBEDTLS_MPI_CHK(mbedtls_ecp_check_privkey(&key->grp, &key->d));
    }

// #endif

    if (ret != 0)
	{
        mbedtls_mpi_free(&key->d);
	}

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
    int ret, key_tries, sign_tries,blind_tries;
    mbedtls_ecp_point R;
	mbedtls_mpi r, s;
    mbedtls_mpi k, e, t; //k: temporary random number of signature
	                     //e: convert from hash message
	                     //t: temporary random number to counter side-channel attacks
	int boat_ecdsPrefix;

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    // if (ctx == NULL || !mbedtls_ecdsa_can_do(ctx->grp.id) || ctx->grp.N.p == NULL)
	if (ctx == NULL || ctx->grp.N.p == NULL)
	{
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
	}

    /* Make sure d is in range 1..n-1 */
    if (mbedtls_mpi_cmp_int(&ctx->d, 1) < 0 || mbedtls_mpi_cmp_mpi(&ctx->d, &ctx->grp.N) >= 0)
	{
        return (MBEDTLS_ERR_ECP_INVALID_KEY);
	}

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

            // MBEDTLS_MPI_CHK(mbedtls_ecp_gen_privkey(&ctx->grp, &k, f_rng, p_rng));

            // MBEDTLS_MPI_CHK(mbedtls_ecp_mul_restartable(&ctx->grp, &R, &k, &ctx->grp.G,
			// 				f_rng_blind,
			// 				p_rng_blind,
			// 				NULL));
            // MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&r, &R.X, &ctx->grp.N));
			MBEDTLS_MPI_CHK(mbedtls_ecp_gen_keypair(&ctx->grp, &k, &R, f_rng, p_rng));
            MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&r, &R.X, &ctx->grp.N));
			
			//-----------------------------------------------------------
			// -boat:boat_ecdsPrefix update 
			boat_ecdsPrefix = *(R.Y.p) & 1;
            if (1 == mbedtls_mpi_cmp_mpi(&R.X, &ctx->grp.N))
			{// R.X > N
                boat_ecdsPrefix |= 2;
            }
			//-----------------------------------------------------------
        }
        while (mbedtls_mpi_cmp_int(&r, 0) == 0);

        /*
         * Step 5: derive MPI from hashed message
         */
        MBEDTLS_MPI_CHK(derive_mpi(&ctx->grp, &e, digest, digestLen));

        /*
         * Generate a random value to blind inv_mod in next step,
         * avoiding a potential timing leak.
         */
        // MBEDTLS_MPI_CHK(mbedtls_ecp_gen_privkey(&ctx->grp, &t, f_rng_blind,
		// 				p_rng_blind));
		blind_tries = 0;
        do
        {
            size_t n_size = (ctx->grp.nbits + 7) / 8;
            MBEDTLS_MPI_CHK(mbedtls_mpi_fill_random(&t, n_size, f_rng, p_rng));
            MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(&t, 8 * n_size - ctx->grp.nbits));

            /* See mbedtls_ecp_gen_keypair() */
            if (++blind_tries > 30)
                return(MBEDTLS_ERR_ECP_RANDOM_FAILED);
        }
        while (mbedtls_mpi_cmp_int(&t, 1) < 0 ||
               mbedtls_mpi_cmp_mpi(&t, &ctx->grp.N) >= 0);

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
    while (mbedtls_mpi_cmp_int(&s, 0) == 0);

	//-----------------------------------------------------------
	// -boat: boat_ecdsPrefix update
	mbedtls_mpi x;
    mbedtls_mpi_init(&x);
    mbedtls_mpi_mul_int(&x, &s, 2);
	// compare 2S with N
	if (0 < mbedtls_mpi_cmp_mpi(&x, &ctx->grp.N))
	{
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

/**
 * @description: 
 * 	This function get pubkey from prikey;
 * @param {BoatKeypairPriKeyType} type
 * 	now only support ecdsa and will support other alg such as SM
 * @param {BoatKeypairPriKeyFormat} format
 * 	support native and native
 * @param {BUINT8} *prikey
 * 	prikey inut
 * @param {BUINT32} prikeyLen
 * 	length of prikey
 * @param {BUINT8} *pubkey
 * 	pubkey output
 * @param {BUINT32} *pubkeyLen
 * 	length of pubkey
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_getPubkey(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format,BUINT8 *prikey, BUINT32 prikeyLen, BUINT8 *pubkey, BUINT32 *pubkeyLen)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_pk_context mbedtls_pkCtx;
	mbedtls_entropy_context entropy;
	boat_try_declare;
	if(prikey == NULL || pubkey == NULL){
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	mbedtls_pk_init(&mbedtls_pkCtx);

	if (format == BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS)
	{
		result = mbedtls_pk_parse_key(&mbedtls_pkCtx, prikey,
									  prikeyLen, NULL, 0);
		if (result != BOAT_SUCCESS)
		{
			mbedtls_pk_free(&mbedtls_pkCtx);
			BoatLog(BOAT_LOG_CRITICAL, "Error: pkcs key parse failed.");
			boat_throw(BOAT_ERROR, BoatGetPubkey_exception);
		}

		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->Q.X, &pubkey[0], 32);
		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->Q.Y, &pubkey[32], 32);
		*pubkeyLen = 64;
	}
	else if (format == BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE)
	{
		mbedtls_ctr_drbg_init(&ctr_drbg);
		mbedtls_entropy_init(&entropy);
		result += mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
		result += mbedtls_pk_setup(&mbedtls_pkCtx, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));
		if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
		{
			BoatLog(BOAT_LOG_VERBOSE, "begin boat_mbedtls_ecp_read_key ");
			result = boat_mbedtls_ecp_read_key(MBEDTLS_ECP_DP_SECP256K1, mbedtls_pk_ec(mbedtls_pkCtx), prikey, prikeyLen);
			BoatLog(BOAT_LOG_VERBOSE, "boat_mbedtls_ecp_read_key result = %x ", result);
		}
		else if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
		{
			result = boat_mbedtls_ecp_read_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(mbedtls_pkCtx), prikey, prikeyLen);
			BoatLog(BOAT_LOG_VERBOSE, "boat_mbedtls_ecp_read_key result = %x ", result);
		}
		else
		{
			BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
			boat_throw(BOAT_ERROR, BoatGetPubkey_exception);
		}

		mbedtls_ecp_mul(&(mbedtls_pk_ec(mbedtls_pkCtx)->grp), &mbedtls_pk_ec(mbedtls_pkCtx)->Q, &mbedtls_pk_ec(mbedtls_pkCtx)->d, &mbedtls_pk_ec(mbedtls_pkCtx)->grp.G, mbedtls_ctr_drbg_random, &ctr_drbg);
		mbedtls_ctr_drbg_free(&ctr_drbg);
		mbedtls_entropy_free(&entropy);

		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->Q.X, &pubkey[0], 32);
		mbedtls_mpi_write_binary(&mbedtls_pk_ec(mbedtls_pkCtx)->Q.Y, &pubkey[32], 32);
		*pubkeyLen = 64;

	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
		// boat_throw(BOAT_ERROR, BoatGetPubkey_exception);
	}

	/* boat catch handle */
	boat_catch(BoatGetPubkey_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }
	/* free */
	mbedtls_pk_free(&mbedtls_pkCtx);

	return result;
}

/**
 * @description: 
 * 	This function gen keypair .
 * @param {BoatKeypairPriKeyType} type
 * 	now only support ecdsa and will support other alg such as SM
 * @param {BoatKeypairPriKeyFormat} format
 * 	support native and pkcs
 * @param {BoatKeypairKeypair} *keypair
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keypair_generation(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format , BoatKeypairKeypair *keypair)
{
	mbedtls_entropy_context  entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_pk_context       key;
	BOAT_RESULT              result = BOAT_SUCCESS;
	
	if ( keypair == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_pk_init(&key);

	result += mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
    result += mbedtls_pk_setup(&key, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));
	
	if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
	{
		result += mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256K1, mbedtls_pk_ec(key),
									  mbedtls_ctr_drbg_random, &ctr_drbg);
	}
	else if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
	{
		result += mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(key),
									  mbedtls_ctr_drbg_random, &ctr_drbg);
	}
	else
	{
		BoatLog(BOAT_LOG_CRITICAL, "unknown private keytype...");
		result = BOAT_ERROR_WALLET_KEY_TYPE_ERR;
	}

	if (format == BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS)
	{
		// 1- update private key
		memset(keypair->prikey.value, 0, sizeof(keypair->prikey.value));
		result += mbedtls_pk_write_key_pem(&key, keypair->prikey.value, sizeof(keypair->prikey.value));
	}
	else
	{

		// 1- update private key
		memset(keypair->prikey.value, 0, sizeof(keypair->prikey.value));

		mbedtls_mpi_write_binary(&mbedtls_pk_ec(key)->d, keypair->prikey.value, 32);

		// 3- update private key type
		keypair->prikey.value_len = (mbedtls_pk_ec(key)->d.n) * (sizeof(mbedtls_mpi_uint));
	}

	// 5- update public key
	mbedtls_mpi_write_binary(&mbedtls_pk_ec(key)->Q.X, &keypair->pubkey.value[0],  32);
    mbedtls_mpi_write_binary(&mbedtls_pk_ec(key)->Q.Y, &keypair->pubkey.value[32], 32);
	keypair->pubkey.value_len = 64;

	// clear
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_pk_free(&key);
	
    return result;
}

/**
 * @description: 
 * 	This function gen signature by digest.
 * @param[in] {BoatKeypairPriKeyType} type
 * 	support ecdsa now.
 * @param[in] {BoatKeypairPriKeyFormat} format
 * 	support native and pkcs
 * @param[in] {BUINT8} *prikey
 * 	private key
 * @param[in] {BUINT32} prikeylen
 * 	length of prikey
 * @param[in] {BUINT8*} digest
 * @param[in] {BUINT32} digestLen
 * @param[out] {BUINT8 *} signature
 * @param[out] {BUINT32} *signatureLen
 * @param[out] {BUINT8} *Prefix
 * 	recovery factor getting public key from signature. The value must bu 0 or 1.
 * @return {*}
 * @author: aitos
 */
BOAT_RESULT BoAT_sign(BoatKeypairPriKeyType type,BoatKeypairPriKeyFormat format,BUINT8 *prikey,BUINT32 prikeylen ,const BUINT8* digest,BUINT32 digestLen, BUINT8 * signature, BUINT32 *signatureLen , BUINT8 *Prefix)
{
	mbedtls_entropy_context  entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_pk_context       mbedtls_pkCtx;
    mbedtls_ecdsa_context *ecPrikey = NULL;
	BUINT8 signatureTmp[139];
	size_t signatureTmpLen = 0;
	BUINT8 raw_r[32];
	BUINT8 raw_s[32];

	BOAT_RESULT result;
	boat_try_declare;
	
	/* param check */
	if ((digest == NULL) || (signature == NULL))
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_pk_init(&mbedtls_pkCtx);
	
	result = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
	if (result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec mbedtls_ctr_drbg_seed.");
        boat_throw(result, BoatSignature_exception);
    }
	
	// if (format == BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS)
	// {
	// 	/* get prikey content according to prikey index */
	// 	//sBoat_get_prikey_content(prikeyCtx,);
	// 	//! @todo function todo
		
	// 	/* parse is prikey content */
	// 	result = mbedtls_pk_parse_key(&mbedtls_pkCtx, prikey,
	// 								  prikeylen, NULL, 0);
	// 	ecPrikey = mbedtls_pk_ec(mbedtls_pkCtx);
	// }
	// else
	{
		// BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
		// boat_throw(BOAT_ERROR, BoatSignature_exception);
		if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
		{
			BoatLog(BOAT_LOG_CRITICAL, "begin mbedtls_ecp_read_key ");
			ecPrikey = BoatCalloc(1, sizeof(mbedtls_ecp_keypair));
			if (ecPrikey != NULL)
			{
				mbedtls_ecp_keypair_init(ecPrikey);
			}
			result = boat_mbedtls_ecp_read_key(MBEDTLS_ECP_DP_SECP256K1, ecPrikey,prikey, prikeylen);
			BoatLog(BOAT_LOG_CRITICAL, "boat_mbedtls_ecp_read_key result = %x ", result);
		}
		else if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
		{
			ecPrikey = BoatCalloc(1, sizeof(mbedtls_ecp_keypair));
			if (ecPrikey != NULL)
			{
				mbedtls_ecp_keypair_init(ecPrikey);
			}
			result = boat_mbedtls_ecp_read_key(MBEDTLS_ECP_DP_SECP256R1, ecPrikey, prikey, prikeylen);
			BoatLog(BOAT_LOG_CRITICAL, "boat_mbedtls_ecp_read_key result = %x ", result);
		}
		else
		{
			BoatLog(BOAT_LOG_CRITICAL, "UN-SUPPORT PRIVATE KEY FORMAT YET.");
			boat_throw(BOAT_ERROR_WALLET_KEY_TYPE_ERR, BoatSignature_exception);
		}
	}
    if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec mbedtls_pk_parse.");
        boat_throw(result, BoatSignature_exception);
    }

	/* signature process */
	result = Boat_private_ecdsa_sign(ecPrikey, digest, digestLen, signatureTmp, &signatureTmpLen, raw_r, raw_s, 
							 		 mbedtls_ctr_drbg_random, &ctr_drbg,
							  		 mbedtls_ctr_drbg_random, &ctr_drbg, (unsigned char *)&Prefix);

	if (result != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec mbedtls_ecdsa_write_signature.");
		boat_throw(result, BoatSignature_exception);
	}
	
	// signature result assign
	
	memcpy(&signature[0],  raw_r, 32);
	memcpy(&signature[32], raw_s, 32);
	*signatureLen = 64;

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
	if (format != BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS)
	{
		mbedtls_ecp_keypair_free(ecPrikey);
		if (ecPrikey != NULL)
		{
			BoatFree(ecPrikey);
		}
	}
	return result;
}

void *BoatMalloc(size_t size)
{
    return(malloc(size));
}


void *BoatCalloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}


void BoatFree(void *mem_ptr)
{
    free(mem_ptr);
}


void BoatSleep(BUINT32 second)
{
    sleep(second);
}

long int BoatGetTimes()
{
	return time(NULL);
}