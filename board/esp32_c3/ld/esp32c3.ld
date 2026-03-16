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

  .data : 
  {
    _data_start = .;
    
    /* Global Pointer Initialization */
    . = ALIGN(16);
    *(.data)
    *(.data*)
    *(.sdata)
    *(.sdata*)
    *(.srodata)
    *(.srodata*)
    *(.rodata)
    *(.rodata*)
    *(.gnu.linkonce.r.*)
    *(.rodata1)
    *(.riscv.*)
    __global_pointer$ = . + 0x800;
    . = ALIGN(16);
    _data_end = .;
  } > DRAM

  /* --- Uninitialized Data (BSS) Section --- */
  .bss : ALIGN(16)
  {
    _bss_start = .;
    *(.sbss .sbss.*)     /* Small BSS */
    *(.bss .bss.*)
    *(COMMON)
    _bss_end = .;
  } > DRAM
}
