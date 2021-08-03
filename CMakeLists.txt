

cmake_minimum_required(VERSION 3.2...3.16 FATAL_ERROR)


project(CURL C)


string(REGEX MATCH "GNU" CMAKE_COMPILER_IS_GNU "${CMAKE_C_COMPILER_ID}")



option(BOAT_PROTOCOL_USE_ETHEREUM "Enable compile the ethereum blockchain protocol" OFF)
option(BOAT_PROTOCOL_USE_PLATONE "Enable compile the platone blockchain protocol" OFF)
option(BOAT_PROTOCOL_USE_FISCOBCOS "Enable compile the fiscobcos blockchain protocol" OFF)
option(BOAT_PROTOCOL_USE_HLFABRIC "Enable compile the hlfabric blockchain protocol" ON)


option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_BOAT_TEST "Build boat test application" OFF)
option(BUILD_BOAT_DEMO "Build boat demo application" ON)


if(NOT (BOAT_PROTOCOL_USE_ETHEREUM OR BOAT_PROTOCOL_USE_PLATONE OR BOAT_PROTOCOL_USE_FISCOBCOS OR BOAT_PROTOCOL_USE_HLFABRIC))
    message(FATAL_ERROR "Select at least one chain")
endif()

if(BOAT_PROTOCOL_USE_FISCOBCOS AND NOT BOAT_PROTOCOL_USE_ETHEREUM)
    message(WARNING "FISCOBCOS depends on ETHEREUM, BOAT_PROTOCOL_USE_FISCOBCOS is ON, but BOAT_PROTOCOL_USE_ETHEREUM is OFF")
    message(STATUS "set BOAT_PROTOCOL_USE_ETHEREUM ON")
    set(BOAT_PROTOCOL_USE_ETHEREUM ON)
endif()


if(BOAT_PROTOCOL_USE_PLATONE AND NOT BOAT_PROTOCOL_USE_ETHEREUM)
    message(WARNING "FISCOBCOS depends on ETHEREUM, BOAT_PROTOCOL_USE_PLATONE is ON, but BOAT_PROTOCOL_USE_ETHEREUM is OFF")
    message(STATUS "set BOAT_PROTOCOL_USE_ETHEREUM ON")
    set(BOAT_PROTOCOL_USE_ETHEREUM ON)
endif()

# Platform target
# The valid option value of PLATFORM_TARGET list as below:
# - linux-default             : Default linux platform
# - Fibocom-L610              : Fibocom's LTE Cat.1 module
# - Fibocom-L718              : Fibocom's LTE Cat.4 module
# - Quectel-BG95              : Quectel's NB-IoT/GSM module
# - Neoway-N58                : Neoway's LTE Cat.1 module
# - YanFei-CUIot-MZ-6         : China Unicom's LTE Cat.1 module
# - ChinaMobile-ML302         : China Mobile's LTE Cat.1 module

set(PLATFORM_TARGET "linux-default" CACHE STRING "Select platform using linux-default")

set(SOFT_CRYPTO "CRYPTO_DEFAULT" CACHE STRING "Set crypto type")

if( PLATFORM_TARGET STREQUAL "linux-default")
    message(STATUS "Platform using linux")
elseif(PLATFORM_TARGET STREQUAL "Fibocom-L610")
    message(STATUS "Platform using Fibocom-L610")
elseif(PLATFORM_TARGET STREQUAL "Fibocom-L718")
    message(STATUS "Platform using Fibocom-L718")
elseif(PLATFORM_TARGET STREQUAL "Quectel-BG95")
    message(STATUS "Platform using Quectel-BG95")
elseif(PLATFORM_TARGET STREQUAL "Neoway-N58")
    message(STATUS "Platform using Neoway-N58")
elseif(PLATFORM_TARGET STREQUAL "YanFei-CUIot-MZ-6")
    message(STATUS "Platform using YanFei-CUIot-MZ-6")
elseif(PLATFORM_TARGET STREQUAL "ChinaMobile-ML302")
    message(STATUS "Platform using ChinaMobile-ML302")
else()
    message(FATAL_ERROR "Platform using ${PLATFORM_TARGET}, but not support")
endif()


# check python3 is installed
if(NOT CMAKE_VERSION VERSION_LESS 3.15.0)
    set(Python3_FIND_STRATEGY LOCATION)
    find_package(Python3 COMPONENTS Interpreter)
    if(Python3_Interpreter_FOUND)
        set(BOAT_PYTHON_EXECUTABLE ${Python3_EXECUTABLE})
    endif()
else()
    find_package(PythonInterp 3)
    if(PYTHONINTERP_FOUND)
        set(BOAT_PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE})
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
endif()



include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/${PLATFORM_TARGET}/src/log)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/http2intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/protocol/common/web3intf)
include_directories(${PROJECT_SOURCE_DIR}/sdk/third-party/nghttp2/include)


if(BUILD_BOAT_DEMO)
add_subdirectory(demo)
endif()

add_subdirectory(vendor)
#add_subdirectory(sdk)