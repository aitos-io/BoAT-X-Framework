
set(MODULE_NAME boatwallet)


add_subdirectory(wallet)
add_subdirectory(third-party)
add_subdirectory(protocol)
add_subdirectory(utilities)
add_subdirectory(rlp)

if(BOAT_PROTOCOL_USE_PLATONE)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatplatone-obj>)
endif()
if(BOAT_PROTOCOL_USE_PLATON)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatplaton-obj>)
endif()
if(BOAT_PROTOCOL_USE_ETHEREUM)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatethereum-obj>)
endif()
if(BOAT_PROTOCOL_USE_FISCOBCOS)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatfiscobcos-obj>)
endif()
if(BOAT_PROTOCOL_USE_HLFABRIC)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boathlfabric-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:http2intf-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:nghttps2-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protos-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protobuf-obj>)
endif()
if(BOAT_PROTOCOL_USE_HWBCS)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boathwbcs-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protos-hwbcs-obj>)
endif()

add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:wallet_obj>
                                    $<TARGET_OBJECTS:protocol_obj>
                                    $<TARGET_OBJECTS:utilities-obj>
                                    $<TARGET_OBJECTS:web3intf-obj>
                                    $<TARGET_OBJECTS:cjson-obj>
                                    $<TARGET_OBJECTS:boatrlp-obj>
                                    ${TARGET})








target_include_directories(${MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    PUBLIC
    $<BUILD_INTERFACE:${KECCAK_FOLDER}>
)


