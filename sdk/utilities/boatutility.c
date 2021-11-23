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

//!@brief Literal representation of log level
const BCHAR * const g_log_level_name_str[] = 
{
    "LOG_CRITICAL",
    "LOG_NORMAL",
    "LOG_VERBOSE"
};


BUINT32 UtilityTrimBin (BOAT_OUT BUINT8 *to_ptr,
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
    else  // if (trim_mode == TRIMBIN_LEFTTRIM)
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
        to_str[to_offset++] = '0';  // "0x"
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
            halfbyte = (octet>>(4-j*4))&0x0F;

            // Trim all leading zeroes, i.e. {0x00, 0x01, 0x00 0xAB} => "100AB"
            if (trim_done == BOAT_FALSE
                && trim_mode == BIN2HEX_LEFTTRIM_QUANTITY
                && halfbyte == 0)
            {
                continue;
            }
            
            if (halfbyte >= 0x0 && halfbyte <= 0x9)
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
        (prefix_0x_mode == BIN2HEX_PREFIX_0x_NO  && to_offset == 0)) // HEX contains nothing
    {
        if (zero_as_null == BOAT_FALSE)
        {
            if (trim_mode == BIN2HEX_LEFTTRIM_QUANTITY)
            {
                to_str[to_offset++] = '0';  // "0"
            }
            else 
            {
                to_str[to_offset++] = '0';  // "00"
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
        if (from_str[0] == '0' 
            && (from_str[1] == 'x' || from_str[1] == 'X'))
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
        if ((from_offset&0x01) == odd_flag)
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
                continue;  // Trim leading zeros.
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
    
    for (i = sizeof(from_host_integer)-1; i>=0; i--)
    {
        octet = (BUINT8)(from_host_integer >> i*8);
        
        if (bool_trim_done == BOAT_FALSE)
        {
            if (octet == 0x00)
            {
                continue;  // Trim MSB zeros.
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

    for (i = sizeof(from_host_integer)-1; i>=0; i--)
    {
        octet = (BUINT8)(from_host_integer >> i*8);
        
        if (bool_trim_done == BOAT_FALSE)
        {
            if (octet == 0x00)
            {
                continue;  // Trim MSB zeros.
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
        for (i = 0; i < value_len/2; i++)
        {
            octet = ((BUINT8*)value_ptr)[i];
            ((BUINT8*)value_ptr)[i] = ((BUINT8*)value_ptr)[value_len-1-i];
            ((BUINT8*)value_ptr)[value_len-1-i] = octet;
        }
    }

    return value_ptr;    
}


BUINT32 UtilityHtonl (BUINT32 from_host_integer)
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
        *((BUINT8 *)&to_host_integer + i) |= (from_big_integer>>((3-i)*8))&0xFF;
    }
    return to_host_integer;
}


double UtilityWeiStrToEthDouble(const BCHAR *wei_str)
{
    BUINT32 wei_bin_len;
    BUINT8  wei_bin[(strlen(wei_str)+1)/2];
    BUINT64 wei_int64;
    BUINT8  *wei_int64_addr_ptr;
    BUINT32 shift_bits;
    double  scale_double;
    double  wei_double;
    double  ether_double;
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
    wei_int64_addr_ptr = (BUINT8*)&wei_int64;

    if (wei_bin_len > sizeof(BUINT64))
    {
        // If wei_bin is more than 8 bytes, copy 8 bytes only
        for (i = 0; i< sizeof(BUINT64); i++)
        {
            // bigendian to littleendian
            wei_int64_addr_ptr[i] =  wei_bin[sizeof(BUINT64) - 1 - i];
        }
        
        shift_bits = (wei_bin_len - sizeof(BUINT64))*8;
    }
    else
    {
        // If wei_bin is no more than 8 bytes, copy effective bytes
        for (i = 0; i< wei_bin_len; i++)
        {
            // bigendian to littleendian
            wei_int64_addr_ptr[i] =  wei_bin[wei_bin_len - 1 - i];
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
    ether_double = wei_double *scale_double / 1e18;

    BoatLog(BOAT_LOG_VERBOSE, "%s wei converted to %f ether", wei_str, ether_double);
    
    return ether_double;
}


void UtilityReadBigendToBignum(const BUINT8 *in_number, utilityBignum256 *out_number)
{
	BUINT32 temp = 0;
	
	for (int i = 0; i < 8; i++) {
		// invariant: temp = (in_number % 2^(32i)) >> 30i
		// get next limb = (in_number % 2^(32(i+1))) >> 32i
		BUINT32 limb = (*(in_number + (7 - i) * 4 + 0)) << 24 |
					   (*(in_number + (7 - i) * 4 + 1)) << 16 |
					   (*(in_number + (7 - i) * 4 + 2)) <<  8 |
					   (*(in_number + (7 - i) * 4 + 3)) <<  0;
		// temp = (in_number % 2^(32(i+1))) << 30i
		temp |= limb << (2*i);
		// store 30 bits into val[i]
		out_number->val[i]= temp & 0x3FFFFFFF;
		// prepare temp for next round
		temp = limb >> (30 - 2*i);
	}
	
	out_number->val[8]= temp;
}


void UtilityWriteBignumToBigend(const utilityBignum256 *in_number, BUINT8 *out_number)
{
	BUINT32 temp = in_number->val[8];
	
	for (int i = 0; i < 8; i++) {
		// invariant: temp = (in_number >> 30*(8-i))
		BUINT32 limb = in_number->val[7 - i];
		temp = (temp << (16 + 2*i)) | (limb >> (14 - 2*i));
		*(out_number + (i) * 4 + 0) = temp >> 24;
		*(out_number + (i) * 4 + 1) = temp >> 16;
		*(out_number + (i) * 4 + 2) = temp >>  8;
		*(out_number + (i) * 4 + 3) = temp >>  0;
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

BUINT64 UtilityBuint8Buf2Uint64(BUINT8 *from,BUINT32 len)
{
    long ret ;
    ret =  (((long)(from[0]&0x7F) << 56) | ((long)from[1] << 48) | ((long)from[2] << 40) | ((long)from[3] << 32) | ((long)from[4] << 24) | ((long)from[5] << 16) | ((long)from[6] << 8) | from[7]);
    // ret = random32() << 32 | random32();
        // ret =  (((long)from[4] << 24) | ((long)from[5] << 16) | ((long)from[6] << 8) | from[7]);
    return ret;
}
#if (BOAT_HWBCS_TLS_SUPPORT == 1)
#include "mbedtls/x509_crt.h"
#include "mbedtls/oid.h"
size_t Utility_find_oid_value_in_name(const mbedtls_x509_name *name, const char* target_short_name, char *value, size_t value_length)
{
    const char *short_name = NULL;
    bool found = false;
    size_t retval = 0;

    while((name != NULL) && !found)
    {
        // if there is no data for this name go to the next one
        if(!name->oid.p)
        {
            name = name->next;
            continue;
        }

        int ret = mbedtls_oid_get_attr_short_name(&name->oid, &short_name);
        if((ret == 0) && (strcmp(short_name, target_short_name) == 0))
        {
            found = true;
        }

        if(found)
        {
            size_t bytes_to_write = (name->val.len >= value_length) ? value_length - 1 : name->val.len;

            for(size_t i = 0; i < bytes_to_write; i++)
            {
                char c = name->val.p[i];
                if (c < 32 || c == 127 || (c > 128 && c < 160))
                {
                    value[i] = '?';
                } else
                {
                    value[i] = c;
                }
            }

            // null terminate
            value[bytes_to_write] = 0;

            retval = name->val.len;
        }

        name = name->next;
    }

    return retval;
}
#endif



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

