


set(PLATON_DEMO_SRC         demo_platon_transfer.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/demo/demo_platon/runtime)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)
include_directories(${PROJECT_SOURCE_DIR}/demo/demo_platone/demo_contract/Solidity)


add_executable(demo_platon_transfer_onetime_internalGen ${PLATON_DEMO_SRC})
target_compile_options(demo_platon_transfer_onetime_internalGen PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_platon_transfer_onetime_internalGen
                       boatwallet
                       boatvendor
                       curl)

add_executable(demo_platon_transfer_onetime_pkcs ${PLATON_DEMO_SRC})
target_compile_options(demo_platon_transfer_onetime_pkcs PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_platon_transfer_onetime_pkcs
                       boatwallet
                       boatvendor
                       curl)

add_executable(demo_platon_transfer_onetime_native ${PLATON_DEMO_SRC})
target_compile_options(demo_platon_transfer_onetime_native PRIVATE -DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_platon_transfer_onetime_native
                       boatwallet
                       boatvendor
                       curl)

add_executable(demo_platon_transfer_create_internalGen ${PLATON_DEMO_SRC})
target_compile_options(demo_platon_transfer_create_internalGen PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION)
target_link_libraries(demo_platon_transfer_create_internalGen
                       boatwallet
                       boatvendor
                       curl)

add_executable(demo_platon_transfer_create_pkcs ${PLATON_DEMO_SRC})
target_compile_options(demo_platon_transfer_create_pkcs PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
target_link_libraries(demo_platon_transfer_create_pkcs
                       boatwallet
                       boatvendor
                       curl)

add_executable(demo_platon_transfer_create_native ${PLATON_DEMO_SRC})
target_compile_options(demo_platon_transfer_create_native PRIVATE -DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
target_link_libraries(demo_platon_transfer_create_native
                       boatwallet
                       boatvendor
                       curl)

add_executable(demo_platon_transfer_load ${PLATON_DEMO_SRC})
target_compile_options(demo_platon_transfer_load PRIVATE -DUSE_LOAD_PERSIST_WALLET)
target_link_libraries(demo_platon_transfer_load
                       boatwallet
                       boatvendor
                       curl)











