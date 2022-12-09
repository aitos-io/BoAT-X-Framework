
set(MODULE_NAME boatwallet)


add_subdirectory(keypair)
add_subdirectory(network)
add_subdirectory(wallet)
add_subdirectory(third-party)
add_subdirectory(protocol)
add_subdirectory(utilities)

if (BOAT_PROTOCOL_USE_ETHEREUM OR BOAT_PROTOCOL_USE_PLATONE OR BOAT_PROTOCOL_USE_PLATON OR BOAT_PROTOCOL_USE_FISCOBCOS OR BOAT_PROTOCOL_USE_QUORUM OR BOAT_PROTOCOL_USE_VENACHAIN OR BOAT_PROTOCOL_USE_CITA)
add_subdirectory(rlp)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatrlp-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:web3intf-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:cjson-obj>)
endif()

if(BOAT_PROTOCOL_USE_VENACHAIN)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatvenachain-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_venachain_obj>)
endif()
if(BOAT_PROTOCOL_USE_PLATONE)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatplatone-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_platone_obj>)
endif()
if(BOAT_PROTOCOL_USE_PLATON)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatplaton-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_platon_obj>)
endif()
if(BOAT_PROTOCOL_USE_ETHEREUM)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatethereum-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_eth_obj>)
endif()
if(BOAT_PROTOCOL_USE_FISCOBCOS)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatfiscobcos-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_fiscobcos_obj>)
endif()
if(BOAT_PROTOCOL_USE_HLFABRIC)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boathlfabric-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:http2intf-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:nghttps2-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protos-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protobuf-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_hlfabric_obj>)
endif()
if(BOAT_PROTOCOL_USE_HWBCS)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boathwbcs-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:http2intf-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:nghttps2-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protos-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protobuf-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protos-hwbcs-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_hwbcs_obj>)
endif()
if(BOAT_PROTOCOL_USE_CHAINMAKER)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatchainmaker-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:http2intf-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:nghttps2-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protos-chainmaker-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:protobuf-obj>)
endif()
if(BOAT_PROTOCOL_USE_QUORUM)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatquorum-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_quorum_obj>)
endif()

if(BOAT_PROTOCOL_USE_CITA)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:boatcita-obj>)
set(TARGET  ${TARGET} $<TARGET_OBJECTS:network_cita_obj>)
endif()


add_library(${MODULE_NAME}  STATIC  $<TARGET_OBJECTS:keypair_obj>
                                    $<TARGET_OBJECTS:wallet_obj>
                                    $<TARGET_OBJECTS:protocol_obj>
                                    $<TARGET_OBJECTS:utilities-obj>
                                    ${TARGET})








target_include_directories(${MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    PUBLIC
    $<BUILD_INTERFACE:${KECCAK_FOLDER}>
)


