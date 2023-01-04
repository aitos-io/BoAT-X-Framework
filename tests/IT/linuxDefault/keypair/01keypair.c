
/**
****************************************************************************************
* @FilePath: keypairCreat.c
* @Author: aitos
* @Date: 2022-10-26 20:47:24
* @LastEditors:
* @LastEditTime: 2022-10-27 10:07:01
* @Descripttion:
****************************************************************************************
*/

#include "tcase_keypair.h"
#include "boatiotsdk.h"
#include "boatlog.h"

/**
 * PKCS format demo key. The original private key of 'pkcs_demoKey' is
 * "fcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca"
 */
const BCHAR *pkcs_demoKey = "-----BEGIN EC PRIVATE KEY-----\n"
                            "MHQCAQEEIPz212cG5mJQ26zJgnvEJzIe25VC1Yp0pnYkslOWBGXKoAcGBSuBBAAK\n"
                            "oUQDQgAEMU/3IAjKpQc8XdURIGQZZJQRHZhPDkp80ahiRAM7KKV9Gmn699pei5fL\n"
                            "qZlYLvlxdQJsoh2IPyObgGr87gBT7w==\n"
                            "-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
const BCHAR *native_demoKey = "0xf1395a1fc3f74f0c867b61292e28e0f6cc98a095535fd6bf04e4169ebc047e61";

const BUINT8 pubkey[] = {0xC8, 0x00, 0xAE, 0xD2, 0x61, 0x2C, 0xB2, 0xB6, 0x2F, 0x4C, 0xED, 0x0C, 0x8E, 0xD5, 0xDF, 0xE7, 0x49, 0xE5, 0x17, 0xC2, 0xCF, 0x1B, 0x31, 0x9C, 0x14, 0xC3, 0x1C, 0x2C, 0x44, 0xE6, 0xA2, 0x9D, 0xEA, 0x68, 0xC3, 0x84, 0x65, 0x02, 0x3E, 0x9C, 0x65, 0xD3, 0x64, 0x02, 0xDA, 0x5E, 0xE4, 0x24, 0xAC, 0x29, 0x55, 0xD9, 0x1A, 0x30, 0xA5, 0x0D, 0x98, 0xE0, 0x94, 0xB4, 0xC2, 0xE9, 0x25, 0x72};
const BUINT8 pubkeyFormPkcs[] = {0x31, 0x4f, 0xf7, 0x20, 0x08, 0xca, 0xa5, 0x07, 0x3c, 0x5d, 0xd5, 0x11, 0x20, 0x64, 0x19, 0x64, 0x94, 0x11, 0x1d, 0x98, 0x4f, 0x0e, 0x4a, 0x7c, 0xd1, 0xa8, 0x62, 0x44, 0x03, 0x3b, 0x28, 0xa5, 0x7d, 0x1a, 0x69, 0xfa, 0xf7, 0xda, 0x5e, 0x8b, 0x97, 0xcb, 0xa9, 0x99, 0x58, 0x2e, 0xf9, 0x71, 0x75, 0x02, 0x6c, 0xa2, 0x1d, 0x88, 0x3f, 0x23, 0x9b, 0x80, 0x6a, 0xfc, 0xee, 0x00, 0x53, 0xef};
const BUINT8 pubkey256R1[] = {0xEC, 0xBB, 0x65, 0xC5, 0xAF, 0x1E, 0x6E, 0xD9, 0x80, 0x16, 0x32, 0xFE, 0x30, 0x18, 0x7D, 0xEA, 0x2B, 0x31, 0x83, 0x6B, 0xF4, 0x2E, 0xB2, 0xD2, 0xF4, 0x0B, 0x78, 0x95, 0xC1, 0x97, 0xBB, 0xDC, 0x0C, 0xA0, 0xD7, 0x31, 0x63, 0x80, 0xAB, 0x59, 0xD4, 0x6A, 0xEE, 0xDD, 0x55, 0xD9, 0xEB, 0xEA, 0x2C, 0x44, 0xB0, 0xFC, 0xE8, 0x50, 0xF7, 0xF8, 0xC6, 0x14, 0xCF, 0x8E, 0xAF, 0x09, 0x37, 0xAF};
BUINT8 binFormatKey[32] = {0};

