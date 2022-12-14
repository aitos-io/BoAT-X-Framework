
cmake_minimum_required (VERSION 3.10)

project (BoAT_3.0 C)

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
option(BOAT_DISCOVERY_PEER_QUERY "Enable node discover "                           OFF)
option(BOAT_PROTOCOL_USE_CHAINMAKER_V1 "Enable compile the chainmaker v1 blockchain protocol" ON)
option(BOAT_PROTOCOL_USE_CHAINMAKER_V2 "Enable compile the chainmaker v2 blockchain protocol" OFF)
option(BOAT_PROTOCOL_USE_VENACHAIN "Enable compile the vena blockchain protocol" ON)
option(BOAT_PROTOCOL_USE_QUORUM "Enable compile the quorum blockchain protocol" ON)




option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_BOAT_TEST "Build boat test application" OFF)
option(BUILD_BOAT_DEMO "Build boat demo application" ON)





if( NOT (BOAT_PROTOCOL_USE_ETHEREUM OR BOAT_PROTOCOL_USE_PLATONE OR BOAT_PROTOCOL_USE_PLATON OR BOAT_PROTOCOL_USE_FISCOBCOS OR BOAT_PROTOCOL_USE_HLFABRIC OR BOAT_PROTOCOL_USE_HWBCS OR BOAT_PROTOCOL_USE_CHAINMAKER OR BOAT_PROTOCOL_USE_VENACHAIN OR BOAT_PROTOCOL_USE_QUORUM))
    message(FATAL_ERROR "Select at least one chain")
endif()
if(BOAT_PROTOCOL_USE_CHAINMAKER_V1 AND BOAT_PROTOCOL_USE_CHAINMAKER_V2)
    message(FATAL_ERROR "chaimaker v1 and v2 could not both be selected at the sametime")
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
# set(SOFT_CRYPTO "CRYPTO_MBEDTLS")

########################################
#add_subdirectory( ${CMAKE_SOURCE_DIR}/vendor/platform/${PLATFORM_TARGET}/externalCmake)

message(STATUS "Platform using : " ${PLATFORM_TARGET})
if( PLATFORM_TARGET STREQUAL "linux-default")
    message(STATUS "Platform using linux")
    set(SOFT_CRYPTO "CRYPTO_DEFAULT")
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

if(BOAT_PROTOCOL_USE_VENACHAIN)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_VENACHAIN=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_VENACHAIN=0)
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

if(BOAT_PROTOCOL_USE_CHAINMAKER_V1)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_CHAINMAKER_V1=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_CHAINMAKER_V1=0)
endif()

if(BOAT_PROTOCOL_USE_CHAINMAKER_V2)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_CHAINMAKER_V2=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_CHAINMAKER_V2=0)
endif()

if(BOAT_PROTOCOL_USE_QUORUM)
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_QUORUM=1)
else()
    set(COMAAND_SCRIPTS_PARAM ${COMAAND_SCRIPTS_PARAM} BOAT_PROTOCOL_USE_QUORUM=0)
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
include_directories(${PROJECT_SOURCE_DIR}/vendor/common/storage)


if(BOAT_PROTOCOL_USE_ETHEREUM OR BOAT_PROTOCOL_USE_PLATONE OR BOAT_PROTOCOL_USE_PLATON OR BOAT_PROTOCOL_USE_FISCOBCOS OR BOAT_PROTOCOL_USE_VENACHAIN OR BOAT_PROTOCOL_USE_QUORUM)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/web3intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/cJSON)
endif()

if(BOAT_PROTOCOL_USE_ETHEREUM)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatethereum)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/ethereum)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_ethereum/demo_contract)
endif()
if(BOAT_PROTOCOL_USE_PLATONE)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatplatone)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/platone)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_platone/demo_contract/Solidity)
endif()
if(BOAT_PROTOCOL_USE_PLATON)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatplaton)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/platon)
endif()
if(BOAT_PROTOCOL_USE_FISCOBCOS)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatfiscobcos)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/fiscobcos)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_fiscobcos/demo_contract)
endif()
if(BOAT_PROTOCOL_USE_VENACHAIN)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatvenachain)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/venachain)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_venachain/demo_contract/Solidity)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_venachain/demo_contract/WASM)
endif()
if(BOAT_PROTOCOL_USE_HLFABRIC)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/hlfabric)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/nghttp2/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/http2intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boathlfabric)
endif()
if(BOAT_PROTOCOL_USE_HWBCS)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/hwbcs)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/nghttp2/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/http2intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boathwbcs)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boathwbcs/protos)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/protos)
endif()
if(BOAT_PROTOCOL_USE_CHAINMAKER_V1)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/chainmaker)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/nghttp2/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/http2intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatchainmaker_v1/boatchainmaker)
endif()
if(BOAT_PROTOCOL_USE_CHAINMAKER_V2)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/chainmaker)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/nghttp2/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/http2intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatchainmaker_v2/boatchainmaker)
endif()
if(BOAT_PROTOCOL_USE_QUORUM)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/boatquorum)
include_directories(${PROJECT_SOURCE_DIR}/sdk/network/quorum)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_quorum/demo_contract)
endif()
if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
include_directories(${PROJECT_SOURCE_DIR}/vendor/common/crypto/crypto_default)
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


