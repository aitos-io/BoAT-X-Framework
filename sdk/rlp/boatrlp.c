/******************************************************************************
 * Copyright (C) 2018-2020 aitos.io
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


//!@brief Check if the descriptor capacity has empty item
#define RlpCheckListDescriptorsCapacity(rlp_list_descriptors_ptr) \
    (( (rlp_list_descriptors_ptr)->descriptor_num < (MAX_RLP_LIST_DESC_NUM) ) ? BOAT_TRUE:BOAT_FALSE)





/******************************************************************************
@brief Initialize an RLP object of STRING type

Function: RlpInitStringObject()

    This function initialize an RlpObject of string type. The caller allocates
    the memory for struct RlpObject and the string to hold by the RlpObject. DO
    NOT free the memory used by RlpObject and the string attached to the RlpObject.
    
    The RLP encoder takes care of the memory dynamically allocated during RLP
    encoding.

    NOTE: Despite its name being "string", it's actually byte stream.


@see RlpInitListObject()

@return
    This function returns BOAT_SUCCESS if successful.\n
    Otherwise it returns one of the error codes.

@param[in] rlp_object_ptr
    The pointer to a valid RlpObject.

@param[in] string_ptr
    Pointer of the string to attach to <rlp_object_ptr>.\n

@param[in] string_len
    Length (in byte) of <string_ptr>.

*******************************************************************************/
BOAT_RESULT RlpInitStringObject(RlpObject *rlp_object_ptr, BUINT8 *string_ptr, BUINT32 string_len)
{

    if( rlp_object_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    if( string_ptr == NULL && string_len != 0 )
    {
        BoatLog(BOAT_LOG_VERBOSE, "NULL RLP String MUST have zero length.");
        return(BOAT_ERROR_INVALID_ARGUMENT);
    }

    rlp_object_ptr->object_type = RLP_OBJECT_TYPE_STRING;
    rlp_object_ptr->object_string.string_ptr = string_ptr;
    rlp_object_ptr->object_string.string_len = string_len;

    return BOAT_SUCCESS;
}


/******************************************************************************
@brief Initialize an RLP object of LIST type

Function: RlpInitListObject()

    This function initialize an RlpObject of list type. The caller allocates
    the memory for struct RlpObject. DO NOT free the memory used by RlpObject.
    
    The RLP encoder takes care of the memory dynamically allocated during RLP
    encoding.

    NOTE: The initial RlpObject of a LIST type is empty and can later attach
    other RlpObject of either STRING type or LIST type into it.


@see RlpInitStringObject()

@return
    This function returns BOAT_SUCCESS if successful.\n
    Otherwise it returns one of the error codes.

@param[in] rlp_object_ptr
    The pointer to a valid RlpObject.


*******************************************************************************/
BOAT_RESULT RlpInitListObject(RlpObject *rlp_object_ptr)
{
    RlpListDescriptors *rlp_list_descriptors_ptr;

    if( rlp_object_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    rlp_object_ptr->object_type = RLP_OBJECT_TYPE_LIST;
    
    rlp_list_descriptors_ptr = BoatMalloc(sizeof(RlpListDescriptors));

    if( rlp_list_descriptors_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Fail to allocate memory for RLP list descriptor.");
        return BOAT_ERROR_OUT_OF_MEMORY;
    }

    rlp_list_descriptors_ptr->descriptor_num = 0;

    rlp_object_ptr->object_list.list_descriptors_ptr = rlp_list_descriptors_ptr;
    rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr = NULL;
    rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = 0;
    
    return BOAT_SUCCESS;
}




/******************************************************************************
@brief Append an RLP object to a parent RLP object of LIST type

Function: RlpEncoderAppendObjectToList()

    This function appends an RlpObject (of either STRING type or LIST type) to
    of a parent RlpObject of LIST type.


@return
    This function returns non-negative descriptor index where the RlpObject is\n
    appended if successful.\n
    Otherwise it returns one of the error codes.

@param[in] to_list_object_ptr
    The parent LIST RlpObject to append to.

@param[in] from_object_ptr
    The child RlpObject to append.

*******************************************************************************/
BSINT32 RlpEncoderAppendObjectToList(RlpObject *to_list_object_ptr, RlpObject *from_object_ptr)
{
    RlpListDescriptors *to_rlp_list_descriptors_ptr;
    BUINT32 descriptor_index;
    boat_try_declare;

    if( to_list_object_ptr == NULL || from_object_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
    }

    if( to_list_object_ptr->object_type != RLP_OBJECT_TYPE_LIST )
    {
        BoatLog(BOAT_LOG_VERBOSE, "\"To RLP Object\" MUST be type List.");
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
    }

    if( to_list_object_ptr->object_list.list_descriptors_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "List descriptors of \"To RLP Object\" MUST be available.");
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
    }

    to_rlp_list_descriptors_ptr = to_list_object_ptr->object_list.list_descriptors_ptr;

    if( RlpCheckListDescriptorsCapacity(to_rlp_list_descriptors_ptr) != BOAT_TRUE )
    {
        BoatLog(BOAT_LOG_VERBOSE, "\"To RLP Object\"\'s RLP descriptors are full.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncoderAppendObjectToList_cleanup);
    }

    if( from_object_ptr->object_type == RLP_OBJECT_TYPE_STRING )
    {
        if(   from_object_ptr->object_string.string_ptr == NULL
           && from_object_ptr->object_string.string_len != 0 )
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP Object\" is NULL String with non-zero length.");
            boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
        }
    }
    else if( from_object_ptr->object_type == RLP_OBJECT_TYPE_LIST )
    {
        if( from_object_ptr->object_list.list_descriptors_ptr == NULL )
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP List Object\" MUST have list descriptors filled.");
            boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderAppendObjectToList_cleanup);
        }
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE, "Unknown \"From RLP Object\" type: %d.", from_object_ptr->object_type);
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncoderAppendObjectToList_cleanup);
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


