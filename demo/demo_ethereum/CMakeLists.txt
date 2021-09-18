


set(ETHEREUM_DEMO_SRC         demo_ethereum_transfer.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_ethereum/runtime)

if(BOAT_PYTHON_EXECUTABLE)
    execute_process(COMMAND ${BOAT_PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/eth2c.py ${PROJECT_SOURCE_DIR}/demo/demo_ethereum/demo_contract/StoreRead.json 
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/demo_ethereum/demo_contract
                    RESULT_VARIABLE result)
    if(${result} EQUAL 1)
        message(FATAL_ERROR "run python gen.py boatconfig.h error")
    endif()
else()
    message(FATAL_ERROR "not fund python3, please install python3")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)
add_subdirectory(demo_contract)


add_executable(demo_ethereum_transfer_onetime_internalGen ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_transfer_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_ethereum_transfer_onetime_internalGen
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_transfer_onetime_pkcs ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_transfer_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_ethereum_transfer_onetime_pkcs
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_transfer_onetime_native ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_transfer_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_ethereum_transfer_onetime_native
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_transfer_create_internalGen ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_transfer_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_ethereum_transfer_create_internalGen
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_transfer_create_pkcs ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_transfer_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_ethereum_transfer_create_pkcs
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_transfer_create_native ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_transfer_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_ethereum_transfer_create_native
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_transfer_load ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_transfer_load PRIVATE -DUSE_LOAD_PERSIST_WALLET)
target_link_libraries(demo_ethereum_transfer_load
                        boatwallet
                        boatvendor
                        curl)


##############

add_executable(demo_ethereum_storeread_onetime_internalGen ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_storeread_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_ethereum_storeread_onetime_internalGen
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_storeread_onetime_pkcs ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_storeread_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_ethereum_storeread_onetime_pkcs
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_storeread_onetime_native ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_storeread_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_ethereum_storeread_onetime_native
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_storeread_create_internalGen ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_storeread_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_ethereum_storeread_create_internalGen
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_storeread_create_pkcs ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_storeread_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS )
target_link_libraries(demo_ethereum_storeread_create_pkcs
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_storeread_create_native ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_storeread_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_ethereum_storeread_create_native
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_ethereum_storeread_load ${ETHEREUM_DEMO_SRC})
target_compile_options(demo_ethereum_storeread_load PRIVATE -DUSE_LOAD_PERSIST_WALLET)
target_link_libraries(demo_ethereum_storeread_load
                        boatwallet
                        boatvendor
                        curl)





