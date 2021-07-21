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
api_hlfabric.c defines the Ethereum wallet API for BoAT IoT SDK.
*/

/* self-header include */
#include "boatinternal.h"
#if PROTOCOL_USE_HLFABRIC == 1
#include "boatprotocols.h"
#include "http2intf.h"
/* protos include */
#include "peer/proposal_response.pb-c.h"
#include "orderer/cluster.pb-c.h"


/*!****************************************************************************
 * @brief Access to the specified node 
 * 
 * @param tx_ptr 
 *   Fabric transaction structure pointer
 *
 * @param nodeInfo 
*    The first element address of nodeinfo structure.
 *  
 * @param nodeMaxNum 
 *   Support maxium node number.
 *   \n For endorser node, this param is equeal with the macro #BOAT_HLFABRIC_ENDORSER_MAX_NUM,
 *   for orderer node, this param is equeal with the macro #BOAT_HLFABRIC_ORDERER_MAX_NUM.
 *
 * @return 
 *   Return \c BOAT_SUCCESS if set successed, otherwise return a failed code.
 ******************************************************************************/
__BOATSTATIC BOAT_RESULT BoatHlfabricTxExec( BoatHlfabricTx *tx_ptr, 
											 BoatHlfabricNodeInfo *nodeInfo, 
											 BUINT32 nodeMaxNum )
{
	BOAT_RESULT result   = BOAT_SUCCESS;
    boat_try_declare;
	
	if( tx_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	
	result = hlfabricProposalTransactionPacked(tx_ptr);
	if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_CRITICAL, "[%s]:packed failed.", tx_ptr->var.args.args[0]);
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricTxProposal_exception);
	}

	/* prepare http2-request argument */
	for( int i = 0; i < nodeMaxNum; i++ )
	{
		if( ( NULL != (BoatHlfabricNodeInfo *)(nodeInfo + i) ) && 
		    ( (nodeInfo + i)->nodeUrl != NULL ) && ( strlen((nodeInfo + i)->nodeUrl) > 0 ) )
		{
			tx_ptr->wallet_ptr->http2Context_ptr->nodeUrl = (nodeInfo + i)->nodeUrl;
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
			if( ( (nodeInfo + i)->hostName != NULL ) && ( strlen((nodeInfo + i)->hostName) > 0 ) )
			{
				tx_ptr->wallet_ptr->http2Context_ptr->hostName   = (nodeInfo + i)->hostName;
				tx_ptr->wallet_ptr->http2Context_ptr->tlsCAchain = &tx_ptr->wallet_ptr->tlsCAchain.ca[0];
			}
#endif
			if (tx_ptr->var.type == HLFABRIC_TYPE_PROPOSAL)
			{
				tx_ptr->wallet_ptr->http2Context_ptr->isProposal = true;
			}
			else
			{
				tx_ptr->wallet_ptr->http2Context_ptr->isProposal = false;
			}
			tx_ptr->wallet_ptr->http2Context_ptr->parseDataPtr = &tx_ptr->endorserResponse;
			http2SubmitRequest(tx_ptr->wallet_ptr->http2Context_ptr);
		}
		else
		{
			break;
		}
	}

	//! boat catch handle
	boat_catch(BoatHlfabricTxProposal_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	 	result = boat_exception;
	}
	
	return result;
}


