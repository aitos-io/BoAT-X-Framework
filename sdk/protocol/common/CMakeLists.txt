

if(BOAT_PROTOCOL_USE_HLFABRIC OR BOAT_PROTOCOL_USE_CHAINMAKER)
add_subdirectory(http2intf)
endif()

add_subdirectory(web3intf)