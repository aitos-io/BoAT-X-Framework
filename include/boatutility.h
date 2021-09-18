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

/*!@brief Header file for utility functions

@file
boatutility.h is header file for boatwallet utility functions.
*/

#ifndef __BOATUTILITY_H__
#define __BOATUTILITY_H__

#include "boatiotsdk.h"
#if PROTOCOL_USE_HWBCS == 1
#include "protocolapi/api_hlfabric.h"
#include "protocolapi/api_hw_bcs.h"
#endif

/*! @defgroup utility boat utility
 * @{
 */

//! The maximum allowed string length
#define BOAT_STRING_MAX_LEN					     4096 

//!@brief Argument type for UtilityTrimBin(), UtilityHexToBin() and UtilityUint32ToBigend()
typedef enum
{
    TRIMBIN_TRIM_NO,    //!< Don't trim zeros
    TRIMBIN_LEFTTRIM    //!< Trim leading or MSB zeros
}TRIMBIN_TRIM_MODE;


//!@brief Argument type for UtilityHexToBin()
typedef enum
{
    BIN2HEX_LEFTTRIM_UNFMTDATA = 0,   //!< Trim {0x00, 0x01, 0x00 0xAB} => "0x000100AB" or "000100AB"
    BIN2HEX_LEFTTRIM_QUANTITY,        //!< Trim {0x00, 0x01, 0x00 0xAB} => "0x100AB" or "100AB"
	BIN2HEX_LEFTTRIM_TWOHEXPERBYTE    //!< Trim {0x00, 0x01, 0x00 0xAB} => "0x0100AB" or "0100AB"
}BIN2HEX_TRIM_MODE;

//!@brief Argument type for UtilityBinToHex()
typedef enum
{
    BIN2HEX_PREFIX_0x_NO = 0,   //<! Prepend "0x" to converted HEX string
    BIN2HEX_PREFIX_0x_YES       //<! Don't prepend "0x" to converted HEX string
}BIN2HEX_PREFIX_0x_MODE;


//!@brief A structure to maintain a dynamic length string.
typedef struct TStringWithLen
{
    BCHAR  *string_ptr;  //!< address of the string storage
    BUINT32 string_len;  //!< string length in byte excluding NULL terminator, equal to strlen(string_ptr)
    BUINT32 string_space;//!< size of the space <string_ptr> pointing to, including null terminator
}StringWithLen;


//!@brief A structure to big number.
//!bignum256 are 256 bits stored as 8*30 bit + 1*16 bit
//!val[0] are lowest 30 bits, val[8] highest 16 bits
typedef struct 
{
	BUINT32 val[9];
} utilityBignum256;



/*!****************************************************************************
 * @brief Round value up to the nearest multiple of step
 * 
 * @param value
 *   The value to round up.
 *
 * @param step
 *   The step of round.
 ******************************************************************************/
#define BOAT_ROUNDUP(value, step) ((((value) - 1)/(step) + 1) * (step))


/*!****************************************************************************
 * @brief Return the minimal value of two
 *
 * @param value1
 *   The first value to compare.
 *
 * @param value2
 *   The second value to compare.
 ******************************************************************************/
#define BOAT_MIN(value1, value2) ((value1) < (value2) ? (value1) : (value2))


/*!****************************************************************************
 * @brief Return the maximal value of two
 *
 * @param value1
 *   The first value to compare.
 *
 * @param value2
 *   The second value to compare.
 ******************************************************************************/
#define BOAT_MAX(value1, value2) ((value1) > (value2) ? (value1) : (value2))

