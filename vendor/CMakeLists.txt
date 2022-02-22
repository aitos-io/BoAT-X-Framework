
set(MODULE_NAME boatvendor)


add_subdirectory(common/storage)
if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
add_subdirectory(common/crypto/crypto_default)

elseif(SOFT_CRYPTO STREQUAL "CRYPTO_MBEDTLS")
add_subdirectory(common/crypto/mbedTLS)
endif()

add_subdirectory(common/crypto/keccak)
add_subdirectory(platform/${PLATFORM_TARGET})


if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:storage_obj>
                                    $<TARGET_OBJECTS:keccak_obj>
                                    $<TARGET_OBJECTS:crypto_default_obj>
                                    $<TARGET_OBJECTS:platform_${PLATFORM_TARGET}_obj>)

elseif(SOFT_CRYPTO STREQUAL "CRYPTO_MBEDTLS")
add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:storage_obj>
                                    $<TARGET_OBJECTS:keccak_obj>
                                    $<TARGET_OBJECTS:crypto_obj>
                                    $<TARGET_OBJECTS:platform_${PLATFORM_TARGET}_obj>)
endif()



target_include_directories(${MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common/storage>
)


