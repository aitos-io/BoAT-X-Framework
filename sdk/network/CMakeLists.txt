

if(BOAT_PROTOCOL_USE_ETHEREUM)
add_subdirectory(ethereum)
endif()

if(BOAT_PROTOCOL_USE_HLFABRIC)
add_subdirectory(hlfabric)
endif()

if(BOAT_PROTOCOL_USE_PLATONE)
add_subdirectory(platone)
endif()
