set(SUB_MODULE_NAME     nghttps2-obj)


add_definitions(-DHAVE_ARPA_INET_H)

file(GLOB_RECURSE SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${SUB_MODULE_NAME}   OBJECT  ${SRC_FILE})
# target_compile_definitions(${SUB_MODULE_NAME}  PRIVATE  HAVE_ARPA_INET_H)
target_include_directories(${SUB_MODULE_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)