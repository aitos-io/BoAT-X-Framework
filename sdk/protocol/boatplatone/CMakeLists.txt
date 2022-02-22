set(SUB_MODULE_NAME boatplatone-obj)

set(FILE_SRC        boatplatone.c)


add_library(${SUB_MODULE_NAME}  OBJECT  ${FILE_SRC})

target_include_directories(${SUB_MODULE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/platform/include
    ${PROJECT_SOURCE_DIR}/sdk/protocol
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
