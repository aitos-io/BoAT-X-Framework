
cmake_minimum_required (VERSION 3.10)

project (BoAT_2.0 C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/library)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/path)

#string(REGEX MATCH "GNU" CMAKE_COMPILER_IS_GNU "${CMAKE_C_COMPILER_ID}")

option(BOAT_PROTOCOL_USE_ETHEREUM "Enable compile the ethereum blockchain protocol" ON)
option(BOAT_PROTOCOL_USE_PLATONE "Enable compile the platone blockchain protocol" ON)
option(BOAT_PROTOCOL_USE_PLATON "Enable compile the platon blockchain protocol" ON)
option(BOAT_PROTOCOL_USE_FISCOBCOS "Enable compile the fiscobcos blockchain protocol" ON)
option(BOAT_PROTOCOL_USE_HLFABRIC "Enable compile the hlfabric blockchain protocol" ON )
option(BOAT_PROTOCOL_USE_HWBCS "Enable compile the huawei blockchain protocol" ON)
option(BOAT_DISCOVERY_PEER_QUERY "Enable node discover "                           ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_BOAT_TEST "Build boat test application" OFF)
option(BUILD_BOAT_DEMO "Build boat demo application" ON)

# set(BOAT_PROTOCOL_USE_ETHEREUM  ON)
# set(BOAT_PROTOCOL_USE_PLATONE  ON)
# set(BOAT_PROTOCOL_USE_PLATON  ON)
# set(BOAT_PROTOCOL_USE_FISCOBCOS  ON)
# set(BOAT_PROTOCOL_USE_HLFABRIC  ON)
# set(BOAT_PROTOCOL_USE_HLHUAWEI  ON)
# set(BOAT_DISCOVERY_PEER_QUERY  ON)

# set(BUILD_SHARED_LIBS  ON)
# set(BUILD_BOAT_TEST  OFF)
# set(BUILD_BOAT_DEMO  ON)


 message(STATUS "has set BOAT_PROTOCOL_USE_FISCOBCOS " ${BOAT_PROTOCOL_USE_FISCOBCOS})
  message(STATUS "has set BOAT_PROTOCOL_USE_PLATONE " ${BOAT_PROTOCOL_USE_PLATONE})

if( NOT (BOAT_PROTOCOL_USE_ETHEREUM OR BOAT_PROTOCOL_USE_PLATONE OR BOAT_PROTOCOL_USE_PLATON OR BOAT_PROTOCOL_USE_FISCOBCOS OR BOAT_PROTOCOL_USE_HLFABRIC OR BOAT_PROTOCOL_USE_HWBCS))
    message(FATAL_ERROR "Select at least one chain")
endif()

if(BOAT_PROTOCOL_USE_FISCOBCOS AND NOT BOAT_PROTOCOL_USE_ETHEREUM)
    message(WARNING "FISCOBCOS depends on ETHEREUM, BOAT_PROTOCOL_USE_FISCOBCOS is ON, but BOAT_PROTOCOL_USE_ETHEREUM is OFF")
    message(STATUS "set BOAT_PROTOCOL_USE_ETHEREUM ON")
    set(BOAT_PROTOCOL_USE_ETHEREUM ON)
endif()

if(BOAT_PROTOCOL_USE_PLATONE AND NOT BOAT_PROTOCOL_USE_ETHEREUM)
    message(WARNING "PLATONE depends on ETHEREUM, BOAT_PROTOCOL_USE_PLATONE is ON, but BOAT_PROTOCOL_USE_ETHEREUM is OFF")
    message(STATUS "set BOAT_PROTOCOL_USE_ETHEREUM ON")
    set(BOAT_PROTOCOL_USE_ETHEREUM ON)
endif()

if(BOAT_PROTOCOL_USE_PLATON AND NOT BOAT_PROTOCOL_USE_ETHEREUM)
    message(WARNING "PLATON depends on ETHEREUM, BOAT_PROTOCOL_USE_PLATON is ON, but BOAT_PROTOCOL_USE_ETHEREUM is OFF")
    message(STATUS "set BOAT_PROTOCOL_USE_ETHEREUM ON")
    set(BOAT_PROTOCOL_USE_ETHEREUM ON)
