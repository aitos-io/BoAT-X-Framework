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

/*!@brief Utility functions

@file
boatutility.c contains utility functions for boatwallet.
*/

#include "boatinternal.h"
#include "boatlog.h"
#include "base64.h"

//!@brief Literal representation of log level
const BCHAR *const g_log_level_name_str[] =
    {
        "LOG_CRITICAL",
        "LOG_NORMAL",
        "LOG_VERBOSE"};

BUINT32 UtilityTrimBin(BOAT_OUT BUINT8 *to_ptr,
                       const BUINT8 *from_ptr,
                       BUINT32 from_len,
                       TRIMBIN_TRIM_MODE trim_mode,
                       BBOOL zero_as_null)
{
    BUINT32 from_offset = 0;

    if (to_ptr == NULL || from_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "<to_ptr> and <from_ptr> cannot be NULL.");
        return 0;
    }

    if (trim_mode == TRIMBIN_TRIM_NO)
    {
        // Duplicate byte array
        if (to_ptr != from_ptr)
        {
            memmove(to_ptr, from_ptr, from_len);
        }
    }
    else // if (trim_mode == TRIMBIN_LEFTTRIM)
    {
        for (from_offset = 0; from_offset < from_len; from_offset++)
        {
            // Trim leading 0x00, i.e. {0x00, 0x01, 0x00 0xAB} => {0x01, 0x00 0xAB}
            if (from_ptr[from_offset] == 0)
            {
                continue;
            }
            else
            {
                memmove(to_ptr, from_ptr + from_offset, from_len - from_offset);
                break;
            }
        }
    }

    // Special process for all zero byte array
    if (trim_mode != TRIMBIN_TRIM_NO && from_offset == from_len)
    {
        if (zero_as_null == BOAT_FALSE)
        {
            to_ptr[0] = 0;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    return from_len - from_offset;
}

BUINT32 UtilityBinToHex(BOAT_OUT BCHAR *to_str,
                        const BUINT8 *from_ptr,
                        BUINT32 from_len,
                        BIN2HEX_TRIM_MODE trim_mode,
                        BIN2HEX_PREFIX_0x_MODE prefix_0x_mode,
                        BBOOL zero_as_null)
{
    BUINT32 to_offset;
    unsigned char halfbyte;
    unsigned char octet;
    char halfbytechar;
    int i, j;
    BBOOL trim_done;

    if (to_str == NULL)
    {
        return 0;
    }

    if (from_ptr == NULL || from_len == 0)
    {
        to_str[0] = '\0';
        return 0;
    }

    to_offset = 0;

    if (prefix_0x_mode == BIN2HEX_PREFIX_0x_YES)
    {
        to_str[to_offset++] = '0'; // "0x"
        to_str[to_offset++] = 'x';
    }

    if (trim_mode == BIN2HEX_LEFTTRIM_UNFMTDATA)
    {
        trim_done = BOAT_TRUE;
    }
    else
    {
        trim_done = BOAT_FALSE;
    }

    for (i = 0; i < from_len; i++)
    {
        octet = from_ptr[i];

        // Trim leading double zeroes, i.e. {0x00, 0x01, 0x00 0xAB} => "0100AB"
        if (trim_done == BOAT_FALSE && octet == 0)
        {
            continue;
        }

        for (j = 0; j < 2; j++)
        {
            halfbyte = (octet >> (4 - j * 4)) & 0x0F;

            // Trim all leading zeroes, i.e. {0x00, 0x01, 0x00 0xAB} => "100AB"
            if (trim_done == BOAT_FALSE && trim_mode == BIN2HEX_LEFTTRIM_QUANTITY && halfbyte == 0)
            {
                continue;
            }

            if (halfbyte <= 0x9)
            {
                halfbytechar = halfbyte + '0';
            }
            else
            {
                halfbytechar = halfbyte - 10 + 'a';
            }

            to_str[to_offset++] = halfbytechar;

            trim_done = BOAT_TRUE;
        }
    }

    // Special process for all zero byte array
    if ((prefix_0x_mode == BIN2HEX_PREFIX_0x_YES && to_offset == 2) || // HEX contains only "0x"
        (prefix_0x_mode == BIN2HEX_PREFIX_0x_NO && to_offset == 0))    // HEX contains nothing
    {
        if (zero_as_null == BOAT_FALSE)
        {
            if (trim_mode == BIN2HEX_LEFTTRIM_QUANTITY)
            {
                to_str[to_offset++] = '0'; // "0"
            }
            else
            {
                to_str[to_offset++] = '0'; // "00"
                to_str[to_offset++] = '0';
            }
        }
        else
        {
            to_offset = 0;
        }
    }

    to_str[to_offset] = '\0';

    return to_offset;
}

BUINT32 UtilityHexToBin(BOAT_OUT BUINT8 *to_ptr,
                        BUINT32 to_size,
                        const BCHAR *from_str,
                        TRIMBIN_TRIM_MODE trim_mode,
                        BBOOL zero_as_null)
{
    BUINT32 from_offset;
    BUINT32 from_len;
    BUINT32 to_offset;
    BUINT32 odd_flag;

    unsigned char octet;
    unsigned char halfbyte;
    char halfbytechar;
    BBOOL bool_trim_done;

    if (to_ptr == NULL || to_size == 0 || from_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "<to_ptr>, <to_size> and <from_str> cannot be 0 or NULL.");
        return 0;
    }

    octet = 0;

    from_len = strlen(from_str);

    from_offset = 0;
    to_offset = 0;

    // Skip leading "0x" or "0X" if there be.
    // Note: If strlen(from_ptr) <= 2 or <from_up_to_len> <= 2, no "0x" prefix
    //       is allowed in HEX string.
    if (from_len > 2)
    {
        if (from_str[0] == '0' && (from_str[1] == 'x' || from_str[1] == 'X'))
        {
            from_offset += 2;
        }
    }

    // if HEX length is odd, treat as if it were left filled with one more '0'
    if ((from_len & 0x01) != 0)
    {
        // length is odd
        odd_flag = 1;
    }
    else
    {
        // length is even
        odd_flag = 0;
    }

    if (trim_mode == TRIMBIN_TRIM_NO)
    {
        bool_trim_done = BOAT_TRUE;
    }
    else
    {
        bool_trim_done = BOAT_FALSE;
    }

    while (from_offset < from_len)
    {
        halfbytechar = from_str[from_offset];

        if (halfbytechar >= '0' && halfbytechar <= '9')
        {
            halfbyte = halfbytechar - '0';
        }
        else if (halfbytechar >= 'A' && halfbytechar <= 'F')
        {
            halfbyte = halfbytechar - 'A' + 0x0A;
        }
        else if (halfbytechar >= 'a' && halfbytechar <= 'f')
        {
            halfbyte = halfbytechar - 'a' + 0x0A;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "<from_str> contains non-HEX character 0x%02x (%c) at Position %d of \"%s\".\n", halfbytechar, halfbytechar, from_offset, from_str);
            if (halfbytechar == ' ' || halfbytechar == '\t')
            {
                BoatLog(BOAT_LOG_NORMAL, "There should be no space between HEX codes.");
            }
            return 0;
        }

        // If from_len is even, pack 2 half bytes to a byte when from_offset is odd.
        // For example, "0x012345" is packed when from_offset points to '1', '3' and '5'.
        //
        // If from_len is odd, pack 2 half bytes to a byte when from_offset is even.
        // For example, "0x12345" is packed when from_offset points to '1', '3' and '5'.
        if ((from_offset & 0x01) == odd_flag)
        {
            octet = halfbyte << 4;
            from_offset++;
            continue;
        }
        else
        {
            octet |= halfbyte;

            if (bool_trim_done == BOAT_FALSE && octet == 0x00)
            {
                from_offset++;
                continue; // Trim leading zeros.
            }
            else
            {
                bool_trim_done = BOAT_TRUE;
            }

            to_ptr[to_offset++] = octet;

            from_offset++;

            // Check capacity of output buffer
            if (to_offset >= to_size)
            {
                break;
            }
        }
    }

    // Special process for trimed all zero HEX string
    if (to_offset == 0 && zero_as_null == BOAT_FALSE)
    {
        to_ptr[0] = 0x00;
        to_offset = 1;
    }

    return to_offset;
}

