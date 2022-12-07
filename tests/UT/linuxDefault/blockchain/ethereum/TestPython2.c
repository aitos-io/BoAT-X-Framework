/******************************************************************************
This file is generated from contract ABI. DO NOT modify it by hand.
******************************************************************************/


// Generated C function interface from smart contract ABI

#include "boatconfig.h"
#include "boatiotsdk.h"


// Types specially defined for mapping from Solidity

    typedef BUINT8 Bbytes1[1];
    typedef BUINT8 Bbytes2[2];
    typedef BUINT8 Bbytes3[3];
    typedef BUINT8 Bbytes4[4];
    typedef BUINT8 Bbytes5[5];
    typedef BUINT8 Bbytes6[6];
    typedef BUINT8 Bbytes7[7];
    typedef BUINT8 Bbytes8[8];

    typedef BUINT8 Bbytes9[9];
    typedef BUINT8 Bbytes10[10];
    typedef BUINT8 Bbytes11[11];
    typedef BUINT8 Bbytes12[12];
    typedef BUINT8 Bbytes13[13];
    typedef BUINT8 Bbytes14[14];
    typedef BUINT8 Bbytes15[15];
    typedef BUINT8 Bbytes16[16];

    typedef BUINT8 Bbytes17[17];
    typedef BUINT8 Bbytes18[18];
    typedef BUINT8 Bbytes19[19];
    typedef BUINT8 Bbytes20[20];
    typedef BUINT8 Bbytes21[21];
    typedef BUINT8 Bbytes22[22];
    typedef BUINT8 Bbytes23[23];
    typedef BUINT8 Bbytes24[24];

    typedef BUINT8 Bbytes25[25];
    typedef BUINT8 Bbytes26[26];
    typedef BUINT8 Bbytes27[27];
    typedef BUINT8 Bbytes28[28];
    typedef BUINT8 Bbytes29[29];
    typedef BUINT8 Bbytes30[30];
    typedef BUINT8 Bbytes31[31];
    typedef BUINT8 Bbytes32[32];

    typedef Bbytes16 BUINT128;
    typedef Bbytes16 BSINT128;
    typedef Bbytes32 BUINT256;
    typedef Bbytes32 BSINT256;

BCHAR *TestPython2_organizer(BoatEthTx *tx_ptr)
{
    BCHAR *call_result_str = NULL;
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if (tx_ptr == NULL)
    {
        return NULL;
    }

    data_field.field_len = (0 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "organizer()";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    call_result_str = BoatEthCallContractFunc(tx_ptr, function_prototye_str, data_field.field_ptr+4, data_field.field_len-4);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(call_result_str);

}

BCHAR *TestPython2_testOneFixedSize(BoatEthTx *tx_ptr, Bbytes7 b7)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (b7 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (1 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testOneFixedSize(bytes7)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr, b7, sizeof(Bbytes7));
    data_offset_ptr += 32;
    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);

    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);
}

