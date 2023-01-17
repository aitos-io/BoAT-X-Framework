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

#include "check.h"
#include "tcase_platone.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>

/* extern suite declaration */
extern Suite *make_keypair_suite(void);
extern Suite *make_network_suite(void);
extern Suite *make_wallet_suite(void);
extern Suite *make_parameters_suite(void);
extern Suite *make_chainAndContract_suite(void);

char g_platone_private_key_buf[1024];

int read_key_content(char* key_ptr)
{       
    int fd = 0;
    int len;

    if (key_ptr == NULL)
    {
        return -1;
    }

    if (TEST_KEY_TYPE == "BOAT_WALLET_PRIKEY_FORMAT_NATIVE")
    {
        fd = open("../../../../../../tests/UT/linuxDefault/blockchain/platone/pri_key/native.key", O_RDONLY);
        if (fd < 0)
        {
            return -1;
        }
        BoatLog(BOAT_LOG_VERBOSE, "Current key type is native");
    }
    else
    {
        fd = open("../../../../../../tests/UT/linuxDefault/blockchain/platone/pri_key/pkcs_key.key", O_RDONLY);
        if (fd < 0)
        {
            return -1;
        }
        BoatLog(BOAT_LOG_VERBOSE, "Current key type is PKCS");
    }
    len = read(fd, key_ptr, 1024);
    if (len < 0)
    {
        return -1;
    }
   
    return 0;
}

int main(int argc, char *argv[])
{
    SRunner *sr = NULL;
    int failed_number = 0;
    int ret = 0;

    /* new adding test suite should create in here */
    Suite *suite_keypair = make_keypair_suite();
    Suite *suite_network = make_network_suite();
    Suite *suite_wallet = make_wallet_suite();
    Suite *suite_parameters = make_parameters_suite();
    Suite *suite_chainandcontract= make_chainAndContract_suite();

    read_key_content(g_platone_private_key_buf);

    /* create srunner and add first suite to it.
    The first suite in a suite runner is always added in function srunner_create,
    here set suite_wallet as first adding suite. */
    sr = srunner_create(suite_keypair);
   
    /* set generate test log in running path */
    srunner_set_log(sr, "test_report_platone.txt");
    /* add other suite to srunner, more test suite should be add in here */
    srunner_add_suite(sr, suite_network);

    srunner_add_suite(sr, suite_wallet);

    srunner_add_suite(sr, suite_parameters);
    
    srunner_add_suite(sr, suite_chainandcontract);

    /* start to run all test case */
    srunner_run_all(sr, CK_NORMAL);
    /* get the failed unit test number */
    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    /* return failed unit number, this return value can be used in CI, CI tools according
    to this return value determine the test is passed or not  */
    return failed_number;
}
