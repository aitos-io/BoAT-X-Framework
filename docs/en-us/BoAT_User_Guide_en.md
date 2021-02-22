# BoAT User Guide

## Introduction

### Overview
This article introduces the functions and usage of BoAT IoT Framework SDK 1.x.  
The intended readers of this article are customers who integrate the BoAT IoT Framework SDK.

### Abbreviated terms
|ABI|Application Binary Interface|
| :-----| :-----|
API|Application Programming Interface|
BoAT|Blockchain of AI Things|
IoT|Internet of Things|
JSON|JavaScript Object Notation|
OS|Operating System|
RLP|Recursive Length Prefix (Encoding/Decoding)|
RPC|Remote Procedure Call|
RTOS|Real Time Operating System|
SDK|Software Development Kit|
WASM|Web Assembly|



## Function and architecture
BoAT IoT Framework is a C language blockchain client framework for cellular modules, which is easy to be transplanted to various modules and helps IoT applications based on cellular modules connect to the blockchain and realize data on-chain services. The functions provided by the framework SDK to IoT applications include initiating on-chain transactions, automatic generation of smart contract C interface code, calling smart contracts, and managing blockchain keys.

**Supported blockchain：**  
Ethereum  
PlatONE 


**Supported Target Operating System：**  
linux 


**Supported Build Operating System：**  
linux/cygwin  

**Main features：**  
Blockchain account (wallet) parameter configuration  
Blockchain key pair generation  
Blockchain account creation/loading/unloading  
Transfer transaction  
Smart contract call (automatically generate C call interface)  
Smart contract call (manual construction)  


 ### Position in the system
 BoAT IoT Framework runs on the application processor of the cellular module in the form of a software lib library. The SDK is provided in the form of C source files and compiled in the development environment of the cellular module.

 For the cellular module in the form of OpenCPU, the BoAT IoT Framework library is linked by the IoT Application to form an IoT application with blockchain link capabilities.

 Figure 2-1 shows the position of BoAT IoT Framwork in the OpenCPU module. As an application layer protocol, BoAT is located above the existing protocol stack of the module and provides blockchain services to IoT Application. The peer layer of BoAT is a blockchain network.  
 
 ![BoAT in System](/en-us/images/BoAT_User_Guide_en-F2-1-BoAT_in_system.png)
 Figure 2-1 The location of the BoAT in the system

 For non-OpenCPU cellular modules, the BoAT IoT Framework library is linked by the module firmware and expanded into AT commands by the module manufacturer for invocation of IoT applications on the host computer, no longer repeated.

 ### SDK architecture
 BoAT IoT Framework SDK is shown in Figure 2-2, which mainly includes Wallet API, blockchain client interface protocol, remote procedure call interface, common components, hardware dependent components and tool components.  
 
 ![BoAT Architecture](/en-us/images/BoAT_User_Guide_en-F2-2-BoAT_architecture.png)
 Figure 2-2 BoAT Architecture
 
 The Wallet API is an interface that the SDK provides to the IoT Application to call, including the SDK public interface and wallet and transaction interfaces for different blockchain protocols.  
 
 The blockchain client interface protocol mainly implements transaction interface protocols for different blockchains, and interacts with blockchain nodes through the RPC interface.

The remote procedure call(RPC) interface implements a warpper for different communication protocols. This component needs to be ported according to the specific communication method supported by the IoT device.  

Public components implement common functions such as RLP encoding, JSON codec, string processing, etc.   

Hardware dependent components are transplanted components involving different hardware, such as cryptography accelerators, secure storage, random numbers, etc. This component needs to be transplanted according to specific hardware. SDK also provides a set of default Hardware dependent components witch implementations by software.  

