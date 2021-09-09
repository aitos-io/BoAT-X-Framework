file(GLOB_RECURSE FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/*.c  ${CMAKE_CURRENT_SOURCE_DIR}/aes/*.c)
add_library(crypto_default_obj  OBJECT  ${FILE_SRC})

target_include_directories(crypto_default_obj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ed25519-donna
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)