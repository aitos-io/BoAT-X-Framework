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

#include "boatinternal.h"


RlpObject g_case_rlp_object_listA;

    RlpObject g_case_rlp_object_stringA1;    // Test Null String
    
    RlpObject g_case_rlp_object_stringA2;
    BUINT8 g_case_rlp_object_stringA2_value[1] = {0x7F};
    
    RlpObject g_case_rlp_object_stringA3;
    BUINT8 g_case_rlp_object_stringA3_value[1] = {0x80};
    
    RlpObject g_case_rlp_object_stringA4;
    BUINT8 g_case_rlp_object_stringA4_value[2] = {0x12, 0x34};
    
    RlpObject g_case_rlp_object_stringA5;
    BUINT8 g_case_rlp_object_stringA5_value[55]; // memset to 0x11
    
    RlpObject g_case_rlp_object_stringA6;
    BUINT8 g_case_rlp_object_stringA6_value[56]; // memset to 0x22
    
        RlpObject g_case_rlp_object_listB;
        
            RlpObject g_case_rlp_object_stringB1;
            BUINT8 g_case_rlp_object_stringB1_value[1] = {0x56};
            
            RlpObject g_case_rlp_object_stringB2;
            BUINT8 g_case_rlp_object_stringB2_value[1] = {0x78};

            RlpObject g_case_rlp_object_listC;  // Test Null List

        


void Case_20_RlpPreCondition(void)
{
    memset(g_case_rlp_object_stringA5_value, 0x11, sizeof(g_case_rlp_object_stringA5_value));
    memset(g_case_rlp_object_stringA6_value, 0x22, sizeof(g_case_rlp_object_stringA6_value));
}



BOAT_RESULT Case_20_RlpInitObject(void)
{
    BOAT_RESULT case_result;
    BOAT_RESULT call_result;
    BCHAR *case_name_str;
    boat_try_declare;

    case_result = 0;

    case_name_str = "Case_20_RlpInitObject_2010";
    call_result = RlpInitListObject(&g_case_rlp_object_listA);
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }

    
    case_name_str = "Case_20_RlpInitObject_2011";
    call_result = RlpInitStringObject(&g_case_rlp_object_stringA1,
                                       NULL,
                                       0);
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }

    case_name_str = "Case_20_RlpInitObject_2012";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listA, &g_case_rlp_object_stringA1);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2013";
    call_result = RlpInitStringObject(&g_case_rlp_object_stringA2,
                                       g_case_rlp_object_stringA2_value,
                                       sizeof(g_case_rlp_object_stringA2_value));
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2014";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listA, &g_case_rlp_object_stringA2);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }

    case_name_str = "Case_20_RlpInitObject_2015";
    call_result = RlpInitStringObject(&g_case_rlp_object_stringA3,
                                       g_case_rlp_object_stringA3_value,
                                       sizeof(g_case_rlp_object_stringA3_value));
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2016";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listA, &g_case_rlp_object_stringA3);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2017";
    call_result = RlpInitStringObject(&g_case_rlp_object_stringA4,
                                       g_case_rlp_object_stringA4_value,
                                       sizeof(g_case_rlp_object_stringA4_value));
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2018";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listA, &g_case_rlp_object_stringA4);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2019";
    call_result = RlpInitStringObject(&g_case_rlp_object_stringA5,
                                       g_case_rlp_object_stringA5_value,
                                       sizeof(g_case_rlp_object_stringA5_value));
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2020";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listA, &g_case_rlp_object_stringA5);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }
    
    
    case_name_str = "Case_20_RlpInitObject_2021";
    call_result = RlpInitStringObject(&g_case_rlp_object_stringA6,
                                       g_case_rlp_object_stringA6_value,
                                       sizeof(g_case_rlp_object_stringA6_value));
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2022";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listA, &g_case_rlp_object_stringA6);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }
    
    
    
    case_name_str = "Case_20_RlpInitObject_2023";
    call_result = RlpInitListObject(&g_case_rlp_object_listB);
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }
    

    case_name_str = "Case_20_RlpInitObject_2024";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listA, &g_case_rlp_object_listB);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }
    
    
    case_name_str = "Case_20_RlpInitObject_2025";
    call_result = RlpInitStringObject(&g_case_rlp_object_stringB1,
                                       g_case_rlp_object_stringB1_value,
                                       sizeof(g_case_rlp_object_stringB1_value));
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2026";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listB, &g_case_rlp_object_stringB1);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2027";
    call_result = RlpInitStringObject(&g_case_rlp_object_stringB2,
                                       g_case_rlp_object_stringB2_value,
                                       sizeof(g_case_rlp_object_stringB2_value));
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2028";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listB, &g_case_rlp_object_stringB2);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }


    case_name_str = "Case_20_RlpInitObject_2029";
    call_result = RlpInitListObject(&g_case_rlp_object_listC);
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }
    

    case_name_str = "Case_20_RlpInitObject_2030";
    call_result = RlpEncoderAppendObjectToList(&g_case_rlp_object_listB, &g_case_rlp_object_listC);
    if( call_result >= BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpInitObject_cleanup);
    }
    
    
    boat_catch(Case_20_RlpInitObject_cleanup)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Exception: %d", boat_exception);
    }
    
    if( case_result < 0 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Case_20_RlpInitObject Failed: %d.", case_result);
        return case_result;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Case_20_RlpInitObject Passed.");
        return BOAT_SUCCESS;
    }
}


