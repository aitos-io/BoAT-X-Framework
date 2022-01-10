PHDRS {
  LR_1 PT_LOAD;
}

SECTIONS {
  ER_RO 0x43000000 : {
    Image$$ER_RO$$Base = .;
	*txm_module_preamble_llvm.o (.text .text.*)
	*txm_module_preamble_llvm.o (Init)
	*txm_module_preamble_llvm.o (.rodata .rodata.*)
    KEEP(* (.text .text.*))
    * (Init)
    * (.rodata .rodata.*)
    Image$$ER_RO$$Limit = .;
  } : LR_1

  Image$$ER_RO$$Length = Image$$ER_RO$$Limit - Image$$ER_RO$$Base;

  ER_RW ALIGN(8) : {
    Image$$ER_RW$$Base = .;
    KEEP(* (.data .data.*))
    Image$$ER_RW$$Limit = .;
  } : LR_1

  Image$$ER_RW$$Length = Image$$ER_RW$$Limit - Image$$ER_RW$$Base;

  ER_ZI ALIGN(8) : {
    Image$$ER_ZI$$Base = .;
   *(.bss* .bss.* .common COMMON)
    Image$$ER_ZI$$Limit = .;
  } : LR_1

  Image$$ER_ZI$$ZI$$Length = Image$$ER_ZI$$Limit - Image$$ER_ZI$$Base;

  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* DWARF 3 */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
  .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
  /DISCARD/ : { *(.ARM.exidx*) *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) *(.init) *(.fini) }
}