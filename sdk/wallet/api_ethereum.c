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

/*!@brief Ethereum wallet API for BoAT IoT SDK

@file
boatethwallet.c defines the Ethereum wallet API for BoAT IoT SDK.
*/

#include "boatinternal.h"

#include "web3intf.h"
#include "boatprotocols.h"
#include "rpcintf.h"

#include "randgenerator.h"
#include "bignum.h"
#include "cJSON.h"

#if BOAT_USE_OPENSSL != 0
#include <openssl/evp.h>
#include <openssl/aes.h>
#endif




/******************************************************************************
@brief Initialize Boat Ethereum Wallet

Function: BoatEthWalletInit()

    This function sets up context of Boat Ethereum Wallet based on given
    configuration.
    
    DO NOT call this function directly. Instead call BoatWalletCreate() with an Ethereum
    wallet configuration like:

    @verbatim
    BoatEthWalletConfig eth_config = {...};\n
    BSINT32 wallet_index;\n
    wallet_index = BoatWalletCreate( BOAT_PROTOCOL_ETHEREUM,\n
                                     &eth_config,\n
                                     sizeof(BoatEthWalletConfig)\n
                                   );
    @endverbatim

    BoatEthWalletInit() MUST be called before any use of Boat Ethereum Wallet.
    BoatEthWalletDeInit() MUST be called after use of Boat Ethereum Wallet.
    

@see BoatEthWalletDeInit() BoatWalletCreate()

@return
    This function returns instance pointer of BoatEthWallet if initialization is successful.\n
    Otherwise it returns NULL.
    

@param[in] config_ptr
    Pointer to Ethereum wallet configuration.

@param[in] config_size
    Size (in byte) of Ethereum wallet configuration.

*******************************************************************************/
BoatEthWallet * BoatEthWalletInit(const BoatEthWalletConfig *config_ptr, BUINT32 config_size)
{
    BoatEthWallet *wallet_ptr;
    BOAT_RESULT result;

    if( config_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return NULL;
    }

    if( sizeof(BoatEthWalletConfig) != config_size )
    {
        BoatLog(BOAT_LOG_NORMAL, "Incorrect configuration size.");
        return NULL;
    }
    
    wallet_ptr = BoatMalloc(sizeof(BoatEthWallet));

    if( wallet_ptr == NULL )
    {
        return NULL;
    }
        
    // Init Web3 interface
    wallet_ptr->web3intf_context_ptr = web3_init();

    if( wallet_ptr->web3intf_context_ptr == NULL )
    {
        BoatFree(wallet_ptr);
        return NULL;
    }

    // Set EIP-155 Compatibility to TRUE by default
    BoatEthWalletSetEIP155Comp(wallet_ptr, config_ptr->eip155_compatibility);

    // Configure private key
    result = BoatEthWalletSetPrivkey(wallet_ptr, config_ptr->priv_key_array);
    if( result != BOAT_SUCCESS)
    {
        web3_deinit(wallet_ptr->web3intf_context_ptr);
        BoatFree(wallet_ptr);
        return NULL;
    }

    // Configure Chain ID
    BoatEthWalletSetChainId(wallet_ptr, config_ptr->chain_id);
    
    // Configure node URL string
    wallet_ptr->network_info.node_url_ptr = NULL;
    result = BoatEthWalletSetNodeUrl(wallet_ptr, config_ptr->node_url_str);
    if( result != BOAT_SUCCESS)
    {
        web3_deinit(wallet_ptr->web3intf_context_ptr);
        BoatFree(wallet_ptr);
        return NULL;
    }
    
    return wallet_ptr;
}


/*!*****************************************************************************
@brief De-initialize Boat Ethereum Wallet

Function: BoatEthWalletDeInit()

    This function de-initialize context of Boat Ethereum Wallet.

    BoatEthWalletInit() MUST be called before any use of Boat Ethereum Wallet.
    BoatEthWalletDeInit() MUST be called after use of Boat Ethereum Wallet.
    

@see BoatEthWalletInit()

@return This function doesn't return any thing.

@param[in] wallet_ptr
    Wallet context pointer
*******************************************************************************/
void BoatEthWalletDeInit(BoatEthWallet *wallet_ptr)
{

    if( wallet_ptr != NULL )
    {

        // Destroy private key in wallet memory
        memset(wallet_ptr->account_info.priv_key_array, 0x00, 32);

        if( wallet_ptr->network_info.node_url_ptr != NULL )
        {
            BoatFree(wallet_ptr->network_info.node_url_ptr);
            wallet_ptr->network_info.node_url_ptr = NULL;
        }

        web3_deinit(wallet_ptr->web3intf_context_ptr);

        BoatFree(wallet_ptr);
    }
    
    return;
}