BUINT8 UtilityUint32ToBigend(BOAT_OUT BUINT8 *to_big_ptr,
                             BUINT32 from_host_integer,
                             TRIMBIN_TRIM_MODE trim_mode)
{
    BBOOL bool_trim_done;
    BSINT8 i;
    BSINT8 binary_index;
    BUINT8 octet;

    binary_index = 0;

    if (trim_mode == TRIMBIN_TRIM_NO)
    {
        bool_trim_done = BOAT_TRUE;
    }
    else // trim_mode == TRIMBIN_TRIM_LEFTTRIM
    {
        bool_trim_done = BOAT_FALSE;
    }

    for (i = sizeof(from_host_integer) - 1; i >= 0; i--)
    {
        octet = (BUINT8)(from_host_integer >> i * 8);

        if (bool_trim_done == BOAT_FALSE)
        {
            if (octet == 0x00)
            {
                continue; // Trim MSB zeros.
            }
            else
            {
                bool_trim_done = BOAT_TRUE;
            }
        }

        to_big_ptr[binary_index++] = octet;
    }

    if (binary_index == 0)
    {
        to_big_ptr[0] = 0x00;
        binary_index = 1;
    }

    return binary_index;
}

BUINT8 UtilityUint64ToBigend(BOAT_OUT BUINT8 *to_big_ptr,
                             BUINT64 from_host_integer,
                             TRIMBIN_TRIM_MODE trim_mode)
{
    BBOOL bool_trim_done;
    BSINT8 i;
    BSINT8 binary_index;
    BUINT8 octet;

    binary_index = 0;

    if (trim_mode == TRIMBIN_TRIM_NO)
    {
        bool_trim_done = BOAT_TRUE;
    }
    else // trim_mode == TRIMBIN_TRIM_LEFTTRIM
    {
        bool_trim_done = BOAT_FALSE;
    }

    for (i = sizeof(from_host_integer) - 1; i >= 0; i--)
    {
        octet = (BUINT8)(from_host_integer >> i * 8);

        if (bool_trim_done == BOAT_FALSE)
        {
            if (octet == 0x00)
            {
                continue; // Trim MSB zeros.
            }
            else
            {
                bool_trim_done = BOAT_TRUE;
            }
        }

        to_big_ptr[binary_index++] = octet;
    }

    if (binary_index == 0)
    {
        to_big_ptr[0] = 0x00;
        binary_index = 1;
    }

    return binary_index;
}

void *UtilityChangeEndian(BOAT_INOUT void *value_ptr, BUINT32 value_len)
{
    BSINT8 i;
    BUINT8 octet;

    if (value_ptr != NULL)
    {
        for (i = 0; i < value_len / 2; i++)
        {
            octet = ((BUINT8 *)value_ptr)[i];
            ((BUINT8 *)value_ptr)[i] = ((BUINT8 *)value_ptr)[value_len - 1 - i];
            ((BUINT8 *)value_ptr)[value_len - 1 - i] = octet;
        }
    }

    return value_ptr;
}

BUINT32 UtilityHtonl(BUINT32 from_host_integer)
{
    BUINT32 to_big_integer;

    UtilityUint32ToBigend((BUINT8 *)&to_big_integer, from_host_integer, TRIMBIN_TRIM_NO);

    return to_big_integer;
}

BUINT32 UtilityNtohl(BUINT32 from_big_integer)
{
    BSINT8 i;
    BUINT32 to_host_integer;

    to_host_integer = 0;
    for (i = 0; i < 4; i++)
    {
        *((BUINT8 *)&to_host_integer + i) |= (from_big_integer >> ((3 - i) * 8)) & 0xFF;
    }
    return to_host_integer;
}

