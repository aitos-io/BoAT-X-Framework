# BoAT-X-Framework for 芯讯通A7670C整合说明


## 一、前言

本文档说明如何在A7670C开发环境中整合BoAT-X-Framework，编译BoAT-X-Framework静态库，以及生成demo程序。


## 二、拷贝文件

约定`<A7670C Root>`是A7670C平台SDK的根目录：

1、拷贝 BoAT代码，将 BoAT-X-Framework 整个文件夹拷贝至`<A7670C Root>`下。

2、拷贝 `BoAT-X-Framework/vendor/platform/Simcom-A7670C/BoatPlatoneDemo/A7670CRootDirCode/sc_demo/src`下的文件到`<A7670C Root>/sc_demo/src`下。

3、拷贝 `BoAT-X-Framework/vendor/platform/Simcom-A7670C/BoatPlatoneDemo/A7670CRootDirCode/sc_demo/inc`下的文件到`<A7670C Root>/sc_demo/inc`下。

4、拷贝并覆盖 `BoAT-X-Framework/vendor/platform/Simcom-A7670C/BoatPlatoneDemo/A7670CRootDirCode/sc_application.c`到`<A7670C Root>`下。

拷贝后的目录和文件结构如下：
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


## 三、文件修改

### 1、添加BoAT-X-Framework生成的静态库.a文件到A7670C平台

  打开`<A7670C Root>/CMakeLists.txt`文件。
  
  首先找到
  ```
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
    "${CMAKE_SOURCE_DIR}/sc_lib/${SCMODULE}/lib/libsc_lib.a"
  )
  endif()
  endif()
  ```
  然后在下方添加以下内容：
  ```
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
      "${CMAKE_SOURCE_DIR}/BoAT-X-Framework/lib/libboatwallet.a"
  )
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
      "${CMAKE_SOURCE_DIR}/BoAT-X-Framework/lib/libboatvendor.a"
  )
  ```

### 2、添加BoAT-X-Framework有关头文件路径到A7670C平台

  打开`<A7670C Root>/sc_demo/CMakeLists.txt`文件。
  
  首先找到 `${CMAKE_SOURCE_DIR}/sc_demo/inc`，然后在下方添加以下内容：
  ```
  ${CMAKE_SOURCE_DIR}/BoAT-X-Framework/include
  ${CMAKE_SOURCE_DIR}/BoAT-X-Framework/include/protocolapi
  ${CMAKE_SOURCE_DIR}/BoAT-X-Framework/vendor/platform/include
  ${CMAKE_SOURCE_DIR}/BoAT-X-Framework/sdk/network/platone
  ```

  再找到 `list(APPEND sc_demo_src ./src/demo_helloworld.c)`，然后在下方添加以下内容：
  ```
  list(APPEND sc_demo_src ./src/boat_platone_demo.c)
  list(APPEND sc_demo_src ./src/my_contract.cpp.abi.c)
  ```


## 四、编译BoAT-X-Framework静态库

### 1、在Cygwin环境下编译BoAT-X-Framework静态库.a文件

   #### a、修改BoAT-X-Framework/Makefile中的target
   ```
   PLATFORM_TARGET ?= Simcom-A7670C
   ```
   
   #### b、打开Cygwin终端进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd <A7670C Root>/BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在BoAT-X-Framework/lib下会生成静态库`libboatvendor.a`、`libboatwallet.a`。
   

### 2、编译A7670C演示demo程序，生成.zip下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在`<A7670C Root>/sc_demo/src/boat_platone_demo.c`。

   打开windows终端进入`<A7670C Root>`
   ```
   cd <A7670C Root>
   make clean
   make A7670C_LANS_1606_V701
   ```

   编译成功会在`<A7670C Root>/out`下生成`A7670C_LANS_1606_V701.zip`下载文件。