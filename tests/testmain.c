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



BOAT_RESULT TestPreCondition(void);
BOAT_RESULT TestPostCondition(void);

// Case declaration
BOAT_RESULT Case_20_RlpMain(void);

BOAT_RESULT Case_10_EthFunMain(void);
BOAT_RESULT Case_11_EthCovMain(void);

BOAT_RESULT Case_15_PlatONEMain(void);

BOAT_RESULT Case_16_PlatONECovMain(void);

int main(int argc, char *argv[])
{

    BOAT_RESULT case_result=BOAT_SUCCESS;
    TestPreCondition();
    
    //case_result += Case_20_RlpMain();

    case_result += Case_10_EthFunMain();
    case_result += Case_11_EthCovMain();

    case_result += Case_15_PlatONEMain();
    case_result += Case_16_PlatONECovMain();

    BoatLog(BOAT_LOG_NORMAL, "case_result: %d.", case_result);
    TestPostCondition();
    
    return 0;
}
