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

**Supported blockchain:**  
Ethereum  
PlatONE 


**Supported Target Operating System:**  
linux 


**Supported Build Operating System:**  
linux/cygwin  

**Main features:**  
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

If you can't avoid the unsuitable characters above in the path, please use the following methods to avoid:  
For linux: In a path without unsuitable characters, create a symbolic link to the SDK directory: ln -s \<SDKRoot\> boatiotsdk, and compile under the path of the symbolic link.  
For Windows: use the SUBST Z: \<SDKRoot\> command to create a virtual drive letter Z: (or other unused drive letter), and compile under the Z: drive.


#### Referenced external environment variables
Modify the environment variables in the following files of the SDK as needed:  
\<SDKRoot\>/external.env: Configure the INCLUDE path that the external compilation environment depends on  
\<SDKRoot\>/hwdep/default/hwdep.env: Configure the INCLUDE path related to hardware or operating system  

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

Before making the call, you first need to compile the contract, and copy the ABI interface description JSON file generated in the contract compilation to the corresponding directory of the SDK: 

|**Contract ABI storage path**|**use**|
| :-----| :-----| 
|\<SDKRoot\>/contract/ethereum|Copy the ABI JSON file of Ethereum Solidity to this directory|
|\<SDKRoot\>/contract/platon|Copy PlatON (WASM) ABI JSON file to this directory|
|\<SDKRoot\>/contract/platone|Copy PlatONE (WASM) ABI JSON file to this directory|

*Note 1: ABI's JSON file must have ".json" as the file name suffix.*  
*Note 2: If multiple contract ABIs are converted and compiled together, different contracts must have different JSON file names, even if they belong to different blockchains.*

During the compilation process, the automatic generation tool will generate the corresponding C interface calling code according to the contract ABI JSON file. If the automatic generation of C interface fails during compilation, you need to delete the unsupported ABI JSON file (or delete the unsupported interface) from the corresponding directory of \<SDKRoot\>/contract, write the C code manually, and assemble the ABI interface. See section 4.5.
### Host compilation
Host compilation means that the compilation environment is consistent with the target environment, for example, to compile x86 programs on x86. There are usually two scenarios for using Host compilation: 
1. In the software commissioning phase, the software functions are tested on the PC.
2. The target software itself runs on devices based on x86/x86-64 processors, such as some edge gateways.

#### Use linux as the compilation environment
Compile Host based on Linux distribution (such as Ubuntu). Generally, there is no need to configure the compilation environment, just make sure that the dependent software has been installed.  
Follow the steps below to compile:
1. Store the SDK source code in a path that meets the requirements of section 3.2.1
2. Optional: Put the ABI JSON file of the smart contract to be called in the corresponding directory of \<SDKRoot\>/contract (see section 3.3)
3. In the \<SDKRoot\> directory, execute the following command:  
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

You can execute the following command to view the environment variable settings in the current shell:  
$export

If the environment variables CC and AR have been set, you can execute the following command to view the current version of CC and AR to confirm whether the desired cross-compilation environment has been pointed to:  
${CC} -v  
${AR} -v  

After the above configuration is completed, follow the steps in section 3.4.1 to compile.

##### A cross-compilation environment integrated with the module development environment
Some OpenCPU modules have integrated a supporting cross-compiler environment in the development environment provided by them, so that customers do not need to install a cross-compiler separately in the Linux system. This is especially convenient for developing application software on multiple modules of different models on a host computer without repeatedly switching the cross-compilation environment.

###### The module development environment is compiled with GNU make
If the module development environment uses GNU make as the compilation project (makefiles in the source code directories at all levels), you can adjust the compilation configuration for the BoAT IoT Framework SDK and incorporate it into the integrated module development environment for compilation.  

Usually, the example of the customer code is provided in the module development environment, and the compilation configuration of the example code of the customer is included in the compilation system.First copy the \<SDKRoot\> directory (bootiotsdk is the directory name in the following example) to the source code in the module development environment, and then modify the Makefile in example directory, to add a target to compile BoAT IoT Framework SDK.  
E.g:  
Assuming that in the original compilation environment, the Makefile of the example in customer source code is as follows:  
```
.PHNOY: all  
all:  
    $(CC) $(CFLAGS) example.c -o example.o 

clean:  
	-rm -rf example.o
```
Adjusted to:
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
When cross-compiling outside of Cygwin, Cygwin still needs to be installed. Because the SDK compilation project relies on some Cygwin tools. The tools that need to be installed are as follows:  

