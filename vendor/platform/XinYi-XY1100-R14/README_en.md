# BoAT-X Framework for XinYi-XY1100-R14 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into XinYi-1100-R14 SDK, compile BoAT-X Framework static library and build the demo program.


## Copy files

Assuming `<XY1100 Root>` to be the root directory of XinYi-XY1100-R14 platform SDK:

1. Copy the entire BoAT-X-Framework directory into `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples`.

2. Create a new folder `boat_demo` under `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples`, copy BoAT-X-Framework/vendor/platform/XinYi-XY1100-R14/XY1100R14RootDirCode/platone_demo/boat_platone_demo.c into `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/boat_demo`.

3. Copy BoAT-X-Framework/vendor/platform/XinYi-XY1100-R14/XY1100R14RootDirCode/platone_demo/my_contract.c into `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/boat_demo`.

4. Copy BoAT-X-Framework/vendor/platform/XinYi-XY1100-R14/XY1100R14RootDirCode/platone_demo/my_contract.h into `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/boat_demo`.


After copying these files, the directory structure should look like:

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


## Files Modification

### 1. Specify demo source files as inputs of Compilation in examples.mk

Open `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/examples.mk` 
Add the following line as below:
```
$(wildcard $(EXAMPLES_SRC_DIR)/boat_demo/*.c) \
```

### 2.Register demo's entry-point function
Open `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/basic/src/main.c`, make an external declaration of the demo entry function, and add the demo entry function to the `user_task_init` function. Use `boat_platone_demo.c `For example, the entry function is `void boat_task_demo_init()`, which can be registered by referring to the following code:
```
extern void boat_task_demo_init();

void user_task_init()
{
  boat_task_demo_init();
}
```

### 3. Add the header file path that needs to be quoted in examples.mk

Open `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/examples.mk` 
Add the following content in the header file path addition:
```
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/BoAT-X-Framework/include
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/BoAT-X-Framework/vendor/platform/include
```


### 4. Add the static library file path to XinYi-XY1100-R14 platform
Open `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/userapp.mk` 
Add the following in the location where the static library is linked:
```
SRCS_LIBS_INC += -L$(USERAPP_SRC_DIR)/examples/BoAT-X-Framework/lib/ -lboatwallet
SRCS_LIBS_INC += -L$(USERAPP_SRC_DIR)/examples/BoAT-X-Framework/lib/ -lboatvendor
```

### 5. Configure the cross-compilation environment of BoAT-X-Framework
Open `<XY1100 Root>/userapp/BoAT-X-Framework/vendor/platform/XinYi-XY1100/external.env` 
Configure the actual path of the local cross compiler after `CC` and `AR`

### 6. Configure the cross-compilation environment of XinYi-XY1100-R14 platform
Configure the cross-compilation environment of XinYi-XY1100-R14 in the local PC according to the documentation requirements of XY1100-R14


## Compile BoAT-X Framework Static library

### 1. Compile BoAT-X Framework static library (under Windows)

   #### a. Configure the target platform in directory BoAT-X-Framework/Makefile
   ```
   PLATFORM_TARGET ?= XinYi-XY1100-R14
   ```

   #### b. Open a Cygwin shell, enter BoAT-X-Framework directory and compile BoAT static library
   ```
   cd <XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/BoAT-X-Framework
   make clean
   make all
   ```

   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/BoAT-X-Framework/lib` directory.


### 2. Build the demo program of XY1100-R14, generate .mimgx file for download

   Demo code for accessing blockchain through BoAT-X Framework is in `XY1100 Root>/TARGETS/xinyiNBSoC/USERAPP/examples/boat_demo/boat_platone_demo.c`

   Open a Windows cmd shell and build the demo:
   ```
   cd <XY1100 Root>
   make clean
   make all
   ```
   After the compilation is successful, the arm.img, ram.bin and flash.bin files will be generated under `<XY1100 Root>/TARGETS/xinyiNBSoC/GCC-ARM/bin`, and the arm.img file will be copied to the baseline, through the Logview provided by the manufacturer The tool merges the baselines into the xinyiNBSoc.mimgx download file.

