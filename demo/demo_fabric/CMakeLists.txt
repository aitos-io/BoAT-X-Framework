


set(FABRIC_DEMO_SRC         demo_fabric_abac.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_fabric/runtime)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)

add_executable(demo_fabric_abac_onetime ${FABRIC_DEMO_SRC})
target_compile_options(demo_fabric_abac_onetime PRIVATE -DUSE_ONETIME_WALLET)
target_link_libraries(demo_fabric_abac_onetime
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_fabric_abac_persist ${FABRIC_DEMO_SRC})
target_compile_options(demo_fabric_abac_persist PRIVATE -DUSE_CREATE_PERSIST_WALLET)
target_link_libraries(demo_fabric_abac_persist
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_fabric_abac_load ${FABRIC_DEMO_SRC})
target_compile_options(demo_fabric_abac_load PRIVATE -DUSE_LOAD_PERSIST_WALLET)
target_link_libraries(demo_fabric_abac_load
                        boatwallet
                        boatvendor
                        curl)