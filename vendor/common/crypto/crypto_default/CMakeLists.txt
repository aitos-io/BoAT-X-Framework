file(GLOB_RECURSE FILE_SRC      ${CMAKE_CURRENT_SOURCE_DIR}/aes/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_definitions(
    -DRAND_PLATFORM_INDEPENDENT
)
add_library(crypto_default_obj  OBJECT  ${FILE_SRC})
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ed25519-donna)
target_include_directories(crypto_default_obj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)