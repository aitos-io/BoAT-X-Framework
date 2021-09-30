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
#if defined(__unix__) || defined(__unix) || defined(unix)
	/* for timestamp source */
	#define _POSIX_C_SOURCE 200809L
	#include <time.h>
#endif
#include "boatconfig.h"
#include "boatiotsdk.h"

const BCHAR * chainmaker_user_key = "-----BEGIN EC PRIVATE KEY-----\n"
									"MHcCAQEEIByphjR4auvodMAWeaWsDXuADlGVi0ODAZtOh7tcIr2hoAoGCCqGSM49\n"
									"AwEHoUQDQgAE56xayRx0/a8KEXPxRfiSzYgJ/sE4tVeI/ZbjpiUX9m0TCJX7W/VH\n"
									"dm6WeJLOdCDuLLNvjGTyt8LLyqyubJI5AA==\n"
									"-----END EC PRIVATE KEY-----\n";


const BCHAR * chainmaker_user_cert = "-----BEGIN CERTIFICATE-----\n"
									 "MIICijCCAi+gAwIBAgIDBS9vMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
									 "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
									 "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
									 "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIwMTIwODA2NTM0M1oXDTI1\n"
									 "MTIwNzA2NTM0M1owgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
									 "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
									 "MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
									 "LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE56xayRx0\n"
									 "/a8KEXPxRfiSzYgJ/sE4tVeI/ZbjpiUX9m0TCJX7W/VHdm6WeJLOdCDuLLNvjGTy\n"
									 "t8LLyqyubJI5AKN7MHkwDgYDVR0PAQH/BAQDAgGmMA8GA1UdJQQIMAYGBFUdJQAw\n"
									 "KQYDVR0OBCIEIMjAiM2eMzlQ9HzV9ePW69rfUiRZVT2pDBOMqM4WVJSAMCsGA1Ud\n"
									 "IwQkMCKAIDUkP3EcubfENS6TH3DFczH5dAnC2eD73+wcUF/bEIlnMAoGCCqGSM49\n"
									 "BAMCA0kAMEYCIQCWUHL0xisjQoW+o6VV12pBXIRJgdeUeAu2EIjptSg2GAIhAIxK\n"
									 "LXpHIBFxIkmWlxUaanCojPSZhzEbd+8LRrmhEO8n\n"
									 "-----END CERTIFICATE-----\n";


const BCHAR * chainmaker_user_sign_key  = "-----BEGIN EC PRIVATE KEY-----\n"
										  "MHcCAQEEIM+5FVFBOHIUeUDH2wOkC4Fn6FnMsLHz0SOTreQKz1bDoAoGCCqGSM49\n"
										  "AwEHoUQDQgAED88H7OwVPitBf2NjCH2ZRM0L9k3csW9nq5qFbyAOSsM+ppi0qrFV\n"
										  "+mcQcGEP4H/VTMxWZCmnVZ7a5d+JQt42OQ==\n"
										  "-----END EC PRIVATE KEY-----\n";


const BCHAR * chainmaker_user_sign_cert = "-----BEGIN CERTIFICATE-----\n"
										  "MIIChzCCAi6gAwIBAgIDDk05MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
										  "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
										  "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
										  "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIwMTIwODA2NTM0M1oXDTI1\n"
										  "MTIwNzA2NTM0M1owgZAxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
										  "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
										  "MQ8wDQYDVQQLEwZjbGllbnQxKzApBgNVBAMTImNsaWVudDEudGxzLnd4LW9yZzEu\n"
										  "Y2hhaW5tYWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQPzwfs7BU+\n"
										  "K0F/Y2MIfZlEzQv2Tdyxb2ermoVvIA5Kwz6mmLSqsVX6ZxBwYQ/gf9VMzFZkKadV\n"
										  "ntrl34lC3jY5o3sweTAOBgNVHQ8BAf8EBAMCAaYwDwYDVR0lBAgwBgYEVR0lADAp\n"
										  "BgNVHQ4EIgQgnb+Rban1rokuCZHYKzDhNm/nqnam4YdneDyfo8CSHzswKwYDVR0j\n"
										  "BCQwIoAgNSQ/cRy5t8Q1LpMfcMVzMfl0CcLZ4Pvf7BxQX9sQiWcwCgYIKoZIzj0E\n"
										  "AwIDRwAwRAIgGYzsN0+mqMOawe0T6eicuW1mlwQCu2Qt0Y8IDbJjuQYCIB85HFCd\n"
										  "UQ908a46G/eQ8YMeQApBkVhFDBN82soA3jQA\n"
										  "-----END CERTIFICATE-----\n";

