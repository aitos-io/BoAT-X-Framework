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

/*!@brief Error codes

@file
boaterrcode.h defines error codes.
*/

#ifndef __BOATERRCODE_H__
#define __BOATERRCODE_H__

#define BOAT_SUCCESS                             (0)
#define BOAT_ERROR                               (-1)
#define BOAT_ERROR_INVALID_ARGUMENT              (-100)
#define BOAT_ERROR_OUT_OF_MEMORY                 (-101)
#define BOAT_ERROR_RLP_ENCODING_FAIL             (-102)
#define BOAT_ERROR_EXT_MODULE_OPERATION_FAIL     (-103)
#define BOAT_ERROR_JSON_PARSE_FAIL               (-104)
#define BOAT_ERROR_RPC_FAILED                    (-105)
#define BOAT_ERROR_TX_PENDING                    (-106)
#define BOAT_ERROR_GEN_RANDOM_FAILED             (-107)
#define BOAT_ERROR_GEN_HASH_FAILED               (-108)
#define BOAT_ERROR_GEN_SIGNATURE_FAILED          (-109)
#define BOAT_ERROR_BAD_FILE_DESCRIPTOR           (-110)


#endif
