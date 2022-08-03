# BoAT-X Framework for Fibocom-FM650 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into Fibocom-FM650 SDK, compile BoAT-X Framework static library and build the demo program.


## Copy files

Assuming `<FM650 Root>` to be the root directory of Fibocom-FM650 platform SDK:

1. Copy the entire BoAT-X-Framework directory into `<FM650 Root>/fibo-sdk/umdp/example`.

2. Create a new folder `boat_demo` under `<FM650 Root>/fibo-sdk/umdp/example`, copy BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/demo_platone_mycontract.c into `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`.

3. Copy BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/my_contract.cpp.abi.c into `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`.

4. Copy BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/my_contract.cpp.abi.h into `<FM650 Root>/fibo-sdk/umdp/example/boat_demo`.

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

### 1. Compile BoAT-X Framework static library (under Linux)

   #### a. Configure the target platform in directory BoAT-X-Framework/Makefile
   ```
   PLATFORM_TARGET ?= XinYi-XY1100
   ```

   #### b. Open a Linux shell, enter `<XY1100 Root>/userapp/BoAT-X-Framework` directory and compile BoAT static library
   ```
   cd <XY1100 Root>/userapp/BoAT-X-Framework
   make clean
   make all
   ```

   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<XY1100 Root>/userapp/BoAT-X-Framework/lib` directory.


### 2. Build the demo program of XY1100, generate .bin file for download

   Demo code for accessing blockchain through BoAT-X Framework is in `<XY1100 Root>/userapp/demo/boat_demo/boat_demo.c`

   Open a Linux shell and build the demo:
   ```
   cd <XY1100 Root>/targets/xinyiNBSoc_M3/Makefile
   make clean
   make all
   ```
   The download image ram.bin and flash.bin will be generated in `<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile/xinyiNBSoc_M3` if building is successful.

