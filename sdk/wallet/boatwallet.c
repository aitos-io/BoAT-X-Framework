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

/*!@brief Boatwallet  entry
@author aitos.io
*/
#include "boatconfig.h"
#include "boatinternal.h"
#include "boatwallet.h"

#include "persiststore.h"
#include "cJSON.h"

#if RPC_USE_LIBCURL == 1
#include "curl/curl.h"
#endif


/**
 * @description: 
 *  This function get one wallet data from Nvram include persistent space and non-persistent space
 * @param {BoatWalletDataCtx} *walletdata
 * @param {BUINT32} startAddr
 *  from this addr to read one wallet data
 * @param {BoatStoreType} storeType
 *  read from persistent space or non-persistent space
 * @param {BUINT32*} outlen
 *  length of each wallet data
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATWallet_GetWalletFromNvram(BoatWalletDataCtx *walletdata , BUINT32 startAddr,BoatStoreType storeType,BUINT32 *outlen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 walletDataLength = 0 , walletDataLengthLen = 0 , paramDataLength = 0 , paramDataLengthLen = 0;
    BUINT32 offset = 0 , offset_obj= 0;
    BUINT8 lengthbytes[3] = {0};

    offset = startAddr;
    /*read the data length of each wallet */
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthbytes,sizeof(lengthbytes),storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet length fail , errorcode = %d ",result);
            return result;
        }
        walletDataLength = UtilityGetLVData_L(lengthbytes);
        if(walletDataLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        walletDataLengthLen = UtilityGetTLV_LL_from_len(walletDataLength);
        offset += walletDataLengthLen;
        offset_obj = offset;
        /* begin to read wallet index */
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthbytes,sizeof(lengthbytes),storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet length fail , errorcode = %d ",result);
            return result;
        }
        paramDataLength = UtilityGetLVData_L(lengthbytes);
        if(paramDataLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        if(paramDataLength != 1){
            BoatLog(BOAT_LOG_NORMAL,"wallet index data err ");
            return BOAT_ERROR;
        }
        paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
        offset += paramDataLengthLen;
        if(offset - offset_obj + paramDataLength > walletDataLength ){
            BoatLog(BOAT_LOG_NORMAL,"wallet index data err ");
            return BOAT_ERROR;
        }
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,&(walletdata->prikeyCtx.wallet_index),paramDataLength,storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet index fail , errorcode = %d ",result);
            return result;
        }
        offset += paramDataLength;
        /* begin to read wallet name */
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthbytes,sizeof(lengthbytes),storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet length fail , errorcode = %d ",result);
            return result;
        }
        paramDataLength = UtilityGetLVData_L(lengthbytes);
        if(paramDataLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
        offset += paramDataLengthLen;
        if(offset - offset_obj + paramDataLength > walletDataLength ){
            BoatLog(BOAT_LOG_NORMAL,"wallet name data err ");
            return BOAT_ERROR;
        }
        walletdata->prikeyCtx.wallet_name = BoatMalloc(paramDataLength + 1);
        walletdata->prikeyCtx.wallet_name[paramDataLength] = 0;
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,(BUINT8*)walletdata->prikeyCtx.wallet_name,paramDataLength,storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet index fail , errorcode = %d ",result);
            return result;
        }
        offset += paramDataLength;
        /* read wallet PriKeyFormat */     
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthbytes,sizeof(lengthbytes),storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet length fail , errorcode = %d ",result);
            return result;
        }
        paramDataLength = UtilityGetLVData_L(lengthbytes);
        if(paramDataLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        // if(paramDataLength != 1){
        //     BoatLog(BOAT_LOG_NORMAL,"wallet format data err ");
        //     return BOAT_ERROR;
        // }
        paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
        offset += paramDataLengthLen;
        if(offset - offset_obj + paramDataLength > walletDataLength ){
            BoatLog(BOAT_LOG_NORMAL,"wallet format len err ");
            return BOAT_ERROR;
        }
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,(BUINT8*)&(walletdata->prikeyCtx.prikey_format),paramDataLength,storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet index fail , errorcode = %d ",result);
            return result;
        }
        offset += paramDataLength;    
        /* read wallet PriKeyType */ 
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthbytes,sizeof(lengthbytes),storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet length fail , errorcode = %d ",result);
            return result;
        }
        paramDataLength = UtilityGetLVData_L(lengthbytes);
        if(paramDataLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        // if(paramDataLength != 1){
        //     BoatLog(BOAT_LOG_NORMAL,"wallet key type data err ");
        //     return BOAT_ERROR;
        // }
        paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
        offset += paramDataLengthLen;
        if(offset - offset_obj + paramDataLength > walletDataLength ){
            BoatLog(BOAT_LOG_NORMAL,"wallet key type data err ");
            return BOAT_ERROR;
        }
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,(BUINT8*)&(walletdata->prikeyCtx.prikey_type),paramDataLength,storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet index fail , errorcode = %d ",result);
            return result;
        }
        offset += paramDataLength;
        /* read wallet public key */
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthbytes,sizeof(lengthbytes),storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet length fail , errorcode = %d ",result);
            return result;
        }
        paramDataLength = UtilityGetLVData_L(lengthbytes);
        if(paramDataLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        if(paramDataLength != 64){
            BoatLog(BOAT_LOG_NORMAL,"wallet pubkey data len err , len = %d ",paramDataLength);
            return BOAT_ERROR;
        }
        paramDataLengthLen = UtilityGetTLV_LL_from_len(paramDataLength);
        offset += paramDataLengthLen;
        if(offset - offset_obj + paramDataLength > walletDataLength ){
            BoatLog(BOAT_LOG_NORMAL,"wallet pubkey data err ");
            return BOAT_ERROR;
        }
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,walletdata->prikeyCtx.pubkey_content,paramDataLength,storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"Read wallet index fail , errorcode = %d ",result);
            return result;
        }
        *outlen = (walletDataLength + walletDataLengthLen);

    return result;
}



