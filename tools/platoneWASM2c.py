#!/usr/bin/python

# Copyright (C) 2018-2021 aitos.io
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# This python script generates PlatONE's C language interface function from contract ABI (WASM).

# Not all contract ABI can be converted to C interface because C is lack of object-oriented programming
# capability. If the tool fails to generate the interface, you may have to organize the contract call
# manually.

# The generated C API is named "<ContractName><ContractApiName>", with a transaction pointer argument
# followed by contract arguments.
#
# For state-less contract call, the generated C API retuns a HEX string representing what is received
# from the blockchain network. If the call is successful, the string is the return value of the contract
# function. The return value string has to be parsed manually as per the contract prototype. If the call
# fails, it returns NULL.
#
# For value transfer or state-ful contract call, i.e. a transaction, the generated C API returns a HEX
# string representing the transaction hash. If the transaction fails, it returns NULL.

import sys
import json
import os.path

generated_declaration_block_str = '''/******************************************************************************
This file is generated from contract ABI. DO NOT modify it by hand.
******************************************************************************/

'''

generated_include_block_str = '''
// Generated C function interface from smart contract ABI

#include "boatconfig.h"
#include "boatiotsdk.h"

'''

# Map from type used in smart contract to type in C
type_mapping = {
    'string'    :'BCHAR*',
    'uint8'     :'BUINT8',
    'uint16'    :'BUINT16',
    'uint32'    :'BUINT32',
    'uint64'    :'BUINT64',
#    'uint128'   :'BUINT128',
    'int8'      :'BSINT8',
    'int16'     :'BSINT16',
    'int32'     :'BSINT32',
    'int64'     :'BSINT64',
#    'int128'    :'BSINT128',
    'void'      :'void'
    }