BOAT_RESULT BoatHlfabricWalletSetAccountInfo( BoatHlfabricWallet *wallet_ptr, 
											  const BoatWalletPriKeyCtx_config prikeyCtx_config,
											  const BoatHlfabricCertInfoCfg certContent )
{
	BOAT_RESULT  result = BOAT_SUCCESS;
	boat_try_declare;

	if( wallet_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	
	/* initialization */
	memset(&wallet_ptr->account_info.prikeyCtx, 0, sizeof(BoatWalletPriKeyCtx) );
	wallet_ptr->account_info.cert.field_ptr     = NULL;
	wallet_ptr->account_info.cert.field_len     = 0;
	
	/* prikey context assignment */
	if( prikeyCtx_config.prikey_content.field_ptr != NULL )
	{
		if( BOAT_SUCCESS != BoatPort_keyCreate( &prikeyCtx_config, &wallet_ptr->account_info.prikeyCtx ) )
		{
			BoatLog( BOAT_LOG_CRITICAL, "Failed to exec BoatPort_keyCreate." );
			return BOAT_ERROR_INVALID_ARGUMENT;
		}
	}
	
	/* cert assignment */
	wallet_ptr->account_info.cert.field_ptr = BoatMalloc(certContent.length);
	if( wallet_ptr->account_info.cert.field_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetAccountInfo_exception);
	}
	memcpy(wallet_ptr->account_info.cert.field_ptr, certContent.content, certContent.length);
	wallet_ptr->account_info.cert.field_len = certContent.length;
	
	/* boat catch handle */
	boat_catch(BoatHlfabricWalletSetAccountInfo_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	 	result = boat_exception;
		/* free malloc param Deinit */
		BoatFree(wallet_ptr->account_info.cert.field_ptr);
		wallet_ptr->account_info.cert.field_len     = 0;
	}
	
	return result;
}

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
BOAT_RESULT BoatHlfabricWalletSetTlsClientInfo( BoatHlfabricWallet *wallet_ptr, 
											    const BoatWalletPriKeyCtx_config prikeyCtx_config,
												const BoatHlfabricCertInfoCfg certContent )
{	
	BOAT_RESULT  result = BOAT_SUCCESS;
	boat_try_declare;
		
	if( wallet_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	
	/* initialization */
	memset(&wallet_ptr->tlsClinet_info.prikeyCtx, 0, sizeof(BoatWalletPriKeyCtx) );
	wallet_ptr->tlsClinet_info.cert.field_ptr         = NULL;
	wallet_ptr->tlsClinet_info.cert.field_len         = 0;
	
	/* prikey context assignment */
	memcpy( &wallet_ptr->tlsClinet_info.prikeyCtx, \
		    &prikeyCtx_config.private_KeyCtx, sizeof(BoatWalletPriKeyCtx));

	/* cert assignment */
	wallet_ptr->tlsClinet_info.cert.field_ptr = BoatMalloc(certContent.length);
	if( wallet_ptr->tlsClinet_info.cert.field_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
		boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetTlsInfo_exception);
	}
	memcpy(wallet_ptr->tlsClinet_info.cert.field_ptr, certContent.content, certContent.length);
	wallet_ptr->tlsClinet_info.cert.field_len = certContent.length;
	
	/* boat catch handle */
	boat_catch(BoatHlfabricWalletSetTlsInfo_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	 	result = boat_exception;
		/* free malloc param Deinit */
		BoatFree(wallet_ptr->tlsClinet_info.cert.field_ptr);
		wallet_ptr->tlsClinet_info.cert.field_len     = 0;
	}
	
	return result;
}
#endif