/******************************************************************************
@brief Replace an RLP object in a parent RLP object of LIST type

Function: RlpEncoderReplaceObjectInList()

    This function replaces an RlpObject at the specified index in a parent
    LIST RlpObject with another RlpObject.

    NOTE: The caller takes case of the memroy used by replaced RlpObject.


@return
    This function returns the replaced RlpObject's descriptor index in the parent
    LIST RlpObject if successful.\n
    Otherwise it returns one of the error codes.

@param[in] to_list_object_ptr
    The parent LIST RlpObject to append to.

@param[in] replace_index
    The descriptor index in <to_list_object_ptr> to replace.

@param[in] from_object_ptr
    The RlpObject to replace with.

*******************************************************************************/
BSINT32 RlpEncoderReplaceObjectInList(RlpObject *to_list_object_ptr, BSINT32 replace_index, RlpObject *from_object_ptr)
{
    boat_try_declare;

    if( to_list_object_ptr == NULL || replace_index < 0 || from_object_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
    }

    if( to_list_object_ptr->object_type != RLP_OBJECT_TYPE_LIST )
    {
        BoatLog(BOAT_LOG_VERBOSE, "\"To RLP Object\" MUST be type List.");
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
    }

    if( to_list_object_ptr->object_list.list_descriptors_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "List descriptors of \"To RLP Object\" MUST be available.");
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
    }


    if( replace_index >= to_list_object_ptr->object_list.list_descriptors_ptr->descriptor_num )
    {
        BoatLog(BOAT_LOG_VERBOSE, "replace_index (%d) MUST be less than descriptor_num (%u).", replace_index, to_list_object_ptr->object_list.list_descriptors_ptr->descriptor_num);
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncoderReplaceObjectInList_cleanup);
    }

    if( from_object_ptr->object_type == RLP_OBJECT_TYPE_STRING )
    {
        if(   from_object_ptr->object_string.string_ptr == NULL
           && from_object_ptr->object_string.string_len != 0 )
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP Object\" is NULL String with non-zero length.");
            boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
        }
    }
    else if( from_object_ptr->object_type == RLP_OBJECT_TYPE_LIST )
    {
        if( from_object_ptr->object_list.list_descriptors_ptr == NULL )
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP List Object\" MUST have list descriptors filled.");
            boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncoderReplaceObjectInList_cleanup);
        }
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE, "Unknown \"From RLP Object\" type: %d.", from_object_ptr->object_type);
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncoderReplaceObjectInList_cleanup);
    }


    to_list_object_ptr->object_list.list_descriptors_ptr->rlp_object_ptr[replace_index] = from_object_ptr;


    boat_catch(RlpEncoderReplaceObjectInList_cleanup)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Exception: %d", boat_exception);
        return boat_exception;
    }
    
    return replace_index;
}


