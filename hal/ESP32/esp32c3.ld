/* Very Slavic OS Linker Script */

/* Вказуємо точку входу (переконайтеся, що у вашому riscv_entry.s вона називається саме так) */
ENTRY(_text_start)

MEMORY
{
    /* IRAM - шина інструкцій (тільки для коду) */
    iram (RX) : ORIGIN = 0x40380000, LENGTH = 0x60000 
    
    /* DRAM - шина даних (для змінних та констант) */
    dram (RW) : ORIGIN = 0x3FC80000, LENGTH = 0x60000
}

_stack_top = ORIGIN(dram) + LENGTH(dram);

SECTIONS
{
    /* 1. СЕКЦІЯ КОДУ (йде в IRAM) */
    .text : SUBALIGN(4)
    {
        _text_start = .;
        
        /* Гарантуємо, що стартап-код буде найпершим */
        KEEP(*(.init))
        KEEP(*(.text.entry)) 
        
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        
        . = ALIGN(4);
        _text_end = .;
    } > iram

    /* 2. СЕКЦІЯ КОНСТАНТ / РЯДКІВ (йде в DRAM!) */
    /* Саме тут живуть ваші "Pryvit from..." */
    .rodata : ALIGN(4)
    {
        _rodata_start = .;
        
        *(.rodata)
        *(.rodata.*)
        *(.srodata)      /* Малі константи RISC-V */
        *(.srodata.*)
        
        . = ALIGN(4);
        _rodata_end = .;
    } > dram

    /* 3. СЕКЦІЯ ІНІЦІАЛІЗОВАНИХ ДАНИХ (йде в DRAM) */
    .data : ALIGN(4)
    {
        _data_start = .;
        
        /* RISC-V Global Pointer. Абсолютно необхідний для правильної роботи C-коду! */
        /* Зазвичай вказує на початок sdata + 0x800 */
        __global_pointer$ = . + 0x800; 
        
        *(.data)
        *(.data.*)
        *(.sdata)        /* Малі змінні RISC-V */
        *(.sdata.*)
        
        . = ALIGN(4);
        _data_end = .;
    } > dram

    /* 4. СЕКЦІЯ НЕІНІЦІАЛІЗОВАНИХ ДАНИХ (йде в DRAM, зануляється стартапом) */
    .bss : ALIGN(4)
    {
        _bss_start = .;
        
        *(.bss)
        *(.bss.*)
        *(.sbss)         /* Малі неініціалізовані змінні */
        *(.sbss.*)
        *(COMMON)
        
        . = ALIGN(4);
        _bss_end = .;
    } > dram
}

PROVIDE(memset = 0x40000354);
PROVIDE(memcpy = 0x40000358);
PROVIDE(memmove = 0x4000035c);
PROVIDE(memcmp = 0x40000360);
PROVIDE(strcpy = 0x40000364);
PROVIDE(strncpy = 0x40000368);
PROVIDE(strcmp = 0x4000036c);
PROVIDE(strncmp = 0x40000370);
PROVIDE(strlen = 0x40000374);
PROVIDE(strstr = 0x40000378);
PROVIDE(bzero = 0x4000037c);
PROVIDE(isalnum = 0x40000388);
PROVIDE(isalpha = 0x4000038c);
PROVIDE(isascii = 0x40000390);
PROVIDE(isblank = 0x40000394);
PROVIDE(iscntrl = 0x40000398);
PROVIDE(isdigit = 0x4000039c);
PROVIDE(islower = 0x400003a0);
PROVIDE(isgraph = 0x400003a4);
PROVIDE(isprint = 0x400003a8);
PROVIDE(ispunct = 0x400003ac);
PROVIDE(isspace = 0x400003b0);
PROVIDE(isupper = 0x400003b4);
PROVIDE(toupper = 0x400003b8);
PROVIDE(tolower = 0x400003bc);
PROVIDE(toascii = 0x400003c0);
PROVIDE(memccpy = 0x400003c4);
PROVIDE(memchr = 0x400003c8);
PROVIDE(memrchr = 0x400003cc);
PROVIDE(strcasecmp = 0x400003d0);
PROVIDE(strcasestr = 0x400003d4);
PROVIDE(strcat = 0x400003d8);
PROVIDE(strdup = 0x400003dc);
PROVIDE(strchr = 0x400003e0);
PROVIDE(strcspn = 0x400003e4);
PROVIDE(strcoll = 0x400003e8);
PROVIDE(strlcat = 0x400003ec);
PROVIDE(strlcpy = 0x400003f0);
PROVIDE(strlwr = 0x400003f4);
PROVIDE(strncasecmp = 0x400003f8);
PROVIDE(strncat = 0x400003fc);
PROVIDE(strndup = 0x40000400);
PROVIDE(strnlen = 0x40000404);
PROVIDE(strrchr = 0x40000408);
PROVIDE(strsep = 0x4000040c);
PROVIDE(strspn = 0x40000410);
PROVIDE(strtok_r = 0x40000414);
PROVIDE(strupr = 0x40000418);
PROVIDE(longjmp = 0x4000041c);
PROVIDE(setjmp = 0x40000420);
PROVIDE(abs = 0x40000424);
PROVIDE(div = 0x40000428);
PROVIDE(labs = 0x4000042c);
PROVIDE(ldiv = 0x40000430);
PROVIDE(qsort = 0x40000434);
PROVIDE(rand_r = 0x40000438);
PROVIDE(rand = 0x4000043c);
PROVIDE(srand = 0x40000440);
PROVIDE(utoa = 0x40000444);
PROVIDE(itoa = 0x40000448);
PROVIDE(atoi = 0x4000044c);
PROVIDE(atol = 0x40000450);
PROVIDE(strtol = 0x40000454);
PROVIDE(strtoul = 0x40000458);
PROVIDE(printf = 0x40000040); 
PROVIDE(__udivdi3 = 0x400008ac);
PROVIDE(__umoddi3 = 0x400008bc);
PROVIDE(ets_printf = 0x40000068);
