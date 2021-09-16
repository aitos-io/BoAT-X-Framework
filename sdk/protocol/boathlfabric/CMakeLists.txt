set(SUB_MODULE_NAME boathlfabric-obj)

add_subdirectory(protos)

set(FILE_SRC    boathlfabric.c)

if(BOAT_DISCOVERY_PEER_QUERY)
set(FILE_SRC    ${FILE_SRC} boathlfabric_discovery.c)
endif()


add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})

target_link_libraries(${SUB_MODULE_NAME}  protos-obj)

add_dependencies(${SUB_MODULE_NAME} protos-obj)
target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    ${PROJECT_SOURCE_DIR}/sdk/protocol
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