/******************************************************************************
@brief Get an RLP object pointer in a parent RLP object by its index

Function: RlpEncoderGetListDescriptor()

    This function returns the RlpObject pointer at the specified index in a parent
    LIST RlpObject.

    NOTE: This doesn't remove the RlpObject from the parent LIST RlpObject. DO NOT
    free the memroy used by the RlpObject. DO NOT modify the content of the
    RlpObject.


@return
    This function returns the pointer to the RlpObject specified by the descriptor\n
    index in the parent if successful.\n
    Otherwise it returns NULL.

@param[in] rlp_object_ptr
    The parent LIST RlpObject.

@param[in] descriptor_index
    The descriptor index to get the RlpObject.

*******************************************************************************/
RlpObject * RlpEncoderGetListDescriptor(RlpObject *rlp_object_ptr, BUINT32 descriptor_index)
{
    RlpListDescriptors *rlp_list_descriptors_ptr;

    if( rlp_object_ptr == NULL)
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return NULL;
    }

    if( rlp_object_ptr->object_type != RLP_OBJECT_TYPE_LIST )
    {
        BoatLog(BOAT_LOG_VERBOSE, "RLP Object MUST be type List.");
        return NULL;
    }

    if( rlp_object_ptr->object_list.list_descriptors_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "List descriptors of RLP Object MUST be available.");
        return NULL;
    }

    rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

    if( descriptor_index >= rlp_list_descriptors_ptr->descriptor_num )
    {
        if( rlp_list_descriptors_ptr->descriptor_num == 0 )
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


/******************************************************************************
@brief Delete an RLP object and all its children

Function: RlpRecursiveDeleteObject()

    This function deletes an RlpObject and all its children RlpObject (if it's
    of LIST and there should be some children).

    NOTE: The caller takes care of the memory used by the struct RlpObject.


@see RlpRecursiveDeleteEncodedStream()

@return
    This function doesn't return anything.


@param[in] rlp_object_ptr
    The parent RlpObject to delete.

*******************************************************************************/
void RlpRecursiveDeleteObject(RlpObject *rlp_object_ptr)
{
    RlpListDescriptors *rlp_list_descriptors_ptr;
    BUINT32 descriptor_index;
    
    if( rlp_object_ptr == NULL )
    {
        return;
    }

    if( rlp_object_ptr->object_type != RLP_OBJECT_TYPE_LIST )
    {
        return;
    }


    if( rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr != NULL )
    {
        BoatFree(rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr);
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr = NULL;
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = 0;
    }
    
    rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

    if( rlp_list_descriptors_ptr == NULL )
    {
        return;
    }

    
    for( descriptor_index = 0; 
         descriptor_index < rlp_list_descriptors_ptr->descriptor_num;
         descriptor_index++ )
    {
        RlpRecursiveDeleteObject(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index]);
    }

    BoatFree(rlp_list_descriptors_ptr);
    rlp_object_ptr->object_list.list_descriptors_ptr = NULL;
    
    return;
}


/******************************************************************************
@brief Delete encoded RLP stream in an RLP object and all its children

Function: RlpRecursiveDeleteEncodedStream()

    This function deletes encoded RLP stream in an RlpObject and all its children
    RlpObject (if it's of LIST and there should be some children).

    This function only deletes the encoded RLP stream while leaves the RlpObject's
    descriptor as is. Thus the tree-ed structure of its children is not affected.

    This function is used in an RLP re-encoding operation. It cleans all previously
    encoded RLP stream and prepares for a re-encoding.


@see RlpRecursiveDeleteObject()

@return
    This function doesn't return anything.


@param[in] rlp_object_ptr
    The parent RlpObject which contains the encoded RLP stream to delete.

*******************************************************************************/
void RlpRecursiveDeleteEncodedStream(RlpObject *rlp_object_ptr)
{
    RlpListDescriptors *rlp_list_descriptors_ptr;
    BUINT32 descriptor_index;
    
    if( rlp_object_ptr == NULL )
    {
        return;
    }

    if( rlp_object_ptr->object_type != RLP_OBJECT_TYPE_LIST )
    {
        return;
    }


    if( rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr != NULL )
    {
        BoatFree(rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr);
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr = NULL;
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = 0;
    }
    
    rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

    if( rlp_list_descriptors_ptr == NULL )
    {
        return;
    }

    
    for( descriptor_index = 0; 
         descriptor_index < rlp_list_descriptors_ptr->descriptor_num;
         descriptor_index++ )
    {
        RlpRecursiveDeleteEncodedStream(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index]);
    }

    
    return;
}


