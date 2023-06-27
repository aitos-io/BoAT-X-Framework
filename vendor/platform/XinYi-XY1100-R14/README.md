# BoAT-X-Framework for 芯翼XinYi-XY1100-R14整合说明


## 一、前言

本文档说明如何在Windows的XinYi-XY1100-R14开发环境中整合BoAT-X-Framework、编译BoAT-X-Framework静态库以及生成demo程序。


## 二、拷贝文件

约定`<XY1100 Root>`是XinYi-XY1100-R14平台SDK的根目录：

1、拷贝BoAT代码，将BoAT-X-Framework整个文件夹拷贝至`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples`下。

2、在`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples`下新建文件夹`boat_demo`，拷贝 BoAT-X-Framework/vendor/platform/XinYi-XY1100-R14/XY1100R14RootDirCode/platone_demo/boat_platone_demo.c到`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/boat_demo`下。

3、拷贝 BoAT-X-Framework/vendor/platform/XinYi-XY1100-R14/XY1100R14RootDirCode/platone_demo/my_contract.c到`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/boat_demo`下。

4、拷贝 BoAT-X-Framework/vendor/platform/XinYi-XY1100-R14/XY1100R14RootDirCode/platone_demo/my_contract.h到`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/boat_demo`下。


拷贝后的目录和文件结构如下：
```
<XY1100 Root>
|
|-- LICENSE
|-- Makefile
|-- readme_xy.txt
|-- APPLIB
|-- ARCH
|-- DRIVERS
|-- KERNEL
|-- SYSAPP
`-- TARGETS 
    `-- xinyiNBSoC
        |-- GCC-ARM    
        `-- USERAPP
            |-- basic
            |-- doc
            |-- userapp.mk
            `-- examples
                |-- examples.mk
                |-- BoAT-X-Framework
                |-- boat_demo   
```


## 三、文件修改

### 1、在examples.mk中指定需要参与编译的demo源文件

打开`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/examples.mk`文件
  
在源文件添加处新添以下内容：
```
$(wildcard $(EXAMPLES_SRC_DIR)/boat_demo/*.c) \
```

### 2、注册demo的入口函数

打开`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/basic/src/main.c`文件，对demo的入口函数做外部声明之后，将demo入口函数添加至`user_task_init`函数中.以`boat_platone_demo.c`为例，入口函数为`void boat_task_demo_init()`,可以参照如下代码进行注册：
```
extern void boat_task_demo_init();

void user_task_init()
{
  boat_task_demo_init();
}

```

### 3、在examples.mk中添加需要引用的头文件路径

打开`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/examples.mk`文件

在头文件路径添加处新添以下内容：
```
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/BoAT-X-Framework/include
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/BoAT-X-Framework/vendor/platform/include
```

### 4、添加BoAT-X-Framework编译生成的静态库.a文件路径到XinYi-XY1100-R14平台

打开`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/userapp.mk`文件
  
在链接静态库的位置添加以下内容：
```
SRCS_LIBS_INC += -L$(USERAPP_SRC_DIR)/examples/BoAT-X-Framework/lib/ -lboatwallet
SRCS_LIBS_INC += -L$(USERAPP_SRC_DIR)/examples/BoAT-X-Framework/lib/ -lboatvendor
```

### 5、配置BoAT-X-Framework的交叉编译环境

打开`<XY1100 Root>/userapp/BoAT-X-Framework/vendor/platform/XinYi-XY1100/external.env`文件
  
在`CC`和`AR`后面配置本地交叉编译器的实际路径
  
### 6、配置XinYi-XY1100-R14平台的交叉编译环境
  
按照XY1100-R14的文档要求在本地PC中配置好XinYi-XY1100-R14的交叉编译环境


## 四、编译BoAT-X-Framework静态库

### 1、在Windows下编译BoAT-X-Framework静态库.a文件
   
   #### a、修改`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/BoAT-X-Framework/Makefile`中的target
   ```
   PLATFORM_TARGET ?= XinYi-XY1100-R14
   ```
   
   #### b、打开Cygwin并进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd <XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在`<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/BoAT-X-Framework/lib`下会生成静态库libboatvendor.a、libboatwallet.a。
   

### 2、编译XY1100-R14演示demo程序，生成.mimgx下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在`XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/boat_demo/boat_platone_demo.c`
   
   打开Windows的cmd终端并进入`<XY1100 Root>`
   ```
   cd <XY1100 Root>
   make clean
   make all
   ```
   编译成功会在`<XY1100 Root>/TARGETS/xinyiNBSoC/GCC-ARM/bin`下生成arm.img、ram.bin和flash.bin文件，将arm.img文件拷贝到基线中，通过厂商提供的Logview工具将基线合并为xinyiNBSoc.mimgx下载文件。
