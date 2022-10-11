/******************************************************************************
 * Copyright (C) 2018-2022 aitos.io
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
#include "tcase_chainmaker.h"
#define EXCEED_STR_MAX_LEN 4097

void check_keypair_data(BoatKeypairDataCtx *keypair_data)
{
    ck_assert_int_eq(keypair_data->prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);

    ck_assert_int_eq(keypair_data->prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
}


START_TEST(test_001keypair_0001CreateOneTimeKeypairExternInjectionSuccess) 
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BSINT32 index;

    BoatIotSdkInit();
    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    index = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(index, 0);


    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0002CreateOneTimeKeypairInternalGenrationSuccess) 
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BSINT32 index;

    BoatIotSdkInit();
    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    index = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(index, 0);


    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0003CreateOneTimeKeypairFailedWithErrKeypairConfig) 
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BSINT32 index;

    BoatIotSdkInit();
    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_UNKNOWN;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    index = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    ck_assert_int_ne(index, 0);

    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_UNKNOWN;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator
    index = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    ck_assert_int_ne(index, 0);

    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type     = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format   = BOAT_KEYPAIR_PRIKEY_FORMAT_UNKNOWN;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator
    index = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    ck_assert_int_ne(index, 0);

    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type     = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format   = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = NULL;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator
    index = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    ck_assert_int_ne(index, 0);

    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type     = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format   = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = 0; //length contain terminator
    index = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
    ck_assert_int_ne(index, 0);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0004CreateOneTimeKeypairSuccessWithNullkeypairName) 
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BSINT32 index;

    BoatIotSdkInit();
    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    index = BoatKeypairCreate(&keypair_config, NULL, BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(index, 0);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0005CreateFiveKeypairSuccess) 
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BoatIotKeypairContext keypairList;
    BSINT32 index;
    BSINT32 result;

    BoatIotSdkInit();
    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 1);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 2);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 3);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 4);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 5);

    result = BoATKeypair_GetKeypairList(&keypairList);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(keypairList.keypairNum, 5);

    check_keypair_data(&keypairList.keypairs[0]);
    check_keypair_data(&keypairList.keypairs[1]);
    check_keypair_data(&keypairList.keypairs[2]);
    check_keypair_data(&keypairList.keypairs[3]);
    check_keypair_data(&keypairList.keypairs[4]);

    BoATKeypair_FreeKeypairContext(keypairList);

    result = BoATIotKeypairDelete(1);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(2);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(3);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(4);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(5);
    ck_assert_int_eq(result, 0);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0006CreateSixKeypairFailed) 
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BSINT32 index;
    BSINT32 result;

    BoatIotSdkInit();
    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 1);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 2);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 3);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 4);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 5);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, -1);

    result = BoATIotKeypairDelete(1);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(2);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(3);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(4);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(5);
    ck_assert_int_eq(result, 0);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001keypair_0007CreateFivePersistKeypairDeletefour) 
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BSINT32 index;
    BSINT32 result;

    BoatIotSdkInit();
    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 1);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 2);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 3);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 4);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 5);

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, -1);

    result = BoATIotKeypairDelete(1);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(2);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(3);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(4);
    ck_assert_int_eq(result, 0);

    result = BoATIotKeypairDelete(5);
    ck_assert_int_eq(result, 0);

    BoatIotSdkDeInit();
   
}
END_TEST

START_TEST(test_001keypair_0008CreateFivePersistKeypairDeleteThreeWriteTwo) 
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BSINT32 index;
    BSINT32 result;

    BoatIotSdkInit();
    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    index = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(index, 1);

    result = BoATIotKeypairDelete(1);
    ck_assert_int_eq(result, 0);

    BoatIotSdkDeInit();
}
END_TEST

Suite *make_keypair_suite(void) 
{
    /* Create Suite */
    Suite *s_keypair = suite_create("keypair");

    /* Create test cases */
    TCase *tc_keypair_api = tcase_create("keypair_api");
    /* Add a test case to the Suite */
    suite_add_tcase(s_keypair, tc_keypair_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_keypair_api, test_001keypair_0001CreateOneTimeKeypairExternInjectionSuccess); 
    tcase_add_test(tc_keypair_api, test_001keypair_0002CreateOneTimeKeypairInternalGenrationSuccess); 
    tcase_add_test(tc_keypair_api, test_001keypair_0003CreateOneTimeKeypairFailedWithErrKeypairConfig); 
    tcase_add_test(tc_keypair_api, test_001keypair_0004CreateOneTimeKeypairSuccessWithNullkeypairName); 
    tcase_add_test(tc_keypair_api, test_001keypair_0005CreateFiveKeypairSuccess); 
    tcase_add_test(tc_keypair_api, test_001keypair_0006CreateSixKeypairFailed); 
    tcase_add_test(tc_keypair_api, test_001keypair_0007CreateFivePersistKeypairDeletefour); 
    tcase_add_test(tc_keypair_api, test_001keypair_0008CreateFivePersistKeypairDeleteThreeWriteTwo);

    return s_keypair;
}
