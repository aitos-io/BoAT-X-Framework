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

/*!@brief Perform RLP Encoding

@file
boatrlp.c contains functions to encode a stream as per RLP encoding rules.
*/

#include "boatinternal.h"
#include "boatlog.h"

//!@brief Check if the descriptor capacity has empty item
#define RlpCheckListDescriptorsCapacity(rlp_list_descriptors_ptr) \
     (((rlp_list_descriptors_ptr)->descriptor_num < (MAX_RLP_LIST_DESC_NUM)) ? BOAT_TRUE:BOAT_FALSE)


BOAT_RESULT RlpInitStringObject(RlpObject *rlp_object_ptr, BUINT8 *string_ptr, BUINT32 string_len)
{

    if (rlp_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (string_ptr == NULL && string_len != 0)
    {
        BoatLog(BOAT_LOG_VERBOSE, "NULL RLP String MUST have zero length.");
        return(BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    }

    rlp_object_ptr->object_type = RLP_OBJECT_TYPE_STRING;
    rlp_object_ptr->object_string.string_ptr = string_ptr;
    rlp_object_ptr->object_string.string_len = string_len;

    return BOAT_SUCCESS;
}


BOAT_RESULT RlpInitListObject(RlpObject *rlp_object_ptr)
{
    RlpListDescriptors *rlp_list_descriptors_ptr;

    if (rlp_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    rlp_object_ptr->object_type = RLP_OBJECT_TYPE_LIST;
    
    rlp_list_descriptors_ptr = BoatMalloc(sizeof(RlpListDescriptors));

    if (rlp_list_descriptors_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Fail to allocate memory for RLP list descriptor.");
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    rlp_list_descriptors_ptr->descriptor_num = 0;

    rlp_object_ptr->object_list.list_descriptors_ptr = rlp_list_descriptors_ptr;
    rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr = NULL;
    rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = 0;
    
    return BOAT_SUCCESS;
}


BSINT32 RlpEncoderAppendObjectToList(RlpObject *to_list_object_ptr, RlpObject *from_object_ptr)
{
    RlpListDescriptors *to_rlp_list_descriptors_ptr;
    BUINT32 descriptor_index;
    boat_try_declare;

    if (to_list_object_ptr == NULL || from_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
    }

    if (to_list_object_ptr->object_type != RLP_OBJECT_TYPE_LIST)
    {
        BoatLog(BOAT_LOG_VERBOSE, "\"To RLP Object\" MUST be type List.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
    }

    if (to_list_object_ptr->object_list.list_descriptors_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "List descriptors of \"To RLP Object\" MUST be available.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
    }

    to_rlp_list_descriptors_ptr = to_list_object_ptr->object_list.list_descriptors_ptr;

    if (RlpCheckListDescriptorsCapacity(to_rlp_list_descriptors_ptr) != BOAT_TRUE)
    {
        BoatLog(BOAT_LOG_VERBOSE, "\"To RLP Object\"\'s RLP descriptors are full.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncoderAppendObjectToList_cleanup);
    }

    if (from_object_ptr->object_type == RLP_OBJECT_TYPE_STRING)
    {
        if (   from_object_ptr->object_string.string_ptr == NULL
            && from_object_ptr->object_string.string_len != 0)
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP Object\" is NULL String with non-zero length.");
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
        }
    }
    else if (from_object_ptr->object_type == RLP_OBJECT_TYPE_LIST)
    {
        if (from_object_ptr->object_list.list_descriptors_ptr == NULL)
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP List Object\" MUST have list descriptors filled.");
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
        }
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE, "Unknown \"From RLP Object\" type: %d.", from_object_ptr->object_type);
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
    }


    descriptor_index = to_rlp_list_descriptors_ptr->descriptor_num;
    to_rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index] = from_object_ptr;

    to_rlp_list_descriptors_ptr->descriptor_num++;

    boat_catch(RlpEncoderAppendObjectToList_cleanup)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Exception: %d", boat_exception);
        return boat_exception;
    }
    
    return (descriptor_index);
}


