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

/*!@brief Header file for performing RAW transaction

@file
boatplatone.h is header file for RAW transaction construction and performing.
*/

#ifndef __BOATPLATONE_H__
#define __BOATPLATONE_H__

#include "boatinternal.h"


/*! @defgroup platonep protocol-platone 
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Construct a raw PlatONE transacton and encodes it as per RLP rules.
 *
 * @details
 *   This function constructs a raw transacton and sends it asynchronously (i.e.
 *   don't wait for it being mined).
    
 *   \n AN INTRODUCTION OF HOW RAW TRANSACTION IS CONSTRUCTED
 *   
 *   <b>[FIELDS IN A RAW TRANSACTION]</b>
 *   A RAW transaction consists of following 10 fields:
 *       1. nonce;
 *       2. gasprice;
 *       3. gaslimit;
 *       4. recipient;
 *       5. value(optional);
 *       6. data(contain at least an 8-byte bigendian txtype field in data);
 *       7. txtype(same value as above, except trimming leading zeros);
 *       8. v;
 *       9. signature.r;
 *      10. signature.s;
 *
 *   These transaction fields are encoded as elements of a LIST in above order
 *   as per RLP encoding rules. "LIST" is a type of RLP field.
 *
 *   EXCEPTION:
 *   For Ethereum any fields (except <recipient>) having a value of zero are
 *   treated as NULL stream in RLP encoding instead of 1-byte-size stream whose
 *   value is 0. For example, nonce = 0 is encoded as 0x80 which represents NULL
 *   instead of 0x00 which represents a 1-byte-size stream whose value is 0.
 *
 *   <b>[HOW TO CONSTRUCT A RAW TRANSACTION]</b>
 *   A RAW transaction is constructed in 4 steps in different ways according to
 *   the blockchain network's EIP-155 compatibility. 
 *
 *   See following article for details about EIP-155: 
 *   https://github.com/ethereum/EIPs/blob/master/EIPS/eip-155.md
 *
 *   <I>CASE 1: If the blockchain network does NOT support EIP-155:</I>
 *   
 *       \n - Step 1: Encode a LIST containing only the first 7 fields.
 *       \n - Step 2: Calculate SHA3 hash of the encoded stream in Step 1.
 *       \n - Step 3: Sign the hash in Step 2. This generates r, s and parity (0 or 1) for recovery identifier.
 *       \n - Step 4: Encode a LIST containing all 10 fields, where
 *               First 7 fields are same as what they are;
 *               v = parity + 27, where parity is given in Step 3;
 *               r and s are given in Step 3.
 *
 *   <I>CASE 2: If the blockchain network DOES support EIP-155:</I>
 *       \n - Step 1: Encode all 10 fields (a LIST containing all 10 fields), where
 *               First 7 fields are same as what they are;
 *               v = Chain ID;
 *               r = 0;
 *               s = 0;
 *               NOTE: zero value fields other than <recipient> are encoded as NULL stream.
 *       \n - Step 2: Same as CASE 1.
 *       \n - Step 3: Same as CASE 1.
 *       \n - Step 4: Encode a LIST containing all 10 fields, where
 *               First 7 fields are same as what they are;
 *               v = Chain ID * 2 + parity + 35, where parity is given in Step 3;
 *               r and s are given in Step 3.
 * 
 * @param[in] tx_ptr
 *       A pointer to the context of the transaction.
 * 
 * @return
 *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
 *   of the error codes.
 ******************************************************************************/
BOAT_RESULT PlatoneSendRawtx(BOAT_INOUT BoatPlatoneTx *tx_ptr);


/*! @}*/


/*!****************************************************************************
 * @brief parse platone nodeinfo form Contract management service
 *
 * @details
 *   This function Obtain node information by calling the contract management service，
 * and parse platone nodeinfo 
 *	 
 * @param[in] monitor
 *   return form contract management service
 * @param[in] result_out
 * platone node info for Boat
 *
 * @return
 *   This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
 *   of the error codes.
 *	 
 * @see 
 ******************************************************************************/
int Platone_get_Nodeinfo(const char * const monitor,nodesResult *result_out);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
