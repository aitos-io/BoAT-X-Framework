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

/*!@brief Define common types and include external header files that are internally used in Boatwallet

@file
boatinternal.h defines common types and include external header files that are internally used in Boatwallet
*/

#ifndef __BOATINTERNAL_H__
#define __BOATINTERNAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "boattypes.h"
#include "boatkeypair.h"
#include "boatplatform_internal.h"
#include "boatiotsdk.h"

//! Define a resonable max length in bytes for general checking for memory
//! allocation related validation. The lower end the embedded system is, the
//! smaller the length is.
#define BOAT_REASONABLE_MAX_LEN                  8192u

#endif
