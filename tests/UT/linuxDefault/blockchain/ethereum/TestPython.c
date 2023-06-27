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

BCHAR *TestPython_organizer(BoatEthTx *tx_ptr)
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

BCHAR *TestPython_testOneBytesArray(BoatEthTx *tx_ptr, BoatFieldVariable*ba1, BUINT32 ba1Len)
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
    BUINT32 ba1_data_length = 0;
    BUINT32 ba1_offset_location[ba1Len];
    BUINT32 ba1_filled_length[ba1Len];
    BUINT32 ba1_actual_length[ba1Len];
    BoatFieldVariable *ba1_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (ba1 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (1 * 32 + 4);
    ba1_ptr = ba1;
    for(i = 0; i < ba1Len; i++)
    {
        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32) + 32;
        ba1_data_length += ba1_filled_length[i];
        ba1_ptr++;
    }
    ba1_data_length += ba1Len * 32 + 32;
    data_field.field_len += ba1_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testOneBytesArray(bytes[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'ba1' offset location filled
    data_offset_location = 32;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'ba1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba1Len; i++)
    {
        if (i == 0) ba1_offset_location[i] = ba1Len * 32;
        else ba1_offset_location[i] = ba1_offset_location[i - 1] + ba1_filled_length[i - 1];
        data_offset_location = ba1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba1_ptr = ba1;
    for (i = 0; i < ba1Len; i++)
    {
        ba1_actual_length[i] = ba1_ptr->field_len;
        UtilityChangeEndian(&ba1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba1_filled_length[i]);
        memcpy(data_offset_ptr, ba1_ptr->field_ptr, ba1_ptr->field_len);
        data_offset_ptr += ba1_filled_length[i];
        ba1_ptr++;
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

BCHAR *TestPython_testTwoBytesArrays(BoatEthTx *tx_ptr, BoatFieldVariable*ba1, BUINT32 ba1Len, BoatFieldVariable*ba2, BUINT32 ba2Len)
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
    BUINT32 ba1_data_length = 0;
    BUINT32 ba1_offset_location[ba1Len];
    BUINT32 ba1_filled_length[ba1Len];
    BUINT32 ba1_actual_length[ba1Len];
    BoatFieldVariable *ba1_ptr;
    BUINT32 ba2_data_length = 0;
    BUINT32 ba2_offset_location[ba2Len];
    BUINT32 ba2_filled_length[ba2Len];
    BUINT32 ba2_actual_length[ba2Len];
    BoatFieldVariable *ba2_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (ba1 == NULL)|| (ba2 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (2 * 32 + 4);
    ba1_ptr = ba1;
    for(i = 0; i < ba1Len; i++)
    {
        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32) + 32;
        ba1_data_length += ba1_filled_length[i];
        ba1_ptr++;
    }
    ba1_data_length += ba1Len * 32 + 32;
    data_field.field_len += ba1_data_length;

    ba2_ptr = ba2;
    for(i = 0; i < ba2Len; i++)
    {
        ba2_filled_length[i] = BOAT_ROUNDUP(ba2_ptr->field_len, 32) + 32;
        ba2_data_length += ba2_filled_length[i];
        ba2_ptr++;
    }
    ba2_data_length += ba2Len * 32 + 32;
    data_field.field_len += ba2_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoBytesArrays(bytes[],bytes[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'ba1' offset location filled
    data_offset_location = 64;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba2' offset location filled
    data_offset_location = ba1_data_length + \
                           + 64;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'ba1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba1Len; i++)
    {
        if (i == 0) ba1_offset_location[i] = ba1Len * 32;
        else ba1_offset_location[i] = ba1_offset_location[i - 1] + ba1_filled_length[i - 1];
        data_offset_location = ba1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba1_ptr = ba1;
    for (i = 0; i < ba1Len; i++)
    {
        ba1_actual_length[i] = ba1_ptr->field_len;
        UtilityChangeEndian(&ba1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba1_filled_length[i]);
        memcpy(data_offset_ptr, ba1_ptr->field_ptr, ba1_ptr->field_len);
        data_offset_ptr += ba1_filled_length[i];
        ba1_ptr++;
    }

    //non-fixed param 'ba2' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba2Len; i++)
    {
        if (i == 0) ba2_offset_location[i] = ba2Len * 32;
        else ba2_offset_location[i] = ba2_offset_location[i - 1] + ba2_filled_length[i - 1];
        data_offset_location = ba2_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba2_ptr = ba2;
    for (i = 0; i < ba2Len; i++)
    {
        ba2_actual_length[i] = ba2_ptr->field_len;
        UtilityChangeEndian(&ba2_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba2_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba2_filled_length[i] = BOAT_ROUNDUP(ba2_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba2_filled_length[i]);
        memcpy(data_offset_ptr, ba2_ptr->field_ptr, ba2_ptr->field_len);
        data_offset_ptr += ba2_filled_length[i];
        ba2_ptr++;
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

BCHAR *TestPython_testTwoBytesArraysAndOneInt(BoatEthTx *tx_ptr, BoatFieldVariable*ba1, BUINT32 ba1Len, BUINT256 i1, BoatFieldVariable*ba2, BUINT32 ba2Len)
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
    BUINT32 ba1_data_length = 0;
    BUINT32 ba1_offset_location[ba1Len];
    BUINT32 ba1_filled_length[ba1Len];
    BUINT32 ba1_actual_length[ba1Len];
    BoatFieldVariable *ba1_ptr;
    BUINT32 ba2_data_length = 0;
    BUINT32 ba2_offset_location[ba2Len];
    BUINT32 ba2_filled_length[ba2Len];
    BUINT32 ba2_actual_length[ba2Len];
    BoatFieldVariable *ba2_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (ba1 == NULL)|| (i1 == NULL)|| (ba2 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (3 * 32 + 4);
    ba1_ptr = ba1;
    for(i = 0; i < ba1Len; i++)
    {
        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32) + 32;
        ba1_data_length += ba1_filled_length[i];
        ba1_ptr++;
    }
    ba1_data_length += ba1Len * 32 + 32;
    data_field.field_len += ba1_data_length;

    ba2_ptr = ba2;
    for(i = 0; i < ba2Len; i++)
    {
        ba2_filled_length[i] = BOAT_ROUNDUP(ba2_ptr->field_len, 32) + 32;
        ba2_data_length += ba2_filled_length[i];
        ba2_ptr++;
    }
    ba2_data_length += ba2Len * 32 + 32;
    data_field.field_len += ba2_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoBytesArraysAndOneInt(bytes[],uint256,bytes[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'ba1' offset location filled
    data_offset_location = 96;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(i1, sizeof(BUINT256));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT256), i1, sizeof(BUINT256));
    data_offset_ptr += 32;
    //param 'ba2' offset location filled
    data_offset_location = ba1_data_length + \
                           + 96;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'ba1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba1Len; i++)
    {
        if (i == 0) ba1_offset_location[i] = ba1Len * 32;
        else ba1_offset_location[i] = ba1_offset_location[i - 1] + ba1_filled_length[i - 1];
        data_offset_location = ba1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba1_ptr = ba1;
    for (i = 0; i < ba1Len; i++)
    {
        ba1_actual_length[i] = ba1_ptr->field_len;
        UtilityChangeEndian(&ba1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba1_filled_length[i]);
        memcpy(data_offset_ptr, ba1_ptr->field_ptr, ba1_ptr->field_len);
        data_offset_ptr += ba1_filled_length[i];
        ba1_ptr++;
    }

    //non-fixed param 'ba2' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba2Len; i++)
    {
        if (i == 0) ba2_offset_location[i] = ba2Len * 32;
        else ba2_offset_location[i] = ba2_offset_location[i - 1] + ba2_filled_length[i - 1];
        data_offset_location = ba2_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba2_ptr = ba2;
    for (i = 0; i < ba2Len; i++)
    {
        ba2_actual_length[i] = ba2_ptr->field_len;
        UtilityChangeEndian(&ba2_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba2_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba2_filled_length[i] = BOAT_ROUNDUP(ba2_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba2_filled_length[i]);
        memcpy(data_offset_ptr, ba2_ptr->field_ptr, ba2_ptr->field_len);
        data_offset_ptr += ba2_filled_length[i];
        ba2_ptr++;
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

BCHAR *TestPython_testTwoBytesArraysAndTwoNonFixed(BoatEthTx *tx_ptr, BCHAR*s1, BoatFieldVariable*ba1, BUINT32 ba1Len, BCHAR*s2, BoatFieldVariable*ba2, BUINT32 ba2Len)
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
    BUINT32 s1Len;
    BUINT32 ba1_data_length = 0;
    BUINT32 ba1_offset_location[ba1Len];
    BUINT32 ba1_filled_length[ba1Len];
    BUINT32 ba1_actual_length[ba1Len];
    BoatFieldVariable *ba1_ptr;
    BUINT32 s2Len;
    BUINT32 ba2_data_length = 0;
    BUINT32 ba2_offset_location[ba2Len];
    BUINT32 ba2_filled_length[ba2Len];
    BUINT32 ba2_actual_length[ba2Len];
    BoatFieldVariable *ba2_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (s1 == NULL)|| (ba1 == NULL)|| (s2 == NULL)|| (ba2 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           BOAT_ROUNDUP(strlen(s2), 32) + 32 + \
                           (4 * 32 + 4);
    ba1_ptr = ba1;
    for(i = 0; i < ba1Len; i++)
    {
        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32) + 32;
        ba1_data_length += ba1_filled_length[i];
        ba1_ptr++;
    }
    ba1_data_length += ba1Len * 32 + 32;
    data_field.field_len += ba1_data_length;

    ba2_ptr = ba2;
    for(i = 0; i < ba2Len; i++)
    {
        ba2_filled_length[i] = BOAT_ROUNDUP(ba2_ptr->field_len, 32) + 32;
        ba2_data_length += ba2_filled_length[i];
        ba2_ptr++;
    }
    ba2_data_length += ba2Len * 32 + 32;
    data_field.field_len += ba2_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoBytesArraysAndTwoNonFixed(string,bytes[],string,bytes[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 's1' offset location filled
    data_offset_location = 128;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba1' offset location filled
    data_offset_location = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           + 128;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 's2' offset location filled
    data_offset_location = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           ba1_data_length + \
                           + 128;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba2' offset location filled
    data_offset_location = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           ba1_data_length + \
                           BOAT_ROUNDUP(strlen(s2), 32) + 32 + \
                           + 128;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 's1' data filled
    //filled fixed size non-fixed array length
    s1Len = strlen(s1);
    nonFixed_actual_length = s1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s1), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s1, s1Len);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'ba1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba1Len; i++)
    {
        if (i == 0) ba1_offset_location[i] = ba1Len * 32;
        else ba1_offset_location[i] = ba1_offset_location[i - 1] + ba1_filled_length[i - 1];
        data_offset_location = ba1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba1_ptr = ba1;
    for (i = 0; i < ba1Len; i++)
    {
        ba1_actual_length[i] = ba1_ptr->field_len;
        UtilityChangeEndian(&ba1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba1_filled_length[i]);
        memcpy(data_offset_ptr, ba1_ptr->field_ptr, ba1_ptr->field_len);
        data_offset_ptr += ba1_filled_length[i];
        ba1_ptr++;
    }

    //non-fixed param 's2' data filled
    //filled fixed size non-fixed array length
    s2Len = strlen(s2);
    nonFixed_actual_length = s2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s2), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s2, s2Len);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'ba2' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba2Len; i++)
    {
        if (i == 0) ba2_offset_location[i] = ba2Len * 32;
        else ba2_offset_location[i] = ba2_offset_location[i - 1] + ba2_filled_length[i - 1];
        data_offset_location = ba2_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba2_ptr = ba2;
    for (i = 0; i < ba2Len; i++)
    {
        ba2_actual_length[i] = ba2_ptr->field_len;
        UtilityChangeEndian(&ba2_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba2_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba2_filled_length[i] = BOAT_ROUNDUP(ba2_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba2_filled_length[i]);
        memcpy(data_offset_ptr, ba2_ptr->field_ptr, ba2_ptr->field_len);
        data_offset_ptr += ba2_filled_length[i];
        ba2_ptr++;
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

BCHAR *TestPython_testTwoBytesArraysAndThreeNonFixed(BoatEthTx *tx_ptr, BCHAR*s1, BUINT8*b1, BUINT32 b1Len, BoatFieldVariable*ba1, BUINT32 ba1Len, BCHAR*s2, BoatFieldVariable*ba2, BUINT32 ba2Len)
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
    BUINT32 s1Len;
    BUINT32 ba1_data_length = 0;
    BUINT32 ba1_offset_location[ba1Len];
    BUINT32 ba1_filled_length[ba1Len];
    BUINT32 ba1_actual_length[ba1Len];
    BoatFieldVariable *ba1_ptr;
    BUINT32 s2Len;
    BUINT32 ba2_data_length = 0;
    BUINT32 ba2_offset_location[ba2Len];
    BUINT32 ba2_filled_length[ba2Len];
    BUINT32 ba2_actual_length[ba2Len];
    BoatFieldVariable *ba2_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (s1 == NULL)|| (b1 == NULL)|| (ba1 == NULL)|| (s2 == NULL)|| (ba2 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           BOAT_ROUNDUP(b1Len , 32) + 32 + \
                           BOAT_ROUNDUP(strlen(s2), 32) + 32 + \
                           (5 * 32 + 4);
    ba1_ptr = ba1;
    for(i = 0; i < ba1Len; i++)
    {
        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32) + 32;
        ba1_data_length += ba1_filled_length[i];
        ba1_ptr++;
    }
    ba1_data_length += ba1Len * 32 + 32;
    data_field.field_len += ba1_data_length;

    ba2_ptr = ba2;
    for(i = 0; i < ba2Len; i++)
    {
        ba2_filled_length[i] = BOAT_ROUNDUP(ba2_ptr->field_len, 32) + 32;
        ba2_data_length += ba2_filled_length[i];
        ba2_ptr++;
    }
    ba2_data_length += ba2Len * 32 + 32;
    data_field.field_len += ba2_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoBytesArraysAndThreeNonFixed(string,bytes,bytes[],string,bytes[])";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 's1' offset location filled
    data_offset_location = 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'b1' offset location filled
    data_offset_location = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba1' offset location filled
    data_offset_location = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           BOAT_ROUNDUP(b1Len , 32) + 32 + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 's2' offset location filled
    data_offset_location = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           BOAT_ROUNDUP(b1Len , 32) + 32 + \
                           ba1_data_length + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'ba2' offset location filled
    data_offset_location = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           BOAT_ROUNDUP(b1Len , 32) + 32 + \
                           ba1_data_length + \
                           BOAT_ROUNDUP(strlen(s2), 32) + 32 + \
                           + 160;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 's1' data filled
    //filled fixed size non-fixed array length
    s1Len = strlen(s1);
    nonFixed_actual_length = s1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s1), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s1, s1Len);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'b1' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = b1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(b1Len , 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, b1, b1Len);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'ba1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba1Len; i++)
    {
        if (i == 0) ba1_offset_location[i] = ba1Len * 32;
        else ba1_offset_location[i] = ba1_offset_location[i - 1] + ba1_filled_length[i - 1];
        data_offset_location = ba1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba1_ptr = ba1;
    for (i = 0; i < ba1Len; i++)
    {
        ba1_actual_length[i] = ba1_ptr->field_len;
        UtilityChangeEndian(&ba1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba1_filled_length[i]);
        memcpy(data_offset_ptr, ba1_ptr->field_ptr, ba1_ptr->field_len);
        data_offset_ptr += ba1_filled_length[i];
        ba1_ptr++;
    }

    //non-fixed param 's2' data filled
    //filled fixed size non-fixed array length
    s2Len = strlen(s2);
    nonFixed_actual_length = s2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s2), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s2, s2Len);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'ba2' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba2Len; i++)
    {
        if (i == 0) ba2_offset_location[i] = ba2Len * 32;
        else ba2_offset_location[i] = ba2_offset_location[i - 1] + ba2_filled_length[i - 1];
        data_offset_location = ba2_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba2_ptr = ba2;
    for (i = 0; i < ba2Len; i++)
    {
        ba2_actual_length[i] = ba2_ptr->field_len;
        UtilityChangeEndian(&ba2_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba2_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba2_filled_length[i] = BOAT_ROUNDUP(ba2_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba2_filled_length[i]);
        memcpy(data_offset_ptr, ba2_ptr->field_ptr, ba2_ptr->field_len);
        data_offset_ptr += ba2_filled_length[i];
        ba2_ptr++;
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

BCHAR *TestPython_testTwoStringAndtwoBytesAndOneBytesArray(BoatEthTx *tx_ptr, BoatFieldVariable*ba1, BUINT32 ba1Len, BCHAR*s1, BCHAR*s2, BUINT256 ui1, BUINT8*b1, BUINT32 b1Len, BUINT8*b2, BUINT32 b2Len)
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
    BUINT32 ba1_data_length = 0;
    BUINT32 ba1_offset_location[ba1Len];
    BUINT32 ba1_filled_length[ba1Len];
    BUINT32 ba1_actual_length[ba1Len];
    BoatFieldVariable *ba1_ptr;
    BUINT32 s1Len;
    BUINT32 s2Len;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (ba1 == NULL)|| (s1 == NULL)|| (s2 == NULL)|| (ui1 == NULL)|| (b1 == NULL)|| (b2 == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           BOAT_ROUNDUP(strlen(s2), 32) + 32 + \
                           BOAT_ROUNDUP(b1Len , 32) + 32 + \
                           BOAT_ROUNDUP(b2Len , 32) + 32 + \
                           (6 * 32 + 4);
    ba1_ptr = ba1;
    for(i = 0; i < ba1Len; i++)
    {
        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32) + 32;
        ba1_data_length += ba1_filled_length[i];
        ba1_ptr++;
    }
    ba1_data_length += ba1Len * 32 + 32;
    data_field.field_len += ba1_data_length;

    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "testTwoStringAndtwoBytesAndOneBytesArray(bytes[],string,string,uint256,bytes,bytes)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'ba1' offset location filled
    data_offset_location = 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 's1' offset location filled
    data_offset_location = ba1_data_length + \
                           + 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 's2' offset location filled
    data_offset_location = ba1_data_length + \
                           BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           + 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(ui1, sizeof(BUINT256));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT256), ui1, sizeof(BUINT256));
    data_offset_ptr += 32;
    //param 'b1' offset location filled
    data_offset_location = ba1_data_length + \
                           BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           BOAT_ROUNDUP(strlen(s2), 32) + 32 + \
                           + 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //param 'b2' offset location filled
    data_offset_location = ba1_data_length + \
                           BOAT_ROUNDUP(strlen(s1), 32) + 32 + \
                           BOAT_ROUNDUP(strlen(s2), 32) + 32 + \
                           BOAT_ROUNDUP(b1Len , 32) + 32 + \
                           + 192;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'ba1' data filled
    //filled non-fixed size non-fixed array length
    nonFixed_actual_length  = ba1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    //filled non-fixed size array data offset
    for (i = 0; i < ba1Len; i++)
    {
        if (i == 0) ba1_offset_location[i] = ba1Len * 32;
        else ba1_offset_location[i] = ba1_offset_location[i - 1] + ba1_filled_length[i - 1];
        data_offset_location = ba1_offset_location[i];
        UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
        data_offset_ptr += 32;

    }

    ba1_ptr = ba1;
    for (i = 0; i < ba1Len; i++)
    {
        ba1_actual_length[i] = ba1_ptr->field_len;
        UtilityChangeEndian(&ba1_actual_length[i], sizeof(BUINT32));
        memset(data_offset_ptr, 0x00, 32);
        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &ba1_actual_length[i], sizeof(BUINT32));
        data_offset_ptr += 32;

        ba1_filled_length[i] = BOAT_ROUNDUP(ba1_ptr->field_len, 32);
        memset(data_offset_ptr, 0x00, ba1_filled_length[i]);
        memcpy(data_offset_ptr, ba1_ptr->field_ptr, ba1_ptr->field_len);
        data_offset_ptr += ba1_filled_length[i];
        ba1_ptr++;
    }

    //non-fixed param 's1' data filled
    //filled fixed size non-fixed array length
    s1Len = strlen(s1);
    nonFixed_actual_length = s1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s1), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s1, s1Len);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 's2' data filled
    //filled fixed size non-fixed array length
    s2Len = strlen(s2);
    nonFixed_actual_length = s2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(s2), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, s2, s2Len);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'b1' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = b1Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(b1Len , 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, b1, b1Len);
    data_offset_ptr += nonFixed_filled_length;

    //non-fixed param 'b2' data filled
    //filled fixed size non-fixed array length
    nonFixed_actual_length = b2Len;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(b2Len , 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, b2, b2Len);
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

BCHAR *TestPython_destroy(BoatEthTx *tx_ptr)
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