#if ( BOAT_HLFABRIC_TLS_SUPPORT == 1 )
BOAT_RESULT BoatHlfabricWalletSetRootCaInfo( BoatHlfabricWallet *wallet_ptr, 
											 const BoatHlfabricCertInfoCfg *rootCaContent,
											 BUINT32 rootCaNumber )
{
	BUINT16      i = 0;

    BOAT_RESULT  result = BOAT_SUCCESS;
	boat_try_declare;
	
	if( rootCaContent == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	if( ( rootCaNumber == 0 ) || ( rootCaNumber > BOAT_HLFABRIC_ROOTCA_MAX_NUM ) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter rootCaNumber out of limit.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	
	/* initialization */
	for( i = 0; i < BOAT_HLFABRIC_ROOTCA_MAX_NUM; i++ )
	{
		wallet_ptr->tlsCAchain.ca[i].field_len = 0;
		wallet_ptr->tlsCAchain.ca[i].field_ptr = NULL;
	}
	
	/* assignment */
	for( i = 0 ; i < rootCaNumber; i++ )
	{
		/* get rootCA file size */
		wallet_ptr->tlsCAchain.ca[i].field_len  = (rootCaContent + i)->length;

		wallet_ptr->tlsCAchain.ca[i].field_ptr  = BoatMalloc( wallet_ptr->tlsCAchain.ca[i].field_len );
		if( wallet_ptr->tlsCAchain.ca[i].field_ptr == NULL )
		{
			BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
			boat_throw( BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetRootCaInfo_exception );
		}
		memset(wallet_ptr->tlsCAchain.ca[i].field_ptr, 0, wallet_ptr->tlsCAchain.ca[i].field_len);
		memcpy(wallet_ptr->tlsCAchain.ca[i].field_ptr, (rootCaContent + i)->content, wallet_ptr->tlsCAchain.ca[i].field_len);
	}
	
	/* boat catch handle */
	boat_catch(BoatHlfabricWalletSetRootCaInfo_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	 	result = boat_exception;
		/* free malloc param Deinit */	
		for( i = 0 ; i < rootCaNumber; i++)
		{
			if(wallet_ptr->tlsCAchain.ca[i].field_ptr != NULL)
			{
				BoatFree(wallet_ptr->tlsCAchain.ca[i].field_ptr);
				wallet_ptr->tlsCAchain.ca[i].field_len = 0;
			}
		}
	}
	
	return result;
}
#endif


BOAT_RESULT BoatHlfabricWalletSetNetworkInfo( BoatHlfabricWallet *wallet_ptr, 
						  const BoatHlfabricNodeInfoCfg *endorserInfo_ptr, BUINT32 endorserNumber,
						  const BoatHlfabricNodeInfoCfg *ordererInfo_ptr , BUINT32 ordererNumber )
{	
	BUINT32      stringLen;
	BUINT16      i = 0;
	
	BOAT_RESULT  result = BOAT_SUCCESS;
	boat_try_declare;
	
	/* parameter check */
	if( wallet_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr should not be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	if( ( endorserNumber == 0 ) || ( endorserNumber > BOAT_HLFABRIC_ENDORSER_MAX_NUM ) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter endorserNumber out of limit.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	if( ( ordererNumber == 0 ) || ( ordererNumber > BOAT_HLFABRIC_ORDERER_MAX_NUM ) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter ordererNumber out of limit.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	
	/* initialization */
	for( i = 0; i < BOAT_HLFABRIC_ORDERER_MAX_NUM; i++ )
	{
		wallet_ptr->network_info.orderer[i].nodeUrl   = NULL;
		wallet_ptr->network_info.orderer[i].hostName  = NULL;
	}
	for( i = 0; i < BOAT_HLFABRIC_ENDORSER_MAX_NUM; i++ )
	{
		wallet_ptr->network_info.endorser[i].nodeUrl   = NULL;
		wallet_ptr->network_info.endorser[i].hostName  = NULL;
	}
	
	/* endorser node URL assignment */
	for (i = 0; i < endorserNumber; i++)
	{
		if( ( (endorserInfo_ptr + i) != NULL ) &&
			( (stringLen = strlen( (endorserInfo_ptr + i)->nodeUrl) ) > 0 ) ) 
		{
			// stringLen check
			if( BOAT_SUCCESS != UtilityStringLenCheck( (endorserInfo_ptr + i)->nodeUrl ) )
			{
				BoatLog( BOAT_LOG_CRITICAL, "ERROR: length of endorser->nodeUrl out of limit.." );
				boat_throw( BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetNetworkInfo_exception );
			}
			
			// nodeURL assignment 
			wallet_ptr->network_info.endorser[i].nodeUrl = BoatMalloc(stringLen + 1);
			if(wallet_ptr->network_info.endorser[i].nodeUrl == NULL)
			{
				BoatLog( BOAT_LOG_CRITICAL, "BoatMalloc failed." );
				boat_throw( BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetNetworkInfo_exception );
			}
			memcpy(wallet_ptr->network_info.endorser[i].nodeUrl, (endorserInfo_ptr + i)->nodeUrl, stringLen + 1);
			
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)		
			/* endorser node hostName assignment */
			if( ( (endorserInfo_ptr + i) != NULL )  && 
				( (stringLen = strlen( (endorserInfo_ptr + i)->hostName) ) > 0 ) )
			{
				// stringLen check
				if( BOAT_SUCCESS != UtilityStringLenCheck( (endorserInfo_ptr + i)->hostName ) )
				{
					BoatLog( BOAT_LOG_CRITICAL, "ERROR: length of endorser->hostName out of limit.." );
					boat_throw( BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetNetworkInfo_exception );
				}
				
				// hostName assignment 
				wallet_ptr->network_info.endorser[i].hostName = BoatMalloc(stringLen + 1);
				if(wallet_ptr->network_info.endorser[i].hostName == NULL)
				{
					BoatLog( BOAT_LOG_CRITICAL, "BoatMalloc failed." );
					boat_throw( BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetNetworkInfo_exception );
				}
				memcpy(wallet_ptr->network_info.endorser[i].hostName, (endorserInfo_ptr + i)->hostName, stringLen + 1);
			}
#endif
		}
		else
		{
			break;
		}
	}
	wallet_ptr->network_info.endorserNum = endorserNumber;
	
	/* orderer node URL assignment */
	for( i = 0; i < ordererNumber; i++ )
	{
		if( ( (ordererInfo_ptr + i) != NULL ) && 
			( (stringLen = strlen( (ordererInfo_ptr + i)->nodeUrl ) ) > 0) )
		{
			// stringLen check
			if( BOAT_SUCCESS != UtilityStringLenCheck( (endorserInfo_ptr + i)->nodeUrl ) )
			{
				BoatLog( BOAT_LOG_CRITICAL, "ERROR: length of orderer->nodeUrl out of limit.." );
				boat_throw( BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetNetworkInfo_exception );
			}
			
			// nodeURL assignment 
			wallet_ptr->network_info.orderer[i].nodeUrl = BoatMalloc(stringLen + 1);
			if (wallet_ptr->network_info.orderer[i].nodeUrl == NULL)
			{
				BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
				boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetNetworkInfo_exception);
			}
			memcpy(wallet_ptr->network_info.orderer[i].nodeUrl, (ordererInfo_ptr + i)->nodeUrl, stringLen + 1);
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)	
			/* endorser node hostName assignment */
			if( ( (ordererInfo_ptr + i) != NULL ) && 
					((stringLen = strlen( (ordererInfo_ptr + i)->hostName) ) > 0) )
			{
				// stringLen check
				if( BOAT_SUCCESS != UtilityStringLenCheck( (endorserInfo_ptr + i)->hostName ) )
				{
					BoatLog( BOAT_LOG_CRITICAL, "ERROR: length of orderer->hostName out of limit.." );
					boat_throw( BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetNetworkInfo_exception );
				}
				
				// hostName assignment
				wallet_ptr->network_info.orderer[i].hostName = BoatMalloc(stringLen + 1);
				if (wallet_ptr->network_info.orderer[i].hostName == NULL)
				{
					BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
					boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricWalletSetNetworkInfo_exception);
				}
				memcpy(wallet_ptr->network_info.orderer[i].hostName, (ordererInfo_ptr + i)->hostName, stringLen + 1);
			}
#endif
		}
		else
		{
			break;
		}
	}
	wallet_ptr->network_info.ordererNum = ordererNumber;

	/* boat catch handle */
	boat_catch( BoatHlfabricWalletSetNetworkInfo_exception )
	{
		BoatLog( BOAT_LOG_CRITICAL, "Exception: %d", boat_exception );
	 	result = boat_exception;
		/* free malloc param Deinit */
		for( i = 0; i < wallet_ptr->network_info.ordererNum; i++ )
		{
			BoatFree(wallet_ptr->network_info.orderer[i].nodeUrl);
			BoatFree(wallet_ptr->network_info.orderer[i].hostName);
		}
		wallet_ptr->network_info.ordererNum = 0;
		for( i = 0; i < wallet_ptr->network_info.endorserNum; i++ )
		{
			BoatFree(wallet_ptr->network_info.endorser[i].nodeUrl);
			BoatFree(wallet_ptr->network_info.endorser[i].hostName);
		}
		wallet_ptr->network_info.endorserNum = 0;
	}
	return result;
}


BoatHlfabricWallet *BoatHlfabricWalletInit( const BoatHlfabricWalletConfig *config_ptr, 
								   		    BUINT32 config_size )
{
    BoatHlfabricWallet *wallet_ptr = NULL;
    BOAT_RESULT         result      = BOAT_SUCCESS;
    BUINT16      i = 0;

	if( ( config_ptr == NULL ) || ( config_size == 0 ) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "config_ptr cannot be NULL or config_size cannot be zero.");
		return NULL;
	}
	
	/* allocate wallet memory */
	wallet_ptr = BoatMalloc(sizeof(BoatHlfabricWallet));
    if( wallet_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to malloc wallet memory.");
        return NULL;
    }
	if( sizeof(BoatHlfabricWalletConfig) != config_size )
    {
        BoatLog(BOAT_LOG_CRITICAL, "Incorrect configuration size.");
        return NULL;
    }
	
	/* initialization */
	wallet_ptr->account_info.cert.field_ptr     = NULL;
	wallet_ptr->account_info.cert.field_len     = 0;
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
#if (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	wallet_ptr->tlsClinet_info.cert.field_ptr         = NULL;
	wallet_ptr->tlsClinet_info.cert.field_len         = 0;
#endif
	for( i = 0; i < BOAT_HLFABRIC_ROOTCA_MAX_NUM; i++ )
	{
		wallet_ptr->tlsCAchain.ca[i].field_len = 0;
		wallet_ptr->tlsCAchain.ca[i].field_ptr = NULL;
	}
#endif
	for( i = 0; i < BOAT_HLFABRIC_ORDERER_MAX_NUM; i++ )
	{
		wallet_ptr->network_info.orderer[i].nodeUrl   = NULL;
		wallet_ptr->network_info.orderer[i].hostName  = NULL;
	}
	for( i = 0; i < BOAT_HLFABRIC_ENDORSER_MAX_NUM; i++ )
	{
		wallet_ptr->network_info.endorser[i].nodeUrl   = NULL;
		wallet_ptr->network_info.endorser[i].hostName  = NULL;
	}

	wallet_ptr->http2Context_ptr               = NULL;

	/* account_info assignment */
	result += BoatHlfabricWalletSetAccountInfo( wallet_ptr,  config_ptr->accountPriKey_config, 
	                                            config_ptr->accountCertContent );
	/* tlsClinet_info assignment */
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) 
#if	(BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	result += BoatHlfabricWalletSetTlsClientInfo( wallet_ptr,  config_ptr->tlsPriKey_config,  
												  config_ptr->tlsClientCertContent );
