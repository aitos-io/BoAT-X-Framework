/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-13 16:37:32
 * @LastEditors: zt222 tao.zheng@aitos.io
 * @LastEditTime: 2022-10-24 20:44:16
 */
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
#include "boatiotsdk.h"
#include "boat_keystore_soft.h"
#include "boatkeypair.h"
#include "persiststore.h"

/**
 * @description:
 * 	This function get prikey by index from Nvram.
 * @param[in] {BUINT8} index
 * @param[out] {BoatKeypairExtraData} *prikey
 * @return {*}
 *   This function returns BOAT_SUCCESS if success.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
static BOAT_RESULT BoAT_GetPirkeyByIndex(BUINT8 index, BoatKeypairExtraData *prikey)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 prikeyNum = 0, prikeyIndex = 0;
    BUINT32 offset = 0;
    BUINT32 paramLength = 0, paramLengthLen = 0;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 prikeynumBytes[4] = {0};
    BUINT8 ciphertext[BOAT_KEYSTORE_PRIKEY_LEN] = {0};
    BoatStoreType storetype;
    if (prikey == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "prikey must not be NULL");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (index == 0)
    { // onetime keypair
        storetype = BOAT_STORE_TYPE_RAM;
    }
    else
    {
        storetype = BOAT_STORE_TYPE_FLASH;
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), storetype);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
    result = utility_check_NumBytes(prikeynumBytes, &prikeyNum);
    BoatLog(BOAT_LOG_NORMAL, "prikey num = %d", prikeyNum);
    /* if read Nvram failed , no keypair */
    if (result != BOAT_SUCCESS || prikeyNum == 0)
    {
        return BOAT_ERROR;
    }
    offset += sizeof(prikeynumBytes);
    for (BUINT8 i = 0; i < prikeyNum; i++)
    {
        /* prikey index */
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storetype);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] index length fail ", i);
            return result;
        }
        paramLength = UtilityGetLVData_L(lengthBytes);
        if (paramLength != 1)
        {
            BoatLog(BOAT_LOG_NORMAL, "prikey index length err ");
            return BOAT_ERROR;
        }
        paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
        offset += paramLengthLen;
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, &prikeyIndex, sizeof(prikeyIndex), storetype);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] index fail ", i);
            return result;
        }
        BoatLog(BOAT_LOG_NORMAL, "prikeyindex = %d , index = %d", prikeyIndex, index);
        offset += paramLength;
        if (prikeyIndex == index)
        {
            /* begin read prikey chiptext */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storetype);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] data length fail ", i);
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength > BOAT_KEYSTORE_PRIKEY_LEN)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey data length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += paramLengthLen;
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, ciphertext, paramLength, storetype);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] data fail ", i);
                return result;
            }
            /*  dec */
            result = BoATSoftRotNvramDec(ciphertext, paramLength, &(prikey->value[0]), &(prikey->value_len));
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "dec keypair[%d] prikey fail ", i);
                return result;
            }
            return result;
        }
        else
        {
            /* prikey data */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storetype);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] index length fail ", i);
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength < 1)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey index length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += (paramLengthLen + paramLength);
        }
    }
    return BOAT_ERROR;
}

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
BOAT_RESULT BoAT_Keystore_Gen_Keypair(BoatKeypairPriKeyType type, BoatKeypairKeypair *keypair)
{
    return BoAT_Keypair_generation(type, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE, keypair);
}

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
BOAT_RESULT BoAT_Keystore_Sign(BoatKeypairPriKeyType type, BUINT8 prikeyIndex, const BUINT8 *digest, BUINT32 digestLen, BUINT8 *signature, BUINT32 *signatureLen, BUINT8 *Prefix)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairExtraData prikey;
    result = BoAT_GetPirkeyByIndex(prikeyIndex, &prikey);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "get prikey by index fail");
        return result;
    }
    result = BoAT_sign(type, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE, prikey.value, prikey.value_len, digest, digestLen, signature, signatureLen, Prefix);
    return result;
}

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
BOAT_RESULT BoAT_Keystore_store_prikey(BUINT8 keypairIndex, BUINT8 *prikey, BUINT32 prikeyLen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 prikeynumBytes[4] = {0};
    BUINT8 storebuf[4 + BOAT_KEYSTORE_PRIKEY_LEN] = {0};
    BUINT8 index = 0, prikeyNum = 0;
    BUINT32 offset = 0, storeOffset = 0;
    BUINT32 paramLength = 0, paramLengthLen = 0, prikeyChipLen = 0;
    BoatStoreType storeType;
    if (keypairIndex == 0)
    {
        storeType = BOAT_STORE_TYPE_RAM;
    }
    else
    {
        storeType = BOAT_STORE_TYPE_FLASH;
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "read prikey num fail,ret = %d ", result);
        // return result;
        prikeyNum = 0;
    }
    else
    {
        result = utility_check_NumBytes(prikeynumBytes, &prikeyNum);
        if (result != BOAT_SUCCESS)
        {
            // BoatLog(BOAT_LOG_NORMAL,"check prikey num fail");
            // return result;
            prikeyNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    BoatLog(BOAT_LOG_NORMAL, "read prikey num  = %d ", prikeyNum);
    if (prikeyNum >= BOAT_MAX_KEYPAIR_NUM)
    {
        return BOAT_ERROR;
    }
    if (BOAT_STORE_TYPE_RAM == storeType)
    {
        prikeyNum = 0;
    }
    offset += sizeof(prikeynumBytes);
    for (size_t i = 0; i < prikeyNum; i++)
    {
        /* index */
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storeType);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "read prikey index lengthbytes fail ,ret = %d ", result);
            return result;
        }
        paramLength = UtilityGetLVData_L(lengthBytes);
        if (paramLength != 1)
        {
            BoatLog(BOAT_LOG_NORMAL, "prikey index length err ");
            return BOAT_ERROR;
        }
        paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
        // offset += paramLengthLen;
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset + paramLengthLen, &index, sizeof(index), storeType);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "read prikey index fail,ret = %d ", result);
            return result;
        }
        BoatLog(BOAT_LOG_NORMAL, "prikey index = %d , keypairIndex = %d ", index, keypairIndex);
        if (index == keypairIndex)
        {
            BoatLog(BOAT_LOG_NORMAL, "keypair index already exist ");
            return BOAT_ERROR;
        }
        offset += (paramLengthLen + paramLength);
        /* prikey data */
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storeType);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "read prikey lengthbytes fail,ret = %d ", result);
            return result;
        }
        paramLength = UtilityGetLVData_L(lengthBytes);
        if (paramLength < 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "prikey data length err ");
            return BOAT_ERROR;
        }
        paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
        offset += (paramLengthLen + paramLength);
    }
    storebuf[storeOffset] = keypairIndex;
    result = add_L_withOffset(storebuf, &storeOffset, 1);
    if (result < BOAT_SUCCESS)
    {
        return result;
    }
    result = BoATSoftRotNvramEnc(prikey, prikeyLen, storebuf + storeOffset, &prikeyChipLen);
    if (result != BOAT_SUCCESS || prikeyChipLen > BOAT_KEYSTORE_PRIKEY_LEN)
    {
        return BOAT_ERROR;
    }
    result = add_L_withOffset(storebuf, &storeOffset, prikeyChipLen);
    if (result < BOAT_SUCCESS)
    {
        return result;
    }
    result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset, storebuf, storeOffset, storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "store prikey data err , ret = %d ", result);
        return result;
    }
    prikeyNum++;
    offset = 0;
    utility_get_NumBytes(prikeyNum, prikeynumBytes);
    result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "store prikey data err ");
    }
    return result;
}

