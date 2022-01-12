# BoAT-X Framework for XinYi-XY1100 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into XinYi-1100 SDK, compile BoAT-X Framework static library and build the demo program.


## Copy files

Assuming `<XY1100 Root>` to be the root directory of XinYi-XY1100 platform SDK:

1. Copy the entire BoAT-X-Framework directory into `<XY1100 Root>/userapp`.

2. Create a new folder `boat_demo` under `<XY1100 Root>/userapp/demo`, copy BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/demo/boat_demo.c into `<XY1100 Root>/userapp/demo/boat_dem/`.

3. Copy BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/demo/my_contract.c into `<XY1100 Root>/userapp/demo/boat_demo`.

4. Copy BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/demo/my_contract.h into `<XY1100 Root>/userapp/demo/boat_demo`.

5. Copy BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/vendor/Makefile into /userapp/BoAT-X-Framework/vendor and overwrite the original file.

6. Copy and overwrite BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/storage/persiststore.c into `<XY1100 Root>/userapp/BoAT-X-Framework/vendor/common/storage`.


After copying these files, the directory structure should look like:

```
<XY1100 Root>
|
|-- LICENSE
|-- Makefile
|-- README.md
|-- TCPIP
|-- arch
|-- examples
|-- kernel
|-- output
|-- sys_app
|-- targets
`-- userapp
    |-- BoAT-X-Framework
    |-- Inc
    |-- Src
    |-- module.mk    
    |-- demo
    |   |-- boat_demo
    |   |   |-- boat_demo.c
    |   |   |-- my_contract.c
    |   |   |-- my_contract.h
```


## Files Modification

### 1. Specify demo source files as inputs of Compilation in module.mk

Open `<XY1100 Root>/userapp/module.mk` 
Add the following two lines as below:
```
C_FILES_FLASH+=$(TOP_DIR)/userapp/demo/boat_demo/boat_demo.c
C_FILES_FLASH+=$(TOP_DIR)/userapp/demo/boat_demo/my_contract.c
```


### 2. Add the header file path that needs to be quoted in module.mk

Open `<XY1100 Root>/userapp/module.mk` 
Add the following content in the header file path addition:
```
CFLAGS+= -I$(TOP_DIR)/TCPIP/net_tool/Dtls/mbedtls-2.6.0/include/mbedtls \
-I$(TOP_DIR)/userapp/BoAT-X-Framework/include \
-I$(TOP_DIR)/userapp/BoAT-X-Framework/vendor/platform/include \
-I$(TOP_DIR)/userapp/BoAT-X-Framework/sdk/include \
-I$(TOP_DIR)/userapp/BoAT-X-Framework/vendor/platform/XinYi-XY1100/src/log \
-I$(TOP_DIR)/userapp/BoAT-X-Framework/sdk/protocol/common/web3intf \
```


### 3. Add the static library file path to XinYi-XY1100 platform
Open `<XY1100 Root>/userapp/module.mk` 
Add the following at the end of the file:
```
LDFLAGS+=-L$(TOP_DIR)/userapp/BoAT-X-Framework/lib/ -lboatwallet    
LDFLAGS+=-L$(TOP_DIR)/userapp/BoAT-X-Framework/lib/ -lboatvendor
```

### 4. Configure the cross-compilation environment of BoAT-X-Framework
Open `<XY1100 Root>/userapp/BoAT-X-Framework/vendor/platform/XinYi-XY1100/external.env` 
Configure the actual path of the local cross compiler after `CC` and `AR`

### 5. Configure the cross-compilation environment of XinYi-XY1100 platform
Open `<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile/makefile`
Configure the path of the local cross compiler into the environment variable at the beginning of the file, such as:
```
PATH := $(PATH):/home/tools/xinyigcc/gcc-arm-none-eabi-9-2019-q4-major/bin
```

### 6. Add a funtion option to support BoAT-X-Framework in XinYi-XY1100 platform
Open `<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile/feature.mk`
Add the following at the end of the file:
```
BOATSDK_SUPPORT=y
```

### 7. Increase the compilation parameters of BoAT-X-Framework in XinYi-XY1100 platform
Open `<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile/makefile`
Add the following in the position of the additional functions of the configuration module:
```
ifeq ($(BOATSDK_SUPPORT),y)
    FLAGS+=-DBOATSDK_SUPPORT=1
endif
```

### 8. Configure the macro options of mbedtls in the XinYi-XY1100 platform
Open `<XY1100 Root>/TCPIP/net_tool/Dtls/mbedtls_port/los_mbedtls_config.h`
Add the following:
```
  #ifdef BOATSDK_SUPPORT

  #define MBEDTLS_ECDSA_C
  #define MBEDTLS_ECP_C
  #define MBEDTLS_ASN1_WRITE_C
  #define MBEDTLS_ECDH_C
  #define MBEDTLS_PK_C
  #define MBEDTLS_ECP_DP_SECP256K1_ENABLED

  #define MBEDTLS_PLATFORM_STD_CALLOC        calloc
  #define MBEDTLS_PLATFORM_STD_FREE            free


  #define MBEDTLS_PEM_WRITE_C
  #define MBEDTLS_PK_WRITE_C
  #define MBEDTLS_BASE64_C
  #define MBEDTLS_PK_PARSE_C
  #define MBEDTLS_ASN1_PARSE_C

  #define MBEDTLS_BIGNUM_C
  #define MBEDTLS_OID_C
  #define MBEDTLS_PEM_PARSE_C

  #endif
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