/**
 * @description: 
 *  This function read wallet data from Nvram by index. Not read prikey content.
 * @param {BoatWalletPriKeyCtx} *PriKeyCtx
 * @param {BUINT8} index
 *  the wallet index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATWallet_GetWalletByIndex(BoatWalletPriKeyCtx *priKeyCtx ,BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
	BUINT8 walletNum = 0 , walletIndex = 0;
	BUINT32 offset = 0 , offset_obj = 0;
	BUINT32  walletLength = 0 , walletLengthLen = 0 , paramLength = 0 , paramLengthLen = 0;
	BUINT8 lengthBytes[3] = {0};
    BoatStoreType storetype;

    if(index == 0){  // onetime wallet
        storetype = BOAT_STORE_TYPE_RAM;
    }else{
        storetype = BOAT_STORE_TYPE_FLASH;
    }

	result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,&walletNum,1,storetype);
    /* if read Nvram failed , no wallet */
    if(result != BOAT_SUCCESS || walletNum == 0){
        BoatLog(BOAT_LOG_NORMAL,"read wallet num ret = %d  num = %d ",result,walletNum);
        return BOAT_ERROR;
    }
	offset ++ ;
	for (BUINT8 i = 0; i < walletNum; i++)
	{
		/* wallet length */
		result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),storetype);
		if(result != BOAT_SUCCESS){
			BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] length fail " , i);
			return result;
		}
		walletLength = UtilityGetLVData_L(lengthBytes);
        if(walletLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        walletLengthLen = UtilityGetTLV_LL_from_len(walletLength);
		offset += walletLengthLen;
		offset_obj = offset;
		/* wallet index */
		result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),storetype);
		if(result != BOAT_SUCCESS){
			BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] index fail " , i);
			return result;
		}
		paramLength = UtilityGetLVData_L(lengthBytes);
		if(paramLength != 1){
            BoatLog(BOAT_LOG_NORMAL,"wallet index length err ");
            return BOAT_ERROR;
        }
		walletIndex = lengthBytes[1];
		if(walletIndex == index){
            priKeyCtx->wallet_index = index;
			offset += 2;
			if(offset - offset_obj > walletLength){  // offset over the length of this walet
                BoatLog(BOAT_LOG_NORMAL,"wallet name offset over wallet length");
				return BOAT_ERROR;
			}
			/* wallet name */
			result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),storetype);
			if(result != BOAT_SUCCESS){
				BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] index fail " , i);
				return result;
			}
			paramLength = UtilityGetLVData_L(lengthBytes);
			if(paramLength < 0){
            	BoatLog(BOAT_LOG_NORMAL,"wallet name length err ");
            	return BOAT_ERROR;
        	}
			paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += paramLengthLen;
            if(offset - offset_obj + paramLength > walletLength){  // offset over the length of this walet
				BoatLog(BOAT_LOG_NORMAL,"wallet name data offset over wallet length");
                return BOAT_ERROR;
			}
            priKeyCtx->wallet_name = BoatMalloc(paramLength);
            if(priKeyCtx->wallet_name == NULL){
                return BOAT_ERROR;
            }
            result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,(BUINT8*)priKeyCtx->wallet_name,paramLength,storetype);
			if(result != BOAT_SUCCESS){
				BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] name  fail " , i);
				return result;
			}
			offset += paramLength ;
			/* wallet PriKeyFormat */
			result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),storetype);
			if(result != BOAT_SUCCESS){
				BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] prikey format fail " , i);
				return result;
			}
			paramLength = UtilityGetLVData_L(lengthBytes);
			if(paramLength < 0){
            	BoatLog(BOAT_LOG_NORMAL,"wallet prike format length err ");
            	return BOAT_ERROR;
        	}
			paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += paramLengthLen;
            if(offset - offset_obj + paramLength > walletLength){  // offset over the length of this walet
				BoatLog(BOAT_LOG_NORMAL,"wallet format offset over wallet length");
                return BOAT_ERROR;
			}
            result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,(BUINT8 *)&(priKeyCtx->prikey_format),paramLength,storetype);
			if(result != BOAT_SUCCESS){
				BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] prikey format  fail " , i);
				return result;
			}
			offset += paramLength ;
			/* wallet prikey type */
			result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),storetype);
			if(result != BOAT_SUCCESS){
				BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] prikey type fail " , i);
				return result;
			}
			paramLength = UtilityGetLVData_L(lengthBytes);
			if(paramLength < 0){
            	BoatLog(BOAT_LOG_NORMAL,"wallet prike type length err ");
            	return BOAT_ERROR;
        	}
			paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += paramLengthLen;
            if(offset - offset_obj + paramLength > walletLength){  // offset over the length of this walet
				BoatLog(BOAT_LOG_NORMAL,"wallet type offset over wallet length");
                return BOAT_ERROR;
			}
            result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,(BUINT8 *)&(priKeyCtx->prikey_type),paramLength,storetype);
			if(result != BOAT_SUCCESS){
				BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] prikey type  fail " , i);
				return result;
			}
			offset += paramLength ;
			/* wallet pubkey */
			result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),storetype);
			if(result != BOAT_SUCCESS){
				BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] pubkey fail " , i);
				return result;
			}
			paramLength = UtilityGetLVData_L(lengthBytes);
			if(paramLength < 0){
            	BoatLog(BOAT_LOG_NORMAL,"wallet pubkey length err ");
            	return BOAT_ERROR;
        	}
			paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += paramLengthLen;
            if(offset - offset_obj + paramLength > walletLength){  // offset over the length of this walet
				BoatLog(BOAT_LOG_NORMAL,"wallet pubkey offset over wallet length");
                return BOAT_ERROR;
			}
            result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,(priKeyCtx->pubkey_content),paramLength,storetype);
			if(result != BOAT_SUCCESS){
				BoatLog(BOAT_LOG_NORMAL,"get wallet[%d] pubkey  fail " , i);
				return result;
			}
			offset += paramLength ;
			return result;
		}else{ // the next wallet
			offset += walletLength;
		}
	}
    return BOAT_ERROR;
}