|**Tools needed**|**use**|
| :------------------| :-----| 
|find|Cygwin's find.exe is needed to search the subdirectories to be compiled recursively. Windows comes with another FIND.EXE with the same name but completely different functions, which cannot be used.|
|rm|Used to delete specified directories and files. The built-in RMDIR/RD and DEL commands of the Windows cmd shell can only be used to delete directories (trees) and files, respectively, and are not compatible with Cygwin's rm.exe in terms of syntax.|
|mkdir|Used to create one or more levels of directories. The built-in MKDIR/MD commands of Windows cmd shell have the same function, but the syntax is not compatible.|
|GNU make|You can install make in Cygwin, or you can compile GNU make based on a compiler on Windows (such as Microsoft Visual Studio). The latter does not depend on Cygwin.|

After installing Cygwin, you need to configure its path. Since some Cygwin tools that SDK compilation relies on have the same names as the Windows built-in tools, you must ensure that the relevant tools referenced in the compilation point to the Cygwin version.

First, in the cmd shell that executes the compilation, execute the following command to increase the search path of Cygwin:  
set PATH=%PATH%;\<Path_to_Cygwin\>\bin  
Where \<Path_to_Cygwin\> is the absolute path of the Cygwin installation directory, such as: C:\Cygwin64  

*Note: The above commands can be written in a bat batch file, or directly added to the Windows system environment variables for easy calling. Note that if you directly add the Windows system environment variables, you must not place Cygwin before the %SystemRoot%\System32 path, otherwise the find version of Cygwin will be called by mistake when calling the FIND command of Windows in other scenarios, which will affect other scenarios Use Windows built-in commands.*  

Then, modify boatiotsdk\Makefile, add the path to the dependent tool:
```
# Commands
CYGWIN_BASE := C:\cygwin64  # Modify to actual path to Cygwin
BOAT_RM := $(CYGWIN_BASE)\bin\rm -rf
BOAT_MKDIR := $(CYGWIN_BASE)\bin\mkdir
BOAT_FIND := $(CYGWIN_BASE)\bin\find

```

Taking Windows 10 as an example, the method to add the search path to the Windows environment variable is:  
a)	Right-click on the Windows logo menu and select "System"  
 ![System](/en-us/images/system.png)   
b)	Click "System Information" on the "About" page  
 ![System Information](/en-us/images/System_Information.png)    
c)	Click "Advanced System Settings" on the "System" page  
 ![Advanced System Settings](/en-us/images/Advanced_System_Settings.png)  
d)	Click "Environment Variables" in the "System Properties" page  
 ![Environment Variables](/en-us/images/Environment_Variables.png)   
e)	Click "Path" in "System Variables" on the "Environment Variables" page, and click "Edit"  
 ![Path](/en-us/images/Path.png)    
f)	On the "Edit Environment Variables" page, click "New", add the bin path under the Cygwin installation directory, and make sure that the new path is located anywhere after the path %SystemRoot%\system32  
 ![Edit Environment Variables](/en-us/images/Edit_Environment_Variables.png)


###### Other adjustments
When cross-compiling outside of Cygwin, in addition to the previous section, the following adjustments are required:

1.	Try make, if it prompts that the path is wrong, change the corresponding path separator in the Makefile from "/" to "\\". Don't change all "/" to "\\" at the beginning, because the Windows version of some tools derived from linux can recognize "/" as a path separator.
2.	Configure the environment variables described in section 3.5.1.1 to point to the correct cross-compilation environment. In these environment variables, the path should be separated by "\\".

### Compile and run Demo
#### ready
SDK provides Demo based on Ethereum and PlatONE. Before running these demos, you need to install the corresponding blockchain node software (or have known nodes) and deploy the smart contracts required by the demo.  

The smart contract used by the demo and its ABI JSON file are placed in:  

|**Demo smart contract**|**Contract ABI JSON file**|**use**|
| :-----| :-----| :-----| 
|\<SDKRoot\>/contract/ethereum/StoreRead.sol|\<SDKRoot\>/contract/ethereum/StoreRead.json|Ethereum demo|
|\<SDKRoot\>/contract/platone/my_contract.cpp|\<SDKRoot\>/contract/platone/my_contract.cpp.abi.json|PlatONE demo|


Before running Ethereum's Demo, you need to install the Ethereum node simulator ganache, as well as the Ethereum smart contract compilation deployment tool truffle.  
Ganache and truffle tools can visit this website: https://truffleframework.com  
Ganache has a ganache-cli version of the command line interface, and a Ganache version of the graphical interface. The ganache-cli of the command line interface and the Ganache 1.x version of the graphical interface will not be saved. If the process of ganache-cli or Ganache 1.x is terminated, the deployed contract will be lost. The command truffle migrate - reset Redeploy the contract, the address of the redeployed contract may change. The Ganache 2.x version of the graphical interface can create a Workspace save state. After closing and reopening the Workspace next time, the deployed contract still does not need to be redeployed.  
In addition to using the ganache simulator, you can also use the Ethereum test network such as Ropsten (you need to apply for a free test token).  


