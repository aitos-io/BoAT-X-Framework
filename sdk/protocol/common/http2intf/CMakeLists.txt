set(SUB_MODULE_NAME http2intf-obj)


set(FILE_SRC        http2intf.c)


add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})
target_link_libraries(${SUB_MODULE_NAME}  protos-obj nghttps2-obj)

add_dependencies(${SUB_MODULE_NAME} protos-obj nghttps2-obj)
target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    ${PROJECT_SOURCE_DIR}/sdk/protocol
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
