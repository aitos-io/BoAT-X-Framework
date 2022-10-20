

if(BOAT_PROTOCOL_USE_ETHEREUM)
add_subdirectory(ethereum)
endif()

if(BOAT_PROTOCOL_USE_HLFABRIC)
add_subdirectory(hlfabric)
endif()

if(BOAT_PROTOCOL_USE_PLATONE)
add_subdirectory(platone)
endif()

if(BOAT_PROTOCOL_USE_PLATON)
add_subdirectory(platon)
endif()

if(BOAT_PROTOCOL_USE_FISCOBCOS)
add_subdirectory(fiscobcos)
endif()
