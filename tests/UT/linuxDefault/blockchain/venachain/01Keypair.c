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
#include "tcase_venachain.h"

#define TEST_GAS_LIMIT "0x6691B7"
#define TEST_GAS_PRICE "0x4A817C800"
#define TEST_IS_SYNC_TX BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS "0x16E598Eba11d0909ee4B3D439f2477dCeE126BCE"

BUINT8 g_binFormatKey[32];
BoatKeypairPriKeyCtx_config g_keypair_config;
BoatVenachainNetworkConfig g_venachain_network_config;

BoatKeypairPriKeyCtx_config get_venachain_keypair_settings()
{
    // set user private key context

    if (TEST_KEY_TYPE == "BOAT_WALLET_PRIKEY_FORMAT_NATIVE")
    {
        g_keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
        g_keypair_config.prikey_content.field_ptr = g_binFormatKey;
        g_keypair_config.prikey_content.field_len = 32;
    }
    else
    {
        g_keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        g_keypair_config.prikey_content.field_ptr = (BUINT8 *)g_venachain_private_key_buf;
        g_keypair_config.prikey_content.field_len = strlen(g_venachain_private_key_buf) + 1;
    }
    g_keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    g_keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    return g_keypair_config;
}

BOAT_RESULT check_venachain_keypairCtx(BoatKeypairPriKeyCtx *keypair2Ctx, BUINT8 keypair1Index, BCHAR *keypair1Name, BoatKeypairPriKeyCtx_config *keypair1Config)
{
    BOAT_RESULT ret = BOAT_ERROR;
    BCHAR temBuf[15] = {0};
    BCHAR *temP = keypair1Name;
    if (keypair1Name == NULL)
    {
        memcpy(temBuf, "keypair", strlen("keypair"));
        temBuf[strlen("keypair")] = 0x30 + keypair1Index;
        temP = temBuf;
    }

    if ((keypair2Ctx->keypair_index != keypair1Index) ||
        (0 != strcmp(keypair2Ctx->keypair_name, temP)) ||
        (keypair2Ctx->prikey_format != keypair1Config->prikey_format) ||
        (keypair2Ctx->prikey_type != keypair1Config->prikey_type))
    {
        return ret;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT check_venachain_keypair_in_keypairList(BoatKeypairPriKeyCtx *keypair, BoatIotKeypairContext *keypair_list)
{
    BOAT_RESULT ret = BOAT_ERROR;

    BUINT8 loop = keypair_list->keypairNum;
    BSINT8 index = -1;

    if (loop == 0)
    {
        return ret;
    }

    for (BUINT8 i = 0; i < loop; i++)
    {
        if (keypair_list->keypairs[i].prikeyCtx.keypair_index == keypair->keypair_index)
        {
            index = i;
            break;
        }
    }

    if (index < 0)
    {

        return ret;
    }

    if (keypair_list->keypairs[index].prikeyCtx.prikey_format != keypair->prikey_format)
    {

        return ret;
    }

    if ((0 != strncmp(keypair_list->keypairs[index].prikeyCtx.keypair_name, keypair->keypair_name, strlen(keypair->keypair_name))) || (strlen(keypair_list->keypairs[index].prikeyCtx.keypair_name) != strlen(keypair->keypair_name)))
    {
        return ret;
    }

    if (keypair_list->keypairs[index].prikeyCtx.prikey_type != keypair->prikey_type)
    {
        return ret;
    }

    if (0 != memcmp(keypair_list->keypairs[index].prikeyCtx.pubkey_content, keypair->pubkey_content, 64))
    {
        return ret;
    }

    return BOAT_SUCCESS;
}

START_TEST(test_001Keypair_0001CreateOneTimeKeypairSuccessNullName)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BoatKeypairPriKeyCtx keypair2_ctx;

    BoatIotSdkInit();
    /* 1. Create keypair1 */
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, NULL, BOAT_STORE_TYPE_RAM);
#if 1
    ck_assert_int_eq(keypair1_index, 0);

    /* 2.Verify keypair1 */
    keypair2_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair2_ctx, 0);
    ck_assert(ret == BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair2_ctx, keypair1_index, NULL, &keypair1_config);
    ck_assert(ret == BOAT_SUCCESS);

    /* 3.Check keypair list */
    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert(ret == BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list);
    ck_assert(ret == BOAT_SUCCESS);
    /* free memory of keypair_name*/
    if (keypair2_ctx.keypair_name != NULL)
    {
        BoatFree(keypair2_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);
#endif
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0002CreateOneTimeKeypairSuccessWithName)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BoatKeypairPriKeyCtx keypair2_ctx;

    BoatIotSdkInit();
    /* 1. Create keypair1 */
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, "oneTime_keypair", BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair1_index, 0);

    /* 2.Verify keypair1 */
    keypair2_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair2_ctx, 0);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair2_ctx, keypair1_index, "oneTime_keypair", &keypair1_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 3.Check keypair list */
    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    /* free memory of keypair_name*/
    if (keypair2_ctx.keypair_name != NULL)
    {
        BoatFree(keypair2_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0003CreateOneTimeKeypairSuccessTwice)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BOAT_RESULT keypair2_index;
    BoatKeypairPriKeyCtx keypair_ctx;

    BoatIotSdkInit();
    /* 1. Create keypair1 */
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, NULL, BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair1_index, 0);

    /* 2. Create keypair2 */
    BoatKeypairPriKeyCtx_config keypair2_config = get_venachain_keypair_settings();
    keypair2_index = BoatKeypairCreate(&keypair2_config, "oneTime_keypair", BOAT_STORE_TYPE_RAM);
