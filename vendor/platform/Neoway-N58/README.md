N58调用boat2.0整合说明


一 前言

按照文档全部移植完整后，boat2.0需要重新make clean,再make all整个BoAT-X-Framework。
N58平台代码也需要先删除out下的所有文件，再执行编译指令生成可下载.pac文件。


二 拷贝文件

1、拷贝boat2.0代码BoAT-X-Framework 整个文件夹放在N58平台代码根目录下，即与主CMakeLists.txt平级。
2、拷贝 BoAT-X-Framework\vendor\platform\N58\N58RootDirCode\demo_entry.c到N58平台代码根目录下。
3、拷贝 BoAT-X-Framework\vendor\platform\N58\N58RootDirCode\my_contract.cpp.abi.c到N58平台代码根目录下。
4、拷贝 BoAT-X-Framework\vendor\platform\N58\N58RootDirCode\my_contract.cpp.abi.h到N58平台代码根目录下。


拷贝后的目录和文件结构如下：

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


三 文件修改

1、添加boat2.0生成的静态库.a文件到N58平台

  打开N58平台代码根目录\cmake\toolchain-gcc.cmake文件
  在set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/libc.a)之前添加以下内容：
    
	set(libbw_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatwallet.a)
    set(libbh_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatvendor.a)
	
2、添加boat2.0有关头文件路径到N58平台
  打开N58平台代码根目录\cmake\CMakeLists.txt文件
  在include_directories(XXX) 之后添加以下内容：
  
  include_directories(BoAT-X-Framework/include BoAT-X-Framework/include/protocolapi)
  
3、添加boat2.0 lib文件到target_link_libraries
  打开N58平台代码根目录\cmake\CMakeLists.txt文件
  找到target_link_libraries(XXX ${libc_file_name}) 字样，在${libc_file_name} 前面添加 ${libbw_file_name} ${libbh_file_name}，如：
  
  target_link_libraries(${target} PRIVATE ${libbw_file_name} ${libbh_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name})

4、添加boat2.0测试Demo、智能合约文件到编译目录
  打开N58平台代码根目录\cmake\CMakeLists.txt文件
  找到add_appimg(${target} xxx ) 字样，在语句
  add_appimg(${target} ${file_ldscript} nwy_drv_lcd_st7735.c demo_entry.c my_contract.cpp.abi.c)

  说明：
  4.1、demo_entry.c 包括main入口、网络初始化、调用合约上链等操作，
      如果和目前应用入口冲突，可注释掉demo_entry.c 中的 appimg_enter()、appimg_exit()，只调用boat_demo_entry()。
	  如果只是用来跑通本demo，可删除当前应用入口文件，只保留boat_entry.c。
  4.2、my_contract.cpp.abi.c my_contract.cpp.abi.h 是boat2.0 make all后自动生成的合约，在BoAT-X-Framework\contract\generated\目录下
     智能合约的生成，详细信息，可参考 BoAT-X-Framework\README.md
	
5、修改BoAT-X-Framework编译平台为Neoway-N58
  打开 BoAT-X-Framework\Makefile文件
  找到 PLATFORM_TARGET ?= XXX 修改其值为 Neoway-N58, 即：
  PLATFORM_TARGET ?= Neoway-N58


注：以下6、7两条修改，是把BoAT-X-Framework编译环境从Linux改成windows下的Cygwin。否则，不用修改。

6、修改BoAT-X-Framework编译命令为Cygwin命令，如Cygwin安装在C:/cygwin64
  打开 BoAT-X-Framework\Makefile文件，修改为以下值：
  CYGWIN_BASE := C:/cygwin64
  BOAT_RM := $(CYGWIN_BASE)/bin/rm -rf
  BOAT_MKDIR := $(CYGWIN_BASE)/bin/mkdir
  BOAT_FIND := $(CYGWIN_BASE)/bin/find

7、打开 BoAT-X-Framework\vendor\platform\N58\external.env文件，修改为以下值：
  CC := $(CURDIR)/../prebuilts/win32/gcc-arm-none-eabi/bin/arm-none-eabi-gcc
  AR := $(CURDIR)/../prebuilts/win32/gcc-arm-none-eabi/bin/arm-none-eabi-ar

四 编译程序

1、编译boat2.0生成静态库.a文件
   a、打开Cygwin终端
   b、cd BoAT-X-Framework
   c、make clean
   d、make all
   e、ls -l lib //可看到生成了libboatvendor.a、libboatwallet.a

2、编译N58程序，生成.pac下载文件。
   a、打开Cygwin终端
   b、cd N58代码根目录
   c、cd out
   d、rm -r appimage_release
   e、cd .. // 回退到N58代码根目录
   f、./nwy_opencpu.bat // 在根目录下执行，编译成功会生成xxx_flash.pac文件


	
	
	