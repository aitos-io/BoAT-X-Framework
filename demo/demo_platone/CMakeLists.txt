


set(PLATONE_DEMO_SRC         demo_platone_mycontract.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_platone/runtime)

if(BOAT_PYTHON_EXECUTABLE)
    execute_process(COMMAND ${BOAT_PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/platoneWASM2c.py ${PROJECT_SOURCE_DIR}/demo/demo_platone/demo_contract/WASM/my_contract.cpp.abi.json 
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/demo_platone/demo_contract/Solidity
                    RESULT_VARIABLE result)
    if(${result} EQUAL 1)
        message(FATAL_ERROR "run python gen.py boatconfig.h error")
    endif()
else()
    message(FATAL_ERROR "not fund python3, please install python3")
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_platone/demo_contract/Solidity)
add_subdirectory(demo_contract/Solidity)


add_executable(demo_platone_mycontract_onetime_internalGen ${PLATONE_DEMO_SRC})
target_compile_options(demo_platone_mycontract_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_platone_mycontract_onetime_internalGen
                       boatwallet
                       boatvendor
                       platone_obj
                       curl)

add_executable(demo_platone_mycontract_onetime_pkcs ${PLATONE_DEMO_SRC})
target_compile_options(demo_platone_mycontract_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_platone_mycontract_onetime_pkcs
                       boatwallet
                       boatvendor
                       platone_obj
                       curl)

add_executable(demo_platone_mycontract_onetime_native ${PLATONE_DEMO_SRC})
target_compile_options(demo_platone_mycontract_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_platone_mycontract_onetime_native
                       boatwallet
                       boatvendor
                       platone_obj
                       curl)

add_executable(demo_platone_mycontract_create_internalGen ${PLATONE_DEMO_SRC})
target_compile_options(demo_platone_mycontract_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_platone_mycontract_create_internalGen
                       boatwallet
                       boatvendor
                       platone_obj
                       curl)


add_executable(demo_platone_mycontract_create_pkcs ${PLATONE_DEMO_SRC})
target_compile_options(demo_platone_mycontract_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_platone_mycontract_create_pkcs
                       boatwallet
                       boatvendor
                       platone_obj
                       curl)

add_executable(demo_platone_mycontract_create_native ${PLATONE_DEMO_SRC})
target_compile_options(demo_platone_mycontract_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_platone_mycontract_create_native
                       boatwallet
                       boatvendor
                       platone_obj
                       curl)

add_executable(demo_platone_mycontract_load ${PLATONE_DEMO_SRC})
target_compile_options(demo_platone_mycontract_load PRIVATE -DUSE_LOAD_PERSIST_WALLET )
target_link_libraries(demo_platone_mycontract_load
                       boatwallet
                       boatvendor
                       platone_obj
                       curl)


                

