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

/*!@brief Perform RAW transaction

@file
boathlfabric.c contains functions to construct a raw transaction, perform it and 
wait for its receipt.
*/


/* self-header include */
#include "boatconfig.h"
#include "boatchainmaker.h"

#include "http2intf.h"
#include "boatplatform_internal.h"



/*!****************************************************************************
 * @brief channel header packed
 * 
 * @details
 *   fabric has two types of headers: signature header and channel header, this
 *   function is the implemention of signature header protobuf pack.
 *   \n channel header is consist of follow fields:
 *       1. type
 *       2. version
 *       3. timestamp
 *       4. channel Id
 *       5. transaction Id
 *       6. extension
 *   channel header packed that is protobuf serialize of above fields.
 *
 * @param tx_ptr 
 *   fabric transaction structure pointer
 *
 * @param[in] txIdBin 
 *   generate transaction id in #hlfabricSignatureHeaderPacked
 *
 * @param[out] output_ptr 
 *   A structure pointer to store signature header protobuf serialize data and length. 
 *   In internal of this function, the memory of store serialize data has been alloced,
 *   caller SHOULD NOT alloc memory for this pointer again.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code. 
 *
 * @see hlfabricSignatureHeaderPacked
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT chainmakerTxHeaderPacked(const BoatChainmakerTx *tx_ptr,
													 BoatFieldVariable *output_ptr)
{
    _Common__TxHeader TxHeader = COMMON__TX_HEADER__INIT;
    BUINT32 pack_len;
	BCHAR  tx_Id_String[72 + 1];

	BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
	
	/* step-1: TxHeader packed */

	/* -chain_id */
	TxHeader.chain_id = tx_ptr->tx_header.chainId;

	/* sender */
	TxHeader.sender->org_id       = tx_ptr.tx_header.sender.orgId;
	TxHeader.sender->member_info  = tx_ptr.tx_header.sender.memberInfo;
	TxHeader.sender->is_full_cert = tx_ptr.tx_header.sender.isFullCert;

	/* -tx_type */
	TxHeader.tx_type = tx_ptr->tx_header.txType;  
	
	/* -timestramp */
	TxHeader.timestamp = tx_ptr->tx_header.timestamp;

	/* -expiration_time */
	TxHeader.expiration_time = tx_ptr->tx_header.expirationTime;
	
	/* -txID */
	memset(tx_Id_String, 0, sizeof(tx_Id_String));
	UtilityBinToHex(tx_Id_String, tx_ptr->tx_header.txId, 36, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_NO, BOAT_FALSE );
	TxHeader.tx_id                  = tx_Id_String;
	
	/* pack the channelHeader */
	pack_len = common__tx_header__get_packed_size(&TxHeader);
	output_ptr->field_ptr = BoatMalloc(pack_len);
	output_ptr->field_len = pack_len;
	common__tx_header__pack(&TxHeader, output_ptr->field_ptr);
    
	return result;
}