BSINT32 RlpEncoderReplaceObjectInList(RlpObject *to_list_object_ptr, BSINT32 replace_index, RlpObject *from_object_ptr)
{
    boat_try_declare;

    if (to_list_object_ptr == NULL || replace_index < 0 || from_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
    }

    if (to_list_object_ptr->object_type != RLP_OBJECT_TYPE_LIST)
    {
        BoatLog(BOAT_LOG_VERBOSE, "\"To RLP Object\" MUST be type List.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
    }

    if (to_list_object_ptr->object_list.list_descriptors_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "List descriptors of \"To RLP Object\" MUST be available.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
    }


    if (replace_index >= to_list_object_ptr->object_list.list_descriptors_ptr->descriptor_num)
    {
        BoatLog(BOAT_LOG_VERBOSE, "replace_index (%d) MUST be less than descriptor_num (%u).", replace_index, to_list_object_ptr->object_list.list_descriptors_ptr->descriptor_num);
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncoderReplaceObjectInList_cleanup);
    }

    if (from_object_ptr->object_type == RLP_OBJECT_TYPE_STRING)
    {
        if ((from_object_ptr->object_string.string_ptr == NULL) && \
            (from_object_ptr->object_string.string_len != 0))
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP Object\" is NULL String with non-zero length.");
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
        }
    }
    else if (from_object_ptr->object_type == RLP_OBJECT_TYPE_LIST)
    {
        if (from_object_ptr->object_list.list_descriptors_ptr == NULL)
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP List Object\" MUST have list descriptors filled.");
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
        }
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE, "Unknown \"From RLP Object\" type: %d.", from_object_ptr->object_type);
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncoderReplaceObjectInList_cleanup);
    }


    to_list_object_ptr->object_list.list_descriptors_ptr->rlp_object_ptr[replace_index] = from_object_ptr;


    boat_catch(RlpEncoderReplaceObjectInList_cleanup)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Exception: %d", boat_exception);
        return boat_exception;
    }
    
    return replace_index;
}


RlpObject * RlpEncoderGetListDescriptor(RlpObject *rlp_object_ptr, BUINT32 descriptor_index)
{
    RlpListDescriptors *rlp_list_descriptors_ptr;

    if (rlp_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return NULL;
    }

    if (rlp_object_ptr->object_type != RLP_OBJECT_TYPE_LIST)
    {
        BoatLog(BOAT_LOG_VERBOSE, "RLP Object MUST be type List.");
        return NULL;
    }

    if (rlp_object_ptr->object_list.list_descriptors_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "List descriptors of RLP Object MUST be available.");
        return NULL;
    }

    rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

    if (descriptor_index >= rlp_list_descriptors_ptr->descriptor_num)
    {
        if (rlp_list_descriptors_ptr->descriptor_num == 0)
        {
            BoatLog(BOAT_LOG_VERBOSE, "Descriptor list of the RLP object is empty.");
        }
        else
        {
            BoatLog(BOAT_LOG_VERBOSE, "Descriptor index %u is out of range: [0, %u].", descriptor_index, rlp_list_descriptors_ptr->descriptor_num - 1);
        }
        
        return NULL;
    }
    
    
    return rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index];
}


void RlpRecursiveDeleteObject(RlpObject *rlp_object_ptr)
{
    RlpListDescriptors *rlp_list_descriptors_ptr;
    BUINT32 descriptor_index;
    
    if (rlp_object_ptr == NULL)
    {
        return;
    }

    if (rlp_object_ptr->object_type != RLP_OBJECT_TYPE_LIST)
    {
        return;
    }


    if (rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr != NULL)
    {
        BoatFree(rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr);
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr = NULL;
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = 0;
    }
    
    rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

    if (rlp_list_descriptors_ptr == NULL)
    {
        return;
    }

    
    for (descriptor_index = 0; 
         descriptor_index < rlp_list_descriptors_ptr->descriptor_num;
         descriptor_index++)
    {
        RlpRecursiveDeleteObject(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index]);
    }

    BoatFree(rlp_list_descriptors_ptr);
    rlp_object_ptr->object_list.list_descriptors_ptr = NULL;
    
    return;
}


void RlpRecursiveDeleteEncodedStream(RlpObject *rlp_object_ptr)
{
    RlpListDescriptors *rlp_list_descriptors_ptr;
    BUINT32 descriptor_index;
    
    if (rlp_object_ptr == NULL)
    {
        return;
    }

    if (rlp_object_ptr->object_type != RLP_OBJECT_TYPE_LIST)
    {
        return;
    }


    if (rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr != NULL)
    {
        BoatFree(rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr);
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr = NULL;
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = 0;
    }
    
    rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

    if (rlp_list_descriptors_ptr == NULL)
    {
        return;
    }

    
    for (descriptor_index = 0; 
         descriptor_index < rlp_list_descriptors_ptr->descriptor_num;
         descriptor_index++)
    {
        RlpRecursiveDeleteEncodedStream(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index]);
    }

    
    return;
}