Before running the PlatONE Demo, you need to install the PlatONE node, as well as smart contract compilation and deployment tools.  
PlatONE source code and tools can visit this website: https://platone.wxblockchain.com  


After completing the node (or simulator) deployment, you need to follow the instructions on the truffle and PlatONE websites to deploy the Demo smart contract. After the smart contract is successfully deployed, the contract address will be generated.

The Demo C code that calls the smart contract is placed in:  

|**Demo C code**|**use**|
| :-----| :-----|
|\<SDKRoot\>/tests/case_30_ethereum.c|Ethereum demo use case|
|\<SDKRoot\>/tests/case_32_platone.c|PlatONE demo use case|

Before compiling the Demo, you need to modify the following parts of the Demo C code:
1. Search for "Set Node URL" and fill in the node URL (including port) as the IP address and RPC port of the actual deployed node or simulator
2. Search for "Set Private Key" and set the client private key to:  
a) For Ethereum, set it to the private key of any account generated by Ganache  
b) For PlatONE, there is no need to modify the private key in the Demo  
3. Search for "Set Contract Address" and modify it to the deployment address of the Demo contract.
4. For Ethereum Demo only, search for "Set Recipient Address" and modify it to the address of any account other than section 2.a) previous generated by ganache.
#### Compile Demo
Execute the following commands in the \<SDKRoot\> directory to compile the SDK call Demo:  
$make tests  
  
  
The generated Demo program is located at: \<SDKRoot\>/build/tests/boattest


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
This problem is usually because in cross-compilation, some libraries refer to the Host library, and the Obj file is generated by cross-compilation, or some libraries are 32-bit and the other is 64-bit. You should carefully check the paths of all libraries to avoid mixed links between Host and Target libraries, or libraries with different bit widths.  
  
  
You can use the following command to check whether the library file is the ARM version or the x86 version, and the bit width:  
$file \<lib or obj file name\>

5. The compilation prompts that the executable file could not be found, or that the parameters are incorrect  
Common tips:  
'make' is not an internal or external command, nor is it an executable program or batch file.  
mkdir… command syntax is incorrect.  
FIND: The parameter format is incorrect  
  
  
This problem is generally caused by compiling under Windows, but Cygwin is not installed, or the paths of BOAT_RM, BOAT_MKDIR, and BOAT_FIND are not correctly configured in the Makefile. Please refer to section 3.5.2 to install Cygwin and configure Makefile.

## Programming model

### Header files and libraries
After the BoAT IoT Framework SDK compile completed, the application can initiate blockchain transactions or call smart contracts through the SDK header files and libraries.

After the SDK is compiled, only the following files are needed for the application when compiling and linking:

- All header files under \<SDKRoot\>/include 
- All library files under \<SDKRoot\>/lib 
- If the C interface code is automatically generated according to the contract ABI JSON file, and all the header files under \<SDKRoot\>/ contract/generated are used

1. Refer to the SDK header file in the application

Add \<SDKRoot\>/include to the header file search path of the application, or copy all header files under \<SDKRoot\>/include to the application header file directory.
In the application-related C code, #include "boatiotsdk.h". The header file is the entry header file of the SDK.
The application does not need to include the other header file directories of the SDK into the search path.

2. Link SDK library files in the application

(1)In the link library of the application, add three static libraries in \<SDKRoot\>/lib in turn:
libboatcontract.a libboatwallet.a libboathwdep.a

(2)In the application link library, add dynamic libraries of curl and openssl:
-lcurl -lcrypto

For cross-compilation, you should ensure that the curl and openssl versions in the development environment are consistent with those in the target version operating environment.

### SDK initialization and destruction
Before calling the SDK, you must call BoatIotSdkInit() to initialize the global resources of the SDK:
```
BOAT_RESULT BoatIotSdktInit(void);
```
After the end of use, you should call BoatIotSdkDeInit() to release resources:
```
void BoatIotSdkDeInit(void);
```

### Blockchain wallet create/load/unload/delete
A wallet is a collection of attributes of a blockchain account. These attributes include key attributes such as private keys and blockchain node URLs. Before initiating a transaction or invoking a smart contract, a wallet must be created or a previously saved wallet must be loaded.

#### Create and load wallet
The SDK supports two types of wallets: one-time wallets and persistent wallets.

One-time wallets are created temporarily when in use, only exist in memory, and become invalid after shutdown.
Persistent wallets will be persistently saved when they are created. After turning off and on again, the persistent wallet that has been created before can be loaded.