#endif
	/* tlsRootCa_info assignment */
	BoatHlfabricWalletSetRootCaInfo(wallet_ptr, config_ptr->rootCaContent, config_ptr->rootCaNumber);
#endif
	/* network_info assignment */
	result += BoatHlfabricWalletSetNetworkInfo( wallet_ptr, config_ptr->endorser, config_ptr->endorserNumber,
												config_ptr->orderer, config_ptr->ordererNumber );
	/* http2Context_ptr assignment */
	wallet_ptr->http2Context_ptr = http2Init();
	
	if(result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to set accountInfo|TlsUInfo|networkInfo.");
		return NULL;
	}
	
    return wallet_ptr;
}


void BoatHlfabricWalletDeInit( BoatHlfabricWallet *wallet_ptr )
{
    BUINT16      i = 0;

    if( NULL == wallet_ptr )
    {
        BoatLog(BOAT_LOG_CRITICAL, "wallet_ptr needn't DeInit: wallet_ptr is NULL.");
        return;
    }

    /* account_info DeInit */
    BoatFree(wallet_ptr->account_info.cert.field_ptr);
    wallet_ptr->account_info.cert.field_len     = 0;

    /* tlsClinet_info DeInit */
#if (BOAT_HLFABRIC_TLS_SUPPORT == 1)
#if (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
    BoatFree(wallet_ptr->tlsClinet_info.cert.field_ptr);
    wallet_ptr->tlsClinet_info.cert.field_len     = 0;
#endif /* #if (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1) */
    // for c99, free(NULL) will return directly, so here
    // use BOAT_HLFABRIC_ROOTCA_MAX_NUM as cyclic maximum is acceptable.
    for( i = 0; i < BOAT_HLFABRIC_ROOTCA_MAX_NUM; i++ )
    {
        BoatFree( wallet_ptr->tlsCAchain.ca[i].field_ptr );
        wallet_ptr->tlsCAchain.ca[i].field_len = 0;
    }
#endif /* #if (BOAT_HLFABRIC_TLS_SUPPORT == 1) */

    /* network_info DeInit */
    for( i = 0; i < wallet_ptr->network_info.ordererNum; i++ )
    {
        BoatFree(wallet_ptr->network_info.orderer[i].nodeUrl);
        BoatFree(wallet_ptr->network_info.orderer[i].hostName);
    }
    wallet_ptr->network_info.ordererNum = 0;
    for( i = 0; i < wallet_ptr->network_info.endorserNum; i++ )
    {
        BoatFree(wallet_ptr->network_info.endorser[i].nodeUrl);
        BoatFree(wallet_ptr->network_info.endorser[i].hostName);
    }
    wallet_ptr->network_info.endorserNum = 0;

    /* http2Context DeInit */
    http2DeInit(wallet_ptr->http2Context_ptr);

    /* wallet_ptr DeInit */
    BoatFree(wallet_ptr);

    /* set NULL after free completed */
    wallet_ptr = NULL; 
}


