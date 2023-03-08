# BoAT-X-Framework for 广和通Fibocom-MA510整合说明


## 一、前言

本文档说明如何在Fibocom-MA510开发环境中整合BoAT-X-Framework，编译BoAT-X-Framework的静态库，以及生成demo程序。


## 二、拷贝文件

约定`<MA510 Root>`是Fibocom-MA510平台SDK的根目录：

1、拷贝BoAT代码，将BoAT-X-Framework 整个文件夹拷贝至`<MA510 Root>/fibocom/example`下。

2、在`<MA510 Root>/fibocom/example`下新建文件夹`boat_demo`，拷贝 BoAT-X-Framework/vendor/platform/Fibocom-MA510/MA510RootDirCode/platone_demo下所有内容到`<MA510 Root>/fibocom/example/boat_demo`下。


拷贝后的目录和文件结构如下：
```
<MA510 Root>
|
|-- bin
|-- common
|-- llvmtools
|-- build_all.sh
|-- example.txt
`-- fibocom
    |-- build   
    |-- example
    |   |-- boat_demo
    |   |-- BoAT-X-Framework
```


## 三、文件修改

### 1、在example.txt中添加支持编译的demo

  打开`<MA510 Root>/example.txt`文件
  
  在文件结尾处新添以下内容：
  ```
boat_demo      -__EXAMPLE_BOATDEMO__ -NO -NO
  ```

### 2、在build_all.sh中添加需要引用的BoAT相关头文件路径和生成的静态库路径

打开`<MA510 Root>/build_all.sh`文件

添加BoAT和MA510引用文件的路径：
```
BOAT_INC_PATH="fibocom/example/BoAT-X-Framework/include"
BOAT_PROTOCOL_INC_PATH="fibocom/example/BoAT-X-Framework/include/protocolapi"
BOAT_LOG_INC_PATH="fibocom/example/BoAT-X-Framework/vendor/platform/Fibocom-MA510/src/log"
BOAT_LIB_PATH="fibocom/example/BoAT-X-Framework/lib"
```

### 3、添加BoAT-X-Framework编译生成的静态库.a文件路径到Fibocom-MA510平台

打开`<MA510 Root>/build_all.sh`文件
  
在链接lib库的位置添加BoAT的静态库，例如：
  ```
$TOOLCHAIN_PATH/clang++ -d -o $DEMO_ELF_OUTPUT_PATH/$DAM_ELF_NAME -target armv7m-none-musleabi -fuse-ld=qcld -lc++ -Wl,-mno-unaligned-access -fuse-baremetal-sysroot -fno-use-baremetal-crt -Wl,-entry=$DAM_RO_BASE $DEMO_APP_OUTPUT_PATH/txm_module_preamble_llvm.o -Wl,-T$DAM_SRC_PATH/../app_dam_demo.ld -Wl,-Map,$DEMO_ELF_OUTPUT_PATH/$DAM_MAP_NAME,-gc-sections -Wl,-gc-sections $DEMO_APP_OUTPUT_PATH/*.o  -L $BOAT_LIB_PATH -lboatwallet -L $BOAT_LIB_PATH -lboatvendor $DAM_LIB_PATH/*.lib
  ```
需要注意所链接的库顺序，因为BoAT的静态库调用了MA510平台的接口，所以应该先链接MA510平台的库，之后再链接BoAT相关的两个静态库

### 4、配置BoAT-X-Framework的交叉编译环境

  打开`<MA510 Root>/fibocom/example/BoAT-X-Framework/vendor/platform/Fibocom-MA510/external.env`文件
  
  在`CC`和`AR`后面配置本地交叉编译器的实际路径，例如：
  ```
CC := $(BOAT_BASE_DIR)/../../../llvmtools/4.0.3/bin/clang
AR := $(BOAT_BASE_DIR)/../../../llvmtools/4.0.3/tools/bin/arm-ar
  ```
  

### 5、添加编译BoAT-X-Framework中需要的头文件路径

打开`<MA510 Root>/fibocom/example/BoAT-X-Framework/vendor/platform/Fibocom-MA510/external.env`文件

添加以下内容：
```
EXTERNAL_INC := -I$(BOAT_BASE_DIR)/../../../common/include/qapi \
                -I$(BOAT_BASE_DIR)/../../../common/include/log/inc \
                -I$(BOAT_BASE_DIR)/../../../common/include/threadx_api \
                -I$(BOAT_BASE_DIR)/../../../llvmtools/4.0.3/armv7m-none-eabi/libc \
                -I$(BOAT_BASE_DIR)/../../../llvmtools/4.0.3/armv7m-none-eabi/libc/include \
```

### 6、添加编译BoAT-X-Framework中需要的编译参数

打开`<MA510 Root>/fibocom/example/BoAT-X-Framework/vendor/platform/Fibocom-MA510/external.env`文件

添加以下内容：
```
EXTERNAL_CFLAGS := -marm -target armv7m-none-musleabi -mfloat-abi=softfp -mfpu=none -mcpu=cortex-a7 -mno-unaligned-access  -fms-extensions -Osize -fshort-enums -Wbuiltin-macro-redefined \
                -DQAPI_TXM_MODULE -DTXM_MODULE -DTX_DAM_QC_CUSTOMIZATIONS -DTX_ENABLE_PROFILING -DTX_ENABLE_EVENT_TRACE -DTX_DISABLE_NOTIFY_CALLBACKS  -DFX_FILEX_PRESENT -DTX_ENABLE_IRQ_NESTING  -DTX3_CHANGES
```


## 四、编译BoAT-X-Framework静态库

### 1、在Linux下编译BoAT-X-Framework静态库.a文件
   
   #### a、修改`<MA510 Root>/fibocom/example/BoAT-X-Framework/Makefile`中的target
   ```
   PLATFORM_TARGET ?= Fibocom-MA510
   ```
   
   #### b、打开Linux终端并进入BoAT-X-Framework目录编译BoAT静态库
   ```
   cd <MA510 Root>/fibocom/example/BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在`<MA510 Root>/fibocom/example/BoAT-X-Framework/lib`下会生成静态库libboatvendor.a和libboatwallet.a。
   

### 2、编译MA510演示demo程序，生成镜像下载文件

   通过BoAT-X-Framework访问区块链的演示代码，在`<MA510 Root>/fibocom/example/boat_demo/src/boat_demo.c`
   
   打开Linux终端并进入`<MA510 Root>`
   ```
   cd <MA510 Root>
   ./build_all.sh llvm -c
   ./build_all.sh llvm boat_demo
   ```
   编译成功会在`<MA510 Root>/bin`下生成oem_app_path.ini、boat_demo_dam_demo.map、boat_demo_dam_demo.elf、boat_demo_dam_demo.bin文件。	
