
set(MODULE_NAME boatwallet)


add_subdirectory(wallet)
add_subdirectory(third-party)
add_subdirectory(protocol)
add_subdirectory(utilities)

add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:wallet_obj>
                                    $<TARGET_OBJECTS:protocol_obj>
                                    $<TARGET_OBJECTS:utilities-obj>
                                    $<TARGET_OBJECTS:http2intf-obj>
                                    $<TARGET_OBJECTS:nghttps2-obj>
                                    $<TARGET_OBJECTS:boathlfabric-obj>
                                    $<TARGET_OBJECTS:protos-obj>
                                    $<TARGET_OBJECTS:protobuf-obj>)





target_include_directories(${MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    PUBLIC
    $<BUILD_INTERFACE:${KECCAK_FOLDER}>
)


