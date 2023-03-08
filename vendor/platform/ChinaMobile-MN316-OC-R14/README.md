# BoAT-X-Framework for 中移物联MN316整合说明


## 一、前言

本文档说明如何在MN316开发环境中整合BoAT-X-Framework、编译BoAT-X-Framework静态库以及生成demo程序。MN316的SDK版本号为MN316_OpenCPU_R14__Standard_1.0.0.2203241036_release。

## 二、拷贝文件

约定`<MN316 Root>`是MN316_OpenCPU_R14平台SDK的根目录：

以下以PlatONE为例。

1、拷贝BoAT代码，将BoAT-X-Framework 整个文件夹拷贝至`<MN316 Root>`下。

2、删除`<MN316 Root>\custom\src`下`custom_main.c`，删除`<MN316 Root>\custom\inc`下`custom_main.h`拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-MN316-OC-R14\MN316RootDirCode\boat_demo.c到`<MN316 Root>\custom\src`下。

3、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-MN316-OC-R14\MN316RootDirCode\cm_demo_asocket.c到`<MN316 Root>\custom\src`下。

4、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-MN316-OC-R14\MN316RootDirCode\my_contract.c到`<MN316 Root>\custom\src`下。

5、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-MN316-OC-R14\MN316RootDirCode\my_contract.h到`<MN316 Root>\custom\inc`下。

拷贝后的目录和文件结构如下：
```
<MN316 Root>
|-- BoAT-X-Framework
`-- custom
    |-- inc
    |-- src
|-- docs
|-- examples
|-- include
|-- prebuild
|-- src
|-- third-party
|-- tools
```


## 三、文件修改

### 1、修改MN316的custom.mk

  打开`<MN316 Root>\custom\custom.mk`文件
  
  修改为如下内容：
  ```
  ###C soruce file on flash
  C_FILES_FLASH += custom/src/boat_demo.c
  C_FILES_FLASH += custom/src/my_contract.c
  C_FILES_FLASH += custom/src/cm_demo_asocket.c

  ###Public include path for other module
  CFLAGS += -Icustom/inc
  CFLAGS +=  -IBoAT-X-Framework\include
  CFLAGS +=  -IBoAT-X-Framework\include\protocolapi
  CFLAGS +=  -IBoAT-X-Framework\include\network
  CFLAGS +=  -IBoAT-X-Framework\vendor\platform\include
  CFLAGS +=  -IBoAT-X-Framework\vendor\platform\ChinaMobile-MN316-OC-R14\src\log
  ```

### 2、修改MN316\tools\scripts\Makefile

  打开`<MN316 Root>\tools\scripts\Makefile`文件
  
  找到以下内容：
  ```
  LIBS = -lsysapp -lcustom -lm
  ```

  修改为：
  ```
  LIBS = -lsysapp -lcustom -lm -lboatwallet -lboatvendor
  ```

### 3、配置BoAT-X-Framework的交叉编译环境

  打开`BoAT-X-Framework\vendor\platform\ChinaMobile-MN316-OC-R14\external.env`文件
  
  ```
  BOAT_FIND  := C:\cygwin64\bin\find
  ```
  将Cygwin的路径修改为真实路径。



## 四、编译BoAT-X-Framework静态库

### 1、在Windows下编译BoAT-X-Framework静态库.a文件
   
   #### a、修改`<MN316 Root>\BoAT-X-Framework\Makefile`中的PLATFORM_TARGET
   ```
   PLATFORM_TARGET ?= ChinaMobile-MN316-OC-R14
   ```
   
   #### b、打开cmd并进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd <MN316 Root>\BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在`<MN316 Root>\BoAT-X-Framework\lib`下会生成静态库libboatvendor.a、libboatwallet.a。
   

### 2、编译MN316演示demo程序
   
  拷贝 BoAT-X-Framework\lib\libboatvendor.a到`<MN316 Root>\prebuild\lib`下。
  拷贝 BoAT-X-Framework\lib\libboatwallet.a到`<MN316 Root>\prebuild\lib`下。

   打开cmd并进入`<MN316 Root>`
   ```
   cd <MN316 Root>
   build.bat clean
   build.bat dbrs_h0
   ```
