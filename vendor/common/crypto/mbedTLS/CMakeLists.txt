file(GLOB_RECURSE FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(mbedtls_obj  OBJECT  ${FILE_SRC})

target_include_directories(mbedtls_obj
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)