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


# This python script generates Plat ONE's C language interface function from contract ABI (solidity).

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
import string

generated_declaration_block_str = '''/******************************************************************************
This file is generated from contract ABI. DO NOT modify it by hand.
******************************************************************************/

'''

generated_include_block_str = '''
// Generated C function interface from smart contract ABI

#include "boatiotsdk.h"

'''

# Types specially for Solidity
generated_types_for_solidity_str = '''
// Types specially defined for mapping from Solidity

    typedef BUINT8 Bbytes1[1];
    typedef BUINT8 Bbytes2[2];
    typedef BUINT8 Bbytes3[3];
    typedef BUINT8 Bbytes4[4];
    typedef BUINT8 Bbytes5[5];
    typedef BUINT8 Bbytes6[6];
    typedef BUINT8 Bbytes7[7];
    typedef BUINT8 Bbytes8[8];

    typedef BUINT8 Bbytes9[9];
    typedef BUINT8 Bbytes10[10];
    typedef BUINT8 Bbytes11[11];
    typedef BUINT8 Bbytes12[12];
    typedef BUINT8 Bbytes13[13];
    typedef BUINT8 Bbytes14[14];
    typedef BUINT8 Bbytes15[15];
    typedef BUINT8 Bbytes16[16];

    typedef BUINT8 Bbytes17[17];
    typedef BUINT8 Bbytes18[18];
    typedef BUINT8 Bbytes19[19];
    typedef BUINT8 Bbytes20[20];
    typedef BUINT8 Bbytes21[21];
    typedef BUINT8 Bbytes22[22];
    typedef BUINT8 Bbytes23[23];
    typedef BUINT8 Bbytes24[24];

    typedef BUINT8 Bbytes25[25];
    typedef BUINT8 Bbytes26[26];
    typedef BUINT8 Bbytes27[27];
    typedef BUINT8 Bbytes28[28];
    typedef BUINT8 Bbytes29[29];
    typedef BUINT8 Bbytes30[30];
    typedef BUINT8 Bbytes31[31];
    typedef BUINT8 Bbytes32[32];

    typedef Bbytes16 BUINT128;
    typedef Bbytes16 BSINT128;
    typedef Bbytes32 BUINT256;
    typedef Bbytes32 BSINT256;

'''


# Map from type used in smart contract to type in C
# Types not listed are not supported
type_mapping = {
    #Non-fixed size types and Non-fixed size Non-fixed array types
    'string'    :'BCHAR*',
    'bytes'     :'BUINT8*',
    'bytes[]'   :'BoatFieldVariable*',
    'string[]'  :'BoatFieldVariable*',
    #'address[]' :'BoatAddress*',

    #Fixed size Non-fixed array types
    'address[]'   :'BoatAddress *',
    'bool[]'      :'BUINT8 *',
    'uint8[]'     :'BUINT8 *',
    'uint16[]'    :'BUINT16 *',
    'uint32[]'    :'BUINT32 *',
    'uint64[]'    :'BUINT64 *',
    'uint128[]'   :'BUINT128 *',

    'uint256[]'   :'BUINT256 *',

    'int8[]'      :'BSINT8 *',
    'int16[]'     :'BSINT16 *',
    'int32[]'     :'BSINT32 *',
    'int64[]'     :'BSINT64 *',
    'int128[]'    :'BSINT128 *',

    'int256[]'    :'BSINT256 *',
    
    'bytes1[]'    :'Bbytes1 *',
    'bytes2[]'    :'Bbytes2 *',
    'bytes3[]'    :'Bbytes3 *',
    'bytes4[]'    :'Bbytes4 *',
    'bytes5[]'    :'Bbytes5 *',
    'bytes6[]'    :'Bbytes6 *',
    'bytes7[]'    :'Bbytes7 *',
    'bytes8[]'    :'Bbytes8 *',
    
    'bytes9[]'    :'Bbytes9 *',
    'bytes10[]'   :'Bbytes10 *',
    'bytes11[]'   :'Bbytes11 *',
    'bytes12[]'   :'Bbytes12 *',
    'bytes13[]'   :'Bbytes13 *',
    'bytes14[]'   :'Bbytes14 *',
    'bytes15[]'   :'Bbytes15 *',
    'bytes16[]'   :'Bbytes16 *',
    
    'bytes17[]'   :'Bbytes17 *',
    'bytes18[]'   :'Bbytes18 *',
    'bytes19[]'   :'Bbytes19 *',
    'bytes20[]'   :'Bbytes20 *',
    'bytes21[]'   :'Bbytes21 *',
    'bytes22[]'   :'Bbytes22 *',
    'bytes23[]'   :'Bbytes23 *',
    'bytes24[]'   :'Bbytes24 *',
    
    'bytes25[]'   :'Bbytes25 *',
    'bytes26[]'   :'Bbytes26 *',
    'bytes27[]'   :'Bbytes27 *',
    'bytes28[]'   :'Bbytes28 *',
    'bytes29[]'   :'Bbytes29 *',
    'bytes30[]'   :'Bbytes30 *',
    'bytes31[]'   :'Bbytes31 *',
    'bytes32[]'   :'Bbytes32 *',

    #Fixed size types
    'address'   :'BoatAddress',
    'bool'      :'BUINT8',
    'uint8'     :'BUINT8',
    'uint16'    :'BUINT16',
    'uint32'    :'BUINT32',
    'uint64'    :'BUINT64',
    'uint128'   :'BUINT128',

    'uint256'   :'BUINT256',

    'int8'      :'BSINT8',
    'int16'     :'BSINT16',
    'int32'     :'BSINT32',
    'int64'     :'BSINT64',
    'int128'    :'BSINT128',

    'int256'    :'BSINT256',
    
    'bytes1'    :'Bbytes1',
    'bytes2'    :'Bbytes2',
    'bytes3'    :'Bbytes3',
    'bytes4'    :'Bbytes4',
    'bytes5'    :'Bbytes5',
    'bytes6'    :'Bbytes6',
    'bytes7'    :'Bbytes7',
    'bytes8'    :'Bbytes8',
    
    'bytes9'    :'Bbytes9',
    'bytes10'   :'Bbytes10',
    'bytes11'   :'Bbytes11',
    'bytes12'   :'Bbytes12',
    'bytes13'   :'Bbytes13',
    'bytes14'   :'Bbytes14',
    'bytes15'   :'Bbytes15',
    'bytes16'   :'Bbytes16',
    
    'bytes17'   :'Bbytes17',
    'bytes18'   :'Bbytes18',
    'bytes19'   :'Bbytes19',
    'bytes20'   :'Bbytes20',
    'bytes21'   :'Bbytes21',
    'bytes22'   :'Bbytes22',
    'bytes23'   :'Bbytes23',
    'bytes24'   :'Bbytes24',
    
    'bytes25'   :'Bbytes25',
    'bytes26'   :'Bbytes26',
    'bytes27'   :'Bbytes27',
    'bytes28'   :'Bbytes28',
    'bytes29'   :'Bbytes29',
    'bytes30'   :'Bbytes30',
    'bytes31'   :'Bbytes31',
    'bytes32'   :'Bbytes32'
    
    }


