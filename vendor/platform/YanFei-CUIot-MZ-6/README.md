# BoAT-X-Framework for 中国联通YanFei-CUIot-MZ-6整合说明


## 一、前言

本文档说明如何在Yanfei OpenCPU开发环境中整合BoAT-X-Framework，编译BoAT-X-Framework静态库，以及生成demo程序。


## 二、拷贝文件

1、拷贝 BoAT代码，BoAT-X-Framework 整个文件夹放在YanFei平台代码根目录下，即与主CMakeLists.txt平级。

2、拷贝 BoAT-X-Framework\vendor\platform\YanFei-CUIot-MZ-6\YanfeiRootDirCode\app\demo\src到Yanfei平台代码根目录\app\demo\src\下。


拷贝后的目录和文件结构如下：
```
<Yanfei平台代码根目录>
|
+---app
+---BoAT-X-Framework
+---cmake
+---components
+---examples
+---ldscripts
+---littlevgl
+---out
+---prebuilt
+---tools
\---CMakeList.txt
```


## 三、文件修改

### 1、添加BoAT-X-Framework生成的静态库.a文件到Yanfei平台

  打开Yanfei平台代码根目录\cmake\toolchain-gcc.cmake文件
  
  添加以下两行到对应可加载到的地方：
  ```
  set(libbw_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Frameworkt/lib/libboatwallet.a)
  set(libbv_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Frameworkt/lib/libboatvendor.a)
  ```

### 2、添加BoAT-X-Framework lib文件到target_link_libraries

  打开Yanfei平台代码根目录\app\demo\CMakeList.txt文件
  
  找到target_link_libraries(XXX ${libc_file_name}) 字样，在${libc_file_name} 前面添加 ${libbw_file_name} ${libbv_file_name} ，如：
  ```
  target_link_libraries(${target} PRIVATE ${libbw_file_name} ${libbv_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
  ```

### 3、添加BoAT-X-Framework有关头文件路径到Yanfei平台

  打开Yanfei平台代码根目录下的主CMakeLists.txt文件
  
  找到include_directories(xxx ), 在最后新建一行添加以下BoAT-X-Frameworkt内容：
  ```
  include_directories(BoAT-X-Frameworkt/include BoAT-X-Frameworkt/include/protocolapi)
  ```

### 4、添加BoAT-X-Framework测试Demo、智能合约文件到编译目录

  打开Yanfei平台代码根目录\app\demo\CMakeList.txt文件

  找到add_appimg(${target} xxx ) 字样，在后面添加${target_file_path}/my_contract.c，如：
  ```
  add_appimg(${target} ${flash_ldscript} ${target_file_path}/${target_file}.c ${target_file_path}/my_contract.c ${OEM})
  ```

## 四、编译BoAT-X-Framework静态库

### 1、在Linux环境下编译BoAT-X-Framework静态库.a文件

   #### a、修改BoAT-X-Framework\Makefile中的target
   ```
   PLATFORM_TARGET ?= YanFei-CUIot-MZ-6
   ```
   
   #### b、打开Linux终端进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在BoAT-X-Framework\lib下会生成静态库libboatvendor.a、libboatwallet.a。
   

### 2、编译Yanfei演示demo程序，生成.pac下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在Yanfei平台代码根目录下的\app\demo\src\main.c
   
   打开Linux终端进入Yanfei根目录
   ```
   cd Yanfei代码根目录
   rm -r out//需要先删除本地之前生成的文件
   . tools/core_launch.sh
   cout
   cmake ../.. -G Ninja
   ninja
   ```
   编译成功后，会在out\appimage_debug\hex下生成main_flash.pac文件。
