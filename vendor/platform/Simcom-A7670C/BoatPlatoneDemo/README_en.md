# BoAT-X Framework for Simcom A7670C Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into A7670C SDK, compile BoAT-X Framework static library and build the demo program under Windows environment.


## Copy files

Assuming `<A7670C Root>` to be the root directory of A7670C SDK:

1. Copy the entire BoAT-X-Framework directory into `<A7670C Root>/app`.

2. Copy files under `BoAT-X-Framework/vendor/platform/Simcom-A7670C/BoatPlatoneDemo/A7670CRootDirCode/sc_demo/src` into `<A7670C Root>/sc_demo/src`.

3. Copy files under `BoAT-X-Framework/vendor/platform/Simcom-A7670C/BoatPlatoneDemo/A7670CRootDirCode/sc_demo/inc` into `<A7670C Root>/sc_demo/inc`.

4. Copy and replace `BoAT-X-Framework/vendor/platform/Simcom-A7670C/BoatPlatoneDemo/A7670CRootDirCode/sc_application.c` into `<A7670C Root>`.


After these files copied, the directory structure should look like:
```
<A7670C Root>
|-- BoAT-X-Framework
|-- config
|-- examples
|-- kernel
|-- sc_lib	
|-- script	
|-- tools
|-- app_build_doc.md
|-- CMakeLists.txt
|-- makeDepend.mak
|-- Makefile
|-- sc_application.c	
`-- sc_demo
    |-- inc
    |   |-- demo_lcd.h
    |   |-- boat_platone_demo.h
    |   |-- my_contract.cpp.abi.h
    |-- src
    |   |-- token
    |   |-- utils
    |   |-- cus_uart.c
    |   |-- cus_urc.c
    |   |-- cus_usb_vcom.c
    |   |-- demo_app_download.c
    |   |-- demo_app_updater.c
    |   |-- demo_audio.c
    |   |-- demo_auto_mqtt.c
    |   |-- demo_ble.c
    |   |-- demo_call.c
    |   |-- demo_cam.c
    |   |-- demo_file_system.c
    |   |-- demo_flash.c
    |   |-- demo_fota.c
    |   |-- demo_ftps.c
    |   |-- demo_ftps_test.c
    |   |-- demo_gpio.c
    |   |-- demo_gps.c
    |   |-- demo_helloworld.c
    |   |-- demo_htp.c
    |   |-- demo_https.c
    |   |-- demo_i2c.c
    |   |-- demo_lcd.c
    |   |-- demo_mqtt.c
    |   |-- demo_network.c
    |   |-- demo_ntp.c
    |   |-- boat_platone_demo.c
    |   |-- demo_pm.c
    |   |-- demo_poc.c
    |   |-- demo_pwm.c
    |   |-- demo_simcard.c
    |   |-- demo_sms.c
    |   |-- demo_spi.c
    |   |-- demo_ssl.c
    |   |-- demo_ssl_test.c
    |   |-- demo_tcpip.c
    |   |-- demo_uart.c
    |   |-- demo_wifi.c
    |   |-- mqtt_OneNET.c
    |   |-- mqtt_tencent.c
    |   |-- my_contract.cpp.abi.c
    |   |-- simcom_demo.c
    |-- CMakeLists.txt
```


## File Modification

### 1. Add BoAT-X-Framework static libraries .a files onto A7670C platform

  Open `<A7670C Root>/CMakeLists.txt`.
  
  Find
  ```
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
    "${CMAKE_SOURCE_DIR}/sc_lib/${SCMODULE}/lib/libsc_lib.a"
  )
  endif()
  endif()
  ```
  add the following content in the last new line:
  ```
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
      "${CMAKE_SOURCE_DIR}/BoAT-X-Framework/lib/libboatwallet.a"
  )
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
      "${CMAKE_SOURCE_DIR}/BoAT-X-Framework/lib/libboatvendor.a"
  )
  ```

### 2. Add the BoAT-X-Framework header files

  Open `<A7670C Root>/sc_demo/CMakeLists.txt`.
  
  Find `${CMAKE_SOURCE_DIR}/sc_demo/inc`, add the following content in the last new line:
  ```
  ${CMAKE_SOURCE_DIR}/BoAT-X-Framework/include
  ${CMAKE_SOURCE_DIR}/BoAT-X-Framework/include/protocolapi
  ${CMAKE_SOURCE_DIR}/BoAT-X-Framework/vendor/platform/include
  ${CMAKE_SOURCE_DIR}/BoAT-X-Framework/sdk/network/platone
  ```

  Then, find `list(APPEND sc_demo_src ./src/demo_helloworld.c)`, add the following content in the last new line:
  ```
  list(APPEND sc_demo_src ./src/boat_platone_demo.c)
  list(APPEND sc_demo_src ./src/my_contract.cpp.abi.c)
  ```
  

## Compile BoAT-X-Framework Static library

### 1. Compile BoAT-X Framework static library (under Cygwin)

   #### a. Configure the target platform in directory BoAT-X-Framework/Makefile
   ```
   PLATFORM_TARGET ?= Simcom-A7670C
   ```
   
   #### b. Open a Windows cmd, enter <A7670C Root>/BoAT-X-Framework directory and compile BoAT static library
   ```
   cd <A7670C Root>/BoAT-X-Framework
   make clean
   make all
   ```
   
   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<A7670C Root>/BoAT-X-Framework/lib` directory.
   

### 2. Build demo program, generate .zip file for download

   Demo code for accessing blockchain through BoAT-X Framework is in `<A7670C Root>/sc_demo/src/boat_platone_demo.c`。

   Open a Windows cmd window and build the download file:
   ```
   cd <A7670C Root>
   make clean
   make A7670C_LANS_1606_V701
   ```
   
   The download file `A7670C_LANS_1606_V701.zip` will be generated under `<A7670C Root>/out` once compiled successfully.