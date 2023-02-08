# BoAT-X-Framework for 中移物联ML302整合说明


## 一、前言

本文档说明如何在ML302开发环境中整合BoAT-X-Framework、编译BoAT-X-Framework静态库以及生成demo程序。ML302的SDK版本号为OpenCPU_3.1.0.2021072708。

## 二、拷贝文件

约定`<ML302 Root>`是XinYi-XY1100平台SDK的根目录：

以下以PlatONE为例。

1、拷贝BoAT代码，将BoAT-X-Framework 整个文件夹拷贝至`<ML302 Root>`下。

2、在`<ML302 Root>\src`下新建文件夹`boat`，拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302v2\ML302RootDirCode\PlatONE_demo\boatPlatONEdemo.c到`<ML302 Root>\src\boat`下。

3、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302v2\ML302RootDirCode\PlatONE_demo\my_contract.cpp.abi.c到`<ML302 Root>\src\boat`下。

4、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302v2\ML302RootDirCode\PlatONE_demo\boat.mk到`<ML302 Root>\src\boat`下。

5、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302v2\ML302RootDirCode\core_export.list到`<ML302 Root>\src\app`下，替换core_export.list文件。

拷贝后的目录和文件结构如下：
```
<ML302 Root>
|
|-- BoAT-X-Framework
|-- doc
|-- firmware
`-- inc
    |-- bluetooth
    |-- cJSON
|-- prebuilt
`-- src
    |-- app
    |-- boat
    |-- demo
|-- tools
```


## 三、文件修改

### 1、修改ML302的makefile

打开`<ML302>\Makefile`文件
  
  找到`#########APP_START##############`
  在下方增加以下内容：
  ```
    include src/boat/boat.mk
    SRC_DIRS += src\demo\wolfssl-3.15.3\src
    SRC_DIRS += src\demo\wolfssl-3.15.3\src\wolfcrypt\src
    INC      += -Isrc\demo\wolfssl-3.15.3\include
    INC      += -Isrc\demo\async_socket
  ```

  然后找到如下段落：
  ```
    #**ftp demo依赖atchart**-zyf
    ifeq ($(cm_atchart_on),y)
    C_DEFINES += -DCM_ATCHART_SUPPORT
    LDFLAGS += prebuilt/common/at_vcom_302.o  prebuilt/common/cm_atchart.o
    ifeq ($(cm_net_autoreg_on),y)
    C_DEFINES += -DCM_NET_AUTOREG_SUPPORT -DCM_HARDWARE_VERSION=\"$(strip $(HWVER))\" -DCM_HARDWARE_$(strip $(HWVER))_SUPPORT -DCM_OPENCPU_MODEL_$(strip $(MODEL_NAME)) -DCM_PROD_VERSION_$(strip $(PROD_VER))
    LDFLAGS += prebuilt/common/cm_net_register.o
    endif
  ```

  将其修改为：
  ```
  C_DEFINES += -DCM_ATCHART_SUPPORT
  LDFLAGS += prebuilt/common/at_vcom_302.o  prebuilt/common/cm_atchart.o

  C_DEFINES += -DCM_NET_AUTOREG_SUPPORT -DCM_HARDWARE_VERSION=\"$(strip $(HWVER))\" -DCM_HARDWARE_$(strip $(HWVER))_SUPPORT -DCM_OPENCPU_MODEL_$(strip $(MODEL_NAME)) -DCM_PROD_VERSION_$(strip $(PROD_VER))
  LDFLAGS += prebuilt/common/cm_net_register.o
  ```

### 2、配置BoAT-X-Framework的交叉编译环境

  打开`BoAT-X-Framework\vendor\platform\ChinaMobile-ML302v2\external.env`文件
  
  ```
  CYGWIN_BASE := D:\cygwin64\bin
  ```
  将Cygwin的路径修改为真实路径。
  
### 3、关闭cm_feature.mk中所有开关
  
  打开`<ML302 Root>\cm_feature.mk`文件，
  
  将其中所有开关从y改成n。

### 4、删除cm_main.c

  找到`<ML302 Root>\src\cm_main_.c`文件并删除。

### 5、修改wolfssl配置

  找到`<ML302 Root>\src\demo\wolfssl-3.15.3\include\wolfssl\wolfcrypt\settings.h`，增加以下宏开关。
  ```
  #define HAVE_ECC
  #undef NO_ECC256
  #define ECC_USER_CURVES
  #define HAVE_AESGCM
  #define WOLFSSL_SHA384
  #define WOLFSSL_SHA512
  #define NO_ASN_TIME
  #define HAVE_TLS_EXTENSIONS
  #define HAVE_SUPPORTED_CURVES
  ```
  
  找到`<ML302 Root>\src\demo\wolfssl-3.15.3\src\internal.c`中的`word32 LowResTimer(void)`，如下：
  ```
    word32 LowResTimer(void)
    {
        return (word32)XTIME(0);
    }
  ```
  修改为：
  ```
    word32 LowResTimer(void)
    {
        return (word32)osiEpochSecond();
    }
  ```

## 四、编译BoAT-X-Framework静态库

### 1、在Windows下编译BoAT-X-Framework静态库.a文件
   
   #### a、修改`<ML302 Root>\BoAT-X-Framework\Makefile`中的PLATFORM_TARGET
   ```
   PLATFORM_TARGET ?= ChinaMobile-ML302v2
   ```

   #### b、修改`<ML302 Root>\BoAT-X-Framework\Makefile`中的CJSON_LIBRARY
   ```
   CJSON_LIBRARY ?= CJSON_OUTTER
   ```
   
   #### c、打开cmd并进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd <ML302 Root>\BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在`<ML302 Root>\BoAT-X-Framework\lib`下会生成静态库libboatvendor.a、libboatwallet.a。
   

### 2、编译ML302演示demo程序，生成.pac下载文件
   
   打开cmd并进入`<ML302 Root>`
   ```
   cd <ML302 Root>
   build.bat CLEAN
   build.bat
   ```
   编译成功会在`<ML302 Root>\firmware\ML302\DNLM\H2\ML302_OpenCPU.pac`下生成ML302_OpenCPU.pac文件。