The tool component provides a set of  Python tools, that be used to generate C language contract call interface of smart contract ABI interface based on Solidity or WASM C++.  



 ### SDK code structure
 
 |Root directory|First-level sub-directory|Secondary sub-directory|description|
 | :----------- | :---------------------- | :-------------------- | :---------|
 |\<SDKRoot\>   |                         |                       |           |
 |              |build                    |                       |Directory to store object and executable files|
 |              |contract                 |                       |Contract ABI and generated C interface file|
 |              |demo                     |                       |Demo application|
 |              |docs                     |                       |API reference manual|
 |              |hwdep                    |                       |Hardware dependencies|
 |              |                         |default                |A default pure software version for development|
 |              |include                  |                       |Header files for application to include.  Application #include "boatiotsdk.h" only.|
 |              |lib                      |                       |Lib files for application to link with|
 |              |sdk                      |                       |SDK source|
 |              |                         |cJSON                  |An open source JSON parser|
 |              |                         |include                |Header files for SDK internal use|
 |              |                         |protocol               |Blockchain client protocol implementation|
 |              |                         |rlp                    |RLP encoder|
 |              |                         |rpc                    |Remote procedure call wrapper|
 |              |                         |utilities              |Utility APIs|
 |              |                         |wallet                 |SDK entry API implementation|
 |              |tests                    |                       |Demo and tests|
 |              |tools                    |                       |Tools for generating C interface from contract ABI|

Note: The build and lib directories are generated during compilation. After the compilation is complete, only the include, lib in the first level directory, and the header files in contract/generated are needed by the application.


## SDK compilation
### Software dependency
BoAT IoT Framework SDK depends on the following software:

|Dependent software|requirements|Build environment|Target environment|
| :-----| :-----| :-----| :-----|
|Host OS|linux，Or Cygwin on Windows|Required||
|Target OS|linux||Required|
|Compiler|gcc，Need to support c99 (9.3.0 is tested)|Required||
|Cross-compiler|arm-oe-linux-gnueabi-gcc 4.9.2 is tested|Required||
|Make|GNU Make (4.3 is tested)|Required||
|Python|Python 3.8.3 (Python 2.7 is also compatible)|Required||
|curl|libcurl and its development files (7.55.1 is tested)|Required|Required|
|OpenSSL|openssl and its development files (1.1.1d is tested)|Required|Required|

Before compiling the SDK and using it, you need to make sure that these software have been installed. On Ubuntu, use the apt install command to install the corresponding package. Under Cygwin, use the Setup program that comes with Cygwin to install.

Under Windows, the SDK does not support compilation in environments other than Cygwin. If you must run outside of Cygwin (for example, a cross-compiler using Windows as the Build environment), please refer to section 3.5.2 to adjust the compiled file.

When porting SDK on RTOS, libcurl and openssl dependencies should be ported.

### Preparation for compilation
#### SDK source path
In the saving path of the SDK source code, starting from the root directory, all directory names at all levels should be composed of English letters, numbers, underscores, and minus signs, and avoid spaces, Chinese, and special symbols such as plus signs, @, and brackets.  
  
  
  
For example, the following is a suitable path:  
/home/developer/project-blockchain/boatiotsdk  
C:\Users\developer\Documents\project\boatiotsdk  

The following paths are not suitable:  
/home/developer/project+blockchain/boatiotsdk  
C:\Documents and Settings\developer\project\boatiotsdk  

If you can't avoid the unsuitable characters above in the path, please use the following methods to avoid：  
For linux: In a path without unsuitable characters, create a symbolic link to the SDK directory: ln -s \<SDKRoot\> boatiotsdk, and compile under the path of the symbolic link.  
For Windows: use the SUBST Z: \<SDKRoot\> command to create a virtual drive letter Z: (or other unused drive letter), and compile under the Z: drive.


#### Referenced external environment variables
Modify the environment variables in the following files of the SDK as needed：  
\<SDKRoot\>/external.env：Configure the INCLUDE path that the external compilation environment depends on  
\<SDKRoot\>/hwdep/default/hwdep.env：Configure the INCLUDE path related to hardware or operating system  

During Host compilation, if gcc and binutils are already installed in the system, there is usually no need to modify these environment variable configurations.  
During cross compilation, if the cross compilation environment needs to configure a specific INCLUDE path, you need to add the path to the above file.  

#### SDK Option configuration
Modify the configuration in the following files of the SDK as needed:  
\<SDKRoot\>/include/boatoptions.h  

These options include the printing level of LOG, the enabled blockchain protocol, etc. See the description in the header file for details.  

### Automatic generation of contract C interface code
Smart contracts are executable codes on the blockchain, executed on blockchain virtual machines (such as EVM and WASM), and called by the client in the form of remote procedure calls (RPC).

Different virtual machines and contract programming languages have different application binary interfaces (ABI). When a client calls a contract function through RPC, it must follow the corresponding ABI assembly interface.