/**
 * @description: 
 *  This function get wallet list ,include persistent wallets and onetime wallet
 * @param[out] {BoatIotWalletContext} *walletList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATWallet_GetWalletList(BoatIotWalletContext *walletList)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 walletNum = 0;
    BUINT8 index = 0;
    BUINT32 walletDataLength = 0 ;
    BUINT32 offset = 0 ;
    // BUINT8 lengthbytes[3] = {0};
    /* persistent wallet  */
    result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,&walletNum,1,BOAT_STORE_TYPE_FLASH);
    BoatLog(BOAT_LOG_NORMAL,"read flash wallet num = %d ,result = %d",walletNum,result);
    /* if read Nvram failed , no wallet */
    if(result != BOAT_SUCCESS){
        walletNum = 0;
        // return BOAT_SUCCESS;
    }
    (*walletList).walletNum = walletNum;
    offset += 1;
    for (int i = 0; i < walletNum; i++)
    {
        /* code */
        result = BoATWallet_GetWalletFromNvram(&(walletList->wallets[index++]),offset,BOAT_STORE_TYPE_FLASH,&walletDataLength);
        if(result != BOAT_SUCCESS){
            return result;
        }
        offset += walletDataLength;

    }
    /* onetime wallet  */
    offset = 0;
    result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,&walletNum,1,BOAT_STORE_TYPE_RAM);
    if(result != BOAT_SUCCESS){
        walletNum = 0;
        // return BOAT_SUCCESS;
    }
    BoatLog(BOAT_LOG_NORMAL,"read ram wallet num = %d ,result = %d",walletNum,result);
    (*walletList).walletNum += walletNum;
    offset += 1;
    for (int i = 0; i < walletNum; i++)
    {
        /* code */
        result = BoATWallet_GetWalletFromNvram(&(walletList->wallets[index++]),offset,BOAT_STORE_TYPE_RAM,&walletDataLength);
        if(result != BOAT_SUCCESS){
            return result;
        }
        offset += walletDataLength;

    }

    return result;
}

/**
 * @description: 
 *  This function use to free BoatIotWalletContext param.
 * @param[in] {BoatIotWalletContext} walletList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATWallet_FreeWalletContext(BoatIotWalletContext walletList)
{
    if(walletList.walletNum == 0){
        return BOAT_SUCCESS;
    }
    for (int i = 0; i < walletList.walletNum; i++)
    {
        /* code */
        walletList.wallets[i].prikeyCtx.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_UNKNOWN;
        walletList.wallets[i].prikeyCtx.prikey_type = BOAT_WALLET_PRIKEY_TYPE_UNKNOWN;
        memset(walletList.wallets[i].prikeyCtx.pubkey_content,0x00,sizeof(walletList.wallets[i].prikeyCtx.pubkey_content));
        walletList.wallets[i].prikeyCtx.wallet_index = 0;
        if(walletList.wallets[i].prikeyCtx.wallet_name != NULL){
            BoatFree(walletList.wallets[i].prikeyCtx.wallet_name);
        }
    }
    walletList.walletNum = 0;
    return BOAT_SUCCESS;
    
}