#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Trim zeros of a binary stream
 * 
 * @details
 *   This function trims zeros of a binary stream and returns the length after
 *   trimming. Currently only left trimming (i.e. trimming leading zeros) is
 *   supported.
 *   \n This function simply ignores leading zeros and copy the first non-zero byte
 *   til the end of the stream to the target buffer. It doesn't treat the data
 *   in buffer as integer, i.e. it doesn't care about the endian.
 *   \n The source and target buffer may be overlapped.
 *   \n For example, {0x00, 0x01, 0x02, 0x00} is trimed to {0x01, 0x02, 0x00} and
 *   returns 3.
 *   
 * @param to_ptr
 *   A buffer to contain the trimmed binary stream. Its size must be at
 *   least <from_len> bytes. <to_ptr> and <from_ptr> may be overlapped.
 *       
 * @param from_ptr
 *   The binary stream to trim. <to_ptr> and <from_ptr> may be overlapped.
 *
 * @param from_len
 *   Length of <from_ptr>.
 *
 * @param trim_mode
 *   Only TRIMBIN_LEFTTRIM is supported, i.e. trim leading zeros.
 *   \n If TRIMBIN_TRIM_NO is specified, this function acts like memmove().
 *       
 * @param zero_as_null
 *   In case the binary stream contains only one or multiple 0x00:
 *   \n If <zero_as_null> is BOAT_TRUE, it will be treated as a null stream and
 *   returns 0.
 *   \n If <zero_as_null> is BOAT_FALSE, it will be trimed to a single 0x00 and
 *   returns 1.
 *   \n <zero_as_null> should be BOAT_TRUE for RLP-Encoding purpose.
 *
 * @return
 *   This function returns how many bytes the trimmed binary stream is.
 ******************************************************************************/
BUINT32 UtilityTrimBin(BOAT_OUT BUINT8 *to_ptr,
					   const BUINT8 *from_ptr,
					   BUINT32 from_len,
					   TRIMBIN_TRIM_MODE trim_mode,
					   BBOOL zero_as_null);


/*!****************************************************************************
 * @brief 
 *   Convert a binary stream to HEX string with optional leading zeros trimming 
 *   and "0x" prefix
 * 
 * @details
 *   This function converts a binary stream to a null-terminated HEX string.
 *   \n Optionally leading zeros can be trimmed during conversion.
 *   \n Optionally "0x" prefix can be prepended to the HEX string.
 *   \n There is no space to delimit every HEX code.
 *   The case of "a" through "f" is always lower case.  
 *   \n There are 3 ways to trim leading zeros. For example, a binary stream
 *   {0x00, 0x01, 0x00, 0xAB} will be converted to string:   
 *      "000100ab": if <trim_mode> = BIN2HEX_LEFTTRIM_UNFMTDATA
 *   \n "100ab"   : if <trim_mode> = BIN2HEX_LEFTTRIM_QUANTITY
 *   \n "0100ab"  : if <trim_mode> = BIN2HEX_LEFTTRIM_TWOHEXPERBYTE
 *
 * @note
 *   This function doesn't treat the binary stream as an integer and thus
 *    it doens't make any endian conversion.
 *
 * @param[out] to_str
 *   The buffer to hold the converted HEX string.\n
 *   Its size should be least <from_len>*2+1 bytes if <prefix_0x_mode>=BIN2HEX_PREFIX_0x_NO,\n
 *   or <from_len>*2+3 bytes if <prefix_0x_mode>=BIN2HEX_PREFIX_0x_YES.\n
 *   If <to_str> is NULL, this function does nothing and returns 0.
 *       
 * @param[in] from_ptr
 *   The binary stream to convert from.\n
 *   If <from_ptr> is NULL and <to_str> is NOT NULL, this function writes a
 *   '\0' to <to_str>, i.e. a NULL string and returns 1.
 *
 * @param[in] from_len
 *   The length of <from_ptr> in byte.\n
 *   If <from_len> is 0 and <to_str> is NOT NULL, this function writes a
 *   '\0' to <to_str>, i.e. a NULL string and returns 1.
 *
 * @param[in] trim_mode
 *   BIN2HEX_LEFTTRIM_UNFMTDATA to trim as unformatted data;\n
 *   BIN2HEX_LEFTTRIM_QUANTITY to trim as a quantity;\n
 *   BIN2HEX_LEFTTRIM_TWOHEXPERBYTE to trim byte with a value of 0.
 *
 * @param[in] prefix_0x_mode
 *   BIN2HEX_PREFIX_0x_YES: Prepend a "0x" prefix at the beginning of the HEX string;
 *   \n BIN2HEX_PREFIX_0x_NO:  Don't prepend "0x" prefix.
 *       
 * @param[in] zero_as_null
 *   In case the binary stream contains only one or multiple 0x00:
 *   \n If <zero_as_null> is BOAT_TRUE, it will be converted to a null sting and
 *   returns 0.
 *   \n If <zero_as_null> is BOAT_FALSE, it will be converted to "0" or "00"
 *   according to <trim_mode>.
 *   \n <zero_as_null> should be BOAT_TRUE for RLP-Encoding purpose.
 *
 * @return
 *   This function returns the length of the converted HEX string excluding NULL
 *   terminator. It equals to strlen of the string.
 *   \n If <to_str> is NULL, this function does nothing and returns 0.
 *   \n If <from_len> is 0 or <from_ptr> is NULL, and <to_str> is NOT NULL, this
 *   function writes a '\0' to <to_str>, i.e. a NULL string and returns 0.
 ******************************************************************************/