The SDK provides the following tools to generate the corresponding C interface code according to the contract ABI, so that in the C code, the smart contract on the chain can be called through the generated interface code like a general C function:  

|**Conversion tool**|**use**|
| :-----| :-----| 
|\<SDKRoot\>/tools/eth2c.py|According to the ABI of Ethereum Solidity, generate C calling code|
|\<SDKRoot\>/tools/platon2c.py|Generate C calling code according to PlatON (WASM) ABI|
|\<SDKRoot\>/tools/platone2c.py|Generate C calling code according to PlatONE (WASM) ABI|

Since contract programming languages generally support object-oriented, and C language does not support object-oriented, and cannot use a unified paradigm to transfer objects, only contract functions whose parameter types are consistent with the built-in types of C language can be converted into C calling code by tools.  

Before making the call, you first need to compile the contract, and copy the ABI interface description JSON file generated in the contract compilation to the corresponding directory of the SDK： 

|**Contract ABI storage path**|**use**|
| :-----| :-----| 
|\<SDKRoot\>/contract/ethereum|Copy the ABI JSON file of Ethereum Solidity to this directory|
|\<SDKRoot\>/contract/platon|Copy PlatON (WASM) ABI JSON file to this directory|
|\<SDKRoot\>/contract/platone|Copy PlatONE (WASM) ABI JSON file to this directory|

*Note 1: ABI's JSON file must have ".json" as the file name suffix.*  
*Note 2: If multiple contract ABIs are converted and compiled together, different contracts must have different JSON file names, even if they belong to different blockchains.*

During the compilation process, the automatic generation tool will generate the corresponding C interface calling code according to the contract ABI JSON file. If the automatic generation of C interface fails during compilation, you need to delete the unsupported ABI JSON file (or delete the unsupported interface) from the corresponding directory of \<SDKRoot\>/contract, write the C code manually, and assemble the ABI interface. See section 4.5.
### Host compilation
Host compilation means that the compilation environment is consistent with the target environment, for example, to compile x86 programs on x86. There are usually two scenarios for using Host compilation： 
1. In the software commissioning phase, the software functions are tested on the PC.
2. The target software itself runs on devices based on x86/x86-64 processors, such as some edge gateways.

#### Use linux as the compilation environment
Compile Host based on Linux distribution (such as Ubuntu). Generally, there is no need to configure the compilation environment, just make sure that the dependent software has been installed.  
Follow the steps below to compile：
1. Store the SDK source code in a path that meets the requirements of section 3.2.1
2. Optional: Put the ABI JSON file of the smart contract to be called in the corresponding directory of \<SDKRoot\>/contract (see section 3.3)
3. In the \<SDKRoot\> directory, execute the following command：  
$make boatlibs

After the compilation is complete, the generated library file is in ./lib. The application should include the header files under ./include and link the libraries under ./lib to achieve the function of accessing the blockchain. See section 4.1.

#### Use Cygwin as the compilation environment
On Windows, the SDK does not support compilation in environments other than Cygwin, nor does it support compilation with compilers other than gcc.

The compilation steps are the same as under linux.

### cross-compilation
In cross-compilation, it is generally necessary to configure the compilation configuration file according to the specific compilation environment.

#### Use linux as the compilation environment
##### Independent cross-compilation environment
Independent compilation environment means that arm-oe-linux-gnueabi-gcc (or similar cross-compiler) has been installed in the Linux system and can be called independently.

The SDK requires at least the following environment variables to be set in the system to point to the cross-compilation environment:  

|**Environment variable**|**Description**|
| :-----| :-----| 
|CC|Point to the cross compiler gcc executable file|
|AR|Point to the cross compiler ar executable file|


When the environment variables of CC and AR are not defined in the environment, GNU make will default CC=cc and AR=ar. Usually, the gcc and bintuils compilation environment of the host are installed in the Linux system. Therefore, if the above environment variables are not defined, the host compilation will be executed.

When configuring a cross-compilation environment, it is usually necessary to execute a specific shell script to set the above environment variables to point to the cross-compilation environment. For the bash shell, commands similar to the following are usually executed:  
$source cross_compiler_config.sh  
or  
$. cross_compiler_config.sh  

