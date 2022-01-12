




if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
file(GLOB_RECURSE CRYPTO_FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/src/port_crypto_default/*.c)
elseif(SOFT_CRYPTO STREQUAL "CRYPTO_MBEDTLS")
file(GLOB_RECURSE CRYPTO_FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/src/port_mbedtls/*.c)
endif()

file(GLOB_RECURSE RPC_FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/src/rpc/*.c)

add_library(platform_${PLATFORM_TARGET}_obj  OBJECT  ${RPC_FILE_SRC} ${CRYPTO_FILE_SRC})

target_link_libraries(platform_${PLATFORM_TARGET}_obj crypto_obj keccak_obj)

target_include_directories(platform_${PLATFORM_TARGET}_obj
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    ${PROJECT_SOURCE_DIR}/vendor/common/crypto_default
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
