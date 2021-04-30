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

/*!@brief RLP Encoding header file

@file
boatrlp.h is the header file for RLP encoding.
*/

#ifndef __BOATRLP_H__
#define __BOATRLP_H__

#include "boatiotsdk.h"


/*! @defgroup rlp boat rlp
 * @{
 */


/*!
Enum Type RlpObjectType
*/
typedef enum
{
    RLP_OBJECT_TYPE_STRING = 0,
    RLP_OBJECT_TYPE_LIST
}RlpObjectType;


#define RLP_STREAM_LEN_UNKNOWN (~0)
typedef struct TRlpEncodedStreamObject
{
    BUINT8 *stream_ptr;
    BUINT32 stream_len;
}RlpEncodedStreamObject;


typedef struct TRlpObjectTypeString
{
    BUINT8 *string_ptr;
    BUINT32 string_len;
}RlpObjectTypeString;


typedef struct TRlpObjectTypeList
{
    struct TRlpListDescriptors *list_descriptors_ptr;
    RlpEncodedStreamObject rlp_encoded_stream_object;
}RlpObjectTypeList;


typedef struct TRlpObject
{
    RlpObjectType object_type;
    
    union
    {
        RlpObjectTypeString object_string;
        RlpObjectTypeList   object_list;
    };
}RlpObject;


#define MAX_RLP_LIST_DESC_NUM 15
typedef struct TRlpListDescriptors
{
    BUINT32 descriptor_num;
    RlpObject *rlp_object_ptr[MAX_RLP_LIST_DESC_NUM];
}RlpListDescriptors;

