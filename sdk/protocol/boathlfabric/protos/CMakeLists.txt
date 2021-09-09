

file(GLOB_RECURSE COMMON_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/common/*.c)
file(GLOB_RECURSE DISCOVERY_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/discovery/*.c)
file(GLOB_RECURSE PROTOBUF_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/google/protobuf/*.c)
file(GLOB_RECURSE GOSSIP_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/gossip/*.c)
file(GLOB_RECURSE IDEMIX_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/idemix/*.c)
file(GLOB_RECURSE QUERYRESULT_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/ledger/queryresult/*.c)
file(GLOB_RECURSE KVRWSET_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/ledger/rwset/kvrwset/*.c)
file(GLOB_RECURSE RWSET_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/ledger/rwset/*.c)
file(GLOB_RECURSE MSP_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/msp/*.c)
file(GLOB_RECURSE ORDERER_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/orderer/*.c)
file(GLOB_RECURSE ETCDRAFT_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/orderer/etcdraft/*.c)
file(GLOB_RECURSE PEER_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/peer/*.c)
file(GLOB_RECURSE LIFTCYCLE_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/peer/lifecycle/*.c)
file(GLOB_RECURSE TOKEN_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/token/*.c)
file(GLOB_RECURSE TRANSIENTSTORE_SRC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/transientstore/*.c)

set(FILE_SRC    ${COMMON_SRC_FILE}
                ${DISCOVERY_SRC_FILE}
                ${PROTOBUF_SRC_FILE}
                ${GOSSIP_SRC_FILE}
                ${IDEMIX_SRC_FILE}
                ${QUERYRESULT_SRC_FILE}
                ${KVRWSET_SRC_FILE}
                ${RWSET_SRC_FILE}
                ${MSP_SRC_FILE}
                ${ORDERER_SRC_FILE}
                ${ETCDRAFT_SRC_FILE}
                ${PEER_SRC_FILE}
                ${LIFTCYCLE_SRC_FILE}
                ${TOKEN_SRC_FILE}
                ${TRANSIENTSTORE_SRC_FILE})

add_library(protos-obj  OBJECT  ${FILE_SRC})

target_link_libraries(protos-obj  protobuf-obj)

add_dependencies(protos-obj protobuf-obj)



target_include_directories(protos-obj
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    PUBLIC
    ${PROJECT_SOURCE_DIR}/sdk/third-party/protobuf-c/include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)