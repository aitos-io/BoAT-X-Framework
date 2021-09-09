

file(GLOB_RECURSE COMMON_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/common/*.c)


set(FILE_SRC    ${COMMON_SRC_FILE})

add_library(protos-huawei-obj  OBJECT  ${FILE_SRC})




target_include_directories(protos-huawei-obj
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/sdk/third-party/protobuf-c/include
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)