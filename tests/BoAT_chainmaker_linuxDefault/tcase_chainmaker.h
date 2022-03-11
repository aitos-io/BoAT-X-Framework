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
#include "boatwallet.h"
#include "protocolapi/api_chainmaker.h"

#define CERT_PRIKEY_LEN 1024

extern char chainmaker_sign_key_buf[];
extern char chainmaker_sign_cert_buf[];
extern char chainmaker_ca_cert_buf[];

#endif