BUINT32 RlpRecursiveCalcEncodingSize(RlpObject *rlp_object_ptr, BOAT_OUT BUINT8 *rlp_head_size_ptr)
{
    BUINT32 encoded_stream_len;
    BUINT32 sub_field_encoded_stream_len;
    BUINT32 sum_of_sub_field_len;
    BUINT8 trimmed_field_len[8];
    BUINT8 trimmed_sizeof_field_len;
    BUINT8 *field_ptr;
    BUINT32 field_len;

    RlpListDescriptors *rlp_list_descriptors_ptr;
    BUINT32 descriptor_index;
    
    if (rlp_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return RLP_STREAM_LEN_UNKNOWN;
    }
    
    if (rlp_object_ptr->object_type == RLP_OBJECT_TYPE_STRING)
    {
        field_ptr = rlp_object_ptr->object_string.string_ptr;
        field_len = rlp_object_ptr->object_string.string_len;
        
        if (field_ptr == NULL && field_len != 0)
        {
            BoatLog(BOAT_LOG_VERBOSE, "String cannot be null unless its length is 0.");
            return RLP_STREAM_LEN_UNKNOWN;
        }

        // Case 1. encoded stream = <field>, if field_len is 1    
        if (field_len == 1 && field_ptr[0] <= 0x7f)
        {
            if (rlp_head_size_ptr != NULL) *rlp_head_size_ptr = 0;
            encoded_stream_len = 1;
        }
        else
        {
        // Case 2. encoded stream = <1 byte prefix>|<field>, if field_len is in range [0,55] except 1
            if (field_len <= 55)
            {
                if (rlp_head_size_ptr != NULL) *rlp_head_size_ptr = 1;
                encoded_stream_len = 1 + field_len;
            }
            else
            {
         // Case 3. encoded stream = <1 byte prefix>|<field_len>|<field>, if field_len >= 56
                    
                // Calculate trimmed size of <field_len>
                trimmed_sizeof_field_len = UtilityUint32ToBigend(
                                                                trimmed_field_len,
                                                                field_len,
                                                                TRIMBIN_LEFTTRIM
                                                                );

                if (rlp_head_size_ptr != NULL) *rlp_head_size_ptr = 1 + trimmed_sizeof_field_len;
                encoded_stream_len = 1 + trimmed_sizeof_field_len + field_len;
            }
        }
    }
    else if (rlp_object_ptr->object_type == RLP_OBJECT_TYPE_LIST)
    {
        sum_of_sub_field_len = 0;

        rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

        if (rlp_list_descriptors_ptr == NULL)
        {
            return RLP_STREAM_LEN_UNKNOWN;
        }

        
        for (descriptor_index = 0; 
             descriptor_index < rlp_list_descriptors_ptr->descriptor_num;
             descriptor_index++)
        {
            sub_field_encoded_stream_len = RlpRecursiveCalcEncodingSize(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index], NULL);

            if (sub_field_encoded_stream_len != RLP_STREAM_LEN_UNKNOWN)
            {
                sum_of_sub_field_len += sub_field_encoded_stream_len;
            }
            else
            {
                sum_of_sub_field_len = RLP_STREAM_LEN_UNKNOWN;
                break;
            }

        }

        if (sum_of_sub_field_len == RLP_STREAM_LEN_UNKNOWN)
        {
            encoded_stream_len = RLP_STREAM_LEN_UNKNOWN;
        }
        else
        {
            // Case 4.(LIST) encoded stream = <1 byte prefix>|<field>, if field_len is in range [0,55]
            if (sum_of_sub_field_len <= 55)
            {
                if (rlp_head_size_ptr != NULL) *rlp_head_size_ptr = 1;
                encoded_stream_len = 1 + sum_of_sub_field_len;
            }
            else
            {
            // Case 5.(LIST) encoded stream = <1 byte prefix>|<field_len>|<field>, if field_len >= 56
                    
                // Calculate trimmed size of <field_len>
                trimmed_sizeof_field_len = UtilityUint32ToBigend(
                                                            trimmed_field_len,
                                                            sum_of_sub_field_len,
                                                            TRIMBIN_LEFTTRIM
                                                        );

                if (rlp_head_size_ptr != NULL) *rlp_head_size_ptr = 1 + trimmed_sizeof_field_len;
                encoded_stream_len = 1 + trimmed_sizeof_field_len + sum_of_sub_field_len;
            }
        }
        
    }
    else
    {
        *rlp_head_size_ptr = 0;
        encoded_stream_len = RLP_STREAM_LEN_UNKNOWN;
        BoatLog(BOAT_LOG_VERBOSE, "Unknown RLP Object type: %d.", rlp_object_ptr->object_type);
    }    

    return encoded_stream_len;
    
}


