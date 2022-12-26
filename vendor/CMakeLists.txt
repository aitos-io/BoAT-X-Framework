
set(MODULE_NAME boatvendor)

add_subdirectory(keystore)
add_subdirectory(common/storage)
if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
add_subdirectory(common/crypto/crypto_default)
endif()


add_subdirectory(platform/${PLATFORM_TARGET})

# if()
set(TARGET  ${TARGET} $<TARGET_OBJECTS:keystore_soft_obj>)
# endif()

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIRS})
  message(STATUS "OpenSSL Found!")
endif()



# if(SOFT_CRYPTO STREQUAL "CRYPTO_DEFAULT")
add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:storage_obj>
                                    $<TARGET_OBJECTS:crypto_default_obj>
                                    $<TARGET_OBJECTS:platform_${PLATFORM_TARGET}_obj>
                                    ${TARGET})

target_link_libraries(${MODULE_NAME} OpenSSL::Crypto OpenSSL::SSL)


target_include_directories(${MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common/storage>
)


