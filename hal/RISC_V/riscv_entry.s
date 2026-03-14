# If you see comments in every line of code, it 
# is just because I do not understand what shit
# is happening here. It is my first experience in
# assembly.
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

  la t0, _bss_start
  la t1, _bss_end

  bge t0, t1, end_clear_bss

clear_bss_loop:
  sw zero, 0(t0)
  addi t0, t0, 4
  blt t0, t1, clear_bss_loop

end_clear_bss:
  call main
