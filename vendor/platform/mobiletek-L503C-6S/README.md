# BoAT-X-Framework for 移柯L503整合说明


## 一、前言

本文档说明如何在L503开发环境中整合BoAT-X-Framework，编译BoAT-X-Framework静态库，以及生成demo程序。


## 二、拷贝文件

约定`<L503 Root>`是L503平台SDK的根目录：

1、拷贝 BoAT代码，将 BoAT-X-Framework 整个文件夹拷贝至`<L503 Root>/app`下。

2、拷贝 `BoAT-X-Framework/vendor/platform/mobiletek-L503C-6S/L503CRootDirCode/app/demo/platone_demo`到`<L503 Root>/app/demo`下。

3、拷贝并覆盖 `BoAT-X-Framework/vendor/platform/mobiletek-L503C-6S/main.c`到`<L503 Root>/app/demo`下。

4、拷贝并覆盖 `BoAT-X-Framework/vendor/platform/mobiletek-L503C-6S/Makefile`到`<L503 Root>/app/demo`下。


拷贝后的目录和文件结构如下：
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


## 三、文件修改

### 1、添加BoAT-X-Framework生成的静态库.a文件到L503平台

  打开`<L503 Root>/app/build/makerules.mk`文件。
  
  首先找到 `DEPENDENCY_INCLUDE_LIB := ${foreach lib, ${LOCAL_EXPORT_LIB}, ${SOFT_WORKDIR}/${lib}}`，然后在下方添加以下内容：
  ```
  DEPENDENCY_INCLUDE_LIB += $(SOFT_WORKDIR)/BoAT-X-Framework/lib/libboatwallet.a
  DEPENDENCY_INCLUDE_LIB += $(SOFT_WORKDIR)/BoAT-X-Framework/lib/libboatvendor.a
  ```

### 2、添加BoAT-X-Framework有关头文件路径到L503平台

  打开`<L503 Root>/app/build/buildvar.mk`文件。
  
  首先找到 `cust_lc/inc`，在后方添加` \`，然后在最后添加以下BoAT-X-Framework内容：
  ```
  BoAT-X-Framework/include/protocolapi \
  BoAT-X-Framework/include \
  BoAT-X-Framework/vendor/platform/include \
  BoAT-X-Framework/sdk/network/platone
  ```

### 3、添加日志打印自定义映射接口

  打开`<L503 Root>/kernel/mbtk/cust_api/customer_api.c`文件。
  
  找到结构体 `mbtk_user_api`，在该结构体中添加函数映射指针：
  ```
  int (*user_uart_printf)(const char *fmt, ...);
  ```

  找到定义映射表结构体 `g_user_api`，在该结构体中添加需要映射函数的初始化：
  ```
  .user_uart_printf = RTI_LOG,
  ```  
  
  打开`<L503 Root>/app/system/inc/mbtk_customer_api.h`文件。
  
  找到结构体 `mbtk_user_api`，在该结构体中添加函数映射指针：
  ```
  int (*user_uart_printf)(const char *fmt, ...);
  ``` 
  
  打开`<L503 Root>/app/system/src/api_customer_init.c`文件。
  
  找到定义 `int (*ol_path_ctrl)(UINT32 path)`;在下方添加定义：
  ```
  int (*user_app_printf)(const char *fmt, ...);
  ```
  
  找到函数 `mbtk_api_init_customer`，在函数中`if(api_table)`下方的大括号内添加：
  ```
  user_app_printf = api_table->user_uart_printf;
  ```
  

## 四、编译BoAT-X-Framework静态库

### 1、在Cygwin环境下编译BoAT-X-Framework静态库.a文件

   #### a、修改BoAT-X-Framework/Makefile中的target
   ```
   PLATFORM_TARGET ?= mobiletek-L503C-6S
   ```
   
   #### b、打开Cygwin终端进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd <L503 Root>/app/BoAT-X-Framework
   ../tools/gnumake.exe clean
   ../tools/gnumake.exe all
   ```
   
   编译成功后，在BoAT-X-Framework/lib下会生成静态库`libboatvendor.a`、`libboatwallet.a`。
   

### 2、编译L503演示demo程序，生成.zip下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在`<L503 Root>/app/demo/platone_demo/platone_demo.c`。
   
   打开windows终端进入`<L503 Root>/app`
   ```
   cd <L503 Root>/app
   build_app.bat
   ```

   编译成功会在`<L503 Root>/app/release`下生成`user_app.bin`文件。

   将`user_app.bin`文件拷贝并覆盖到`<L503 Root>/kernel/mbtk/releasepack/L503C_6S/images`下。

   打开windows终端进入`<L503 Root>/kernel`
   ```
   cd <L503 Root>/kernel
   make_clean.bat
   L503_6S_build_open.bat
   ```

   编译成功会在`<L503 Root>/kernel/OUTPUT_L503C_6S`下生成`ASR_CRANEL_EVB_CRANEL_A0_08MB.zip`下载文件。