*Note: The persistence implementation method in \<SDKRoot\>/hwdep/default/storage is for reference only. In commercial products, it is recommended to consider a more secure persistence method based on actual hardware capabilities.*

When creating and loading a wallet, you should always call BoatWalletCreate() in the same thread.

When creating a wallet, you need to pass in wallet configuration parameters according to the specific blockchain protocol.

Aitos Technology provides customers with non-exclusive test node services of some blockchain protocols for free, so that customers can verify and demonstrate when integrating BoAT IoT Framework. Please contact customer service representative for relevant information.

These test nodes are only for customers' non-exclusive use in development and demonstration. Customers are not allowed to use it for commerce, public service, military, and other purposes that involve state or commercial secrets or may have significant social impact.

Once customers use these test nodes, they are deemed to have:
1. Aitos Technology reserves the right to transfer, close these test nodes, or clear node data at any time, and does not assume the obligation to notify customers in advance before transferring, closing nodes or clearing data.
2. Aitos Technology is not responsible for the direct or indirect losses incurred by customers when using the test node service, regardless of whether these losses are related to the test node service.
3. These test node services are for non-exclusive use. Any information passed to the test node may be obtained by a third party.
```
BSINT32 BoatWalletCreate(BoatProtocolType protocol_type,
                         const BCHAR *wallet_name_str,
                         const void * wallet_config_ptr,
                         BUINT32 wallet_config_size);
```
Parameters:

||||
|-----|-----|-----|
|[in]|**protocol_type**|The blockchain protocol. See boattypes.h for supported protocol.|
|[in]|**wallet_name_str**|A string of wallet name.<br>If the given \<wallet_name_str\> is NULL, a one-time wallet is created.<br>Otherwise a persistent wallet with the given name will be created or loaded.|
|[in]|**wallet_config_ptr**|Configuration (e.g. crypto key) for the wallet.<br>The exact configuration definition is determinted by the specified \<protocol_type\>.|
|[in]|**wallet_config_size**|Size (in byte) of configuration specified by \<wallet_config_ptr\>.|

**return value:**
This function returns the non-negative index of the loaded wallet.
It returns -1 if wallet creation fails.

Example:
```
   BoatEthWalletConfig wallet_config;
   BoatEthWallet *wallet_ptr;
    
    // Set Private Key
    UtilityHex2Bin( wallet_config.priv_key_array,
                    32,
                    "0x84bbaa51f0530db722055a0aab7f89720d3f5c0ab7573f5c658bd5d1530af2a2",
                    TRIMBIN_TRIM_NO,
                    BOAT_FALSE);
    
    wallet_config.chain_id = 1;
    wallet_config.eip155_compatibility = 0;
    
    // Set Node URL
    wallet_config.node_url_str[BOAT_NODE_URL_MAX_LEN-1] = '\0';
    strncpy(wallet_config.node_url_str, "http://127.0.0.1:7545", BOAT_NODE_URL_MAX_LEN-1);

    // Create one-time wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM,
                             NULL,
                             &wallet_config,
                             sizeof(BoatEthWalletConfig));

    // Create a new persistent wallet or overwrite an existed wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM,
                             "boateth.keystore",
                             &wallet_config,
                             sizeof(BoatEthWalletConfig));

    // Load an existed persistent wallet
    index = BoatWalletCreate(BOAT_PROTOCOL_ETHEREUM,
                             "boateth.keystore",
                             NULL,
                             sizeof(BoatEthWalletConfig));
```

#### unload wallet
Unloading the wallet will unload the loaded wallet from the memory. It will not delete the persistent wallet, but the persistent wallet cannot be used until it is loaded again.
```
void BoatWalletUnload(BSINT32 wallet_index);
```
Parameters:

||||
|-----|-----|-----|
|[in]|**wallet_index**|The wallet index to unload.|


#### Delete wallet
Deleting the wallet will delete the persistent wallet. If the wallet has been loaded before the deletion, after the deletion, the persistent wallet will become a one-time wallet, which can still be used before being unload.
```
void BoatWalletDelete(BCHAR * wallet_name_str);
```
Parameters:

||||
|-----|-----|-----|
|[in]|**wallet_name_str**|The wallet name to delete.|

### Key generation
The key that needs to be configured when creating a wallet can be inputted externally or generated by the SDK.

Take Ethereum as an example:
```
BOAT_RESULT BoatEthWalletGeneratePrivkey( BOAT_OUT BUINT8 priv_key_array[32] );
```
Parameters:

||||
|-----|-----|-----|
|[in]|**priv_key_array**|A 32-byte buffer to hold the generated private key.|

**return value:**
This function returns BOAT_SUCCESS if private key is successfully generated.
Otherwise it returns one of the error codes.