endif()

if(BOAT_PROTOCOL_USE_HWBCS AND NOT BOAT_PROTOCOL_USE_HLFABRIC)
    message(WARNING "HWBCS depends on FABRIC, BOAT_PROTOCOL_USE_HWBCS is ON, but BOAT_PROTOCOL_USE_HLFABRIC is OFF")
    message(STATUS "set BOAT_PROTOCOL_USE_HLFABRIC ON")
    set(BOAT_PROTOCOL_USE_HLFABRIC ON)
endif()

# Platform target
# The valid option value of PLATFORM_TARGET list as below:
# - linux-default             : Default linux platform
# - Fibocom-L610              : Fibocom's LTE Cat.1 module
# - Quectel-BG95              : Quectel's NB-IoT/GSM module
# - Neoway-N58                : Neoway's LTE Cat.1 module
# - YanFei-CUIot-MZ-6         : China Unicom's LTE Cat.1 module
# - ChinaMobile-ML302         : China Mobile's LTE Cat.1 module
# - MTK-MT3620                : MTK-MT3620 module

set(PLATFORM_TARGET "linux-default")

# Soft-crypto Dependencies
# The valid option value of SOFT_CRYPTO list as below:
# - CRYPTO_DEFAULT      : default soft crypto algorithm
# - CRYPTO_MBEDTLS      : mbedtls crypto algorithm
set(SOFT_CRYPTO "CRYPTO_MBEDTLS")

########################################
#add_subdirectory( ${CMAKE_SOURCE_DIR}/vendor/platform/${PLATFORM_TARGET}/externalCmake)

message(STATUS "Platform using : " ${PLATFORM_TARGET})
if( PLATFORM_TARGET STREQUAL "linux-default")
    message(STATUS "Platform using linux")
elseif(PLATFORM_TARGET STREQUAL "Fibocom-L610")
    message(STATUS "Platform using Fibocom-L610")
elseif(PLATFORM_TARGET STREQUAL "Quectel-BG95")
    message(STATUS "Platform using Quectel-BG95")
elseif(PLATFORM_TARGET STREQUAL "Neoway-N58")
    message(STATUS "Platform using Neoway-N58")
elseif(PLATFORM_TARGET STREQUAL "YanFei-CUIot-MZ-6")
    message(STATUS "Platform using YanFei-CUIot-MZ-6")
elseif(PLATFORM_TARGET STREQUAL "ChinaMobile-ML302")
    message(STATUS "Platform using ChinaMobile-ML302")
elseif(PLATFORM_TARGET STREQUAL "MTK-MT3620")
    message(STATUS "Platform using MTK-MT3620")
else()
    message(FATAL_ERROR "Platform using ${PLATFORM_TARGET}, but not support")
endif()

# check python3 is installed
if(NOT CMAKE_VERSION VERSION_LESS 3.15.0)
    set(Python3_FIND_STRATEGY LOCATION)
    find_package(Python3 COMPONENTS Interpreter)
    if(Python3_Interpreter_FOUND)
        set(BOAT_PYTHON_EXECUTABLE ${Python3_EXECUTABLE})
         message(STATUS "found python !!!!!!")
    endif()
else()
    find_package(PythonInterp 3)
    if(PYTHONINTERP_FOUND)
        set(BOAT_PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE})
        message(STATUS "found python !!!!!!")
    endif()
endif()


if(BOAT_PROTOCOL_USE_ETHEREUM)
    set(COMAAND_SCRIPTS_PARAM  ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_ETHEREUM=1)
else()
    set(COMAAND_SCRIPTS_PARAM  ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_ETHEREUM=0)
endif()

if(BOAT_PROTOCOL_USE_PLATONE)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_PLATONE=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_PLATONE=0)
endif()

