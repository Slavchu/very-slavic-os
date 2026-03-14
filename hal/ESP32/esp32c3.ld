ENTRY(_start)

MEMORY
{
    iram (rx) : ORIGIN = 0x40380000, LENGTH = 400K
    
    dram (rw) : ORIGIN = 0x3FC80000, LENGTH = 400K
}

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        _text_start = .;
        
        KEEP(*(.init))      
        
        *(.text)
        *(.text.*)          
        
        . = ALIGN(4);
        _text_end = .;
    } > iram

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
    } > dram

    .data :
    {
        . = ALIGN(4);
        _data_start = .;
        
        __global_pointer$ = . + 0x800; 
        
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)

        . = ALIGN(4);
        _data_end = .;
    } > dram

    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
        
        . = ALIGN(4);
        _bss_end = .;
    } > dram

    _stack_top = ORIGIN(dram) + LENGTH(dram);
}
