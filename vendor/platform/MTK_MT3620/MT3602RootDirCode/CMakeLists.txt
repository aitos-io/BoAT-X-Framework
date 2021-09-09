
include_directories(BoAT-X-Framework/include BoAT-X-Framework/include/protocolapi)

target_link_libraries(${target} PRIVATE ${libbw_file_name} ${libbh_file_name}  ${libc_file_name} ${libm_file_name} ${libgcc_file_name})


add_appimg(${target} ${file_ldscript} nwy_drv_lcd_st7735.c demo_entry.c my_contract.cpp.abi.c)



