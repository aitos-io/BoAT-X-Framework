


set(FISCOBCOS_DEMO_SRC         demo_fiscobcos_helloworld.c)


if(BOAT_PYTHON_EXECUTABLE)
    execute_process(COMMAND ${BOAT_PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/fiscobcos2c.py ${PROJECT_SOURCE_DIR}/demo/demo_fiscobcos/demo_contract/HelloWorld.json 
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/demo_fiscobcos/demo_contract
                    RESULT_VARIABLE result)
    if(${result} EQUAL 1)
        message(FATAL_ERROR "run python gen.py boatconfig.h error")
    endif()
else()
    message(FATAL_ERROR "not fund python3, please install python3")
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_fiscobcos/demo_contract)
add_subdirectory(demo_contract)


#add_executable(demo_fiscobcos_onetime ${FISCOBCOS_DEMO_SRC})
#target_compile_options(demo_fiscobcos_onetime PRIVATE -DUSE_ONETIME_WALLET)
#target_link_libraries(demo_fiscobcos_onetime
#                        boatwallet
#                        boatvendor
#                        fiscobcos_obj
#                        curl)