BOAT_RESULT BoatHlfabricTxInit( BoatHlfabricTx *tx_ptr, 
							    const BoatHlfabricWallet *wallet_ptr,
							    const BCHAR *chaincodeId_path_str,
							    const BCHAR *chaincodeId_name_str,
							    const BCHAR *chaincodeId_version_str,
							    const BCHAR *channelId_str,
						        const BCHAR *orgName_str )
{
	BUINT32       stringLen;
	BCHAR *paramSrcList[5];
	BCHAR **paramDstList[5];
	BUINT16      i = 0;
    BOAT_RESULT  result = BOAT_SUCCESS;

    boat_try_declare;

	if( ( tx_ptr == NULL ) || (wallet_ptr == NULL) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments 'tx_ptr' or 'wallet_ptr' cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
	}
	
	/* tx_ptr instance reset */
	/* ----->tx_ptr->wallet_ptr reset */
	tx_ptr->wallet_ptr          = NULL;
	/* ----->tx_ptr->var reset */
	tx_ptr->var.type            = HLFABRIC_TYPE_IDLE;
	tx_ptr->var.timestamp.sec   = 0;
	tx_ptr->var.timestamp.nanos = 0;
	tx_ptr->var.nonce.field_len = 0;
	for( i = 0; i < 24; i++ )
	{
		tx_ptr->var.nonce.field[i] = 0;
	}
	tx_ptr->var.chaincodeId.name    = NULL;
	tx_ptr->var.chaincodeId.path    = NULL;
	tx_ptr->var.chaincodeId.version = NULL;
	tx_ptr->var.args.nArgs       = 0;
	for( i = 0; i < BOAT_HLFABRIC_ARGS_MAX_NUM; i++ )
	{
		tx_ptr->var.args.args[i] = NULL;
	}
	tx_ptr->var.channelId           = NULL;
	tx_ptr->var.orgName             = NULL;
	/* ----->tx_ptr->endorserResponse reset */
	tx_ptr->endorserResponse.responseCount = 0;
	for( i = 0; i < BOAT_HLFABRIC_ENDORSER_MAX_NUM; i++)
	{
		tx_ptr->endorserResponse.response[i].contentPtr          = NULL;
		tx_ptr->endorserResponse.response[i].responseType        = HLFABRIC_TYPE_IDLE;
		tx_ptr->endorserResponse.response[i].payload.field_len   = 0;
		tx_ptr->endorserResponse.response[i].payload.field_ptr   = NULL;
		tx_ptr->endorserResponse.response[i].endorser.field_len  = 0;
		tx_ptr->endorserResponse.response[i].endorser.field_ptr  = NULL;
		tx_ptr->endorserResponse.response[i].signature.field_len = 0;
		tx_ptr->endorserResponse.response[i].signature.field_ptr = NULL;
	}
	
	/* wallet assignment */
	tx_ptr->wallet_ptr = (BoatHlfabricWallet *)wallet_ptr;
	
	/* paramList assignment */
	paramSrcList[0] = (BCHAR *)chaincodeId_path_str;
	paramSrcList[1] = (BCHAR *)chaincodeId_name_str;
	paramSrcList[2] = (BCHAR *)chaincodeId_version_str;
	paramSrcList[3] = (BCHAR *)channelId_str;
	paramSrcList[4] = (BCHAR *)orgName_str;
	paramDstList[0] = &tx_ptr->var.chaincodeId.path;
	paramDstList[1] = &tx_ptr->var.chaincodeId.name;
	paramDstList[2] = &tx_ptr->var.chaincodeId.version;
	paramDstList[3] = &tx_ptr->var.channelId;
	paramDstList[4] = &tx_ptr->var.orgName;
	
	for( i = 0; i < sizeof(paramSrcList)/sizeof(paramSrcList[0]); i++)
	{
		if( ( paramSrcList[i] != NULL ) && ( (stringLen = strlen(paramSrcList[i])) > 0 ) )
		{
			*paramDstList[i] = BoatMalloc(stringLen + 1);
			if( *paramDstList[i] == NULL )
			{
				BoatLog(BOAT_LOG_CRITICAL, "BoatMalloc failed.");
				boat_throw(BOAT_ERROR_OUT_OF_MEMORY, BoatHlfabricTxInit_exception);
			}
			memcpy( *paramDstList[i], paramSrcList[i], stringLen + 1 );
		}
	}

	/* boat catch handle */
	boat_catch(BoatHlfabricTxInit_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	 	result = boat_exception;
		/* free malloc param Deinit */
		for(int i = 0; i < sizeof(paramDstList)/sizeof(paramDstList[0]); i++)
		{
			BoatFree(paramDstList[i]);
		}
	}

	return result;
}


