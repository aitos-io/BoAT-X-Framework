# BoAT-X-Framework for 中移物联ML302整合说明


## 一、前言

本文档说明如何在ML302 OpenCPU开发环境中整合BoAT-X-Framework，编译BoAT-X-Framework静态库，以及生成demo程序。


## 二、拷贝文件

约定`<ML302 Root>`是ML302平台SDK的根目录：

1、拷贝 BoAT代码，将BoAT-X-Framework 整个文件夹拷贝至`<ML302 Root>`下，即与主Makefile平级。

2、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\boatdemo.c到`<ML302 Root>\src`下。

3、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\my_contract.cpp.abi.c到`<ML302 Root>\src`下。

4、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\my_contract.cpp.abi.h到`<ML302 Root>\src`下。


拷贝后的目录和文件结构如下：
```
<ML302 Root>
|
+---BoAT-X-Framework
+---doc
+---firmware
+---inc
+---out
+---prebuilt
+---src
|   \---boatdemo.c
|   \---my_contract.cpp.abi.c
|   \---my_contract.cpp.abi.h
+---tools
\---Makefile
```


## 三、文件修改

### 1、在主Makefile中定义BoAT-X-Framework路径变量

  打开`<ML302 Root>\Makefile`文件
  
  在BINMAKER := python tools/private/elf2flash.py之后新建一行添加以下内容：
  ```
  BOAT_BASE := ./BoAT-X-Framework
  ```

### 2、添加BoAT-X-Framework编译生成的静态库.a文件路径到ML302平台

  打开`<ML302 Root>\Makefile`文件
  
  在LDFLAGS += -T prebuilt/components/appstart/flashrun.ld.obj -Wl,-Map=firmware/$(strip $(HWVER))/8910DM_ML302.map之后新建一行添加以下内容：
  ```
  LDFLAGS += $(BOAT_BASE)/lib/libboatwallet.a $(BOAT_BASE)/lib/libboatvendor.a
  ```

### 3、添加BoAT-X-Framework有关头文件路径到ML302 SDK搜索路径中

  打开`<ML302 Root>\Makefile`文件
  
  在INC :=  -I'inc\os' 之后新建一行添加以下内容：
  ```
  INC +=  -I$(BOAT_BASE)\include -I$(BOAT_BASE)\include\protocolapi
  ```


## 四、编译BoAT-X-Framework静态库

### 1、在Cygwin下编译BoAT-X-Framework静态库.a文件
   
   #### a、修改`<ML302 Root>\BoAT-X-Framework\Makefile`中的target
   ```
   PLATFORM_TARGET ?= ChinaMobile-ML302
   ```
   
   #### b、打开Cygwin终端并进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd <ML302 Root>/BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在`<ML302 Root>\BoAT-X-Framework\lib`下会生成静态库libboatvendor.a、libboatwallet.a。
   

### 2、编译ML302演示demo程序，生成.pac下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在`<ML302 Root>\src\boatdemo.c`
   
   打开Windows终端并进入`<ML302 Root>`
   ```
   cd <ML302 Root>
   build.bat CLEAN
   build.bat DNLM
   ```
   编译成功会在`<ML302 Root>\firmware\DNLM`下生成ML302_OpenCPU.pac文件。
