# BoAT-X Framework for Mobiletek L503 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into L503 SDK, compile BoAT-X Framework static library and build the demo program under Windows environment.


## Copy files

Assuming `<L503 Root>` to be the root directory of L503 SDK:

1. Copy the entire BoAT-X-Framework directory into `<L503 Root>/app`.

2. Copy `BoAT-X-Framework/vendor/platform/mobiletek-L503C-6S/L503CRootDirCode/app/demo/platone_demo` into `<L503 Root>/app/demo`.

3. Copy `BoAT-X-Framework/vendor/platform/mobiletek-L503C-6S/main.c` into `<L503 Root>/app/demo`.

4. Copy `BoAT-X-Framework/vendor/platform/mobiletek-L503C-6S/Makefile` into `<L503 Root>/app/demo`.


After these files copied, the directory structure should look like:
```
<L503 Root>
|
|-- bootloader
|-- kernel
`-- app
    |-- BoAT-X-Framework
    |-- build
    |-- config
    |-- release
    |-- system
    |-- tools	
	|-- build_app.bat	
    |-- Makefile	
    |-- demo
    |   |-- audio_demo
    |   |-- cam_demo
    |   |-- device_demo
    |   |-- fota_demo
    |   |-- fs_demo
    |   |-- ftp_demo
    |   |-- gps_demo
    |   |-- hello_test
	|   |-- http_demo
    |   |-- lcd_demo
    |   |-- mqtt_demo
    |   |-- nw_demo
    |   |-- os_demo
    |   |-- sms_demo
    |   |-- uart_demo
    |   |-- visual_at_demo
    |   |-- wifi_demo
    |   |-- main.c
    |   |-- Makefile
    |   |-- platone_demo
    |   |   |-- platone_demo.c
    |   |   |-- my_contract.cpp.abi.c
    |   |   |-- my_contract.cpp.abi.h
    |   |   |-- Makefile
    |   |   |-- include
    |   |   |   |-- platone_demo.h
```


## File Modification

### 1. Add BoAT-X-Framework static libraries .a files onto L503 platform

  Open `<L503 Root>/app/build/makerules.mk`.
  
  Find `DEPENDENCY_INCLUDE_LIB := ${foreach lib, ${LOCAL_EXPORT_LIB}, ${SOFT_WORKDIR}/${lib}}`, add the following content in the last new line:
  ```
  DEPENDENCY_INCLUDE_LIB += $(SOFT_WORKDIR)/BoAT-X-Framework/lib/libboatwallet.a
  DEPENDENCY_INCLUDE_LIB += $(SOFT_WORKDIR)/BoAT-X-Framework/lib/libboatvendor.a
  ```

### 2. Add the BoAT-X-Framework header files

  Open `<L503 Root>/app/build/buildvar.mk`.
  
  Find `cust_lc/inc`, add ` \` after `cust_lc/inc`, add the following content in the last new line:
  ```
  BoAT-X-Framework/include/protocolapi \
  BoAT-X-Framework/include \
  BoAT-X-Framework/vendor/platform/include \
  BoAT-X-Framework/sdk/network/platone
  ```

### 3. Add a custom mapping interface for log printing

  Open `<L503 Root>/kernel/mbtk/cust_api/customer_api.c`.
  
  Find struct `mbtk_user_api`, add a function mapping pointer to this struct：
  ```
  int (*user_uart_printf)(const char *fmt, ...);
  ```

  Find the definition mapping table struct `g_user_api`, add initializations that require mapping functions to this struct:
  ```
  .user_uart_printf = RTI_LOG,
  ```  
  
  Open `<L503 Root>/app/system/inc/mbtk_customer_api.h`.
  
  Find struct `mbtk_user_api`, add a function mapping pointer to this struct：
  ```
  int (*user_uart_printf)(const char *fmt, ...);
  ``` 
  
  Open `<L503 Root>/app/system/src/api_customer_init.c`.
  
  Find `int (*ol_path_ctrl)(UINT32 path);`, add the following content in the last new line:
  ```
  int (*user_app_printf)(const char *fmt, ...);
  ```
  
  Find function `mbtk_api_init_customer`, add the following content in the brace below `if(api_table)` in function:
  ```
  user_app_printf = api_table->user_uart_printf;
  ```
  

## Compile BoAT-X-Framework Static library

### 1. Compile BoAT-X Framework static library (under Cygwin)

   #### a. Configure the target platform in directory BoAT-X-Framework\Makefile
   ```
   PLATFORM_TARGET ?= mobiletek-L503C-6S
   ```
   
   #### b. Open a Cygwin shell, enter <ML302 Root>/BoAT-X-Framework directory and compile BoAT static library
   ```
   cd <L503 Root>/app/BoAT-X-Framework
   ../tools/gnumake.exe clean
   ../tools/gnumake.exe all
   ```
   
   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<L503 Root>\BoAT-X-Framework\lib` directory.
   

### 2. Build demo program, generate .zip file for download

   Demo code for accessing blockchain through BoAT-X Framework is in `<L503 Root>/app/demo/platone_demo/platone_demo.c`。
   
   Open a Windows cmd window and build the app:
   ```
   cd <L503 Root>/app
   build_app.bat
   ```
   
   An `user_app.bin` file will be generated under `<L503 Root>/app/release` once compiled successfully.

   Copy `user_app.bin` into `<L503 Root>/kernel/mbtk/releasepack/L503C_6S/images`.

   Open a Windows cmd window and build the kernel:
   ```
   cd <L503 Root>/kernel
   make_clean.bat
   L503_6S_build_open.bat
   ```
   
   The download file `ASR_CRANEL_EVB_CRANEL_A0_08MB.zip` will be generated under `<L503 Root>/kernel/OUTPUT_L503C_6S` once compiled successfully.