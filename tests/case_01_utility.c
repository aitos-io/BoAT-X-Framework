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

#if 0 //! @todo Adjust it at a later time

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "boattypes.h"

#include "boatutility.h"

#include "check.h"


START_TEST(test_uti_trimBin) {
    BUINT8 test_from_ptr_1[16]={0x00,0x00,0x10,0x11,0x12,0x13,0x00,0x00};
    BUINT8 test_to_ptr_1[16]={0};
    ck_assert( UtilityTrimBin( test_to_ptr_1,test_from_ptr_1,16,TRIMBIN_LEFTTRIM,BOAT_TRUE) == 6);
    ck_assert_mem_eq(test_to_ptr_1, &test_from_ptr_1[2], 6);

    ck_assert( UtilityTrimBin( test_to_ptr_1,test_from_ptr_1,16,TRIMBIN_TRIM_NO,BOAT_TRUE) == 8);
    ck_assert_mem_eq(test_to_ptr_1, test_from_ptr_1, 8);

    BUINT8 test_from_ptr_2[16]={0};
    BUINT8 test_to_ptr_2[16]={0};
    ck_assert( UtilityTrimBin( test_to_ptr_2,test_from_ptr_2,16,TRIMBIN_LEFTTRIM,BOAT_TRUE) == 0);
    ck_assert_mem_eq(test_to_ptr_2, test_from_ptr_2, 1);

    ck_assert( UtilityTrimBin( test_to_ptr_2,test_from_ptr_2,16,TRIMBIN_LEFTTRIM,BOAT_FALSE) == 1);
    ck_assert( test_to_ptr_2 == NULL);

}
END_TEST


Suite * make_uti_suite(void) {
    Suite *s_utility = suite_create("ethereum");  // Create Suite
    TCase *tc_uti_api = tcase_create("eth_api");  // Create test cases
    suite_add_tcase(s_utility, tc_uti_api);       // Add a test case to the Suite
    tcase_add_test(tc_uti_api, test_uti_trimBin); // Test cases are added to the test set
    
    return s_utility;
}

int main(int argc, char *argv[])
{
    SRunner *sr;
    sr = srunner_create(make_uti_suite()); // Add Suite to SRunner
    srunner_run_all(sr, CK_NORMAL);
    int n = srunner_ntests_failed(sr);     //Run all the test cases
    srunner_free(sr);
	return 0;
}
#else
int main(int argc, char *argv[])
{
	return 0;
}
#endif