.section .text.boot
    .globl _start
_start:
    mv s0, a0
    mv s1, a1

    la sp, __stack_top
    la t0, __bss_start
    la t1, __bss_end
1:  bgeu t0, t1, 2f
    sw zero, 0(t0)
    addi t0, t0, 4
    j 1b

2:
    mv a0, s0
    mv a1, s1
    call kmain
3:  wfi
    j 3b