### Transfer call
Token transfer from this account to other accounts (not all blockchain protocols support transfers).

Take Ethereum as an example:
```
BOAT_RESULT BoatEthTransfer(BoatEthTx *tx_ptr,
                            BCHAR * value_hex_str);
```

Parameters:

||||
|-----|-----|-----|
|[in]|**tx_ptr**|Transaction pointer.|
|[in]|**value_hex_str**|A string representing the value (Unit: wei) to transfer, in HEX format like "0x89AB3C".<br>Note that decimal value is not accepted. If a decimal value such as "1234" is specified, it's treated as "0x1234".|

**return value:**
This function returns BOAT_SUCCESS if transfer is successful.
Otherwise it returns one of the error codes.

### Contract call (automatically generated)

#### Restrictions on automatically generated contracts
Since contract programming languages generally support object-oriented, and C language does not support object-oriented, and cannot use a unified paradigm to transfer objects, only contract functions whose parameter types are consistent with the built-in types of C language can be converted into C calling code by tools.

For contracts written by Solidity, the tool supports the following parameter types:
  - address
  - bool
  - uint8
  - uint16
  - uint32
  - uint64
  - uint128
  - uint256
  - int8
  - int16
  - int32
  - int64
  - int128
  - int256
  - bytes1
  - bytes2
  - bytes3
  - bytes4
  - bytes5
  - bytes6
  - bytes7
  - bytes8
  - bytes9
  - bytes10
  - bytes11
  - bytes12
  - bytes13
  - bytes14
  - bytes15
  - bytes16 
  - bytes17
  - bytes18
  - bytes19
  - bytes20
  - bytes21
  - bytes22
  - bytes23
  - bytes24
  - bytes25
  - bytes26
  - bytes27
  - bytes28
  - bytes29
  - bytes30
  - bytes31
  - bytes32
  - bytes
  - string

For WASM contracts written in C++, the tool supports the following parameter types:
  - string
  - uint8
  - uint16
  - uint32
  - uint64
  - int8
  - int16
  - int32
  - int64
  - void

If the contract function contains unsupported parameter types, the automatic conversion tool will not be able to complete the conversion, and the contract must be written manually.

#### Automatically generated contract call interface
The successfully generated contract call interface is the following C function:
```
BCHAR * <Contract ABI JSON file name >_< Contract function name>(<Wallet type> *tx_ptr, …);
```
E.g:
```
BCHAR * StoreRead_saveList(BoatEthTx *tx_ptr, Bbytes32 newEvent);
```
The first parameter of the calling interface is always the pointer of the initialized transaction object.

If the call is successful, the return value of the call interface is a string:
- For a contract call (transaction) that will change the state of the blockchain, the returned string is the hash of the transaction, expressed in the form of a HEX string.
- For contract calls that do not change the state of the blockchain, the returned string is the "result" string in the node's return message, and the "result" string represents the return value of the contract function.

*Note: The memory of the string is allocated inside the SDK. The application must copy the string to the memory allocated by the application for subsequent use, and must not modify the content of the returned string or save the pointer to the string for use.*

The contract function that changes the state of the blockchain refers to any function that changes the persistent storage information in the blockchain ledger.For example, functions that write and modify member variables in the contract are all contract functions that change the state of the blockchain.If a contract function only modifies the local variables in the function, but does not modify the contract member variables, then the contract function is a contract function that does not change the state of the blockchain.

In some contract programming languages, the two types of contract functions have a clear modifier difference on the prototype, while in other programming languages, there is no obvious mark. In the ABI, these two types of functions have different identifiers.

For example, take the following Ethereum Solidity contract as an example:
```
contract StoreRead {
    
    bytes32[] eventList;
    
    function saveList(bytes32 newEvent) public {
        eventList.push(newEvent);
    }
    
    function readListLength() public view returns (uint32 length_) {
        length_ = uint32(eventList.length);
    }
}
```
After the contract is compiled, the ABI corresponding to the two functions are described as follows:
```
"abi": [
    {
      "constant": false,
      "inputs": [
        {
          "name": "newEvent",
          "type": "bytes32"
        }
      ],
      "name": "saveList",
      "outputs": [],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "readListLength",
      "outputs": [
        {
          "name": "length_",
          "type": "uint32"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    }
]
```
In the above contract, eventList is a member variable of the contract. saveList() will change the value of eventList, which is a contract function that changes the state of the blockchain; readListLength() has a view modifier and only reads the attributes of eventList without changing its value. It is a contract function that does not change the state of the blockchain .

Pay special attention to the fact that although the contract function and C call interface function that change the state of the blockchain and do not change the state of the blockchain are very similar in prototype, the calling principles of the two are quite different.

