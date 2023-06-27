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
#include "persiststore.h"
#include "boatkeystore.h"

#include "fibo_opencpu.h"

#include <sys/time.h>

extern unsigned short boat_htons(unsigned short n);

uint32_t random32(void)
{
    // static uint32_t seed = 0;
    // seed = osiEpochSecond();
    // srand(seed);

    // return seed;
    BUINT8 rand[4] = {0};
    fibo_rng_generate(rand, sizeof(rand));
    return ((rand[0] << 24) | (rand[1] << 16) | (rand[2] << 8) | rand[3]);
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
    BSINT32 file_fd = -1;
    (void)rsvd;

    if (size == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param 'size' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    *size = fibo_file_getSize(BOAT_FILE_STOREDATA);

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
    BSINT32 file_fd = -1;

    BSINT32 count = 0;
    BUINT32 size = 0;
    BUINT8 *buf_zero = NULL;

    (void)rsvd;

    if (writeBuf == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    file_fd = fibo_file_open(BOAT_FILE_STOREDATA, FS_O_RDWR);
    if (file_fd < 0)
    {
        file_fd = fibo_file_open(BOAT_FILE_STOREDATA, FS_O_CREAT);
        fibo_file_close(file_fd);
        file_fd = fibo_file_open(BOAT_FILE_STOREDATA, FS_O_RDWR);
    }
    /* write to file-system */
    // file_ptr = fopen(fileName, "wb");
    if (file_fd < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to create file: %s.", BOAT_FILE_STOREDATA);
        return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
    }

    size = fibo_file_getSize(BOAT_FILE_STOREDATA);
    if (size < offset)
    {
        /*move to the end of the file*/
        fibo_file_seek(file_fd, 0, SEEK_END);
        buf_zero = BoatMalloc(offset - size);
        if (NULL == buf_zero)
        {
            fibo_file_close(file_fd);
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        memset(buf_zero, 0x00, offset - size);
        count = fibo_file_write(file_fd, buf_zero, offset - size);
        BoatFree(buf_zero);
        if (count != (offset - size))
        {
            fibo_file_close(file_fd);
            BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
            return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
        }
        fibo_file_seek(file_fd, 0, SEEK_END);
    }
    else
    {
        if (fibo_file_seek(file_fd, offset, SEEK_SET) < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "fibo_file_seek err ");
        }
    }
    count = fibo_file_write(file_fd, writeBuf, writeLen);
    fibo_file_close(file_fd);
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
    BSINT32 file_fd = -1;

    BSINT32 count = 0;
    BUINT32 size = 0;
    (void)rsvd;

    if ((BOAT_FILE_STOREDATA == NULL) || (readBuf == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "param 'readBuf' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    /* read from file-system */
    file_fd = fibo_file_open(BOAT_FILE_STOREDATA, FS_O_RDONLY);
    if (file_fd >= 0)
    {
        /*move to the end of the file*/
        size = fibo_file_getSize(BOAT_FILE_STOREDATA);
        if (readLen + offset > size)
        {
            fibo_file_close(file_fd);
            return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
        }
        fibo_file_seek(file_fd, offset, SEEK_SET);
        count = fibo_file_read(file_fd, readBuf, readLen);
        fibo_file_close(file_fd);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
        return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
    }

    if (count != readLen)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", BOAT_FILE_STOREDATA);
        return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
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

    if (0 != fibo_file_delete(fileName))
    {
        return BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL;
    }
    else
    {
        return BOAT_SUCCESS;
    }
}

#if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_CHAINMAKER_V1 == 1 || PROTOCOL_USE_CHAINMAKER_V2 == 1 || BOAT_PROTOCOL_USE_HWBCS == 1)
/******************************************************************************
                              BOAT SOCKET WARPPER
                            THIS ONLY USED BY FABRIC OR CHAINMAKER
*******************************************************************************/
#if (BOAT_PROTOCOL_USE_HWBCS == 1)
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

#if (BOAT_TLS_SUPPORT == 1)
    connectfd = fibo_ssl_sock_create();
    if (connectfd == -1)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }
    if (fibo_ssl_sock_connect(connectfd, ip, atoi(port)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error ,code = %d", fibo_get_ssl_errcode());
        fibo_ssl_sock_close(connectfd);
        return -1;
    }
    /// get ssl socket fd
    int fd = fibo_ssl_sock_get_fd(connectfd);

    // set sock fd to unblock
    fibo_sock_lwip_fcntl(fd, F_SETFL, fibo_sock_lwip_fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
    BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, connectfd);
#else
    GAPP_TCPIP_ADDR_T addr;
    if (getHostByIpOrName(address, &addr) != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "getaddrinfo != 0, return HTTPCLIENT_UNRESOLVED_DNS");
        return HTTPCLIENT_UNRESOLVED_DNS;
    }

    addr.sin_port = boat_htons(/*7545*/ client->remote_port);
    if (client->socket < 0)
    {
        return HTTPCLIENT_ERROR_CONN;
    }

    if ((connectfd = fibo_sock_create(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }

    if (fibo_sock_connect(connectfd, &addr) == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error");
        fibo_ssl_sock_close(connectfd);
        return -1;
    }
#endif

    return connectfd;
}

#if (BOAT_TLS_SUPPORT == 1)
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
                        const BoatFieldVariable clientCert, BSINT32 *socketfd, void **tlsContext, void *rsvd)
{

    int ret = 0;

    fibo_set_ssl_chkmode(1);
    ret = fibo_write_ssl_file("TRUSTFILE", caChain.field_ptr, strlen(caChain.field_ptr));
    if (BOAT_TLS_IDENTIFY_CLIENT == 1)
    {
        ret = fibo_write_ssl_file("CAFILE", clientCert.field_ptr, clientCert.field_len);
        ret = fibo_write_ssl_file("CAKEY", clientPrikey.field_ptr, clientPrikey.field_len);
    }
    ret = BoatConnect(address, NULL);
    if (ret == -1)
    {
        BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
        return BOAT_ERROR;
    }
    *socketfd = ret;
    // fibo_taskSleep(10000);
    BoatLog(BOAT_LOG_NORMAL, " BoatTlsInit OK");
    return BOAT_SUCCESS;
}
#endif

BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    //! @todo BOAT_HLFABRIC_TLS_SUPPORT implementation in crypto default.
    BSINT32 ret = fibo_ssl_sock_send(sockfd, buf, len);
    // BoatLog(BOAT_LOG_VERBOSE, "write ssl send = %d ", ret);
    return ret;
#else
    return send(sockfd, buf, len, 0);
#endif
}

static int ssl_recv_unblock(INT32 sock, void *buf, INT32 size, INT32 timeout)
{
    struct timeval tm = {0};
    fd_set rset;
    BUINT8 *temp = buf;

    int fd = fibo_ssl_sock_get_fd(sock);

    int ret = fibo_ssl_sock_recv(sock, buf, size);
    if (ret > 0)
    {
        // BoatLog(BOAT_LOG_VERBOSE, "recv data size:%d", ret);
        return ret;
    }
    else if (ret < 0)
    {
        // BoatLog(BOAT_LOG_VERBOSE, "recv data fail");
        return ret;
    }

    FD_ZERO(&rset);
    FD_SET(fd, &rset);
    tm.tv_sec = timeout / 1000;
    tm.tv_usec = (timeout % 1000) * 1000;

    ret = select(fd + 1, &rset, NULL, NULL, timeout > 0 ? &tm : NULL);
    if (ret < 0)
    {
        BoatLog(BOAT_LOG_VERBOSE, "select failed:%s", strerror(errno));
        return -1;
    }
    else if (ret == 0)
    {
        BoatLog(BOAT_LOG_VERBOSE, "select timeout");
        return -1;
    }
    else
    {
        ret = fibo_ssl_sock_recv(sock, buf, size);
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
    }
}

BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    BSINT32 ret = ssl_recv_unblock(sockfd, buf, len, 10 * 1000);
    return ret;
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
    fibo_ssl_sock_close(sockfd);
#else
    close(sockfd);
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
