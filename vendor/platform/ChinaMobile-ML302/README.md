ChinaMobile-ML302调用boat2.0整合说明


一 前言

按照文档全部移植完成后，boat2.0需要重新make clean,再make all整个boatiotsdk。


二 拷贝文件

1、拷贝boat2.0代码，BoAT-X-Framework 整个文件夹放在ML302平台代码根目录下，即与主Makefile平级。
2、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\boatdemo.c到ML302平台代码根目录\src下。
3、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\my_contract.cpp.abi.c到ML302平台代码根目录\src下。
4、拷贝 BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\my_contract.cpp.abi.h到ML302平台代码根目录\src下。


拷贝后的目录和文件结构如下：

<ML302平台代码根目录>
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



三 文件修改
1、在主Makefile中定义boat2.0路径变量
  打开ML302平台代码根目录下的主Makefile文件
  在BINMAKER := python tools/private/elf2flash.py之后新建一行添加以下内容：
  
  BOAT_BASE := ./BoAT-X-Framework

2、添加boat2.0生成的静态库.a文件到ML302平台
  打开ML302平台代码根目录下的主Makefile文件
  在LDFLAGS += -T prebuilt/components/appstart/flashrun.ld.obj -Wl,-Map=firmware/$(strip $(HWVER))/8910DM_ML302.map之后新建一行添加以下内容：

  LDFLAGS += $(BOAT_BASE)/lib/libboatwallet.a $(BOAT_BASE)/lib/libboatvendor.a

3、添加boat2.0有关头文件路径到ML302平台
  打开ML302平台代码根目录下的主Makefile文件
  在INC :=  -I'inc\os' 之后新建一行添加以下内容：
  
  INC +=  -I$(BOAT_BASE)\include -I$(BOAT_BASE)\include\protocolapi


四 编译程序

1、编译boat2.0生成静态库.a文件
   a、修改CygWin安装路径
   打开BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\external.env文件，修改以下值为当前电脑CygWin的实际安装目录
   CYGWIN_BASE := D:\cygwin64
   b、打开win10终端进入BoAT-X-Framework目录
   c、cd BoAT-X-Framework
   d、make clean
   e、make all
   f、dir -l lib //可看到生成了libboatvendor.a、libboatwallet.a

2、编译ML302程序，生成.pac下载文件。
   a、打开win10终端进入ML302根目录
   b、cd ML302代码根目录
   c、build.bat CLEAN
   d、build.bat DNLM　／/ 在根目录下执行，编译成功会在firmware\DNLM下生成ML302_OpenCPU.pac文件


	
	
	