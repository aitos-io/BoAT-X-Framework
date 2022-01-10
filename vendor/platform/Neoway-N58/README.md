# BoAT-X-Framework for 有方科技N58整合说明


## 一、前言

本文档说明如何在N58 OpenCPU开发环境中整合BoAT-X-Framework，编译BoAT-X-Framework静态库，以及生成demo程序。

文档中的路径均为在Linux下和Cygwin下的路径，如果需要在windows下进行拷贝，需要将路径中的`/`字符改为`\`。

## 二、拷贝文件

1、拷贝 boat2.0代码BoAT-X-Framework 整个文件夹放在N58平台代码根目录下，即与主CMakeLists.txt平级。  
2、拷贝 BoAT-X-Framework/vendor/platform/Neoway-N58/N58RootDirCode/demo_entry.c到N58平台代码根目录下。  
3、拷贝 BoAT-X-Framework/vendor/platform/Neoway-N58/N58RootDirCode/my_contract.cpp.abi.c到N58平台代码根目录下。  
4、拷贝 BoAT-X-Framework/vendor/platform/Neoway-N58/N58RootDirCode/my_contract.cpp.abi.h到N58平台代码根目录下。  


拷贝后的目录和文件结构如下：

```
<N58平台代码根目录>
|  
+---BoAT-X-Framework  
+---cmake  
|   \---toolchain-gcc.cmake  
+---components  
+---ldscripts  
+---prebuilts  
+---tools  
\---boatentry.c  
\---CMakeLists.txt  
\---my_contract.cpp.abi.c  
\---my_contract.cpp.abi.h  
```

## 三、文件修改

### 1、Add BoAT-X Framework libraries path

  打开N58平台代码根目录/cmake/toolchain-gcc.cmake文件。  
  在之前添加以下内容：
    
    set(libbw_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatwallet.a)
    set(libbv_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatvendor.a)
	
### 2、添加BoAT-X-Framework有关头文件路径到N58平台
  打开N58平台代码根目录/cmake/CMakeLists.txt文件。  
  在include_directories(XXX) 之后添加以下内容：
  
  	include_directories(BoAT-X-Framework/include BoAT-X-Framework/include/protocolapi)
  
### 3、添加BoAT-X-Framework lib文件到N58平台
  打开N58平台代码根目录/cmake/CMakeLists.txt文件。  
  找到target_link_libraries(XXX ${libc_file_name}) 字样，在${libc_file_name} 前面添加 ${libbw_file_name} ${libbv_file_name}，如下所示：  
  
  	target_link_libraries(${target} PRIVATE ${libbw_file_name} ${libbv_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name})

### 4、添加boat2.0测试Demo和智能合约文件到编译目录
  打开N58平台代码根目录/cmake/CMakeLists.txt文件。  
  找到add_appimg(${target} xxx ) 字样，在小括号内加入demo_entry.c my_contract.cpp.abi.c，如下所示：  
  	
	add_appimg(${target} ${file_ldscript} nwy_drv_lcd_st7735.c demo_entry.c my_contract.cpp.abi.c)  

  说明：  
#### 4.1、demo_entry.c 包括main入口、网络初始化和调用合约上链等操作  
  如果和目前应用入口冲突，可注释掉demo_entry.c 中的 appimg_enter()和appimg_exit()，只调用boat_demo_entry()。  
  如果只是用来跑通本demo，可删除当前应用入口文件，只保留boat_entry.c。  
#### 4.2、my_contract.cpp.abi.c my_contract.cpp.abi.h 是boat2.0 make demo后自动生成的合约  
  智能合约的生成，详细信息，可参考 BoAT-X-Framework/README.md  
	
### 5、修改BoAT-X-Framework编译平台为Neoway-N58
  打开N58平台代码根目录/BoAT-X-Framework/Makefile文件。  
  找到PLATFORM_TARGET ?= XXX 修改其值为 Neoway-N58, 即：  
  	
	PLATFORM_TARGET ?= Neoway-N58  


注：以下6、7两条修改，是把BoAT-X-Framework编译环境从Linux改成Windows下的Cygwin。如果是在Linux下编译，可以直接跳过6，7两步。

### 6、修改BoAT-X-Framework编译命令为Cygwin命令
  打开N58平台代码根目录/BoAT-X-Framework/vendor/platform/Neoway-N58/external.env文件。假设Cygwin安装在C:/cygwin64下，则修改为以下值： 
  	
    CYGWIN_BASE := C:/cygwin64 //根据实际Cygwin安装路径修改  
    BOAT_RM := $(CYGWIN_BASE)/bin/rm -rf  
    BOAT_MKDIR := $(CYGWIN_BASE)/bin/mkdir  
    BOAT_FIND := $(CYGWIN_BASE)/bin/find  

### 7、修改BoAT-X-Framework指定编译器
  打开N58平台代码根目录/BoAT-X-Framework/vendor/platform/Neoway-N58/external.env文件，修改CC和AR为以下值：  
  	
    CC := $(CURDIR)/../prebuilts/win32/gcc-arm-none-eabi/bin/arm-none-eabi-gcc  
  	AR := $(CURDIR)/../prebuilts/win32/gcc-arm-none-eabi/bin/arm-none-eabi-ar  

## 四、编译程序

### 1、编译boat2.0生成静态库.a文件
   a、Windows下打开Cygwin终端，或者Linux环境为下打开shell  
   b、cd N58代码根目录/BoAT-X-Framework  
   c、make clean  
   d、make all  
   e、ls -l lib //可看到生成了libboatvendor.a和libboatwallet.a  

### 2、编译N58程序，生成.pac下载文件
   a、Windows下打开Cygwin终端，或者Linux环境为下打开shell   
   b、cd N58代码根目录  
   c、rm -r out  
   d、./nwy_opencpu.bat //在根目录下执行，编译成功会生成xxx_flash.pac文件  


	
	
	