void BoatHlfabricTxDeInit( BoatHlfabricTx *tx_ptr )
{
	if( tx_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Tx argument cannot be nULL.");
		return;
	}
	
	/* wallet_ptr DeInit */
	//DO NOTHING:wallet deinit will be implemented in BoatIotSdkDeInit(...)
	
	/* var DeInit */
	/* -----var.type */
	//DO NOTHING
	/* -----var.timestamp */
	//DO NOTHING
	/* -----var.nonce */
	//DO NOTHING
	/* -----var.chaincodeId */
	BoatFree(tx_ptr->var.chaincodeId.path);
	tx_ptr->var.chaincodeId.path    = NULL;
	BoatFree(tx_ptr->var.chaincodeId.name);
	tx_ptr->var.chaincodeId.name    = NULL;
	BoatFree(tx_ptr->var.chaincodeId.version);
	tx_ptr->var.chaincodeId.version = NULL;
	/* -----var.args */
	tx_ptr->var.args.nArgs = 0;
	for( int i = 0; i < BOAT_HLFABRIC_ARGS_MAX_NUM; i++ )
	{
		tx_ptr->var.args.args[i] = NULL;
	}
	/* -----var.channelId */
	BoatFree( tx_ptr->var.channelId );
	tx_ptr->var.channelId = NULL;
	/* -----var.orgName */
	BoatFree( tx_ptr->var.orgName );
	tx_ptr->var.orgName = NULL;
	
	/* endorserResponse DeInit */
	//DO NOTHING:THIS FIELD HAD FREE AFTER EVERY TRANSCATION COMPLETED
}


