/* Define the entry point */
ENTRY(_text_start)

MEMORY
{
  /* Internal SRAM 1: Instruction Bus */
  IRAM (RX) : ORIGIN = 0x40380400, LENGTH = 16K 
  /* Internal SRAM 1: Data Bus */
  DRAM (RW) : ORIGIN = 0x3FC80000 + LENGTH(IRAM), LENGTH = 128K
}

_stack_top = ORIGIN(DRAM) + LENGTH(DRAM);

SECTIONS
{
  /* --- Instruction Section --- */
  .text : ALIGN(4)
  {
    _text_start = .;
    KEEP(*(.init))
    KEEP(*(.text.entry))
    *(.text .text.*)
    _text_end = .;
  } > IRAM
  .vectors : ALIGN(256)
  {
    _vectors_start = .;
    KEEP(*(.vectors.riscv))
    KEEP(*(.vectors.esp32)) 
  } > IRAM

  .func_tables : {
     . = ALIGN(4);   
     PROVIDE(_func_tables_start = .);
     KEEP(*(SORT_BY_NAME(.func_tables.*)))
     PROVIDE(_func_tables_end = .);
  } > DRAM

  .data : ALIGN(4)
  {
    _data_start = .;
    __global_pointer$ = . + 0x800;
    
    *(.data)
    *(.data*)
    *(.sdata)
    *(.sdata*)
    . = ALIGN(16);
    _data_end = .;
  } > DRAM

  .rodata : ALIGN(4)
  {
    *(.rodata .rodata.*)
    *(.srodata .srodata.*)
  } > DRAM

  .bss : ALIGN(16)
  {
    _bss_start = .;
    *(.sbss .sbss.*)
    *(.bss .bss.*)
    *(COMMON)
    _bss_end = .;
  } > DRAM
  .tdata : ALIGN(4)
  {
    _tdata_start = .;
    *(.tdata .tdata.* .gnu.linkonce.td.*)
    _tdata_end = .;
  } > DRAM

  .tbss (NOLOAD) : ALIGN(4)
  {
    _tbss_start = .;
    *(.tbss .tbss.* .gnu.linkonce.tb.*)
    *(.tcommon)
    _tbss_end = .;
  } > DRAM

  . = ALIGN(16);
  PROVIDE(__heap_start = .);
  PROVIDE(__heap_end = _stack_top - 0x1000);
}
