
set(MODULE_NAME boatwallet)


add_subdirectory(wallet)
add_subdirectory(third-party)
add_subdirectory(protocol)

add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:wallet_obj>
                                    $<TARGET_OBJECTS:protocol_obj>)



target_include_directories(${MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    PUBLIC
    $<BUILD_INTERFACE:${KECCAK_FOLDER}>
)


