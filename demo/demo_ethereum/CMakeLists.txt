


set(ETHEREUM_DEMO_SRC         demo_ethereum_transfer.c)

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