The "cross_compiler_config.sh" in the above example is not a script in this SDK, but a configuration script in the cross-compilation environment. For the specific name and location, please refer to the relevant instructions of the cross-compilation environment.  
The "source" or "." in the example is necessary, which makes the script execute in the context of the current shell, so the modification of the environment variables in the script can take effect in the current shell.

You can execute the following command to view the environment variable settings in the current shell：  
$export

If the environment variables CC and AR have been set, you can execute the following command to view the current version of CC and AR to confirm whether the desired cross-compilation environment has been pointed to：  
${CC} -v  
${AR} -v  

After the above configuration is completed, follow the steps in section 3.4.1 to compile.

##### A cross-compilation environment integrated with the module development environment
Some OpenCPU modules have integrated a supporting cross-compiler environment in the development environment provided by them, so that customers do not need to install a cross-compiler separately in the Linux system. This is especially convenient for developing application software on multiple modules of different models on a host computer without repeatedly switching the cross-compilation environment.

###### The module development environment is compiled with GNU make
If the module development environment uses GNU make as the compilation project (makefiles in the source code directories at all levels), you can adjust the compilation configuration for the BoAT IoT Framework SDK and incorporate it into the integrated module development environment for compilation.  

Usually, the example of the customer code is provided in the module development environment, and the compilation configuration of the example code of the customer is included in the compilation system.First copy the \<SDKRoot\> directory (bootiotsdk is the directory name in the following example) to the source code in the module development environment, and then modify the Makefile in example directory, to add a target to compile BoAT IoT Framework SDK.  
E.g：  
Assuming that in the original compilation environment, the Makefile of the example in customer source code is as follows：  
```
.PHNOY: all  
all:  
    $(CC) $(CFLAGS) example.c -o example.o 

clean:  
	-rm -rf example.o
```
Adjusted to：
```
.PHNOY: all boatiotsdkall boatiotsdkclean  
all: boatiotsdkall  
    $(CC) $(CFLAGS) example.c -o example.o 

clean: boatiotsdkclean    
	-rm -rf example.o  

boatiotsdkall:
	make -C boatiotsdk boatlibs

boatiotsdkclean:
	make -C boatiotsdk clean

```
Among them, boatiotsdk is the directory where the SDK is located, and the -C parameter after make specifies to enter the boatiotsdk directory and then compile according to Makefile.

*Note: In the Makefile, the command under target must start with a Tab (ASCII code 0x09), not a space.*

The above steps are only used to compile the SDK library. After the SDK library compile completed, the compiled library needs to be integrated into the module development environment. See section 4.1 for details.

###### The module development environment is compiled with non-GNU make
Since BoAT IoT Framework SDK uses GNU make as the compilation project, if the module development environment uses non-GNU Make compilation projects (such as Ninja, ant, etc.), or uses the automatic generation tools of the compilation project (such as automake, CMake), it cannot Compile the SDK directly in the module development environment.  

To compile the SDK in such a module development environment, you need to release the gcc and binutils compilation tools in the module development environment, and configure the environment variables described in section 3.5.1.1 so that they can be called in the system, which is equivalent to independent cross-compilation Environment, and then compile the SDK.

#### Use Windows as the compilation environment
Under Windows, the SDK does not support compilation in environments other than Cygwin. If the cross compiler with Windows as the build environment can only be run outside Cygwin, the compilation environment and compilation configuration files should be adjusted.  
When cross-compiling outside of Cygwin, you still need to install Cygwin and adjust the Makefile.

###### Install Cygwin
When cross-compiling outside of Cygwin, Cygwin still needs to be installed. Because the SDK compilation project relies on some Cygwin tools. The tools that need to be installed are as follows：

###### Other adjustments
When cross-compiling outside of Cygwin, in addition to the previous section, the following adjustments are required:

1.	Try make, if it prompts that the path is wrong, change the corresponding path separator in the Makefile from "/" to "\\". Don't change all "/" to "\\" at the beginning, because the Windows version of some tools derived from linux can recognize "/" as a path separator.
2.	Configure the environment variables described in section 3.5.1.1 to point to the correct cross-compilation environment. In these environment variables, the path should be separated by "\\".

### Compile and run Demo
#### ready
SDK provides Demo based on Ethereum and PlatONE. Before running these demos, you need to install the corresponding blockchain node software (or have known nodes) and deploy the smart contracts required by the demo.

