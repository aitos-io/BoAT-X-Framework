# BoAT-X Framework for China Unicom YanFei-CUIot-MZ-6 Integration Guideline


## About This Guideline

This guide describes how to integrate BoAT-X Framework source code into YanFei OpenCPU SDK, compile BoAT-X Framework static library and build the demo program under Windows environment.


## Copy files

Assuming `<YanFei Root>` to be the root directory of YanFei OpenCPU SDK:

1. Copy the entire BoAT-X-Framework directory into `<YanFei Root>`, on the same level where YanFei SDK's top CMakeLists.txt locates.

2. Copy `BoAT-X-Framework/vendor/platform/YanFei-CUIot-MZ-6/YanfeiRootDirCode/app/demo/src` into `<YanFei Root>/app/demo/src/`.


After copying these files, the directory structure should look like:

```
<YanFei Root>
|
+---app
+---BoAT-X-Framework
+---cmake
+---components
+---examples
+---ldscripts
+---littlevgl
+---out
+---prebuilt
+---tools
\---CMakeList.txt
```


## File Modification

### 1. Add BoAT-X Framework libraries path

Open `<YanFei Root>/cmake/toolchain-gcc.cmake` 
Add the following two lines of code as below:
```
set(libbw_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Frameworkt/lib/libboatwallet.a)
set(libbv_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-X-Frameworkt/lib/libboatvendor.a)
```


### 2. Add BoAT-X-Framework lib files to "target_link_libraries"

Open `<YanFei Root>/app/demo/CMakeList.txt` 
Find target_link_libraries(XXX ${libc_file_name}) and add ${libbw_file_name} ${libbv_file_name} before ${libc_file_name}, such as:

  ```
  target_link_libraries(${target} PRIVATE ${libbw_file_name} ${libbv_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
  ```


### 3. Add the BoAT-X Framework header files
Open `<YanFei Root>/CMakeLists.txt` 
Find include_directories(xxx), add the following content in the last new line:
  ```
  include_directories(BoAT-X-Frameworkt/include BoAT-X-Frameworkt/include/protocolapi)
  ```

### 4. Add demo and smart contract files of BoAT-X-Framework
Open `<YanFei Root>/app/demo/CMakeList.txt` 
Find add_appimg(${target} xxx) and add ${target_file_path}/my_contract.c at the end, such as:
```
add_appimg(${target} ${flash_ldscript} ${target_file_path}/${target_file}.c ${target_file_path}/my_contract.c ${OEM})
```

## Compile BoAT-X Framework Static library

### 1. Compile BoAT-X Framework static library (under Linux)

   #### a. Configure the target platform in directory BoAT-X-Framework/Makefile
   ```
   PLATFORM_TARGET ?= YanFei-CUIot-MZ-6
   ```

   #### b. Open a Linux shell, enter `<YanFei Root>/BoAT-X-Framework` directory and compile BoAT static library
   ```
   cd <YanFei Root>/BoAT-X-Framework
   make clean
   make all
   ```

   After compiling, static library `libboatvendor.a` and `libboatwallet.a` will be created in `<YanFei Root>/BoAT-X-Framework/lib` directory.


### 2. Build demo program，generate .pac file for download

   Demo code for accessing blockchain through BoAT-X Framework is in `<YanFei Root>/app/demo/src/main.c`

   Open a Linux shell and build the demo:
   ```
   cd <YanFei Root>
   rm -r out
   . tools/core_launch.sh
   cout
   cmake ../.. -G Ninja
   ninja
   ```

   The download image `<YanFei Root>/out/appimage_debug/hex/main_flash.pac` will be will be generated if building is successful.