BUINT32 UtilityBinToHex(BOAT_OUT BCHAR *to_str,
						const BUINT8 *from_ptr,
						BUINT32 from_len,
						BIN2HEX_TRIM_MODE trim_mode,
						BIN2HEX_PREFIX_0x_MODE prefix_0x_mode,
						BBOOL zero_as_null);


/*!****************************************************************************
 * @brief Convert a HEX string to binary stream with optional leading zeros trimming
 * 
 * @details
 *   This function converts a HEX string to a binary stream.
 *   \n Optionally leading zeros can be trimmed during conversion.
 *   \n If there is "0x" prefix at the beginning of the HEX string, it's ignored.
 *   \n There should be no space between HEX codes.
 *   \n If <to_size> is too small to hold the converted binary stream, only the
 *   beginning <to_size> bytes are converted and written to <to_ptr>.
 *   \n The trim_mode is the same as UtilityTrimBin(). Odd length of HEX string is
 *   allowed as if it were left filled with a "0". For example, a HEX string
 *   "0x00123ab" is treated as "0x000123ab" and converted to:
 *   {0x01, 0x23, 0xab}: if <trim_mode> = TRIMBIN_LEFTTRIM;
 *   {0x00, 0x01, 0x23, 0xab}: if <trim_mode> = TRIMBIN_TRIM_NO
 *
 * @note
 *   This function doesn't treat the HEX string as an integer and thus
 *   it doens't make any endian conversion.
 *
 * @param[out] to_ptr
 *   The buffer to hold the converted binary stream.
 *
 * @param[in] to_size
 *   The size of <to_ptr> in bytes.
 *   \n It's safe to ensure <to_size> >= (strlen(from_str)+1)/2 bytes.
 *       
 * @param[in] from_str
 *   The null-terminated HEX string to convert from. "0x" prefix is optional
 *   and ignored. There should be no space between HEX codes.
 *       
 * @param[in] trim_mode
 *   TRIMBIN_LEFTTRIM: Trim leading zeros;\n
 *   TRIMBIN_TRIM_NO:  Don't trim.
 *
 * @param[in] zero_as_null
 *   In case the HEX string contains only one or multiple "00" and <trim_mode>
 *   is TRIMBIN_LEFTTRIM:
 *   \n If <zero_as_null> is BOAT_TRUE, nothing will be written to <to_ptr> and
 *   it returns 0.
 *   \n If <zero_as_null> is BOAT_FALSE, it will be converted to a single 0x00
 *   and returns 1.
 *   \n <zero_as_null> should be BOAT_TRUE for RLP-Encoding purpose.
 *
 * @return
 *   This function returns how many bytes the converted binary stream is if the
 *   conversion completes successfully.
 *   \n If the output buffer is too small to hold the converted binary stream, 
 *   it returns <to_size>.
 *   \n If any non-HEX character is encountered, or <from_str> is an all-zero HEX
 *   string and <trim_mode> is TRIMBIN_LEFTTRIM, it returns 0.
 *	
 * @see UtilityTrimBin()
*******************************************************************************/
BUINT32 UtilityHexToBin(BOAT_OUT BUINT8 *to_ptr,
                        BUINT32 to_size,
                        const BCHAR *from_str,
                        TRIMBIN_TRIM_MODE trim_mode,
                        BBOOL zero_as_null);


