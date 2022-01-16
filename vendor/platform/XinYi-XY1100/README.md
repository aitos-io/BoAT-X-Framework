# BoAT-X-Framework for 芯翼XinYi-XY1100整合说明


## 一、前言

本文档说明如何在XinYi-XY1100开发环境中整合BoAT-X-Framework、编译BoAT-X-Framework静态库以及生成demo程序。


## 二、拷贝文件

约定`<XY1100 Root>`是XinYi-XY1100平台SDK的根目录：

1、拷贝 BoAT代码，将BoAT-X-Framework 整个文件夹拷贝至`<XY1100 Root>/userapp`下。

2、在`<XY1100 Root>/userapp/demo`下新建文件夹`boat_demo`,拷贝 BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/demo/boat_demo.c到`<XY1100 Root>/userapp/demo/boat_demo`下。

3、拷贝 BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/demo/my_contract.c到`<XY1100 Root>/userapp/demo/boat_demo`下。

4、拷贝 BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/demo/my_contract.h到`<XY1100 Root>/userapp/demo/boat_demo`下。

5、拷贝并覆盖 BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/vendor/Makefile到`<XY1100 Root>/userapp/BoAT-X-Framework/vendor`下。

6、拷贝并覆盖 BoAT-X-Framework/vendor/platform/XinYi-XY1100/XY1100RootDirCode/storage/persiststore.c到`<XY1100 Root>/userapp/BoAT-X-Framework/vendor/common/storage`下。


拷贝后的目录和文件结构如下：
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


## 三、文件修改

### 1、在module.mk中指定需要参与编译的demo源文件

  打开`<XY1100 Root>/userapp/module.mk`文件
  
  在源文件添加处新添以下内容：
  ```
  C_FILES_FLASH+=$(TOP_DIR)/userapp/demo/boat_demo/boat_demo.c
  C_FILES_FLASH+=$(TOP_DIR)/userapp/demo/boat_demo/my_contract.c
  ```

### 2、在module.mk中添加需要引用的头文件路径

打开`<XY1100 Root>/userapp/module.mk`文件

在头文件路径添加处新添以下内容：
```
    CFLAGS+= -I$(TOP_DIR)/TCPIP/net_tool/Dtls/mbedtls-2.6.0/include/mbedtls \
             -I$(TOP_DIR)/userapp/BoAT-X-Framework/include \
             -I$(TOP_DIR)/userapp/BoAT-X-Framework/vendor/platform/include \
             -I$(TOP_DIR)/userapp/BoAT-X-Framework/sdk/include \
             -I$(TOP_DIR)/userapp/BoAT-X-Framework/vendor/platform/XinYi-XY1100/src/log \
             -I$(TOP_DIR)/userapp/BoAT-X-Framework/sdk/protocol/common/web3intf \
```

### 3、添加BoAT-X-Framework编译生成的静态库.a文件路径到XinYi-XY1100平台

打开`<XY1100 Root>/userapp/module.mk`文件
  
  在文件最后添加以下内容：
  ```
    LDFLAGS+=-L$(TOP_DIR)/userapp/BoAT-X-Framework/lib/ -lboatwallet    
    LDFLAGS+=-L$(TOP_DIR)/userapp/BoAT-X-Framework/lib/ -lboatvendor
  ```

### 4、配置BoAT-X-Framework的交叉编译环境

  打开`<XY1100 Root>/userapp/BoAT-X-Framework/vendor/platform/XinYi-XY1100/external.env`文件
  
  在`CC`和`AR`后面配置本地交叉编译器的实际路径
  
### 5、配置XinYi-XY1100平台的交叉编译环境
  
  打开`<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile/makefile`文件
  
  在文件开头将本地交叉编译器的路径配置进环境变量中，示例如下：
  ```
  PATH := $(PATH):/home/tools/xinyigcc/gcc-arm-none-eabi-9-2019-q4-major/bin
  ```

### 6、在XinYi-XY1100平台中增加BoAT-X-Framework的功能支持选项

打开`<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile/feature.mk`文件

在文件最后添加以下内容：
```
    BOATSDK_SUPPORT=y
```

### 7、在XinYi-XY1100平台中增加BoAT-X-Framework的编译参数

打开`<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile/makefile`文件

在配置模块附加功能的位置添加以下内容：
```
    ifeq ($(BOATSDK_SUPPORT),y)
        FLAGS+=-DBOATSDK_SUPPORT=1
    endif
```

### 8、在XinYi-XY1100平台中配置mbedtls的宏选项

打开`<XY1100 Root>/TCPIP/net_tool/Dtls/mbedtls_port/los_mbedtls_config.h`文件

添加以下内容：
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

## 四、编译BoAT-X-Framework静态库

### 1、在Linux下编译BoAT-X-Framework静态库.a文件
   
   #### a、修改`<XY1100 Root>/userapp/BoAT-X-Framework/Makefile`中的target
   ```
   PLATFORM_TARGET ?= XinYi-XY1100
   ```
   
   #### b、打开Linux终端并进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd <XY1100 Root>/userapp/BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在`<XY1100 Root>/userapp/BoAT-X-Framework/lib`下会生成静态库libboatvendor.a、libboatwallet.a。
   

### 2、编译XY1100演示demo程序，生成.bin下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在`<XY1100 Root>/userapp/demo/boat_demo/boat_demo.c`
   
   打开Linux终端并进入`<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile`
   ```
   cd <XY1100 Root>/targets/xinyiNBSoc_M3/Makefile
   make clean
   make all
   ```
   编译成功会在`<XY1100 Root>/targets/xinyiNBSoc_M3/Makefile/xinyiNBSoc_M3`下生成ram.bin和flash.bin文件。
