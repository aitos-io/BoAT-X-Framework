# BoAT-X-Framework for 广和通L610整合说明


## 一、前言

本文档说明如何在L610 OpenCPU开发环境中整合BoAT-X-Framework，编译BoAT-X-Framework静态库，以及生成demo程序。


## 二、拷贝文件

约定`<L610 Root>`是L610平台SDK的根目录：

1、拷贝 BoAT代码，将 BoAT-X-Framework 整个文件夹拷贝至`<L610 Root>`下，即与主CMakeLists.txt平级。

2、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/PlatONE_demo/boatdemo.c到`<L610 Root>`下。

3、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/PlatONE_demo/my_contract.cpp.abi.c到`<L610 Root>`下。

4、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/PlatONE_demo/my_contract.cpp.abi.h到`<L610 Root>`下。


拷贝后的目录和文件结构如下：
```
<L610 Root>
|
+---BoAT-X-Framework
+---cmake
+---components
+---ldscripts
+---out
+---prebuilt
+---tools
\---CMakeList.txt
\---boatdemo.c
\---my_contract.cpp.abi.c
\---my_contract.cpp.abi.h
```


## 三、文件修改

### 1、添加BoAT-X-Framework生成的静态库.a文件到L610平台

  打开`<L610 Root>/cmake/toolchain-gcc.cmake`文件。
  
  首先找到if(CONFIG_CPU_ARM_CA5)，然后在下方大括号内添加以下内容：
  ```
  set(libboatwallet_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatwallet.a)
  set(libboatvendor_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatvendor.a)
  ```

### 2、添加BoAT-X-Framework有关头文件路径到L610平台

  打开`<L610 Root>/CMakeLists.txt`文件。
  
  找到include_directories(xxx )，在最后添加以下BoAT-X-Framework内容：
  ```
  include_directories(BoAT-X-Framework/include BoAT-X-Framework/include/protocolapi BoAT-X-Framework/vendor/platform/include)
  ```

### 3、添加BoAT-X-Framework lib文件到target_link_libraries

  打开`<L610 Root>/CMakeLists.txt`文件。
  
  在if(CONFIG_APPIMG_LOAD_FLASH)下方大括号内找到target_link_libraries(XXX ${libc_file_name}) 字样，在${libc_file_name} 前面添加 ${libboatwallet_file_name} ${libboatvendor_file_name} ，如：
  ```
  target_link_libraries(${target} PRIVATE ${libboatwallet_file_name} ${libboatvendor_file_name} ${libc_file_name} ${libm_file_name} 
  ```

### 4、添加BoAT-X-Framework测试Demo、智能合约文件到编译目录

  打开`<L610 Root>/CMakeLists.txt`文件。

  在if(CONFIG_APPIMG_LOAD_FLASH)下方大括号内找到add_appimg(${target} xxx ) 字样，在最后添加boatdemo.c my_contract.cpp.abi.c，如：
  ```
  add_appimg(${target} ${flash_ldscript} demo.c my_contract.cpp.abi.c)
  ```

## 四、编译BoAT-X-Framework静态库

### 1、在Linux环境下编译BoAT-X-Framework静态库.a文件

   #### a、修改BoAT-X-Framework/Makefile中的target
   ```
   PLATFORM_TARGET ?= Fibocom-L610
   ```

   #### b、修改BoAT-X-Framework/Makefile中的区块链的选项（以PlatONE为例）
   ```
   BOAT_PROTOCOL_USE_PLATONE       ?= 1
   ```

   #### c、打开Linux终端进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在BoAT-X-Framework/lib下会生成静态库libboatvendor.a、libboatwallet.a。
   

### 2、编译L610演示demo程序，生成.pac下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在`<L610 Root>/boatdemo.c`中。
   
   打开Linux终端进入L610根目录。
   ```
   cd <L610 Root>
   rm -r out//需要先删除本地之前生成的文件
   . tools/core_launch.sh
   cout
   cmake ../.. -G Ninja
   ninja
   ```
   编译成功会在out/appimage_debug/hex/下生成.pac文件。	
