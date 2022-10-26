/*
 * @Author: zt222 tao.zheng@aitos.io
 * @Date: 2022-10-26 10:37:51
 * @LastEditors: zt222 tao.zheng@aitos.io
 * @LastEditTime: 2022-10-26 11:45:51
 * @FilePath: \BoAT-Engine-pre3.0\tests\IT\linuxDefault\keypair\tcase_entry.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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

#include "check.h"
#include "tcase_keypair.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>


Suite *make_keypairCreat_suite(void);

int read_file_content(char *file,char* file_ptr)
{       
    int fd = 0;
    int len;

    if ((file == NULL))
    {
        printf("file is NULL \n");
        return -1;
    }

    fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        printf("open file fail , fd = %d \n",fd);
        return -1;
    }

    len = read(fd, file_ptr, 1024);
    if (len < 0)
    {
        printf("read file fail , len = %d \n",len);
        return -1;
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    SRunner *sr       = NULL;
    int failed_number = 0;
    int ret = 0;

    /* new adding test suite should create in here */
    Suite *suite_keypairCreat    = make_keypairCreat_suite();
    // Suite *suite_transaction = make_transaction_suite();
    // Suite *suite_nodesDiscover = make_nodesDiscover_suite();
    

    /* create srunner and add first suite to it.
    The first suite in a suite runner is always added in function srunner_create,
    here set suite_wallet as first adding suite. */
    sr = srunner_create(suite_keypairCreat);
    // srunner_add_suite(sr, suite_transaction);
    // srunner_add_suite(sr, suite_nodesDiscover);

    /* set generate test log in running path */
    srunner_set_log(sr, "test_statistics_report.txt");

    /* start to run all test case */
    srunner_run_all(sr, CK_NORMAL);
    /* get the failed unit test number */
    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    /* return failed unit number, this return value can be used in CI, CI tools according
    to this return value determine the test is passed or not  */
    return failed_number;
}