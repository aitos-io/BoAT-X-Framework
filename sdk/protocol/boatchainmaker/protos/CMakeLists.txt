

file(GLOB_RECURSE COMMON_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/common/*.c)

set(FILE_SRC    ${COMMON_SRC_FILE})

add_library(protos-chainmaker-obj  OBJECT  ${FILE_SRC})

target_link_libraries(protos-chainmaker-obj  protobuf-obj)

add_dependencies(protos-chainmaker-obj protobuf-obj)



target_include_directories(protos-chainmaker-obj
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    PUBLIC
    ${PROJECT_SOURCE_DIR}/sdk/third-party/protobuf-c/include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)