/*!****************************************************************************
 * @brief Convert a host endian BUINT32 to bigendian with optional MSB zeros trimming
 * 
 * @details
 *   This function converts a host endian (typically littleendian) BUINT32 integer
 *   to bigendian.
 *   \n Optionally most siganificant zeros can be trimmed during conversion.
 *   \n The trim_mode is the same as UtilityTrimBin(). For example, a BUINT32 integer
 *   0x000123ab is converted to:
 *   \n {0x01, 0x23, 0xab}: if <trim_mode> = TRIMBIN_LEFTTRIM;
 *   \n {0x00, 0x01, 0x23, 0xab}: if <trim_mode> = TRIMBIN_TRIM_NO
 *
 * @param[out] to_big_ptr
 *   The buffer to hold the converted binary stream.
 *       
 * @param[in] from_host_integer
 *   The host endian BUINT32 integer to convert.
 *
 * @param[in] trim_mode
 *   TRIMBIN_LEFTTRIM: Trim MSB zeros;\n
 *   TRIMBIN_TRIM_NO:  Don't trim.
 *
 * @return
 *   This function returns how many bytes the converted bigendian integer is.
 *   \n If <trim_mode> = TRIMBIN_TRIM_NO, it always returns 4.
 * @see UtilityTrimBin()
 ******************************************************************************/
BUINT8 UtilityUint32ToBigend(BOAT_OUT BUINT8 *to_big_ptr,
							 BUINT32 from_host_integer,
							 TRIMBIN_TRIM_MODE trim_mode);


/*!****************************************************************************
 * @brief Convert a host endian BUINT64 to bigendian with optional MSB zeros trimming
 * 
 * @details
 *   This function converts a host endian (typical littleendian) BUINT64 integer
 *   to bigendian.
 *   \n Optionally most siganificant zeros can be trimmed during conversion.
 *   \n It's a 64-bit version of UtilityUint32ToBigend(). 
 *   
 * @param[out] to_big_ptr
 *   The buffer to hold the converted binary stream.
 *       
 * @param[in] from_host_integer
 *   The host endian BUINT64 integer to convert.
 *
 * @param[in] trim_mode
 *   TRIMBIN_LEFTTRIM: Trim MSB zeros;\n
 *   TRIMBIN_TRIM_NO:  Don't trim.
 *   
 * @return
 *   This function returns how many bytes the converted bigendian integer is.
 *   \n If <trim_mode> = TRIMBIN_TRIM_NO, it always returns 8.
 *
 * @see UtilityUint32ToBigend()  UtilityTrimBin()
*******************************************************************************/
BUINT8 UtilityUint64ToBigend(BOAT_OUT BUINT8 *to_big_ptr,
							 BUINT64 from_host_integer,
							 TRIMBIN_TRIM_MODE trim_mode);


/*!****************************************************************************
 * @brief Change endianness
 * 
 * @details
 *   This function changes a value's endianness. It simply swaps Byte[0] and
 *   Byte[n-1], Byte[1] and Byte[n-2]...
 *
 * @param[inout] value_ptr
 *   The value to change endianness.
 *       
 * @param[in] value_len
 *   Size (in byte) of <value_ptr>.
 *  
 * @return
 *   This function returns the pointer of the converted value.
 *   \n If <value_ptr> is NULL, it returns NULL.  
 ******************************************************************************/
void * UtilityChangeEndian(BOAT_INOUT void *value_ptr, BUINT32 value_len);


/*!*****************************************************************************
 * @brief Convert a host endian BUINT32 to bigendian
 * 
 * @details
 *   This function converts a host endian (typically littleendian) BUINT32 integer
 *   to bigendian.
 *
 * @param[in] from_host_integer
 *   The host endian BUINT32 integer to convert.
 *
 * @return
 *   This function returns the converted bigendian BUINT32 integer.
 * 	
 * @see UtilityUint32ToBigend()
 ******************************************************************************/
BUINT32 UtilityHtonl(BUINT32 from_host_integer);