/**
 * @description: 
 *  This function initialize wallet list
 * @param[inout] {BoatWalletDataCtx} *mWalletDataCtx
 * @return 
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATWallet_Init_BoatIotWalletContext(BoatIotWalletContext *mWalletList){
    mWalletList->walletNum = 0;
    for (int i = 0; i < BOAT_MAX_WALLET_NUM; i++)
    {
        /* code */
        mWalletList->wallets[i].extraData.value_len = 0;
        memset(mWalletList->wallets[i].extraData.value,0x00,sizeof(mWalletList->wallets[i].extraData.value));
        mWalletList->wallets[i].prikeyCtx.wallet_index = 0;
        mWalletList->wallets[i].prikeyCtx.wallet_name = NULL;
    }
    
    return BOAT_SUCCESS;
}



/**
 * @description: 
 *  This function get unused wallet index from persistent wallets.
 * @return {*}
 *  This function returns unused wallet index if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATWallet_GetFreeIndex_From_Persistent(void)
{
    BOAT_RESULT result = BOAT_ERROR;
    // BUINT8 flag = 0;
    int i ,j;
    BoatIotWalletContext walletList;
    BoATWallet_Init_BoatIotWalletContext(&walletList);
    result = BoATWallet_GetWalletList(&walletList);
    if(result != BOAT_SUCCESS){
        // BoATWallet_FreeWalletContext(walletList);
        // return result;
        BoatLog(BOAT_LOG_NORMAL,"get wallet list fail");
    }
    if(walletList.walletNum == BOAT_MAX_WALLET_NUM){
        BoATWallet_FreeWalletContext(walletList);
        return BOAT_ERROR;
    }
    for (i = 0; i < BOAT_MAX_WALLET_NUM; i++)
    {
        /* code */
        for ( j = 0; j < walletList.walletNum; j++)
        {
            /* code */
            if(walletList.wallets[j].prikeyCtx.wallet_index == (i+1))
            {
                // BoATWallet_FreeWalletContext(walletList);
                // return i;
                break;
            }
        }
        if(j == walletList.walletNum){
            BoATWallet_FreeWalletContext(walletList);
            return i+1;
        }
    }
    BoATWallet_FreeWalletContext(walletList);
    if(walletList.walletNum == 0){
        BoatLog(BOAT_LOG_NORMAL,"have no wallet");
        return 0;
    }
    return BOAT_ERROR;
}

/**
 * @description: 
 *  This function get the length of wallet data. 
 *  data like this:
 *  L  LV LV LV 
 * @param[in] {BoatWalletDataCtx} *mWalletDataCtx
 * @param[out] {BUINT32} *len
 *  length of all the wallet data
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATWallet_Get_DataCtx_Len(BoatWalletDataCtx *mWalletDataCtx,BUINT32 *len)
{
    BUINT32 paramLength = 0,paramLengthLen = 0;
    BUINT32 walletLength = 0;
    if(mWalletDataCtx == NULL){
        BoatLog(BOAT_LOG_NORMAL,"mWalletDataCtx is NULL");
        return BOAT_ERROR;
    }
    if(mWalletDataCtx->prikeyCtx.wallet_name == NULL){
        BoatLog(BOAT_LOG_NORMAL,"wallet name  is NULL");
        return BOAT_ERROR;
    }
    /* wallet index*/
    paramLength = sizeof(mWalletDataCtx->prikeyCtx.wallet_index);
    paramLengthLen =   UtilityGetTLV_LL_from_len(paramLength);
    walletLength += (paramLength + paramLengthLen);
    /*wallet name*/
    paramLength = strlen(mWalletDataCtx->prikeyCtx.wallet_name);
    paramLengthLen  = UtilityGetTLV_LL_from_len(paramLength);
    walletLength += (paramLength + paramLengthLen);
    /* wallet PriKeyFormat */
    paramLength = sizeof(mWalletDataCtx->prikeyCtx.prikey_format);
    paramLengthLen  = UtilityGetTLV_LL_from_len(paramLength);
    walletLength += (paramLength + paramLengthLen);
    /* wallet prikey type*/
    paramLength = sizeof(mWalletDataCtx->prikeyCtx.prikey_type);
    paramLengthLen  = UtilityGetTLV_LL_from_len(paramLength);
    walletLength += (paramLength + paramLengthLen);
    /* wallet pubkey */
    paramLength = sizeof(mWalletDataCtx->prikeyCtx.pubkey_content);
    paramLengthLen  = UtilityGetTLV_LL_from_len(paramLength);
    walletLength += (paramLength + paramLengthLen);
    /* wallet prikey */
    //*data_len + 31 + 32 + 16 + 16
    paramLength = mWalletDataCtx->extraData.value_len + 31 + 32 + 16 + 16;
    paramLengthLen  = UtilityGetTLV_LL_from_len(paramLength);
    walletLength += (paramLength + paramLengthLen);
    /* all the data*/
    paramLengthLen  = UtilityGetTLV_LL_from_len(walletLength);
    walletLength +=  paramLengthLen;
    BoatLog(BOAT_LOG_NORMAL,"walletLength = %d ",walletLength);
    *len = walletLength;
    return BOAT_SUCCESS;
}

