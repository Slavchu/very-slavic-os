# If you see comments in every line of code, it 
# is just because I do not understand what shit
# is happening here. It is my first experience in
# assembly.

# global variables provided by linker
.global UART0
.global RTCCNTL
.global TIMERG0
.global TIMERG1

# constants
   .section .rodata
boot_msg:
    .asciz "Very slavic os bootloader\r\nBooted successfuly\r\n"

# section code
  .section .init
  .global _start
  .type _start, @function
_start:
  csrw mstatus, zero        # Disabling interrupts

  .option push
  .option norelax           # Disabling optimiazation
  la gp, __global_pointer$  # Set global pointer register
  .option pop

  la sp, _stack_top         # Set stack pointer
  addi sp, sp, -128         # this will be reserved for context

  la t0, _bss_start
  la t1, _bss_end

  bge t0, t1, end_clear_bss

clear_bss_loop:
  sw zero, 0(t0)
  addi t0, t0, 4
  blt t0, t1, clear_bss_loop

end_clear_bss:
disable_rtc_watchdog:
    la t0, RTCCNTL

    li t1, 0x50D83AA1
    sw t1, 0xA8(t0)       
    sw zero, 0x90(t0)
    sw zero, 0x8C(t0)

    # --- FEED superwatchdog
    li t1, 0x8F1D312A
    sw t1, 0xB0(t0)

    lw t1, 0xAC(t0)
    li t2, 0x80000000
    or t1, t1, t2
    sw t1, 0xAC(t0)

    sw zero, 0xB4(t0)

    # ---TIMER GROUP 0 WDT
    la t0, TIMERG0

    lw t1, 0xFC(t0)
    li t2, 0xFFFFFDFF
    and t1, t1, t2
    sw t1, 0xFC(t0)
    sw zero, 0x48(t0)


    # --- TIMER GROUP 1 WDT
    la t0, TIMERG1
    sw zero, 0x48(t0)

print_boot_message:
  la t0, UART0
  la t1, boot_msg
print_loop:
  lbu t2, 0(t1)
  beqz t2, print_done
wait_fifo:
  lw t3, 0x1C(t0)
  srli t3, t3, 16
  andi t3, t3, 0xFF
  li t4, 120
  bge t3, t4, wait_fifo

  sw t2, 0(t0)
  addi t1, t1, 1
  j print_loop
print_done:

execute_core:
call main
