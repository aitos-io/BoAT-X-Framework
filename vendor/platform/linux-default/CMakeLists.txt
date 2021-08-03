




if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
file(GLOB_RECURSE CRYPTO_FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/src/port_crypto_default/*.c)
elseif(SOFT_CRYPTO STREQUAL "CRYPTO_MBEDTLS")
file(GLOB_RECURSE CRYPTO_FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/src/port_mbedtls/*.c)
endif()

file(GLOB_RECURSE RPC_FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/src/rpc/*.c)

add_library(platform_obj  OBJECT  ${RPC_FILE_SRC} ${CRYPTO_FILE_SRC})

target_link_libraries(platform_obj crypto_obj keccak_obj)

target_include_directories(platform_obj
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