#if 1
    ck_assert_int_eq(keypair2_index, 0);

    /* 3.Verify keypair */
    keypair_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair_ctx, 0);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair_ctx, keypair2_index, "oneTime_keypair", &keypair2_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.Check keypair list */
    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    /* free memory of keypair_name*/
    if (keypair_ctx.keypair_name != NULL)
    {
        BoatFree(keypair_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);
#endif
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0004CreateOneTimeWalletFailureNullConfig)
{
    BOAT_RESULT keypair_index;
    BoatIotSdkInit();

    /* Create keypair */
    BoatKeypairPriKeyCtx_config *keypair_config = NULL;
    keypair_index = BoatKeypairCreate(keypair_config, "oneTime_keypair", BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0005CreateOneTimeWalletFailureWrongGenModeConfig)
{
    BOAT_RESULT keypair_index;
    BoatIotSdkInit();

    /* Create keypair */
    BoatKeypairPriKeyCtx_config keypair_config;

    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = 0;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config, "oneTime_keypair", BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, BOAT_ERROR_KEYPAIR_KEY_GENMODE_ERR);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0006CreateOneTimeWalletFailureWrongKeyTypeConfig)
{
    BOAT_RESULT keypair_index;
    BoatIotSdkInit();

    /* Create keypair */
    BoatKeypairPriKeyCtx_config keypair_config;

    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
    keypair_config.prikey_type = 0;

    keypair_index = BoatKeypairCreate(&keypair_config, "oneTime_keypair", BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, BOAT_ERROR_KEYPAIR_KEY_TYPE_ERR);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0007CreateOneTimeWalletFailureWrongKeyFormatConfig)
{
    BOAT_RESULT keypair_index;
    BoatIotSdkInit();

    /* Create keypair */
    BoatKeypairPriKeyCtx_config keypair_config;

    keypair_config.prikey_format = 0;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config, "oneTime_keypair", BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, BOAT_ERROR_KEYPAIR_KEY_FORMAT_ERR);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0008CreateOneTimeWalletFailureWrongNativeKeyLenConfig)
{
    BOAT_RESULT keypair_index;
    BoatIotSdkInit();

    /* Create keypair */
    BoatKeypairPriKeyCtx_config keypair_config;

    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 513;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config, "oneTime_keypair", BOAT_STORE_TYPE_RAM);
#if 0
    ck_assert_int_eq(keypair_index, BOAT_ERROR_COMMON_OUT_OF_MEMORY);
