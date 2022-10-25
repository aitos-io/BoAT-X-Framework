

add_subdirectory(cJSON)

if(BOAT_PROTOCOL_USE_HLFABRIC OR BOAT_PROTOCOL_USE_CHAINMAKER OR BOAT_PROTOCOL_USE_HWBCS)
add_subdirectory(protos)
add_subdirectory(nghttp2)
add_subdirectory(protobuf-c)
endif()