# BoAT-X Framework for Fibocom-FM650 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into Fibocom-FM650 SDK, compile BoAT-X Framework static library and build the demo program.


## Copy files

Assuming `<FM650 Root>` to be the root directory of Fibocom-FM650 platform SDK:

1. Copy the entire BoAT-X-Framework directory into `<FM650 Root>/fibo-sdk/umdp/example`.

2. Create a new folder `boat_demo` under `<FM650 Root>/fibo-sdk/umdp/example`, copy BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/platone_demo/demo_platone_mycontract.c into `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`.

3. Copy BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/platone_demo/my_contract.cpp.abi.c into `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`.

4. Copy BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/platone_demo/my_contract.cpp.abi.h into `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`.

5. Copy BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/Makefile into `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`.

6. Copy BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/libcurl.so.4.5.0 into `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`.

7. Copy and overwrite BoAT-X-Framework/vendor/platform/Fibocom-FM650/external.env into `<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default`.


After copying these files, the directory structure should look like:

```
<FM650 Root>
|
|-- build.sh
|-- config.ini
|-- fibo-kernel
|-- fibo-rootfs
|-- fibo_sign_package
|-- curl
|-- fibo-compiletool
|-- fibo-ota
|-- fibo-uboot
`-- fibo-sdk
    |-- Makefile
    |-- umdp
    |   |-- bin
    |   |-- config
    |   |-- include
    |   |-- lib
    |   |-- example
    |   |   |-- Makefile
    |   |   |-- BoAT-X-Framework
    |   |   |-- boat_demo
    |   |   |   |-- Makefile
    |   |   |   |-- demo_platone_mycontract.c
    |   |   |   |-- my_contract.cpp.abi.c
    |   |   |   |-- my_contract.cpp.abi.h
    |   |   |   |-- libcurl.so.4.5.0
```


## Files Modification

### 1. Add the path to the BoAT header file that needs to be referenced in the Makefile

Open `<FM650 Root>/fibo-sdk/Makefile` 
Add the following content in the header file path addition:
```
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/include
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/vendor/platform/include
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/src/log
```


### 2. Configure curl header file path

Open `<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/external.env` 
Configure the actual path of the FM650 platform curl header file after `EXTERNAL_INC`, for example:
```
EXTERNAL_INC := -I/home/linux/FM650/fg650-opensdk
```

## Compile BoAT-X Framework Static library

### 1. Configure the cross compiler

   #### a. Open a terminal on Linux and go to the fibo-compiletool directory
   ```
   cd <FM650 Root>/fibo-compiletool
   ```

   #### b. Make the cross-compilation instructions work on this terminal
   ```
   source environment-setup-aarch64-unisoc-linux
   ```

   After it takes effect successfully, execute `echo $CC` and `echo $AR` in the terminal to see the corresponding command content, and the subsequent compilation of the BoAT static library must also be completed in the terminal.


### 2. Compile the BoAT static library

  #### a. Configure the compilation information in the main Makefile, open `<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/Makefile`, and modify the following (Take PlatONE as an example):
  ```
   BOAT_PROTOCOL_USE_PLATONE    ?= 1

   PLATFORM_TARGET ?= linux-default
  ```

  #### b. Compile the BoAT static library
  ```
   cd <FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework
   make clean
   make all
  ```
  After successful compilation, the static libraries libboatvendor.a and libboatwallet.a will be generated under `<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/lib`.

## Compile and run the demo calling BoAT

### 1. Configure blockchain information (Take PlatONE as an example)

  #### a. Configure the deployed or known available node address to the `demoUrl` variable in `<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/demo_platone_mycontract.c` ,for example:
  ```
  const BCHAR *demoUrl = "http://121.0.0.1:7545";

  ```

  #### b. Configure the smart contract address deployed on the chain to `demoRecipientAddress` in `<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/demo_platone_mycontract.c` variable,for example:
  ```
  const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";
  ```

### 2. Referring to smart contracts

  #### a. Compile the smart contract ABI file into a c interface file according to the BoAT user manual

  #### b. Refer to the smart contract c interface file in `<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/demo_platone_mycontract.c` and call the c interface of the contract, (with smart contract my_contract for example) for example:
  ```
  #include "my_contract.cpp.abi.h"
  ```
  ```
  result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
  ```

### 3. compile demo

  #### a. Run `<FM650 Root>/build.sh` and configure the project
  ```
  ./build.sh project FG650_CN_OPENCPU_OPEN
  ```

  #### b. Compile fibo-sdk
  ```
  ./build.sh sdk
  ```
  After successful execution, the `Object` folder will be generated under `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`, which contains the final executable file `call_boat_test`

### 4. run demo

  #### a. Power on the FM650 board and connect the PC via USB cable

  #### b. Follow the board's documentation to turn on the ADB function

  #### c. Open a terminal on the pc, and push the executable file `call_boat_test` to the board through ADB (take /home/user as an example)
  ```
  adb push call_boat_test /home/user
  ```

  #### d. Insert an available SIM card into the SIM card slot of the FM650

  #### e. Open a terminal on the PC and run the demo in the FM650
  ```
  adb shell
  cd /home/user
  chmod +x call_boat_test
  ./call_boat_test
  ```