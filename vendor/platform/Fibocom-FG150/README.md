# BoAT-X-Framework for Fibocom-FG150整合说明


## 一、前言

本文档说明如何在Fibocom-FG150开发环境中整合BoAT-X-Framework、编译BoAT-X-Framework静态库以及生成demo程序。


## 二、拷贝文件

约定`<FG150 Root>`是Fibocom-FG150平台SDK的根目录：

1、拷贝 BoAT代码，将BoAT-X-Framework 整个文件夹拷贝至`<FG150 Root>/fibo-sdk/umdp/example`下。

2、在`<FG150 Root>/fibo-sdk/umdp/example`下新建文件夹`boat_demo`，拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FG150/FG150_demo/platone_demo/demo_platone_mycontract.c到`<FG150 Root>/fibo-sdk/umdp/example/boat_demo`下。

3、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FG150/FG150_demo/platone_demo/my_contract.cpp.abi.c到`<FG150 Root>/fibo-sdk/umdp/example/boat_demo`下。

4、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FG150/FG150_demo/platone_demo/my_contract.cpp.abi.h到`<FG150 Root>/fibo-sdk/umdp/example/boat_demo`下。

5、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FG150/FG150_demo/libcurl.so.4.5.0到`<FG150 Root>/fibo-sdk/umdp/example/boat_demo`下。

6、拷贝并覆盖 BoAT-X-Framework/vendor/platform/Fibocom-FG150/external.env到`<FG150 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default`下。


拷贝后的目录和文件结构如下：
```
<FG150 Root>
|
|-- build.sh
|-- config.ini
|-- fibo-kernel
|-- fibo-rootfs
|-- fibo_sign_package
|-- curl
|-- fibo-compiletool
|-- fibo-ota
|-- fibo-uboot
`-- fibo-sdk
    |-- Makefile
    |-- umdp
    |   |-- bin
    |   |-- config
    |   |-- include
    |   |-- lib
    |   |-- example
    |   |   |-- Makefile
    |   |   |-- BoAT-X-Framework
    |   |   |-- boat_demo
    |   |   |   |-- demo_platone_mycontract.c
    |   |   |   |-- my_contract.cpp.abi.c
    |   |   |   |-- my_contract.cpp.abi.h
    |   |   |   |-- libcurl.so.4.5.0
```


## 三、文件修改


### 1、在Makefile中添加需要引用的BoAT头文件路径

打开`<FG150 Root>/fibo-sdk/Makefile`文件

在配置头文件目录的地方新添加以下内容：
```
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/include
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/vendor/platform/include
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/src/log
```

  
## 四、编译BoAT-X-Framework静态库

### 1、配置交叉编译器
   
   #### a、在Linux上打开一个终端，进入到fibo-compiletool目录
   ```
    cd <FG150 Root>/fibo-compiletool
   ```

   #### b、使交叉编译指令在该终端上生效
   ```
    source environment-setup-aarch64-unisoc-linux
   ```
   生效成功后，在该终端执行`echo $CC`和`echo $AR`会看到相应的命令内容，后续的BoAT静态库的编译也必须保证在该终端中完成。

### 2、编译BoAT静态库

   #### a、配置主Makefile中的编译信息，打开`<FG150 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/Makefile`，修改以下内容（以PlatONE为例）：
   ```
   BOAT_PROTOCOL_USE_PLATONE    ?= 1

   PLATFORM_TARGET ?= linux-default
   ```
   
   #### b、编译BoAT静态库
   ```
   cd <FG150 Root>/fibo-sdk/umdp/example/BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在`<FG150 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/lib`下会生成静态库libboatvendor.a、libboatwallet.a。
   
## 五、编译并运行调用BoAT的demo

### 1、配置区块链信息（以PlatONE为例）

  #### a、将部署好或者已知可用的节点地址配置给`<FG150 Root>/fibo-sdk/umdp/example/boat_demo/demo_platone_mycontract.c`中的`demoUrl`变量，例如：
  ```
  const BCHAR *demoUrl = "http://121.0.0.1:7545";

  ```
  #### b、将已在链上部署好的智能合约地址配置给`<FG150 Root>/fibo-sdk/umdp/example/boat_demo/demo_platone_mycontract.c`中的`demoRecipientAddress`变量，例如：
  ```
  const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";
  ```

### 2、引用智能合约

  #### a、按照BoAT用户手册将智能合约ABI文件编译成c接口文件

  #### b、在`<FG150 Root>/fibo-sdk/umdp/example/boat_demo/demo_platone_mycontract.c`中引用智能合约c接口文件并调用合约的c接口，（以智能合约my_contract为例）例如：
  ```
  #include "my_contract.cpp.abi.h"
  ```
  ```
  result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
  ```

### 3、编译demo

  在终端中执行如下命令：
  ```
  cd boat_demo
  $(CC) $(BUILD_FLAGS) -I. -I../BoAT-X-Framework/include -I../BoAT-X-Framework/vendor/platform/include my_contract.cpp.abi.c demo_platone_mycontract.c ../BoAT-X-Framework/lib/libboatwallet.a ../BoAT-X-Framework/lib/libboatvendor.a libcurl.so.4.5.0 -o sample_platone.bin
  ```
  执行成功后，会在`<FG150 Root>/fibo-sdk/umdp/example/boat_demo`下生成可执行文件`sample_platone.bin`

### 4、运行demo

  #### a、将FG150板子上电并通过USB线连接PC

  #### b、按照板子的文档说明，打开ADB功能

  #### c、在pc上打开一个终端，通过ADB将可执行文件`sample_platone.bin`push到板子内（以/home/user为例）
  ```
  adb push sample_platone.bin /home/user
  ```

  #### d、在FG150的SIM卡槽中插入一张可用的SIM卡

  #### e、在PC上打开一个终端，并运行FG150中的demo
  ```
  adb shell
  cd /home/user
  chmod +x sample_platone.bin
  ./sample_platone.bin
  ```
