set(SUB_MODULE_NAME         wallet_obj)

set(FILE_SRC    boatversion.c)

if(BOAT_PROTOCOL_USE_ETHEREUM)
set(FILE_SRC    ${FILE_SRC} api_ethereum.c)
endif()

if(BOAT_PROTOCOL_USE_PLATONE)
set(FILE_SRC    ${FILE_SRC} api_platone.c)
endif()

if(BOAT_PROTOCOL_USE_PLATON)
set(FILE_SRC    ${FILE_SRC} api_platon.c)
endif()

if(BOAT_PROTOCOL_USE_FISCOBCOS)
set(FILE_SRC    ${FILE_SRC} api_fiscobcos.c)
endif()


if(BOAT_PROTOCOL_USE_HLFABRIC)
set(FILE_SRC    ${FILE_SRC} api_hlfabric.c)
endif()

if(BOAT_PROTOCOL_USE_HWBCS)
set(FILE_SRC    ${FILE_SRC} api_hw_bcs.c)
endif()

if(BOAT_PROTOCOL_USE_VENACHAIN)
set(FILE_SRC    ${FILE_SRC} api_venachain.c)
endif()

if(BOAT_PROTOCOL_USE_CHAINMAKER)
set(FILE_SRC    ${FILE_SRC} api_chainmaker.c)
endif()

if(BOAT_PROTOCOL_USE_QUORUM)
set(FILE_SRC    ${FILE_SRC} api_quorum.c)
endif()

add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})


# target_link_libraries(${SUB_MODULE_NAME}  boatvendor cjson-obj  protocol_obj protos-obj)

target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    ${PROJECT_SOURCE_DIR}/sdk/protocol
    if(BOAT_PROTOCOL_USE_HWBCS)
    ${PROJECT_SOURCE_DIR}/sdk/protocol/boathwbcs/protos
    endif()
    if(BOAT_PROTOCOL_USE_CHAINMAKER)
    ${PROJECT_SOURCE_DIR}/sdk/third-party/protobuf-c/include
    ${PROJECT_SOURCE_DIR}/sdk/protocol/boatchainmaker/protos
    ${PROJECT_SOURCE_DIR}/sdk/protocol/boatchainmaker
    endif()
    if(BOAT_PROTOCOL_USE_HLFABRIC)
    ${PROJECT_SOURCE_DIR}/sdk/third-party/protos
    endif()
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


