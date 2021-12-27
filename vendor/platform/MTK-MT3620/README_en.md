# BoAT-X Framework for MTK-MT3620 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into MTK-MT3620 SDK, compile BoAT-X Framework static library and build the demo program.


## Copy files

Assuming `<MT3620 Root>` to be the root directory of MTK-MT3620 platform SDK:

1. Copy the entire BoAT-X-Framework directory into `<MT3620 Root>`.

2. Delete `<MT3620 Root>/BoAT-X-Framework/CMakeLists.txt`.

3. Copy `<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/CMakeLists-lib.txt` into `<MT3620 Root>/BoAT-X-Framework/`, rename `CMakeLists-lib.txt` to `CMakeLists.txt`.

4. Copy `<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/CMakeSettings.json` into `<MT3620 Root>/BoAT-X-Framework/`.

5. Create new folders `boat_demo` and `boatiotsdk` under `<MT3620 Root>/customer`.

6. Copy `<MT3620 Root>/BoAT-X-Framework/include` into `<MT3620 Root>/customer/boatiotsdk`.

7. Copy `<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/HelloWorld.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/HelloWorld.h`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/my_contract.cpp.abi.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/my_contract.cpp.abi.h`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/StoreRead.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/StoreRead.h` into `<MT3620 Root>/customer/boat_demo`. 

8. Copy `<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/app_manifest.json`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/applibs_versions.h`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/CMakeLists.txt`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/CMakeSettings.json`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/launch.vs.json`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main.h`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-ethereum.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-fiscobcos.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-fiscobcos.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-platon.c`、`<MT3620 Root>/BoAT-X-Framework/vendor/platform/MTK-MT3620/MT3620RootDirCode/main-platone.c` into `<MT3620 Root>/customer`.

After these files copied, the directory structure should look like:

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


## Compile BoAT-X Framework Static library

1. Open `Visual Studio 2019`.

2. Click on `Open a local folder`.

3. Select `MT3620 Root`. 

4. Double-click`<MT3620 Root>/BoAT-X-Framework/CMakeLists.txt`.

5. Click on `down-arrow`on the right of `Slect Startup Item `, select `Current Document`.

6. Click on `Build`,click on `Build All`.

   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<MT3620 Root>/BoAT-X-Framework/lib` directory.


## Debug demo program

1. Copy`<MT3620 Root>/BoAT-X-Framework/lib` into `<MT3620 Root>/customer/boatiotsdk`.

2. Double-click `<MT3620 Root>/customer/CMakeLists.txt`。

3. Click on `down-arrow`on the right of `Slect Startup Item `, select `Current Document`.

4. Click on `Build`,click on `Build All`.

5. After compiling, Click the small green arrow `Current Document(CMakeLists.txt)` in the toolbar ,demo will run on the board.
