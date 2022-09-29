set(SUB_MODULE_NAME         network_eth_obj)

set(FILE_SRC    network_ethereum.c)

add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})


target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


