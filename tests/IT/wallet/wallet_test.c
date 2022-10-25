/*
 * @Description: 
 * @Author: aitos
 * @Date: 2022-09-04 11:28:58
 * @LastEditors: zt222 tao.zheng@aitos.io
 * @LastEditTime: 2022-10-24 19:34:55
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
__BOATSTATIC BOAT_RESULT createOnetimekeypair()
{
    BSINT32 index;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

	/* keypair_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: internal generated.");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[pkcs].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[native].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #endif


	/* create ethereum keypair */
    index = BoatKeypairCreate( &keypair_config, "keypairOnetime",BOAT_STORE_TYPE_RAM);
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    
    return BOAT_SUCCESS;
}
// #endif

// #if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT createPersistkeypair(BCHAR *keypair_name)
{
    BSINT32 index;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

    /* keypair_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: internal generated.");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[pkcs].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[native].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #endif

    BoatLog(BOAT_LOG_NORMAL,"prikey genmode = %d ",keypair_config.prikey_genMode);
	/* create ethereum keypair */
    index = BoatKeypairCreate( &keypair_config , keypair_name,BOAT_STORE_TYPE_FLASH);
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create persist keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }


    return BOAT_SUCCESS;
}
// #endif

int main(int argc, char *argv[])
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatIotKeypairContext keypairList;
    boat_try_declare;
    BoatLog(BOAT_LOG_NORMAL,"begin to test ");
    result = createOnetimekeypair();
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair type: create persist keypair.");
    result = createPersistkeypair("Persistkeypair01");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "keypair creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = createPersistkeypair("Persistkeypair02");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "keypair creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = createPersistkeypair("Persistkeypair03");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "keypair creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = createPersistkeypair("Persistkeypair04");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "keypair creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = createPersistkeypair("Persistkeypair05");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "keypair creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = BoATIotKeypairDelete(4);

    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "keypair delete failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    result = BoATKeypair_GetKeypairList(&keypairList);
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "get keypair list failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    for (size_t i = 0; i < keypairList.keypairNum; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL , "keypair[%d].index = %d ",i,keypairList.keypairs[i].prikeyCtx.keypair_index);
        BoatLog(BOAT_LOG_NORMAL , "keypair[%d].name  = %s ",i,keypairList.keypairs[i].prikeyCtx.keypair_name);
        BoatLog(BOAT_LOG_NORMAL , "keypair[%d].prikey format = %d ",i,keypairList.keypairs[i].prikeyCtx.prikey_format);
        BoatLog(BOAT_LOG_NORMAL , "keypair[%d].prikey type = %d ",i,keypairList.keypairs[i].prikeyCtx.prikey_type);
    }
    BoATKeypair_FreeKeypairContext(keypairList);
    
    result = createPersistkeypair("Persistkeypair04");
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "keypair creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    result = BoATIotKeypairDelete(0);

    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "keypair delete failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}


    result = BoATKeypair_GetKeypairList(&keypairList);
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "get keypair list failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    for (size_t i = 0; i < keypairList.keypairNum; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL , "keypair[%d].index = %d ",i,keypairList.keypairs[i].prikeyCtx.keypair_index);
        BoatLog(BOAT_LOG_NORMAL , "keypair[%d].name  = %s ",i,keypairList.keypairs[i].prikeyCtx.keypair_name);
        BoatLog(BOAT_LOG_NORMAL , "keypair[%d].prikey format = %d ",i,keypairList.keypairs[i].prikeyCtx.prikey_format);
        BoatLog(BOAT_LOG_NORMAL , "keypair[%d].prikey type = %d ",i,keypairList.keypairs[i].prikeyCtx.prikey_type);
    }



    BoatLog(BOAT_LOG_CRITICAL, "keypair test success: ");
    boat_catch(ethereum_storeread_demo_catch)
    {
    }
}