class CFunctionGen():

    def __init__(self, abi_file_name, output_path):
        self.abi_object = None
        self.c_file_content = ''
        self.h_file_content = ''

        with open(abi_file_name) as file_handle:
            self.abi_object = json.load(file_handle)
            self.abi_file_name = os.path.basename(abi_file_name)
            #print(self.abi_object);

        self.output_path = output_path
        

    def require_endian_change(self, abitype):
        types_to_change_endian = {
            #'bool'      :'BUINT8',
            #'uint8'     :'BUINT8',
            'uint16'    :'BUINT16',
            'uint32'    :'BUINT32',
            'uint64'    :'BUINT64',
            'uint128'   :'BUINT128',
            'uint256'   :'BUINT256',
            #'int8'      :'BSINT8',
            'int16'     :'BSINT16',
            'int32'     :'BSINT32',
            'int64'     :'BSINT64',
            'int128'    :'BSINT128',
            'int256'    :'BSINT256',
            'address'   :'BoatAddress',

            'address[]'   :'BoatAddress *',
            'bool[]'      :'BUINT8 *',
            'uint8[]'     :'BUINT8 *',
            'uint16[]'    :'BUINT16 *',
            'uint32[]'    :'BUINT32 *',
            'uint64[]'    :'BUINT64 *',
            'uint128[]'   :'BUINT128 *',

            'uint256[]'   :'BUINT256 *',

            'int8[]'      :'BSINT8 *',
            'int16[]'     :'BSINT16 *',
            'int32[]'     :'BSINT32 *',
            'int64[]'     :'BSINT64 *',
            'int128[]'    :'BSINT128 *',

            'int256[]'    :'BSINT256 *'
        }

        if abitype in types_to_change_endian.keys():
            return True
        else:
            return False

    #check the input type is supported or not
    def is_supported_type(self, abitype):
        if abitype in type_mapping.keys():
            return True
        elif abitype[abitype.find('[') + 1:].find('[') != -1:
            return False
        elif abitype[0:abitype.find('[')] in type_mapping.keys():
            return True
        else: 
            return False
    
    def is_value_type(self, abitype):
        types_of_value = {
            'bool'      :'BUINT8',
            'uint8'     :'BUINT8',
            'uint16'    :'BUINT16',
            'uint32'    :'BUINT32',
            'uint64'    :'BUINT64',

            'int8'      :'BSINT8',
            'int16'     :'BSINT16',
            'int32'     :'BSINT32',
            'int64'     :'BSINT64',
        }

        if abitype.find('[') != -1:
            if abitype[:abitype.find('[')] in types_of_value.keys():
                return True
        elif abitype in types_of_value.keys():
            return True
        else:
            return False

    def is_right_aligned_type(self, abitype):
        types_of_left_aligned = {
            'address'   :'BoatAddress',
            'bool'      :'BUINT8',
            'uint8'     :'BUINT8',
            'uint16'    :'BUINT16',
            'uint32'    :'BUINT32',
            'uint64'    :'BUINT64',
            'uint128'   :'BUINT128',

            'uint256'   :'BUINT256',

            'int8'      :'BSINT8',
            'int16'     :'BSINT16',
            'int32'     :'BSINT32',
            'int64'     :'BSINT64',
            'int128'    :'BSINT128',

            'int256'    :'BSINT256',

            'address[]'   :'BoatAddress *',
            'bool[]'      :'BUINT8 *',
            'uint8[]'     :'BUINT8 *',
            'uint16[]'    :'BUINT16 *',
            'uint32[]'    :'BUINT32 *',
            'uint64[]'    :'BUINT64 *',
            'uint128[]'   :'BUINT128 *',

            'uint256[]'   :'BUINT256 *',

            'int8[]'      :'BSINT8 *',
            'int16[]'     :'BSINT16 *',
            'int32[]'     :'BSINT32 *',
            'int64[]'     :'BSINT64 *',
            'int128[]'    :'BSINT128 *',

            'int256[]'    :'BSINT256 *'
        }

        if abitype in types_of_left_aligned.keys():
            return True
        else:
            return False

    def is_signed_value_type(self, abitype):
        types_of_signed_value = {
            'int8'      :'BSINT8',
            'int16'     :'BSINT16',
            'int32'     :'BSINT32',
            'int64'     :'BSINT64',
            'int128'    :'BSINT128',
            'int256'    :'BSINT256',

            'int8[]'      :'BSINT8 *',
            'int16[]'     :'BSINT16 *',
            'int32[]'     :'BSINT32 *',
            'int64[]'     :'BSINT64 *',
            'int128[]'    :'BSINT128 *',

            'int256[]'    :'BSINT256 *'
        }

        if abitype in types_of_signed_value.keys():
            return True
        else:
            return False

    def get_FixedArray_length(self, abitype):
        s = abitype.find('[')
        e = abitype.find(']')
        return int(abitype[s + 1 : e])

    def get_FixedArray_type(self, abitype):
        e = abitype.find('[')
        return type_mapping[abitype[:e]]

    #check abitype is non-fixed size non-fixed array type or not
    def is_NonFixedSizeNonFixedArray_type(self, abitype):
        if abitype == 'bytes[]':
            return True
        elif abitype == 'string[]':
            return True
        else:
            return False

    #check abitype is non-fixed size fixed array type or not
    def is_NonFixedSizeFixedArray_type(self, abitype):
        if ((abitype.find('bytes[') != -1) and (abitype != 'bytes[]')):
            return True
        elif ((abitype.find('string[') != -1) and (abitype != 'string[]')):
            return True
        else:
            return False
    
    #check abitype is non-fixed type or not
    def is_NonFixedSize_type(self, abitype):
        if abitype == 'string':
            return True
        elif abitype == 'bytes':
            return True
        else:
            return False

    #check abitype is fixed size non-fixed array type or not
    def is_FixedSizeNonFixedArray_type(self, abitype):
        if ((abitype.find('[]') != -1) and (abitype != 'string[]') and (abitype != 'bytes[]')):
            return True
        else:
            return False

    #check abitype is fixed size fixed array type or not
    def is_FixedSizeFixedArray_type(self, abitype):
        if ((abitype.find('[') != -1) and (abitype.find('[]') == -1) and \
            (abitype.find('string') == -1) and (abitype.find('bytes[') == -1)):
            return True
        else:
            return False

    def is_needFlagInputLen_type(self, abitype):
        if abitype == 'bytes':
            return True
        elif abitype.find('[]') != -1:
            return True
        else:
            return False

    #check function has non-fixed array input type or not,such as bytes[],string[],bytes[5],etc
    def is_has_nonFixedArray_type(self, abi_item):
        inputs     = abi_item['inputs']
        inputs_len = len(inputs)
        
        i = 0
        while  i < inputs_len:
            inputType = inputs[i]['type']
            if (self.is_NonFixedSizeFixedArray_type(inputType) == True or\
                self.is_FixedSizeNonFixedArray_type(inputType) == True or\
                self.is_NonFixedSizeNonFixedArray_type(inputType) == True):
                return True
            i += 1
        
        return False

    #check function has non-fixed input type or not
    def is_has_nonFixed_type(self, abi_item):
        inputs     = abi_item['inputs']
        inputs_len = len(inputs)
        
        i = 0
        while  i < inputs_len:
            inputType = inputs[i]['type']
            if (self.is_NonFixedSizeFixedArray_type(inputType) == True or\
                self.is_FixedSizeNonFixedArray_type(inputType) == True or
                self.is_FixedSizeFixedArray_type(inputType) == True or
                self.is_NonFixedSize_type(inputType) == True or\
                self.is_NonFixedSizeNonFixedArray_type(inputType) == True):
                return True
            i += 1
        
        return False
    
    def is_Change_Blockchain_State(self, abi_item):
        if ('constant' in abi_item):
            if abi_item['constant'] == True:
                return True
            else:
                return False
        else:
            if (abi_item['stateMutability'] == 'nonpayable') or (abi_item['stateMutability'] == 'payable'):
                return False
            else:
                return True

    def get_input_type(self, abi_item):
        if abi_item in type_mapping:
            return abi_item
        elif (self.is_NonFixedSizeFixedArray_type(abi_item) == True or\
            self.is_FixedSizeFixedArray_type(abi_item) == True):
            return abi_item[:abi_item.find('[')]
        else: #not supported
            print(abi_item + '(): Solidity type (' + abi_item + ') is incompatible with C interface auto generator.')
            print('You may have to manually construct the transaction.')
            quit(-1)

    def gen_input_name(self, abi_item):
        if len(abi_item['name']) == 0:
            #print 'input name is null'
            return type_mapping[abi_item['type']] + 'Value'
        else:
            return abi_item['name']

    def gen_nonFixed_mallocSize_exp(self, abi_item, spaceNum = 38):
        inputs     = abi_item['inputs']
        inputs_len = len(inputs)
        nonFixedSize_type_malloc = ''

        i = 0
        while  i < inputs_len:
            inputType = inputs[i]['type']
            inputName = self.gen_input_name(inputs[i])
            if (self.is_NonFixedSizeNonFixedArray_type(inputType) == True or \
                self.is_NonFixedSizeFixedArray_type(inputType) == True or \
                self.is_FixedSizeNonFixedArray_type(inputType) == True):
                pass #can't compute length here,compute teh length later
            elif self.is_NonFixedSize_type(inputType) == True:
                if len(nonFixedSize_type_malloc) != 0:
                    nonFixedSize_type_malloc +=  ' ' * spaceNum
                if self.is_needFlagInputLen_type(inputType) == True:
                    nonFixedSize_type_malloc += 'BOAT_ROUNDUP(' + inputName + 'Len , 32) + 32' + ' + \\\n'
                else:
                    nonFixedSize_type_malloc += 'BOAT_ROUNDUP(' + 'strlen(' + inputName + '), 32) + 32' + ' + \\\n'   
            else:
                pass
            
            i += 1

        if len(nonFixedSize_type_malloc) != 0:
            nonFixedSize_type_malloc += ' ' * spaceNum
        return nonFixedSize_type_malloc
    
    def gen_Fixed_mallocSize_exp(self, abi_item):
        inputs     = abi_item['inputs']
        inputs_len = len(inputs)

        cnt = 0
        i = 0
        while  i < inputs_len:
            inputType = inputs[i]['type']
            if self.is_FixedSizeFixedArray_type(inputType) == True:
                cnt += self.get_FixedArray_length(inputType)
            else:
                cnt = cnt + 1
            
            i += 1

        return str(cnt)
        
    #gen non-fixed type offset location
    #string rtn
    def get_nonFixed_offset(self, abi_item, index):
        inputs     = abi_item['inputs']
        inputs_len = len(inputs)
        #offset_int = inputs_len * 32
        offset_int = 0
        offset_str = ''

        i = 0
        while  i < inputs_len:
            inputType = inputs[i]['type']
            inputName = self.gen_input_name(inputs[i])
            if self.is_FixedSizeFixedArray_type(inputType) == True:
                offset_int += self.get_FixedArray_length(inputType) * 32
            else:
                offset_int += 32

            i += 1
        
        i = 0
        while  i < index:
            inputType = inputs[i]['type']
            inputName = self.gen_input_name(inputs[i])
            if (self.is_NonFixedSizeNonFixedArray_type(inputType) == True or \
                self.is_NonFixedSizeFixedArray_type(inputType) == True or \
                self.is_FixedSizeNonFixedArray_type(inputType) == True):
                if len(offset_str) != 0:
                    offset_str +=  ' ' * 27
                offset_str += inputName + '_data_length + \\\n'
            elif self.is_NonFixedSize_type(inputType) == True:
                if len(offset_str) != 0:
                    offset_str +=  ' ' * 27
                if self.is_needFlagInputLen_type(inputType) == True:
                    offset_str += 'BOAT_ROUNDUP(' + inputName + 'Len , 32) + 32 ' + '+ \\\n'
                else:
                    offset_str += 'BOAT_ROUNDUP(' + 'strlen(' + inputName + '), 32) + 32 ' + '+ \\\n'   
            else:
                pass
            i += 1

        if len(offset_str) != 0:
            return offset_str + ' ' * 27 + '+ ' + str(offset_int)
        return str(offset_int)

    #genetate non-fixed data length
    def get_nonFixed_length(self, input):
        inputType = input['type']
        inputName = self.gen_input_name(input)
        length_str = ''
        
        if self.is_NonFixedSize_type(inputType) == True:
            if self.is_needFlagInputLen_type(inputType) == True:
                length_str += 'BOAT_ROUNDUP(' + inputName + 'Len , 32)'
            else:
                length_str += 'BOAT_ROUNDUP(' + 'strlen(' + inputName + '), 32)'
        elif (self.is_NonFixedSizeFixedArray_type(inputType) == True or\
            self.is_NonFixedSizeNonFixedArray_type(inputType) == True):
            length_str += 'BOAT_ROUNDUP(' + inputName + 'Len , 32)'
        elif self.is_FixedSizeNonFixedArray_type(inputType) == True:
            length_str += inputName + 'Len;\n'
        else:
            pass

        return length_str
    
    def generate_nonFixedArray_values(self, abi_item):
        vaules_str = ''

        inputs     = abi_item['inputs']
        inputs_len = len(inputs)
        
        i = 0
        while  i < inputs_len:
            inputType = inputs[i]['type']
            inputName = self.gen_input_name(inputs[i])
            if (self.is_NonFixedSizeNonFixedArray_type(inputType) == True or \
                self.is_NonFixedSizeFixedArray_type(inputType) == True or \
                self.is_FixedSizeNonFixedArray_type(inputType) == True):

                # Generate C code for set non-fixed size fixed array input length
                if self.is_NonFixedSizeFixedArray_type(inputType) == True:
                    vaules_str += '    BUINT32 ' + inputName + 'Len = ' + str(self.get_FixedArray_length(inputType)) + ';\n'

                vaules_str += '    BUINT32 ' + inputName + '_data_length = 0;\n'
                vaules_str += '    BUINT32 ' + inputName + '_offset_location[' + inputName + 'Len];\n'
                vaules_str += '    BUINT32 ' + inputName + '_filled_length[' + inputName + 'Len];\n'
                vaules_str += '    BUINT32 ' + inputName + '_actual_length[' + inputName + 'Len];\n'
                if self.is_FixedSizeNonFixedArray_type(inputType) == True:
                    vaules_str += '    ' + self.get_FixedArray_type(inputType) + ' *' + inputName + '_ptr;\n'
                else:
                    vaules_str += '    BoatFieldVariable *' + inputName + '_ptr;\n'
                
            elif inputType == 'string':
                vaules_str += '    BUINT32 ' + inputName + 'Len;\n'
            i += 1

        return vaules_str
    
    def generate_nonFixedArray_length(self, abi_item):
        length_str = ''

        inputs     = abi_item['inputs']
        inputs_len = len(inputs)
        
        i = 0
        while  i < inputs_len:
            inputType = inputs[i]['type']
            inputName = self.gen_input_name(inputs[i])
            if (self.is_NonFixedSizeNonFixedArray_type(inputType) == True or\
                self.is_NonFixedSizeFixedArray_type(inputType) == True):
                length_str += '    ' + inputName + '_ptr = ' + inputName + ';\n'
                length_str += '    for(i = 0; i < ' + inputName + 'Len; i++)\n'
                length_str += '    {\n'
                length_str += '        ' + inputName + '_filled_length[i] = BOAT_ROUNDUP(' + inputName + '_ptr->field_len, 32) + 32;\n'
                length_str += '        ' + inputName + '_data_length += ' + inputName + '_filled_length[i];\n'
                length_str += '        ' + inputName + '_ptr++;\n'
                length_str += '    }\n'
                if self.is_NonFixedSizeFixedArray_type(inputType) == True:
                    length_str += '    ' + inputName + '_data_length += ' + inputName + 'Len * 32;\n'
                else:
                    length_str += '    ' + inputName + '_data_length += ' + inputName + 'Len * 32 + 32;\n'
                length_str += '    data_field.field_len += ' + inputName + '_data_length;\n\n'
            elif self.is_FixedSizeNonFixedArray_type(inputType) == True:
                length_str += '    ' + inputName + '_data_length += ' + inputName + 'Len * 32 + 32;\n'
                length_str += '    data_field.field_len += ' + inputName + '_data_length;\n\n'
            else:
                pass
            i += 1

        return length_str

    def generate_NonFixedSizeFixedArray_inputs(self, abi_item):
        result_str = ''

        inputs     = abi_item['inputs']
        inputs_len = len(inputs)
        
        i = 0
        while  i < inputs_len:
            inputType = inputs[i]['type']
            inputName = self.gen_input_name(inputs[i])
            if self.is_NonFixedSizeFixedArray_type(inputType) == True:
                result_str += '    ' + inputName + 'Len = ' + str(self.get_FixedArray_length(inputType)) + ';\n'

            i += 1

        result_str += '\n'

        return result_str

    def add_Fill_Offset_String(self,layers):
        fill_offset_str = ""
        fill_offset_str += '    ' * layers + 'UtilityChangeEndian(&data_offset_location, sizeof(BUINT32));\n'
        fill_offset_str += '    ' * layers + 'memset(data_offset_ptr, 0x00, 32);\n'
        fill_offset_str += '    ' * layers + 'memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &data_offset_location, sizeof(BUINT32));\n'
        fill_offset_str += '    ' * layers + 'data_offset_ptr += 32;\n\n'
        return fill_offset_str

    def add_FixedSize_String(self,inputType,c_address_sign,inputName,param_size_str,layers):
        fixed_size_str = ""
        if self.require_endian_change(inputType) == True:
            fixed_size_str += '    ' * layers + 'UtilityChangeEndian(' + c_address_sign + inputName + ', ' + param_size_str + ');\n'

        if self.is_right_aligned_type(inputType) == False:
            fixed_size_str += '    ' * layers + 'memcpy(data_offset_ptr, ' + c_address_sign + inputName + ', ' + param_size_str + ');\n'
        else:
            fixed_size_str += '    ' * layers + 'memcpy(data_offset_ptr + 32 - ' + param_size_str + ', ' + c_address_sign + inputName + ', ' + param_size_str + ');\n'
        fixed_size_str += '    ' * layers + 'data_offset_ptr += 32;\n'
        return fixed_size_str

    def generate_c_funcs(self):
    
        if self.abi_object != None:
        
            self.c_file_content += generated_declaration_block_str
            self.c_file_content += generated_include_block_str
            self.c_file_content += generated_types_for_solidity_str
            
            self.h_file_content += generated_declaration_block_str
            self.h_file_content += generated_include_block_str
            self.h_file_content += generated_types_for_solidity_str
            
            for abi_item in self.abi_object['abi']:
                if abi_item['type'] == 'function':
                    self.generate_func_prototype(abi_item)
                    self.generate_func_body(abi_item)
    
    def generate_func_prototype(self, abi_item):
        inputName_str = ''

        # Extract type of return value
        if len(abi_item['outputs']) == 0:
            retval_str = 'BCHAR *' #'void'
        else:
            #retval_str = type_mapping[abi_item['outputs'][0]['type']]
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
        while i < inputs_len:
            input = inputs[i]

            # bytes3[5] -> bytes3
            inputType = self.get_input_type(input['type'])
            inputName_str = self.gen_input_name(input)
            if self.is_supported_type(input['type']) == False:
                print(abi_item['name'] + '(): Solidity type (' + input['type'] + ') is incompatible with C interface auto generator.')
                print('You may have to manually construct the transaction.')
                quit(-1)

            
            if self.is_NonFixedSizeFixedArray_type(input['type']) == True:
                input_str += 'BoatFieldVariable *' + inputName_str
            elif self.is_FixedSizeFixedArray_type(input['type']) == True:
                input_str += type_mapping[inputType] + ' ' + inputName_str + '[' + str(self.get_FixedArray_length(input['type'])) + ']'
            elif type_mapping[inputType].endswith('*'):
                input_str += type_mapping[inputType] + inputName_str
            else:
                input_str += type_mapping[inputType] + ' ' + inputName_str

            if self.is_needFlagInputLen_type(input['type']) == True:
                input_str += ', BUINT32 ' + inputName_str + 'Len'
                
            if i != inputs_len - 1:
                input_str += ', '
            i = i + 1
        
        input_str += ')'

        # Generate function prototype
        self.c_file_content += retval_str + func_name_str + input_str + '\n'
        self.h_file_content += retval_str + func_name_str + input_str + ';\n'


    def generate_func_body(self, abi_item):
    
        func_body_str = '{\n'
        inputName_str = ''

        # Generate local variables
        if self.is_Change_Blockchain_State(abi_item):
            func_body_str += '    BCHAR *call_result_str = NULL;\n'
        else:
            func_body_str += '    static BCHAR tx_hash_str[67] = \"\";\n'

        func_body_str += '    BoatFieldVariable data_field = {NULL, 0};\n'
        func_body_str += '    BCHAR *function_prototye_str;\n'
        func_body_str += '    BUINT8 field_bytes32[32];\n'
        func_body_str += '    BUINT8 fixedsize_bytes32[32];\n'
        func_body_str += '    BUINT8 *data_offset_ptr;\n'
        if self.is_has_nonFixed_type(abi_item) == True:
            func_body_str += '    BUINT32 data_offset_location;\n'
            func_body_str += '    BUINT32 nonFixed_filled_length;\n'
            func_body_str += '    (void) nonFixed_filled_length;\n'
            func_body_str += '    BUINT32 nonFixed_actual_length;\n'
        func_body_str += self.generate_nonFixedArray_values(abi_item)
        func_body_str += '    BUINT32 i;\n'

        func_body_str += '    (void) fixedsize_bytes32;\n'
        func_body_str += '    (void) i;\n'

        func_body_str += '    boat_try_declare;\n\n'

        inputs = abi_item['inputs']
        
        # input param check
        inputs_param     = abi_item['inputs']
        inputs_param_len = len(inputs_param)
        inputs_param_body_str_tmp = ''
        if inputs_param_len != 0:
            i = 0
            while  i < inputs_param_len:
                if (self.is_NonFixedSize_type(inputs_param[i]['type']) or not(self.is_value_type(inputs_param[i]['type']))):
                    inputParamName_str = self.gen_input_name(inputs_param[i])
                    inputs_param_body_str_tmp  += '|| (' + inputParamName_str + ' == NULL)'
                i = i + 1
            if len(inputs_param_body_str_tmp) == 0:
                func_body_str += '    if (tx_ptr == NULL)\n'
            else:
                func_body_str += '    if ((tx_ptr == NULL) ' + inputs_param_body_str_tmp +')\n'
        else:
            func_body_str += '    if (tx_ptr == NULL)\n'

        func_body_str     += '    {\n'
        func_body_str     += '        return NULL;\n'
        func_body_str     += '    }\n\n'

        # Set Nonce
        if not self.is_Change_Blockchain_State(abi_item):
            func_body_str += '    boat_try(BoatPlatoneTxSetNonce(tx_ptr, BOAT_PLATONE_NONCE_AUTO));\n\n'



        # Extract solidity function inputs
        inputs_len = len(inputs)
        nonFixed_filedLen_str = self.gen_nonFixed_mallocSize_exp(abi_item, 27)
        fixed_size_str        = '(' + self.gen_Fixed_mallocSize_exp(abi_item) + ' * 32 + 4)'
        func_body_str += '    data_field.field_len = ' + nonFixed_filedLen_str + fixed_size_str + ';\n'
        func_body_str += self.generate_nonFixedArray_length(abi_item)
        
        func_body_str += '    data_field.field_ptr = BoatMalloc(data_field.field_len);\n'
        func_body_str += '    if (data_field.field_ptr == NULL)\n'
        func_body_str += '    {\n'
        func_body_str += '        boat_throw(BOAT_ERROR, cleanup);\n'
        func_body_str += '    }\n'
         
        func_body_str += '    data_offset_ptr = data_field.field_ptr;\n\n'


        #func_body_str += '    printf("The filled length is %x\\n", data_field.field_len);\n'

        # Construct solidity function prototype
        # Solidity function name
        sol_func_proto_str = abi_item['name'] + '('
        i = 0
        while  i < inputs_len:
            input = inputs[i]
            sol_func_proto_str += input['type']
            if i != inputs_len - 1:
                sol_func_proto_str += ','
            i = i + 1
        sol_func_proto_str += ')'

        # Generate C code for function selector
        func_body_str += '    function_prototye_str = \"' + sol_func_proto_str + '\";\n'
        func_body_str += '    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);\n'

        func_body_str += '    memcpy(data_offset_ptr, field_bytes32, 4);\n'
        func_body_str += '    data_offset_ptr += 4;\n\n'

        # Assemble function arguments
        inputIndex = 0
        for input in inputs:
            inputIndex += 1 #only be used for third param of get_nonFixed_offset(...)

            inputType = self.get_input_type(input['type'])
            inputName_str = self.gen_input_name(input)
            #param_size_str is a single value size(byte)
            if (inputType.find('[') != -1):
                param_size_str = 'sizeof(' + type_mapping[inputType[:inputType.find('[')]] + ')'
            param_size_str = 'sizeof(' + type_mapping[inputType] + ')'
            
            # Only prefix "&" to native C integer types to obtain its address. No "&" for an array type.
            if self.is_value_type(input['type']) == True:
                c_address_sign = '&'
            else:
                c_address_sign = ''

            if (self.is_NonFixedSizeNonFixedArray_type(input['type']) == True or \
                self.is_FixedSizeNonFixedArray_type(input['type']) == True or \
                self.is_NonFixedSizeFixedArray_type(input['type']) == True or \
                self.is_NonFixedSize_type(input['type']) == True):
                #fill offset location value
                func_body_str += '    //param \'' + inputName_str + '\' offset location filled\n'
                func_body_str += '    data_offset_location = ' + self.get_nonFixed_offset( abi_item, inputIndex - 1) + ';\n'

                func_body_str += self.add_Fill_Offset_String(1)
            elif self.is_FixedSizeFixedArray_type(input['type']) == True:

                func_body_str += '    //param \'' + inputName_str + '\' data filled\n'
                func_body_str += '    for(i = 0; i < ' + str(self.get_FixedArray_length(input['type'])) + '; i++)\n'
                func_body_str += '    {\n'
                func_body_str += '        memcpy(fixedsize_bytes32, ' + c_address_sign + inputName_str + '[i], ' + param_size_str + ');\n'
                if self.is_signed_value_type(input['type']) == True:
                    func_body_str += '        if (' + inputName_str + '[i] >= 0)\n'
                    func_body_str += '        {\n'
                    func_body_str += '            memset(data_offset_ptr, 0x00, 32);\n'
                    func_body_str += '        }\n'
                    func_body_str += '        else\n'
                    func_body_str += '        {\n'
                    func_body_str += '            memset(data_offset_ptr, 0xFF, 32);\n'
                    func_body_str += '        }\n'
                else:
                    func_body_str += '        memset(data_offset_ptr, 0x00, 32);\n'
                func_body_str += self.add_FixedSize_String(inputType, c_address_sign,'fixedsize_bytes32',param_size_str, 2)
                func_body_str += '    }\n\n'
            else:
                if self.is_signed_value_type(input['type']) == True:
                    func_body_str += '    if (' + inputName_str + ' >= 0)\n'
                    func_body_str += '    {\n'
                    func_body_str += '        memset(data_offset_ptr, 0x00, 32);\n'
                    func_body_str += '    }\n'
                    func_body_str += '    else\n'
                    func_body_str += '    {\n'
                    func_body_str += '        memset(data_offset_ptr, 0xFF, 32);\n'
                    func_body_str += '    }\n'
                else:
                    func_body_str += '    memset(data_offset_ptr, 0x00, 32);\n'    
                func_body_str += self.add_FixedSize_String(inputType, c_address_sign, inputName_str, param_size_str, 1)

        #non-fixed data fill
        i = 0
        while  i < inputs_len:
            input = inputs[i]

            inputName_str   = self.gen_input_name(input)
            #if len(nonFixed_length) != 0:

            if (self.is_NonFixedSizeNonFixedArray_type(input['type']) == False and \
                self.is_NonFixedSizeFixedArray_type(input['type']) == False and \
                self.is_FixedSizeNonFixedArray_type(input['type']) == False and \
                self.is_NonFixedSize_type(input['type']) == False):
                i += 1
                continue
            
            func_body_str  += '    //non-fixed param \'' + inputName_str + '\' data filled\n'
            #non-fixed data length

            if (self.is_NonFixedSizeNonFixedArray_type(input['type']) == True or \
                self.is_NonFixedSizeFixedArray_type(input['type']) == True):
                    
                if self.is_NonFixedSizeNonFixedArray_type(input['type']) == True:
                    func_body_str += '    //filled non-fixed size non-fixed array length\n'
                    func_body_str += '    nonFixed_actual_length  = ' + inputName_str + 'Len' + ';\n'
                    func_body_str += '    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));\n'
                    func_body_str += '    memset(data_offset_ptr, 0x00, 32);\n'
                    func_body_str += '    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));\n'
                    func_body_str += '    data_offset_ptr += 32;\n\n'

                func_body_str += '    //filled non-fixed size array data offset\n'
                func_body_str += '    for (i = 0; i < ' + inputName_str + 'Len; i++)\n'
                func_body_str += '    {\n'
                func_body_str += '        if (i == 0) ' + inputName_str + '_offset_location[i] = ' + inputName_str + 'Len * 32;\n'
                func_body_str += '        else ' + inputName_str + '_offset_location[i] = ' + inputName_str + '_offset_location[i - 1] + ' + inputName_str + '_filled_length[i - 1];\n'
                func_body_str += '        data_offset_location = ' + inputName_str + '_offset_location[i];\n'
                func_body_str += self.add_Fill_Offset_String(2)
                func_body_str += '    }\n\n'

                func_body_str += '    ' + inputName_str + '_ptr = ' + inputName_str + ';\n'

                func_body_str += '    for (i = 0; i < ' + inputName_str + 'Len; i++)\n'
                func_body_str += '    {\n'
                func_body_str += '        ' + inputName_str + '_actual_length[i] = ' + inputName_str + '_ptr->field_len;\n'
                func_body_str += '        UtilityChangeEndian(&' + inputName_str + '_actual_length[i], sizeof(BUINT32));\n'
                func_body_str += '        memset(data_offset_ptr, 0x00, 32);\n'
                func_body_str += '        memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &'+ inputName_str +'_actual_length[i], sizeof(BUINT32));\n'
                func_body_str += '        data_offset_ptr += 32;\n\n'
                func_body_str += '        ' + inputName_str + '_filled_length[i] = BOAT_ROUNDUP(' + inputName_str + '_ptr->field_len, 32);\n'
                #func_body_str += '        printf("The filled length is %x\\n", ' + inputName_str + '_filled_length[i]);\n'
                func_body_str += '        memset(data_offset_ptr, 0x00, ' + inputName_str + '_filled_length[i]);\n'
                func_body_str += '        memcpy(data_offset_ptr, ' + inputName_str + '_ptr->field_ptr, ' + inputName_str + '_ptr->field_len);\n'
                func_body_str += '        data_offset_ptr += ' + inputName_str + '_filled_length[i];\n'
                func_body_str += '        ' + inputName_str + '_ptr++;\n'
                func_body_str += '    }\n\n'
            
            elif self.is_NonFixedSize_type(input['type']) == True:
                func_body_str += '    //filled fixed size non-fixed array length\n'
                if (input['type'] == 'string'):
                    func_body_str += '    ' + inputName_str + 'Len = strlen(' + inputName_str + ');\n'

                func_body_str += '    nonFixed_actual_length = ' + inputName_str + 'Len' + ';\n'
                func_body_str += '    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));\n'
                func_body_str += '    memset(data_offset_ptr, 0x00, 32);\n'
                func_body_str += '    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));\n'
                func_body_str += '    data_offset_ptr += 32;\n\n'

                func_body_str += '    nonFixed_filled_length = ' + self.get_nonFixed_length(input) + ';\n'
                func_body_str += '    memset(data_offset_ptr, 0x00, nonFixed_filled_length);\n'
                func_body_str += '    memcpy(data_offset_ptr, ' + inputName_str + ', ' + inputName_str + 'Len' + ');\n'
                func_body_str += '    data_offset_ptr += nonFixed_filled_length;\n\n'
            else:#fixed size non-fixed array
                inputType = self.get_input_type(input['type'])
                if (inputType.find('[') != -1):
                    param_size_str = 'sizeof(' + type_mapping[inputType[:inputType.find('[')]] + ')'
                else:
                    param_size_str = 'sizeof(' + type_mapping[inputType] + ')'

                func_body_str += '    //filled fixed size non-fixed array length\n'
                func_body_str += '    nonFixed_actual_length = ' + inputName_str + 'Len' + ';\n'
                func_body_str += '    UtilityChangeEndian(&nonFixed_actual_length, sizeof(BUINT32));\n'
                func_body_str += '    memset(data_offset_ptr, 0x00, 32);\n'
                func_body_str += '    memcpy(data_offset_ptr + 32 - sizeof(BUINT32), &nonFixed_actual_length, sizeof(BUINT32));\n'
                func_body_str += '    data_offset_ptr += 32;\n\n'

                func_body_str += '    //filled fixed size non-fixed array data\n'
                func_body_str += '    ' + inputName_str + '_ptr = ' + inputName_str + ';\n'
                func_body_str += '    for(i = 0; i < ' + inputName_str + 'Len; i++)\n'
                func_body_str += '    {\n'
                func_body_str += '        memcpy(fixedsize_bytes32, ' + inputName_str + '_ptr, ' + param_size_str + ');\n'
                if self.is_signed_value_type(input['type']) == True:
                    func_body_str += '        if (*' + inputName_str + '_ptr >= 0)\n'
                    func_body_str += '        {\n'
                    func_body_str += '            memset(data_offset_ptr, 0x00, 32);\n'
                    func_body_str += '        }\n'
                    func_body_str += '        else\n'
                    func_body_str += '        {\n'
                    func_body_str += '            memset(data_offset_ptr, 0xFF, 32);\n'
                    func_body_str += '        }\n'
                else:
                    func_body_str += '        memset(data_offset_ptr, 0x00, 32);\n'
                func_body_str += self.add_FixedSize_String(inputType, '','fixedsize_bytes32',param_size_str, 2)
                func_body_str += '        ' + inputName_str + '_ptr++;\n'
                func_body_str += '    }\n\n'
            i = i + 1

        if self.is_Change_Blockchain_State(abi_item):
            # for state-less funciton call
            func_body_str += '    call_result_str = BoatPlatoneCallContractFunc(tx_ptr, data_field.field_ptr, data_field.field_len);\n\n'
        else:
            # for stateful transaction
            func_body_str += '    boat_try(BoatPlatoneTxSetData(tx_ptr, &data_field));\n\n'
            func_body_str += '    boat_try(BoatPlatoneTxSend(tx_ptr));\n\n'
            func_body_str += '    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);\n\n'

        # Cleanup Label
        func_body_str += '''
    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        '''

        func_body_str += '\n    BoatFree(data_field.field_ptr);\n'

        if self.is_Change_Blockchain_State(abi_item):
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
        #print(c_func_object.h_file_content)
        #print(c_func_object.c_file_content)
        




if __name__ == '__main__':
    main()