/**
 * @description: 
 *  This function get all the data of wallet.
 *  like this:
 *  L  LV LV LV 
 * @param {BoatWalletDataCtx} *mWalletDataCtx
 * @param {BUINT8} *data
 * wallet data
 * @param[in/out] {BUINT32} *datalen
 * in: length of data
 * out: real length of wallet data
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATWallet_Get_DataCtx_Data(BoatWalletDataCtx *mWalletDataCtx,BUINT8 *data,BUINT32 *datalen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 paramLength = 0;
    BUINT32 walletLength = 0,offset = 0;
    if(mWalletDataCtx == NULL){
        BoatLog(BOAT_LOG_NORMAL,"wallet data is NULL");
        return BOAT_ERROR;
    }
    if(data == NULL ){
        BoatLog(BOAT_LOG_NORMAL,"out data  buf is NULL");
        return BOAT_ERROR;
    }

    /* wallet index*/
   data[offset] = mWalletDataCtx->prikeyCtx.wallet_index;
   result = add_L_withOffset(data,&offset,sizeof(mWalletDataCtx->prikeyCtx.wallet_index));
   if(result < BOAT_SUCCESS){
    BoatLog(BOAT_LOG_NORMAL,"get wallet index LV fail");
    return BOAT_ERROR;
   }
    /*wallet name*/
    memcpy(data+offset,mWalletDataCtx->prikeyCtx.wallet_name,strlen(mWalletDataCtx->prikeyCtx.wallet_name));
    result = add_L_withOffset(data,&offset,strlen(mWalletDataCtx->prikeyCtx.wallet_name));
    if(result < BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"get wallet name LV fail");
        return BOAT_ERROR;
    }
    /* wallet PriKeyFormat */
    // data[offset] = mWalletDataCtx->prikeyCtx.prikey_format;
    memcpy(data+offset,&(mWalletDataCtx->prikeyCtx.prikey_format),sizeof(mWalletDataCtx->prikeyCtx.prikey_format));
    result = add_L_withOffset(data,&offset,sizeof(mWalletDataCtx->prikeyCtx.prikey_format));
    if(result < BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"get wallet format LV fail");
    return BOAT_ERROR;
    }
    /* wallet prikey type*/
    // data[offset] = mWalletDataCtx->prikeyCtx.prikey_type;
    memcpy(data+offset,&(mWalletDataCtx->prikeyCtx.prikey_type),sizeof(mWalletDataCtx->prikeyCtx.prikey_type));
    result = add_L_withOffset(data,&offset,sizeof(mWalletDataCtx->prikeyCtx.prikey_type));
    if(result < BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"get wallet type LV fail");
    return BOAT_ERROR;
    }
    /* wallet pubkey */
    memcpy(data + offset , mWalletDataCtx->prikeyCtx.pubkey_content,sizeof(mWalletDataCtx->prikeyCtx.pubkey_content));
    result = add_L_withOffset(data,&offset,sizeof(mWalletDataCtx->prikeyCtx.pubkey_content));
    if(result < BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"get wallet pubkey LV fail");
    return BOAT_ERROR;
    }
    /* wallet prikey */
    BUINT8 dataEnc[mWalletDataCtx->extraData.value_len + 31 + 32 + 16 + 16];
    BoatLog_hexdump(BOAT_LOG_NORMAL,"prikey : ",mWalletDataCtx->extraData.value,mWalletDataCtx->extraData.value_len);
    result = BoATSoftRotNvramEnc(mWalletDataCtx->extraData.value,mWalletDataCtx->extraData.value_len,dataEnc,&paramLength);
    BoatLog_hexdump(BOAT_LOG_NORMAL,"prikey cip : ",dataEnc,paramLength);
    memcpy(data + offset , dataEnc,paramLength);
    result = add_L_withOffset(data,&offset,paramLength);
    if(result < BOAT_SUCCESS){
    BoatLog(BOAT_LOG_NORMAL,"get wallet prikey LV fail");
    return BOAT_ERROR;
    }
    /* all the data*/
    walletLength = offset;
    offset = 0;
    result = add_L_withOffset(data,&offset,walletLength);
    if(result < BOAT_SUCCESS){
    return BOAT_ERROR;
    }
    *datalen = offset;
    return BOAT_SUCCESS;
}


/**
 * @description: 
 *   This function store wallet data into Nvram
 * @param {BoatWalletDataCtx} *mWalletDataCtx
 * @param {BoatStoreType} storeType
 *  store in persistent or non-persistent space
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *  for details.
 * @author: aitos
 */
