


set(HUAWEI_DEMO_SRC         demo_huawei.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_huawei/runtime)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)

add_executable(demo_huawei_onetime ${HUAWEI_DEMO_SRC})
target_compile_options(demo_huawei_onetime PRIVATE -DUSE_ONETIME_WALLET)
target_link_libraries(demo_huawei_onetime
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_huawei_persist ${HUAWEI_DEMO_SRC})
target_compile_options(demo_huawei_persist PRIVATE -DUSE_CREATE_PERSIST_WALLET)
target_link_libraries(demo_huawei_persist
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_huawei_load ${HUAWEI_DEMO_SRC})
target_compile_options(demo_huawei_load PRIVATE -DUSE_LOAD_PERSIST_WALLET)
target_link_libraries(demo_huawei_load
                        boatwallet
                        boatvendor
                        curl)