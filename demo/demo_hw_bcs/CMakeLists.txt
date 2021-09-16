


set(HW_BCS_DEMO_SRC         demo_hw_bcs.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_hw_bcs/runtime)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)

add_executable(demo_hw_bcs_onetime ${HW_BCS_DEMO_SRC})
target_compile_options(demo_hw_bcs_onetime PRIVATE -DUSE_ONETIME_WALLET)
target_link_libraries(demo_hw_bcs_onetime
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_hw_bcs_persist ${HW_BCS_DEMO_SRC})
target_compile_options(demo_hw_bcs_persist PRIVATE -DUSE_CREATE_PERSIST_WALLET)
target_link_libraries(demo_hw_bcs_persist
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_hw_bcs_load ${HW_BCS_DEMO_SRC})
target_compile_options(demo_hw_bcs_load PRIVATE -DUSE_LOAD_PERSIST_WALLET)
target_link_libraries(demo_hw_bcs_load
                        boatwallet
                        boatvendor
                        curl)