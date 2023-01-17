# BoAT-X-Framework for Fibocom-MA510 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into Fibocom-MA510 SDK, compile BoAT-X Framework static library and build the demo program.

## Copy Files

Assuming `<MA510 Root>` to be the root directory of Fibocom-MA510 platform SDK:

1、Copy the entire BoAT-X-Framework directory into `<MA510 Root>/fibocom/example`.

2、Create a new folder `boat_demo` under `<MA510 Root>/fibocom/example`,copy all the contents under BoAT-X-Framework/vendor/platform/Fibocom-MA510/MA510RootDirCode/platone_demo to `<MA510 Root>/fibocom/example/boat_demo`.

After copying these files, the directory structure should look like:
```
<MA510 Root>
|
|-- bin
|-- common
|-- llvmtools
|-- build_all.sh
|-- example.txt
`-- fibocom
    |-- build   
    |-- example
    |   |-- boat_demo
    |   |-- BoAT-X-Framework
```


## Files Modification

### 1、Add a demo supporting compilation in example.txt

  Open `<MA510 Root>/example.txt`
  
  Add the following at the end of the file:
  ```
boat_demo      -__EXAMPLE_BOATDEMO__ -NO -NO
  ```

### 2、In build_all.sh, add the BoAT related header file path and the generated static library path that need to be referenced

Open `<MA510 Root>/build_all.sh`

Update the reference files path to add the following new contents:
```
BOAT_INC_PATH="fibocom/example/BoAT-X-Framework/include"
BOAT_PROTOCOL_INC_PATH="fibocom/example/BoAT-X-Framework/include/protocolapi"
BOAT_LOG_INC_PATH="fibocom/example/BoAT-X-Framework/vendor/platform/Fibocom-MA510/src/log"
BOAT_LIB_PATH="fibocom/example/BoAT-X-Framework/lib"
```

### 3、Add the static library file path to Fibocom-MA510 platform

Open `<MA510 Root>/build_all.sh`
  
Add the static library of BoAT at the location of the link lib library, for example:
  ```
$TOOLCHAIN_PATH/clang++ -d -o $DEMO_ELF_OUTPUT_PATH/$DAM_ELF_NAME -target armv7m-none-musleabi -fuse-ld=qcld -lc++ -Wl,-mno-unaligned-access -fuse-baremetal-sysroot -fno-use-baremetal-crt -Wl,-entry=$DAM_RO_BASE $DEMO_APP_OUTPUT_PATH/txm_module_preamble_llvm.o -Wl,-T$DAM_SRC_PATH/../app_dam_demo.ld -Wl,-Map,$DEMO_ELF_OUTPUT_PATH/$DAM_MAP_NAME,-gc-sections -Wl,-gc-sections $DEMO_APP_OUTPUT_PATH/*.o  -L $BOAT_LIB_PATH -lboatwallet -L $BOAT_LIB_PATH -lboatvendor $DAM_LIB_PATH/*.lib
  ```
Note the order in which the libraries are linked, since the BoAT static library calls the MA510 interface, the MA510 library should be linked first, followed by the boat-related two static libraries.

### 4、Configure the cross-compilation environment of BoAT-X-Framework

  Open `<MA510 Root>/fibocom/example/BoAT-X-Framework/vendor/platform/Fibocom-MA510/external.env`
  
  Configure the actual path of the local cross compiler after `CC` and `AR`, for example:
  ```
CC := $(BOAT_BASE_DIR)/../../../llvmtools/4.0.3/bin/clang
AR := $(BOAT_BASE_DIR)/../../../llvmtools/4.0.3/tools/bin/arm-ar
  ```
  

### 5、Add the path of the header files needed to compile BoAT-X-Framework

Open `<MA510 Root>/fibocom/example/BoAT-X-Framework/vendor/platform/Fibocom-MA510/external.env`

Add the following content:
```
EXTERNAL_INC := -I$(BOAT_BASE_DIR)/../../../common/include/qapi \
                -I$(BOAT_BASE_DIR)/../../../common/include/log/inc \
                -I$(BOAT_BASE_DIR)/../../../common/include/threadx_api \
                -I$(BOAT_BASE_DIR)/../../../llvmtools/4.0.3/armv7m-none-eabi/libc \
                -I$(BOAT_BASE_DIR)/../../../llvmtools/4.0.3/armv7m-none-eabi/libc/include \
```

### 6、Increase the compilation parameters of BoAT-X-Framework in Fibocom-MA510 platform

Open `<MA510 Root>/fibocom/example/BoAT-X-Framework/vendor/platform/Fibocom-MA510/external.env`

Add the following content:
```
EXTERNAL_CFLAGS := -marm -target armv7m-none-musleabi -mfloat-abi=softfp -mfpu=none -mcpu=cortex-a7 -mno-unaligned-access  -fms-extensions -Osize -fshort-enums -Wbuiltin-macro-redefined \
                -DQAPI_TXM_MODULE -DTXM_MODULE -DTX_DAM_QC_CUSTOMIZATIONS -DTX_ENABLE_PROFILING -DTX_ENABLE_EVENT_TRACE -DTX_DISABLE_NOTIFY_CALLBACKS  -DFX_FILEX_PRESENT -DTX_ENABLE_IRQ_NESTING  -DTX3_CHANGES
```


## Compile BoAT-X Framework Static Library

### 1、Compile BoAT-X Framework static library (under Linux)
   
   #### a、Configure the target platform in directory BoAT-X-Framework/Makefile

   Open `<MA510 Root>/fibocom/example/BoAT-X-Framework/Makefile`,set PLATFORM_TARGET:
   ```
   PLATFORM_TARGET ?= Fibocom-MA510
   ```
   
   #### b、Open a Linux shell, enter `<MA510 Root>/userapp/BoAT-X-Framework` directory and compile BoAT static library

   ```
   cd <MA510 Root>/fibocom/example/BoAT-X-Framework
   make clean
   make all
   ```
   
   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<MA510 Root>/userapp/BoAT-X-Framework/lib` directory.
   

### 2、Compile MA510 demo program and generate mirror download files

   Demo code for accessing blockchain through BoAT-X Framework is in`<MA510 Root>/fibocom/example/boat_demo/src/boat_demo.c`
   
   Open a Linux shell and build the demo: 
   ```
   cd <MA510 Root>
   ./build_all.sh llvm -c
   ./build_all.sh llvm boat_demo
   ```
   The mirror download files oem_app_path.ini、boat_demo_dam_demo.map、boat_demo_dam_demo.elf and boat_demo_dam_demo.bin will be generated in `<MA510 Root>/bin` if building is successful.
