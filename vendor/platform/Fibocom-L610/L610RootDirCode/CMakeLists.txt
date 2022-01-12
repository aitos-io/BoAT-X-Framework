
include_directories(BoAT-X-Framework/include BoAT-X-Framework/include/protocolapi)

    add_appimg(${target} ${flash_ldscript} 
    	demo.c my_contract.cpp.abi.c)
    target_link_libraries(${target} PRIVATE ${libboatwallet_file_name} ${libboathvendor_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
