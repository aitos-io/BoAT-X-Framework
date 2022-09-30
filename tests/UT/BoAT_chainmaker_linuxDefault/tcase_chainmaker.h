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
#ifndef __TCASE_CHAINMAKER_H__
#define __TCASE_CHAINMAKER_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "check.h"
#include "boatlog.h"
#include "boattypes.h"
#include "protocolapi/api_chainmaker.h"

#define CERT_PRIKEY_LEN 1024

typedef enum {
    WALLET_ONEITEM = 0,
    WALLET_PERSIST = 1

}WALLET_TYPE;

extern char chainmaker_client_sign_prikey[];
extern char chainmaker_client_sign_cert[];
extern char chainmaker_client_tls_cert[];

extern BOAT_RESULT chainmaker_create_network(WALLET_TYPE wallet_type, BUINT8* network_index_ptr);
extern BOAT_RESULT chainmaker_create_keypair(WALLET_TYPE wallet_type, BUINT8* keypair_index_ptr);
#endif