__BOATSTATIC BOAT_RESULT BoATWallet_DataCtx_Store(BoatWalletDataCtx *mWalletDataCtx,BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 walletNum = 0;
    BUINT32 walletLength = 0 ,walletLengthLen = 0, offset = 0 ;
    BUINT8 lengthbytes[3] = {0};
    // BUINT8 testbuf[1024] = {0};
    BUINT8 *walletData = NULL;

    result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,&walletNum,1,storeType);
    /* if read Nvram failed , no wallet */
    if(result != BOAT_SUCCESS){
        walletNum = 0;
    }
    if(walletNum >= BOAT_MAX_WALLET_NUM){
        return BOAT_ERROR;
    }
    if(storeType == BOAT_STORE_TYPE_RAM)
    {
        walletNum = 0;
    }
    offset += 1;
    for (int i = 0; i < walletNum; i++)
    {
        /* wallet length */
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthbytes,sizeof(lengthbytes),storeType);
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NONE,"read wallet length fail");
            return result;
        }
        walletLength = UtilityGetLVData_L(lengthbytes);
        if(walletLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        walletLengthLen = UtilityGetTLV_LL_from_len(walletLength);
        offset += (walletLength + walletLengthLen);
    }
    /* get wallet data */
    result = BoATWallet_Get_DataCtx_Len(mWalletDataCtx,&walletLength);
    if(result < BOAT_SUCCESS){
        return result;
    }
    BoatLog(BOAT_LOG_NONE," walletLength = %d ",walletLength);
    walletData = BoatMalloc(walletLength);
    if(walletData == NULL){
        return BOAT_ERROR;
    }
    result = BoATWallet_Get_DataCtx_Data(mWalletDataCtx,walletData,&walletLength);
    if(result != BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NONE," get wallet data fail ");
        BoatFree(walletData);
        return result;
    }
    BoatLog_hexdump(BOAT_LOG_NORMAL," wallet data : ",walletData,walletLength);
    /* store wallet data */
    result = BoATStoreSoftRotNvram(BOAT_STORE_KEYPAIR,offset,walletData,walletLength,storeType);
    if(result != BOAT_SUCCESS){
        return result;
    }
    // result = BoatReadSoftRotNvram(0,testbuf,4,0);
    // BoatLog_hexdump(BOAT_LOG_NORMAL,"store data 000 : ",testbuf,4);
    BoatFree(walletData);
    walletNum ++;
    result = BoATStoreSoftRotNvram(BOAT_STORE_KEYPAIR,0,&walletNum,1,storeType);
    // if(result != BOAT_SUCCESS){
    //     return result;
    // }
    //  result = BoatReadSoftRotNvram(0,testbuf,walletLength+1,0);
    BoatLog(BOAT_LOG_NONE," wallet num = %d ,result = %d ",walletNum,result);
    // BoatLog_hexdump(BOAT_LOG_NORMAL,"store data : ",testbuf,walletLength+1);


    return result;
}