#endif
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0009CreateOneTimeWalletFailureWrongPKCSKeyLenConfig)
{
    BOAT_RESULT keypair_index;
    BoatIotSdkInit();

    /* Create keypair */
    BoatKeypairPriKeyCtx_config keypair_config;

    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)g_venachain_private_key_buf;
    keypair_config.prikey_content.field_len = 513;

    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config, "oneTime_keypair", BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, BOAT_ERROR_COMMON_OUT_OF_MEMORY);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0010CreatePersistKeypairSuccess)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BoatKeypairPriKeyCtx keypair2_ctx;

    BoatIotSdkInit();
    /* 1. Create keypair1 */
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, "Persistkeypair1", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(keypair1_index, 5);
    ck_assert_int_gt(keypair1_index, 0);

    /* 2.Verify keypair1 */
    keypair2_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair2_ctx, keypair1_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair2_ctx, keypair1_index, "Persistkeypair1", &keypair1_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 3.Check keypair list */
    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    /* free memory of keypair_name*/
    if (keypair2_ctx.keypair_name != NULL)
    {
        BoatFree(keypair2_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);

    /* 4.Delete the keypair */
    ret = BoATIotKeypairDelete(keypair1_index); // To prevent the impact on subsequent test cases
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0011Create2PersistKeypairSuccess)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BOAT_RESULT keypair2_index;
    BoatKeypairPriKeyCtx keypair3_ctx;
    BoatKeypairPriKeyCtx keypair4_ctx;

    BoatIotSdkInit();
    /* 1. Create keypair1 */
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, "Persistkeypair1", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(keypair1_index, 5);
    ck_assert_int_gt(keypair1_index, 0);

    /* 2. Create keypair2 */
    BoatKeypairPriKeyCtx_config keypair2_config = get_venachain_keypair_settings();
    keypair2_index = BoatKeypairCreate(&keypair2_config, "Persistkeypair2", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(keypair2_index, 5);
    ck_assert_int_gt(keypair2_index, 0);

    /* 3.Check whether keypair1_index and keypair2_index are equal */
    ck_assert_int_ne(keypair1_index, keypair2_index);

    /* 4.Verify keypair */
    keypair3_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair3_ctx, keypair1_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair3_ctx, keypair1_index, "Persistkeypair1", &keypair1_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    keypair4_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair4_ctx, keypair2_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair4_ctx, keypair2_index, "Persistkeypair2", &keypair2_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 5.Check keypair list */
    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_venachain_keypair_in_keypairList(&keypair3_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_venachain_keypair_in_keypairList(&keypair4_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    /* free memory of keypair_name*/
    if (keypair3_ctx.keypair_name != NULL)
    {
        BoatFree(keypair3_ctx.keypair_name);
    }
    if (keypair4_ctx.keypair_name != NULL)
    {
        BoatFree(keypair4_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);

    /* 6.Delete keypairs*/
    ret = BoATIotKeypairDelete(keypair1_index); // To prevent the impact on subsequent test cases
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = BoATIotKeypairDelete(keypair2_index); // To prevent the impact on subsequent test cases
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0012DeleteOnetimeKeypairSuccess)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BoatKeypairPriKeyCtx keypair2_ctx;

    BoatIotSdkInit();

    /* 1.Execute 001_0001*/
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, NULL, BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair1_index, 0);

    keypair2_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair2_ctx, 0);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair2_ctx, keypair1_index, NULL, &keypair1_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 2.Delete Onetime-keypair*/
    ret = BoATIotKeypairDelete(0);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotKeypairContext keypair_list1;
    ret = BoATKeypair_GetKeypairList(&keypair_list1);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list1);
    ck_assert_int_eq(ret, BOAT_ERROR);
    /* free memory of keypair_name*/
    if (keypair2_ctx.keypair_name != NULL)
    {
        BoatFree(keypair2_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);
    BoATKeypair_FreeKeypairContext(keypair_list1);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0013DeleteOnetimeKeypairFailureTwice)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BoatKeypairPriKeyCtx keypair2_ctx;

    BoatIotSdkInit();

    /* 1.Execute 002_0001*/
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, NULL, BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair1_index, 0);

    keypair2_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair2_ctx, 0);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair2_ctx, keypair1_index, NULL, &keypair1_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = BoATIotKeypairDelete(0);
    ck_assert_int_eq(ret, BOAT_SUCCESS); // first delete

    BoatIotKeypairContext keypair_list1;
    ret = BoATKeypair_GetKeypairList(&keypair_list1);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list1);
    ck_assert_int_eq(ret, BOAT_ERROR);
    /* free memory of keypair_name*/
    if (keypair2_ctx.keypair_name != NULL)
    {
        BoatFree(keypair2_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);
    BoATKeypair_FreeKeypairContext(keypair_list1);

    /* 2. Delete again*/
    ret = BoATIotKeypairDelete(0);
    ck_assert_int_eq(ret, BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0014DeleteOnetimeKeypairFailureThenRecover)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BoatKeypairPriKeyCtx keypair2_ctx;

    BoatIotSdkInit();

    /* 1.Execute 001_0001*/
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, NULL, BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair1_index, 0);

    keypair2_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair2_ctx, 0);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair2_ctx, keypair1_index, NULL, &keypair1_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    /* free memory of keypair_name*/
    if (keypair2_ctx.keypair_name != NULL)
    {
        BoatFree(keypair2_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);

    /* 2.Delete Onetime-keypair*/
    ret = BoATIotKeypairDelete(0);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    //!@todo

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0015DeleteKeypairFailureWrongIndex)
{
    BOAT_RESULT ret;

    BoatIotSdkInit();

    ret = BoATIotKeypairDelete(5);
    ck_assert_int_eq(ret, BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0016DeletePersistKeypairSuccess)
{
    BOAT_RESULT ret;
    BOAT_RESULT keypair1_index;
    BoatKeypairPriKeyCtx keypair2_ctx;

    BoatIotSdkInit();

    /* 1.Execute 001_0010*/
    BoatKeypairPriKeyCtx_config keypair1_config = get_venachain_keypair_settings();
    keypair1_index = BoatKeypairCreate(&keypair1_config, "Persistkeypair1", BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(keypair1_index, 5);
    ck_assert_int_gt(keypair1_index, 0);

    keypair2_ctx.keypair_name = NULL;
    ret = BoATKeypair_GetKeypairByIndex(&keypair2_ctx, keypair1_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypairCtx(&keypair2_ctx, keypair1_index, "Persistkeypair1", &keypair1_config);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 2.Delete the keypair */
    ret = BoATIotKeypairDelete(keypair1_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 3.Check the keypairList*/
    BoatIotKeypairContext keypair_list1;
    ret = BoATKeypair_GetKeypairList(&keypair_list1);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    ret = check_venachain_keypair_in_keypairList(&keypair2_ctx, &keypair_list1);
    ck_assert_int_eq(ret, BOAT_ERROR);
    /* free memory of keypair_name*/
    if (keypair2_ctx.keypair_name != NULL)
    {
        BoatFree(keypair2_ctx.keypair_name);
    }
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);
    BoATKeypair_FreeKeypairContext(keypair_list1);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_001Keypair_0017DeletePersistKeypairFailureNonExistentIndex)
{
    BOAT_RESULT ret;
    BUINT8 i, j, pickNum;

    BoatIotSdkInit();

    /* 1.Pick a NonExistent keypairIndex*/
    BoatIotKeypairContext keypair_list;
    ret = BoATKeypair_GetKeypairList(&keypair_list);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BUINT8 keypairNum = keypair_list.keypairNum;
    for (i = 1; i < 5; i++)
    {
        for (j = 0; j < keypairNum; j++)
        {
            if (i == keypair_list.keypairs->prikeyCtx.keypair_index)
            {
                break;
            }
        }

        if (j >= keypairNum)
        {
            pickNum = i;
            break;
        }
    }

    ck_assert_int_lt(pickNum, 5);
    ck_assert_int_gt(pickNum, 0);
    /* deinit keypair list*/
    BoATKeypair_FreeKeypairContext(keypair_list);

    /* 2.Delete the keypair */
    ret = BoATIotKeypairDelete(pickNum);
    ck_assert_int_eq(ret, BOAT_ERROR_KEYPAIR_KEY_INEXISTENCE);

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

    tcase_add_test(tc_keypair_api, test_001Keypair_0001CreateOneTimeKeypairSuccessNullName);
    tcase_add_test(tc_keypair_api, test_001Keypair_0002CreateOneTimeKeypairSuccessWithName);
    tcase_add_test(tc_keypair_api, test_001Keypair_0003CreateOneTimeKeypairSuccessTwice);
    tcase_add_test(tc_keypair_api, test_001Keypair_0004CreateOneTimeWalletFailureNullConfig);
    tcase_add_test(tc_keypair_api, test_001Keypair_0005CreateOneTimeWalletFailureWrongGenModeConfig);
    tcase_add_test(tc_keypair_api, test_001Keypair_0006CreateOneTimeWalletFailureWrongKeyTypeConfig);
    tcase_add_test(tc_keypair_api, test_001Keypair_0007CreateOneTimeWalletFailureWrongKeyFormatConfig);
    tcase_add_test(tc_keypair_api, test_001Keypair_0008CreateOneTimeWalletFailureWrongNativeKeyLenConfig);
    tcase_add_test(tc_keypair_api, test_001Keypair_0009CreateOneTimeWalletFailureWrongPKCSKeyLenConfig);
    tcase_add_test(tc_keypair_api, test_001Keypair_0010CreatePersistKeypairSuccess);
    tcase_add_test(tc_keypair_api, test_001Keypair_0011Create2PersistKeypairSuccess);
    tcase_add_test(tc_keypair_api, test_001Keypair_0012DeleteOnetimeKeypairSuccess);
    tcase_add_test(tc_keypair_api, test_001Keypair_0013DeleteOnetimeKeypairFailureTwice);
    tcase_add_test(tc_keypair_api, test_001Keypair_0014DeleteOnetimeKeypairFailureThenRecover);
    tcase_add_test(tc_keypair_api, test_001Keypair_0015DeleteKeypairFailureWrongIndex);
    tcase_add_test(tc_keypair_api, test_001Keypair_0016DeletePersistKeypairSuccess);
    tcase_add_test(tc_keypair_api, test_001Keypair_0017DeletePersistKeypairFailureNonExistentIndex);

    return s_keypair;
}