BOAT_RESULT Case_20_RlpEncode(void)
{
    RlpEncodedStreamObject parent_storage;
    BUINT8 parent_storage_buffer[8];
    BOAT_RESULT call_result;
    BOAT_RESULT case_result;
    BCHAR *case_name_str;
    BUINT32 i;
    boat_try_declare;

    memset(parent_storage_buffer, 0x00, sizeof(parent_storage_buffer));

    case_result = 0;
    
        
    case_name_str = "Case_20_RlpEncode_2110";
    call_result = RlpEncode(&g_case_rlp_object_stringA4, NULL);
    if( call_result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpEncode_cleanup);
    }


    case_name_str = "Case_20_RlpEncode_2111";
    parent_storage.stream_ptr = parent_storage_buffer;
    parent_storage.stream_len = 1;
    
    call_result = RlpEncode(&g_case_rlp_object_stringA4, &parent_storage);
    if( call_result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpEncode_cleanup);
    }
    

    case_name_str = "Case_20_RlpEncode_2112";
    parent_storage.stream_ptr = parent_storage_buffer;
    parent_storage.stream_len = 3;
    
    call_result = RlpEncode(&g_case_rlp_object_stringA4, &parent_storage);
    if( call_result == BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "%s: parent_storage.stream_len = %u.", case_name_str, parent_storage.stream_len);
        for( i = 0; i < parent_storage.stream_len; i++ )
        {
            printf("%02x ", parent_storage.stream_ptr[i]);
        }
        putchar('\n');

        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpEncode_cleanup);
    }
    

    case_name_str = "Case_20_RlpEncode_2113";
    call_result = RlpEncode(&g_case_rlp_object_listA, NULL);
    if( call_result == BOAT_SUCCESS )
    {
        RlpEncodedStreamObject *storage_ptr;
        
        storage_ptr = RlpGetEncodedStream(&g_case_rlp_object_listA);
        
        BoatLog(BOAT_LOG_NORMAL, "%s: Encoded RLP stream: %u bytes.", case_name_str, storage_ptr->stream_len);
        for( i = 0; i < storage_ptr->stream_len; i++ )
        {
            printf("%02x ", g_case_rlp_object_listA.object_list.rlp_encoded_stream_object.stream_ptr[i]);
        }
        putchar('\n');
        
        BoatLog(BOAT_LOG_NORMAL, "%s Passed.", case_name_str);
    }
    else
    {
        case_result -= 1;
        BoatLog(BOAT_LOG_NORMAL, "%s Failed.", case_name_str);
        boat_throw(BOAT_ERROR_TEST_CASE_FAIL, Case_20_RlpEncode_cleanup);
    }

    boat_catch(Case_20_RlpEncode_cleanup)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Exception: %d", boat_exception);
    }
    
    
    if( case_result < 0 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Case_20_RlpEncode Failed: %d.", case_result);
        return case_result;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Case_20_RlpEncode Passed.");
        return BOAT_SUCCESS;
    }    
        
}


BOAT_RESULT Case_20_RlpDelete(void)
{
    RlpRecursiveDeleteObject(&g_case_rlp_object_listA);
    
    return BOAT_SUCCESS;
}


BOAT_RESULT Case_20_RlpMain(void)
{
    BOAT_RESULT case_result = BOAT_SUCCESS;
    
    Case_20_RlpPreCondition();
    
    case_result += Case_20_RlpInitObject();
    case_result += Case_20_RlpEncode();
    case_result += Case_20_RlpDelete();

    if( case_result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Case_20_Rlp Failed: %d.", case_result);
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Case_20_Rlp Passed.");
    }
    
    return case_result;
}

