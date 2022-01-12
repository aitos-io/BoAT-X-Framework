set(SUB_MODULE_NAME         wallet_obj)

set(FILE_SRC    boatwallet.c
                boatversion.c)

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


add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})


target_link_libraries(${SUB_MODULE_NAME}  boatvendor cjson-obj  protocol_obj protos-obj)

target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    ${PROJECT_SOURCE_DIR}/sdk/protocol
    ${PROJECT_SOURCE_DIR}/sdk/protocol/boathwbcs/protos
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


