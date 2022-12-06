
if(BOAT_PROTOCOL_USE_ETHEREUM)
add_subdirectory(demo_ethereum)
endif()

if(BOAT_PROTOCOL_USE_PLATONE)
add_subdirectory(demo_platone)
endif()

if(BOAT_PROTOCOL_USE_PLATON)
add_subdirectory(demo_platon)
endif()

if(BOAT_PROTOCOL_USE_FISCOBCOS)
add_subdirectory(demo_fiscobcos)
endif()

if(BOAT_PROTOCOL_USE_HLFABRIC)
add_subdirectory(demo_fabric)
endif()

if(BOAT_PROTOCOL_USE_HWBCS)
add_subdirectory(demo_hw_bcs)
endif()

if(BOAT_PROTOCOL_USE_VENACHAIN)
add_subdirectory(demo_venachain)
endif()

if(BOAT_PROTOCOL_USE_CHAINMAKER)
add_subdirectory(demo_chainmaker)
endif()

if(BOAT_PROTOCOL_USE_QUORUM)
add_subdirectory(demo_quorum)
endif()

if(BOAT_PROTOCOL_USE_CITA)
add_subdirectory(demo_cita)
endif()