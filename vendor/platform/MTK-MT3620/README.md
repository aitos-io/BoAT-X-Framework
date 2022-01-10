# BoAT-X-Framework for MTK-MT3620整合说明


## 一、前言

本文档说明如何在MTK-MT3620开发环境中整合BoAT-X-Framework、编译BoAT-X-Framework静态库以及生成demo程序。


## 二、拷贝文件

约定`<MT3620 Root>`是MTK-MT3620平台SDK的根目录：

1、拷贝 BoAT代码，将BoAT-X-Framework 整个文件夹拷贝至`<MT3620 Root>/`下。

2、删除`<MT3620 Root>/BoAT-X-Framework/CMakeLists.txt`。

3、拷贝`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/CMakeLists-lib.txt`到`<MT3620 Root>/BoAT-X-Framework/`下，重命名`CMakeLists-lib.txt`为`CMakeLists.txt`。

4、拷贝`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/CMakeSettings.json`到`<MT3620 Root>/BoAT-X-Framework/`下。

5、`<MT3620 Root>/customer`下新建文件夹`boat_demo`、`boatiotsdk`。

6、拷贝`<MT3620 Root>/BoAT-X-Framework/include`到`<MT3620 Root>/customer/boatiotsdk`下。

7、拷贝`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/HelloWorld.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/HelloWorld.h`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/my_contract.cpp.abi.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/my_contract.cpp.abi.h`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/StoreRead.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/StoreRead.h`到`<MT3620 Root>/customer/boat_demo`下。

8、拷贝`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/app_manifest.json`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/applibs_versions.h`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/CMakeLists.txt`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/CMakeSettings.json`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/launch.vs.json`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main.h`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-ethereum.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-fiscobcos.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-fiscobcos.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-platon.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-platone.c`到`<MT3620 Root>/customer`下。

拷贝后的目录和文件结构如下：
```
<MT3620 Root>
|
|-- BoAT-X-Framework
|-- customer
    |-- boat_demo
    |-- boatiotsdk
        |-- include
|-- Hardware
|-- tools
```


## 三、编译BoAT-X-Framework静态库

1、打开`Visual Studio 2019`。

2、点击 `打开本地文件夹`。

3、选择MTK-MT3620平台SDK。

4、双击`<MT3620 Root>/BoAT-X-Framework/CMakeLists.txt`。

5、点击`选择启动项`右侧向下箭头，选择`当前文件`。

6、点击`生成`，点击`全部生成`。

   编译成功后，在`<MT3620 Root>/BoAT-X-Framework/lib`下会生成静态库libboatvendor.a、libboatwallet.a。
   

## 四、Debug demo程序

1、拷贝`<MT3620 Root>/BoAT-X-Framework/lib`到`<MT3620 Root>/customer/boatiotsdk`下。

2、双击`<MT3620 Root>/customer/CMakeLists.txt`。

3、点击`选择启动项`右侧向下箭头，选择`当前文件`。

4、点击`生成`，点击`全部生成`。

5、编译成功后，点击工具栏中绿色小箭头`当前文件(CMakeLists.txt)`,demo会运行到板子上执行。