#### Compile Demo
Execute the following commands in the <SDKRoot> directory to compile the SDK call Demo：
$make tests

The generated Demo program is located at：\<SDKRoot\>/build/tests/boattest


### Trouble shooting in compilation
1.  A message similar to "Makefile: 120: *** missing delimiter. Stop" is prompted during compilation.  

This problem is generally caused by the command under target in the Makefile that does not start with Tab (ASCII code 0x09). Note that when you press the Tab key, the text editor may replace the Tab character with a number of spaces. The text editor should be set not to replace Tab with spaces.

2. The prompt "curl/curl.h" cannot be found during compilation  

This problem is caused by the fact that curl and its development files are not installed in the system. For Host compilation on a Linux distribution, please note that only installing the curl package is not enough, you also need to install its development file package. The development file package has different names in different linux distributions, usually named similar to curl-devel, or libcurl. For details, please refer to the package management tool of the Linux distribution you are using.

If curl is compiled with source code and is not installed in the system directory, you should specify its search path in external.env, and specify the path where the curl library is located when linking.

In cross-compilation, pay special attention to the search path and library should point to the header files and libraries in the cross-compilation environment, and should not point to the path on the Host that executes the compilation.

3. The prompt " openssl/evp.h " cannot be found during compilation  

The problem is caused by the fact that OpenSSL and its development files are not installed in the system. Refer to curl.

4. When cross-compiling and linking, it prompts that the byte order, bit width or ELF format does not match  

5. The compilation prompts that the executable file could not be found, or that the parameters are incorrect  

## Programming model
### Header files and libraries
After the BoAT IoT Framework SDK compile completed, the application can initiate blockchain transactions or call smart contracts through the SDK header files and libraries.

### SDK initialization and destruction
Before calling the SDK, you must call BoatIotSdkInit() to initialize the global resources of the SDK：

### Blockchain wallet create/load/unload/delete
A wallet is a collection of attributes of a blockchain account. These attributes include key attributes such as private keys and blockchain node URLs. Before initiating a transaction or invoking a smart contract, a wallet must be created or a previously saved wallet must be loaded.
#### Create and load wallet
The SDK supports two types of wallets: one-time wallets and persistent wallets.

One-time wallets are created temporarily when in use, only exist in memory, and become invalid after shutdown.
Persistent wallets will be persistently saved when they are created. After turning off and on again, the persistent wallet that has been created before can be loaded.

#### unload wallet
Unloading the wallet will unload the loaded wallet from the memory. It will not delete the persistent wallet, but the persistent wallet cannot be used until it is loaded again.
#### Delete wallet
Deleting the wallet will delete the persistent wallet. If the wallet has been loaded before the deletion, after the deletion, the persistent wallet will become a one-time wallet, which can still be used before being unload.

### Key generation
The key that needs to be configured when creating a wallet can be inputted externally or generated by the SDK.

### Transfer call
Token transfer from this account to other accounts (not all blockchain protocols support transfers).

### Contract call (automatically generated)
#### Restrictions on automatically generated contracts
Since contract programming languages generally support object-oriented, and C language does not support object-oriented, and cannot use a unified paradigm to transfer objects, only contract functions whose parameter types are consistent with the built-in types of C language can be converted into C calling code by tools.

#### Automatically generated contract call interface
The successfully generated contract call interface is the following C function：

#### Transaction initialization
To call the automatically generated contract interface, first initialize a transaction object, and then call the generated contract interface.  
Even if the called contract function does not change the state of the blockchain, a transaction object needs to be initialized first.
#### Call contract interface
After completing the transaction initialization, you can call the automatically generated contract interface：

### Manually construct contract calls
If the automatic generation tool cannot generate the C call interface, you need to manually construct the transaction message.

The manual construction of transactions needs to follow the ABI interface of the specific blockchain protocol.


## Suggestions for porting SDK to RTOS
If the SDK is ported to RTOS, the following points should generally be followed:


## BoAT's extended AT command suggestion
### Create/load wallet AT^BCWALT
### Unload wallet AT^BUWALT
### Delete wallet AT^BDWALT
### Contract function call AT^BCALLFUNC
### Transfer AT^BTRANS

## Reference
BoAT API Reference Manual
