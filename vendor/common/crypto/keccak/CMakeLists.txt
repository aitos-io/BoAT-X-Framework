file(GLOB_RECURSE FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_library(keccak_obj  OBJECT  ${FILE_SRC})

target_include_directories(keccak_obj
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)