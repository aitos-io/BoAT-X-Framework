add_subdirectory(common)

if(BOAT_PROTOCOL_USE_ETHEREUM)
add_subdirectory(boatethereum)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatethereum-obj>)
endif()

if(BOAT_PROTOCOL_USE_PLATONE)
add_subdirectory(boatplatone)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatplatone-obj>)
endif()

if(BOAT_PROTOCOL_USE_PLATON)
add_subdirectory(boatplaton)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatplaton-obj>)
endif()

if(BOAT_PROTOCOL_USE_FISCOBCOS)
add_subdirectory(boatfiscobcos)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatfiscobcos-obj>)
endif()

if(BOAT_PROTOCOL_USE_HLFABRIC)
add_subdirectory(boathlfabric)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boathlfabric-obj>)
endif()

if(BOAT_PROTOCOL_USE_HWBCS)
add_subdirectory(boathwbcs)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boathwbcs-obj>)
endif()

if(BOAT_PROTOCOL_USE_VENACHAIN)
add_subdirectory(boatvenachain)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatvenachain-obj>)
endif()

if(BOAT_PROTOCOL_USE_CHAINMAKER_V1)
add_subdirectory(boatchainmaker_v1)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatchainmaker-obj>)
endif()

if(BOAT_PROTOCOL_USE_CHAINMAKER_V2)
add_subdirectory(boatchainmaker_v2)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatchainmaker-obj>)
endif()

if(BOAT_PROTOCOL_USE_QUORUM)
add_subdirectory(boatquorum)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatquorum-obj>)
endif()

if(BOAT_PROTOCOL_USE_CITA)
add_subdirectory(boatcita)
set(PROTOS  ${PROTOS} $<TARGET_OBJECTS:boatcita-obj>)
endif()

add_library(protocol_obj    OBJECT  ${PROTOS})

# target_link_libraries(protocol_obj  ${PROTOS})
