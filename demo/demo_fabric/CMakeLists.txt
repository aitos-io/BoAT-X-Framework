


set(FABRIC_DEMO_SRC         demo_fabric_abac.c)


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)

add_executable(demo_fabric_abac ${FABRIC_DEMO_SRC})
target_compile_options(demo_fabric_abac PRIVATE -DUSE_ONETIME_WALLET)
target_link_libraries(demo_fabric_abac
                        ${PROJECT_SOURCE_DIR}/lib/libboatwallet.a
                        boatvendor
                        curl)