/**
****************************************************************************************
* @brief:
*  This function get keypair config ;
* @param[in/out] *keypair_config
* @return
*   This function has no return value;
****************************************************************************************
*/
void get_keypair_external_native_256k1_config(BoatKeypairPriKeyCtx_config *keypair_config)
{
    (void)binFormatKey; // avoid warning
    keypair_config->prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config->prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config->prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config->prikey_content.field_ptr = binFormatKey;
    keypair_config->prikey_content.field_len = 32;
    return;
}

void get_keypair_internal_native_256k1_config(BoatKeypairPriKeyCtx_config *keypair_config)
{
    (void)binFormatKey; // avoid warning
    keypair_config->prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
    keypair_config->prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    return;
}

void get_keypair_external_pkcs_256k1_config(BoatKeypairPriKeyCtx_config *keypair_config)
{
    (void)binFormatKey; // avoid warning
    keypair_config->prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config->prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config->prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    keypair_config->prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
    keypair_config->prikey_content.field_len = strlen(pkcs_demoKey) + 1; // length contain terminator
    return;
}

void get_keypair_external_native_256R1_config(BoatKeypairPriKeyCtx_config *keypair_config)
{
    (void)binFormatKey; // avoid warning
    keypair_config->prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config->prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config->prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config->prikey_content.field_ptr = binFormatKey;
    keypair_config->prikey_content.field_len = 32;
    return;
}

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime keypair success
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0001CreateOneTimeKeypairSuccess)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config);

    /* create keypair */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get keypair content by index */
    createkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* check result of get keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in keypair struct */
    ck_assert_int_eq(createkeypair.keypair_index, 0);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(createkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(createkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key in the struct */
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkey, sizeof(pubkey)), 0);
    /* free keypair name */
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }
    /* delete the keypair */
    BoATIotKeypairDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create one persist keypair success
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0002CreatePersistKeypairSuccess)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native format , 256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);

    /* createkeypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get keypair content by index */
    createkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* check result of get keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair index in the struct , must be 1 */
    ck_assert_int_eq(createkeypair.keypair_index, 1);
    /* check the keypair name in the struct , must be same to the name in the cteating interface */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairPersist01"), 0);
    /* chect format in the struct , must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* chect key type in the struct ,must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key */
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkey, sizeof(pubkey)), 0);
    /* free keypair_name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }
    /* delete the keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create one persist keypair success, key format must be native
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0003CreatePersistKeypairNativeSuccess)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native format , 256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get keypair content by index */
    createkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* check result of get keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair index in the struct , must be 1 */
    ck_assert_int_eq(createkeypair.keypair_index, 1);
    /* check the keypair name in the struct , must be same to the name in the cteating interface */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairPersist01"), 0);
    /* chect format in the struct , must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* chect key type in the struct ,must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key */
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkey, sizeof(pubkey)), 0);
    /* free keypair_name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }
    /* delete the keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create one persist keypair success, key format must be pkcs
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0004CreatePersistKeypairPKCSSuccess)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , pkcs format , 256K1  */
    get_keypair_external_pkcs_256k1_config(&keypair_config);

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get keypair content by index */
    createkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* check result of get keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair index in the struct , must be 1 */
    ck_assert_int_eq(createkeypair.keypair_index, 1);
    /* check the keypair name in the struct , must be same to the name in the cteating interface */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairPersist01"), 0);
    /* chect format in the struct , must be NATIVE
       input of create keypair interface is PKCS , but the out of keypair create interface is NATIVE,
       becase the key format created alway be NATIVE , and stored by native format */
    ck_assert_int_eq(createkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* chect key type in the struct ,must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key */
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkeyFormPkcs, sizeof(pubkeyFormPkcs)), 0);
    /* free keypair_name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }
    /* delete the keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create one persist keypair success, key type must be 256K1
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0005CreatePersistKeypair256K1Success)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native format , 256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get keypair content by index */
    createkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* check result of get keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair index in the struct , must be 1 */
    ck_assert_int_eq(createkeypair.keypair_index, 1);
    /* check the keypair name in the struct , must be same to the name in the cteating interface */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairPersist01"), 0);
    /* chect format in the struct , must be NATIVE */
    ck_assert_int_eq(createkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* chect key type in the struct ,must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key */
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkey, sizeof(pubkey)), 0);
    /* free keypair_name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }

    /* delete the keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create one persist keypair success, key type must be 256R1
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0006CreatePersistKeypair256R1Success)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native format , 256R1  */
    get_keypair_external_native_256R1_config(&keypair_config);

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get keypair content by index */
    createkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* check result of get keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair index in the struct , must be 1 */
    ck_assert_int_eq(createkeypair.keypair_index, 1);
    /* check the keypair name in the struct , must be same to the name in the cteating interface */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairPersist01"), 0);
    /* chect format in the struct , must be NATIVE */
    ck_assert_int_eq(createkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* chect key type in the struct ,must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);
    /* check public key */
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkey256R1, sizeof(pubkey256R1)), 0);
    /* free keypair_name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }
    /* delete the keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create one persist keypair success, key genmode must be external
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0007CreatePersistKeypairEXTERNALSuccess)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native format , 256R1  */
    get_keypair_external_native_256R1_config(&keypair_config);

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get keypair content by index */
    createkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* check result of get keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair index in the struct , must be 1 */
    ck_assert_int_eq(createkeypair.keypair_index, 1);
    /* check the keypair name in the struct , must be same to the name in the cteating interface */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairPersist01"), 0);
    /* chect format in the struct , must be NATIVE */
    ck_assert_int_eq(createkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* chect key type in the struct ,must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);
    /* check public key */
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkey256R1, sizeof(pubkey256R1)), 0);
    /* free keypair_name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }
    /* delete the keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create one persist keypair success, key genmode must be internal
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0008CreatePersistKeypairINTERNALSuccess)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from internal ,  256K1  */
    get_keypair_internal_native_256k1_config(&keypair_config);

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get keypair content by index */
    createkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* check result of get keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair index in the struct , must be 1 */
    ck_assert_int_eq(createkeypair.keypair_index, 1);
    /* check the keypair name in the struct , must be same to the name in the cteating interface */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairPersist01"), 0);
    /* chect format in the struct , must be NATIVE */
    ck_assert_int_eq(createkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* chect key type in the struct ,must be same to the config */
    ck_assert_int_eq(createkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* free keypair_name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }

    /* delete the keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete onetime keypair success
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0009DeleteOneTimeKeypairSuccess)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native ,  256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);

    /* create keypair , store type must be RAM  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0*/
    ck_assert_int_eq(rtnVal, 0);
    /* delete the keypair */
    BoATIotKeypairDelete(rtnVal);
    /* get keypair content by index , index = 0 (one time keypair)*/
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, 0);
    /* rtnVal must be BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE , because the keypair has be deleted , have no this keypair*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete persist keypair success
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0010DeletePersistKeypairSuccess)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native ,  256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);

    /* create keypair , store type must be FLASH  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1 , persist index starts from 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* delete the keypair */
    BoATIotKeypairDelete(rtnVal);
    /* get keypair content by index */
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, rtnVal);
    /* rtnVal must be BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE , because the keypair has be deleted , have no this keypair*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create keypair fail ;
*  store type is error , not ram or flash .
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0011CreateKeypair_storeTypeERR)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native ,  256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);

    /* create keypair , store type unknown  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_UNKNOWN);
    /* check result of create keypair , failed and the result must be BOAT_ERROR_KEYPAIR_KEY_STORETYPE_ERR*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_STORETYPE_ERR);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create keypair fail ;
*  genmode is error , not internal or external .
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0012CreateOneTimeKeypair_PriKeyGenModeERR)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native ,  256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);
    /* change the genmode to unknown*/
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_UNKNOWN;

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check result of create keypair , failed and the result must be BOAT_ERROR_KEYPAIR_KEY_GENMODE_ERR*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_GENMODE_ERR);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create keypair fail ;
*  key format is wrong , not native or pkcs .
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0013CreateOneTimeKeypair_PriKeyFormatERR)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native ,  256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);
    /* change the format to unknown*/
    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_UNKNOWN;

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check result of create keypair , failed and the result must be BOAT_ERROR_KEYPAIR_KEY_FORMAT_ERR*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_FORMAT_ERR);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create keypair fail ;
*  key type is wrong , not 256K1 or 256R1 .
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0014CreateOneTimeKeypair_PriKeyTypeERR)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config : gen from external , native ,  256K1  */
    get_keypair_external_native_256k1_config(&keypair_config);
    /* change the type to unknown*/
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_UNKNOWN;

    /* create keypair  */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check result of create keypair , failed and the result must be BOAT_ERROR_KEYPAIR_KEY_TYPE_ERR*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_TYPE_ERR);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete keypair fail ;
