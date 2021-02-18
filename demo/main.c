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

#include "boatiotsdk.h"

#if PROTOCOL_USE_ETHEREUM == 1
BOAT_RESULT BoatEthereumMainEntry(void);
#endif

#if PROTOCOL_USE_PLATONE == 1
BOAT_RESULT BoatPlatoneMainEntry(void);
#endif

/*!*****************************************************************************
@brief the main entry function

Function: main()

    This function is the main entry function.

@return
    This function returns 0.

@param[in] argc
    int of argc.
    
@param[in] argv
    Pointer of argv.

*******************************************************************************/
int main(int argc, char *argv[])
{
    BoatIotSdkInit();
#if PROTOCOL_USE_PLATONE == 1
    BoatPlatoneMainEntry();
#endif
#if PROTOCOL_USE_ETHEREUM == 1
    BoatEthereumMainEntry();
#endif

    BoatIotSdkDeInit();

    return 0;
}