/******************************************************************************
@brief Set BoatWallet: URL of blockchain node

Function: BoatEthWalletSetNodeUrl()

    This function sets the URL of the blockchain node to connect to.

    A URL is composed of protocol, IP address/name and port, in a form:
    http://a.b.com:8545

@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.

@param[in] wallet_ptr
    Wallet context pointer.    

@param[in] node_url_ptr
    A string indicating the URL of blockchain node to connect to.
        
*******************************************************************************/
BOAT_RESULT BoatEthWalletSetNodeUrl(BoatEthWallet *wallet_ptr, const BCHAR *node_url_ptr)
{
    BOAT_RESULT result;
    
    if( wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    // Set Node URL
    if( node_url_ptr != NULL )
    {
        if( wallet_ptr->network_info.node_url_ptr != NULL )
        {
            BoatFree(wallet_ptr->network_info.node_url_ptr);
        }

        // +1 for NULL Terminator
        wallet_ptr->network_info.node_url_ptr = BoatMalloc(strlen(node_url_ptr)+1);
        
        if( wallet_ptr->network_info.node_url_ptr != NULL )
        {
            strcpy(wallet_ptr->network_info.node_url_ptr, node_url_ptr);
            result = BOAT_SUCCESS;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to allocate memory for Node URL string.");
            result = BOAT_ERROR;
        }
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        result = BOAT_ERROR;
    }

    return result;
}


/******************************************************************************
@brief Set BoatWallet: EIP-155 Compatibility

Function: BoatEthWalletSetEIP155Comp()

    This function sets if the network supports EIP-155.

    If the network supports EIP-155, set it to BOAT_TRUE.
    Otherwise set it to BOAT_FALSE.

@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.

@param[in] wallet_ptr
    Wallet context pointer.    

@param[in] eip155_compatibility
    BOAT_TRUE if the network supports EIP-155. Otherwise BOAT_FALSE.
        
*******************************************************************************/
BOAT_RESULT BoatEthWalletSetEIP155Comp(BoatEthWallet *wallet_ptr, BUINT8 eip155_compatibility)
{
    if( wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    // Set EIP-155 Compatibility
    wallet_ptr->network_info.eip155_compatibility = eip155_compatibility;
    return BOAT_SUCCESS;
}


/******************************************************************************
@brief Set BoatWallet: Chain ID

Function: BoatEthWalletSetChainId()

    This function sets the chain ID of the network.

    If the network supports EIP-155, chain ID is part of the transaction
    message to sign.

    If the network doesn't support EIP-155, chain ID is ignored.

@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.

@param[in] wallet_ptr
    Wallet context pointer.    

@param[in] chain_id
    Chain ID of the blockchain network to use.
        
*******************************************************************************/
BOAT_RESULT BoatEthWalletSetChainId(BoatEthWallet *wallet_ptr, BUINT32 chain_id)
{
    if( wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    // Set Chain ID
    wallet_ptr->network_info.chain_id = chain_id;
    return BOAT_SUCCESS;
}


/******************************************************************************
@brief Set BoatWallet: Private Key

Function: BoatEthWalletSetPrivkey()

    This function sets the private key of the wallet account.

    A private key is 256 bit. If it's treated as a UINT256 in bigendian, the
    valid private key value for Ethereum is [1, n-1], where n is
    0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141.

    This function will call BoatEthWalletCheckPrivkey() to check the validity of
    the private key.

    The public key is automatically derived from the private key and the
    address of the account is calculated from the public key.

    In case co-sign is used, this function set the half key shard of the
    private key. The public key is calculated by co-sign algorithm with the
    co-sign server.

    NOTE: Be very careful to PROTECT the private key.

@see
    BoatEthWalletCheckPrivkey()

@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.

@param[in] wallet_ptr
    Wallet context pointer.    

@param[in] priv_key_array
    Private key to use.
        
*******************************************************************************/
BOAT_RESULT BoatEthWalletSetPrivkey(BoatEthWallet *wallet_ptr, const BUINT8 priv_key_array[32])
{
    BUINT8 pub_key[65];
    BUINT8 pub_key_digest[32];
    BOAT_RESULT result;

    if( wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }


    if( priv_key_array == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Private key cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
    result = BoatEthWalletCheckPrivkey(priv_key_array);

    if( result != BOAT_SUCCESS )
    {
        BoatLog(BOAT_LOG_NORMAL, "Private key is not valid.");
        return BOAT_ERROR;
    }
    
    
    // Set private key and calculate public key as well as address
    // PRIVATE KEY MUST BE SET BEFORE SETTING NONCE AND GASPRICE

    memcpy(wallet_ptr->account_info.priv_key_array, priv_key_array, 32);
    
    // Calculate address from private key;
    ecdsa_get_public_key65(
                            &secp256k1,
                            wallet_ptr->account_info.priv_key_array,
                            pub_key);

    // pub_key[] is a 65-byte array with pub_key[0] being 0x04 SECG prefix followed by 64-byte public key
    // Thus skip pub_key[0]
    memcpy(wallet_ptr->account_info.pub_key_array, &pub_key[1], 64);

    keccak_256(&pub_key[1], 64, pub_key_digest);

    memcpy(wallet_ptr->account_info.address, pub_key_digest+12, 20); // Address is the least significant 20 bytes of public key's hash

    return BOAT_SUCCESS;
}


/******************************************************************************
@brief Generate Private Key

Function: BoatEthWalletGeneratePrivkey()

    This function generates the private key of the wallet account and calls
    BoatEthWalletSetPrivkey() to save it into wallet account.

    A private key is a 256 bit random number up to a value slightly smaller than
    all bits being 1. See BoatEthWalletCheckPrivkey() for the details.

    In case co-sign is used, this is the half key shard.

    NOTE: Be very careful to PROTECT the private key.

@see
    BoatEthWalletSetPrivkey() BoatEthWalletCheckPrivkey()

@return
    This function returns BOAT_SUCCESS if private key is successfully generated.\n
    Otherwise it returns one of the error codes.

@param[out] priv_key_array
    A 32-byte buffer to hold the generated private key.
        
*******************************************************************************/
BOAT_RESULT BoatEthWalletGeneratePrivkey(BOAT_OUT BUINT8 priv_key_array[32])
{
    BUINT32 key_try_count;
    BOAT_RESULT result;

    if( priv_key_array == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Buffer to hold private key cannot be NULL.");
        return BOAT_ERROR;
    }
    
    // Try at most 100 times to find a random number fits for Ethereum private key
    for( key_try_count = 0; key_try_count < 100; key_try_count++ )
    {
    
        result = random_stream(priv_key_array, 32);

        if( result != BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to generate private key.");
            break;
        }        

        result = BoatEthWalletCheckPrivkey(priv_key_array);

        if( result == BOAT_SUCCESS )
        {
            break;
        }            
    }

    return result;
    
}


/******************************************************************************
@brief Check validity of given private key

Function: BoatEthWalletCheckPrivkey()

    This function checks the validity of the private key.

    A private key is 256 bit. If it's treated as a UINT256 in bigendian, the
    valid private key value for Ethereum is [1, n-1], where n is
    0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141


@return
    This function returns BOAT_SUCCESS if the key is valid.\n
    Otherwise it returns one of the error codes.
    

@param[in] priv_key_array
    Private key to check.
        
*******************************************************************************/
BOAT_RESULT BoatEthWalletCheckPrivkey(const BUINT8 priv_key_array[32])
{
    bignum256 priv_key_bn256;
    bignum256 priv_key_max_bn256;

    // Valid private key value (as a UINT256) for Ethereum is [1, n-1], where n is
    // 0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141
    const BUINT32 priv_key_max_u256[8] = {0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6,
                                         0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

    if( priv_key_array == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Private key cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    // Check whether private_key_array is in valid range
    
    // Convert private key from UINT256 to Bignum256 format
    bn_read_le(priv_key_array, &priv_key_bn256);

    if( bn_is_zero(&priv_key_bn256) != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "Private key cannot be all zeros. Fail to set private key.");
        return BOAT_ERROR;
    }

    // Convert priv_key_max_u256 from UINT256 to Bignum256 format
    bn_read_le((const uint8_t *)priv_key_max_u256, &priv_key_max_bn256);

    // Check whether priv_key_bn256 is less than priv_key_max_bn256
    if( bn_is_less(&priv_key_bn256, &priv_key_max_bn256) == 0 )
    {
        BoatLog(BOAT_LOG_NORMAL, "Private key does not conform to Ethereum. Fail to set private key.");
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;

}


/******************************************************************************
@brief Get Balance of the wallet account

Function: BoatEthWalletGetBalance()

    This function gets the balance of the wallet account from network.

    If the account is not available (i.e. the address of the wallet account
    never appears as a recipient in any successful non-zero-value transaction),
    the balance returned is 0. It's not possible to distinguish an unavailable
    account from a zero-balance account.


@return
    This function returns the complete json message of the RPC method response.\n
    If any error occurs, it returns NULL.
    

@param[in] wallet_ptr
    Wallet context pointer.

@param[in] alt_address_str
    A string representing which address to get balance from.
    If NULL, get balance of the wallet's owner address.\n
    Otherwise, get balance of the specified altered address, in HEX format like\n
    "0x19c91A4649654265823512a457D2c16981bB64F5".

*******************************************************************************/
BCHAR * BoatEthWalletGetBalance(BoatEthWallet *wallet_ptr, BCHAR *alt_address_str)
{
    BUINT8 alt_address[BOAT_ETH_ADDRESS_SIZE];   // Binary altered address converted from alt_address_str
    BUINT8 *address_ptr;     // Point to an address in binary format, either wallet
                             // owner's or the one converted from alt_address_str
    BCHAR address_str[43];   // Address in string format, converted from address_ptr
    
    Param_eth_getBalance param_eth_getBalance;
    BCHAR *tx_balance_str;

    if( wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return NULL;
    }

    if( alt_address_str == NULL )
    {
        // Query Wallet's Owner address
        // PRIVATE KEY MUST BE SET BEFORE GETTING BALANCE, BECAUSE GETTING BALANCE FROM
        // NETWORK NEEDS ETHEREUM ADDRESS, WHICH IS COMPUTED FROM KEY

        address_ptr = wallet_ptr->account_info.address;
    }
    else
    {
        // Query specified altered address

            UtilityHex2Bin(
                    alt_address,
                    BOAT_ETH_ADDRESS_SIZE,
                    alt_address_str,
                    TRIMBIN_TRIM_NO,
                    BOAT_TRUE
                  );

        address_ptr = alt_address;
    }
    

    // Get balance from network
    // Return value of web3_eth_getBalance() is balance in wei
    
    UtilityBin2Hex(
        address_str,
        address_ptr,
        BOAT_ETH_ADDRESS_SIZE,
        BIN2HEX_LEFTTRIM_UNFMTDATA,
        BIN2HEX_PREFIX_0x_YES,
        BOAT_FALSE
        );
        
    param_eth_getBalance.address_str = address_str;
    param_eth_getBalance.block_num_str = "latest";


    
    tx_balance_str = web3_eth_getBalance(wallet_ptr->web3intf_context_ptr,
                                    wallet_ptr->network_info.node_url_ptr,
                                    &param_eth_getBalance);

    if( tx_balance_str == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Fail to get balance from network.");
        return NULL;
    }

    return tx_balance_str;
}







/*!*****************************************************************************
@brief Prase RPC method RESPONSE

   This function Prase "result" segment.
   If "result" object is string, this function will returns contents of "result" . 
   If "result" object is still json object, the parameter named "child_name" will actived,
   if "child_name" object is string, this function will returns contents of "child_name"; 
   if "child_name" object is other types, his function will prompt "un-implemention yet".
   For other types of "result" this function is not support yet.

@param[in] json_string
	 The json to be parsed.

@param[in] child_name
	 if "result" item is json object, this param will actived.

@param[out] result_out
	 The buffer to store prase result.
	 Caller can allocate memory for this param, or can initial it with {NULL, 0},
	 this function will expand the memory if it too small to store prase result.
	 
@return
    This function returns BOAT_SUCCESS if prase successed. Otherwise
    it returns an error code.
*******************************************************************************/
BOAT_RESULT BoatEthPraseRpcResponseResult( const BCHAR * json_string, 
										   const BCHAR * child_name, 
										   BoatFieldVariable *result_out )
{
	return web3_parse_json_result(json_string, child_name, result_out);
}



/******************************************************************************
@brief Initialize Ethereum Transaction

Function: BoatEthTxInit()

    This function initialize an Ethereum transaction.
    
    

@see BoatEthTxSetGasPrice() BoatEthTxSetGasLimit() BoatEthTxSetRecipient()

@return
    This function returns BOAT_SUCCESS if initialization is successful.\n
    Otherwise it returns one of the error codes.
    

@param[in] wallet_ptr
    The wallet pointer that this transaction is combined with.

@param[in] tx_ptr
    Pointer a transaction object.

@param[in] is_sync_tx
    For a stateful transaction, specify BOAT_TRUE to wait until the transaction is mined.\n
    Specifiy BOAT_FALSE to allow multiple transactions to be sent continuously in a short time.\n
    For a state-less contract call, this option is ignored.

@param[in] gasprice_str
    A HEX string representing the gas price (unit: wei) to be used in the transaction.\n
    Set <gasprice> = NULL to obtain gas price from network.\n
    BoatEthTxSetGasPrice() can later be called to modify the gas price at any\n
    time before the transaction is executed.

@param[in] gaslimit_str
    A HEX string representing the gas limit to be used in the transaction.\n
    BoatEthTxSetGasLimit() can later be called to modify the gas limit at any\n
    time before the transaction is executed.

@param[in] recipient_str
    A HEX string representing the recipient address, in HEX format like\n
    "0x19c91A4649654265823512a457D2c16981bB64F5".\n
    BoatEthTxSetRecipient() can later be called to modify the recipient at any\n
    time before the transaction is executed.

*******************************************************************************/
BOAT_RESULT BoatEthTxInit(BoatEthWallet *wallet_ptr,
                          BoatEthTx *tx_ptr,
                          BBOOL is_sync_tx,
                          BCHAR * gasprice_str,
                          BCHAR * gaslimit_str,
                          BCHAR *recipient_str)
{
    BOAT_RESULT result;

    if( wallet_ptr == NULL || tx_ptr == NULL || recipient_str == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    tx_ptr->wallet_ptr = wallet_ptr;

    memset(&tx_ptr->rawtx_fields, 0x00, sizeof(tx_ptr->rawtx_fields));


    // Set synchronous transaction flag
    tx_ptr->is_sync_tx = is_sync_tx;
    
    // Initialize gasprice

    BoatFieldMax32B gasprice;
    // Either manually set the gas price or get the price from network
    if( gasprice_str != NULL )
    {
        // Manually
        gasprice.field_len =
        UtilityHex2Bin(
                        gasprice.field,
                        32,
                        gasprice_str,
                        TRIMBIN_LEFTTRIM,
                        BOAT_TRUE
                      );

        result = BoatEthTxSetGasPrice(tx_ptr, &gasprice);
    }
    else
    {
        // To use the price obtained from network, simply call BoatTxSetGasPrice(NULL)
        result = BoatEthTxSetGasPrice(tx_ptr, NULL);
    }

    if( result != BOAT_SUCCESS )
    {
        return BOAT_ERROR;
    }



    // Initialize gaslimit
    BoatFieldMax32B gaslimit;
    
    gaslimit.field_len =
    UtilityHex2Bin(
                    gaslimit.field,
                    32,
                    gaslimit_str,
                    TRIMBIN_LEFTTRIM,
                    BOAT_TRUE
                  );

    result = BoatEthTxSetGasLimit(tx_ptr, &gaslimit);

    if( result != BOAT_SUCCESS )
    {
        return BOAT_ERROR;
    }


    // Initialize recipient
    BUINT8 recipient[BOAT_ETH_ADDRESS_SIZE];
    BUINT32 converted_len;
    converted_len = UtilityHex2Bin(
                        recipient,
                        BOAT_ETH_ADDRESS_SIZE,
                        recipient_str,
                        TRIMBIN_TRIM_NO,
                        BOAT_TRUE
                      );

    if( converted_len == 0 )
    {
        return BOAT_ERROR;
    }

    result = BoatEthTxSetRecipient(tx_ptr, recipient);

    if( result != BOAT_SUCCESS )
    {
        return BOAT_ERROR;
    }

    // Initialize value = 0
    result = BoatEthTxSetValue(tx_ptr, NULL);

    if( result != BOAT_SUCCESS )
    {
        return BOAT_ERROR;
    }
    
    return BOAT_SUCCESS;
}


/******************************************************************************
@brief Set Transaction Parameter: Transaction Nonce

Function: BoatEthTxSetNonce()

    This function sets the nonce to the transaction count of the account
    obtained from network.

    This function can be called after BoatEthTxInit() has been called.

@see BoatEthTxInit()

@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.
    

@param[in] tx_ptr
    The pointer to the transaction.

@param[in] nonce
    The nonce to use in the transaction.\n
    If BOAT_ETH_NONCE_AUTO (0xFFFFFFFFFFFFFFFF) is specified, the nonce is\n
    obtained from network.
        
*******************************************************************************/
BOAT_RESULT BoatEthTxSetNonce(BoatEthTx *tx_ptr, BUINT64 nonce)
{
    BCHAR account_address_str[43];
    Param_eth_getTransactionCount param_eth_getTransactionCount;
    BCHAR *tx_count_str;
	BOAT_RESULT result = BOAT_SUCCESS;

    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    // PRIVATE KEY MUST BE SET BEFORE SETTING NONCE, BECAUSE GETTING NONCE FROM
    // NETWORK NEEDS ETHEREUM ADDRESS, WHICH IS COMPUTED FROM KEY


    // Get transaction count from network
    // Return value of web3_eth_getTransactionCount() is transaction count
    
    UtilityBin2Hex(
        account_address_str,
        tx_ptr->wallet_ptr->account_info.address,
        BOAT_ETH_ADDRESS_SIZE,
        BIN2HEX_LEFTTRIM_UNFMTDATA,
        BIN2HEX_PREFIX_0x_YES,
        BOAT_FALSE
        );
        
    param_eth_getTransactionCount.address_str = account_address_str;
    param_eth_getTransactionCount.block_num_str = "latest";

    if (BOAT_ETH_NONCE_AUTO == nonce)
    {
        tx_count_str = web3_eth_getTransactionCount(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                        tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                        &param_eth_getTransactionCount);

		result = BoatEthPraseRpcResponseResult( tx_count_str, "", 
												&tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
        if( result != BOAT_SUCCESS )
        { 
            BoatLog(BOAT_LOG_CRITICAL, "Fail to get transaction count from network.");
            return result;
        }
		
        // Set nonce from transaction count
        tx_ptr->rawtx_fields.nonce.field_len =
        UtilityHex2Bin(
                        tx_ptr->rawtx_fields.nonce.field,
                        32,
                        (BCHAR*)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
                        TRIMBIN_LEFTTRIM,
                        BOAT_TRUE
                      );
    }
    else
    {
        tx_ptr->rawtx_fields.nonce.field_len = 
                UtilityUint64ToBigend(
                            tx_ptr->rawtx_fields.nonce.field,
                            nonce,
                            TRIMBIN_LEFTTRIM
                            );
    }

    BoatLog(BOAT_LOG_NORMAL, "tx_ptr->rawtx_fields.nonce.field_len = %d", tx_ptr->rawtx_fields.nonce.field_len);
    BoatLog(BOAT_LOG_NORMAL, "tx_ptr->rawtx_fields.nonce.field = %02x%02x", tx_ptr->rawtx_fields.nonce.field[0], tx_ptr->rawtx_fields.nonce.field[1]);

    return BOAT_SUCCESS;
}


/******************************************************************************
@brief Set Transaction Parameter: GasPrice

Function: BoatEthTxSetGasPrice()

    This function sets the gas price of the transaction.


@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.


@param[in] tx_ptr
    Pointer to the transaction structure.

@param[in] gas_price_ptr
    The gas price in wei.\n
    If <gas_price_ptr> is NULL, the gas price obtained from network is used.
        
*******************************************************************************/
BOAT_RESULT BoatEthTxSetGasPrice(BoatEthTx *tx_ptr, BoatFieldMax32B *gas_price_ptr)
{
    BCHAR *gas_price_from_net_str;
    BOAT_RESULT result = BOAT_SUCCESS;

    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    // If gas price is specified, use it
    // Otherwise use gas price obtained from network
    if( gas_price_ptr != NULL )
    {
        memcpy(&tx_ptr->rawtx_fields.gasprice,
                gas_price_ptr,
                sizeof(BoatFieldMax32B));
    }
    else
    {
        // Get current gas price from network
        // Return value of web3_eth_gasPrice is in wei
        
        gas_price_from_net_str = web3_eth_gasPrice(tx_ptr->wallet_ptr->web3intf_context_ptr, tx_ptr->wallet_ptr->network_info.node_url_ptr);
		result = BoatEthPraseRpcResponseResult( gas_price_from_net_str, "", 
												&tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
        if( result != BOAT_SUCCESS )
        {
            BoatLog(BOAT_LOG_NORMAL, "Fail to get gasPrice from network.");
            result = BOAT_ERROR;
        }
        else
        {
            // Set transaction gasPrice with the one got from network
            tx_ptr->rawtx_fields.gasprice.field_len =
            UtilityHex2Bin(
                            tx_ptr->rawtx_fields.gasprice.field,
                            32,
                            (BCHAR*)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr,
                            TRIMBIN_LEFTTRIM,
                            BOAT_TRUE
                          );

            BoatLog(BOAT_LOG_VERBOSE, "Use gasPrice from network: %s wei.", gas_price_from_net_str);
        }
    }
    
    return result;
}


/******************************************************************************
@brief Set Transaction Parameter: GasLimit

Function: BoatEthTxSetGasLimit()

    This function sets the gas limit of the transaction.


@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.

@param[in] tx_ptr
    Pointer to the transaction structure.    

@param[in] gas_limit_ptr
    The gas limit is generally a fixed value on the corresponding chain.
        
*******************************************************************************/
BOAT_RESULT BoatEthTxSetGasLimit(BoatEthTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
{
    if( tx_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    // Set gasLimit
    if( gas_limit_ptr != NULL )
    {
        memcpy(&tx_ptr->rawtx_fields.gaslimit,
                gas_limit_ptr,
                sizeof(BoatFieldMax32B));

        return BOAT_SUCCESS;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
}


/******************************************************************************
@brief Set Transaction Parameter: Recipient Address

Function: BoatEthTxSetRecipient()

    This function sets the address of the transaction recipient.


@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.

@param[in] tx_ptr
    Pointer to the transaction structure.   

@param[in] address
    The address of the recipient
        
*******************************************************************************/
BOAT_RESULT BoatEthTxSetRecipient(BoatEthTx *tx_ptr, BUINT8 address[BOAT_ETH_ADDRESS_SIZE])
{
    if( tx_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
    // Set recipient's address
    memcpy(&tx_ptr->rawtx_fields.recipient,
            address,
            BOAT_ETH_ADDRESS_SIZE);

    return BOAT_SUCCESS;    
}


/******************************************************************************
@brief Set Transaction Parameter: Value

Function: BoatEthTxSetValue()

    This function sets the value of the transaction.


@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.

@param[in] tx_ptr
    Pointer to the transaction structure.    

@param[in] value_ptr
    The value of the transaction.\n
    If <value_ptr> is NULL, it's treated as no value being transfered.
        
*******************************************************************************/
BOAT_RESULT BoatEthTxSetValue(BoatEthTx *tx_ptr, BoatFieldMax32B *value_ptr)
{
    if( tx_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
    // Set value
    if( value_ptr != NULL )
    {
        memcpy(&tx_ptr->rawtx_fields.value,
                value_ptr,
                sizeof(BoatFieldMax32B));

    }
    else
    {
        // If value_ptr is NULL, value is treated as 0.
        // NOTE: value.field_len == 0 has the same effect as
        //       value.field_len == 1 && value.field[0] == 0x00 for RLP encoding
        tx_ptr->rawtx_fields.value.field_len = 0;
    }

    return BOAT_SUCCESS;
    
}


/******************************************************************************
@brief Set Transaction Parameter: Data

Function: BoatEthTxSetData()

    This function sets the data of the transaction.


@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.

@param[in] tx_ptr
    Pointer to the transaction structure.     

@param[in] data_ptr
    The data of the transaction. Note that data_ptr->field_ptr itself is only\n
    a pointer without any associated storage by default. The caller must\n
    allocate appropriate memory for it.\n
    If <data_ptr> is NULL, it's treated as no data being transfered.
        
*******************************************************************************/
BOAT_RESULT BoatEthTxSetData(BoatEthTx *tx_ptr, BoatFieldVariable *data_ptr)
{
    if( tx_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
    // Set data
    if( data_ptr != NULL )
    {
        // NOTE: tx_ptr->rawtx_fields.data.field_ptr is a pointer
        //       The caller must make sure the storage it points to is available
        //       until the transaction is sent.
        memcpy(&tx_ptr->rawtx_fields.data,
                data_ptr,
                sizeof(BoatFieldVariable));

    }
    else
    {
        // If data_ptr is NULL, value is treated as 0.
        // NOTE: data.field_len == 0 has the same effect as
        //       data.field_len == 1 && data.field_ptr[0] == 0x00 for RLP encoding
        tx_ptr->rawtx_fields.data.field_len = 0;
    }

    return BOAT_SUCCESS;
    
}


/******************************************************************************
@brief Sign and send a transaction. Also call a stateful contract function.

Function: BoatEthTxSend()

    This function sign and set a transaction.

    BoatWalletSetXXX() and BoatTxSetXXX() functions must be properly called
    before call this function.

    A transaction whose recipient may be an EOA address or a contract address.
    In latter case it's usually a contract function call.

    This function invokes the eth_sendRawTransaction RPC method.
    eth_sendRawTransaction method only applies the transaction and returns a
    transaction hash. The transaction is not verified (got mined) until the
    nodes in the network get into consensus about the transaction. This
    function will invoke eth_getTransactionReceipt method to wait for the
    transaction being mined or timeout.

    If the transaction is a contract function call, the caller cannot get its
    return value because the transaction is asynchronously executed. It's a
    good practice to save the return value in a state variable and use
    BoatEthCallContractFunc() to call a "read" contract function that could read
    and return the state variable.


    NOTE:

    Any contract function that may change the state of the contract shall
    be called in a transaction way. "state" is the "global variable" used in a
    contract.

    Any contract function that doesn't change the state of the contract can
    be called either in a transaction way or by BoatEthCallContractFunc(), which
    invokes the eth_call RPC method. However the former will consume gas and
    latter doesn't consume gas.

@see BoatEthCallContractFunc()
    
    
@return
    This function returns BOAT_SUCCESS if setting is successful.\n
    Otherwise it returns one of the error codes.
    

@param[in] tx_ptr
    Pointer to the transaction structure.
*******************************************************************************/
BOAT_RESULT BoatEthTxSend(BoatEthTx *tx_ptr)
{
    BOAT_RESULT result;
    
    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }

    if( tx_ptr->is_sync_tx == BOAT_FALSE )
    {
        result = EthSendRawtx(tx_ptr);
    }
    else
    {
        result = EthSendRawtxWithReceipt(tx_ptr);
    }
    
    return result;
}


/******************************************************************************
@brief Call a state-less contract function

Function: BoatEthCallContractFunc()

    This function calls contract function that doesn't change the state of the
    contract. "state" is the "global variable" used in a contract.

    This function invokes the eth_call RPC method. eth_call method requests the
    blockchain node to execute the function without affecting the block chain.
    The execution runs only on the requested node thus it can return immediately
    after the execution. This function synchronously return the return value
    of the eth_call method, which is the return value of the contract function.

    To call contract function that may change the state, use BoatEthTxSend()
    instead.

    If call a contract function that may change the state with
    BoatEthCallContractFunc(), the function will be executed and return a value,
    but none of the states will change.

@see BoatEthTxSend()

@return
    This function returns the complete json message of the RPC method response.\n
    If any error occurs, it returns NULL.
    

@param[in] tx_ptr
    Transaction pointer

@param[in] func_proto_str
    A string representing the prototype of the called function.\n
    Note: uint is treated as uint256.\n
    e.g.: for the contract function:\n
        function readListByIndex(uint index) public view returns (bytes32 event_)\n
    its prototype is "readListByIndex(uint256)"

@param[in] func_param_ptr
    A byte stream containing the parameters to pass to the function.\n
    The layout conforms to Ethereum ABI: https://solidity.readthedocs.io/en/develop/abi-spec.html\n
    If <func_param_ptr> is NULL, this function doesn't take any parameter.

@param[in] func_param_len
    Length of <func_param_ptr> in byte.
        
*******************************************************************************/
BCHAR * BoatEthCallContractFunc(
                    BoatEthTx *tx_ptr,
                    BCHAR *func_proto_str,
                    BUINT8 *func_param_ptr,
                    BUINT32 func_param_len)
{

    BUINT8 function_selector[32];

    // +4 for function selector, *2 for bin to HEX, + 3 for "0x" prefix and NULL terminator
    BCHAR data_str[(func_param_len + 4)*2 + 3]; // Compiler MUST support C99 to allow variable-size local array
 
    Param_eth_call param_eth_call;
    BCHAR *retval_str;

    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return NULL;
    }
    
    if( func_param_ptr == NULL && func_param_len != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return NULL;
    }

    BCHAR recipient_hexstr[BOAT_ETH_ADDRESS_SIZE*2+3];
    
    UtilityBin2Hex(
                recipient_hexstr,
                tx_ptr->rawtx_fields.recipient,
                BOAT_ETH_ADDRESS_SIZE,
                BIN2HEX_LEFTTRIM_UNFMTDATA,
                BIN2HEX_PREFIX_0x_YES,
                BOAT_FALSE
                );
    param_eth_call.to = recipient_hexstr;

    // Function call consumes zero gas but gasLimit and gasPrice must be specified.
    param_eth_call.gas = "0x1fffff";
    param_eth_call.gasPrice = "0x8250de00";

    keccak_256((BUINT8*)func_proto_str, strlen(func_proto_str), function_selector);

    

    // Set function selector
    UtilityBin2Hex(
            data_str,
            function_selector,
            4,
            BIN2HEX_LEFTTRIM_UNFMTDATA,
            BIN2HEX_PREFIX_0x_YES,
            BOAT_FALSE);

    // Set function parameters
    UtilityBin2Hex(
            data_str+10,        // "0x12345678" function selector prefixed
            func_param_ptr,
            func_param_len,
            BIN2HEX_LEFTTRIM_UNFMTDATA,
            BIN2HEX_PREFIX_0x_NO,
            BOAT_FALSE);

    
    param_eth_call.data = data_str;

    param_eth_call.block_num_str = "latest";

    retval_str = web3_eth_call( tx_ptr->wallet_ptr->web3intf_context_ptr,
                                tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                &param_eth_call);

    
    return retval_str;

}


/******************************************************************************
@brief Transfer Ether to spceified recipient

Function: BoatEthTransfer()

    This function transfer Ether from the wallet's owner account to the specified
    recipient account.

    Before calling this function, all necessary wallet fields such as private key,
    node URL and etc. must be set correctly.


@return
    This function returns BOAT_SUCCESS if transfer is successful.\n
    Otherwise it returns one of the error codes.
    

@param[in] tx_ptr
    Transaction pointer.

@param[in] value_hex_str
    A string representing the value (Unit: wei) to transfer, in HEX format like\n
    "0x89AB3C".\n
    Note that decimal value is not accepted. If a decimal value such as "1234"\n
    is specified, it's treated as "0x1234".
        
*******************************************************************************/
BOAT_RESULT BoatEthTransfer(BoatEthTx *tx_ptr, BCHAR * value_hex_str)
{
    BoatFieldMax32B value;
    BOAT_RESULT result;
   
    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL|| value_hex_str == NULL )
    {
        BoatLog(BOAT_LOG_NORMAL, "Argument cannot be NULL.");
        return BOAT_ERROR_INVALID_ARGUMENT;
    }
    
    // Set nonce
    result = BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO);
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;
    

    // Set value

    value.field_len =
    UtilityHex2Bin(
                    value.field,
                    32,
                    value_hex_str,
                    TRIMBIN_LEFTTRIM,
                    BOAT_TRUE
                  );

    result =BoatEthTxSetValue(tx_ptr, &value);

    if( result != BOAT_SUCCESS ) return BOAT_ERROR;


    // Set data
    result = BoatEthTxSetData(tx_ptr, NULL);
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;

    
    // Perform the transaction
    // NOTE: Field v,r,s are calculated automatically
    result = BoatEthTxSend(tx_ptr);
    if( result != BOAT_SUCCESS ) return BOAT_ERROR;

    return BOAT_SUCCESS;
}


/******************************************************************************
@brief Wait for a transaction being mined.

Function: BoatEthGetTransactionReceipt()

    This function polls receipt by transaction hash and waits for the transaction
    being mined.
    
    Be sure the transaction object pointed by <tx_ptr> has been called with
    EthSendRawtxis().
    

@return
    This function returns BOAT_SUCCESS if successful. Otherwise it returns one\n
    of the error codes.
    

@param[in] tx_ptr
        A pointer to the context of the transaction.

*******************************************************************************/
BOAT_RESULT BoatEthGetTransactionReceipt(BoatEthTx *tx_ptr)
{
    BCHAR tx_hash_str[67];
    BCHAR *tx_status_str;
    Param_eth_getTransactionReceipt param_eth_getTransactionReceipt;
    BSINT32 tx_mined_timeout;

    BOAT_RESULT result = BOAT_SUCCESS;

    if( tx_ptr == NULL || tx_ptr->wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        return BOAT_ERROR;
    }

    UtilityBin2Hex(
                tx_hash_str,
                tx_ptr->tx_hash.field,
                tx_ptr->tx_hash.field_len,
                BIN2HEX_LEFTTRIM_UNFMTDATA,
                BIN2HEX_PREFIX_0x_YES,
                BOAT_FALSE);

    tx_mined_timeout = BOAT_WAIT_PENDING_TX_TIMEOUT;
    param_eth_getTransactionReceipt.tx_hash_str = tx_hash_str;

    do
    {
        BoatSleep(BOAT_MINE_INTERVAL); // Sleep waiting for the block being mined
        
        tx_status_str = web3_eth_getTransactionReceiptStatus(tx_ptr->wallet_ptr->web3intf_context_ptr,
                                        tx_ptr->wallet_ptr->network_info.node_url_ptr,
                                        &param_eth_getTransactionReceipt);
		result = BoatEthPraseRpcResponseResult( tx_status_str, "status", 
												&tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf);
        if( result != BOAT_SUCCESS )
		{
            BoatLog(BOAT_LOG_NORMAL, "Fail to get transaction receipt due to RPC failure.");
            result = BOAT_ERROR_RPC_FAIL;
            break;
        }
        else
        {
            // status of tx_status_str == "": the transaction is pending
            // status of tx_status_str == "0x1": the transaction is successfully mined
            // status of tx_status_str == "0x0": the transaction fails
            if( tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr[0] != '\0' )
            {
                if( strcmp((BCHAR*)tx_ptr->wallet_ptr->web3intf_context_ptr->web3_result_string_buf.field_ptr, "0x1") == 0 )
                {
                    BoatLog(BOAT_LOG_NORMAL, "Transaction has got mined.");
                    result = BOAT_SUCCESS;
                    break;
                }
                else
                {
                    BoatLog(BOAT_LOG_NORMAL, "Transaction fails.");
                    result = BOAT_ERROR;
                }
            }
            
            tx_mined_timeout -= BOAT_MINE_INTERVAL;

        }
        
    }while(tx_mined_timeout > 0);

    if( tx_mined_timeout <= 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "Wait for pending transaction timeout. This does not mean the transaction fails.");
        result = BOAT_ERROR_TX_NOT_MINED;
    }

    return result;

}