Any change to the state of the blockchain needs to be implemented through blockchain transactions and reach a consensus across the entire network. The contract function that changes the state of the blockchain is called asynchronously. When called, it only initiates the blockchain transaction, and the contract will not be executed before the blockchain network packs the transaction into blocks. Therefore, when the contract function that changes the state of the blockchain is called, the return value is only the hash value used to identify the transaction, not the return value in the form of the contract function. When designing a smart contract, the public interface function that changes the state of the blockchain, the information it tries to return should be stored in the contract member variable, and the receipt of the transaction is queried through BoatEthGetTransactionReceipt(). After success, use another Obtained in the form of a read function.


Contract functions that do not change the state of the blockchain, only need the blockchain node to read the existing information in its database, without transactions and consensus, so the call to this type of function is a synchronous call.The return value is the return value in the form of the contract function.

#### Transaction initialization
To call the automatically generated contract interface, first initialize a transaction object, and then call the generated contract interface.  
Even if the called contract function does not change the state of the blockchain, a transaction object needs to be initialized first.

Take Ethereum as an example (different blockchain protocol parameters are different):
```
BOAT_RESULT BoatEthTxInit(BoatEthWallet *wallet_ptr,
                          BoatEthTx *tx_ptr,
                          BBOOL is_sync_tx,
                          BCHAR * gasprice_str,
                          BCHAR * gaslimit_str,
                          BCHAR *recipient_str)
```						 
Parameters:

||||
|-----|-----|-----|
|[in]|**wallet_ptr**|The wallet pointer that this transaction is combined with.|
|[in]|**tx_ptr**|Pointer a transaction object.|
|[in]|**is_sync_tx**|For a stateful transaction, specify BOAT_TRUE to wait until the transaction is mined.<br>Specifiy BOAT_FALSE to allow multiple transactions to be sent continuously in a short time.<br>For a state-less contract call, this option is ignored.|
|[in]|**gasprice**|A HEX string representing the gas price (unit: wei) to be used in the transaction.<br>Set \<gasprice\> = NULL to obtain gas price from network.<br>BoatEthTxSetGasPrice() can later be called to modify the gas price at any time before the transaction is executed.|
|[in]|**gaslimit**|A HEX string representing the gas limit to be used in the transaction.<br>BoatEthTxSetGasLimit() can later be called to modify the gas limit at any time before the transaction is executed.|
|[in]|**recipient_str**|A HEX string representing the recipient address, in HEX format like"0x19c91A4649654265823512a457D2c16981bB64F5".<br>BoatEthTxSetRecipient() can later be called to modify the recipient at any time before the transaction is executed. |

**return value:**
This function returns BOAT_SUCCESS if initialization is successful.
Otherwise it returns one of the error codes.

E.g:
```
BoatEthTx tx_ctx;
BOAT_RESULT result;
result = BoatEthTxInit(
                       wallet_ptr,
                       &tx_ctx,
                       BOAT_TRUE,
                       NULL,
                       "0x333333",
                       "0x9e7f3ae22cf97939a2e4cd68dd33bb29268a1ec9");
```
#### Call contract interface
After completing the transaction initialization, you can call the automatically generated contract interface:

E.g:
```
BCHAR *result_str;
result_str = StoreRead_saveList(&tx_ctx, (BUINT8*)"HelloWorld");
```
Note: Please refer to 4.6.2 for the description of the return value

### Manually construct contract calls
If the automatic generation tool cannot generate the C call interface, you need to manually construct the transaction message.

The manual construction of transactions needs to follow the ABI interface of the specific blockchain protocol.

Example 1: Ethereum transaction structure
Step 1: Call BoatEthTxInit() to initialize the transaction

Step 2.1: Set transaction parameter nonce:
BOAT_RESULT BoatEthTxSetNonce(BoatEthTx *tx_ptr,
BUINT64 nonce);
Nonce is usually set to BOAT_ETH_NONCE_AUTO, and the nonce value is obtained from the network.

Step 2.2: If necessary, set the "value" parameter of the transaction(in the initialized transaction object, value defaults to 0):
```
BOAT_RESULT BoatEthTxSetValue(BoatEthTx *tx_ptr,
BoatFieldMax32B *value_ptr);
```
Step 3a: For contract calls (transactions) that change the state of the blockchain, set the "data" parameter of the transaction:
```
BOAT_RESULT BoatEthTxSetData(BoatEthTx *tx_ptr,
BoatFieldVariable *data_ptr);
```
Among them, the format of data_ptr follows the Ethereum ABI, including the first 4 bytes of the Keccak-256 hash of the contract function prototype as the Function Selector, and then the parameters are arranged in sequence:
<https://solidity.readthedocs.io/en/develop/abi-spec.html>