BOAT_RESULT BoatHlfabricTxSetTimestamp( BoatHlfabricTx *tx_ptr, 
										const BUINT64 sec, 
										const BUINT64 nanos )
{
   if( tx_ptr == NULL )
   {
	   BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
	   return BOAT_ERROR_INVALID_ARGUMENT;
   }
   
   tx_ptr->var.timestamp.sec   = sec;
   tx_ptr->var.timestamp.nanos = nanos;
   
   return BOAT_SUCCESS;
}


BOAT_RESULT BoatHlfabricTxSetArgs( BoatHlfabricTx *tx_ptr, 
								   const BCHAR *arg1, 
								   ... )
{
	va_list      ap;
	BCHAR        *args   = NULL;
	BOAT_RESULT result   = BOAT_SUCCESS;
   
	if( tx_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}

	tx_ptr->var.args.nArgs = 0;
   
	va_start(ap, arg1);

	tx_ptr->var.args.args[tx_ptr->var.args.nArgs] = (BCHAR *)arg1;
	tx_ptr->var.args.nArgs++;
	
	while( NULL != (args = va_arg(ap, BCHAR*)) )
	{
		tx_ptr->var.args.args[tx_ptr->var.args.nArgs] = (BCHAR *)args;
		tx_ptr->var.args.nArgs++;
		if( tx_ptr->var.args.nArgs > BOAT_HLFABRIC_ARGS_MAX_NUM )
		{
			result = BOAT_ERROR_OUT_OF_MEMORY;
			break;
		}
	}
	va_end(ap);

	return result;
}