*  have no keypair to delete
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0015DeleteKeypair_NoKeypair)
{
    BSINT32 rtnVal;
    /* index form 0 to BOAT_MAX_KEYPAIR_NUM , index of onetime keypair is 0 ,
     persist's are from 1 to BOAT_MAX_KEYPAIR_NUM */
    for (size_t i = 0; i <= BOAT_MAX_KEYPAIR_NUM; i++)
    {
        /* code */
        rtnVal = BoATIotKeypairDelete(i);
        /* check result of delete keypair , there have no keypair to delete ,
         so the result must be  BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE*/
        ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete keypair fail ;
*  Index exceed to BOAT_MAX_KEYPAIR_NUM
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0016DeleteKeypair_IndexExceed)
{
    BSINT32 rtnVal;
    /* index form 0 to BOAT_MAX_KEYPAIR_NUM , index of onetime keypair is 0 ,
     persist's are from 1 to BOAT_MAX_KEYPAIR_NUM ;*/

    rtnVal = BoATIotKeypairDelete(BOAT_MAX_KEYPAIR_NUM + 1);
    /* check result of delete keypair , index of keypair to delete is bigger than BOAT_MAX_KEYPAIR_NUM ,
     so the result must be  BOAT_ERROR_KEYPAIR_KEY_INDEX_EXCEED*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair list fail ;
*  the input param (BoatIotKeypairContext) is NULL
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0017GetKeypairList_ParamNULL)
{
    BSINT32 rtnVal;
    /* get keypair list with NULL input param*/
    rtnVal = BoATKeypair_GetKeypairList(NULL);
    /* check result of delete keypair ,  the result must be  BOAT_ERROR_COMMON_INVALID_ARGUMENT*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair list with no keypair created ;
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0018GetKeypairList_NOKeypair)
{
    BSINT32 rtnVal;
    BoatIotKeypairContext keypairList;
    /* get keypair list with no keipair created , the interface return success but the kepair mun  is 0*/
    rtnVal = BoATKeypair_GetKeypairList(&keypairList);
    /* check result of get keypair list */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair num in the list , must be 0*/
    ck_assert_int_eq(keypairList.keypairNum, 0);
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypairList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair list with one  onetime keypair created ;
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0019GetKeypairList_OnlyOnetimeKeypair)
{
    BSINT32 rtnVal;
    BoatIotKeypairContext keypairList;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* create onetime keypair */
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config);
    /* create keypair , store type must be ram*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get keypair list with only one onetime keipair created */
    rtnVal = BoATKeypair_GetKeypairList(&keypairList);
    /* check result of get keypair list */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair num in the list , must be 1*/
    ck_assert_int_eq(keypairList.keypairNum, 1);
    /* check  persist keypair num in the list , must be 0*/
    ck_assert_int_eq(keypairList.keypairPersistentNum, 0);
    /* check content of keypair list */
    /* index of the onetime keypair must be 0*/
    ck_assert_int_eq(keypairList.keypairs[0].prikeyCtx.keypair_index, 0);
    /* format of the keypair list must be same to config's */
    ck_assert_int_eq(keypairList.keypairs[0].prikeyCtx.prikey_format, keypair_config.prikey_format);
    /* key type of the keypair list must be same to config's */
    ck_assert_int_eq(keypairList.keypairs[0].prikeyCtx.prikey_type, keypair_config.prikey_type);
    /* keypair name must be same as created */
    ck_assert_int_eq(strcmp(keypairList.keypairs[0].prikeyCtx.keypair_name, "keypairOnetime"), 0);
    /* check public key in the list*/
    ck_assert_int_eq(memcmp(keypairList.keypairs[0].prikeyCtx.pubkey_content, pubkey, sizeof(pubkey)), 0);
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypairList);
    /* delete the keypair */
    BoATIotKeypairDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair list with one  onetime keypair and one persist keypair ;
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0020GetKeypairList_OnetimeAndPersistKeypair)
{
    BSINT32 rtnVal;
    BoatIotKeypairContext keypairList;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* create onetime keypair */
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config);
    /* create keypair , store type must be ram*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* create persist keypair*/
    /* create keypair , store type must be flash*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairpersist", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    /* get keypair list with only one onetime keipair and one persist keypair created */
    rtnVal = BoATKeypair_GetKeypairList(&keypairList);
    /* check result of get keypair list */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair num in the list , must be 2 , two keypairs created */
    ck_assert_int_eq(keypairList.keypairNum, 2);
    /* check  persist keypair num in the list , must be 1
    there is only one persist keypair created */
    ck_assert_int_eq(keypairList.keypairPersistentNum, 1);
    /* check content of keypair list */
    /* index of the onetime keypair must be 0*/
    ck_assert_int_eq(keypairList.keypairs[1].prikeyCtx.keypair_index, 0);
    /* format of the keypair list must be same to config's */
    ck_assert_int_eq(keypairList.keypairs[1].prikeyCtx.prikey_format, keypair_config.prikey_format);
    /* key type of the keypair list must be same to config's */
    ck_assert_int_eq(keypairList.keypairs[1].prikeyCtx.prikey_type, keypair_config.prikey_type);
    /* keypair name must be same as created */
    ck_assert_int_eq(strcmp(keypairList.keypairs[1].prikeyCtx.keypair_name, "keypairOnetime"), 0);
    /* check public key in the list*/
    ck_assert_int_eq(memcmp(keypairList.keypairs[1].prikeyCtx.pubkey_content, pubkey, sizeof(pubkey)), 0);

    /* check content of keypair list */
    /* index of the persist keypair must be 1*/
    ck_assert_int_eq(keypairList.keypairs[0].prikeyCtx.keypair_index, 1);
    /* format of the keypair list must be same to config's */
    ck_assert_int_eq(keypairList.keypairs[0].prikeyCtx.prikey_format, keypair_config.prikey_format);
    /* key type of the keypair list must be same to config's */
    ck_assert_int_eq(keypairList.keypairs[0].prikeyCtx.prikey_type, keypair_config.prikey_type);
    /* keypair name must be same as created */
    ck_assert_int_eq(strcmp(keypairList.keypairs[0].prikeyCtx.keypair_name, "keypairpersist"), 0);
    /* check public key in the list*/
    ck_assert_int_eq(memcmp(keypairList.keypairs[0].prikeyCtx.pubkey_content, pubkey, sizeof(pubkey)), 0);

    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypairList);
    /* delete onetime keypair */
    BoATIotKeypairDelete(0);
    /* delete the persist keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair content by index ;
*  the input param is NULL .
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0021GetKeypairByIndex_ParamNULL)
{
    BSINT32 rtnVal;
    /* get keypair by index , but the first param is NULL */
    rtnVal = BoATKeypair_GetKeypairByIndex(NULL, 0);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair content by index ;
