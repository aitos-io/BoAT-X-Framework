# BoAT-X Framework for China Mobile ML302 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into ML302 OpenCPU SDK, compile BoAT-X Framework static library and build the demo program under Windows environment.


## Copy files

Assuming `<ML302 Root>` to be the root directory of ML302 OpenCPU SDK:

1. Copy the entire BoAT-X-Framework directory into `<ML302 Root>`, on the same level where ML302 SDK's top Makefile locates.

2. Copy `<ML302 Root>\BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\boatdemo.c` into `<ML302 Root>\src`.

3. Copy `<ML302 Root>\BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\my_contract.cpp.abi.c` into `<ML302 Root>\src`.

4. Copy `<ML302 Root>\BoAT-X-Framework\vendor\platform\ChinaMobile-ML302\ML302RootDirCode\src\my_contract.cpp.abi.h` into `<ML302 Root>\src`.


After copying these files, the directory structure should look like:

```
<ML302 Root>
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
\---Makefile    // ML302 Top Makefile
```


## Top Makefile Modification

### 1. Configure BoAT-X Framework's base directory

  In the top Makefile, add this piece of code under line `BINMAKER := python tools/private/elf2flash.py` as below:
  ```
  BOAT_BASE := ./BoAT-X-Framework
  ```

### 2. Add BoAT-X Framework libraries path

  In the top Makefile, add one line under `LDFLAGS += -T prebuilt/components/appstart/flashrun.ld.obj -Wl,-Map=firmware/$(strip $(HWVER))/8910DM_ML302.map` as below:
  ```
  LDFLAGS += $(BOAT_BASE)/lib/libboatwallet.a $(BOAT_BASE)/lib/libboatvendor.a
  ```

### 3. Add search path to the BoAT-X Framework header files

  In the top Makefile, add one line of code under `INC :=  -I'inc\os' ` as below:
  ```
  INC += -I$(BOAT_BASE)\include -I$(BOAT_BASE)\include\protocolapi
  ```


## Compile BoAT-X Framework Static library

### 1. Compile BoAT-X Framework static library (under Cygwin)
   
   #### a. Configure the target platform in directory BoAT-X-Framework\Makefile
   ```
   PLATFORM_TARGET ?= ChinaMobile-ML302
   ```
   
   #### b. Open a Cygwin shell, enter `<ML302 Root>/BoAT-X-Framework` directory and compile BoAT static library
   ```
   cd <ML302 Root>/BoAT-X-Framework
   make clean
   make all
   ```
   
   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<ML302 Root>\BoAT-X-Framework\lib` directory.
   

### 2. Build demo program，generate .pac file for download

   Demo code for accessing blockchain through BoAT-X Framework is in `<ML302 Root>\src\boatdemo.c`
   
   Open a Windows cmd window and build the demo:
   ```
   cd  <ML302 Root>
   build.bat CLEAN
   build.bat DNLM
   ```
   
   The download image `<ML302 Root>\firmware\DNLMML302_OpenCPU.pac` will be will be generated if building is successful.
   
