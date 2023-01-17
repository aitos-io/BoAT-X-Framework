# BoAT-X Framework for Fibocom L610 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into L610 OpenCPU SDK, compile BoAT-X Framework static library and build the demo program in Linux.


## Copy files

Assuming `<L610 Root>` to be the root directory of L610 OpenCPU SDK:

1. Copy the entire BoAT-X-Framework directory into `<L610 Root>`, on the same level where L610 SDK's top CMakeLists.txt locates.

2. Copy `BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/PlatONE_demo/boatdemo.c` into `<L610 Root>`.

3. Copy `BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/PlatONE_demo/my_contract.cpp.abi.c` into `<L610 Root>`.

4. Copy `BoAT-X-Framework/vendor/platform/Fibocom-L610/L610RootDirCode/PlatONE_demo/my_contract.cpp.abi.h` into `<L610 Root>`.


After copying these files, the directory structure should look like:

```
<L610 Root>
|
+---BoAT-X-Framework
+---cmake
+---components
+---ldscripts
+---out
+---prebuilt
+---tools
\---CMakeList.txt
\---boatdemo.c
\---my_contract.cpp.abi.c
\---my_contract.cpp.abi.h
```


## File Modification

### 1. Add BoAT-X-Framework static libraries .a files onto L610 platform

Open `<L610 Root>/cmake/toolchain-gcc.cmake`.  
Add the following two lines as below:

    set(libboatwallet_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatwallet.a)
    set(libboatvendor_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Framework/lib/libboatvendor.a)


### 2. Add the BoAT-X-Framework header files
Open `<L610 Root>/CMakeLists.txt`.  
Find include_directories(xxx), add the following content in the last new line:

    include_directories(BoAT-X-Frameworkt/include BoAT-X-Frameworkt/include/protocolapi BoAT-X-Framework/vendor/platform/include)


### 3. Add the link to the BoAT-X-Framework libs

Open `<L610 Root>/CMakeList.txt`.  
In curly braces below `if(CONFIG_APPIMG_LOAD_FLASH)`, find `target_link_libraries(XXX ${libc_file_name})` and add `${libboatwallet_file_name} ${libboatvendor_file_name}` before `${libc_file_name}`, such as:

    target_link_libraries(${target} PRIVATE ${libboatwallet_file_name} ${libboatvendor_file_name} ${libc_file_name} ${libm_file_name} 


### 4. Add BoAT-X-Framework demo & sample smart contracts into compile directory
Open `<L610 Root>/CMakeList.txt`.  
In curly braces below `if(CONFIG_APPIMG_LOAD_FLASH)`,find `add_appimg(${target} xxx)` and add `demo.c my_contract.cpp.abi.c` at the end, such as:

    add_appimg(${target} ${flash_ldscript} boatdemo.c my_contract.cpp.abi.c)


## Compile BoAT-X-Framework Static library

### 1. Compile BoAT-X-Framework static library (under Linux)

   #### a. Configure the target platform in directory `<L610 Root>/BoAT-X-Framework/Makefile`
   ```
      PLATFORM_TARGET ?= Fibocom-L610
   ```
   
   #### b. Modify the blockchain options in BoAT-X-Framework/Makefile (take PlatONE as an example)
   ```
   BOAT_PROTOCOL_USE_PLATONE       ?= 1
   ```


   #### c. Open a Linux shell, enter `<L610 Root>/BoAT-X-Framework` directory and compile BoAT static library
   ```
      cd <L610 Root>/BoAT-X-Framework
      make clean
      make all
   ```

   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<L610 Root>/BoAT-X-Framework/lib` directory.


### 2. Build demo program，generate .pac file for download

   Demo code for accessing blockchain through BoAT-X Framework is in `<L610 Root>/boatdemo.c`

   Open a Linux shell and build the demo:

      cd <L610 Root>
      rm -r out//The files generated on the local PC must be deleted first
      . tools/core_launch.sh
      cout
      cmake ../.. -G Ninja
      ninja


   A .pac file will be generated under  `<L610 Root>/out/appimage_debug/hex/` once compiled successfully.

