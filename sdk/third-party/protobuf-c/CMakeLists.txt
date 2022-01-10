set(SUB_MODULE_NAME     protobuf-obj)

set(SRC_FILE      src/protobuf-c.c)

add_library(${SUB_MODULE_NAME}   OBJECT  ${SRC_FILE})

target_include_directories(${SUB_MODULE_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)