

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

if(BOAT_PROTOCOL_USE_QUORUM)
add_subdirectory(quorum)
endif()

if(BOAT_PROTOCOL_USE_HWBCS)
add_subdirectory(hwbcs)
endif()

if(BOAT_PROTOCOL_USE_VENACHAIN)
add_subdirectory(venachain)
endif()

if(BOAT_PROTOCOL_USE_CHAINMAKER_V1 OR BOAT_PROTOCOL_USE_CHAINMAKER_V2)
add_subdirectory(chainmaker)
endif()

if(BOAT_PROTOCOL_CITA)
add_subdirectory(cita)
endif()