Step 4a: Send transaction
```
BOAT_RESULT BoatEthTxSend(BoatEthTx *tx_ptr);
```

Step 4b: For contract calls that do not change the state of the blockchain, call the State-less contract function
```
BCHAR * BoatEthCallContractFunc(BoatEthTx *tx_ptr,
                                BCHAR *func_proto_str,
                                BUINT8 *func_param_ptr,
                                BUINT32 func_param_len);
```
Among them, the format of func_param_ptr follows the same rules as Step 3a.

Example 2: PlatONE transaction structure
Step 1: Call BoatPlatONETxInit() to initialize the transaction, and the transaction type field is set according to the actual transaction type.

Step 2.1: Set transaction parameter nonce:
BOAT_RESULT BoatPlatoneTxSetNonce(BoatEthTx *tx_ptr,
BUINT64 nonce); 
The nonce is usually set to BOAT_PLATONE_NONCE_AUTO, and the nonce value is obtained from the network.

Step 2.2: If necessary, set the "value" parameter of the transaction (in the initialized transaction object, value defaults to 0):
```
BOAT_RESULT BoatPlatoneTxSetValue(BoatEthTx *tx_ptr,
BoatFieldMax32B *value_ptr);
```
Step 3a: For contract calls (transactions) that change the state of the blockchain, set the "data" parameter of the transaction:
```
BOAT_RESULT BoatPlatoneTxSetData(BoatEthTx *tx_ptr,
BoatFieldVariable *data_ptr);
```
Among them, data_ptr is coded according to RLP and is sequentially compiled into the following fields:
```
{
  TransactionType (Fixed unsigned 64bit, BigEndian),
  FunctionName,
  FunctionArgument1,
  FunctionArgument2,
…
}
```
RLP encoding is as follows:
- a) Call RlpInitListObject() to initialize the top-level LIST object
- b) Call RlpInitStringObject() to initialize the first encoding field object
- c) Call RlpEncoderAppendObjectToList() to add the first encoding field object to the top-level LIST object
- d) Repeat b and c until all code objects are added to the top LIST object
- e) Call RlpEncode() to perform RLP encoding on the top-level LIST object and its subobjects
- f) Call RlpGetEncodedStream() to get the encoded stream
- g) After completing the contract call, call RlpRecursiveDeleteObject() to destroy the top-level LIST object and all its child objects

Step 4a: Send transaction
```
BOAT_RESULT BoatPlatoneTxSend(BoatEthTx *tx_ptr);
```

Step 3b: For contract calls that do not change the state of the blockchain, call the State-less contract function
```
BCHAR * BoatPlatoneCallContractFunc(BoatPlatoneTx *tx_ptr,
                                    BUINT8 *rlp_param_ptr,
                                    BUINT32 rlp_param_len)
```
Among them, the format of rlp_param_ptr follows the same rules as Step 3a.


For the specific calling method, please refer to the automatically generated code of the Demo attached to the SDK, which is located under \<SDKRoot\>/contract/generated.

## Suggestions for porting SDK to RTOS
If the SDK is ported to RTOS, the following points should generally be followed:
###### 1. Remove the dependency on curl

curl is a communication protocol library under linux, used in the SDK to support http/https communication. Blockchain nodes usually use the http/https protocol to communicate.

For modules using RTOS, you should add a call package to the module's http/https interface in \<SDKRoot\>/sdk/rpc, and modify \<SDKRoot\>/include/boatoptions.h, close RPC_USE_LIBCURL and set the new RPC USE OPTION.


###### 2. Remove the dependency on OpenSSL

OpenSSL is a cryptography library under linux, used in the SDK to support CSPRNG (cryptographic secure pseudo-random number generator) and AES encryption and decryption.

If the random number generator depends on hardware, a new hardware platform should be added to \<SDKRoot\>/hwdep. If you use the software pseudo-random number generator (it should be noted that the adverse impact on security), you should modify \<SDKRoot\>/include/boatoptions.h, turn off BOAT_USE_OPENSSL, and adjust the code in \<SDKRoot\>/hwdep/default/rng Make it reference the pseudo-random number generator provided by the system.

The AES encryption and decryption is used in the SDK for persistent storage of the wallet. If encryption is not required (it should be noted that the adverse impact on security), you can modify \<SDKRoot\>/hwdep/default/storage, cancel encryption, or use other encryption algorithms that do not rely on OpenSSL.


###### 3. Remove dependence on the file system

The SDK uses files as a persistent storage method for the wallet. If the RTOS does not support the file system, you should modify \<SDKRoot\>/hwdep/default/storage to modify the read/write file to a persistence method supported by the system.

