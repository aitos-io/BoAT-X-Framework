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

BCHAR *TestPython2_organizer(BoatEthTx *tx_ptr);
BCHAR *TestPython2_testOneFixedSize(BoatEthTx *tx_ptr, Bbytes7 b7);
BCHAR *TestPython2_testTwoFixedSize(BoatEthTx *tx_ptr, BSINT8 i8, BoatAddress ad1);
BCHAR *TestPython2_testThreeFixedSize(BoatEthTx *tx_ptr, BUINT8 b, BoatAddress ad2, Bbytes20 b20);
BCHAR *TestPython2_testOneFixedSizeNonFixedArray(BoatEthTx *tx_ptr, BSINT128 *i128a, BUINT32 i128aLen);
BCHAR *TestPython2_testOneFixedSizeFixedArray(BoatEthTx *tx_ptr, BoatAddress aa[2]);
BCHAR *TestPython2_testTwoFixedSizeArrays(BoatEthTx *tx_ptr, Bbytes5 *b5a, BUINT32 b5aLen, BUINT16 uia[3]);
BCHAR *TestPython2_testTwoFixedSizeArraysAndOneNonFixedSizeFixedArray(BoatEthTx *tx_ptr, Bbytes29 *b29a, BUINT32 b29aLen, Bbytes13 b13a[3], BoatFieldVariable *sa, Bbytes7 b7);
BCHAR *TestPython2_testOneStringArray(BoatEthTx *tx_ptr, BoatFieldVariable*sa, BUINT32 saLen);
BCHAR *TestPython2_testTwoStringArrays(BoatEthTx *tx_ptr, BoatFieldVariable*sa1, BUINT32 sa1Len, BoatFieldVariable*sa2, BUINT32 sa2Len);
BCHAR *TestPython2_testTwoTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen);
BCHAR *TestPython2_testThreeTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen, BoatFieldVariable *ba);
BCHAR *TestPython2_testFourTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen, BoatFieldVariable *ba, BCHAR*s);
BCHAR *TestPython2_testFiveTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen, BoatFieldVariable *ba, BCHAR*s, BSINT64 *i64a, BUINT32 i64aLen);
BCHAR *TestPython2_testAllTypes(BoatEthTx *tx_ptr, BUINT16 ui16, BoatFieldVariable*sa, BUINT32 saLen, BoatFieldVariable *ba, BCHAR*s, BSINT64 *i64a, BUINT32 i64aLen, BSINT128 i128a[6]);
BCHAR *TestPython2_testFourFixedArray(BoatEthTx *tx_ptr, BSINT256 i256a[6], BoatFieldVariable *sa, BUINT8 bla[3], BoatFieldVariable *bya);
BCHAR *TestPython2_testFiveNonFixedArray(BoatEthTx *tx_ptr, BoatFieldVariable*sa1, BUINT32 sa1Len, BSINT8 *i8a, BUINT32 i8aLen, BSINT16 *i16a, BUINT32 i16aLen, BSINT32 *i32a, BUINT32 i32aLen, BoatFieldVariable*sa2, BUINT32 sa2Len);
BCHAR *TestPython2_testThreeFixedSizeAndTwoArrayAndOneNonFixedSize(BoatEthTx *tx_ptr, BUINT64 ui64, BUINT8*by, BUINT32 byLen, BUINT128 ui128, BoatFieldVariable*bya1, BUINT32 bya1Len, BoatFieldVariable*bya2, BUINT32 bya2Len, BUINT256 ui256);
BCHAR *TestPython2_destroy(BoatEthTx *tx_ptr);