const BCHAR * chainmaker_root_ca_cert =  "-----BEGIN CERTIFICATE-----\n"
										 "MIICrzCCAlWgAwIBAgIDDsPeMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
										 "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
										 "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
										 "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIwMTIwODA2NTM0M1oXDTMw\n"
										 "MTIwNjA2NTM0M1owgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
										 "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
										 "MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
										 "YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAT7NyTIKcjtUVeMn29b\n"
										 "GKeEmwbefZ7g9Uk5GROl+o4k7fiIKNuty1rQHLQUvAvkpxqtlmOpPOZ0Qziu6Hw6\n"
										 "hi19o4GnMIGkMA4GA1UdDwEB/wQEAwIBpjAPBgNVHSUECDAGBgRVHSUAMA8GA1Ud\n"
										 "EwEB/wQFMAMBAf8wKQYDVR0OBCIEIDUkP3EcubfENS6TH3DFczH5dAnC2eD73+wc\n"
										 "UF/bEIlnMEUGA1UdEQQ+MDyCDmNoYWlubWFrZXIub3Jngglsb2NhbGhvc3SCGWNh\n"
										 "Lnd4LW9yZzEuY2hhaW5tYWtlci5vcmeHBH8AAAEwCgYIKoZIzj0EAwIDSAAwRQIg\n"
										 "ar8CSuLl7pA4Iy6ytAMhR0kzy0WWVSElc+koVY6pF5sCIQCDs+vTD/9V1azmbDXX\n"
										 "bjoWeEfXbFJp2X/or9f4UIvMgg==\n"
										 "-----END CERTIFICATE-----\n";


const BCHAR *chainmaker_consensus_url       = "127.0.0.1:12301";
const BCHAR *chainmaker_consensus_hostName  = "chainmaker.org";

BoatChainmakerWalletConfig *g_chaninmaker_wallet_ptr;

void chainmaker_set_node_info(BoatChainmakerNode node)
{
	int length; 
	node.addr = BoatMalloc(strlen(chainmaker_consensus_url)+1);
	memcpy(node.addr, chainmaker_consensus_url, strlen(chainmaker_consensus_url)+1);

	node.tlsHostName = BoatMalloc(strlen(chainmaker_consensus_hostName)+1);
	memcpy(node.tlsHostName, chainmaker_consensus_hostName, strlen(chainmaker_consensus_hostName)+1);

	node.useTLS = TRUE;
	node.connCnt = 10;
}

__BOATSTATIC BOAT_RESULT chainmakerWalletPrepare(void)
{
	BOAT_RESULT index;

	BoatChainmakerWalletConfig wallet_config = {0};
	//set clinet private key context
	wallet_config.user_pri_key_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.user_pri_key_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.user_pri_key_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.user_pri_key_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_user_key;
	wallet_config.user_pri_key_config.prikey_content.field_len = strlen(chainmaker_user_key) + 1; //length contain terminator

	//set clinet cert context
	wallet_config.user_cert_content.length = strlen(chainmaker_user_cert) + 1;
	memcpy(wallet_config.user_cert_content.content, chainmaker_user_cert, wallet_config.user_cert_content.length);

	//set tls key context
	wallet_config.user_sign_pri_key_config.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	wallet_config.user_sign_pri_key_config.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
	wallet_config.user_sign_pri_key_config.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
	wallet_config.user_sign_pri_key_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_user_sign_key;
	wallet_config.user_sign_pri_key_config.prikey_content.field_len = strlen(chainmaker_user_sign_key) + 1; //length contain terminator

	//set tls cert context
	wallet_config.user_sign_cert_content.length = strlen(chainmaker_user_sign_cert) + 1;
	memcpy(wallet_config.user_sign_cert_content.content, chainmaker_user_sign_cert, wallet_config.user_sign_cert_content.length);

	if (strlen(chainmaker_root_ca_cert) < BOAT_HLFABRIC_CERT_MAX_LEN)
	{
		memcpy(wallet_config.roo_ca_cert, chainmaker_root_ca_cert, strlen(chainmaker_root_ca_cert));	
	}
	chainmaker_set_node_info(wallet_config.node_info);
	
	/* create fabric wallet */
	index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatChainmakerWalletConfig));

	if(index == BOAT_ERROR)
	{
		//BoatLog( BOAT_LOG_CRITICAL, "fabricWalletPrepare failed." );
		return BOAT_ERROR;
	}
	g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(index);
	return BOAT_SUCCESS;
}


int main(int argc, char *argv[])
{
	BOAT_RESULT       result  = BOAT_SUCCESS;
	
	/* step-1: Boat SDK initialization */
    BoatIotSdkInit();
	
	/* step-2: prepare wallet */
	result = chainmakerWalletPrepare();
	if (result != BOAT_SUCCESS)
	{
		//BoatLog(BOAT_LOG_CRITICAL, "fabricWalletPrepare failed.");
		return -1;
	}
    return 0;
}
