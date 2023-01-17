/**
****************************************************************************************
* @FilePath: keypairUnitTest.c
* @Author: aitos
* @Date: 2022-10-27 15:53:40
* @LastEditors:
* @LastEditTime: 2022-10-27 15:53:40
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
const BCHAR *native_demoKey[6] = {"0xf1395a1fc3f74f0c867b61292e28e0f6cc98a095535fd6bf04e4169ebc047e61",
                                  "0x5054D432A17B8E286426D478A830ED1C0F3716A62CDCF84B76A0C6EDDF8AC86A",
                                  "0x4F438FD72E8DA43DEE7B1F05BAF9A0E1C1AF3AA4AA261029E92C3A14FC60DB95",
                                  "0x44F64C1BBF7459A17EFF5404754B51E847225C85FADDFEF46BB699F3D025F3A0",
                                  "0x18C5F23CA6B1ABD077C2B59898FDA94D07B2B5C11E0AB37429467D4BFE725ABD",
                                  "0x63D2B0E7B30AE1CECE9CFA1CB2AA783C9B1DF1F2C378A8813C27897059CDD13E"};
const BUINT8 pubkey_256k1[6][64] = {{0xC8, 0x00, 0xAE, 0xD2, 0x61, 0x2C, 0xB2, 0xB6, 0x2F, 0x4C, 0xED, 0x0C, 0x8E, 0xD5, 0xDF, 0xE7, 0x49, 0xE5, 0x17, 0xC2, 0xCF, 0x1B, 0x31, 0x9C, 0x14, 0xC3, 0x1C, 0x2C, 0x44, 0xE6, 0xA2, 0x9D, 0xEA, 0x68, 0xC3, 0x84, 0x65, 0x02, 0x3E, 0x9C, 0x65, 0xD3, 0x64, 0x02, 0xDA, 0x5E, 0xE4, 0x24, 0xAC, 0x29, 0x55, 0xD9, 0x1A, 0x30, 0xA5, 0x0D, 0x98, 0xE0, 0x94, 0xB4, 0xC2, 0xE9, 0x25, 0x72},
                                    {0x2C, 0x2B, 0x48, 0x68, 0x64, 0x0B, 0xE9, 0x2E, 0x99, 0x3A, 0xE1, 0xC5, 0x62, 0xCD, 0x75, 0xC6, 0xB2, 0xA2, 0x3D, 0xEA, 0x5C, 0x1F, 0x8A, 0xA8, 0x2C, 0x17, 0x82, 0x7D, 0xB7, 0xB1, 0xE8, 0x56, 0x51, 0x9A, 0x6B, 0x54, 0x28, 0xD3, 0xD1, 0x7C, 0x18, 0x37, 0x4F, 0xCC, 0xBF, 0x27, 0x6E, 0x5E, 0x31, 0x10, 0x7C, 0xB9, 0xEA, 0xCD, 0x0B, 0xC0, 0xDC, 0x02, 0x29, 0x56, 0x99, 0x0E, 0xE8, 0xC6},
                                    {0x2A, 0x0A, 0xB7, 0x09, 0xB1, 0x9D, 0x7A, 0x9D, 0xE7, 0x90, 0x4D, 0xE8, 0x69, 0x89, 0xF2, 0x12, 0x9F, 0x54, 0x45, 0x7F, 0x17, 0x60, 0xD1, 0xA0, 0xE9, 0x0A, 0x6D, 0xF0, 0x6D, 0x5F, 0xEA, 0x97, 0x5F, 0x94, 0x1C, 0x0C, 0x62, 0x09, 0xE8, 0x98, 0x2A, 0x31, 0x32, 0xAE, 0x88, 0x06, 0xE3, 0x13, 0x3F, 0xE5, 0xCF, 0x53, 0x7C, 0x5D, 0x73, 0x11, 0x6F, 0x5C, 0x99, 0xC0, 0x1E, 0x79, 0x04, 0x61},
                                    {0x7F, 0xD4, 0xE8, 0x35, 0x9D, 0x60, 0x86, 0xAD, 0xEF, 0xD1, 0xDA, 0xC0, 0x22, 0xA8, 0x64, 0x05, 0xD3, 0x7B, 0x09, 0x79, 0x3E, 0x01, 0x20, 0x09, 0x4B, 0xD2, 0xA5, 0x15, 0x36, 0x0B, 0x15, 0x1E, 0x7D, 0x8E, 0xEB, 0x5F, 0xAD, 0x1D, 0x69, 0x76, 0x98, 0x35, 0x86, 0x54, 0x61, 0xE4, 0xE7, 0x05, 0x1B, 0xBA, 0x2A, 0x15, 0xAF, 0x9B, 0xD7, 0xD2, 0x64, 0x27, 0xFA, 0x76, 0xBB, 0xCE, 0x7C, 0xF2},
                                    {0x36, 0x83, 0x4B, 0x11, 0xF5, 0xA2, 0x06, 0xA9, 0x3F, 0x27, 0xE4, 0x58, 0x7A, 0x34, 0x3E, 0x8C, 0x8C, 0xCC, 0x59, 0x93, 0x21, 0xFE, 0x68, 0xB5, 0x8F, 0xFD, 0x56, 0x56, 0x2D, 0x18, 0x96, 0x91, 0x76, 0xCB, 0xE9, 0x5C, 0xC6, 0xC3, 0x1A, 0x70, 0x0D, 0x4C, 0xE5, 0x2F, 0x73, 0x52, 0x65, 0x33, 0x52, 0x72, 0x80, 0x9D, 0xA6, 0xB2, 0x56, 0xDE, 0xE1, 0x37, 0x41, 0xE7, 0x73, 0x62, 0x93, 0x85},
                                    {0xEE, 0x5F, 0xDF, 0xC6, 0xE5, 0x74, 0xCB, 0x8C, 0x49, 0x6D, 0x5E, 0x32, 0xA0, 0x0F, 0x88, 0x7A, 0x76, 0x91, 0x42, 0x4D, 0x3D, 0x8A, 0xB6, 0xAE, 0xF5, 0x0F, 0xE5, 0xE9, 0x14, 0xF5, 0xDF, 0x09, 0x38, 0xAD, 0x9E, 0x38, 0x36, 0xDF, 0xB7, 0xEE, 0x6C, 0xCE, 0x9E, 0xAA, 0x1E, 0x0F, 0xF6, 0x58, 0xA5, 0x7F, 0x18, 0xF4, 0xC9, 0x62, 0x47, 0xE1, 0xDE, 0x03, 0x8D, 0xE9, 0xCA, 0x89, 0x6E, 0x92}};
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
void get_keypair_external_native_256k1_config(BoatKeypairPriKeyCtx_config *keypair_config, BUINT8 prikeyIndex)
{
    (void)binFormatKey; // avoid warning
    keypair_config->prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config->prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config->prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey[prikeyIndex], TRIMBIN_TRIM_NO, BOAT_FALSE);
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

void get_keypair_external_native_256R1_config(BoatKeypairPriKeyCtx_config *keypair_config, BUINT8 prikeyIndex)
{
    (void)binFormatKey; // avoid warning
    keypair_config->prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config->prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config->prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey[prikeyIndex], TRIMBIN_TRIM_NO, BOAT_FALSE);
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
START_TEST(test_002Keypair_0001CreateOneTimeKeypairSuccess)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 1;
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config, prikey_index);

    /* create keypair */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get keypair content by index */
    creatkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, rtnVal);
    /* chect result of getting keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in keypair struct */
    ck_assert_int_eq(creatkeypair.keypair_index, 0);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(creatkeypair.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key in the struct */
    ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[prikey_index], sizeof(pubkey_256k1[prikey_index])), 0);
    /* don't delete the keypair ,this keypair will be used in the next test case */
    // BoATIotKeypairDelete(0);
    /* free memory of keypair_name*/
    if (creatkeypair.keypair_name != NULL)
    {
        BoatFree(creatkeypair.keypair_name);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create another onetime keypair success
*  this keypair will replace the onetime keypair created by last case.
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0002CreateMoreOneTimeKeypairSuccess)
{
    BSINT32 rtnVal;
    /* using another key to differ from the last one */
    BUINT8 prikey_index = 0;
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config, prikey_index);
    /* create keypair */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime00", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* create keypair */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime01", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get keypair content by index */
    creatkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, rtnVal);
    /* chect result of getting keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in keypair struct */
    ck_assert_int_eq(creatkeypair.keypair_index, 0);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(creatkeypair.keypair_name, "keypairOnetime01"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key in the struct */
    ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[prikey_index], sizeof(pubkey_256k1[prikey_index])), 0);
    /* don't delete the keypair ,this keypair will be used in the next test case */
    // BoATIotKeypairDelete(0);
    /* free memory of keypair_name*/
    if (creatkeypair.keypair_name != NULL)
    {
        BoatFree(creatkeypair.keypair_name);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist keypair success
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0003CreatePersistKeypairSuccess)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 1;
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config, prikey_index);

    /* create keypair , store type must be FLASH*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist01", BOAT_STORE_TYPE_FLASH);
    /* check keypair index ,must be 1 , this is the first persist keypair ,
        index of persist keypair from 1 to BOAT_MAX_KEYPAIR_NUM */
    ck_assert_int_eq(rtnVal, 1);
    /* get keypair content by index */
    creatkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, rtnVal);
    /* chect result of getting keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in keypair struct */
    ck_assert_int_eq(creatkeypair.keypair_index, 1);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(creatkeypair.keypair_name, "keypairPersist01"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key in the struct */
    ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[prikey_index], sizeof(pubkey_256k1[prikey_index])), 0);
    /* don't delete the keypair ,this keypair will be used in the next test case */
    // BoATIotKeypairDelete(0);
    /* free memory of keypair_name*/
    if (creatkeypair.keypair_name != NULL)
    {
        BoatFree(creatkeypair.keypair_name);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create more persist keypair success
*  One persist is created by last case ;
*  will create all the rest of the persist keypairs;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0004CreateMorePersistKeypairSuccess)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 0;
    BCHAR keypairName[64] = {0};
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    for (prikey_index = 2; prikey_index <= BOAT_MAX_KEYPAIR_NUM; prikey_index++)
    {

        /* get config */
        get_keypair_external_native_256k1_config(&keypair_config, prikey_index);
        memset(keypairName, 0x00, sizeof(keypairName));
        snprintf(keypairName, sizeof(keypairName), "keypairPersist%02d", prikey_index);
        /* create keypair , store type must be FLASH*/
        rtnVal = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_FLASH);
        /* check keypair index */
        ck_assert_int_eq(rtnVal, prikey_index);
        /* get keypair content by index */
        creatkeypair.keypair_name = NULL;
        rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, rtnVal);
        /* chect result of getting keypair by index*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check index in keypair struct */
        ck_assert_int_eq(creatkeypair.keypair_index, prikey_index);
        /* check keypair name in the struct */
        ck_assert_int_eq(strcmp(creatkeypair.keypair_name, keypairName), 0);
        /* check keypair format in the struct ,must be the same as in the config */
        ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
        /* check keypair type in the struct ,must be the same as in the config*/
        ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
        /* check public key in the struct */
        ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[prikey_index], sizeof(pubkey_256k1[prikey_index])), 0);
        /* don't delete the keypair ,this keypair will be used in the next test case */
        // BoATIotKeypairDelete(0);
        /* free memory of keypair_name*/
        if (creatkeypair.keypair_name != NULL)
        {
            BoatFree(creatkeypair.keypair_name);
        }
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create more than  BOAT_MAX_KEYPAIR_NUM persist keypair ;
*  Boat allows to create BOAT_MAX_KEYPAIR_NUM persist keypairs, so this case will return error;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0005Create6PersistKeypairSuccess)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 0;
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;

    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config, prikey_index);
    /* create keypair , store type must be FLASH*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist06", BOAT_STORE_TYPE_FLASH);
    /* check keypair result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_KEYPAIR_KEY_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair list  ;
*   Above test cases creat 5 persist keypairs .
*   index of native_demoKey[0] belong to the onetime keypair , the others belong to persist keypairs;
*   The first five of the list are persist keypairs , the last one is onetime keypair
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0006GetKeypairListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 0;
    BCHAR keypairName[64] = {0};
    BoatKeypairPriKeyCtx_config keypair_config;
    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config, prikey_index);

    /* create keypair */
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime01", BOAT_STORE_TYPE_RAM);
    /* check keypair index ,must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    BoatIotKeypairContext keypairList;
    /* get keypair list */
    rtnVal = BoATKeypair_GetKeypairList(&keypairList);
    /* check the result of get the list*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair num*/
    ck_assert_int_eq(keypairList.keypairNum, BOAT_MAX_KEYPAIR_NUM + 1);
    /* check persist keypair num */
    ck_assert_int_eq(keypairList.keypairPersistentNum, BOAT_MAX_KEYPAIR_NUM);
    /* check everyone in the list */
    for (size_t i = 0; i < BOAT_MAX_KEYPAIR_NUM; i++)
    {
        memset(keypairName, 0x00, sizeof(keypairName));
        snprintf(keypairName, sizeof(keypairName), "keypairPersist%02d", i + 1);
        /* keypair index */
        ck_assert_int_eq(keypairList.keypairs[i].prikeyCtx.keypair_index, i + 1);
        /* keypair format */
        ck_assert_int_eq(keypairList.keypairs[i].prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
        /* keypair type*/
        ck_assert_int_eq(keypairList.keypairs[i].prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
        /* keypair name */
        ck_assert_int_eq(strcmp(keypairList.keypairs[i].prikeyCtx.keypair_name, keypairName), 0);
        /* keypair pubkey */
        ck_assert_int_eq(memcmp(keypairList.keypairs[i].prikeyCtx.pubkey_content, pubkey_256k1[i + 1], sizeof(pubkey_256k1[i + 1])), 0);
    }
    /* check the onetime keypair*/
    /* keypair index */
    ck_assert_int_eq(keypairList.keypairs[BOAT_MAX_KEYPAIR_NUM].prikeyCtx.keypair_index, 0);
    /* keypair format */
    ck_assert_int_eq(keypairList.keypairs[BOAT_MAX_KEYPAIR_NUM].prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* keypair type*/
    ck_assert_int_eq(keypairList.keypairs[BOAT_MAX_KEYPAIR_NUM].prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* keypair name */
    ck_assert_int_eq(strcmp(keypairList.keypairs[BOAT_MAX_KEYPAIR_NUM].prikeyCtx.keypair_name, "keypairOnetime01"), 0);
    /* keypair pubkey */
    ck_assert_int_eq(memcmp(keypairList.keypairs[BOAT_MAX_KEYPAIR_NUM].prikeyCtx.pubkey_content, pubkey_256k1[0], sizeof(pubkey_256k1[0])), 0);
    /* release list*/

    BoATKeypair_FreeKeypairContext(keypairList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete persist keypair[3]  ;
*   Above test cases creat 5 persist keypairs .
*   index of native_demoKey[1-5] belong to the  persist keypairs;
*   The first five of the list are persist keypairs , the last one is onetime keypair;
*   Delete the persist keypair that index = 3 ;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0007DeletePersistKeypairSuccess_index3)
{
    BSINT32 rtnVal;
    rtnVal = BoATIotKeypairDelete(3);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get  keypair list ;
*   Above test cases creat 5 persist keypairs  and delete one ;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0008GetKeypairListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BCHAR keypairName[64] = {0};
    BoatIotKeypairContext keypairList;
    /* get keypair list */
    rtnVal = BoATKeypair_GetKeypairList(&keypairList);
    /* check the result of get the list*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair num*/
    ck_assert_int_eq(keypairList.keypairNum, 4);
    /* check persist keypair num */
    ck_assert_int_eq(keypairList.keypairPersistentNum, 4);
    /* check everyone in the list */
    for (size_t i = 0; i < BOAT_MAX_KEYPAIR_NUM - 1; i++)
    {
        index = i + 1;
        if (index > 2)
        {
            index++;
        }
        memset(keypairName, 0x00, sizeof(keypairName));
        snprintf(keypairName, sizeof(keypairName), "keypairPersist%02d", index);
        /* keypair index */
        ck_assert_int_eq(keypairList.keypairs[i].prikeyCtx.keypair_index, index);
        /* keypair format */
        ck_assert_int_eq(keypairList.keypairs[i].prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
        /* keypair type*/
        ck_assert_int_eq(keypairList.keypairs[i].prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
        /* keypair name */
        ck_assert_int_eq(strcmp(keypairList.keypairs[i].prikeyCtx.keypair_name, keypairName), 0);
        /* keypair pubkey */
        ck_assert_int_eq(memcmp(keypairList.keypairs[i].prikeyCtx.pubkey_content, pubkey_256k1[index], sizeof(pubkey_256k1[index])), 0);
    }
    /* release list*/
    BoATKeypair_FreeKeypairContext(keypairList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : creat new persist  keypair  ;
*   Above test cases creat 5 persist keypairs  and delete one which index = 3 ;
*   So the index of the new persist keypair will be 3.
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0009CreatePersistKeypairSuccess)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 3;
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;

    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config, prikey_index);
    /* create keypair , store type must be FLASH*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairPersist03", BOAT_STORE_TYPE_FLASH);
    /* check keypair result , new index must be 3 */
    ck_assert_int_eq(rtnVal, 3);
    /* get keypair content by index */
    creatkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, rtnVal);
    /* chect result of getting keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in keypair struct */
    ck_assert_int_eq(creatkeypair.keypair_index, 3);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(creatkeypair.keypair_name, "keypairPersist03"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key in the struct */
    ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[prikey_index], sizeof(pubkey_256k1[prikey_index])), 0);
    /* free memory of keypair_name*/
    if (creatkeypair.keypair_name != NULL)
    {
        BoatFree(creatkeypair.keypair_name);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : creat new onetime  keypair  ;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0010CreateOnetimeKeypairSuccess)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 0;
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;

    /* get config */
    get_keypair_external_native_256k1_config(&keypair_config, prikey_index);
    /* create keypair , store type must be RAM*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    /* check keypair result , new index must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get keypair content by index */
    creatkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, rtnVal);
    /* chect result of getting keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in keypair struct */
    ck_assert_int_eq(creatkeypair.keypair_index, 0);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(creatkeypair.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key in the struct */
    ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[prikey_index], sizeof(pubkey_256k1[prikey_index])), 0);
    /* free memory of keypair_name*/
    if (creatkeypair.keypair_name != NULL)
    {
        BoatFree(creatkeypair.keypair_name);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get keypair content by index  ;
*  Above test cases creat 5 persist keypairs ;
*  Get all the persist keypairs by index , from 1 to 5;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0011GetKeypairByIndex)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 0;
    BCHAR keypairName[64] = {0};
    BoatKeypairPriKeyCtx creatkeypair;
    for (size_t i = 1; i <= BOAT_MAX_KEYPAIR_NUM; i++)
    {
        memset(keypairName, 0x00, sizeof(keypairName));
        snprintf(keypairName, sizeof(keypairName), "keypairPersist%02d", i);
        /* get keypair content by index */
        creatkeypair.keypair_name = NULL;
        rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, i);
        /* chect result of getting keypair by index*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check index in keypair struct */
        ck_assert_int_eq(creatkeypair.keypair_index, i);
        /* check keypair name in the struct */
        ck_assert_int_eq(strcmp(creatkeypair.keypair_name, keypairName), 0);
        /* check keypair format in the struct ,must be the same as created */
        ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
        /* check keypair type in the struct ,must be the same as created*/
        ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
        /* check public key in the struct */
        ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[i], sizeof(pubkey_256k1[i])), 0);
        /* free memory of keypair_name*/
        if (creatkeypair.keypair_name != NULL)
        {
            BoatFree(creatkeypair.keypair_name);
        }
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete all the persist keypairs  ;
*   Above test cases creat 5 persist keypairs .
*   index of native_demoKey[1-5] belong to the  persist keypairs;
*   Delete all the persist keypair that index from 1 to 5 ;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0012DeleteAllKeypairSuccess)
{
    BSINT32 rtnVal;
    for (size_t i = 1; i <= BOAT_MAX_KEYPAIR_NUM; i++)
    {
        rtnVal = BoATIotKeypairDelete(i);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get  keypair list ;
*   Above test cases deleted all the persist keypairs  ;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0013GetKeypairListSuccess)
{
    BSINT32 rtnVal;
    BoatIotKeypairContext keypairList;
    /* get keypair list */
    rtnVal = BoATKeypair_GetKeypairList(&keypairList);
    /* check the result of get the list*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check keypair num*/
    ck_assert_int_eq(keypairList.keypairNum, 0);
    /* release list*/
    BoATKeypair_FreeKeypairContext(keypairList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create most keypairs ;
*   Above test cases deleted all the persist keypairs  ;
*   Will creat 1 onetime keypair and 5 persist keypairs;
* @return
****************************************************************************************
*/
START_TEST(test_002Keypair_0014CreateAllKeypairSuccess)
{
    BSINT32 rtnVal;
    BUINT8 prikey_index = 0;
    BCHAR keypairName[64] = {0};
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config;
    for (prikey_index = 1; prikey_index <= BOAT_MAX_KEYPAIR_NUM; prikey_index++)
    {

        /* get config */
        get_keypair_external_native_256k1_config(&keypair_config, prikey_index);
        memset(keypairName, 0x00, sizeof(keypairName));
        snprintf(keypairName, sizeof(keypairName), "keypairPersist%02d", prikey_index);
        /* create keypair , store type must be FLASH*/
        rtnVal = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_FLASH);
        /* check keypair index */
        ck_assert_int_eq(rtnVal, prikey_index);
        /* get keypair content by index */
        creatkeypair.keypair_name = NULL;
        rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, rtnVal);
        /* check index in keypair struct */
        ck_assert_int_eq(creatkeypair.keypair_index, prikey_index);
        /* check keypair name in the struct */
        ck_assert_int_eq(strcmp(creatkeypair.keypair_name, keypairName), 0);
        /* check keypair format in the struct ,must be the same as in the config */
        ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
        /* check keypair type in the struct ,must be the same as in the config*/
        ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
        /* check public key in the struct */
        ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[prikey_index], sizeof(pubkey_256k1[prikey_index])), 0);
        /* free memory of keypair_name*/
        if (creatkeypair.keypair_name != NULL)
        {
            BoatFree(creatkeypair.keypair_name);
        }
    }

    /* create onetime keypair*/
    /* onetime keypair selects the first prikey*/
    prikey_index = 0;
    get_keypair_external_native_256k1_config(&keypair_config, prikey_index);
    /* create keypair , store type must be RAM*/
    rtnVal = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    /* check keypair index , index of onetime keypair must be 0*/
    ck_assert_int_eq(rtnVal, 0);
    /* get keypair content by index */
    creatkeypair.keypair_name = NULL;
    rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair, 0);
    /* chect result of getting keypair by index*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in keypair struct */
    ck_assert_int_eq(creatkeypair.keypair_index, prikey_index);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(creatkeypair.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    /* check public key in the struct */
    ck_assert_int_eq(memcmp(creatkeypair.pubkey_content, pubkey_256k1[prikey_index], sizeof(pubkey_256k1[prikey_index])), 0);
    /* free memory of keypair_name*/
    if (creatkeypair.keypair_name != NULL)
    {
        BoatFree(creatkeypair.keypair_name);
    }
}
END_TEST

Suite *make_keypairIntfTest_suite(void)
{
    /* Create Suite */
    Suite *s_keypairUnitTest = suite_create("keypairUnit");

    /* Create test cases */
    TCase *tc_keypairUnitTest_api = tcase_create("keypairUnit_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_keypairUnitTest, tc_keypairUnitTest_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0001CreateOneTimeKeypairSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0002CreateMoreOneTimeKeypairSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0003CreatePersistKeypairSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0004CreateMorePersistKeypairSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0005Create6PersistKeypairSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0006GetKeypairListSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0007DeletePersistKeypairSuccess_index3);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0008GetKeypairListSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0009CreatePersistKeypairSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0010CreateOnetimeKeypairSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0011GetKeypairByIndex);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0012DeleteAllKeypairSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0013GetKeypairListSuccess);
    tcase_add_test(tc_keypairUnitTest_api, test_002Keypair_0014CreateAllKeypairSuccess);

    return s_keypairUnitTest;
}
