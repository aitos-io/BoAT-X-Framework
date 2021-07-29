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

/*!@brief bech32 encode functions

@file
boatbech32.c contains utility functions for boatwallet.
*/

#include "boatinternal.h"
#include "boatlog.h"

#if PROTOCOL_USE_PLATON == 1
#include "boatplaton.h"

const char *BECH32ALPHABET = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

/*static BSINT8 CHARSET_REV = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                             15, -1, 10, 17, 21, 20, 26, 30,  7,  5, -1, -1, -1, -1, -1, -1,
                             -1, 29, -1, 24, 13, 25,  9,  8, 23, -1, 18, 22, 31, 27, 19, -1,
                              1,  0,  3, 16, 11, 28, 12, 14,  6,  4,  2, -1, -1, -1, -1, -1,
                             -1, 29, -1, 24, 13, 25,  9,  8, 23, -1, 18, 22, 31, 27, 19, -1,
                              1,  0,  3, 16, 11, 28, 12, 14,  6,  4,  2, -1, -1, -1, -1, -1};*/

BUINT32 convertBits(const BUINT8 *in, BUINT32 inlen, BUINT8 *out, BUINT32 fromBits, BUINT32 toBits)  
{
    BUINT32 acc = 0;
    BUINT32 bits = 0;
    BUINT32 i;
    BUINT8 value;
    BUINT32 outLen = 0;
    BUINT32 maxv = (1 << toBits) - 1; //0xF
    BUINT32 max_acc = (1 << (fromBits + toBits - 1)) - 1;//0x7FF
    for (i = 0; i < inlen; i++) 
    {
        value = *(in + i) & 0xff;
        if ((value >> fromBits) != 0) 
        {
            //throw new RuntimeException(
            //        String.format("Input value '%X' exceeds '%d' bit size", value, fromBits));
        }
        acc = ((acc << fromBits) | value) & max_acc;
        bits += fromBits;
        while (bits >= toBits)
        {
            bits -= toBits;
            *(out + outLen++) = (acc >> bits) & maxv;
        }
    }

    if (bits > 0)
    {
        *(out + outLen++) = (acc << (toBits - bits)) & maxv;
    }
    return outLen;
}

BUINT8 *expandHrp(const BUINT8 *hrp, BUINT8 hrplen, BUINT8 *out) 
{
    BUINT8 i;
    BUINT8 c;
    
    if (hrp == NULL)
    {
        return NULL;
    }

    out = BoatMalloc(hrplen * 2 + 1);
    if (out == NULL)
    {
        return NULL;
    }

    for (i = 0; i < hrplen; i++) 
    {
        c = *(hrp + i) & 0x7f; // Limit to standard 7-bit ASCII
        *(out + i) = (BUINT8)((c > 5) & 0x07);
        *(out + i + hrplen + 1) = (BUINT8)(c & 0x1f);
    }
    return out;
}

BUINT8 *Bech32Polymod(const BUINT8 *hrp, BUINT8 hrplen, const BUINT8 *data, BUINT32 datalen)
{
    BUINT32 chk = 1;
    BUINT32 b,i,j;
    BUINT8 *out = BoatMalloc(6);
    BUINT8 *p;

    memset(out, 0, 6);

    for (i = 0; i < 3; i++)
    {
        if (i == 0) 
        {
            p = hrp;
        }
        else if (i == 1)
        {
            p = data;
        }
        else if (i == 2)
        {
            p = out;
        }

        for (j = 0; j < hrplen; j++) 
        {
            b = chk >> 25;
            chk = ((chk & 0x1ffffff) << 5) ^ (*(p + j));
            if ((b &  1) != 0) chk ^= 0x3b6a57b2;
            if ((b &  2) != 0) chk ^= 0x26508e6d;
            if ((b &  4) != 0) chk ^= 0x1ea119fa;
            if ((b &  8) != 0) chk ^= 0x3d4233dd;
            if ((b & 16) != 0) chk ^= 0x2a1462b3;
        }
    }
    
    chk = chk ^ 1;
    for (i = 0; i < 6; i++) 
    {
         *(out + i) = (BUINT8)((chk >> (5 * (5 - i))) & 31);
    }
    return out;
}

BSINT32 BoatBech32Encode(const BUINT8 *in, BUINT32 inlen, BUINT8 *out, const BUINT8 *hrp, BUINT8 hrplen)
{
    if (in == NULL || hrp == NULL || out == NULL)
    {
        return -1;
    }
    if (hrplen == 0 || hrplen > 83)
    {
        return -1;
    }

    BUINT8 *base32Data;
    BUINT8 *expandHRPData;
    BUINT32 base32OutLen = base32_encoded_length(inlen);
    BUINT8 *bech32Chk;
    BUINT32 i;

    printf("The outlen is %d\n", base32OutLen);

    base32Data = BoatMalloc(base32OutLen);
    if (base32Data == NULL)
    {
        return -1;
    }

    convertBits(in, inlen, base32Data, 8, 5);

    bech32Chk = Bech32Polymod(expandHrp(hrp, hrplen, expandHRPData), hrplen * 2 + 1, base32Data, base32OutLen);

    //out = BoatMalloc(hrplen + 1 + base32OutLen + 6 + 1);
    memcpy(out, hrp, hrplen);
    *(out + hrplen) = '1';

    for (i = 0; i < base32OutLen; i++)
    {
        *(out + hrplen + 1 + i) = *(BECH32ALPHABET + *(base32Data + i));
    }
    for (i = 0; i < 6; i++)
    {
        *(out + hrplen + 1 + base32OutLen + i) = *(BECH32ALPHABET + *(bech32Chk + i));
    }
    *(out + hrplen + 1 + base32OutLen + 6) = 0;

    BoatFree(expandHRPData);
    BoatFree(base32Data);
    BoatFree(bech32Chk);
    
    return hrplen + 1 + base32OutLen + 6;
}

#endif /* end of PROTOCOL_USE_PLATON */
