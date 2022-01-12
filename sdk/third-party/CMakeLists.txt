

add_subdirectory(cJSON)

if(BOAT_PROTOCOL_USE_HLFABRIC)
add_subdirectory(nghttp2)
add_subdirectory(protobuf-c)
endif()