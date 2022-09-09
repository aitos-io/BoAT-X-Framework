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

#ifndef __TCASE_FABRIC_H__
#define __TCASE_FABRIC_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "check.h"

#include "boatiotsdk.h"
#include "protocolapi/api_hlfabric_discovery.h"
// #include "protocolapi/api_hlfabric.h"

#define CERT_PRIKEY_LEN 1024

#define file_client_key     "../../../tests/BoAT_fabric_linuxDefault/cert_key/client.key"
#define file_client_cert    "../../../tests/BoAT_fabric_linuxDefault/cert_key/client.crt"
#define file_org1_tls_cert  "../../../tests/BoAT_fabric_linuxDefault/cert_key/org1-tls-cert.crt"
#define file_org2_tls_cert  "../../../tests/BoAT_fabric_linuxDefault/cert_key/org2-tls-cert.crt"
#define file_order_tls_cert  "../../../tests/BoAT_fabric_linuxDefault/cert_key/order-tls-cert.crt"
#define file_client_key_false  "../../../tests/BoAT_fabric_linuxDefault/cert_key/client_err.key"
#define file_client_cert_false  "../../../tests/BoAT_fabric_linuxDefault/cert_key/client_err.crt"

#define ORDER_PORT          ":7050"
#define ORG1_PEER0_PORT     ":7051"
#define ORG1_PEER1_PORT     ":8051"
#define ORG2_PEER0_PORT     ":9051"
#define ORG2_PEER1_PORT     ":10051"

#define ORDER_HOSTNAME      "orderer.example.com"
#define ORG1_PEER0_HOSTNAME "peer0.org1.example.com"
#define ORG1_PEER1_HOSTNAME "peer1.org1.example.com"
#define ORG2_PEER0_HOSTNAME "peer0.org2.example.com"
#define ORG2_PEER1_HOSTNAME "peer1.org2.example.com"



extern char fabric_client_cert_buf[];
extern char fabric_client_key_buf[];
extern char fabric_org1_tls_cert_buf[];
extern char fabric_org2_tls_cert_buf[];
extern char fabric_order_tls_cert_buf[];
extern char fabric_client_key_false_buf[];
extern char fabric_client_cert_false_buf[];



#endif