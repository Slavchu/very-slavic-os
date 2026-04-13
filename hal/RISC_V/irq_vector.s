.section .vectors.riscv
.global _vectors_start
.align 8
_vectors_start:
    j _panic_trap
    .rept 31
    j _clint_trap
    .endr

.section .text
.global _clint_trap
.global _panic_trap
.global interrupt_dispatcher
.global riscv_interrupts_panic
_clint_trap:
#saving registers
    addi sp, sp, -132
    sw ra,  0(sp)
    sw gp,  8(sp)   # x3
    sw tp,  12(sp)  # x4
    sw t0,  16(sp)  # x5
    sw t1,  20(sp)  # x6
    sw t2,  24(sp)  # x7
    sw s0,  28(sp)  # x8 / fp
    sw s1,  32(sp)  # x9
    sw a0,  36(sp)  # x10
    sw a1,  40(sp)  # x11
    sw a2,  44(sp)  # x12
    sw a3,  48(sp)  # x13
    sw a4,  52(sp)  # x14
    sw a5,  56(sp)  # x15
    sw a6,  60(sp)  # x16
    sw a7,  64(sp)  # x17
    sw s2,  68(sp)  # x18
    sw s3,  72(sp)  # x19
    sw s4,  76(sp)  # x20
    sw s5,  80(sp)  # x21
    sw s6,  84(sp)  # x22
    sw s7,  88(sp)  # x23
    sw s8,  92(sp)  # x24
    sw s9,  96(sp)  # x25
    sw s10, 100(sp) # x26
    sw s11, 104(sp) # x27
    sw t3,  108(sp) # x28
    sw t4,  112(sp) # x29
    sw t5,  116(sp) # x30
    sw t6,  120(sp) # x31
    csrr t0, mepc   # pc
    sw t0, 124(sp)
    addi t0, sp, 132 #sp   
    sw t0, 128(sp)

# calling interrupt implementation
    mv a0, sp
    call interrupt_dispatcher
    la t0, next_ctx
    lw sp, 0(t0)
# restoring registers
    lw t0, 120(sp)
    csrw mepc, t0 # restoring pc
    lw ra,  0(sp)
    lw gp,  8(sp)
    lw tp,  12(sp)
    lw t0,  16(sp)
    lw t1,  20(sp)
    lw t2,  24(sp)
    lw s0,  28(sp)
    lw s1,  32(sp)
    lw a0,  36(sp)
    lw a1,  40(sp)
    lw a2,  44(sp)
    lw a3,  48(sp)
    lw a4,  52(sp)
    lw a5,  56(sp)
    lw a6,  60(sp)
    lw a7,  64(sp)
    lw s2,  68(sp)
    lw s3,  72(sp)
    lw s4,  76(sp)
    lw s5,  80(sp)
    lw s6,  84(sp)
    lw s7,  88(sp)
    lw s8,  92(sp)
    lw s9,  96(sp)
    lw s10, 100(sp)
    lw s11, 104(sp)
    lw t3,  108(sp)
    lw t4,  112(sp)
    lw t5,  116(sp)
    lw t6,  120(sp)
    addi sp, sp, 128
    mret

_panic_trap:
    addi sp, sp, -128
    sw ra,  0(sp)
    sw gp,  8(sp)   # x3
    sw tp,  12(sp)  # x4
    sw t0,  16(sp)  # x5
    sw t1,  20(sp)  # x6
    sw t2,  24(sp)  # x7
    sw s0,  28(sp)  # x8 / fp
    sw s1,  32(sp)  # x9
    sw a0,  36(sp)  # x10
    sw a1,  40(sp)  # x11
    sw a2,  44(sp)  # x12
    sw a3,  48(sp)  # x13
    sw a4,  52(sp)  # x14
    sw a5,  56(sp)  # x15
    sw a6,  60(sp)  # x16
    sw a7,  64(sp)  # x17
    sw s2,  68(sp)  # x18
    sw s3,  72(sp)  # x19
    sw s4,  76(sp)  # x20
    sw s5,  80(sp)  # x21
    sw s6,  84(sp)  # x22
    sw s7,  88(sp)  # x23
    sw s8,  92(sp)  # x24
    sw s9,  96(sp)  # x25
    sw s10, 100(sp) # x26
    sw s11, 104(sp) # x27
    sw t3,  108(sp) # x28
    sw t4,  112(sp) # x29
    sw t5,  116(sp) # x30
    sw t6,  120(sp) # x31
    csrr t0, mepc   # pc
    sw t0, 124(sp)
# calling interrupt implementation
    mv a0, sp
    call riscv_interrupts_panic
