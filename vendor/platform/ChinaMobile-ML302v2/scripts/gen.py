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


# This python script generates configure macros of specific platform.


import sys
import os.path
import string

license =  '/******************************************************************************\n' + \
 '* Copyright (C) 2018-2021 aitos.io\n'                                                        + \
 '*\n'                                                                                         + \
 '* Licensed under the Apache License, Version 2.0 (the "License");\n'                         + \
 '* you may not use this file except in compliance with the License.\n'                        + \
 '* You may obtain a copy of the License at\n'                                                 + \
 '*\n'                                                                                         + \
 '*     http://www.apache.org/licenses/LICENSE-2.0\n'                                          + \
 '*\n'                                                                                         + \
 '* Unless required by applicable law or agreed to in writing, software\n'                     + \
 '* distributed under the License is distributed on an "AS IS" BASIS,\n'                       + \
 '* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n'                + \
 '* See the License for the specific language governing permissions and\n'                     + \
 '* limitations under the License.\n'                                                          + \
 '*****************************************************************************/\n'            + \
'\n\n'


class ConfigContentGen():
    def __init__(self):
        self.config_content = ''

    def save_config_file(self):
            config_file_name = './vendor/platform/include/boatconfig.h'
            #config_file_name = './boatconfig.h'
            with open(config_file_name, 'w') as config_file_handle:
                config_file_handle.write(self.config_content)
    
    def gen_config_content(self):
        self.config_content += '\n'
        self.config_content += '#define  RPC_USE_MBEDHTTPPORT     1'
        self.config_content += '\n'

    def gen_config_tips(self, platform):
        self.config_content +=license
        self.config_content += \
            '/****************************************************************************\n' +\
            ' THIS FILE IS AUTO GENERATED FOR SPECIAL PLATFORM ' + platform + '\n'            +\
            ' DO NOT MODIFY THIS FILE MANUALLY\n'                                             +\
            ' ****************************************************************************/\n\n' 
    
    def gen_condHead(self):  
        self.config_content += '#ifndef __BOATCONFIG_H__\n'
        self.config_content += '#define __BOATCONFIG_H__\n'
        self.config_content += '\n\n'

    def gen_condTail(self):  
        self.config_content += '\n\n'
        self.config_content += '#endif\n'

def main():
    argc  = len(sys.argv)
    
    configContent_obj = ConfigContentGen()

    #generate tips, input parameter argv[1] is the platform name
    configContent_obj.gen_config_tips(sys.argv[1])

    #generate conditional compilation head
    configContent_obj.gen_condHead()

    #generate chain config content
    i = 2
    while  i < argc:
        strlist = sys.argv[i].split('=')
        configContent_obj.config_content += '#define' + '  ' + \
                                             strlist[0].replace('BOAT_', '') + ' '*5 + strlist[1]
        configContent_obj.config_content += '\n'
        i += 1        
    
    #generate RPC config content
    configContent_obj.gen_config_content()

    #generate conditional compilation tail
    configContent_obj.gen_condTail()

    #save config file
    configContent_obj.save_config_file()

if __name__ == '__main__':
    main()