BOAT_RESULT RlpEncode(RlpObject *rlp_object_ptr, RlpEncodedStreamObject *parent_storage_ptr)
{
    RlpEncodedStreamObject stream_object;
    
    RlpListDescriptors *rlp_list_descriptors_ptr;
    BUINT32 descriptor_index;

    BUINT32 rlp_encoded_stream_len;
    BUINT8 rlp_head_size;

    BUINT32 sub_field_rlp_stream_len;

    BUINT32 field_len;
    const BUINT8 *field_ptr;
    BUINT32 offset;
    BUINT8 trimmed_field_len[8];
    BUINT8 trimmed_sizeof_field_len;

    BOAT_RESULT result;
    
#define RLP_PREFIX_BASE_STRING 0x80
#define RLP_PREFIX_BASE_LIST   0xC0
    
    boat_try_declare;

    if (rlp_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncode_cleanup);
    }

    rlp_encoded_stream_len = RlpRecursiveCalcEncodingSize(rlp_object_ptr, &rlp_head_size);

    if (rlp_encoded_stream_len == RLP_STREAM_LEN_UNKNOWN)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Fail to calculate RLP stream size.");
        boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncode_cleanup);
    }

    if (parent_storage_ptr != NULL)
    {
        if (   parent_storage_ptr->stream_ptr == NULL
            || parent_storage_ptr->stream_len < rlp_encoded_stream_len)
        {
            BoatLog(BOAT_LOG_VERBOSE, "Parent storage doesn't have enough space to hold encoded String.");
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncode_cleanup);
        }
    }
    
    if (rlp_object_ptr->object_type == RLP_OBJECT_TYPE_STRING)
    {
        if (parent_storage_ptr == NULL)
        {
            BoatLog(BOAT_LOG_VERBOSE, "<parent_storage_ptr> cannot be NULL if RLP object is type String.");
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncode_cleanup);
        }

        if (  rlp_object_ptr->object_string.string_ptr == NULL
            && rlp_object_ptr->object_string.string_len != 0)
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP Object\" is NULL String with non-zero length.");
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncode_cleanup);
        }

        // Ready to encode a String Object
        field_len = rlp_object_ptr->object_string.string_len;
        field_ptr = rlp_object_ptr->object_string.string_ptr;
        
        offset = 0;

        // Case 1. encoded stream = <field>, if field_len is 1    
        if (field_len == 1 && field_ptr[0] <= 0x7f)
        {
            // <field> only
            parent_storage_ptr->stream_ptr[offset++] = field_ptr[0];
        }
        else
        {
        // Case 2. encoded stream = <1 byte prefix>|<field>, if field_len is in range [0,55] except 1
            if (field_len <= 55)
            {
                // <prefix>
                parent_storage_ptr->stream_ptr[offset++] = RLP_PREFIX_BASE_STRING + field_len;
                
                // <field>
                if (field_len != 0)
                {
                    memcpy(parent_storage_ptr->stream_ptr + offset, field_ptr, field_len);
                    offset += field_len;
                }
            }
            else
            {
         // Case 3. encoded stream = <1 byte prefix>|<field_len>|<field>, if field_len >= 56
                    
                // Calculate trimmed size of <field_len>
                trimmed_sizeof_field_len = UtilityUint32ToBigend(
                                                                trimmed_field_len,
                                                                field_len,
                                                                TRIMBIN_LEFTTRIM
                                                                );

                // <prefix>
                parent_storage_ptr->stream_ptr[offset++] = RLP_PREFIX_BASE_STRING + trimmed_sizeof_field_len + 55;

                // <field_len>
                memcpy(parent_storage_ptr->stream_ptr + offset, trimmed_field_len, trimmed_sizeof_field_len);
                offset += trimmed_sizeof_field_len;
                
                // <field>
                memcpy(parent_storage_ptr->stream_ptr + offset, field_ptr, field_len);
                offset += field_len;

                if (offset != rlp_encoded_stream_len)
                {
                    BoatLog(BOAT_LOG_CRITICAL, "RLP encoding internal error: Error Size, memory overflowing.");
                    boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncode_cleanup);
                }
            }
        }
    }
    else if (rlp_object_ptr->object_type == RLP_OBJECT_TYPE_LIST)
    {
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr =
            BoatMalloc(rlp_encoded_stream_len);

        if (rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr == NULL)
        {
            BoatLog(BOAT_LOG_VERBOSE, "Fail to allocate memory for RLP encoded stream.");
            boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, RlpEncode_cleanup);
        }

        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = rlp_encoded_stream_len;

        stream_object.stream_ptr = rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr + rlp_head_size;
        stream_object.stream_len = rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len - rlp_head_size;

        rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

        if (rlp_list_descriptors_ptr == NULL)
        {
            BoatLog(BOAT_LOG_VERBOSE, "RLP List descriptor is NULL.");
            boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncode_cleanup);
        }
  
        for (descriptor_index = 0; 
             descriptor_index < rlp_list_descriptors_ptr->descriptor_num;
             descriptor_index++)
        {
            sub_field_rlp_stream_len = RlpRecursiveCalcEncodingSize(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index], NULL);

            if (sub_field_rlp_stream_len == RLP_STREAM_LEN_UNKNOWN)
            {
                BoatLog(BOAT_LOG_VERBOSE, "Fail to calculate RLP stream size.");
                boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncode_cleanup);
            }

            result = RlpEncode(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index], &stream_object);

            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "RLP encoding fails.");
                boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncode_cleanup);
            }
  
            if (stream_object.stream_len < sub_field_rlp_stream_len)
            {
                BoatLog(BOAT_LOG_CRITICAL, "RLP encoding internal error: Error Size.");
                boat_throw(BOAT_ERROR_RLP_ENCODER_FAIL, RlpEncode_cleanup);
            }

            stream_object.stream_ptr += sub_field_rlp_stream_len;
            stream_object.stream_len -= sub_field_rlp_stream_len;
        }

        // Fill RLP List Head
        offset = 0;
        // Case 4.(LIST) encoded stream = <1 byte prefix>|<field>, if field_len is in range [0,55]
        if ((rlp_encoded_stream_len - rlp_head_size) <= 55)
        {
            // <prefix>
            rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr[offset++] = RLP_PREFIX_BASE_LIST + (rlp_encoded_stream_len - rlp_head_size);
        }
        else
        {
        // Case 5.(LIST) encoded stream = <1 byte prefix>|<field_len>|<field>, if field_len >= 56
                
            // Calculate trimmed size of <field_len>
            trimmed_sizeof_field_len = UtilityUint32ToBigend(
                                                            trimmed_field_len,
                                                            (rlp_encoded_stream_len - rlp_head_size),
                                                            TRIMBIN_LEFTTRIM
                                                            );

            // <prefix>
            rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr[offset++] = RLP_PREFIX_BASE_LIST + trimmed_sizeof_field_len + 55;

            // <field_len>
            memcpy (rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr + offset,
                    trimmed_field_len,
                    trimmed_sizeof_field_len);

            offset += trimmed_sizeof_field_len;
        }

        if (parent_storage_ptr != NULL)
        {
            memcpy(parent_storage_ptr->stream_ptr,
                   rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr,
                   rlp_encoded_stream_len);
        }  
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE, "Unknown RLP Object type: %d.", rlp_object_ptr->object_type);
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, RlpEncode_cleanup);
    }

    boat_catch(RlpEncode_cleanup)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Exception: %d", boat_exception);
        result = boat_exception;
    }
    else
    {
        result = BOAT_SUCCESS;
    }

    if (parent_storage_ptr != NULL)
    {
        if (rlp_object_ptr->object_type == RLP_OBJECT_TYPE_LIST)
        {
            if (rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr != NULL)
            {
                BoatFree(rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr);
                rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr = NULL;
                rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = 0;
            }
        }
    }
    
    return result;
}


BOAT_RESULT RlpReEncode(RlpObject *rlp_object_ptr, RlpEncodedStreamObject *parent_storage_ptr)
{

    if (rlp_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    
    RlpRecursiveDeleteEncodedStream(rlp_object_ptr);

    return(RlpEncode(rlp_object_ptr, parent_storage_ptr));
    
}


RlpEncodedStreamObject * RlpGetEncodedStream(RlpObject *rlp_object_ptr)
{
    if (rlp_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return NULL;
    }

    if (rlp_object_ptr->object_type == RLP_OBJECT_TYPE_LIST)
    {
        return &(rlp_object_ptr->object_list.rlp_encoded_stream_object);
    }
    
    return NULL;
}
