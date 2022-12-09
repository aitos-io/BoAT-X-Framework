


set(VENACHAIN_DEMO_SRC         demo_venachain_mycontract.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_venachain/runtime)

if(BOAT_PYTHON_EXECUTABLE)
    execute_process(COMMAND ${BOAT_PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/venachainSolidity2c.py ${PROJECT_SOURCE_DIR}/demo/demo_venachain/demo_contract/Solidity/mycontract.cpp.abi.json 
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/demo_venachain/demo_contract/Solidity
                    RESULT_VARIABLE result)
    if(${result} EQUAL 1)
        message(FATAL_ERROR "run python gen.py boatconfig.h error")
    endif()
    execute_process(COMMAND ${BOAT_PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/venachainWASM2c.py ${PROJECT_SOURCE_DIR}/demo/demo_venachain/demo_contract/WASM/mycontract.cpp.abi.json 
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/demo_venachain/demo_contract/WASM
                    RESULT_VARIABLE result)
    if(${result} EQUAL 1)
        message(FATAL_ERROR "run python gen.py boatconfig.h error")
    endif()
else()
    message(FATAL_ERROR "not fund python3, please install python3")
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_venachain/demo_contract/Solidity)
add_subdirectory(demo_contract/Solidity)


add_executable(demo_venachain_mycontract_onetime_internalGen ${VENACHAIN_DEMO_SRC})
target_compile_options(demo_venachain_mycontract_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_venachain_mycontract_onetime_internalGen
                       boatwallet
                       boatvendor
                       venachain_obj
                       curl)

add_executable(demo_venachain_mycontract_onetime_pkcs ${VENACHAIN_DEMO_SRC})
target_compile_options(demo_venachain_mycontract_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_venachain_mycontract_onetime_pkcs
                       boatwallet
                       boatvendor
                       venachain_obj
                       curl)

add_executable(demo_venachain_mycontract_onetime_native ${VENACHAIN_DEMO_SRC})
target_compile_options(demo_venachain_mycontract_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_venachain_mycontract_onetime_native
                       boatwallet
                       boatvendor
                       venachain_obj
                       curl)

add_executable(demo_venachain_mycontract_create_internalGen ${VENACHAIN_DEMO_SRC})
target_compile_options(demo_venachain_mycontract_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_venachain_mycontract_create_internalGen
                       boatwallet
                       boatvendor
                       venachain_obj
                       curl)


add_executable(demo_venachain_mycontract_create_pkcs ${VENACHAIN_DEMO_SRC})
target_compile_options(demo_venachain_mycontract_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_venachain_mycontract_create_pkcs
                       boatwallet
                       boatvendor
                       venachain_obj
                       curl)

add_executable(demo_venachain_mycontract_create_native ${VENACHAIN_DEMO_SRC})
target_compile_options(demo_venachain_mycontract_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_venachain_mycontract_create_native
                       boatwallet
                       boatvendor
                       venachain_obj
                       curl)



                