/******************************************************************************
@brief Calculate the encoded RLP stream size in an RLP object and all its children

Function: RlpRecursiveCalcEncodingSize()

    This function calculates the size of the encoded RLP stream in an RlpObject
    and all its children RlpObject (if it's of LIST and there should be some children).

    The size returned is the total size of the stream that would be encoded by
    RlpEncode(). It's used to allocate memory before executing an RLP encoding
    operation.

    NOTE: This function doesn't actually encode the stream and doesn't allocate
    any dynamic memory.
    

@see RlpEncode()

@return
    This function returns the size of the encoded stream.


@param[in] rlp_object_ptr
    The RlpObject to be encode.

@param[out] rlp_head_size_ptr
    The RLP head size within the encoded stream.
    
*******************************************************************************/
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
    
    if( rlp_object_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return RLP_STREAM_LEN_UNKNOWN;
    }
    
    if( rlp_object_ptr->object_type == RLP_OBJECT_TYPE_STRING )
    {
        field_ptr = rlp_object_ptr->object_string.string_ptr;
        field_len = rlp_object_ptr->object_string.string_len;
        
        if(   field_ptr == NULL && field_len != 0 )
        {
            BoatLog(BOAT_LOG_VERBOSE, "String cannot be null unless its length is 0.");
            return RLP_STREAM_LEN_UNKNOWN;
        }

        // Case 1. encoded stream = <field>, if field_len is 1    
        if( field_len == 1 && field_ptr[0] <= 0x7f)
        {
            if( rlp_head_size_ptr != NULL ) *rlp_head_size_ptr = 0;
            encoded_stream_len = 1;
        }
        else
        {
        // Case 2. encoded stream = <1 byte prefix>|<field>, if field_len is in range [0,55] except 1
            if( field_len <= 55 )
            {
                if( rlp_head_size_ptr != NULL ) *rlp_head_size_ptr = 1;
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

                if( rlp_head_size_ptr != NULL ) *rlp_head_size_ptr = 1 + trimmed_sizeof_field_len;
                encoded_stream_len = 1 + trimmed_sizeof_field_len + field_len;
            }
        }
    }
    else if( rlp_object_ptr->object_type == RLP_OBJECT_TYPE_LIST )
    {
        sum_of_sub_field_len = 0;

        rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

        if( rlp_list_descriptors_ptr == NULL )
        {
            return RLP_STREAM_LEN_UNKNOWN;
        }

        
        for( descriptor_index = 0; 
             descriptor_index < rlp_list_descriptors_ptr->descriptor_num;
             descriptor_index++ )
        {
            sub_field_encoded_stream_len = RlpRecursiveCalcEncodingSize( rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index], NULL);

            if( sub_field_encoded_stream_len != RLP_STREAM_LEN_UNKNOWN )
            {
                sum_of_sub_field_len += sub_field_encoded_stream_len;
            }
            else
            {
                sum_of_sub_field_len = RLP_STREAM_LEN_UNKNOWN;
                break;
            }

        }

        if( sum_of_sub_field_len == RLP_STREAM_LEN_UNKNOWN )
        {
            encoded_stream_len = RLP_STREAM_LEN_UNKNOWN;
        }
        else
        {
            // Case 4.(LIST) encoded stream = <1 byte prefix>|<field>, if field_len is in range [0,55]
            if( sum_of_sub_field_len <= 55 )
            {
                if( rlp_head_size_ptr != NULL ) *rlp_head_size_ptr = 1;
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

                if( rlp_head_size_ptr != NULL ) *rlp_head_size_ptr = 1 + trimmed_sizeof_field_len;
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




/******************************************************************************
@brief Encode an RLP stream as per RLP encoding rules.

Function: RlpEncode()

    This function recursively encodes an RLP stream for the specified RlpObject
    and its children (if there be).

    
    There are 3 possible types of RLP encoding structure as per RLP rules:
    1. encoded = <field>, if field_len is 1
    2. encoded = <1 byte prefix>|<field>, if field_len is in range [0,55] except 1
    3. encoded = <1 byte prefix>|<field_len>|<field>, if field_len >= 56
    where "|" means concatenaion.

    If encoding completes successfully, call RlpGetEncodedStream() to get the
    encoded RLP stream.

    If the RlpObject is no longer required, call RlpRecursiveDeleteObject() to
    delete all tree-ed RlpObject.

@note See https://github.com/ethereum/wiki/wiki/RLP for details of the encoding rule.

@see RlpRecursiveCalcEncodingSize() RlpGetEncodedStream() RlpRecursiveDeleteObject()

@return 
    This function returns BOAT_SUCCESS if successful.\n
    Otherwise it returns one of the error codes.


    @param[in] rlp_object_ptr
        The RlpObject to encode.

    @param[in] parent_storage_ptr
        The parent stream object to store the encoded RLP stream.\n\n
        If the <rlp_object_ptr> is the most outer LIST RlpObject, <parent_storage_ptr>\n
        can be specified with NULL, which allows the RLP encoder to automatically\n
        calculate the stream size and allocate memory for it.\n\n
        For a STRING RlpObject, the caller must calculate its size with\n
        RlpRecursiveCalcEncodingSize() and allcoate <parent_storage_ptr> for it. 

*******************************************************************************/
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

    if( rlp_object_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncode_cleanup);
    }

    rlp_encoded_stream_len = RlpRecursiveCalcEncodingSize(rlp_object_ptr, &rlp_head_size);

    if( rlp_encoded_stream_len == RLP_STREAM_LEN_UNKNOWN )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Fail to calculate RLP stream size.");
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncode_cleanup);
    }

    if( parent_storage_ptr != NULL )
    {
        if(   parent_storage_ptr->stream_ptr == NULL
           || parent_storage_ptr->stream_len < rlp_encoded_stream_len )
        {
            BoatLog(BOAT_LOG_VERBOSE, "Parent storage doesn't have enough space to hold encoded String.");
            boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncode_cleanup);
        }
    }
    
    if( rlp_object_ptr->object_type == RLP_OBJECT_TYPE_STRING )
    {
        if( parent_storage_ptr == NULL )
        {
            BoatLog(BOAT_LOG_VERBOSE, "<parent_storage_ptr> cannot be NULL if RLP object is type String.");
            boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncode_cleanup);
        }

        if(   rlp_object_ptr->object_string.string_ptr == NULL
           && rlp_object_ptr->object_string.string_len != 0 )
        {
            BoatLog(BOAT_LOG_VERBOSE, "\"From RLP Object\" is NULL String with non-zero length.");
            boat_throw(BOAT_ERROR_INVALID_ARGUMENT, RlpEncode_cleanup);
        }

        

        // Ready to encode a String Object
        field_len = rlp_object_ptr->object_string.string_len;
        field_ptr = rlp_object_ptr->object_string.string_ptr;
        
        offset = 0;

        // Case 1. encoded stream = <field>, if field_len is 1    
        if( field_len == 1 && field_ptr[0] <= 0x7f)
        {
            // <field> only
            parent_storage_ptr->stream_ptr[offset++] = field_ptr[0];
        }
        else
        {
        // Case 2. encoded stream = <1 byte prefix>|<field>, if field_len is in range [0,55] except 1
            if( field_len <= 55 )
            {
                // <prefix>
                parent_storage_ptr->stream_ptr[offset++] = RLP_PREFIX_BASE_STRING + field_len;
                
                // <field>
                if( field_len != 0 )
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

                if( offset != rlp_encoded_stream_len )
                {
                    BoatLog(BOAT_LOG_CRITICAL, "RLP encoding internal error: Error Size, memory overflowing.");
                    boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncode_cleanup);
                }

            }
        }
        
        
    }
    else if( rlp_object_ptr->object_type == RLP_OBJECT_TYPE_LIST )
    {
        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr =
            BoatMalloc(rlp_encoded_stream_len);

        if( rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr == NULL )
        {
            BoatLog(BOAT_LOG_VERBOSE, "Fail to allocate memory for RLP encoded stream.");
            boat_throw(BOAT_ERROR_OUT_OF_MEMORY, RlpEncode_cleanup);
        }

        rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = rlp_encoded_stream_len;

        stream_object.stream_ptr = rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr + rlp_head_size;
        stream_object.stream_len = rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len - rlp_head_size;



        rlp_list_descriptors_ptr = rlp_object_ptr->object_list.list_descriptors_ptr;

        if( rlp_list_descriptors_ptr == NULL )
        {
            BoatLog(BOAT_LOG_VERBOSE, "RLP List descriptor is NULL.");
            boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncode_cleanup);
        }

        
        for( descriptor_index = 0; 
             descriptor_index < rlp_list_descriptors_ptr->descriptor_num;
             descriptor_index++ )
        {
            sub_field_rlp_stream_len = RlpRecursiveCalcEncodingSize(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index], NULL);

            if( sub_field_rlp_stream_len == RLP_STREAM_LEN_UNKNOWN )
            {
                BoatLog(BOAT_LOG_VERBOSE, "Fail to calculate RLP stream size.");
                boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncode_cleanup);
            }

            result = RlpEncode(rlp_list_descriptors_ptr->rlp_object_ptr[descriptor_index], &stream_object);

            if( result != BOAT_SUCCESS )
            {
                BoatLog(BOAT_LOG_NORMAL, "RLP encoding fails.");
                boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncode_cleanup);
            }



            
            if( stream_object.stream_len < sub_field_rlp_stream_len )
            {
                BoatLog(BOAT_LOG_CRITICAL, "RLP encoding internal error: Error Size.");
                boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncode_cleanup);
            }

            stream_object.stream_ptr += sub_field_rlp_stream_len;
            stream_object.stream_len -= sub_field_rlp_stream_len;

        }


        // Fill RLP List Head
        
        offset = 0;
        // Case 4.(LIST) encoded stream = <1 byte prefix>|<field>, if field_len is in range [0,55]
        if( (rlp_encoded_stream_len - rlp_head_size) <= 55 )
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
            memcpy( rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr + offset,
                    trimmed_field_len,
                    trimmed_sizeof_field_len);

            offset += trimmed_sizeof_field_len;
        }

        if( parent_storage_ptr != NULL )
        {
            memcpy( parent_storage_ptr->stream_ptr,
                    rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr,
                    rlp_encoded_stream_len);
        }
        
        
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE, "Unknown RLP Object type: %d.", rlp_object_ptr->object_type);
        boat_throw(BOAT_ERROR_RLP_ENCODING_FAIL, RlpEncode_cleanup);
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

    if( parent_storage_ptr != NULL )
    {
        if( rlp_object_ptr->object_type == RLP_OBJECT_TYPE_LIST )
        {
            if( rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr != NULL )
            {
                BoatFree(rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr);
                rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_ptr = NULL;
                rlp_object_ptr->object_list.rlp_encoded_stream_object.stream_len = 0;
            }
        }
    }
    
    return result;
}


