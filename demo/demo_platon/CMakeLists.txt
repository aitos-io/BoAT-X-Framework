


set(PLATON_DEMO_SRC         demo_platon_transfer.c)



include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_platone/demo_contract/Solidity)


#add_executable(demo_fiscobcos_onetime ${FISCOBCOS_DEMO_SRC})
#target_compile_options(demo_fiscobcos_onetime PRIVATE -DUSE_ONETIME_WALLET)
#target_link_libraries(demo_fiscobcos_onetime
#                        boatwallet
#                        boatvendor
#                        fiscobcos_obj
#                        curl)
