/*
 * @Description: 
 * @Author: aitos
 * @Date: 2022-09-04 11:28:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-06 20:18:07
 */
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
#include "boatconfig.h"
#include "boatiotsdk.h"
#include "boatlog.h"
#include "account_ethereum.h"

/**
 * test node url
 */

const BCHAR * demoUrl = "http://192.168.132.190:7545";

// #if defined(USE_ONETIME_WALLET) 
__BOATSTATIC BOAT_RESULT createOnetimeAccount()
{
    BSINT32 index;
    BoatEthAccountConfig account_config = {0};

    account_config.chain_id             = 1;
    account_config.eip155_compatibility = BOAT_FALSE;
    strncpy(account_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

	/* create ethereum wallet */
    index = BoatEthAccountCreate( &account_config, BOAT_STORE_TYPE_RAM);
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    
    return BOAT_SUCCESS;
}



// #endif

// #if defined(USE_CREATE_PERSIST_WALLET)
__BOATSTATIC BOAT_RESULT createPersistAccount(void)
{
    BSINT32 index;
    BoatEthAccountConfig account_config = {0};

    account_config.chain_id             = 1;
    account_config.eip155_compatibility = BOAT_FALSE;
    strncpy(account_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

	/* create ethereum wallet */
    index = BoatEthAccountCreate( &account_config, BOAT_STORE_TYPE_FLASH);
    if (index < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }

    return BOAT_SUCCESS;
}
// #endif

int main(int argc, char *argv[])
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatIotAccountContext accountList;
    boat_try_declare;
    BoatLog(BOAT_LOG_NORMAL,"begin to test ");
    result = createOnetimeAccount();
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
    result = createPersistAccount();
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = createPersistAccount();
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = createPersistAccount();
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet creat failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    result = BoATEthAccountDelete(2);

    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet delete failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    result = BoATEth_GetAccountList(&accountList);
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "get wallet list failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    for (size_t i = 0; i < accountList.accountNum; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL , "account[%d].index = %d ",i,accountList.accounts[i].accountIndex);
        BoatLog(BOAT_LOG_NORMAL , "account[%d].chainID  = %d ",i,accountList.accounts[i].chain_id);
        BoatLog(BOAT_LOG_NORMAL , "account[%d].eip55 = %d ",i,accountList.accounts[i].eip155_compatibility);
        BoatLog(BOAT_LOG_NORMAL , "account[%d].url = %s ",i,accountList.accounts[i].node_url_str);
    }
    BoATEth_FreeAccountContext(accountList);
    
        result = BoATEthAccountDelete(0);

    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "wallet delete failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}


    result = BoATEth_GetAccountList(&accountList);
    if(result != BOAT_SUCCESS)
	{
		 BoatLog(BOAT_LOG_CRITICAL, "get wallet list failed: %d.", result);
        boat_throw(result, ethereum_storeread_demo_catch);
	}

    for (size_t i = 0; i < accountList.accountNum; i++)
    {
        /* code */
        BoatLog(BOAT_LOG_NORMAL , "account[%d].index = %d ",i,accountList.accounts[i].accountIndex);
        BoatLog(BOAT_LOG_NORMAL , "account[%d].chainID  = %d ",i,accountList.accounts[i].chain_id);
        BoatLog(BOAT_LOG_NORMAL , "account[%d].eip55 = %d ",i,accountList.accounts[i].eip155_compatibility);
        BoatLog(BOAT_LOG_NORMAL , "account[%d].url = %s ",i,accountList.accounts[i].node_url_str);
    }
    BoATEth_FreeAccountContext(accountList);



    BoatLog(BOAT_LOG_CRITICAL, "account test success: ");
    boat_catch(ethereum_storeread_demo_catch)
    {
    }
}