class CFunctionGen():

    def __init__(self, abi_file_name, output_path):
        self.abi_object = None
        self.c_file_content = ''
        self.h_file_content = ''

        with open(abi_file_name) as file_handle:
            self.abi_object = json.load(file_handle)
            self.abi_file_name = os.path.basename(abi_file_name)

        self.output_path = output_path
        


    def generate_c_funcs(self):
    
        if self.abi_object != None:
        
            self.c_file_content += generated_declaration_block_str
            self.c_file_content += generated_include_block_str

            self.h_file_content += generated_declaration_block_str
            self.h_file_content += generated_include_block_str
            
            for abi_item in self.abi_object:
                if abi_item['type'] == 'function':
                    self.generate_func_prototype(abi_item)
                    self.generate_func_body(abi_item)


    
    def generate_func_prototype(self, abi_item):
        # Extract type of return value
        if len(abi_item['outputs']) == 0:
            retval_str = 'BCHAR *' #'void'
        else:
            # For stateful transaction, returns Tx Hash; 
            # For state-less function call, returns a string representing the return value
            retval_str = 'BCHAR *'
    
        # Extract function name (Prefix with ABI file name because multiple contracts may have same function names)
        func_name_str = self.abi_file_name.replace('.json','')
        func_name_str = func_name_str.replace('.','_') + '_' + abi_item['name']
        
        # Extract function arguments
        inputs = abi_item['inputs']
        inputs_len = len(inputs)

        input_str = '('
        input_str += 'BoatPlatoneTx *tx_ptr'

        if inputs_len != 0:
            input_str += ', '

        i = 0
        while  i < inputs_len:
            input = inputs[i]
            try:
                input_str += type_mapping[input['type']] + ' ' + input['name']
            except:
                print(abi_item['name'] + '(): WASM C++ type (' + input['type'] + ') is incompatible with C interface auto generator.')
                print('You may have to manually construct the transaction.')
                quit(-1)

            if i != inputs_len -1:
                input_str += ', '
            i = i + 1
        
        input_str += ')'

        # Generate function prototype
        self.c_file_content += retval_str + func_name_str + input_str + '\n'
        self.h_file_content += retval_str + func_name_str + input_str + ';\n'


    def generate_func_body(self, abi_item):
    
        func_body_str = '{\n'
        
        # Generate local variables
        if abi_item['constant'] == 'true':
            func_body_str += '    BCHAR *call_result_str = NULL;\n'
        else:
            func_body_str += '    static BCHAR tx_hash_str[67] = \"\";\n'
            func_body_str += '    BoatFieldVariable data_field;\n'

        func_body_str += '    RlpEncodedStreamObject *rlp_stream_ptr;\n'
        func_body_str += '    RlpObject rlp_object_list;\n'
        func_body_str += '    RlpObject rlp_object_txtype;\n'
        func_body_str += '    BUINT64 txtype;\n'
        func_body_str += '    RlpObject rlp_object_string_func_name;\n'

        # Local variables extracted from function arguments
        inputs = abi_item['inputs']
        for input in inputs:
            func_body_str += '    RlpObject rlp_object_string_' + input['name'] + ';\n'
        
        func_body_str += '    boat_try_declare;\n\n'

        # Set Nonce
        if abi_item['constant'] != 'true':
            func_body_str += '    boat_try(BoatPlatoneTxSetNonce(tx_ptr, BOAT_PLATONE_NONCE_AUTO));\n\n'
         
        # Initialize List
        func_body_str += '    boat_try(RlpInitListObject(&rlp_object_list));\n\n'

        # Append Transaction Type 
        func_body_str += '    txtype = tx_ptr->rawtx_fields.txtype;\n'
        func_body_str += '    boat_try(' + 'RlpInitStringObject(&rlp_object_txtype, UtilityChangeEndian(&txtype, sizeof(txtype)), sizeof(txtype)));\n'
        func_body_str += '    boat_try(' + '0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_txtype));\n\n'
        
        # Append Contract Function Name
        func_body_str += '    boat_try(' + 'RlpInitStringObject(&rlp_object_string_func_name' + ', (BUINT8*)\"' + abi_item['name'] + '\", strlen(\"' + abi_item['name'] + '\")));\n'
        func_body_str += '    boat_try(' + '0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_string_func_name));\n\n'

        # Append arguments 
        for input in inputs:

            if input['type'] == 'string':
                func_body_str += '    boat_try(' + 'RlpInitStringObject(&rlp_object_string_' + input['name'] + ', (BUINT8*)' + input['name'] + ', strlen(' + input['name'] + ')));\n'
            else:
                try:
                    param_size_str = 'sizeof(' + type_mapping[input['type']] + ')'
                except:
                    print(abi_item['name'] + '(): WASM C++ type (' + input['type'] + ') is incompatible with C interface auto generator.')
                    print('You may have to manually construct the transaction.')
                    quit(-1)

                func_body_str += '    boat_try(' + 'RlpInitStringObject(&rlp_object_string_' + input['name'] + ', UtilityChangeEndian(&' + input['name'] + ', ' + param_size_str + ')' + ', ' + param_size_str + '));\n'

            func_body_str += '    boat_try(' + '0 > RlpEncoderAppendObjectToList(&rlp_object_list, &rlp_object_string_' + input['name'] + '));\n\n'

        # Encode RLP list for function parameters
        func_body_str += '    boat_try(RlpEncode(&rlp_object_list, NULL));\n\n'
        func_body_str += '    rlp_stream_ptr = RlpGetEncodedStream(&rlp_object_list);\n\n'


        if abi_item['constant'] == 'true':
            # for state-less funciton call
            func_body_str += '    call_result_str = BoatPlatoneCallContractFunc(tx_ptr, rlp_stream_ptr->stream_ptr, rlp_stream_ptr->stream_len);\n\n'
        else:
            # for stateful transaction
            func_body_str += '    data_field.field_ptr = rlp_stream_ptr->stream_ptr;\n'
            func_body_str += '    data_field.field_len = rlp_stream_ptr->stream_len;\n'
            func_body_str += '    boat_try(BoatPlatoneTxSetData(tx_ptr, &data_field));\n\n'
            func_body_str += '    boat_try(BoatPlatoneTxSend(tx_ptr));\n\n'
            func_body_str += '    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);\n\n'
         
        
        # Cleanup Label
        func_body_str += '''
    boat_catch(cleanup)
    {
        RlpRecursiveDeleteObject(&rlp_object_list);
        return(NULL);
    }
        '''

        func_body_str += '\n    RlpRecursiveDeleteObject(&rlp_object_list);\n'

        if abi_item['constant'] == 'true':
            func_body_str += '    return(call_result_str);\n'
        else:
            func_body_str += '    return(tx_hash_str);\n'

        func_body_str += '\n}\n\n'
        self.c_file_content += func_body_str
        

 
    def save_c_file(self):
        if self.abi_object != None:
            c_file_name = self.output_path + '/' + self.abi_file_name.replace('.json','.c')
            with open(c_file_name, 'w') as c_file_handle:
                c_file_handle.write(self.c_file_content)

            h_file_name = self.output_path + '/' + self.abi_file_name.replace('.json','.h')
            with open(h_file_name, 'w') as h_file_handle:
                h_file_handle.write(self.h_file_content)

def main():
    argc  = len(sys.argv)
    if argc <= 1 or argc >= 4:
        print('Usage: ' + sys.argv[0] + ' <ABI File> ' + '[<Output path>]')
    else:
        if argc == 2:
            output_path = './'
        else:
            output_path = sys.argv[2]
                
        c_func_object = CFunctionGen(sys.argv[1], output_path)

        c_func_object.generate_c_funcs()
        c_func_object.save_c_file()
       




if __name__ == '__main__':
    main()