*  The index to be read is exceeded to BOAT_MAX_KEYPAIR_NUM
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0022GetKeypairByIndex_IndexExceed)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    /* get keypair by index , the index is bigger than BOAT_MAX_KEYPAIR_NUM */
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, BOAT_MAX_KEYPAIR_NUM + 1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair content by index ;
*  if index equal 0 , get the onetime keypair content
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0023GetKeypairByIndex_Onetime)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* create onetime keypair */
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config);
    /* create keypair , store type must be ram*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* create persist keypair*/
    /* create keypair , store type must be flash*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairpersist", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    createkeypair.keypair_name = NULL;
    /* get keypair by index , the index of onetime keypair is 0 */
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, 0);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair content*/
    /* index of the onetime keypair must be 0*/
    ck_assert_int_eq(createkeypair.keypair_index, 0);
    /* format of the keypair must be same to config's */
    ck_assert_int_eq(createkeypair.prikey_format, keypair_config.prikey_format);
    /* key type of the keypair must be same to config's */
    ck_assert_int_eq(createkeypair.prikey_type, keypair_config.prikey_type);
    /* keypair name must be same as created */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairOnetime"), 0);
    /* check public key in the struct*/
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkey, sizeof(pubkey)), 0);
    /* free memory of keypair name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }

    /* delete onetime keypair */
    BoATIotKeypairDelete(0);
    /* delete the persist keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair content by index ;
*  the index of persist keypair from 1 to BOAT_MAX_KEYPAIR_NUM;
*  if only create one persist keypair ,the index is 1
* @return
****************************************************************************************
*/
START_TEST(test_001Keypair_0024GetKeypairByIndex_Persist)
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx createkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* create onetime keypair */
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config);
    /* create keypair , store type must be ram*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* create persist keypair*/
    /* create keypair , store type must be flash*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairpersist", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    createkeypair.keypair_name = NULL;
    /* get keypair by index , the index of persist keypair is 1 */
    rtnVal = BoATKeypair_GetKeypairByIndex(&createkeypair, 1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair content*/
    /* index of the persist keypair must be 1*/
    ck_assert_int_eq(createkeypair.keypair_index, 1);
    /* format of the keypair must be same to config's */
    ck_assert_int_eq(createkeypair.prikey_format, keypair_config.prikey_format);
    /* key type of the keypair must be same to config's */
    ck_assert_int_eq(createkeypair.prikey_type, keypair_config.prikey_type);
    /* keypair name must be same as created */
    ck_assert_int_eq(strcmp(createkeypair.keypair_name, "keypairpersist"), 0);
    /* check public key in the struct*/
    ck_assert_int_eq(memcmp(createkeypair.pubkey_content, pubkey, sizeof(pubkey)), 0);
    /* free memory of keypair name*/
    if (createkeypair.keypair_name != NULL)
    {
        BoatFree(createkeypair.keypair_name);
    }

    /* delete onetime keypair */
    BoATIotKeypairDelete(0);
    /* delete the persist keypair */
    BoATIotKeypairDelete(1);
}
END_TEST