BCHAR *TestPython2_testTwoFixedSize(BoatEthTx *tx_ptr, BSINT8 i8, BoatAddress ad1)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (ad1 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (2 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoFixedSize(int8,address)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    if (i8 >= 0)
    {
        memset(data_offset_ptr, 0x00, 32);
    }
    else
    {
        memset(data_offset_ptr, 0xFF, 32);
    }
    memcpy(data_offset_ptr + 32 - sizeof(BSINT8), &i8, sizeof(BSINT8));
    data_offset_ptr += 32;
    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(ad1, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), ad1, sizeof(BoatAddress));
    data_offset_ptr += 32;
    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testThreeFixedSize(BoatEthTx *tx_ptr, BUINT8 b, BoatAddress ad2, Bbytes20 b20)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (ad2 == NULL)|| (b20 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (3 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testThreeFixedSize(bool,address,bytes20)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT8), &b, sizeof(BUINT8));
    data_offset_ptr += 32;
    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(ad2, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), ad2, sizeof(BoatAddress));
    data_offset_ptr += 32;
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr, b20, sizeof(Bbytes20));
    data_offset_ptr += 32;
    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testOneFixedSizeNonFixedArray(BoatEthTx *tx_ptr, BSINT128 *i128a, BUINT32 i128aLen)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 i128a_data_length = 0;
    BUINT32 i128a_offset_location[i128aLen];
    BUINT32 i128a_filled_length[i128aLen];
    BUINT32 i128a_actual_length[i128aLen];
    BSINT128 *i128a_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (i128a == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (1 * 32 + 4);
    i128a_data_length += i128aLen * 32 + 32;
    data_field.field_len += i128a_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testOneFixedSizeNonFixedArray(int128[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'i128a' offset location filled
    data_offset_location = 32;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'i128a' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = i128aLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled fixed size non-fixed array data
    i128a_ptr = i128a;
    for(i = 0; i < i128aLen; i++)
    {
        memcpy(fixedsize_bytes32, i128a_ptr, sizeof(BSINT128));
        if (*i128a_ptr >= 0)
        {
            memset(data_offset_ptr, 0x00, 32);
        }
        else
        {
            memset(data_offset_ptr, 0xFF, 32);
        }
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BSINT128));
        memcpy(data_offset_ptr + 32 - sizeof(BSINT128), fixedsize_bytes32, sizeof(BSINT128));
        data_offset_ptr += 32;
        i128a_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testOneFixedSizeFixedArray(BoatEthTx *tx_ptr, BoatAddress aa[2])
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (aa == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (2 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testOneFixedSizeFixedArray(address[2])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'aa' data filled
    for(i = 0; i < 2; i++)
    {
        memcpy(fixedsize_bytes32, aa[i], sizeof(BoatAddress));
        memset(data_offset_ptr, 0x00, 32);
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BoatAddress));
        memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), fixedsize_bytes32, sizeof(BoatAddress));
        data_offset_ptr += 32;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testTwoFixedSizeArrays(BoatEthTx *tx_ptr, Bbytes5 *b5a, BUINT32 b5aLen, BUINT16 uia[3])
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 b5a_data_length = 0;
    BUINT32 b5a_offset_location[b5aLen];
    BUINT32 b5a_filled_length[b5aLen];
    BUINT32 b5a_actual_length[b5aLen];
    Bbytes5 *b5a_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (b5a == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (4 * 32 + 4);
    b5a_data_length += b5aLen * 32 + 32;
    data_field.field_len += b5a_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoFixedSizeArrays(bytes5[],uint16[3])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'b5a' offset location filled
    data_offset_location = 128;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'uia' data filled
    for(i = 0; i < 3; i++)
    {
        memcpy(fixedsize_bytes32, &uia[i], sizeof(BUINT16));
        memset(data_offset_ptr, 0x00, 32);
        UtilityChangeEndian(&fixedsize_bytes32, sizeof(BUINT16));
        memcpy(data_offset_ptr + 32 - sizeof(BUINT16), &fixedsize_bytes32, sizeof(BUINT16));
        data_offset_ptr += 32;
    }

    //non-fixed param 'b5a' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = b5aLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled fixed size non-fixed array data
    b5a_ptr = b5a;
    for(i = 0; i < b5aLen; i++)
    {
        memcpy(fixedsize_bytes32, b5a_ptr, sizeof(Bbytes5));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr, fixedsize_bytes32, sizeof(Bbytes5));
        data_offset_ptr += 32;
        b5a_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testTwoFixedSizeArraysAndOneNonFixedSizeFixedArray(BoatEthTx *tx_ptr, Bbytes29 *b29a, BUINT32 b29aLen, Bbytes13 b13a[3], BoatFieldVariable *sa, Bbytes7 b7)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 b29a_data_length = 0;
    BUINT32 b29a_offset_location[b29aLen];
    BUINT32 b29a_filled_length[b29aLen];
    BUINT32 b29a_actual_length[b29aLen];
    Bbytes29 *b29a_ptr;
    BUINT32 saLen = 5;
    BUINT32 sa_data_length = 0;
    BUINT32 sa_offset_location[saLen];
    BUINT32 sa_filled_length[saLen];
    BUINT32 sa_actual_length[saLen];
    BoatFieldVariable *sa_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (b29a == NULL)|| (b13a == NULL)|| (sa == NULL)|| (b7 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (6 * 32 + 4);
    b29a_data_length += b29aLen * 32 + 32;
    data_field.field_len += b29a_data_length;

    sa_ptr = sa;
    for(i = 0; i < saLen; i++)
    {
        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32) + 32;
        sa_data_length += sa_filled_length[i];
        sa_ptr++;
    }
    sa_data_length += saLen * 32;
    data_field.field_len += sa_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoFixedSizeArraysAndOneNonFixedSizeFixedArray(bytes29[],bytes13[3],string[5],bytes7)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'b29a' offset location filled
    data_offset_location = 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'b13a' data filled
    for(i = 0; i < 3; i++)
    {
        memcpy(fixedsize_bytes32, b13a[i], sizeof(Bbytes13));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr, fixedsize_bytes32, sizeof(Bbytes13));
        data_offset_ptr += 32;
    }

    //param 'sa' offset location filled
    data_offset_location = b29a_data_length + \
                           + 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr, b7, sizeof(Bbytes7));
    data_offset_ptr += 32;
    //non-fixed param 'b29a' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = b29aLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled fixed size non-fixed array data
    b29a_ptr = b29a;
    for(i = 0; i < b29aLen; i++)
    {
        memcpy(fixedsize_bytes32, b29a_ptr, sizeof(Bbytes29));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr, fixedsize_bytes32, sizeof(Bbytes29));
        data_offset_ptr += 32;
        b29a_ptr++;
    }

    //non-fixed param 'sa' data filled
    //filled non-fixed size array data offset
    for (i = 0; i < saLen; i++)
    {
        if (i == 0) sa_offset_location[i] = saLen * 32;
        else sa_offset_location[i] = sa_offset_location[i - 1] + sa_filled_length[i - 1];
        data_offset_location = sa_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa_ptr = sa;
    for (i = 0; i < saLen; i++)
    {
        sa_actual_length[i] = sa_ptr->field_len;
        UtilityChangeEndian(&sa_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa_filled_length[i]);
        memcpy(data_offset_ptr, sa_ptr->field_ptr, sa_ptr->field_len);
        data_offset_ptr += sa_filled_length[i];
        sa_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testOneStringArray(BoatEthTx *tx_ptr, BoatFieldVariable*sa, BUINT32 saLen)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 sa_data_length = 0;
    BUINT32 sa_offset_location[saLen];
    BUINT32 sa_filled_length[saLen];
    BUINT32 sa_actual_length[saLen];
    BoatFieldVariable *sa_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (sa == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (1 * 32 + 4);
    sa_ptr = sa;
    for(i = 0; i < saLen; i++)
    {
        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32) + 32;
        sa_data_length += sa_filled_length[i];
        sa_ptr++;
    }
    sa_data_length += saLen * 32 + 32;
    data_field.field_len += sa_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testOneStringArray(string[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'sa' offset location filled
    data_offset_location = 32;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'sa' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = saLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < saLen; i++)
    {
        if (i == 0) sa_offset_location[i] = saLen * 32;
        else sa_offset_location[i] = sa_offset_location[i - 1] + sa_filled_length[i - 1];
        data_offset_location = sa_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa_ptr = sa;
    for (i = 0; i < saLen; i++)
    {
        sa_actual_length[i] = sa_ptr->field_len;
        UtilityChangeEndian(&sa_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa_filled_length[i]);
        memcpy(data_offset_ptr, sa_ptr->field_ptr, sa_ptr->field_len);
        data_offset_ptr += sa_filled_length[i];
        sa_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testTwoStringArrays(BoatEthTx *tx_ptr, BoatFieldVariable*sa1, BUINT32 sa1Len, BoatFieldVariable*sa2, BUINT32 sa2Len)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 sa1_data_length = 0;
    BUINT32 sa1_offset_location[sa1Len];
    BUINT32 sa1_filled_length[sa1Len];
    BUINT32 sa1_actual_length[sa1Len];
    BoatFieldVariable *sa1_ptr;
    BUINT32 sa2_data_length = 0;
    BUINT32 sa2_offset_location[sa2Len];
    BUINT32 sa2_filled_length[sa2Len];
    BUINT32 sa2_actual_length[sa2Len];
    BoatFieldVariable *sa2_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (sa1 == NULL)|| (sa2 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (2 * 32 + 4);
    sa1_ptr = sa1;
    for(i = 0; i < sa1Len; i++)
    {
        sa1_filled_length[i] = BOAT_ROUNDUP(sa1_ptr->field_len, 32) + 32;
        sa1_data_length += sa1_filled_length[i];
        sa1_ptr++;
    }
    sa1_data_length += sa1Len * 32 + 32;
    data_field.field_len += sa1_data_length;

    sa2_ptr = sa2;
    for(i = 0; i < sa2Len; i++)
    {
        sa2_filled_length[i] = BOAT_ROUNDUP(sa2_ptr->field_len, 32) + 32;
        sa2_data_length += sa2_filled_length[i];
        sa2_ptr++;
    }
    sa2_data_length += sa2Len * 32 + 32;
    data_field.field_len += sa2_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoStringArrays(string[],string[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'sa1' offset location filled
    data_offset_location = 64;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'sa2' offset location filled
    data_offset_location = sa1_data_length + \
                           + 64;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'sa1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = sa1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < sa1Len; i++)
    {
        if (i == 0) sa1_offset_location[i] = sa1Len * 32;
        else sa1_offset_location[i] = sa1_offset_location[i - 1] + sa1_filled_length[i - 1];
        data_offset_location = sa1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa1_ptr = sa1;
    for (i = 0; i < sa1Len; i++)
    {
        sa1_actual_length[i] = sa1_ptr->field_len;
        UtilityChangeEndian(&sa1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa1_filled_length[i] = BOAT_ROUNDUP(sa1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa1_filled_length[i]);
        memcpy(data_offset_ptr, sa1_ptr->field_ptr, sa1_ptr->field_len);
        data_offset_ptr += sa1_filled_length[i];
        sa1_ptr++;
    }

    //non-fixed param 'sa2' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = sa2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < sa2Len; i++)
    {
        if (i == 0) sa2_offset_location[i] = sa2Len * 32;
        else sa2_offset_location[i] = sa2_offset_location[i - 1] + sa2_filled_length[i - 1];
        data_offset_location = sa2_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa2_ptr = sa2;
    for (i = 0; i < sa2Len; i++)
    {
        sa2_actual_length[i] = sa2_ptr->field_len;
        UtilityChangeEndian(&sa2_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa2_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa2_filled_length[i] = BOAT_ROUNDUP(sa2_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa2_filled_length[i]);
        memcpy(data_offset_ptr, sa2_ptr->field_ptr, sa2_ptr->field_len);
        data_offset_ptr += sa2_filled_length[i];
        sa2_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testTwoTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 sa_data_length = 0;
    BUINT32 sa_offset_location[saLen];
    BUINT32 sa_filled_length[saLen];
    BUINT32 sa_actual_length[saLen];
    BoatFieldVariable *sa_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (sa == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (2 * 32 + 4);
    sa_ptr = sa;
    for(i = 0; i < saLen; i++)
    {
        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32) + 32;
        sa_data_length += sa_filled_length[i];
        sa_ptr++;
    }
    sa_data_length += saLen * 32 + 32;
    data_field.field_len += sa_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoTypes(uint16,string[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(&ui16, sizeof(BUINT16));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT16), &ui16, sizeof(BUINT16));
    data_offset_ptr += 32;
    //param 'sa' offset location filled
    data_offset_location = 64;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'sa' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = saLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < saLen; i++)
    {
        if (i == 0) sa_offset_location[i] = saLen * 32;
        else sa_offset_location[i] = sa_offset_location[i - 1] + sa_filled_length[i - 1];
        data_offset_location = sa_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa_ptr = sa;
    for (i = 0; i < saLen; i++)
    {
        sa_actual_length[i] = sa_ptr->field_len;
        UtilityChangeEndian(&sa_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa_filled_length[i]);
        memcpy(data_offset_ptr, sa_ptr->field_ptr, sa_ptr->field_len);
        data_offset_ptr += sa_filled_length[i];
        sa_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testThreeTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen, BoatFieldVariable *ba)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 sa_data_length = 0;
    BUINT32 sa_offset_location[saLen];
    BUINT32 sa_filled_length[saLen];
    BUINT32 sa_actual_length[saLen];
    BoatFieldVariable *sa_ptr;
    BUINT32 baLen = 4;
    BUINT32 ba_data_length = 0;
    BUINT32 ba_offset_location[baLen];
    BUINT32 ba_filled_length[baLen];
    BUINT32 ba_actual_length[baLen];
    BoatFieldVariable *ba_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (sa == NULL)|| (ba == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (3 * 32 + 4);
    sa_ptr = sa;
    for(i = 0; i < saLen; i++)
    {
        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32) + 32;
        sa_data_length += sa_filled_length[i];
        sa_ptr++;
    }
    sa_data_length += saLen * 32 + 32;
    data_field.field_len += sa_data_length;

    ba_ptr = ba;
    for(i = 0; i < baLen; i++)
    {
        ba_filled_length[i] = BOAT_ROUNDUP(ba_ptr->field_len, 32) + 32;
        ba_data_length += ba_filled_length[i];
        ba_ptr++;
    }
    ba_data_length += baLen * 32;
    data_field.field_len += ba_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testThreeTypes(uint16,string[],bytes[4])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(&ui16, sizeof(BUINT16));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT16), &ui16, sizeof(BUINT16));
    data_offset_ptr += 32;
    //param 'sa' offset location filled
    data_offset_location = 96;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba' offset location filled
    data_offset_location = sa_data_length + \
                           + 96;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'sa' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = saLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < saLen; i++)
    {
        if (i == 0) sa_offset_location[i] = saLen * 32;
        else sa_offset_location[i] = sa_offset_location[i - 1] + sa_filled_length[i - 1];
        data_offset_location = sa_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa_ptr = sa;
    for (i = 0; i < saLen; i++)
    {
        sa_actual_length[i] = sa_ptr->field_len;
        UtilityChangeEndian(&sa_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa_filled_length[i]);
        memcpy(data_offset_ptr, sa_ptr->field_ptr, sa_ptr->field_len);
        data_offset_ptr += sa_filled_length[i];
        sa_ptr++;
    }

    //non-fixed param 'ba' data filled
    //filled non-fixed size array data offset
    for (i = 0; i < baLen; i++)
    {
        if (i == 0) ba_offset_location[i] = baLen * 32;
        else ba_offset_location[i] = ba_offset_location[i - 1] + ba_filled_length[i - 1];
        data_offset_location = ba_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba_ptr = ba;
    for (i = 0; i < baLen; i++)
    {
        ba_actual_length[i] = ba_ptr->field_len;
        UtilityChangeEndian(&ba_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba_filled_length[i] = BOAT_ROUNDUP(ba_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba_filled_length[i]);
        memcpy(data_offset_ptr, ba_ptr->field_ptr, ba_ptr->field_len);
        data_offset_ptr += ba_filled_length[i];
        ba_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testFourTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen, BoatFieldVariable *ba, BCHAR*s)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 sa_data_length = 0;
    BUINT32 sa_offset_location[saLen];
    BUINT32 sa_filled_length[saLen];
    BUINT32 sa_actual_length[saLen];
    BoatFieldVariable *sa_ptr;
    BUINT32 baLen = 4;
    BUINT32 ba_data_length = 0;
    BUINT32 ba_offset_location[baLen];
    BUINT32 ba_filled_length[baLen];
    BUINT32 ba_actual_length[baLen];
    BoatFieldVariable *ba_ptr;
    BUINT32 sLen;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (sa == NULL)|| (ba == NULL)|| (s == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = BOAT_ROUNDUP(strlen(s), 32) + 32 + \
                           (4 * 32 + 4);
    sa_ptr = sa;
    for(i = 0; i < saLen; i++)
    {
        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32) + 32;
        sa_data_length += sa_filled_length[i];
        sa_ptr++;
    }
    sa_data_length += saLen * 32 + 32;
    data_field.field_len += sa_data_length;

    ba_ptr = ba;
    for(i = 0; i < baLen; i++)
    {
        ba_filled_length[i] = BOAT_ROUNDUP(ba_ptr->field_len, 32) + 32;
        ba_data_length += ba_filled_length[i];
        ba_ptr++;
    }
    ba_data_length += baLen * 32;
    data_field.field_len += ba_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testFourTypes(uint16,string[],bytes[4],string)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(&ui16, sizeof(BUINT16));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT16), &ui16, sizeof(BUINT16));
    data_offset_ptr += 32;
    //param 'sa' offset location filled
    data_offset_location = 128;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba' offset location filled
    data_offset_location = sa_data_length + \
                           + 128;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 's' offset location filled
    data_offset_location = sa_data_length + \
                           ba_data_length + \
                           + 128;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'sa' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = saLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < saLen; i++)
    {
        if (i == 0) sa_offset_location[i] = saLen * 32;
        else sa_offset_location[i] = sa_offset_location[i - 1] + sa_filled_length[i - 1];
        data_offset_location = sa_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa_ptr = sa;
    for (i = 0; i < saLen; i++)
    {
        sa_actual_length[i] = sa_ptr->field_len;
        UtilityChangeEndian(&sa_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa_filled_length[i]);
        memcpy(data_offset_ptr, sa_ptr->field_ptr, sa_ptr->field_len);
        data_offset_ptr += sa_filled_length[i];
        sa_ptr++;
    }

    //non-fixed param 'ba' data filled
    //filled non-fixed size array data offset
    for (i = 0; i < baLen; i++)
    {
        if (i == 0) ba_offset_location[i] = baLen * 32;
        else ba_offset_location[i] = ba_offset_location[i - 1] + ba_filled_length[i - 1];
        data_offset_location = ba_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba_ptr = ba;
    for (i = 0; i < baLen; i++)
    {
        ba_actual_length[i] = ba_ptr->field_len;
        UtilityChangeEndian(&ba_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba_filled_length[i] = BOAT_ROUNDUP(ba_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba_filled_length[i]);
        memcpy(data_offset_ptr, ba_ptr->field_ptr, ba_ptr->field_len);
        data_offset_ptr += ba_filled_length[i];
        ba_ptr++;
    }

    //non-fixed param 's' data filled
    //filled fixed size non-fixed array length
    sLen = strlen(s);
    nonFixed_actual_length = sLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s, sLen);
    data_offset_ptr += nonFixed_filled_length;

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testFiveTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen, BoatFieldVariable *ba, BCHAR*s, BSINT64 *i64a, BUINT32 i64aLen)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 sa_data_length = 0;
    BUINT32 sa_offset_location[saLen];
    BUINT32 sa_filled_length[saLen];
    BUINT32 sa_actual_length[saLen];
    BoatFieldVariable *sa_ptr;
    BUINT32 baLen = 4;
    BUINT32 ba_data_length = 0;
    BUINT32 ba_offset_location[baLen];
    BUINT32 ba_filled_length[baLen];
    BUINT32 ba_actual_length[baLen];
    BoatFieldVariable *ba_ptr;
    BUINT32 sLen;
    BUINT32 i64a_data_length = 0;
    BUINT32 i64a_offset_location[i64aLen];
    BUINT32 i64a_filled_length[i64aLen];
    BUINT32 i64a_actual_length[i64aLen];
    BSINT64 *i64a_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (sa == NULL)|| (ba == NULL)|| (s == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = BOAT_ROUNDUP(strlen(s), 32) + 32 + \
                           (5 * 32 + 4);
    sa_ptr = sa;
    for(i = 0; i < saLen; i++)
    {
        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32) + 32;
        sa_data_length += sa_filled_length[i];
        sa_ptr++;
    }
    sa_data_length += saLen * 32 + 32;
    data_field.field_len += sa_data_length;

    ba_ptr = ba;
    for(i = 0; i < baLen; i++)
    {
        ba_filled_length[i] = BOAT_ROUNDUP(ba_ptr->field_len, 32) + 32;
        ba_data_length += ba_filled_length[i];
        ba_ptr++;
    }
    ba_data_length += baLen * 32;
    data_field.field_len += ba_data_length;

    i64a_data_length += i64aLen * 32 + 32;
    data_field.field_len += i64a_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testFiveTypes(uint16,string[],bytes[4],string,int64[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(&ui16, sizeof(BUINT16));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT16), &ui16, sizeof(BUINT16));
    data_offset_ptr += 32;
    //param 'sa' offset location filled
    data_offset_location = 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba' offset location filled
    data_offset_location = sa_data_length + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 's' offset location filled
    data_offset_location = sa_data_length + \
                           ba_data_length + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'i64a' offset location filled
    data_offset_location = sa_data_length + \
                           ba_data_length + \
                           BOAT_ROUNDUP(strlen(s), 32) + 32 + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'sa' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = saLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < saLen; i++)
    {
        if (i == 0) sa_offset_location[i] = saLen * 32;
        else sa_offset_location[i] = sa_offset_location[i - 1] + sa_filled_length[i - 1];
        data_offset_location = sa_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa_ptr = sa;
    for (i = 0; i < saLen; i++)
    {
        sa_actual_length[i] = sa_ptr->field_len;
        UtilityChangeEndian(&sa_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa_filled_length[i]);
        memcpy(data_offset_ptr, sa_ptr->field_ptr, sa_ptr->field_len);
        data_offset_ptr += sa_filled_length[i];
        sa_ptr++;
    }

    //non-fixed param 'ba' data filled
    //filled non-fixed size array data offset
    for (i = 0; i < baLen; i++)
    {
        if (i == 0) ba_offset_location[i] = baLen * 32;
        else ba_offset_location[i] = ba_offset_location[i - 1] + ba_filled_length[i - 1];
        data_offset_location = ba_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba_ptr = ba;
    for (i = 0; i < baLen; i++)
    {
        ba_actual_length[i] = ba_ptr->field_len;
        UtilityChangeEndian(&ba_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba_filled_length[i] = BOAT_ROUNDUP(ba_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba_filled_length[i]);
        memcpy(data_offset_ptr, ba_ptr->field_ptr, ba_ptr->field_len);
        data_offset_ptr += ba_filled_length[i];
        ba_ptr++;
    }

    //non-fixed param 's' data filled
    //filled fixed size non-fixed array length
    sLen = strlen(s);
    nonFixed_actual_length = sLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s, sLen);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'i64a' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = i64aLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled fixed size non-fixed array data
    i64a_ptr = i64a;
    for(i = 0; i < i64aLen; i++)
    {
        memcpy(fixedsize_bytes32, i64a_ptr, sizeof(BSINT64));
        if (*i64a_ptr >= 0)
        {
            memset(data_offset_ptr, 0x00, 32);
        }
        else
        {
            memset(data_offset_ptr, 0xFF, 32);
        }
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BSINT64));
        memcpy(data_offset_ptr + 32 - sizeof(BSINT64), fixedsize_bytes32, sizeof(BSINT64));
        data_offset_ptr += 32;
        i64a_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testAllTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen, BoatFieldVariable *ba, BCHAR*s, BSINT64 *i64a, BUINT32 i64aLen, BSINT128 i128a[6])
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 sa_data_length = 0;
    BUINT32 sa_offset_location[saLen];
    BUINT32 sa_filled_length[saLen];
    BUINT32 sa_actual_length[saLen];
    BoatFieldVariable *sa_ptr;
    BUINT32 baLen = 4;
    BUINT32 ba_data_length = 0;
    BUINT32 ba_offset_location[baLen];
    BUINT32 ba_filled_length[baLen];
    BUINT32 ba_actual_length[baLen];
    BoatFieldVariable *ba_ptr;
    BUINT32 sLen;
    BUINT32 i64a_data_length = 0;
    BUINT32 i64a_offset_location[i64aLen];
    BUINT32 i64a_filled_length[i64aLen];
    BUINT32 i64a_actual_length[i64aLen];
    BSINT64 *i64a_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (sa == NULL)|| (ba == NULL)|| (s == NULL)|| (i128a == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = BOAT_ROUNDUP(strlen(s), 32) + 32 + \
                           (11 * 32 + 4);
    sa_ptr = sa;
    for(i = 0; i < saLen; i++)
    {
        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32) + 32;
        sa_data_length += sa_filled_length[i];
        sa_ptr++;
    }
    sa_data_length += saLen * 32 + 32;
    data_field.field_len += sa_data_length;

    ba_ptr = ba;
    for(i = 0; i < baLen; i++)
    {
        ba_filled_length[i] = BOAT_ROUNDUP(ba_ptr->field_len, 32) + 32;
        ba_data_length += ba_filled_length[i];
        ba_ptr++;
    }
    ba_data_length += baLen * 32;
    data_field.field_len += ba_data_length;

    i64a_data_length += i64aLen * 32 + 32;
    data_field.field_len += i64a_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testAllTypes(uint16,string[],bytes[4],string,int64[],int128[6])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(&ui16, sizeof(BUINT16));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT16), &ui16, sizeof(BUINT16));
    data_offset_ptr += 32;
    //param 'sa' offset location filled
    data_offset_location = 352;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba' offset location filled
    data_offset_location = sa_data_length + \
                           + 352;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 's' offset location filled
    data_offset_location = sa_data_length + \
                           ba_data_length + \
                           + 352;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'i64a' offset location filled
    data_offset_location = sa_data_length + \
                           ba_data_length + \
                           BOAT_ROUNDUP(strlen(s), 32) + 32 + \
                           + 352;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'i128a' data filled
    for(i = 0; i < 6; i++)
    {
        memcpy(fixedsize_bytes32, i128a[i], sizeof(BSINT128));
        memset(data_offset_ptr, 0x00, 32);
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BSINT128));
        memcpy(data_offset_ptr + 32 - sizeof(BSINT128), fixedsize_bytes32, sizeof(BSINT128));
        data_offset_ptr += 32;
    }

    //non-fixed param 'sa' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = saLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < saLen; i++)
    {
        if (i == 0) sa_offset_location[i] = saLen * 32;
        else sa_offset_location[i] = sa_offset_location[i - 1] + sa_filled_length[i - 1];
        data_offset_location = sa_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa_ptr = sa;
    for (i = 0; i < saLen; i++)
    {
        sa_actual_length[i] = sa_ptr->field_len;
        UtilityChangeEndian(&sa_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa_filled_length[i]);
        memcpy(data_offset_ptr, sa_ptr->field_ptr, sa_ptr->field_len);
        data_offset_ptr += sa_filled_length[i];
        sa_ptr++;
    }

    //non-fixed param 'ba' data filled
    //filled non-fixed size array data offset
    for (i = 0; i < baLen; i++)
    {
        if (i == 0) ba_offset_location[i] = baLen * 32;
        else ba_offset_location[i] = ba_offset_location[i - 1] + ba_filled_length[i - 1];
        data_offset_location = ba_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba_ptr = ba;
    for (i = 0; i < baLen; i++)
    {
        ba_actual_length[i] = ba_ptr->field_len;
        UtilityChangeEndian(&ba_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba_filled_length[i] = BOAT_ROUNDUP(ba_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba_filled_length[i]);
        memcpy(data_offset_ptr, ba_ptr->field_ptr, ba_ptr->field_len);
        data_offset_ptr += ba_filled_length[i];
        ba_ptr++;
    }

    //non-fixed param 's' data filled
    //filled fixed size non-fixed array length
    sLen = strlen(s);
    nonFixed_actual_length = sLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s, sLen);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'i64a' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = i64aLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled fixed size non-fixed array data
    i64a_ptr = i64a;
    for(i = 0; i < i64aLen; i++)
    {
        memcpy(fixedsize_bytes32, i64a_ptr, sizeof(BSINT64));
        if (*i64a_ptr >= 0)
        {
            memset(data_offset_ptr, 0x00, 32);
        }
        else
        {
            memset(data_offset_ptr, 0xFF, 32);
        }
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BSINT64));
        memcpy(data_offset_ptr + 32 - sizeof(BSINT64), fixedsize_bytes32, sizeof(BSINT64));
        data_offset_ptr += 32;
        i64a_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testFourFixedArray(BoatEthTx *tx_ptr, BSINT256 i256a[6], BoatFieldVariable *sa, BUINT8 bla[3], BoatFieldVariable *bya)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 saLen = 5;
    BUINT32 sa_data_length = 0;
    BUINT32 sa_offset_location[saLen];
    BUINT32 sa_filled_length[saLen];
    BUINT32 sa_actual_length[saLen];
    BoatFieldVariable *sa_ptr;
    BUINT32 byaLen = 4;
    BUINT32 bya_data_length = 0;
    BUINT32 bya_offset_location[byaLen];
    BUINT32 bya_filled_length[byaLen];
    BUINT32 bya_actual_length[byaLen];
    BoatFieldVariable *bya_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (i256a == NULL)|| (sa == NULL)|| (bya == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (11 * 32 + 4);
    sa_ptr = sa;
    for(i = 0; i < saLen; i++)
    {
        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32) + 32;
        sa_data_length += sa_filled_length[i];
        sa_ptr++;
    }
    sa_data_length += saLen * 32;
    data_field.field_len += sa_data_length;

    bya_ptr = bya;
    for(i = 0; i < byaLen; i++)
    {
        bya_filled_length[i] = BOAT_ROUNDUP(bya_ptr->field_len, 32) + 32;
        bya_data_length += bya_filled_length[i];
        bya_ptr++;
    }
    bya_data_length += byaLen * 32;
    data_field.field_len += bya_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testFourFixedArray(int256[6],string[5],bool[3],bytes[4])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'i256a' data filled
    for(i = 0; i < 6; i++)
    {
        memcpy(fixedsize_bytes32, i256a[i], sizeof(BSINT256));
        memset(data_offset_ptr, 0x00, 32);
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BSINT256));
        memcpy(data_offset_ptr + 32 - sizeof(BSINT256), fixedsize_bytes32, sizeof(BSINT256));
        data_offset_ptr += 32;
    }

    //param 'sa' offset location filled
    data_offset_location = 352;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'bla' data filled
    for(i = 0; i < 3; i++)
    {
        memcpy(fixedsize_bytes32, &bla[i], sizeof(BUINT8));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT8), &fixedsize_bytes32, sizeof(BUINT8));
        data_offset_ptr += 32;
    }

    //param 'bya' offset location filled
    data_offset_location = sa_data_length + \
                           + 352;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'sa' data filled
    //filled non-fixed size array data offset
    for (i = 0; i < saLen; i++)
    {
        if (i == 0) sa_offset_location[i] = saLen * 32;
        else sa_offset_location[i] = sa_offset_location[i - 1] + sa_filled_length[i - 1];
        data_offset_location = sa_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa_ptr = sa;
    for (i = 0; i < saLen; i++)
    {
        sa_actual_length[i] = sa_ptr->field_len;
        UtilityChangeEndian(&sa_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa_filled_length[i] = BOAT_ROUNDUP(sa_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa_filled_length[i]);
        memcpy(data_offset_ptr, sa_ptr->field_ptr, sa_ptr->field_len);
        data_offset_ptr += sa_filled_length[i];
        sa_ptr++;
    }

    //non-fixed param 'bya' data filled
    //filled non-fixed size array data offset
    for (i = 0; i < byaLen; i++)
    {
        if (i == 0) bya_offset_location[i] = byaLen * 32;
        else bya_offset_location[i] = bya_offset_location[i - 1] + bya_filled_length[i - 1];
        data_offset_location = bya_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    bya_ptr = bya;
    for (i = 0; i < byaLen; i++)
    {
        bya_actual_length[i] = bya_ptr->field_len;
        UtilityChangeEndian(&bya_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &bya_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        bya_filled_length[i] = BOAT_ROUNDUP(bya_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, bya_filled_length[i]);
        memcpy(data_offset_ptr, bya_ptr->field_ptr, bya_ptr->field_len);
        data_offset_ptr += bya_filled_length[i];
        bya_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testFiveNonFixedArray(BoatEthTx *tx_ptr, BoatFieldVariable*sa1, BUINT32 sa1Len, BSINT8 *i8a, BUINT32 i8aLen, BSINT16 *i16a, BUINT32 i16aLen, BSINT32 *i32a, BUINT32 i32aLen, BoatFieldVariable*sa2, BUINT32 sa2Len)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 sa1_data_length = 0;
    BUINT32 sa1_offset_location[sa1Len];
    BUINT32 sa1_filled_length[sa1Len];
    BUINT32 sa1_actual_length[sa1Len];
    BoatFieldVariable *sa1_ptr;
    BUINT32 i8a_data_length = 0;
    BUINT32 i8a_offset_location[i8aLen];
    BUINT32 i8a_filled_length[i8aLen];
    BUINT32 i8a_actual_length[i8aLen];
    BSINT8 *i8a_ptr;
    BUINT32 i16a_data_length = 0;
    BUINT32 i16a_offset_location[i16aLen];
    BUINT32 i16a_filled_length[i16aLen];
    BUINT32 i16a_actual_length[i16aLen];
    BSINT16 *i16a_ptr;
    BUINT32 i32a_data_length = 0;
    BUINT32 i32a_offset_location[i32aLen];
    BUINT32 i32a_filled_length[i32aLen];
    BUINT32 i32a_actual_length[i32aLen];
    BSINT32 *i32a_ptr;
    BUINT32 sa2_data_length = 0;
    BUINT32 sa2_offset_location[sa2Len];
    BUINT32 sa2_filled_length[sa2Len];
    BUINT32 sa2_actual_length[sa2Len];
    BoatFieldVariable *sa2_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (sa1 == NULL)|| (sa2 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (5 * 32 + 4);
    sa1_ptr = sa1;
    for(i = 0; i < sa1Len; i++)
    {
        sa1_filled_length[i] = BOAT_ROUNDUP(sa1_ptr->field_len, 32) + 32;
        sa1_data_length += sa1_filled_length[i];
        sa1_ptr++;
    }
    sa1_data_length += sa1Len * 32 + 32;
    data_field.field_len += sa1_data_length;

    i8a_data_length += i8aLen * 32 + 32;
    data_field.field_len += i8a_data_length;

    i16a_data_length += i16aLen * 32 + 32;
    data_field.field_len += i16a_data_length;

    i32a_data_length += i32aLen * 32 + 32;
    data_field.field_len += i32a_data_length;

    sa2_ptr = sa2;
    for(i = 0; i < sa2Len; i++)
    {
        sa2_filled_length[i] = BOAT_ROUNDUP(sa2_ptr->field_len, 32) + 32;
        sa2_data_length += sa2_filled_length[i];
        sa2_ptr++;
    }
    sa2_data_length += sa2Len * 32 + 32;
    data_field.field_len += sa2_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testFiveNonFixedArray(string[],int8[],int16[],int32[],string[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'sa1' offset location filled
    data_offset_location = 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'i8a' offset location filled
    data_offset_location = sa1_data_length + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'i16a' offset location filled
    data_offset_location = sa1_data_length + \
                           i8a_data_length + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'i32a' offset location filled
    data_offset_location = sa1_data_length + \
                           i8a_data_length + \
                           i16a_data_length + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'sa2' offset location filled
    data_offset_location = sa1_data_length + \
                           i8a_data_length + \
                           i16a_data_length + \
                           i32a_data_length + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'sa1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = sa1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < sa1Len; i++)
    {
        if (i == 0) sa1_offset_location[i] = sa1Len * 32;
        else sa1_offset_location[i] = sa1_offset_location[i - 1] + sa1_filled_length[i - 1];
        data_offset_location = sa1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa1_ptr = sa1;
    for (i = 0; i < sa1Len; i++)
    {
        sa1_actual_length[i] = sa1_ptr->field_len;
        UtilityChangeEndian(&sa1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa1_filled_length[i] = BOAT_ROUNDUP(sa1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa1_filled_length[i]);
        memcpy(data_offset_ptr, sa1_ptr->field_ptr, sa1_ptr->field_len);
        data_offset_ptr += sa1_filled_length[i];
        sa1_ptr++;
    }

    //non-fixed param 'i8a' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = i8aLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled fixed size non-fixed array data
    i8a_ptr = i8a;
    for(i = 0; i < i8aLen; i++)
    {
        memcpy(fixedsize_bytes32, i8a_ptr, sizeof(BSINT8));
        if (*i8a_ptr >= 0)
        {
            memset(data_offset_ptr, 0x00, 32);
        }
        else
        {
            memset(data_offset_ptr, 0xFF, 32);
        }
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BSINT8));
        memcpy(data_offset_ptr + 32 - sizeof(BSINT8), fixedsize_bytes32, sizeof(BSINT8));
        data_offset_ptr += 32;
        i8a_ptr++;
    }

    //non-fixed param 'i16a' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = i16aLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled fixed size non-fixed array data
    i16a_ptr = i16a;
    for(i = 0; i < i16aLen; i++)
    {
        memcpy(fixedsize_bytes32, i16a_ptr, sizeof(BSINT16));
        if (*i16a_ptr >= 0)
        {
            memset(data_offset_ptr, 0x00, 32);
        }
        else
        {
            memset(data_offset_ptr, 0xFF, 32);
        }
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BSINT16));
        memcpy(data_offset_ptr + 32 - sizeof(BSINT16), fixedsize_bytes32, sizeof(BSINT16));
        data_offset_ptr += 32;
        i16a_ptr++;
    }

    //non-fixed param 'i32a' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = i32aLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled fixed size non-fixed array data
    i32a_ptr = i32a;
    for(i = 0; i < i32aLen; i++)
    {
        memcpy(fixedsize_bytes32, i32a_ptr, sizeof(BSINT32));
        if (*i32a_ptr >= 0)
        {
            memset(data_offset_ptr, 0x00, 32);
        }
        else
        {
            memset(data_offset_ptr, 0xFF, 32);
        }
        UtilityChangeEndian(fixedsize_bytes32, sizeof(BSINT32));
        memcpy(data_offset_ptr + 32 - sizeof(BSINT32), fixedsize_bytes32, sizeof(BSINT32));
        data_offset_ptr += 32;
        i32a_ptr++;
    }

    //non-fixed param 'sa2' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = sa2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < sa2Len; i++)
    {
        if (i == 0) sa2_offset_location[i] = sa2Len * 32;
        else sa2_offset_location[i] = sa2_offset_location[i - 1] + sa2_filled_length[i - 1];
        data_offset_location = sa2_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    sa2_ptr = sa2;
    for (i = 0; i < sa2Len; i++)
    {
        sa2_actual_length[i] = sa2_ptr->field_len;
        UtilityChangeEndian(&sa2_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &sa2_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        sa2_filled_length[i] = BOAT_ROUNDUP(sa2_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, sa2_filled_length[i]);
        memcpy(data_offset_ptr, sa2_ptr->field_ptr, sa2_ptr->field_len);
        data_offset_ptr += sa2_filled_length[i];
        sa2_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_testThreeFixedSizeAndTwoArrayAndOneNonFixedSize(BoatEthTx *tx_ptr, BUINT64 ui64, BUINT8*by, BUINT32 byLen, BUINT128 ui128, BoatFieldVariable*bya1, BUINT32 bya1Len, BoatFieldVariable*bya2, BUINT32 bya2Len, BUINT256 ui256)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 data_offset_location;
    BUINT32 nonFixed_filled_length;
    (void) nonFixed_filled_length;
    BUINT32 nonFixed_actual_length;
    BUINT32 bya1_data_length = 0;
    BUINT32 bya1_offset_location[bya1Len];
    BUINT32 bya1_filled_length[bya1Len];
    BUINT32 bya1_actual_length[bya1Len];
    BoatFieldVariable *bya1_ptr;
    BUINT32 bya2_data_length = 0;
    BUINT32 bya2_offset_location[bya2Len];
    BUINT32 bya2_filled_length[bya2Len];
    BUINT32 bya2_actual_length[bya2Len];
    BoatFieldVariable *bya2_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (by == NULL)|| (ui128 == NULL)|| (bya1 == NULL)|| (bya2 == NULL)|| (ui256 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = BOAT_ROUNDUP(byLen , 32) + 32 + \
                           (6 * 32 + 4);
    bya1_ptr = bya1;
    for(i = 0; i < bya1Len; i++)
    {
        bya1_filled_length[i] = BOAT_ROUNDUP(bya1_ptr->field_len, 32) + 32;
        bya1_data_length += bya1_filled_length[i];
        bya1_ptr++;
    }
    bya1_data_length += bya1Len * 32 + 32;
    data_field.field_len += bya1_data_length;

    bya2_ptr = bya2;
    for(i = 0; i < bya2Len; i++)
    {
        bya2_filled_length[i] = BOAT_ROUNDUP(bya2_ptr->field_len, 32) + 32;
        bya2_data_length += bya2_filled_length[i];
        bya2_ptr++;
    }
    bya2_data_length += bya2Len * 32 + 32;
    data_field.field_len += bya2_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testThreeFixedSizeAndTwoArrayAndOneNonFixedSize(uint64,bytes,uint128,bytes[],bytes[],uint256)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(&ui64, sizeof(BUINT64));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT64), &ui64, sizeof(BUINT64));
    data_offset_ptr += 32;
    //param 'by' offset location filled
    data_offset_location = 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(ui128, sizeof(BUINT128));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT128), ui128, sizeof(BUINT128));
    data_offset_ptr += 32;
    //param 'bya1' offset location filled
    data_offset_location = BOAT_ROUNDUP(byLen , 32) + 32 + \
                           + 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'bya2' offset location filled
    data_offset_location = BOAT_ROUNDUP(byLen , 32) + 32 + \
                           bya1_data_length + \
                           + 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(ui256, sizeof(BUINT256));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT256), ui256, sizeof(BUINT256));
    data_offset_ptr += 32;
    //non-fixed param 'by' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = byLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(byLen , 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, by, byLen);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'bya1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = bya1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < bya1Len; i++)
    {
        if (i == 0) bya1_offset_location[i] = bya1Len * 32;
        else bya1_offset_location[i] = bya1_offset_location[i - 1] + bya1_filled_length[i - 1];
        data_offset_location = bya1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    bya1_ptr = bya1;
    for (i = 0; i < bya1Len; i++)
    {
        bya1_actual_length[i] = bya1_ptr->field_len;
        UtilityChangeEndian(&bya1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &bya1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        bya1_filled_length[i] = BOAT_ROUNDUP(bya1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, bya1_filled_length[i]);
        memcpy(data_offset_ptr, bya1_ptr->field_ptr, bya1_ptr->field_len);
        data_offset_ptr += bya1_filled_length[i];
        bya1_ptr++;
    }

    //non-fixed param 'bya2' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = bya2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < bya2Len; i++)
    {
        if (i == 0) bya2_offset_location[i] = bya2Len * 32;
        else bya2_offset_location[i] = bya2_offset_location[i - 1] + bya2_filled_length[i - 1];
        data_offset_location = bya2_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    bya2_ptr = bya2;
    for (i = 0; i < bya2Len; i++)
    {
        bya2_actual_length[i] = bya2_ptr->field_len;
        UtilityChangeEndian(&bya2_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &bya2_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        bya2_filled_length[i] = BOAT_ROUNDUP(bya2_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, bya2_filled_length[i]);
        memcpy(data_offset_ptr, bya2_ptr->field_ptr, bya2_ptr->field_len);
        data_offset_ptr += bya2_filled_length[i];
        bya2_ptr++;
    }

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *TestPython2_destroy(BoatEthTx *tx_ptr)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if (tx_ptr == NULL)
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (0 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "destroy()";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