/**
 * @description:
 *  This cunction delete prikey by index.
 * @param {BUINT8} index
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 */
BOAT_RESULT BoAT_DeletePirkeyByIndex(BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 i = 0;
    BUINT8 prikeyNum = 0, prikeyNumNew = 0, prikeyIndex = 0;
    BUINT32 offset = 0, offset_moveFrom = 0, offset_moveTo = 0;
    BUINT32 prikeyLength = 0, paramLength = 0, paramLengthLen = 0;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 prikeynumBytes[4] = {0};
    BUINT8 *prikeyData = NULL;
    boat_try_declare;
    if (index > BOAT_MAX_KEYPAIR_NUM)
    {
        return BOAT_ERROR;
    }
    /* onetime prikey
       index of onetime prikey must be 0
    */
    if (index == 0)
    {
        /* set prikey num of onetime prikey to 0 */
        memset(prikeynumBytes, 0x00, sizeof(prikeynumBytes));
        result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, 0, prikeynumBytes, sizeof(prikeynumBytes), BOAT_STORE_TYPE_RAM);
        return result;
    }
    else
    { // persistent prikey
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), BOAT_STORE_TYPE_FLASH);
        /* if read Nvram failed , no keypair */
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        result = utility_check_NumBytes(prikeynumBytes, &prikeyNum);
        if (result != BOAT_SUCCESS || prikeyNum == 0)
        {
            return result;
        }
        offset += sizeof(prikeynumBytes);
        for (i = 0; i < prikeyNum; i++)
        {
            /* index */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read prikey index lengthbytes fail ,ret = %d ", result);
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength != 1)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey index length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            // offset += keypairLengthLen;
            /* keypair index  */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset + paramLengthLen, &prikeyIndex, sizeof(prikeyIndex), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read keypair index err ");
                return result;
            }
            if (prikeyIndex == index)
            {
                break;
            }
            offset += (paramLengthLen + paramLength);
            /* prikey data */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read prikey lengthbytes fail,ret = %d ", result);
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength < 0)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey data length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += (paramLengthLen + paramLength);
        }
        BoatLog(BOAT_LOG_NORMAL, "i= %d ", i);
        if (i >= prikeyNum)
        {
            BoatLog(BOAT_LOG_NORMAL, "not find the prikey ");
            return BOAT_ERROR;
        }
        prikeyNumNew = prikeyNum - 1;
        utility_get_NumBytes(prikeyNumNew, prikeynumBytes);
        result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, 0, prikeynumBytes, sizeof(prikeynumBytes), BOAT_STORE_TYPE_FLASH); // only need to reset keypair length bytes
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "delete keypair fail ");
            return result;
        }

        if (i == (prikeyNum - 1))
        { /* last one keypair*/
            memset(lengthBytes, 0x00, sizeof(lengthBytes));
            result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH); // only need to reset keypair length bytes
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "delete keypair fail");
                boat_throw(BOAT_ERROR, updataPrikeyData_exception);
            }
        }
        else
        {
            offset_moveTo = offset;
            // the next keypair
            offset += (paramLengthLen + paramLength);
            /* prikey content*/
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read prikey lengthbytes fail,ret = %d ", result);
                boat_throw(result, updataPrikeyData_exception);
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength < 0)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey data length err ");
                boat_throw(BOAT_ERROR, updataPrikeyData_exception);
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += (paramLengthLen + paramLength);
            offset_moveFrom = offset;
            i++;
            prikeyLength = 0;
            for (; i < prikeyNum; i++)
            {
                /* index */
                result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
                if (result != BOAT_SUCCESS)
                {
                    boat_throw(result, updataPrikeyData_exception);
                }
                paramLength = UtilityGetLVData_L(lengthBytes);
                if (paramLength < 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, "keypair data length err ");
                    boat_throw(BOAT_ERROR, updataPrikeyData_exception);
                }
                paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
                offset += (paramLengthLen + paramLength);
                prikeyLength += (paramLengthLen + paramLength);
                result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
                if (result != BOAT_SUCCESS)
                {
                    boat_throw(result, updataPrikeyData_exception);
                }
                paramLength = UtilityGetLVData_L(lengthBytes);
                if (paramLength < 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, "keypair data length err ");
                    boat_throw(BOAT_ERROR, updataPrikeyData_exception);
                }
                paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
                offset += (paramLengthLen + paramLength);
                prikeyLength += (paramLengthLen + paramLength);
            }
            // all the keypairs'length after index keypair
            prikeyData = BoatMalloc(prikeyLength);
            if (NULL == prikeyData)
            {
                BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory ");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, updataPrikeyData_exception);
            }
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset_moveFrom, prikeyData, prikeyLength, BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatFree(prikeyData);
                BoatLog(BOAT_LOG_NORMAL, "read keypair data fail ");
                boat_throw(BOAT_ERROR_STORAGE_FILE_READ_FAIL, updataPrikeyData_exception);
            }
            result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset_moveTo, prikeyData, prikeyLength, BOAT_STORE_TYPE_FLASH);
            BoatFree(prikeyData);
            if (result != BOAT_SUCCESS)
            {
                /* recover keypairNum */
                boat_throw(BOAT_ERROR_STORAGE_FILE_WRITE_FAIL, updataPrikeyData_exception);
            }
        }
    }

    //! boat catch handle
    boat_catch(updataPrikeyData_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
        /* recover prikeyNum */
        utility_get_NumBytes(prikeyNum, prikeynumBytes);
        offset = 0;
        result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), BOAT_STORE_TYPE_FLASH);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "delete keypair fail ");
            return result;
        }
    }
    return BOAT_SUCCESS;
}
