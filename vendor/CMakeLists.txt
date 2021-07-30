
set(MODULE_NAME boatvendor)


add_subdirectory(common/storage)
add_subdirectory(common/crypto/mbedTLS)
add_subdirectory(common/crypto/keccak)

if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:storage_obj>)

elseif(SOFT_CRYPTO STREQUAL "CRYPTO_MBEDTLS")
add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:storage_obj> $<TARGET_OBJECTS:keccak_obj> $<TARGET_OBJECTS:mbedtls_obj>)
endif()

# target_link_libraries(${MODULE_NAME}
#     ConfigManager
#     icd_convert
#     controller_service
#     CloudService
#     utils)





target_include_directories(${MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    PUBLIC
    $<BUILD_INTERFACE:${KECCAK_FOLDER}>
)


