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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "boattypes.h"
#include "boatwallet.h"
#include "protocolapi/api_chainmaker.h"
#include "tcase_common.h"
#include "check.h"

static BoatHlchainmakerWallet* g_chaninmaker_wallet_ptr;
static BoatHlchainmakerWalletConfig wallet_config = {0};
#define USE_ONETIME_WALLET

static BOAT_RESULT ChainmakerWalletPrepare(void)
{
    BOAT_RESULT index;

    //set user private key context
    wallet_config.user_prikey_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.user_prikey_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
    wallet_config.user_prikey_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
    wallet_config.user_prikey_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_user_key;
    wallet_config.user_prikey_config.prikey_content.field_len = strlen(chainmaker_user_key) + 1; 

    //set user cert context
    wallet_config.user_cert_content.length = strlen(chainmaker_user_cert);
    memcpy(wallet_config.user_cert_content.content, chainmaker_user_cert, wallet_config.user_cert_content.length);
    
    //set url and name
    strncpy(wallet_config.node_url_arry, test_chainmaker_node_url,   strlen(test_chainmaker_node_url));
    strncpy(wallet_config.host_name_arry, test_chainmaker_host_name, strlen(test_chainmaker_host_name));

    //tls ca cert
    wallet_config.org_tls_ca_cert.length = strlen(chainmaker_tls_ca_cert);
    memcpy(wallet_config.org_tls_ca_cert.content, chainmaker_tls_ca_cert, wallet_config.org_tls_ca_cert.length);

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

static BOAT_RESULT test_contrct_invoke_prepara(BoatHlchainmakerTx  *tx_ptr)
{
    BOAT_RESULT  result  = BOAT_SUCCESS;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare wallet */
    result = ChainmakerWalletPrepare();
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "ChainmakerWalletPrepare() failed.");
        return -1;
    }

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, chain_id, org_id, tx_ptr);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatHlChainmakerTxInit() failed.");
        return -1;
    }

    result = BoatHlchainmakerAddTxParam(tx_ptr, 6, "time", "6543235", "file_hash", "ab3456df5799b87c77e7f85", "file_name", "name005");
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatHlchainmakerAddTxParam() failed.");
        return -1;
    }
    return result;
}
    

START_TEST(test_03Contract_0001InvokeFailureTxNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatInvokeReponse  invoke_reponse;

    result = BoatHlchainmakerContractInvoke(NULL, "save", "fact", true, &invoke_reponse); ;
    ck_assert(result == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_03Contract_0002InvokeFailureMethodNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatInvokeReponse  invoke_reponse;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractInvoke(&tx_ptr, NULL, "fact", true, &invoke_reponse); ;
    ck_assert(result == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_03Contract_0003InvokeFailureContractNull) 
{
    BOAT_RESULT        result;
    BoatHlchainmakerTx tx_ptr;
    BoatInvokeReponse  invoke_reponse;

    result = test_contrct_invoke_prepara(&tx_ptr);
    ck_assert_int_eq(result, BOAT_SUCCESS);

    result = BoatHlchainmakerContractInvoke(&tx_ptr, "save", NULL, true, &invoke_reponse); ;
    ck_assert(result == 0);
}
END_TEST


Suite *make_contract_suite(void) 
{
    /* Create Suite */
    Suite *s_contract = suite_create("contract");

    /* Create test cases */
    TCase *tc_contract_api = tcase_create("contract_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_contract, tc_contract_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_contract_api, test_03Contract_0001InvokeFailureTxNull); 
    tcase_add_test(tc_contract_api, test_03Contract_0002InvokeFailureMethodNull);  
    tcase_add_test(tc_contract_api, test_03Contract_0003InvokeFailureContractNull);  
     
    return s_contract;
}
