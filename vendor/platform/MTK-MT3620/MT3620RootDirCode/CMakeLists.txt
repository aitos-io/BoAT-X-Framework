cmake_minimum_required (VERSION 3.10)

project (BoAT_MT3620 C)

azsphere_configure_tools(TOOLS_REVISION "20.04")
azsphere_configure_api(TARGET_API_SET "10")

# Add include directories 
include_directories(boatiotsdk/include
					boatiotsdk/include/protocolapi
					boat_demo
					)

# Add sub directories
#add_subdirectory(demo)
	
#add compile option
add_compile_options(-std=c99 -Wall -Os)
#add link option
add_compile_options(-Wl,-Map)

#set executable file output path
#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/demo) 
link_directories("boatiotsdk/lib")
# Create executable
# -create demo_fiscobcos_helloworld
add_executable (${PROJECT_NAME}  main-platone.c  "boat_demo/my_contract.cpp.abi.c" )
target_include_directories (${PROJECT_NAME} PUBLIC ${AZURE_SPHERE_API_SET_DIR}/usr/include/azureiot)
target_link_libraries (${PROJECT_NAME} libboatwallet.a libboatvendor.a  azureiot curl applibs pthread gcc_s c wolfssl)



azsphere_target_hardware_definition(${PROJECT_NAME} TARGET_DIRECTORY "../Hardware/mt3620_rdb" TARGET_DEFINITION "sample_hardware.json")
azsphere_target_add_image_package( ${PROJECT_NAME} RESOURCE_FILES)
