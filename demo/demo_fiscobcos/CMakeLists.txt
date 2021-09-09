


set(FISCOBCOS_DEMO_SRC         demo_fiscobcos_helloworld.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_fiscobcos/runtime)

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



add_executable(demo_fiscobcos_helloworld_onetime_internalGen ${FISCOBCOS_DEMO_SRC})
target_compile_options(demo_fiscobcos_helloworld_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_fiscobcos_helloworld_onetime_internalGen
                       boatwallet
                       boatvendor
                       fiscobcos_obj
                       curl)

add_executable(demo_fiscobcos_helloworld_onetime_pkcs ${FISCOBCOS_DEMO_SRC})
target_compile_options(demo_fiscobcos_helloworld_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_fiscobcos_helloworld_onetime_pkcs
                       boatwallet
                       boatvendor
                       fiscobcos_obj
                       curl)

add_executable(demo_fiscobcos_helloworld_onetime_native ${FISCOBCOS_DEMO_SRC})
target_compile_options(demo_fiscobcos_helloworld_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_fiscobcos_helloworld_onetime_native
                       boatwallet
                       boatvendor
                       fiscobcos_obj
                       curl)


add_executable(demo_fiscobcos_helloworld_create_internalGen ${FISCOBCOS_DEMO_SRC})
target_compile_options(demo_fiscobcos_helloworld_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_fiscobcos_helloworld_create_internalGen
                       boatwallet
                       boatvendor
                       fiscobcos_obj
                       curl)

add_executable(demo_fiscobcos_helloworld_create_pkcs ${FISCOBCOS_DEMO_SRC})
target_compile_options(demo_fiscobcos_helloworld_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_fiscobcos_helloworld_create_pkcs
                       boatwallet
                       boatvendor
                       fiscobcos_obj
                       curl)

add_executable(demo_fiscobcos_helloworld_create_native ${FISCOBCOS_DEMO_SRC})
target_compile_options(demo_fiscobcos_helloworld_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_fiscobcos_helloworld_create_native
                       boatwallet
                       boatvendor
                       fiscobcos_obj
                       curl)

add_executable(demo_fiscobcos_helloworld_load ${FISCOBCOS_DEMO_SRC})
target_compile_options(demo_fiscobcos_helloworld_load PRIVATE -DUSE_LOAD_PERSIST_WALLET)
target_link_libraries(demo_fiscobcos_helloworld_load
                       boatwallet
                       boatvendor
                       fiscobcos_obj
                       curl)











