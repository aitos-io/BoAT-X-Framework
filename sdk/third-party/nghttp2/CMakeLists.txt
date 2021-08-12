set(SUB_MODULE_NAME     nghttps2-obj)

file(GLOB_RECURSE SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${SUB_MODULE_NAME}   OBJECT  ${SRC_FILE})

target_include_directories(${SUB_MODULE_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)