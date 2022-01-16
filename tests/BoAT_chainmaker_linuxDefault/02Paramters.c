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
#include "tcase_common.h"

static BoatHlchainmakerWallet* g_chaninmaker_wallet_ptr;
static BoatHlchainmakerWalletConfig wallet_config = {0};
#define USE_ONETIME_WALLET

static BOAT_RESULT chainmakerWalletPrepare(void)
{
    BOAT_RESULT index;

    //set user private key context
    wallet_config.user_prikey_cfg.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.user_prikey_cfg.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
    wallet_config.user_prikey_cfg.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
    wallet_config.user_prikey_cfg.prikey_content.field_ptr = (BUINT8 *)chainmaker_sign_key_buf;
    wallet_config.user_prikey_cfg.prikey_content.field_len = strlen(chainmaker_sign_key_buf) + 1; 

    //set user cert context
    wallet_config.user_cert_cfg.length = strlen(chainmaker_sign_cert_buf);
    memcpy(wallet_config.user_cert_cfg.content, chainmaker_sign_cert_buf, wallet_config.user_cert_cfg.length);

     //tls ca cert
    wallet_config.tls_ca_cert_cfg.length = strlen(chainmaker_ca_cert_buf) + 1;
    memcpy(wallet_config.tls_ca_cert_cfg.content, chainmaker_ca_cert_buf, wallet_config.tls_ca_cert_cfg.length);

    strncpy(wallet_config.node_url_cfg,  TEST_CHAINMAKER_NODE_URL,  strlen(TEST_CHAINMAKER_NODE_URL));
    strncpy(wallet_config.host_name_cfg, TEST_CHAINMAKER_HOST_NAME, strlen(TEST_CHAINMAKER_HOST_NAME));
    strncpy(wallet_config.chain_id_cfg,  TEST_CHAINMAKER_CHAIN_ID,  strlen(TEST_CHAINMAKER_CHAIN_ID));
    strncpy(wallet_config.org_id_cfg,    TEST_CHAINMAKER_ORG_ID,    strlen(TEST_CHAINMAKER_ORG_ID));
    // create wallet
#if defined(USE_ONETIME_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
#elif defined(USE_CREATE_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker.cfg", &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
#elif defined(USE_LOAD_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker.cfg", NULL, sizeof(BoatHlchainmakerWalletConfig));
#else
    return BOAT_ERROR;
#endif
    if (index == BOAT_ERROR)
    {
        return BOAT_ERROR;
    }
    
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(index);
    if (g_chaninmaker_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}


static BOAT_RESULT param_init_check(BoatHlchainmakerTx* tx_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if (tx_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    result = strncmp(tx_ptr->wallet_ptr->node_info.chain_id_info, TEST_CHAINMAKER_CHAIN_ID, strlen(TEST_CHAINMAKER_CHAIN_ID));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    result = strncmp(tx_ptr->wallet_ptr->node_info.org_id_info, TEST_CHAINMAKER_ORG_ID, strlen(TEST_CHAINMAKER_ORG_ID));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    if (tx_ptr->wallet_ptr != g_chaninmaker_wallet_ptr)
    {
         return BOAT_ERROR;
    }

    return result;
}


static BOAT_RESULT param_add_check(BoatHlchainmakerTx* tx_ptr, int num)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if ((tx_ptr->trans_para.n_parameters != num) || (num > 4))
    {
        return BOAT_ERROR;
    }

    result = strncmp(tx_ptr->trans_para.parameters[0].key, "key1", strlen("key1"));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    result = strncmp(tx_ptr->trans_para.parameters[0].value, "value1", strlen("value1"));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    if (num == 1)
    {
        return result;
    }

    result = strncmp(tx_ptr->trans_para.parameters[1].key, "key2", strlen("key2"));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    result = strncmp(tx_ptr->trans_para.parameters[1].value, "value2", strlen("value2"));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    if (num == 2)
    {
        return result;
    }

    result = strncmp(tx_ptr->trans_para.parameters[2].key, "key3", strlen("key3"));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }  

    result = strncmp(tx_ptr->trans_para.parameters[2].value, "value3", strlen("value3"));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }
    if (num == 3)
    {
        return result;
    }

    result = strncmp(tx_ptr->trans_para.parameters[3].key, "key4", strlen("key4"));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    result = strncmp(tx_ptr->trans_para.parameters[3].value, "value4", strlen("value4"));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    return result;
}

