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
 #include "tcase_platon.h"

BOAT_RESULT check_platon_wallet(BoatPlatONWallet *wallet_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    result = strncmp(wallet_ptr->account_info.prikeyCtx.extra_data.value, platon_private_key_buf, strlen(platon_private_key_buf));
    if (result != 0) 
    {
        return result;
    }
    
    result = strncmp(wallet_ptr->network_info.node_url_ptr, TEST_PLATON_NODE_URL, strlen(TEST_PLATON_NODE_URL));
    if (result != 0)
    {
        return result;
    }

    return BOAT_SUCCESS;
}