BOAT_RESULT BoatHlfabricTxEvaluate( BoatHlfabricTx *tx_ptr )
{
	BoatHlfabricNodeInfo urlTmp[2]    = {{NULL,NULL}, {NULL,NULL}};
	BOAT_RESULT result  = BOAT_SUCCESS;
    boat_try_declare;
	
	if( tx_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	if( tx_ptr->var.args.args[0] == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments args[0] cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	

	BoatLog(BOAT_LOG_NORMAL, "Evaluate will execute...");
	
	/* submit query */
	tx_ptr->var.type = HLFABRIC_TYPE_PROPOSAL;
	urlTmp[0]        = tx_ptr->wallet_ptr->network_info.endorser[0];
	result           = BoatHlfabricTxExec(tx_ptr, &urlTmp[0], BOAT_HLFABRIC_ENDORSER_MAX_NUM);
	BoatLog_hexasciidump(BOAT_LOG_NORMAL, "query result", 
							tx_ptr->endorserResponse.response[0].payload.field_ptr,
							tx_ptr->endorserResponse.response[0].payload.field_len);
	
	/* free the unpacked response data */
	for( int i = 0; i < tx_ptr->endorserResponse.responseCount; i++ )
	{
		if( tx_ptr->endorserResponse.response[i].responseType == HLFABRIC_TYPE_PROPOSAL )
		{
			protos__proposal_response__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
	}
	tx_ptr->endorserResponse.responseCount = 0;
	
	/* boat catch handle */
	boat_catch(BoatHlfabricTxEvaluate_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	 	result = boat_exception;
	}

	return result;
}


BOAT_RESULT BoatHlfabricTxSubmit( BoatHlfabricTx *tx_ptr )
{
	BoatHlfabricNodeInfo urlTmp[2]    = {{NULL,NULL}, {NULL,NULL}};
	BOAT_RESULT result  = BOAT_SUCCESS;
    boat_try_declare;
	
	if( tx_ptr == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	if( tx_ptr->var.args.args[0] == NULL )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Arguments args[0] cannot be NULL.");
		return BOAT_ERROR_INVALID_ARGUMENT;
	}
	
	BoatLog(BOAT_LOG_NORMAL, "Submit will execute...");
	
	/* invoke-step1: submit proposal to endorer */
	tx_ptr->var.type = HLFABRIC_TYPE_PROPOSAL;
	result           = BoatHlfabricTxExec(tx_ptr, &tx_ptr->wallet_ptr->network_info.endorser[0], BOAT_HLFABRIC_ENDORSER_MAX_NUM);
	
	/* invoke-step2: submit transaction to orderer */
	tx_ptr->var.type = HLFABRIC_TYPE_TRANSACTION;
	result           = BoatHlfabricTxExec(tx_ptr, &tx_ptr->wallet_ptr->network_info.orderer[0], BOAT_HLFABRIC_ORDERER_MAX_NUM);

	/* free the unpacked response data */
	for( int i = 0; i < tx_ptr->endorserResponse.responseCount; i++ )
	{
		if( tx_ptr->endorserResponse.response[i].responseType == HLFABRIC_TYPE_PROPOSAL )
		{
			protos__proposal_response__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
		else
		{
			orderer__submit_response__free_unpacked(tx_ptr->endorserResponse.response[i].contentPtr, NULL);
		}
	}
	tx_ptr->endorserResponse.responseCount = 0;
	
	/* boat catch handle */
	boat_catch(BoatHlfabricTxSubmit_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
	 	result = boat_exception;
	}

	return result;
}
#endif /* end of PROTOCOL_USE_HLFABRIC */
