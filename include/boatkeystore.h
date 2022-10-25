/*
 * @Description: 
 * @Author: aitos
 * @Date: 2022-09-13 20:26:08
 * @LastEditors: zt222 tao.zheng@aitos.io
 * @LastEditTime: 2022-10-24 19:31:09
 */
#ifndef __BOATKEYSTORE_H__
#define __BOATKEYSTORE_H__
#include "boatiotsdk.h"

/**
 * @description: 
 *  This function store prikey data. Prikey should bind with keypair
 * @param[in] {BUINT8} keypairIndex
 *  current index of keypair
 * @param[in] {BUINT8} *prikey
 *  prikey data
 * @param[in] {BUINT32} prikeyLen
 *  length of prikey
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_store_prikey(BUINT8 keypairIndex,BUINT8 *prikey,BUINT32 prikeyLen);

/**
 * @description: 
 * 	This function gen signature by digest.
 * @param[in] {BoatKeypairPriKeyType} type
 * 	support ecdsa now.
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
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_Sign(BoatKeypairPriKeyType type,BUINT8 prikeyIndex, const BUINT8* digest,BUINT32 digestLen, BUINT8 * signature, BUINT32 *signatureLen , BUINT8 *Prefix);


/**
 * @description: 
 *  This function gen keypair through software
 * @param[in] {BoatKeypairPriKeyType} type
 * @param[out] {BoatKeypairKeypair} *keypair
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_Gen_Keypair(BoatKeypairPriKeyType type,  BoatKeypairKeypair *keypair);

/**
 * @description: 
 *  This cunction delete prikey by index.
 * @param {BUINT8} index
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 */
BOAT_RESULT BoAT_DeletePirkeyByIndex(BUINT8 index);

#endif