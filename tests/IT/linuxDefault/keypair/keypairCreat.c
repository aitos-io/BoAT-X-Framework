/*
 * @Author: zt222 tao.zheng@aitos.io
 * @Date: 2022-10-26 11:22:31
 * @LastEditors: zt222 tao.zheng@aitos.io
 * @LastEditTime: 2022-10-26 16:38:58
 * @FilePath: \BoAT-Engine-pre3.0\tests\IT\linuxDefault\keypair\keypairCreat.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "tcase_keypair.h"
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

const BUINT8 pubkey[] = {0xC8,0x00,0xAE,0xD2,0x61,0x2C,0xB2,0xB6,0x2F,0x4C,0xED,0x0C,0x8E,0xD5,0xDF,0xE7,0x49,0xE5,0x17,0xC2,0xCF,0x1B,0x31,0x9C,0x14,0xC3,0x1C,0x2C,0x44,0xE6,0xA2,0x9D,0xEA,0x68,0xC3,0x84,0x65,0x02,0x3E,0x9C,0x65,0xD3,0x64,0x02,0xDA,0x5E,0xE4,0x24,0xAC,0x29,0x55,0xD9,0x1A,0x30,0xA5,0x0D,0x98,0xE0,0x94,0xB4,0xC2,0xE9,0x25,0x72};
BUINT8 binFormatKey[32]           = {0};
void get_keypair_external_native_256k1_config(BoatKeypairPriKeyCtx_config *keypair_config){
    (void)binFormatKey; //avoid warning
    keypair_config->prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config->prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config->prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config->prikey_content.field_ptr = binFormatKey;
    keypair_config->prikey_content.field_len = 32;
    return ;
}

START_TEST(test_001Keypair_0001CreateOneTimeKeypairSuccess) 
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config ;
    get_keypair_external_native_256k1_config(&keypair_config);



    /* 1. execute unit test */
    rtnVal = BoatKeypairCreate( &keypair_config, "keypairOnetime",BOAT_STORE_TYPE_RAM);

    ck_assert_int_eq(rtnVal, 0);
    rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair,rtnVal);
    ck_assert_int_eq(creatkeypair.keypair_index, 0);
    ck_assert_int_eq(strcmp(creatkeypair.keypair_name,"keypairOnetime") , 0);
    ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    ck_assert_int_eq(memcmp(creatkeypair.pubkey_content,pubkey,sizeof(pubkey)) , 0);
    BoATIotKeypairDelete(0);
}
END_TEST

START_TEST(test_001Keypair_0002CreatePersistKeypairSuccess) 
{
    BSINT32 rtnVal;
    BoatKeypairPriKeyCtx creatkeypair;
    BoatKeypairPriKeyCtx_config keypair_config ;
    get_keypair_external_native_256k1_config(&keypair_config);

    /* 1. execute unit test */
    rtnVal = BoatKeypairCreate( &keypair_config, "keypairPersist01",BOAT_STORE_TYPE_FLASH);

    ck_assert_int_eq(rtnVal, 1);
    rtnVal = BoATKeypair_GetKeypairByIndex(&creatkeypair,rtnVal);
    ck_assert_int_eq(creatkeypair.keypair_index, 1);
    ck_assert_int_eq(strcmp(creatkeypair.keypair_name,"keypairPersist01") , 0);
    ck_assert_int_eq(creatkeypair.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    ck_assert_int_eq(creatkeypair.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1);
    ck_assert_int_eq(memcmp(creatkeypair.pubkey_content,pubkey,sizeof(pubkey)) , 0);
    BoATIotKeypairDelete(1);
}
END_TEST





Suite *make_keypairCreat_suite(void) 
{
    /* Create Suite */
    Suite *s_keypairCreat = suite_create("keypairCreat");

    /* Create test cases */
    TCase *tc_keypairCreat_api = tcase_create("keypairCreat_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_keypairCreat, tc_keypairCreat_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0001CreateOneTimeKeypairSuccess);
    tcase_add_test(tc_keypairCreat_api, test_001Keypair_0002CreatePersistKeypairSuccess);


    return s_keypairCreat;
}