/*!****************************************************************************
 * @brief Convert a bigendian BUINT32 to host endian
 * 
 * @details
 *   This function converts a bigendian BUINT32 integer to host endian 
 *   (typically littleendian).
 * 
 * @param[in] from_big_integer
 *   The big endian BUINT32 integer to convert.
 *
 * @return
 *   This function returns the converted host endian BUINT32 integer.
 *
 * @see UtilityHtonl()
 ******************************************************************************/
BUINT32 UtilityNtohl(BUINT32 from_host_integer);


/*!****************************************************************************
 * @brief Convert a string representing wei in HEX to ether in double float
 * 
 * @details
 *   This function converts a string representing wei in HEX to ether in double
 *   float.
 *   \n 1 ether is 1e18 wei. Note that in Ethereum an integer type is up to 256 bits
 *   while most C compilers natively support up to 64-bit integer. A 64-bit
 *   unsigned interger can represent up to 2^64 - 1, which is roughly 1.845e19.
 *   Thus it's not possible to convert more than 1.845e19 wei (i.e. 18.45 ether)
 *   to native integer type.
 *   \n Converting integer type wei to double float type ether loses pricision.
 *   Fortunately the unit of ether is usually for friendly human-reading only
 *   and slight pricision loss is not a problem.
 *
 * @param[in] wei_str
 *   The HEX-represented string of wei, either in "0xabcd..." or "abcd..." format.
 *
 * @return
 *   This function returns the converted ether in double float.
*******************************************************************************/
double UtilityWeiStrToEthDouble(const BCHAR *wei_str);


/*!****************************************************************************
* @brief Convert a 32 bytes binary data to big number format
*
* @details
*    Input 32 bytes binary data should be big endian. for example:
*    - 0x12       should stored as 0x00, 0x00, ..., 0x00, 0x00, 0x00, 0x12.
*    - 0x1234     should stored as 0x00, 0x00, ..., 0x00, 0x00, 0x12, 0x34.
*    - 0x12345678 should stored as 0x00, 0x00, ..., 0x12, 0x34, 0x56, 0x78.
*
* @param[in] in_number
*    Input data pointer, It's should be a 32 bytes length array.
*
* @param[out] out_number
*    The pointer to stored converted data in big number format.
*
* @see UtilityWriteBignumToBigend
*******************************************************************************/
void UtilityReadBigendToBignum(const BUINT8 *in_number, utilityBignum256 *out_number);


/*!****************************************************************************
* @brief Convert a 32 bytes binary data to big number format
*
* @details
*    Output 32 bytes binary data is  big endian format. for example:
*    - 0x12       should stored as 0x00, 0x00, ..., 0x00, 0x00, 0x00, 0x12.
*    - 0x1234     should stored as 0x00, 0x00, ..., 0x00, 0x00, 0x12, 0x34.
*    - 0x12345678 should stored as 0x00, 0x00, ..., 0x12, 0x34, 0x56, 0x78.
*
* @param[in] in_number
*    Input data pointer, It's in big number format.
*
* @param[out] out_number
*    output data pointer, It's should be a 32 bytes length array.
*
* @see UtilityReadBigendToBignum
*******************************************************************************/
void UtilityWriteBignumToBigend(const utilityBignum256 *in_number, BUINT8 *out_number);


/*!****************************************************************************
* @brief check the string length is out of limit or not
*
* @details
*    check the string length is out of limit or not
* @param[in] string
*    The string that to be checked.

* @return
*   If the string length less than maxium limit, return \c BOAT_SUCCESS, else
*   return  BOAT_ERROR.
*******************************************************************************/
BOAT_RESULT UtilityStringLenCheck(const BCHAR *string);

BUINT64 UtilityBuint8Buf2Uint64(BUINT8* from,BUINT32 len);


#if(BOAT_HWBCS_TLS_SUPPORT == 1)
#include "mbedtls/x509_crt.h"
#include "mbedtls/oid.h"
size_t Utility_find_oid_value_in_name(const mbedtls_x509_name *name, const char* target_short_name, char *value, size_t value_length);
#endif

char *Utility_itoa(int num, char *str, int radix);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
