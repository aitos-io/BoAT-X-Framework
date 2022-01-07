
# BoAT Framework
[![Issue](https://img.shields.io/github/issues/aitos-io/BoAT-X-Framework)](https://github.com/aitos-io/BoAT-X-Framework/issues)![Forks](https://img.shields.io/github/forks/aitos-io/BoAT-X-Framework)![Stars](https://img.shields.io/github/stars/aitos-io/BoAT-X-Framework)[![GitHub Release](https://img.shields.io/github/license/aitos-io/BoAT-X-Framework)](https://github.com/aitos-io/BoAT-X-Framework/blob/master/LICENSE)[![Join the chat at https://gitter.im/BoAT-X/community](https://badges.gitter.im/BoAT-X/community.svg)](https://gitter.im/BoAT-X/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

![BoAT logo](https://aitos-io.github.io/BoAT-X-Framework/logo/BoAT_RGB_Horizontal_100.png)


## Introduction
Welcome to the official implementation of BoAT Framework! 

**BoAT** blockchain application framework is an IoT-device-oriented lightweight blockchain client SDK written in C language. **BoAT** stands for *Blockchain of AI Things* and  depicts the vision that a boat transfers trustworthy information from the data source to the data lake.


### The Pain Point

Most blockchain or BaaS (Blockchain as a Service) variants bring node or client/wallet software. However, these software are designed for personal computers, cloud servers or smartphones and are usually written in high-level languages like Go, Java, JavaScript, Python, etc. Some require a cumbersome virtual machine or interpreter to execute, and some even have to download code dynamically at runtime. Meanwhile, IoT devices are not as powerful and typically run RTOS or lightweight Linux. Due to constrained resources, most IoT devices could only support native C language applications and thus can hardly access blockchain directly.


### The Solution

There are two methods to resolve the problem. First, the IoT devices send data to a traditional centralized cloud server, the data hub, and then relay the data to the blockchain. This method resolves the problem with some trade-off of higher single-point failure probability at the data hub.

The other method is, the IoT devices directly invoke on-chain smart contracts via a blockchain client that meets the device environment and the blockchain. This method allows every IoT device to access blockchain in an independent and distributed way with the trade-off that a C-language blockchain client SDK has to be developed and ported to various device hardware and software environments.

The latter is the way **BoAT** is walking on. To ease off the effort that IoT device manufacturers have to take, *aitos.io* initiates and contributes to **BoAT**, the C-language blockchain application framework, enabling IoT devices to access the blockchain easily. 


### Supported Blockchains and IoT Modules

See [Supported List](./SUPPORTED_LIST.md) for supported blockchains and IoT modules.


### Release
For full list of new features, please read [aitos.io Release Notes](https://github.com/aitos-io/BoAT-X-Framework/releases).


### Project Status Report
For project status update, please visit [BoAT Project Status Update Reports](https://github.com/aitos-io/project-status-update).

### Whitepaper
Please see [BoAT Blockchain IoT Module Product White Paper](./docs/en-us/BoAT_Blockchain_IoT_Module_Product_White_Paper_EN.pdf).

### Blockchain Module Technology and Application
Please see [BoAT Blockchain IoT Module Technology and Application](./docs/en-us/BoAT_Blockchain_IoT_Module_Technology_and_Application_en.pdf).

## Quick Start

### Dependencies 

BoAT is tested with the following dependencies:  
(The dependent software with a lower version will most likely work well but are not tested).

#### General
In general, following dependencies are required:

Dependencies    | Requirements
----------------|:------------:
Host OS         |Depending on the development tool chain of the underlying device
Make            |GNU Make (4.3 is tested)
Python          |3.8.3 (Python 2.7 is also compatible)


#### For Cygwin64 or linux_x86_64
To build BoAT for demo purpose on a personal computer, following additional dependencies are required:

Dependencies    | Requirements
----------------|:------------:
gcc             |4.9.2 is tested
libcurl         |7.55.1 is tested
openssl         |1.1.1d is tested


#### Embedded devices
For embedded devices, the exact configuration depends on the development toolchain of the underlying device.  
Linux-based embedded devices are similar to that on x86_64, except for the exact compiler and dependent library version may be device-specific. Meanwhile, RTOS-based embedded devices are much diverse.

### Before building the demo
As BoAT is a client for blockchain, some environments must be prepared before building the demo.

#### Make use of a deployed blockchain or deploy your own blockchain
For public blockchains, you must prepare some gas for transaction fees in advance. If a simulator is available for that blockchain, it's a better choice.

For consortium blockchains, you must consult the administrator to give you access permission on the blockchains. Or you may deploy your own consortium blockchains node for test purposes.

#### Deploy the smart contract
The smart contracts used in the demo locate in ./contract.  
Follow the blockchain's official website for details on how to compile and deploy the contract.

#### Modify the demo code
Modify the demo code in ./demo:
+ Replace the private key and account address (if necessary) with an appropriate one
+ Replace the smart contract address with the one you got when deploying the contract
+ Replace the URL of the blockchain node with the real one



### Build From Source

#### Code Directories
```
<SDKRoot>
|
+---build           | Directory to store object and executable files
+---demo            | Demo application
+---docs            | API reference manual
+---vendor          | Special vendor dependency
|   \---common      |     Universal soft algorithms implementation
|   \---platform    |     Dependency of different platforms
+---include         | Header files for application to include
+---lib             | Lib files for application to link with
+---sdk             | SDK source
|   +---third-party |     Third party libraries
|   +---include     |     Header files for SDK internal use
|   +---protocol    |     Blockchain client protocol implementation
|   +---rlp         |     RLP encoder
|   +---utilities   |     Utility APIs
|   \---wallet      |     SDK entry API implementation
+---tests           | Test cases
\---tools           | Tools for generating C interface from contract ABI

NOTE: ./build and ./lib are created in building

```

#### To build BoAT demo application
```
$make demo
```
The built demo application locates at:
./build/demo/boatdemo


#### To build BoAT libraries only
```
$make
```
or
```
$make boatlibs
```
The built libraries locate at:
./lib/libboatwallet.a
./lib/libboatvendor.a


#### To clean everything
```
$make clean
```



### Run BoAT demo on x86_64
If BoAT is built for Cygwin or linux-x86_64, run the demo application:
real node of an Ethereum compatible blockchain network must be available.
```
$chmod a+x ./build/demo/demo_<protocol>/<demo_name>
$./build/demo/demo_<protocol>/<demo_name>
```
\<protocol\> can be `ethereum` `fiscobcos` `platone` `fabric` `platon` `hw_bcs` `chainmaker`.  
Make sure the network connection to the blockchain node (or blockchain simulator) is available.


## Using BoAT in Your Code

### Contract C Interface Generation

The smart contract is the code running on the blockchain virtual machine. Smart contracts run like remote API calls. Though the programming language of smart contracts is not C, it has defined ABI (Application Binary Interface). Therefore, remote calls to the contract must follow the ABI.

However, manually applying the rule of ABI is quite complex for embedded C programmers. BoAT IoT Framework SDK provides some tools to generate C interface codes from the ABI. The generated C API can be called from other parts within the C project. Though not all contract ABI can be converted to a C interface due to a lack of object-oriented programming capability, the tools could ease many obligations.

The generation tools are written in Python and lie in ./tools.

Copy the ABI json file generated by truffle or C-tool during contract compilation, to the corresponding directory in ./contract. The generation tool will be called against the ABI file during the making. You can include generated head files (./contract/generated) in your C code to call the APIs.


### How to Call a Contract in Your C code

1. Call BoatWalletCreate() with appropriate configuration to create a wallet. The private key in the configuration must follow the actual key in your control. You can also generate a private key by calling BoatXXWalletGeneratePrivkey() if you know what you’re doing.

2. Call BoatXXTxInit() with the wallet reference and other parameters to initialize a transaction object (even if it’s a state-less call), where XX is the blockchain protocol name.

3. Call generated C interface API with the initialized transaction object and other arguments.

4. Check the return value of the C interface API. If it’s not NULL, parse the string content as per contract prototype.

See demo code for reference.

To manually organize a contract call, refer to the generated C API codes. Note that the interface definition is different for different blockchain protocols.


### Configure Your Makefile and C code

To use BoAT in your own code, please following these steps.

#### Build BoAT libraries
Place BoAT source somewhere in your project and build BoAT libraries.

#### Generate C interface code
Generate C interface code from contract ABI and add the generated C files into project for building.
See [*Contract C Interface Generation*](#contract-c-interface-generation) for how to generate them.

#### Modify Makefile of your project
Add include file search path:
+   \<SDKRoot\>/include
+   Path to generated C interface header file
    
Add to link options all library files in <SDKRoot>/lib in sequence:
+   libboatwallet.a
+   libboatvendor.a

Add to link options: -lcurl


#### Modify your C code
Add include header files:
```
#include "boatconfig.h"
#include "boatiotsdk.h"
```

Follow instructions in [*How to Call a Contract in Your C code*](#how-to-call-a-contract-in-your-c-code).



## Documentation

For detailed information, please visit [BoAT documentation](https://aitos-io.github.io/BoAT-X-Framework)


## FAQ
TBD

## Community
The BoAT-X Framework community can be found at:
+ Contact Mail: info@aitos.io
+ Report Bugs: [BoAT-X Issues](https://github.com/aitos-io/BoAT-X-Framework/issues)
+ Medium: https://aitos-io.medium.com/
+ LinkedIn: https://www.linkedin.com/company/aitos-io
+ WeChat Group: Add WeChat "miatang51213" to join the group

To chat with other community members you can join the [BoAT-X Gitter](https://gitter.im/BoAT-X/community)



## Contribution
We are glad to have contributors out of the core team; contributions, including (but not limited to) style/bug fixes,
implementation of features, proposals of schemes/algorithms, and thorough documentation, are welcomed. Please refer to our [Contribution Guideline](./CONTRIBUTING.md) for more information.

Find development documentation at [BoAT documentation](https://aitos-io.github.io/BoAT-X-Framework).

Submit Pull Requests at [Pull Requests](https://github.com/aitos-io/BoAT-X-Framework/pulls)


## License

Apache License 2.0, see [LICENSE](./LICENSE).


## Join us as a BoAT-Mariner and pioneer your IoT+Blockchain path

**Open source by [aitos.io](http://www.aitos.io/)**

[![aitos.io](https://aitos-io.github.io/BoAT-X-Framework/logo/aitos_logo_100.png)](http://www.aitos.io/)