double UtilityWeiStrToEthDouble(const BCHAR *wei_str)
{
    BUINT32 wei_bin_len;
    BUINT8 wei_bin[(strlen(wei_str) + 1) / 2];
    BUINT64 wei_int64;
    BUINT8 *wei_int64_addr_ptr;
    BUINT32 shift_bits;
    double scale_double;
    double wei_double;
    double ether_double;
    BUINT32 i;

    // Conver wei from HEX to binary with leading zeros trimmed
    wei_bin_len = UtilityHexToBin(wei_bin, sizeof(wei_bin), wei_str,
                                  TRIMBIN_LEFTTRIM, BOAT_FALSE);

    // Above binary representation of wei is in bigendian and it's possibly
    // larger than BUINT64 (8 bytes).
    // To convert it to littleendian, every byte in wei_bin is copied to wei_int64
    // reversely.
    //
    // If wei_bin is more than 8 bytes, only most significant 8 bytes are copied,
    // as if the integer were right shifted by some bits until its value is
    // no more than BUINT64. After it's converted to double, 2^shift_bits is
    // multiplied by to recover its value.
    //
    // If wei_bin is no more than 8 bytes, only effective bytes are copied to
    // wei_int64.

    wei_int64 = 0;
    wei_int64_addr_ptr = (BUINT8 *)&wei_int64;

    if (wei_bin_len > sizeof(BUINT64))
    {
        // If wei_bin is more than 8 bytes, copy 8 bytes only
        for (i = 0; i < sizeof(BUINT64); i++)
        {
            // bigendian to littleendian
            wei_int64_addr_ptr[i] = wei_bin[sizeof(BUINT64) - 1 - i];
        }

        shift_bits = (wei_bin_len - sizeof(BUINT64)) * 8;
    }
    else
    {
        // If wei_bin is no more than 8 bytes, copy effective bytes
        for (i = 0; i < wei_bin_len; i++)
        {
            // bigendian to littleendian
            wei_int64_addr_ptr[i] = wei_bin[wei_bin_len - 1 - i];
        }
        shift_bits = 0;
    }

    // Convert BUINT64 to double
    wei_double = (double)wei_int64;

    // Recover shifted bits if any.
    // DO NOT calculate 2^shift_bits by (0x1 << shift_bits), because shift_bits
    // may be more than 64 bits. Calculate it in double instead.
    scale_double = 1.0;

    for (i = 0; i < shift_bits; i++)
    {
        scale_double *= 2.0;
    }

    // Convert wei to ether by division by 1e18
    ether_double = wei_double * scale_double / 1e18;

    BoatLog(BOAT_LOG_VERBOSE, "%s wei converted to %f ether", wei_str, ether_double);

    return ether_double;
}

void UtilityReadBigendToBignum(const BUINT8 *in_number, utilityBignum256 *out_number)
{
    BUINT32 temp = 0;

    for (int i = 0; i < 8; i++)
    {
        // invariant: temp = (in_number % 2^(32i)) >> 30i
        // get next limb = (in_number % 2^(32(i+1))) >> 32i
        BUINT32 limb = (*(in_number + (7 - i) * 4 + 0)) << 24 |
                       (*(in_number + (7 - i) * 4 + 1)) << 16 |
                       (*(in_number + (7 - i) * 4 + 2)) << 8 |
                       (*(in_number + (7 - i) * 4 + 3)) << 0;
        // temp = (in_number % 2^(32(i+1))) << 30i
        temp |= limb << (2 * i);
        // store 30 bits into val[i]
        out_number->val[i] = temp & 0x3FFFFFFF;
        // prepare temp for next round
        temp = limb >> (30 - 2 * i);
    }

    out_number->val[8] = temp;
}

void UtilityWriteBignumToBigend(const utilityBignum256 *in_number, BUINT8 *out_number)
{
    BUINT32 temp = in_number->val[8];

    for (int i = 0; i < 8; i++)
    {
        // invariant: temp = (in_number >> 30*(8-i))
        BUINT32 limb = in_number->val[7 - i];
        temp = (temp << (16 + 2 * i)) | (limb >> (14 - 2 * i));
        *(out_number + (i)*4 + 0) = temp >> 24;
        *(out_number + (i)*4 + 1) = temp >> 16;
        *(out_number + (i)*4 + 2) = temp >> 8;
        *(out_number + (i)*4 + 3) = temp >> 0;
        temp = limb;
    }
}

BOAT_RESULT UtilityStringLenCheck(const BCHAR *string)
{
    for (int i = 0; i < BOAT_STRING_MAX_LEN; i++)
    {
        if (*(string + i) == '\0')
        {
            return BOAT_SUCCESS;
        }
    }
    return BOAT_ERROR;
}

/*
uint32_t random32(void)
{
    static uint32_t seed = 0;
    // Linear congruential generator from Numerical Recipes
    // https://en.wikipedia.org/wiki/Linear_congruential_generator
    seed = 1664525 * seed + 1013904223;

    return seed;
}
*/

BUINT64 UtilityBuint8Buf2Uint64(BUINT8 *from)
{
    BUINT64 ret;
    ret = (((BUINT64)(from[0] & 0x7F) << 56) | ((BUINT64)from[1] << 48) | ((BUINT64)from[2] << 40) | ((BUINT64)from[3] << 32) | ((BUINT64)from[4] << 24) | ((BUINT64)from[5] << 16) | ((BUINT64)from[6] << 8) | from[7]);
    // ret = random32() << 32 | random32();
    // ret =  (((long)from[4] << 24) | ((long)from[5] << 16) | ((long)from[6] << 8) | from[7]);
    return ret;
}

BBOOL UtilityStringIsHex(const BCHAR *input)
{
    BUINT32 i;

    if (input == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "The UtilityStringIsHex input is null");
        return BOAT_FALSE;
    }

    if (input[0] != '0')
    {
        BoatLog(BOAT_LOG_CRITICAL, "The UtilityStringIsHex input can not find prefix 0");
        return BOAT_FALSE;
    }

    if ((input[1] != 'x') && (input[1] != 'X'))
    {
        BoatLog(BOAT_LOG_CRITICAL, "The UtilityStringIsHex input can not find prefix x or X");
        return BOAT_FALSE;
    }

    for (i = 2; i < BOAT_STRING_MAX_LEN; i++)
    {
        if (input[i] == '\0')
        {
            return BOAT_TRUE;
        }
        if ((input[i] < '0') || ((input[i] > '9') && (input[i] < 'A')) || ((input[i] > 'F') && (input[i] < 'a')) || (input[i] > 'f'))
        {
            BoatLog(BOAT_LOG_CRITICAL, "The UtilityStringIsHex find illegal character %d.", input[i]);
            return BOAT_FALSE;
        }
    }
    BoatLog(BOAT_LOG_CRITICAL, "The UtilityStringIsHex input can not find \\0");

    return BOAT_FALSE;
}

