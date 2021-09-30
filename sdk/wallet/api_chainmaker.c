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

/*!@brief Ethereum wallet API for BoAT IoT SDK

@file
api_chainmaker.c defines the Ethereum wallet API for BoAT IoT SDK.
*/

/* self-header include */
#include "boatinternal.h"


BoatchainmakerGenerateTxRequest

void BoatCHainmakerTxRequestDeInit(BoatChainmkaerTxRequest *tx_ptr)
{
	if (tx_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Tx argument cannot be nULL.");
		return;
	}
	
	BoatFree();
}


//cacl hash
BOAT_RESULT BoaTChainmakerCalcUnsignedTxRequestBytes(BoatChainmkaerTxRequest *tx_ptr) {
	BOAT_RESULT result = BOAT_SUCCESS;
	return result;	
}

BOAT_RESULT BoatChainmakerGenerateTxRequest(BoatChainmkaerTxRequest *tx_ptr, BUINT32             txType, BoatChainmakerTransactPayload* payload)						 
{
	BOAT_RESULT result = BOAT_SUCCESS;

	return result;
}

BOAT_RESULT BoatChainmakerTxSetTimestamp(BUINT64 *time_stamp)
{
	if (time_stamp == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

#if defined(__unix__) || defined(__unix) || defined(unix)
	struct timespec txTimestamp;
	clock_gettime(CLOCK_REALTIME, &txTimestamp);
	*time_stamp = txTimestamp.tv_sec;
	
#else
	long int timesec = 0;
	time(&timesec);
	*time_stamp = timesec;
#endif

	return BOAT_SUCCESS;
}




