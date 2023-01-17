# BoAT-X-Framework for Fibocom-FM650整合说明


## 一、前言

本文档说明如何在Fibocom-FM650开发环境中整合BoAT-X-Framework、编译BoAT-X-Framework静态库以及生成demo程序。


## 二、拷贝文件

约定`<FM650 Root>`是Fibocom-FM650平台SDK的根目录：

1、拷贝 BoAT代码，将BoAT-X-Framework 整个文件夹拷贝至`<FM650 Root>/fibo-sdk/umdp/example`下。

2、在`<FM650 Root>/fibo-sdk/umdp/example`下新建文件夹`boat_demo`，拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/platone_demo/demo_platone_mycontract.c到`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下。

3、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/platone_demo/my_contract.cpp.abi.c到`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下。

4、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/platone_demo/my_contract.cpp.abi.h到`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下。

5、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/Makefile到`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下。

6、拷贝 BoAT-X-Framework/vendor/platform/Fibocom-FM650/FM650_demo/libcurl.so.4.5.0到`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下。

7、拷贝并覆盖 BoAT-X-Framework/vendor/platform/Fibocom-FM650/external.env到`<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default`下。


拷贝后的目录和文件结构如下：
```
<FM650 Root>
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
    |   |   |   |-- Makefile
    |   |   |   |-- demo_platone_mycontract.c
    |   |   |   |-- my_contract.cpp.abi.c
    |   |   |   |-- my_contract.cpp.abi.h
    |   |   |   |-- libcurl.so.4.5.0
```


## 三、文件修改


### 1、在Makefile中添加需要引用的BoAT头文件路径

打开`<FM650 Root>/fibo-sdk/Makefile`文件

在配置头文件目录的地方新添加以下内容：
```
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/include
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/vendor/platform/include
export CFLAGS+=-I$(CUR_PATH)/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/src/log
```

### 2、配置curl头文件路径

  打开`<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/external.env`文件
  
  在`EXTERNAL_INC`后面配置FM650平台curl头文件的实际路径，例如：
```
EXTERNAL_INC := -I/home/linux/FM650/fg650-opensdk
```
  
## 四、编译BoAT-X-Framework静态库

### 1、配置交叉编译器
   
   #### a、在Linux上打开一个终端，进入到fibo-compiletool目录
   ```
    cd <FM650 Root>/fibo-compiletool
   ```

   #### b、使交叉编译指令在该终端上生效
   ```
    source environment-setup-aarch64-unisoc-linux
   ```
   生效成功后，在该终端执行`echo $CC`和`echo $AR`会看到相应的命令内容，后续的BoAT静态库的编译也必须保证在该终端中完成。

### 2、编译BoAT静态库

   #### a、配置主Makefile中的编译信息，打开`<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/Makefile`，修改以下内容（以PlatONE为例）：
   ```
   BOAT_PROTOCOL_USE_PLATONE    ?= 1

   PLATFORM_TARGET ?= linux-default
   ```
   
   #### b、编译BoAT静态库
   ```
   cd <FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework
   make clean
   make all
   ```
   
   编译成功后，在`<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/lib`下会生成静态库libboatvendor.a、libboatwallet.a。
   
## 五、编译并运行调用BoAT的demo

### 1、配置区块链信息（以PlatONE为例）

  #### a、将部署好或者已知可用的节点地址配置给`<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/demo_platone_mycontract.c`中的`demoUrl`变量，例如：
  ```
  const BCHAR *demoUrl = "http://121.0.0.1:7545";

  ```
  #### b、将已在链上部署好的智能合约地址配置给`<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/demo_platone_mycontract.c`中的`demoRecipientAddress`变量，例如：
  ```
  const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";
  ```

### 2、引用智能合约

  #### a、按照BoAT用户手册将智能合约ABI文件编译成c接口文件

  #### b、在`<FM650 Root>/fibo-sdk/umdp/example/BoAT-X-Framework/vendor/platform/linux-default/demo_platone_mycontract.c`中引用智能合约c接口文件并调用合约的c接口，（以智能合约my_contract为例）例如：
  ```
  #include "my_contract.cpp.abi.h"
  ```
  ```
  result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
  ```

### 3、编译demo

  #### a、运行`<FM650 Root>/build.sh`并配置项目
  ```
  ./build.sh project FG650_CN_OPENCPU_OPEN
  ```

  #### b、编译fibo-sdk
  ```
  ./build.sh sdk
  ```
  执行成功后，会在`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下生成`Object`文件夹，其中包含最终生成的可执行文件`call_boat_test`

### 4、运行demo

  #### a、将FM650板子上电并通过USB线连接PC

  #### b、按照板子的文档说明，打开ADB功能

  #### c、在pc上打开一个终端，通过ADB将可执行文件`call_boat_test`push到板子内（以/home/user为例）
  ```
  adb push call_boat_test /home/user
  ```

  #### d、在FM650的SIM卡槽中插入一张可用的SIM卡

  #### e、在PC上打开一个终端，并运行FM650中的demo
  ```
  adb shell
  cd /home/user
  chmod +x call_boat_test
  ./call_boat_test
  ```