/******************************************************************************
@brief Re-encode an RLP stream as per RLP encoding rules.

Function: RlpReEncode()

    This function recursively cleans previously encoded RLP stream for the
    specified RlpObject and re-encode the RLP stream.

    
@see RlpEncode()

@return 
    This function returns BOAT_SUCCESS if successful.\n
    Otherwise it returns one of the error codes.


    @param[in] rlp_object_ptr
        The RlpObject to encode.

    @param[in] parent_storage_ptr
        The parent stream object to store the encoded RLP stream.\n\n
        If the <rlp_object_ptr> is the most outer LIST RlpObject, <parent_storage_ptr>\n
        can be specified with NULL, which allows the RLP encoder to automatically\n
        calculate the stream size and allocate memory for it.\n\n
        For a STRING RlpObject, the caller must calculate its size with\n
        RlpRecursiveCalcEncodingSize() and allcoate <parent_storage_ptr> for it. 

*******************************************************************************/
BOAT_RESULT RlpReEncode(RlpObject *rlp_object_ptr, RlpEncodedStreamObject *parent_storage_ptr)
{

    if( rlp_object_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
    RlpRecursiveDeleteEncodedStream(rlp_object_ptr);

    return(RlpEncode(rlp_object_ptr, parent_storage_ptr));
    
}


/******************************************************************************
@brief Get the encoded RLP stream

Function: RlpGetEncodedStream()

    This function returns encoded RLP stream of the specified RlpObject.
    
    Make sure RlpEncode() or RlpReEncode() has been successfully called for
    the RlpObject.

    
@see RlpEncode() RlpReEncode()

@return 
    This function returns pointer to the encoded RLP stream object.\n
    Otherwise it returns NULL.


    @param[in] rlp_object_ptr
        The encoded RlpObject.

*******************************************************************************/
RlpEncodedStreamObject * RlpGetEncodedStream(RlpObject *rlp_object_ptr)
{
    if( rlp_object_ptr == NULL )
    {
        BoatLog(BOAT_LOG_VERBOSE, "Argument cannot be NULL.");
        return NULL;
    }

    if( rlp_object_ptr->object_type == RLP_OBJECT_TYPE_LIST )
    {
        return &(rlp_object_ptr->object_list.rlp_encoded_stream_object);
    }
    
    return NULL;
}