BOAT_RESULT BoatFieldVariable_malloc_size_expand(BoatFieldVariable *mem, BUINT32 step_size)
{
    // free before malloc to avoid allocate more memory at same time
    if (mem->field_ptr != NULL)
    {
        BoatFree(mem->field_ptr);
    }

    mem->field_len += step_size;
    mem->field_ptr = BoatMalloc(mem->field_len);
    if (mem->field_ptr == NULL)
    {
        mem->field_len = 0;
        BoatLog(BOAT_LOG_NORMAL, "BoatFieldVariable_malloc_size_expand failed.");
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    BoatLog(BOAT_LOG_NORMAL, "Excuting %s:[%d].", __func__, mem->field_len);
    return BOAT_SUCCESS;
}

char *Utility_itoa(int num, char *str, int radix)
{ /*索引表*/
    char index[] = "0123456789ABCDEF";
    unsigned unum; /*中间变量*/
    int i = 0, j, k;
    /*确定unum的值*/
    if (radix == 10 && num < 0) /*十进制负数*/
    {
        unum = (unsigned)-num;
        str[i++] = '-';
    }
    else
        unum = (unsigned)num; /*其他情况*/
    /*转换*/
    do
    {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;
    } while (unum);
    str[i] = '\0';
    /*逆序*/
    if (str[0] == '-')
        k = 1; /*十进制负数*/
    else
        k = 0;

    for (j = k; j <= (i - 1) / 2; j++)
    {
        char temp;
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }
    return str;
}

/*****************************************************************
密钥格式转换
*******************************************************************/
/*
    从数组中获取第一个tlv格式数据。默认tag的长度都是一个字节。
*/

int UtilityGetTLV_LL(BUINT8 *input, BUINT32 len)
{
    // input[0] 是tag
    if (input[1] < 0x0080)
    {
        return 1;
    }
    else if (input[1] == 0x81)
    {
        return 2;
    }
    else if (input[1] == 0x82)
    {
        return 3;
    }
    else
    {
        return -1;
    }
}

// int UtilityGetTLV_LL(BUINT8 *input, BUINT32 len)
// {
//     // input[0] 是tag
//     if ((input[1] & 0x80) == 0)
//     {
//         return 1;
//     }
//     else
//     {
//         return (input[1] & 0x7F);
//     }
// }

/**
 * @description:
 *  This function get the actual number of bytes in the data length field.
 * @param {BUINT32} len
 * @return {*}
 *  This function will return the number of bytes of len
 * @author: aitos
 */
int UtilityGetTLV_LL_from_len(BUINT32 len)
{
    // input[0] 是tag
    if (len < 0x0080)
    {
        return 1;
    }
    else if ((len / 0x80) == 1)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

// int UtilityGetTLV_L(BUINT8 *input,BUINT32 len){
//     //input[0] 是tag
//     if(input[1] <= 0x0080){
//         return input[1];
//     }else if(input[1] == 0x81){
//         return input[2];
//     }else if(input[1] == 0x82){
//         return ((input[2] << 8 )| input[3]);
//     }else{
//         return -1;
//     }

// }
int UtilityGetTLV_L(BUINT8 *input, BUINT32 len)
{
    // input[0] 是tag
    //  目前只考虑最长两个字节的实际长度
    if ((input[1] & 0x80) == 0x00)
    {
        return input[1];
    }
    else if (input[1] == 0x81)
    {
        return input[2];
    }
    else if (input[1] == 0x82)
    {
        return ((input[2] << 8) | input[3]);
    }
    else
    {
        return -1;
    }
}

/**
 * @description:
 *  This function gets the real length of data according to length bytes.
 * @param {BUINT8} *input
 *  the bytes of length , minimum 1 byte, maximum 3 bytes
 * @return {*}
 *  This function will return the real length if success.
 *  Otherwise it will return -1
 * @author: aitos
 */
int UtilityGetLVData_L(BUINT8 *input)
{
    // input[0] 是tag
    //  目前只考虑两个字节的实际长度
    if ((input[0] & 0x80) == 0x00)
    {
        return input[0];
    }
    else if (input[0] == 0x81)
    {
        return input[1];
    }
    else if (input[0] == 0x82)
    {
        return ((input[1] << 8) | input[2]);
    }
    else
    {
        return -1;
    }
}

BOAT_RESULT UtilityGetTLV(BUINT8 *input, BUINT32 inputlen, TLVStruct *tlv)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    BUINT32 offset = 0, len = 0;
    int ll = 0;

    ll = UtilityGetTLV_LL(input, inputlen);
    if (ll < 0)
    {
        return BOAT_ERROR;
    }
    len = UtilityGetTLV_L(input, inputlen);
    if (len < 0)
    {
        return BOAT_ERROR;
    }
    if (len + ll + 1 > inputlen)
    {
        return BOAT_ERROR;
    }
    offset += (1 + ll);
    (*tlv).tag = input[0];
    (*tlv).Llen = ll;
    (*tlv).len = len;
    (*tlv).data = input + offset;
    return ret;
}

int add_TL_withOffset(BUINT8 bTag, BUINT8 *pbBuff, BUINT32 *nOffset, BUINT32 nLen)
{
    BUINT32 nTLLen = 0;
    BUINT8 bTLV[10];
    BUINT8 *buf = NULL;
    buf = BoatMalloc(nLen);
    if (NULL == buf)
    {
        return -1;
    }

    memset(bTLV, 0, 10);

    if (bTag == BoAT_ASN1_BIT_STRING)
    {
        memcpy(buf, pbBuff + *nOffset, nLen);
        memcpy(pbBuff + *nOffset + 1, buf, nLen);
        nLen += 1;
        pbBuff[*nOffset] = 0x00;
    }
    //鍒ゆ柇nLen澶у皬
    if (nLen < 0x80) // nLen < 0x80
    {
        nTLLen = 2;
        bTLV[0] = bTag;
        bTLV[1] = nLen;
    }
    else if (nLen / 0x80 == 1) // 0xFF > nlen >=0x80
    {
        nTLLen = 3;
        bTLV[0] = bTag;
        bTLV[1] = 0x81;
        bTLV[2] = nLen;
    }
    else
    {
        nTLLen = 4;
        bTLV[0] = bTag;
        bTLV[1] = 0x82;
        bTLV[2] = (nLen & 0xFF00) >> 8;
        bTLV[3] = (nLen & 0x00FF);
    }
    if (nLen != 0)
    {
        memcpy(buf, pbBuff + *nOffset, nLen);
        memcpy(pbBuff + *nOffset, bTLV, nTLLen);
        memcpy(pbBuff + *nOffset + nTLLen, buf, nLen);
        *nOffset += (nLen + nTLLen);
    }

    BoatFree(buf);
    return nLen + nTLLen;
}

/**
 * @description:
 *  This function add L into pbBuff.
 * @param[in/out] {BUINT8*} pbBuff
 *  data bytes
 * @param[in/out] {BUINT32*} nOffset
 *  in : offset of L
 *  out: new offset of data , end of LV
 * @param[in] {BUINT32} nLen
 *  L
 * @return {*}
 *  This function return length of LV if successfully executed ,
 *  Otherwise it will return -1.
 * @author: aitos
 */
int add_L_withOffset(BUINT8 *pbBuff, BUINT32 *nOffset, BUINT32 nLen)
{
    BUINT32 nTLLen = 0;
    BUINT8 bTLV[10];
    BUINT8 *buf = NULL;
    buf = BoatMalloc(nLen);
    if (NULL == buf)
    {
        return -1;
    }

    memset(bTLV, 0, 10);

    if (nLen < 0x80) // nLen < 0x80
    {
        nTLLen = 1;
        bTLV[0] = nLen;
    }
    else if (nLen / 0x80 == 1) // 0xFF > nlen >=0x80
    {
        nTLLen = 2;
        bTLV[0] = 0x81;
        bTLV[1] = nLen;
    }
    else
    {
        nTLLen = 3;
        bTLV[0] = 0x82;
        bTLV[1] = (nLen & 0xFF00) >> 8;
        bTLV[2] = (nLen & 0x00FF);
    }
    if (nLen != 0)
    {
        memcpy(buf, pbBuff + *nOffset, nLen);
        memcpy(pbBuff + *nOffset, bTLV, nTLLen);
        memcpy(pbBuff + *nOffset + nTLLen, buf, nLen);
        *nOffset += (nLen + nTLLen);
    }

    BoatFree(buf);
    return nLen + nTLLen;
}

void UtilityFreeKeypair(KeypairNative keypair)
{
    keypair.alg = 0;
    if ((keypair.prikeylen != 0) && (keypair.prikey != NULL))
    {
        BoatFree(keypair.prikey);
        keypair.prikey = NULL;
    }
    keypair.prikeylen = 0;
    if (keypair.pubkeylen != 0 && keypair.pubkey != NULL)
    {
        BoatFree(keypair.pubkey);
        keypair.pubkey = NULL;
    }
    keypair.pubkeylen = 0;
}

BOAT_RESULT UtilityPKCS2Native(BCHAR *input, KeypairNative *keypair)
{
    BOAT_RESULT ret = BOAT_ERROR;
    BCHAR *begin = NULL;
    BCHAR *end = NULL;
    BCHAR *realdata = NULL;
    BUINT8 pkcsDataHex[160] = {0};
    BUINT8 oid_secp256k1[] = {0x00, 0x0a};
    BUINT8 oid_secp256r1[] = {0x03, 0x01, 0x07};
    BUINT8 oid_sm2sm3[] = {0x2a, 0x81, 0x1c, 0xcf, 0x55, 0x01, 0x82, 0x2d};
    BCHAR bytedata = 0;
    BUINT16 offset = 0, len = 0, offset_level_3 = 0, offset_level_4 = 0, offset_level_5 = 0;
    BUINT16 j = 0;
    TLVStruct TLV_Level_1, TLV_Level_2, TLV_Level_3, TLV_Level_4, TLV_Level_5;
    (*keypair).alg = KEYPAIT_ALG_UNKNOWN;
    (*keypair).prikey = NULL;
    (*keypair).prikeylen = 0;
    (*keypair).pubkey = NULL;
    (*keypair).pubkeylen = 0;
    if (strstr(input, PRIKEY_EC_PKCS_BEGIN) != NULL)
    {
        begin = strstr(input, PRIKEY_EC_PKCS_BEGIN);
        end = strstr(input, PRIKEY_EC_PKCS_END);
        if (NULL == begin || NULL == end)
        {
            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native have no begin or end .");
            return BOAT_ERROR;
        }
        offset = begin - input + strlen(PRIKEY_EC_PKCS_BEGIN);
        len = end - begin - strlen(PRIKEY_EC_PKCS_BEGIN);
        realdata = BoatMalloc(len);
        if (NULL == realdata)
        {
            BoatLog(BOAT_LOG_NORMAL, " fail to malloc realdata .");
            return BOAT_ERROR;
        }
        for (int i = 0; i < len; i++)
        {
            bytedata = input[offset + i];
            if (bytedata != ' ' && bytedata != 0x0A && bytedata != 0x0D)
            {
                realdata[j++] = bytedata;
            }
        }

        len = BoAT_base64_decode(realdata, j, pkcsDataHex);
        BoatFree(realdata);
        if (len == 0)
        {
            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native base64 decode err .");
            return BOAT_ERROR;
        }
        // BoatLog_hexdump(BOAT_LOG_VERBOSE, "pkcsDataHex : ",
        // 					pkcsDataHex, len);

        ret = UtilityGetTLV(pkcsDataHex, len, &TLV_Level_1);
        if (ret != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 1 err .");
            return ret;
        }
        if (TLV_Level_1.tag != (BoAT_ASN1_CONSTRUCTED | BoAT_ASN1_SEQUENCE))
        {
            BoatLog(BOAT_LOG_NORMAL, "UtilityPKCS2Native tlv 1 tag err : %02x.", TLV_Level_1.tag);
            return BOAT_ERROR;
        }
        offset = 0;
        while (offset < TLV_Level_1.len)
        {
            ret = UtilityGetTLV(TLV_Level_1.data + offset, TLV_Level_1.len - offset, &TLV_Level_2);
            if (ret != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 2 err .");
                return ret;
            }
            // BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native TLV_Level_2.tag = %02x .",TLV_Level_2.tag);
            if (TLV_Level_2.tag == BoAT_ASN1_OCTET_STRING)
            {
                // (*keypair).prikey = TLV_Level_2.data;
                (*keypair).prikey = BoatMalloc(TLV_Level_2.len);
                if (NULL == (*keypair).prikey)
                {
                    if ((*keypair).pubkey != NULL)
                    {
                        BoatFree((*keypair).pubkey);
                    }
                    return BOAT_ERROR;
                }
                memcpy((*keypair).prikey, TLV_Level_2.data, TLV_Level_2.len);
                (*keypair).prikeylen = TLV_Level_2.len;
            }
            if ((TLV_Level_2.tag & 0xF0) == (BoAT_ASN1_CONSTRUCTED | BoAT_ASN1_CONTEXT_SPECIFIC))
            {
                while (offset_level_3 < TLV_Level_2.len)
                {
                    ret = UtilityGetTLV(TLV_Level_2.data + offset_level_3, TLV_Level_2.len - offset_level_3, &TLV_Level_3);
                    if (ret != BOAT_SUCCESS)
                    {
                        BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 3 err .");
                        return ret;
                    }
                    // BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native TLV_Level_3.tag = %02x . datalen = %02x",TLV_Level_3.tag,TLV_Level_3.len);
                    if (TLV_Level_3.tag == BoAT_ASN1_OID)
                    {
                        BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native TLV_Level_3.len = %d ", TLV_Level_3.len);
                        if (TLV_Level_3.len >= sizeof(oid_secp256r1))
                        {
                            if (memcmp(TLV_Level_3.data + TLV_Level_3.len - sizeof(oid_secp256k1), oid_secp256k1, sizeof(oid_secp256k1)) == 0)
                            {
                                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native oid_secp256k1 .");
                                (*keypair).alg = KEYPAIT_ALG_SECP256K1;
                                break;
                            }
                            else if (memcmp(TLV_Level_3.data + TLV_Level_3.len - sizeof(oid_secp256r1), oid_secp256r1, sizeof(oid_secp256r1)) == 0)
                            {
                                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native oid_secp256r1 .");
                                (*keypair).alg = KEYPAIT_ALG_SECP256R1;
                                break;
                            }
                            else if (TLV_Level_3.len >= sizeof(oid_sm2sm3))
                            {
                                if (memcmp(TLV_Level_3.data, oid_sm2sm3, sizeof(oid_sm2sm3) - 2) == 0)
                                {
                                    (*keypair).alg = KEYPAIT_ALG_SM;
                                    BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native oid_sm .");
                                    break;
                                }
                                else
                                {
                                    (*keypair).alg = KEYPAIT_ALG_UNKNOWN;
                                    BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native unknown .");
                                    break;
                                }
                            }
                            else
                            {
                                (*keypair).alg = KEYPAIT_ALG_UNKNOWN;
                                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native unknown .");
                                break;
                            }
                        }
                        else
                        {
                            (*keypair).alg = KEYPAIT_ALG_UNKNOWN;
                            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native unknown .");
                            break;
                        }
                    }
                    else if (TLV_Level_3.tag == BoAT_ASN1_BIT_STRING)
                    {
                        if (TLV_Level_3.len != 0x42)
                        {
                            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native pubkeylen err : %02x  .", TLV_Level_3.len);
                            return BOAT_ERROR;
                        }
                        (*keypair).pubkeylen = TLV_Level_3.len - 2;
                        // (*keypair).pubkey = TLV_Level_3.data + 2;
                        (*keypair).pubkey = BoatMalloc(TLV_Level_3.len - 2);
                        if (NULL == (*keypair).pubkey)
                        {
                            if ((*keypair).prikey != NULL)
                            {
                                BoatFree((*keypair).prikey);
                            }
                            return BOAT_ERROR;
                        }
                        memcpy((*keypair).pubkey, TLV_Level_3.data + 2, TLV_Level_3.len - 2);
                        return BOAT_SUCCESS;
                    }
                    offset_level_3 += TLV_Level_3.len + TLV_Level_3.Llen + 1;
                }
            }

            offset += TLV_Level_2.len + TLV_Level_2.Llen + 1;
        }
    }
    else if (strstr(input, PRIKEY_PKCS_BEGIN) != NULL)
    { //"-----BEGIN PRIVATE KEY-----\n"
        begin = strstr(input, PRIKEY_PKCS_BEGIN);
        end = strstr(input, PRIKEY_PKCS_END);
        if (NULL == begin || NULL == end)
        {
            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native have no begin or end .");
            return BOAT_ERROR;
        }
        offset = begin - input + strlen(PRIKEY_PKCS_BEGIN);
        len = end - begin - strlen(PRIKEY_PKCS_BEGIN);
        realdata = BoatMalloc(len);
        if (NULL == realdata)
        {
            BoatLog(BOAT_LOG_NORMAL, " fail to malloc realdata .");
            return BOAT_ERROR;
        }
        for (int i = 0; i < len; i++)
        {
            bytedata = input[offset + i];
            if (bytedata != ' ' && bytedata != 0x0A && bytedata != 0x0D)
            {
                realdata[j++] = bytedata;
            }
        }

        len = BoAT_base64_decode(realdata, j, pkcsDataHex);
        BoatFree(realdata);
        if (len == 0)
        {
            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native base64 decode err .");
            return BOAT_ERROR;
        }
        // BoatLog_hexdump(BOAT_LOG_VERBOSE, "pkcsDataHex : ",
        // 					pkcsDataHex, len);

        ret = UtilityGetTLV(pkcsDataHex, len, &TLV_Level_1);
        if (ret != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 1 err .");
            return ret;
        }
        if (TLV_Level_1.tag != (BoAT_ASN1_CONSTRUCTED | BoAT_ASN1_SEQUENCE))
        {
            BoatLog(BOAT_LOG_NORMAL, "UtilityPKCS2Native tlv 1 tag err : %02x.", TLV_Level_1.tag);
            return BOAT_ERROR;
        }
        offset = 0;
        while (offset < TLV_Level_1.len)
        {
            ret = UtilityGetTLV(TLV_Level_1.data + offset, TLV_Level_1.len - offset, &TLV_Level_2);
            if (ret != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 2 err .");
                return ret;
            }
            // BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native TLV_Level_2.tag = %02x .",TLV_Level_2.tag);
            if (TLV_Level_2.tag == BoAT_ASN1_OCTET_STRING)
            {
                // (*keypair).prikey = TLV_Level_2.data;
                offset_level_3 = 0;
                while (offset_level_3 < TLV_Level_2.len)
                {
                    ret = UtilityGetTLV(TLV_Level_2.data + offset_level_3, TLV_Level_2.len - offset_level_3, &TLV_Level_3);
                    if (ret != BOAT_SUCCESS)
                    {
                        BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 3 err .");
                        return ret;
                    }
                    if (TLV_Level_3.tag == (BoAT_ASN1_CONSTRUCTED | BoAT_ASN1_SEQUENCE))
                    {
                        offset_level_4 = 0;
                        while (offset_level_4 < TLV_Level_3.len)
                        {
                            ret = UtilityGetTLV(TLV_Level_3.data + offset_level_4, TLV_Level_3.len - offset_level_4, &TLV_Level_4);
                            if (ret != BOAT_SUCCESS)
                            {
                                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 3 err .");
                                return ret;
                            }
                            if (TLV_Level_4.tag == BoAT_ASN1_OCTET_STRING)
                            {
                                // (*keypair).prikey = TLV_Level_2.data;
                                (*keypair).prikey = BoatMalloc(TLV_Level_4.len);
                                if (NULL == (*keypair).prikey)
                                {
                                    if ((*keypair).pubkey != NULL)
                                    {
                                        BoatFree((*keypair).pubkey);
                                    }
                                    return BOAT_ERROR;
                                }
                                memcpy((*keypair).prikey, TLV_Level_4.data, TLV_Level_4.len);
                                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native prikey TLV_Level_4.len = %d .", TLV_Level_4.len);
                                (*keypair).prikeylen = TLV_Level_4.len;
                            }
                            if (TLV_Level_4.tag == 0xA1)
                            {
                                offset_level_5 = 0;
                                while (offset_level_5 < TLV_Level_4.len)
                                {
                                    ret = UtilityGetTLV(TLV_Level_4.data + offset_level_5, TLV_Level_4.len - offset_level_5, &TLV_Level_5);
                                    if (ret != BOAT_SUCCESS)
                                    {
                                        BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 3 err .");
                                        return ret;
                                    }
                                    BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native TLV_Level_5 tag = %d , len = %d .", TLV_Level_5.tag, TLV_Level_5.len);
                                    if (TLV_Level_5.tag == 0x03)
                                    {
                                        if (TLV_Level_5.len != 0x42)
                                        {
                                            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native pubkeylen err : %02x  .", TLV_Level_5.len);
                                            return BOAT_ERROR;
                                        }
                                        (*keypair).pubkeylen = TLV_Level_5.len - 2;
                                        // (*keypair).pubkey = TLV_Level_3.data + 2;
                                        (*keypair).pubkey = BoatMalloc(TLV_Level_5.len - 2);
                                        if (NULL == (*keypair).pubkey)
                                        {
                                            if ((*keypair).prikey != NULL)
                                            {
                                                BoatFree((*keypair).prikey);
                                            }
                                            return BOAT_ERROR;
                                        }
                                        memcpy((*keypair).pubkey, TLV_Level_5.data + 2, TLV_Level_5.len - 2);
                                        return BOAT_SUCCESS;
                                    }
                                    offset_level_5 += TLV_Level_5.len + TLV_Level_5.Llen + 1;
                                }
                            }
                            offset_level_4 += TLV_Level_4.len + TLV_Level_4.Llen + 1;
                        }
                    }
                    offset_level_3 += TLV_Level_3.len + TLV_Level_3.Llen + 1;
                }
            }
            if ((TLV_Level_2.tag & 0xF0) == (BoAT_ASN1_CONSTRUCTED | BoAT_ASN1_SEQUENCE))
            {
                offset_level_3 = 0;
                while (offset_level_3 < TLV_Level_2.len)
                {
                    ret = UtilityGetTLV(TLV_Level_2.data + offset_level_3, TLV_Level_2.len - offset_level_3, &TLV_Level_3);
                    if (ret != BOAT_SUCCESS)
                    {
                        BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native get tlv 3 err .");
                        return ret;
                    }
                    // BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native TLV_Level_3.tag = %02x . datalen = %02x",TLV_Level_3.tag,TLV_Level_3.len);
                    if (TLV_Level_3.tag == BoAT_ASN1_OID)
                    {
                        BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native TLV_Level_3.len = %d ", TLV_Level_3.len);
                        if (TLV_Level_3.len >= sizeof(oid_secp256r1))
                        {
                            if (memcmp(TLV_Level_3.data + TLV_Level_3.len - sizeof(oid_secp256k1), oid_secp256k1, sizeof(oid_secp256k1)) == 0)
                            {
                                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native oid_secp256k1 .");
                                (*keypair).alg = KEYPAIT_ALG_SECP256K1;
                                break;
                            }
                            else if (memcmp(TLV_Level_3.data + TLV_Level_3.len - sizeof(oid_secp256r1), oid_secp256r1, sizeof(oid_secp256r1)) == 0)
                            {
                                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native oid_secp256r1 .");
                                (*keypair).alg = KEYPAIT_ALG_SECP256R1;
                                break;
                            }
                            else if (TLV_Level_3.len >= sizeof(oid_sm2sm3))
                            {
                                if (memcmp(TLV_Level_3.data, oid_sm2sm3, sizeof(oid_sm2sm3) - 2) == 0)
                                {
                                    (*keypair).alg = KEYPAIT_ALG_SM;
                                    BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native oid_sm .");
                                    break;
                                }
                                else
                                {
                                    (*keypair).alg = KEYPAIT_ALG_UNKNOWN;
                                    BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native unknown .");
                                }
                            }
                            else
                            {
                                (*keypair).alg = KEYPAIT_ALG_UNKNOWN;
                                BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native unknown .");
                            }
                        }
                        else
                        {
                            (*keypair).alg = KEYPAIT_ALG_UNKNOWN;
                            BoatLog(BOAT_LOG_NORMAL, " UtilityPKCS2Native unknown .");
                        }
                    }
                    offset_level_3 += TLV_Level_3.len + TLV_Level_3.Llen + 1;
                }
            }

            offset += TLV_Level_2.len + TLV_Level_2.Llen + 1;
            BoatLog(BOAT_LOG_NORMAL, " offset = %d .", offset);
        }
    }

    return ret;
}

BCHAR *UtilityNative2PKCS(KeypairNative keypair)
{
    BUINT8 oid_secp256k1[] = {0x2b, 0x81, 0x04, 0x00, 0x0a};
    BUINT8 oid_secp256r1[] = {0x2b, 0x81, 0x04, 0x03, 0x01, 0x07};
    BUINT8 oid_sm2sm3[] = {0x2a, 0x81, 0x1c, 0xcf, 0x55, 0x01, 0x82, 0x2d};
    BUINT8 version[] = {0x02, 0x01, 0x01};
    BUINT8 *dataHex = NULL;
    BCHAR *outStr = NULL;
    BUINT8 object_sn = 0;
    BUINT32 len = 0, len_level_2 = 0, offset = 0, offset_bat = 0;
    len += 3;                                                                    // version
    len += UtilityGetTLV_LL_from_len(keypair.prikeylen) + 1 + keypair.prikeylen; // prikey
    if (keypair.alg == KEYPAIT_ALG_SECP256K1)
    {
        len_level_2 = UtilityGetTLV_LL_from_len(sizeof(oid_secp256k1)) + 1 + sizeof(oid_secp256k1);
    }
    else if (keypair.alg == KEYPAIT_ALG_SECP256R1)
    {
        len_level_2 = UtilityGetTLV_LL_from_len(sizeof(oid_secp256r1)) + 1 + sizeof(oid_secp256r1);
    }
    else if (keypair.alg == KEYPAIT_ALG_SM)
    {
        len_level_2 = UtilityGetTLV_LL_from_len(sizeof(oid_sm2sm3)) + 1 + sizeof(oid_sm2sm3);
    }
    else
    {
        return NULL;
    }
    len += UtilityGetTLV_LL_from_len(len_level_2) + 1 + len_level_2; // oid
    len_level_2 = UtilityGetTLV_LL_from_len(keypair.pubkeylen) + 1 + keypair.pubkeylen;
    len += UtilityGetTLV_LL_from_len(len_level_2) + 1 + len_level_2; // pubkey
    len += UtilityGetTLV_LL_from_len(len) + 1 + len;                 // all hex data
    dataHex = BoatMalloc(len);
    if(NULL == dataHex){
        return NULL;
    }
    memcpy(dataHex + offset,version,sizeof(version));
    offset += sizeof(version);
    memcpy(dataHex + offset, keypair.prikey, keypair.prikeylen);
    add_TL_withOffset(BoAT_ASN1_OCTET_STRING, dataHex, &offset, keypair.prikeylen); // prikey
    offset_bat = offset;
    if (keypair.alg == KEYPAIT_ALG_SECP256K1)
    {
        memcpy(dataHex + offset, oid_secp256k1, sizeof(oid_secp256k1));
        len = add_TL_withOffset(BoAT_ASN1_OID, dataHex, &offset, sizeof(oid_secp256k1)); // oid
    }
    else if (keypair.alg == KEYPAIT_ALG_SECP256R1)
    {
        memcpy(dataHex + offset, oid_secp256r1, sizeof(oid_secp256r1));
        len = add_TL_withOffset(BoAT_ASN1_OID, dataHex, &offset, sizeof(oid_secp256r1)); // oid
    }
    else if (keypair.alg == KEYPAIT_ALG_SM)
    {
        memcpy(dataHex + offset, oid_sm2sm3, sizeof(oid_sm2sm3));
        len = add_TL_withOffset(BoAT_ASN1_OID, dataHex, &offset, sizeof(oid_sm2sm3)); // oid
    }
    add_TL_withOffset((BoAT_ASN1_CONSTRUCTED | BoAT_ASN1_CONTEXT_SPECIFIC) + object_sn, dataHex, &offset_bat, len); // oid
    object_sn++;
    offset = offset_bat;
    dataHex[offset] = 0x04;
    memcpy(dataHex + offset + 1, keypair.pubkey, keypair.pubkeylen);
    len = add_TL_withOffset(BoAT_ASN1_BIT_STRING, dataHex, &offset, keypair.pubkeylen + 1);                         // pubkey
    add_TL_withOffset((BoAT_ASN1_CONSTRUCTED | BoAT_ASN1_CONTEXT_SPECIFIC) + object_sn, dataHex, &offset_bat, len); // pubkey
    len = offset_bat;
    offset = 0;
    add_TL_withOffset((BoAT_ASN1_CONSTRUCTED | BoAT_ASN1_SEQUENCE), dataHex, &offset, len); // all
    outStr = BoatMalloc(offset + strlen(PRIKEY_PKCS_BEGIN) + strlen(PRIKEY_PKCS_END) + 7);
    memset(outStr, 0, offset + strlen(PRIKEY_PKCS_BEGIN) + strlen(PRIKEY_PKCS_END) + 7);
    len = BoAT_base64_encode(dataHex, offset, outStr + strlen(PRIKEY_PKCS_BEGIN) + 2);
    memcpy(outStr, PRIKEY_PKCS_BEGIN, strlen(PRIKEY_PKCS_BEGIN));
    offset = strlen(PRIKEY_PKCS_BEGIN);
    outStr[offset++] = 0x0D;
    outStr[offset++] = 0x0A;
    offset += len;
    outStr[offset++] = 0x0D;
    outStr[offset++] = 0x0A;
    memcpy(outStr + offset, PRIKEY_PKCS_END, strlen(PRIKEY_PKCS_BEGIN));
    outStr[offset++] = 0x0D;
    outStr[offset++] = 0x0A;
    outStr[offset++] = 0x00;
    BoatFree(dataHex);
    return outStr;
}

/**
 * @description:
 *  This function convert r,s to asn.1 . length of signature must be 64 and r's length is 32,s's length is 32.
 * @param {BUINT8} *signature
 *  data of signature
 * @param {BUINT32} signaturelen
 *  length of signature ,must be 64
 * @param {BUINT8} *out
 *  asn.1 data out
 * @param {BUINT32} *outlen
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT utility_signature_to_asn1(BUINT8 *signature, BUINT32 signaturelen, BUINT8 *out, BUINT32 *outlen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 offset = 0;
    if (signature == NULL || out == NULL || outlen == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (signaturelen != 64)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    out[offset++] = 0x30;
    if ((signature[0] & 0x80) == 0x80)
    {
        out[offset++] = 0x45;
        out[offset++] = 0x02;
        out[offset++] = 0x21;
        out[offset++] = 0x00;
    }
    else
    {
        out[offset++] = 0x44;
        out[offset++] = 0x02;
        out[offset++] = 0x20;
    }
    memcpy(out + offset, signature, 32);
    offset += 32;
    out[offset++] = 0x02;
    out[offset++] = 0x20;
    memcpy(out + offset, signature + 32, 32);
    offset += 32;
    *outlen = offset;
    return result;
}

/**
 * @description:
 *  This function get  num of keypair / network / prikey object from 4 bytes.
 *  the format of 4 bytes data like this:
 *  0xAA 0x55 num crc
 *  crc is the XOR result of the first 3 bytes of data
 * @param[in] {BUINT8} temp
 *  the 4 bytes data
 * @param[out] {BUINT8} *num
 *  quantity of keypair/network/prikey objects
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT utility_check_NumBytes(BUINT8 temp[4], BUINT8 *num)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 crc = 0;
    if (temp[0] != 0xAA || temp[1] != 0x55)
    {
        return BOAT_ERROR;
    }
    crc = temp[0] ^ temp[1];
    crc = crc ^ temp[2];
    if (crc != temp[3])
    {
        return BOAT_ERROR;
    }
    *num = temp[2];
    return result;
}

/**
 * @description:
 *  This function get  4 bytes data of  keypair / network / prikey object by num.
 *  the format of 4 bytes data like this:
 *  0xAA 0x55 num crc
 *  crc is the XOR result of the first 3 bytes of data
 * @param {BUINT8} num
 * @param {BUINT8} temp
 * @return {*}
 *  This function have no return.
 * @author: aitos
 */
void utility_get_NumBytes(BUINT8 num, BUINT8 temp[4])
{
    BUINT8 crc = 0;
    temp[0] = 0xAA;
    temp[1] = 0x55;
    temp[2] = num;
    crc = temp[0] ^ temp[1];
    crc = crc ^ temp[2];
    temp[3] = crc;
    return;
}