if(BOAT_PROTOCOL_USE_PLATON)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_PLATON=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_PLATON=0)
endif()

if(BOAT_PROTOCOL_USE_FISCOBCOS)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_FISCOBCOS=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_FISCOBCOS=0)
endif()

if(BOAT_PROTOCOL_USE_HLFABRIC)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_HLFABRIC=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_HLFABRIC=0)
endif()

if(BOAT_PROTOCOL_USE_HWBCS)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_HWBCS=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_HWBCS=0)
endif()

if(BOAT_DISCOVERY_PEER_QUERY)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_DISCOVERY_PEER_QUERY=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_DISCOVERY_PEER_QUERY=0)
endif()

message(STATUS "set BOAT_PROTOCOL_USE_FISCOBCOS " ${BOAT_PROTOCOL_USE_FISCOBCOS})

if(BOAT_PYTHON_EXECUTABLE)
    execute_process(COMMAND ${BOAT_PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/platform/${PLATFORM_TARGET}/scripts/gen.py ${PLATFORM_TARGET} ${COMAAND_SCRIPTS_PARAM}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE result)
    if(${result} EQUAL 1)
        message(FATAL_ERROR "run python gen.py boatconfig.h error")
    endif()
else()
    message(FATAL_ERROR "not fund python3, please install python3")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Os -Wall")

if(CMAKE_COMPILER_IS_GNU)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-gc-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-builtin-macro-redefined -U__FILE__ -D__FILE__='\"$(subst $(dir $<),,$<)\"'")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/${PLATFORM_TARGET}/src/log)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/http2intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/web3intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/cJSON)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/nghttp2/include)
if(BOAT_PROTOCOL_USE_ETHEREUM)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatethereum)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_ethereum/demo_contract)
endif()
if(BOAT_PROTOCOL_USE_PLATONE)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatplatone)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_platone/demo_contract/Solidity)
endif()
if(BOAT_PROTOCOL_USE_PLATON)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatplaton)
endif()
if(BOAT_PROTOCOL_USE_FISCOBCOS)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatfiscobcos)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_fiscobcos/demo_contract)
endif()
if(BOAT_PROTOCOL_USE_HLFABRIC)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boathlfabric)
endif()
if(BOAT_PROTOCOL_USE_HWBCS)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boathwbcs)
endif()
if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
include_directories(${PROJECT_SOURCE_DIR}/vendor/common/crypto/crypto_default)
endif()
if(SOFT_CRYPTO STREQUAL "CRYPTO_MBEDTLS")
include_directories(${PROJECT_SOURCE_DIR}/vendor/common/crypto/mbedTLS/include)
endif()

#add compile option
#add_compile_options(-std=c99 -Wall -Os)
#add link option
#add_compile_options(-Wl,-Map)

if(BUILD_BOAT_DEMO)
add_subdirectory(demo)
endif()

add_subdirectory(vendor)
add_subdirectory(sdk)

#Create executable
#-create demo_ethereum_storeread
# if (BOAT_PROTOCOL_USE_ETHEREUM)
# add_executable (demo_ethereum_storeread demo/demo_ethereum_storeread.c)
# target_link_libraries (demo_ethereum_storeread boatwallet boathwdep boatcontract curl)
# endif()
# #-create demo_ethereum_transfer
# add_executable (demo_ethereum_transfer demo/demo_ethereum_transfer.c)
# target_link_libraries (demo_ethereum_transfer boatwallet boathwdep boatcontract curl)
# #-create demo_fabric_invoke
# add_executable (demo_fabric_invoke demo/demo_fabric_invoke.c)
# target_link_libraries (demo_fabric_invoke boatwallet boathwdep boatcontract curl)
# #-create demo_fiscobcos_helloworld
# add_executable (${PROJECT_NAME} demo/demo_fiscobcos_helloworld.c)
# #-create demo_platone_mycontract
# add_executable (demo_platone_mycontract demo/demo_platone_mycontract.c)
# target_link_libraries (demo_platone_mycontract boatwallet boathwdep boatcontract curl)

