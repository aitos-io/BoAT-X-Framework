set(SUB_MODULE_NAME boatplaton-obj)

file(GLOB_RECURSE SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

message(STATUS "platon src : " ${SRC_FILE})
add_library(${SUB_MODULE_NAME}  OBJECT  ${SRC_FILE})

target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    ${PROJECT_SOURCE_DIR}/sdk/protocol
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