When only one persistent wallet needs to be supported, the wallet name can be ignored in the persistence method.

###### 4. Memory trimming

If the memory of the target system is too tight to load, you can try to trim the memory. The points that can be cropped include:

a) According to actual needs, in \<SDKRoot\>/include/boatoptions.h, close the blockchain protocol that does not need to be supported
b) According to the actual situation, reduce the storage space BOAT_NODE_URL_MAX_LEN of the node URL string in \<SDKRoot\>/include/boatoptions.h
c) According to actual needs, reduce the number of wallets BOAT_MAX_WALLET_NUM in \<SDKRoot\>/include/boatwallet.h
d) According to the actual situation, reduce the maximum number of members supported in a LIST in \<SDKRoot\>/include/boatrlp.h MAX_RLP_LIST_DESC_NUM
e) According to the actual situation, reduce the size of the send/receive buffer WEB3_JSON_STRING_BUF_MAX_SIZE in \<SDKRoot\>/sdk/protocol/common/web3intf.h, and the size of the result string Buffer WEB3_RESULT_STRING_BUF_MAX_SIZE


###### If after the above cropping, the memory is still too large to fit, you can try:
- a) According to actual needs, the simple RLP encoding method for specific transaction parameters is used to replace the recursive general RLP encoding method in \<SDKRoot\>/sdk/rlp
- b) According to actual needs, tailor the APIs that are not actually used

## BoAT's extended AT command suggestion

### Create/load wallet AT^BCWALT
|**Command**|**Response(s)**|
| :-----| :-----| 
|Write Command:<br>^BCWALT=\<protocol_type\>,\<wallet_name\>[,\<wallet_config\>]|^BCWALT: \<wallet_index\><br>OK<br>|
|Test Command:<br>^BCWALT=?|+BCWALT: (list of supported \<protocol_type\>s)<br>OK<br>|

Features:
Create/load wallet, corresponding to BoatWalletCreate().

Parameters:
\<protocol_type\>: integer type
- 1: Ethereum
- 2: Hyper Ledger Fabric (not yet supported)
- 3: PlatON
- 4: PlatONE

\<wallet_name\>: string type; 
a string of wallet name

\<wallet_config\>: string type;
A JSON string representing the wallet configuration of \<protocol_type\>. The exact format is manufacture specific.

\<wallet_index\>: integer type; 
the index of the created wallet

### Unload wallet AT^BUWALT
|**Command**|**Response(s)**|
| :-----| :-----| 
|Write Command:<br>^BUWALT=<wallet_index>|<br>OK<br>|
|Test Command:<br>^BUWALT=?|+BUWALT: (list of loadeded \<wallet_index\>s)<br>OK<br>|

Features:
Unload wallet，corresponding to BoatWalletUnload ().

Parameters:
\<wallet_index\>: integer type; wallet index to unload, previously returned by ^BCWALT
### Delete wallet AT^BDWALT
|**Command**|**Response(s)**|
| :-----| :-----| 
|Write Command:<br>^BDWALT=\<wallet_name\>|<br>OK<br>|

Features:
Delete the created persistent wallet, corresponding to BoatWalletDelete()。

Parameters:
\<wallet_name\>: string type; the name of the wallet to delete

### Contract function call AT^BCALLFUNC
|**Command**|**Response(s)**|
| :-----| :-----| 
|Write Command:<br>^BCALLFUNC=\<wallet_index\>,\<tx_object\>|<br>OK<br>|

Features:
Initiating a contract call corresponds to the C call interface generated according to the contract ABI JSON file.

Parameters:
\<wallet_index\>: integer type; wallet index combined with the contract call, previously returned by ^BCWALT

\<tx_object\>: string type;
A JSON string representing the transaction object as per the generated C contract interface. The exact format is manufacture specific.

\<wallet_index\>: integer type; the index of the created wallet

### Transfer AT^BTRANS
|**Command**|**Response(s)**|
| :-----| :-----| 
|Write Command:<br>^BTRANS=\<wallet_index\>,\<recipient\>,\<value\>|<br>OK<br>|

Features:
Initiate a transfer of the designated Wallet (not all blockchains support transfers). For example, for Ethereum, it corresponds to BoatEthTransfer().

Parameters:
\<wallet_index\>: integer type; wallet index combined with the transfer (the payer), previously returned by ^BCWALT

\<recipient\>: string type;
A HEX string representing the recipient (payee) address.

\<value\>: integer type; the value to transfer, represented by the smallest token unit of the protocol (e.g. for Ethereum, the unit is wei or 10-18 ether).
## Reference
BoAT API Reference Manual

[1].BoAT IoT Framework SDK Reference Manual, AITOS.20.70.100IF, V1.0.0