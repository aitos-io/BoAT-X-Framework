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
#include "tcase_fabric.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>

/* extern suite declaration */
extern Suite *make_wallet_suite(void);
extern Suite *make_parameters_suite(void);
extern Suite *make_contract_suite(void);

char fabric_client_cert_buf[CERT_PRIKEY_LEN] = {0};
char fabric_client_key_buf[CERT_PRIKEY_LEN] = {0};
char fabric_org1_tls_cert_buf[CERT_PRIKEY_LEN] = {0};
char fabric_org2_tls_cert_buf[CERT_PRIKEY_LEN] = {0};
char fabric_order_tls_cert_buf[CERT_PRIKEY_LEN] = {0};
char fabric_client_key_false_buf[CERT_PRIKEY_LEN] = {0};


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
   Suite *suite_wallet    = make_wallet_suite();
   ret = read_file_content(file_client_key,fabric_client_key_buf);
   if(ret < 0){
      printf("read file_client_key fail \n");
      return ret;
   }
   ret = read_file_content(file_client_cert,fabric_client_cert_buf);
   if(ret < 0){
      printf("read file_client_cert fail \n");
      return ret;
   }
   ret = read_file_content(file_org1_tls_cert,fabric_org1_tls_cert_buf);
   if(ret < 0){
      printf("read file_org1_tls_cert fail \n");
      return ret;
   }
   ret = read_file_content(file_org2_tls_cert,fabric_org2_tls_cert_buf);
   if(ret < 0){
      printf("read file_org2_tls_cert fail \n");
      return ret;
   }
   ret = read_file_content(file_order_tls_cert,fabric_order_tls_cert_buf);
   if(ret < 0){
      printf("read file_order_tls_cert fail \n");
      return ret;
   }
   ret = read_file_content(file_client_key_false,fabric_client_key_false_buf);
   if(ret < 0){
      printf("read file_order_tls_cert fail \n");
      return ret;
   }

   /* create srunner and add first suite to it.
    The first suite in a suite runner is always added in function srunner_create,
    here set suite_wallet as first adding suite. */
   sr = srunner_create(suite_wallet);
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