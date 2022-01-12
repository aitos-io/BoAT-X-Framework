set(SUB_MODULE_NAME boathwbcs-obj)

add_subdirectory(protos)

set(FILE_SRC    boathwbcs.c)

add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})

target_link_libraries(${SUB_MODULE_NAME}  protos-hwbcs-obj)

add_dependencies(${SUB_MODULE_NAME} protos-hwbcs-obj)

target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    ${PROJECT_SOURCE_DIR}/sdk/protocol
    ${PROJECT_SOURCE_DIR}/sdk/protocol/boathlfabric/protos
    ${PROJECT_SOURCE_DIR}/sdk/third-party/protobuf-c/include
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
