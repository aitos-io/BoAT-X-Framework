/******************************************************************************
This file is generated from contract ABI. DO NOT modify it by hand.
******************************************************************************/


// Generated C function interface from smart contract ABI

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

BCHAR *mycontract_cpp_abi_retrieve(BoatVenachainTx *tx_ptr)
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

    function_prototye_str = "retrieve()";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    call_result_str = BoatVenachainCallContractFunc(tx_ptr, data_field.field_ptr, data_field.field_len);


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

BCHAR *mycontract_cpp_abi_store(BoatVenachainTx *tx_ptr, BCHAR*word)
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
    BUINT32 wordLen;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (word == NULL))
    {
        return NULL;
    }

    boat_try(BoatVenachainTxSetNonce(tx_ptr, BOAT_VENACHAIN_NONCE_AUTO));

    data_field.field_len = BOAT_ROUNDUP(strlen(word), 32) + 32 + \
                           (1 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "store(string)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    //param 'word' offset location filled
    data_offset_location = 32;
    UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));
    data_offset_ptr += 32;

    //non-fixed param 'word' data filled
    //filled fixed size non-fixed array length
    wordLen = strlen(word);
    nonFixed_actual_length = wordLen;
    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));
    memset(data_offset_ptr, 0x00, 32);
    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));
    data_offset_ptr += 32;

    nonFixed_filled_length = BOAT_ROUNDUP(strlen(word), 32);
    memset(data_offset_ptr, 0x00, nonFixed_filled_length);
    memcpy(data_offset_ptr, word, wordLen);
    data_offset_ptr += nonFixed_filled_length;

    boat_try(BoatVenachainTxSetData(tx_ptr, &data_field));

    boat_try(BoatVenachainTxSend(tx_ptr));

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

