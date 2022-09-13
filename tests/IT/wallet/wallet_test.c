/*
 * @Description: 
 * @Author: aitos
 * @Date: 2022-09-04 11:28:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-06 15:15:55
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
#include "boatconfig.h"
#include "boatiotsdk.h"
#include "boatlog.h"


/**
 * PKCS format demo key. The original private key of 'pkcs_demoKey' is 
 * "fcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca"
 */
const BCHAR *pkcs_demoKey =  "-----BEGIN EC PRIVATE KEY-----\n"
                             "MHQCAQEEIPz212cG5mJQ26zJgnvEJzIe25VC1Yp0pnYkslOWBGXKoAcGBSuBBAAK\n"
                             "oUQDQgAEMU/3IAjKpQc8XdURIGQZZJQRHZhPDkp80ahiRAM7KKV9Gmn699pei5fL\n"
                             "qZlYLvlxdQJsoh2IPyObgGr87gBT7w==\n"
                             "-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
const BCHAR *native_demoKey = "0xf1395a1fc3f74f0c867b61292e28e0f6cc98a095535fd6bf04e4169ebc047e61";



// #if defined(USE_ONETIME_WALLET) 
__BOATSTATIC BOAT_RESULT createOnetimeWallet()
{
    BSINT32 index;
    BoatWalletPriKeyCtx_config wallet_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

	/* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        wallet_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        wallet_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        wallet_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        wallet_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
        wallet_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        wallet_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        wallet_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #endif


	/* create ethereum wallet */
    index = BoatWalletCreate( &wallet_config, "walletOnetime",BOAT_STORE_TYPE_RAM);
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    
    return BOAT_SUCCESS;
}
// #endif

// #if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatWalletPriKeyCtx_config wallet_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

    /* wallet_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: internal generated.");
        wallet_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[pkcs].");
        wallet_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
        wallet_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        wallet_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        wallet_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet format: external injection[native].");
        wallet_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        wallet_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_NATIVE;
        wallet_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        wallet_config.prikey_content.field_ptr = binFormatKey;
        wallet_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        wallet_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_INTERNAL_GENERATION;
        wallet_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256K1;
    #endif

    BoatLog(BOAT_LOG_NORMAL,"prikey genmode = %d ",wallet_config.prikey_genMode);
	/* create ethereum wallet */
    index = BoatWalletCreate( &wallet_config , wallet_name,BOAT_STORE_TYPE_FLASH);
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }


    return BOAT_SUCCESS;
}
// #endif

int main(int argc, char *argv[])
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatIotWalletContext walletList;
    boat_try_declare;
    BoatLog(BOAT_LOG_NORMAL,"begin to test ");
    result = createOnetimeWallet();
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = createPersistWallet("PersistWallet01");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = createPersistWallet("PersistWallet02");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = createPersistWallet("PersistWallet03");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = BoATIotWalletDelete(2);

    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet delete failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    result = BoATWallet_GetWalletList(&walletList);
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "get wallet list failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    for (size_t i = 0; i < walletList.walletNum; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL , "wallet[%d].index = %d ",i,walletList.wallets[i].prikeyCtx.wallet_index);
        BoatLog(BOAT_LOG_NORMAL , "wallet[%d].name  = %s ",i,walletList.wallets[i].prikeyCtx.wallet_name);
        BoatLog(BOAT_LOG_NORMAL , "wallet[%d].prikey format = %d ",i,walletList.wallets[i].prikeyCtx.prikey_format);
        BoatLog(BOAT_LOG_NORMAL , "wallet[%d].prikey type = %d ",i,walletList.wallets[i].prikeyCtx.prikey_type);
    }
    BoATWallet_FreeWalletContext(walletList);
    
        result = BoATIotWalletDelete(0);

    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet delete failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}


    result = BoATWallet_GetWalletList(&walletList);
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "get wallet list failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    for (size_t i = 0; i < walletList.walletNum; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL , "wallet[%d].index = %d ",i,walletList.wallets[i].prikeyCtx.wallet_index);
        BoatLog(BOAT_LOG_NORMAL , "wallet[%d].name  = %s ",i,walletList.wallets[i].prikeyCtx.wallet_name);
        BoatLog(BOAT_LOG_NORMAL , "wallet[%d].prikey format = %d ",i,walletList.wallets[i].prikeyCtx.prikey_format);
        BoatLog(BOAT_LOG_NORMAL , "wallet[%d].prikey type = %d ",i,walletList.wallets[i].prikeyCtx.prikey_type);
    }



    BoatLog(BOAT_LOG_CRITICAL, "wallet test success: ");
    boat_catch(ethereum_storeread_demo_catch)
    {
    }
}