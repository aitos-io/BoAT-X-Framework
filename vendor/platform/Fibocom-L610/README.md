# BoAT-X-Framework for 广和通L610整合说明


## 一、前言

本文档说明如何在L610 OpenCPU开发环境中整合BoAT-X-Framework，编译BoAT-X-Framework静态库，以及生成demo程序。


## 二、拷贝文件

1、拷贝 BoAT代码，BoAT-X-Framework 整个文件夹放在L610平台代码根目录下，即与主CMakeLists.txt平级。

2、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/demo.c到L610平台代码根目录下。

3、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/my_contract.cpp.abi.c到L610平台代码根目录下。

4、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/my_contract.cpp.abi.h到L610平台代码根目录下。


拷贝后的目录和文件结构如下：
```
<L610平台代码根目录>
|
+---BoAT-X-Framework
+---cmake
+---components
+---ldscripts
+---out
+---prebuilt
+---tools
\---CMakeList.txt
\---demo.c
\---my_contract.cpp.abi.c
\---my_contract.cpp.abi.h
```


## 三、文件修改

### 1、添加BoAT-X-Framework生成的静态库.a文件到L610平台

  打开L610平台代码根目录/cmake/toolchain-gcc.cmake文件。
  
  首先找到if(CONFIG_CPU_ARM_CA5)，然后在下方大括号内添加以下内容：
  ```
  set(libboatwallet_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatwallet.a)
  set(libboatvendor_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatvendor.a)
  ```

### 2、添加BoAT-X-Framework有关头文件路径到L610平台

  打开L610平台代码根目录下的主CMakeLists.txt文件。
  
  找到include_directories(xxx ), 在最后添加以下BoAT-X-Framework内容：
  ```
  include_directories(BoAT-X-Framework/include BoAT-X-Framework/include/protocolapi BoAT-X-Framework/vendor/platform/include)
  ```

### 3、添加BoAT-X-Framework lib文件到target_link_libraries

  打开L610平台代码根目录下的主CMakeLists.txt文件。
  
  在if(CONFIG_APPIMG_LOAD_FLASH)下方大括号内找到target_link_libraries(XXX ${libc_file_name}) 字样，在${libc_file_name} 前面添加 ${libboatwallet_file_name} ${libboatvendor_file_name} ，如：
  ```
  target_link_libraries(${target} PRIVATE ${libboatwallet_file_name} ${libboatvendor_file_name} ${libc_file_name} ${libm_file_name} 
  ```

### 4、添加BoAT-X-Framework测试Demo、智能合约文件到编译目录

  打开L610平台代码根目录下的主CMakeLists.txt文件。

  在if(CONFIG_APPIMG_LOAD_FLASH)下方大括号内找到add_appimg(${target} xxx ) 字样，在最后添加demo.c my_contract.cpp.abi.c，如：
  ```
  add_appimg(${target} ${flash_ldscript} demo.c my_contract.cpp.abi.c)
  ```

## 四、编译BoAT-X-Framework静态库

### 1、在Linux环境下编译BoAT-X-Framework静态库.a文件

   #### a、修改BoAT-X-Framework/Makefile中的target
   ```
   PLATFORM_TARGET ?= Fibocom-L610
   ```

   #### b、关闭FABRIC链宏开关  

   由于资源限制，该平台暂时不支持FABRIC链。所以，需要打开主makefile，找到`BOAT_PROTOCOL_USE_HLFABRIC`,将后面参数改为0，即`BOAT_PROTOCOL_USE_HLFABRIC  ?= 0`。
   
   #### c、打开Linux终端进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在BoAT-X-Framework/lib下会生成静态库libboatvendor.a、libboatwallet.a。
   

### 2、编译L610演示demo程序，生成.pac下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在L610平台代码根目录下的demo.c。
   
   打开Linux终端进入L610根目录。
   ```
   cd L610代码根目录
   rm -r out//需要先删除本地之前生成的文件
   . tools/core_launch.sh
   cout
   cmake ../.. -G Ninja
   ninja
   ```
   编译成功会在out/appimage_debug/hex/下生成.pac文件。	