#ifdef __cplusplus
extern "C" {
#endif

/*!****************************************************************************
 * @brief Initialize an RLP object of STRING type
 *
 * @details
 *   This function initialize an RlpObject of string type. The caller allocates
 *   the memory for struct RlpObject and the string to hold by the RlpObject. DO
 *   NOT free the memory used by RlpObject and the string attached to the RlpObject.
 *   \n The RLP encoder takes care of the memory dynamically allocated during RLP
 *   encoding.
 *
 * @note
 *   Despite its name being "string", it's actually byte stream.
 *
 * @param[in] rlp_object_ptr
 *   The pointer to a valid RlpObject.
 *
 * @param[in] string_ptr
 *   Pointer of the string to attach to <rlp_object_ptr>.\n
 *
 * @param[in] string_len
 *   Length (in byte) of <string_ptr>.
 *
 * @return
 *   This function returns BOAT_SUCCESS if successful.\n
 *   Otherwise it returns one of the error codes.
 *
 * @see RlpInitListObject()
 ******************************************************************************/
BOAT_RESULT RlpInitStringObject(RlpObject *rlp_object_ptr, BUINT8 *string_ptr, BUINT32 string_len);


/*!****************************************************************************
 * @brief Initialize an RLP object of LIST type
 *
 * @details
 *   This function initialize an RlpObject of list type. The caller allocates
 *   the memory for struct RlpObject. DO NOT free the memory used by RlpObject.  
 *   \n The RLP encoder takes care of the memory dynamically allocated during RLP
 *   encoding.
 *
 * @note
 *   The initial RlpObject of a LIST type is empty and can later attach
 *    other RlpObject of either STRING type or LIST type into it.
 *
 * @param[in] rlp_object_ptr
 *   The pointer to a valid RlpObject.
 *
 * @return
 *   This function returns BOAT_SUCCESS if successful.\n
 *   Otherwise it returns one of the error codes.
 *	
 * @see RlpInitStringObject()
 ******************************************************************************/
BOAT_RESULT RlpInitListObject(RlpObject *rlp_object_ptr);


/*!****************************************************************************
 * @brief Append an RLP object to a parent RLP object of LIST type
 *
 * @details
 *   This function appends an RlpObject (of either STRING type or LIST type) to
 *   of a parent RlpObject of LIST type.
 *
 * @param[in] to_list_object_ptr
 *   The parent LIST RlpObject to append to.
 *
 * @param[in] from_object_ptr
 *   The child RlpObject to append.
 *
 *@return
 *   This function returns non-negative descriptor index where the RlpObject is
 *   appended if successful.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h 
 *   for details.
 ******************************************************************************/
BOAT_RESULT RlpEncoderAppendObjectToList(RlpObject *to_list_object_ptr, RlpObject *from_object_ptr);


/*!****************************************************************************
 * @brief Replace an RLP object in a parent RLP object of LIST type
 *
 * @details
 *   This function replaces an RlpObject at the specified index in a parent
 *   LIST RlpObject with another RlpObject.
 *
 * @note
 *   The caller takes case of the memroy used by replaced RlpObject.
 *
 * @param[in] to_list_object_ptr
 *   The parent LIST RlpObject to append to.
 *
 * @param[in] replace_index
 *   The descriptor index in <to_list_object_ptr> to replace.
 *
 * @param[in] from_object_ptr
 *   The RlpObject to replace with.
 *
 * @return
 *   This function returns the replaced RlpObject's descriptor index in the parent
 *   LIST RlpObject if successful.
 *   \n It returns one of the error codes if append. Refer to header file boaterrcode.h for details.
 ******************************************************************************/
BSINT32 RlpEncoderReplaceObjectInList(RlpObject *to_list_object_ptr, BSINT32 replace_index, RlpObject *from_object_ptr);


/*!****************************************************************************
 * @brief Get an RLP object pointer in a parent RLP object by its index
 *
 * @details
 *   This function returns the RlpObject pointer at the specified index in a parent
 *   LIST RlpObject.
 *
 * @note
 *   This doesn't remove the RlpObject from the parent LIST RlpObject. DO NOT
 *   free the memroy used by the RlpObject. DO NOT modify the content of the
 *   RlpObject.
 *
 * @param[in] rlp_object_ptr
 *    The parent LIST RlpObject.
 *
 * @param[in] descriptor_index
 *   The descriptor index to get the RlpObject.
 *
 * @return
 *   This function returns the pointer to the RlpObject specified by the descriptor
 *   index in the parent if successful.
 *   \n It returns NULL if encode failed.
 ******************************************************************************/
RlpObject * RlpEncoderGetListDescriptor(RlpObject *rlp_object_ptr, BUINT32 descriptor_index);


/*!****************************************************************************
 * @brief Delete an RLP object and all its children
 *
 * @details
 *   This function deletes an RlpObject and all its children RlpObject (if it's
 *   of LIST and there should be some children).
 *
 * note
 *   The caller takes care of the memory used by the struct RlpObject.
 *
 * @param[in] rlp_object_ptr
 *   The parent RlpObject to delete.
 *
 * @return
 *   This function doesn't return anything.
 *
 * @see RlpRecursiveDeleteEncodedStream()
 ******************************************************************************/
void RlpRecursiveDeleteObject(RlpObject *rlp_object_ptr);


/*!****************************************************************************
 * @brief Delete encoded RLP stream in an RLP object and all its children
 *
 * @details
 *   This function deletes encoded RLP stream in an RlpObject and all its children
 *   RlpObject (if it's of LIST and there should be some children).
 *   \n This function only deletes the encoded RLP stream while leaves the RlpObject's
 *   descriptor as is. Thus the tree-ed structure of its children is not affected.
 *   \n This function is used in an RLP re-encoding operation. It cleans all previously
 *   encoded RLP stream and prepares for a re-encoding.
 *
 * @param[in] rlp_object_ptr
 *   The parent RlpObject which contains the encoded RLP stream to delete.
 *
 * @return
 *   This function doesn't return anything.
 *
 * @see RlpRecursiveDeleteObject()
 ******************************************************************************/
void RlpRecursiveDeleteEncodedStream(RlpObject *rlp_object_ptr);


/*!****************************************************************************
 * @brief Calculate the encoded RLP stream size in an RLP object and all its children
 *
 * @details
 *   This function calculates the size of the encoded RLP stream in an RlpObject
 *   and all its children RlpObject (if it's of LIST and there should be some children).
 *   \n The size returned is the total size of the stream that would be encoded by
 *   RlpEncode(). It's used to allocate memory before executing an RLP encoding
 *   operation.
 *
 * @note
 *   This function doesn't actually encode the stream and doesn't allocate
 *   any dynamic memory.
 *   
 * @param[in] rlp_object_ptr
 *   The RlpObject to be encode.
 *
 * @param[out] rlp_head_size_ptr
 *   The RLP head size within the encoded stream.
 *
 * @return
 *   This function returns the size of the encoded stream.
 *
 * @see RlpEncode()
 ******************************************************************************/
BUINT32 RlpRecursiveCalcEncodingSize(RlpObject *rlp_object_ptr, BOAT_OUT BUINT8 *rlp_head_size_ptr);


/*!****************************************************************************
 * @brief Encode an RLP stream as per RLP encoding rules.
 *
 * @details
 *   This function recursively encodes an RLP stream for the specified RlpObject
 *   and its children (if there be).
 *   \n There are 3 possible types of RLP encoding structure as per RLP rules:
 *   1. encoded = <field>, if field_len is 1
 *   2. encoded = <1 byte prefix>|<field>, if field_len is in range [0,55] except 1
 *   3. encoded = <1 byte prefix>|<field_len>|<field>, if field_len >= 56
 *   where "|" means concatenaion.
 *   \n If encoding completes successfully, call RlpGetEncodedStream() to get the
 *   encoded RLP stream.
 *   \n If the RlpObject is no longer required, call RlpRecursiveDeleteObject() to
 *   delete all tree-ed RlpObject.
 *
 * @note
 *   See https://github.com/ethereum/wiki/wiki/RLP for details of the encoding rule.
 *
 *   @param[in] rlp_object_ptr
 *   The RlpObject to encode.
 *
 * @param[in] parent_storage_ptr
 *   The parent stream object to store the encoded RLP stream.\n\n
 *   If the <rlp_object_ptr> is the most outer LIST RlpObject, <parent_storage_ptr>
 *   can be specified with NULL, which allows the RLP encoder to automatically
 *   calculate the stream size and allocate memory for it.\n\n
 *   For a STRING RlpObject, the caller must calculate its size with
 *   RlpRecursiveCalcEncodingSize() and allcoate <parent_storage_ptr> for it. 
 *
 * @return 
 *   This function returns BOAT_SUCCESS if successful.
 *   \n It returns one of the error codes if encode failed. Refer to header file boaterrcode.h for details.
 *
 * @see RlpRecursiveCalcEncodingSize() RlpGetEncodedStream() RlpRecursiveDeleteObject()
 ******************************************************************************/
BOAT_RESULT RlpEncode(RlpObject *rlp_object_ptr, RlpEncodedStreamObject *parent_storage_ptr);


/*!****************************************************************************
 * @brief Re-encode an RLP stream as per RLP encoding rules.
 *
 * @details
 *   This function recursively cleans previously encoded RLP stream for the
 *   specified RlpObject and re-encode the RLP stream.
 *
 * @param[in] rlp_object_ptr
 *   The RlpObject to encode.
 *
 * @param[in] parent_storage_ptr
 *   The parent stream object to store the encoded RLP stream.\n\n
 *   If the <rlp_object_ptr> is the most outer LIST RlpObject, <parent_storage_ptr>\n
 *   can be specified with NULL, which allows the RLP encoder to automatically\n
 *   calculate the stream size and allocate memory for it.\n\n
 *   For a STRING RlpObject, the caller must calculate its size with\n
 *   RlpRecursiveCalcEncodingSize() and allcoate <parent_storage_ptr> for it. 
 *
 * @return 
 *   This function returns BOAT_SUCCESS if successful.
 *   \n It returns one of the error codes if re-encode failed. Refer to header file boaterrcode.h for details.
 *
 * @see RlpEncode()
 ******************************************************************************/
BOAT_RESULT RlpReEncode(RlpObject *rlp_object_ptr, RlpEncodedStreamObject *parent_storage_ptr);


/*!****************************************************************************
 *@brief Get the encoded RLP stream
 *
 * @details
 *   This function returns encoded RLP stream of the specified RlpObject.  
 *   \n Make sure RlpEncode() or RlpReEncode() has been successfully called for
 *   the RlpObject.
 *
 * @param[in] rlp_object_ptr
 *   The encoded RlpObject.
 *  
 * @return 
 *   This function returns pointer to the encoded RLP stream object.
 *   \n It returns NULL if get failed.
 * 
 * @see RlpEncode() RlpReEncode()
 ******************************************************************************/
RlpEncodedStreamObject * RlpGetEncodedStream(RlpObject *rlp_object_ptr);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