START_TEST(test_002Parameters_0001TxinitSuccess) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;
    rtnVal = chainmakerWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, &tx_ptr);
    ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(param_init_check(&tx_ptr) == BOAT_SUCCESS);

}
END_TEST

START_TEST(test_002Parameters_0002TxinitxFailureNullpara) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;
    rtnVal = chainmakerWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatHlChainmakerTxInit(NULL, &tx_ptr);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);

    rtnVal = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, NULL);
    ck_assert(rtnVal == BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST


START_TEST(test_002Parameters_0003AddTxParamSuccess) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 6, "key1", "value1", "key2", "value2", 
                                                    "key3", "value3", NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    ck_assert_int_eq(param_add_check(&tx_ptr, 3), BOAT_SUCCESS);
}
END_TEST


START_TEST(test_002Parameters_0004AddTxParamFailureShortParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 6, "key1", "value1", "key2", "value2", "key3", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_002Parameters_0005AddTxParamFailureLongParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 12, "key1", "value1", "key2", "value2", "key3", "value3", 
                                                     "key4", "value4", "key5", "value5", "key6", "value6", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_002Parameters_0006AddTxParamFailureOddParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 9, "key1", "value1", "key2", "value2", "key3", "value3", 
                                                     "key4", "value4", "key5", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_002Parameters_0007AddTxParamSucessNumberNULLParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 0, NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    ck_assert_int_eq(tx_ptr.trans_para.n_parameters, 0);
}
END_TEST

START_TEST(test_002Parameters_0008AddTxParamFailureTxNULLParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(NULL, 6, "key1", "vlaue1", "key2", "vlaue2", "key3", "value3", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_002Parameters_0009AddTxParamSucessTxLessMaxEvenParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 8, "key1", "value1", "key2", "value2", "key3", "value3", 
                                                    "key4", "value4",  NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    ck_assert_int_eq(param_add_check(&tx_ptr, 4), BOAT_SUCCESS);
}
END_TEST

START_TEST(test_002Parameters_0010AddTxParamFailureMoreMaxOddParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 11, "key1", "value1", "key2", "value2", "key3", "value3", 
                                                     "key4", "value4", "key5", "value5", "key6", NULL);
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST


Suite *make_parameters_suite(void) 
{
    /* Create Suite */
    Suite *s_param = suite_create("param");

    /* Create test cases */
    TCase *tc_param_api = tcase_create("param_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_param, tc_param_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_param_api, test_002Parameters_0001TxinitSuccess);  
    tcase_add_test(tc_param_api, test_002Parameters_0002TxinitxFailureNullpara);  
    tcase_add_test(tc_param_api, test_002Parameters_0003AddTxParamSuccess);  
    tcase_add_test(tc_param_api, test_002Parameters_0004AddTxParamFailureShortParam);  
    tcase_add_test(tc_param_api, test_002Parameters_0005AddTxParamFailureLongParam); 
    tcase_add_test(tc_param_api, test_002Parameters_0006AddTxParamFailureOddParam);   
    tcase_add_test(tc_param_api, test_002Parameters_0007AddTxParamSucessNumberNULLParam);  
    tcase_add_test(tc_param_api, test_002Parameters_0008AddTxParamFailureTxNULLParam); 
    tcase_add_test(tc_param_api, test_002Parameters_0009AddTxParamSucessTxLessMaxEvenParam);  
    tcase_add_test(tc_param_api, test_002Parameters_0010AddTxParamFailureMoreMaxOddParam);  

    return s_param;
}

