 add_appimg(${target} ${flash_ldscript} ${target_file_path}/${target_file}.c ${target_file_path}/my_contract.c ${OEM})
 target_link_libraries(${target} PRIVATE ${libbw_file_name} ${libbh_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
