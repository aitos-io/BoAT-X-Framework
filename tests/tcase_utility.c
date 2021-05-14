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
#include "boatutility.h"
#include "tcase_common.h"
#include "check.h"


START_TEST(test_utility_xxxxx) {
    ck_assert(1);
}
END_TEST


Suite * make_utility_suite(void) {
    /* Create Suite */
    Suite *s_utility = suite_create("utility");
    /* Create test cases */
    TCase *tc_uti_api = tcase_create("utility_api");
    /* Add a test case to the Suite */  
    suite_add_tcase(s_utility, tc_uti_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_uti_api, test_utility_xxxxx); 
    
    return s_utility;
}

