file(GLOB_RECURSE FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(crypto_obj  OBJECT  ${FILE_SRC})

target_include_directories(crypto_obj
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)