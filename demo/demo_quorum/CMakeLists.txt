


# set(QUORUM_DEMO_SRC         demo_quorum_transfer.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_quorum/runtime)

if(BOAT_PYTHON_EXECUTABLE)
    execute_process(COMMAND ${BOAT_PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/quorum2c.py ${PROJECT_SOURCE_DIR}/demo/demo_quorum/demo_contract/SimpleStorage.json 
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/demo_quorum/demo_contract
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


add_executable(demo_quorum_public_storeread_onetime_internalGen demo_quorum_public_storeread.c)
target_compile_options(demo_quorum_public_storeread_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_quorum_public_storeread_onetime_internalGen
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_public_storeread_onetime_pkcs demo_quorum_public_storeread.c)
target_compile_options(demo_quorum_public_storeread_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_quorum_public_storeread_onetime_pkcs
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_public_storeread_onetime_native demo_quorum_public_storeread.c)
target_compile_options(demo_quorum_public_storeread_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_quorum_public_storeread_onetime_native
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_public_storeread_create_internalGen demo_quorum_public_storeread.c)
target_compile_options(demo_quorum_public_storeread_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_quorum_public_storeread_create_internalGen
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_public_storeread_create_pkcs demo_quorum_public_storeread.c)
target_compile_options(demo_quorum_public_storeread_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_quorum_public_storeread_create_pkcs
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_public_storeread_create_native demo_quorum_public_storeread.c)
target_compile_options(demo_quorum_public_storeread_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_quorum_public_storeread_create_native
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_public_storeread_load demo_quorum_public_storeread.c)
target_compile_options(demo_quorum_public_storeread_load PRIVATE -DUSE_LOAD_PERSIST_WALLET )
target_link_libraries(demo_quorum_public_storeread_load
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)


##############

add_executable(demo_quorum_private_storeread_onetime_internalGen demo_quorum_private_storeread.c)
target_compile_options(demo_quorum_private_storeread_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_quorum_private_storeread_onetime_internalGen
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_private_storeread_onetime_pkcs demo_quorum_private_storeread.c)
target_compile_options(demo_quorum_private_storeread_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_quorum_private_storeread_onetime_pkcs
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_private_storeread_onetime_native demo_quorum_private_storeread.c)
target_compile_options(demo_quorum_private_storeread_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_quorum_private_storeread_onetime_native
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_private_storeread_create_internalGen demo_quorum_private_storeread.c)
target_compile_options(demo_quorum_private_storeread_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_quorum_private_storeread_create_internalGen
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_private_storeread_create_pkcs demo_quorum_private_storeread.c)
target_compile_options(demo_quorum_private_storeread_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_quorum_private_storeread_create_pkcs
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_private_storeread_create_native demo_quorum_private_storeread.c)
target_compile_options(demo_quorum_private_storeread_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_quorum_private_storeread_create_native
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

add_executable(demo_quorum_private_storeread_load demo_quorum_private_storeread.c)
target_compile_options(demo_quorum_private_storeread_load PRIVATE -DUSE_LOAD_PERSIST_WALLET )
target_link_libraries(demo_quorum_private_storeread_load
                        boatwallet
                        boatvendor
                        curl
                        quorum_obj)

##############

add_executable(demo_quorum_transfer_storeread_onetime_internalGen demo_quorum_transfer.c)
target_compile_options(demo_quorum_transfer_storeread_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_quorum_transfer_storeread_onetime_internalGen
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_quorum_transfer_storeread_onetime_pkcs demo_quorum_transfer.c)
target_compile_options(demo_quorum_transfer_storeread_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_quorum_transfer_storeread_onetime_pkcs
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_quorum_transfer_storeread_onetime_native demo_quorum_transfer.c)
target_compile_options(demo_quorum_transfer_storeread_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_quorum_transfer_storeread_onetime_native
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_quorum_transfer_storeread_create_internalGen demo_quorum_transfer.c)
target_compile_options(demo_quorum_transfer_storeread_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_quorum_transfer_storeread_create_internalGen
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_quorum_transfer_storeread_create_pkcs demo_quorum_transfer.c)
target_compile_options(demo_quorum_transfer_storeread_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_quorum_transfer_storeread_create_pkcs
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_quorum_transfer_storeread_create_native demo_quorum_transfer.c)
target_compile_options(demo_quorum_transfer_storeread_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_quorum_transfer_storeread_create_native
                        boatwallet
                        boatvendor
                        curl)

add_executable(demo_quorum_transfer_storeread_load demo_quorum_transfer.c)
target_compile_options(demo_quorum_transfer_storeread_load PRIVATE -DUSE_LOAD_PERSIST_WALLET )
target_link_libraries(demo_quorum_transfer_storeread_load
                        boatwallet
                        boatvendor
                        curl)