/**
 * @description: 
 *  This function initialize wallet data context
 * @param[inout] {BoatWalletDataCtx} *mWalletDataCtx
 * @return 
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATIotWalletInit(BoatWalletDataCtx *mWalletDataCtx){
    mWalletDataCtx->extraData.value_len = 0;
    memset(mWalletDataCtx->extraData.value,0x00,sizeof(mWalletDataCtx->extraData.value));
    mWalletDataCtx->prikeyCtx.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_UNKNOWN;
    mWalletDataCtx->prikeyCtx.wallet_index = 0;
    mWalletDataCtx->prikeyCtx.wallet_name = NULL;
    mWalletDataCtx->prikeyCtx.prikey_type = BOAT_WALLET_PRIKEY_TYPE_UNKNOWN;
    memset(mWalletDataCtx->prikeyCtx.pubkey_content,0x00,sizeof(mWalletDataCtx->prikeyCtx.pubkey_content));
    return BOAT_SUCCESS;
}

/**
 * @description: 
 *  This function deinitialize wallet data context
 * @param[inout] {BoatWalletDataCtx} *mWalletDataCtx
 * @return 
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATIotWalletDeInit(BoatWalletDataCtx *mWalletDataCtx){
    mWalletDataCtx->extraData.value_len = 0;
    memset(mWalletDataCtx->extraData.value,0x00,sizeof(mWalletDataCtx->extraData.value));
    mWalletDataCtx->prikeyCtx.prikey_format = BOAT_WALLET_PRIKEY_FORMAT_UNKNOWN;
    mWalletDataCtx->prikeyCtx.wallet_index = 0;
    if(mWalletDataCtx->prikeyCtx.wallet_name != NULL){
        BoatFree(mWalletDataCtx->prikeyCtx.wallet_name);
    }
    mWalletDataCtx->prikeyCtx.wallet_name = NULL;
    mWalletDataCtx->prikeyCtx.prikey_type = BOAT_WALLET_PRIKEY_TYPE_UNKNOWN;
    memset(mWalletDataCtx->prikeyCtx.pubkey_content,0x00,sizeof(mWalletDataCtx->prikeyCtx.pubkey_content));
    return BOAT_SUCCESS;
}
/**
 * @description: 
 * This function creat wallet
 * @param[in] {BoatWalletPriKeyCtx_config} walletConfig
 * @param[in] {BCHAR} *walletName
 *  If walletName is NULL , BoAT will slect one testing name.
 * @param[in] {BoatStoreType} storeType
 *  For onetime wallet or test , select store in ram .
 *  For persistent wallet ,slect store in flash.
 * @return 
 *   This function returns wallet index if creat wallet successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoatWalletCreate(BoatWalletPriKeyCtx_config *walletConfig,BCHAR *walletName,BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatWalletDataCtx mWalletDataCtx ;
    BUINT8 walletIndex = 0;
    result = BoATIotWalletInit(&mWalletDataCtx);
    if(result != BOAT_SUCCESS){
        return result;
    }
    if(storeType == BOAT_STORE_TYPE_FLASH){
        result = BoATWallet_GetFreeIndex_From_Persistent();
        if(result < BOAT_SUCCESS){
            return result;
        }
        walletIndex = result;  // from 1 to BOAT_MAX_WALLET_NUM
    }else{
        walletIndex = 0;  // the index of onetimewallet is always 0
    }

    BoatLog(BOAT_LOG_NORMAL,"wallet index = %d ",walletIndex);
    result = BoatPort_keyCreate(walletConfig,&mWalletDataCtx);
    if(result != BOAT_SUCCESS){
        return result;
    }
    BoatLog(BOAT_LOG_NORMAL,"BoatPort_keyCreate success ");
    if(walletName == NULL){
        mWalletDataCtx.prikeyCtx.wallet_name = BoatMalloc(strlen("wallet")+2);// wallet+index+/0
        memcpy(mWalletDataCtx.prikeyCtx.wallet_name,"wallet",strlen("wallet"));
        mWalletDataCtx.prikeyCtx.wallet_name[strlen("wallet")] = 0x30 + walletIndex;
        mWalletDataCtx.prikeyCtx.wallet_name[strlen("wallet")+1] = 0;
    }else{
        mWalletDataCtx.prikeyCtx.wallet_name = BoatMalloc(strlen(walletName)+1);
        strcpy(mWalletDataCtx.prikeyCtx.wallet_name,walletName);
    }
    mWalletDataCtx.prikeyCtx.wallet_index = walletIndex;
    result = BoATWallet_DataCtx_Store(&mWalletDataCtx,storeType);
    if(result != BOAT_SUCCESS){
        return result;
    }
    memset(mWalletDataCtx.extraData.value,0x00,mWalletDataCtx.extraData.value_len);

    BoATIotWalletDeInit(&mWalletDataCtx);
    return walletIndex;

}

/**
 * @description: 
 *  This function deletes the wallet which walletIndex equals index.
 *  This function will delete the wallet data in Nvram.
 * @param {BUINT8} index
 *  the index want to delete
 * @return {*}
 *   This function returns BOAT_SUCCESS if delete successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATIotWalletDelete(BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 i = 0;
    BUINT8 walletNum = 0 ,walletNumNew = 0, walletIndex = 0;
    BUINT32 offset = 0, offset_moveFrom = 0 , offset_moveTo = 0;
    BUINT32 walletLength = 0 , walletLengthLen = 0 , paramLength = 0 , paramLengthLen = 0 ;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 *walletData = NULL;
    if(index >= BOAT_MAX_WALLET_NUM){
        return BOAT_ERROR;
    }
    /* onetime wallet 
       index of onetime wallet must be 0
    */
    if(index == 0){
        /* set wallet num of onetime wallet to 0 */
        memset(lengthBytes,0x00,sizeof(lengthBytes));
        result = BoATStoreSoftRotNvram(BOAT_STORE_KEYPAIR,0,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_RAM);
        return result;
    }else{ // persistent wallet
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,&walletNum,1,BOAT_STORE_TYPE_FLASH);
    /* if read Nvram failed , no wallet */
    if(result != BOAT_SUCCESS || walletNum == 0){
        return result;
    }
    offset ++;
    for ( i = 0; i < walletNum; i++)
    {
        /* read each wallet length */
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS ){
            return result;
        }
        walletLength = UtilityGetLVData_L(lengthBytes);
        if(walletLength < 0){
            BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
            return BOAT_ERROR;
        }
        walletLengthLen = UtilityGetTLV_LL_from_len(walletLength);
        // offset += walletLengthLen;
        /* wallet index  */
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset + walletLengthLen,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS ){
            return result;
        }
        paramLength = UtilityGetLVData_L(lengthBytes);
        if(paramLength != 1){
            BoatLog(BOAT_LOG_NORMAL,"wallet index length err ");
            return BOAT_ERROR;
        }
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset + walletLengthLen + 1,&walletIndex,sizeof(walletIndex),BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS ){
            BoatLog(BOAT_LOG_NORMAL,"read wallet index err ");
            return result;
        }
        if(walletIndex == index){
            break;
        }
        offset += (walletLengthLen + walletLength);
    }
    BoatLog(BOAT_LOG_NORMAL,"i= %d ",i);
    if(i >= walletNum){
        BoatLog(BOAT_LOG_NORMAL,"not find the wallet ");
        return BOAT_ERROR;
    }
    walletNumNew = walletNum -1;
    result = BoATStoreSoftRotNvram(BOAT_STORE_KEYPAIR,0,&walletNumNew,sizeof(walletNumNew),BOAT_STORE_TYPE_FLASH); // only need to reset wallet length bytes
    if(result != BOAT_SUCCESS){
        BoatLog(BOAT_LOG_NORMAL,"delete wallet fail ");
        return result;
    }

    if(i == (walletNum -1)){  /* last one wallet*/
        memset(lengthBytes,0x00,sizeof(lengthBytes));
        result = BoATStoreSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH); // only need to reset wallet length bytes
        if(result != BOAT_SUCCESS){
            /* recover walletNum */
        result = BoATStoreSoftRotNvram(BOAT_STORE_KEYPAIR,offset,&walletNum,sizeof(walletNum),BOAT_STORE_TYPE_FLASH); 
        if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"delete wallet fail ");
            return result;
        }
            BoatLog(BOAT_LOG_NORMAL,"delete wallet fail ");
            return result;
        }
    }else{
        offset_moveTo = offset;
        // the next wallet
        offset += (walletLengthLen + walletLength);
        offset_moveFrom = offset;
        i++;  
        walletLength = 0;
        for ( ; i < walletNum; i++)
        {
            /* code */
            result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset,lengthBytes,sizeof(lengthBytes),BOAT_STORE_TYPE_FLASH);
            if(result != BOAT_SUCCESS ){
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if(paramLength < 0){
                BoatLog(BOAT_LOG_NORMAL,"wallet data length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += (paramLengthLen + paramLength);
            walletLength += (paramLengthLen + paramLength);
        }
        // all the wallets'length after index wallet
        walletData = BoatMalloc(walletLength);
        if(NULL == walletData){
            BoatLog(BOAT_LOG_NONE,"fail to malloc memory ");
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        result = BoatReadSoftRotNvram(BOAT_STORE_KEYPAIR,offset_moveFrom,walletData,walletLength,BOAT_STORE_TYPE_FLASH);
        if(result != BOAT_SUCCESS){
            BoatFree(walletData);
            BoatLog(BOAT_LOG_NONE,"read wallet data fail ");
            return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
        }
        result = BoATStoreSoftRotNvram(BOAT_STORE_KEYPAIR,offset_moveTo,walletData,walletLength,BOAT_STORE_TYPE_FLASH);
        BoatFree(walletData);
        if(result != BOAT_SUCCESS){
            /* recover walletNum */
            result = BoATStoreSoftRotNvram(BOAT_STORE_KEYPAIR,offset,&walletNum,sizeof(walletNum),BOAT_STORE_TYPE_FLASH); 
            if(result != BOAT_SUCCESS){
            BoatLog(BOAT_LOG_NORMAL,"delete wallet fail ");
            return result;
            }
            BoatLog(BOAT_LOG_NONE,"store wallet data fail ");
            return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
        }
    }
    }

    
    return result;
}


BOAT_RESULT BoatIotSdkInit(void)
{
    // BUINT32 i;
#if ((PROTOCOL_USE_ETHEREUM == 1) || (PROTOCOL_USE_PLATON == 1) || (PROTOCOL_USE_PLATONE == 1) || (PROTOCOL_USE_FISCOBCOS == 1) || (PROTOCOL_USE_VENACHAIN == 1))    
    cJSON_Hooks hooks;
#endif    
    
#if RPC_USE_LIBCURL == 1
    CURLcode curl_result;
#endif

#if ((PROTOCOL_USE_ETHEREUM == 1) || (PROTOCOL_USE_PLATON == 1) || (PROTOCOL_USE_PLATONE == 1) || (PROTOCOL_USE_FISCOBCOS == 1) || (PROTOCOL_USE_VENACHAIN == 1))
    hooks.malloc_fn = BoatMalloc;
    hooks.free_fn   = BoatFree;
    cJSON_InitHooks(&hooks);
#endif

	// For Multi-Thread Support: CreateMutex Here
    
#if RPC_USE_LIBCURL == 1
    curl_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    
    if (curl_result != CURLE_OK)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unable to initialize curl.");
        return BOAT_ERROR_CURL_INIT_FAIL;
    }
#endif

    return BOAT_SUCCESS;
}


void BoatIotSdkDeInit(void)
{
    // BUINT32 i;
    

#if RPC_USE_LIBCURL == 1
    curl_global_cleanup();
#endif

// For Multi-Thread Support: DeleteMutex Here
}