Suite *make_keypairIntfTest_suite(void)
{
    /* Create Suite */
    Suite *s_keypaircreate = suite_create("keypairCreat");

    /* Create test cases */
    TCase *tc_keypairCreat_api = tcase_create("keypairCreat_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_keypaircreate, tc_keypairCreat_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0001CreateOneTimeKeypairSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0002CreatePersistKeypairSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0003CreatePersistKeypairNativeSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0004CreatePersistKeypairPKCSSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0005CreatePersistKeypair256K1Success);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0006CreatePersistKeypair256R1Success);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0007CreatePersistKeypairEXTERNALSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0008CreatePersistKeypairINTERNALSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0009DeleteOneTimeKeypairSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0010DeletePersistKeypairSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0011CreateKeypair_storeTypeERR);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0012CreateOneTimeKeypair_PriKeyGenModeERR);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0013CreateOneTimeKeypair_PriKeyFormatERR);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0014CreateOneTimeKeypair_PriKeyTypeERR);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0015DeleteKeypair_NoKeypair);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0016DeleteKeypair_IndexExceed);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0017GetKeypairList_ParamNULL);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0018GetKeypairList_NOKeypair);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0019GetKeypairList_OnlyOnetimeKeypair);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0020GetKeypairList_OnetimeAndPersistKeypair);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0021GetKeypairByIndex_ParamNULL);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0022GetKeypairByIndex_IndexExceed);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0023GetKeypairByIndex_Onetime);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0024GetKeypairByIndex_Persist);

    return s_keypaircreate;
}