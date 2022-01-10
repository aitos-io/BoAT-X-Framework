
file(GLOB_RECURSE FILE_SRC     ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_library(ethereum_obj  OBJECT  ${FILE_SRC})


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/platform/include)

target_link_libraries(ethereum_obj)




