#include "trap.h"

.section .text
.global trap_entry
.align 2

trap_entry:
    addi sp, sp, -TF_SIZE

    sw t0, TF_T0(sp)

    addi t0, sp, TF_SIZE
    sw t0, TF_SP(sp)

    sw ra,  TF_RA(sp)
    sw gp,  TF_GP(sp)
    sw tp,  TF_TP(sp)
    sw t1,  TF_T1(sp)
    sw t2,  TF_T2(sp)
    sw s0,  TF_S0(sp)
    sw s1,  TF_S1(sp)
    sw a0,  TF_A0(sp)
    sw a1,  TF_A1(sp)
    sw a2,  TF_A2(sp)
    sw a3,  TF_A3(sp)
    sw a4,  TF_A4(sp)
    sw a5,  TF_A5(sp)
    sw a6,  TF_A6(sp)
    sw a7,  TF_A7(sp)
    sw s2,  TF_S2(sp)
    sw s3,  TF_S3(sp)
    sw s4,  TF_S4(sp)
    sw s5,  TF_S5(sp)
    sw s6,  TF_S6(sp)
    sw s7,  TF_S7(sp)
    sw s8,  TF_S8(sp)
    sw s9,  TF_S9(sp)
    sw s10, TF_S10(sp)
    sw s11, TF_S11(sp)
    sw t3,  TF_T3(sp)
    sw t4,  TF_T4(sp)
    sw t5,  TF_T5(sp)
    sw t6,  TF_T6(sp)

    csrr t0, sstatus
    sw t0, TF_SSTATUS(sp)

    csrr t0, sepc
    sw t0, TF_SEPC(sp)

    csrr t0, stval
    sw t0, TF_STVAL(sp)

    csrr t0, scause
    sw t0, TF_SCAUSE(sp)

    mv a0, sp
    call handle_trap

    lw t0, TF_SSTATUS(sp)
    csrw sstatus, t0

    lw t0, TF_SEPC(sp)
    csrw sepc, t0

    lw ra,  TF_RA(sp)
    lw gp,  TF_GP(sp)
    lw tp,  TF_TP(sp)
    lw t1,  TF_T1(sp)
    lw t2,  TF_T2(sp)
    lw s0,  TF_S0(sp)
    lw s1,  TF_S1(sp)
    lw a0,  TF_A0(sp)
    lw a1,  TF_A1(sp)
    lw a2,  TF_A2(sp)
    lw a3,  TF_A3(sp)
    lw a4,  TF_A4(sp)
    lw a5,  TF_A5(sp)
    lw a6,  TF_A6(sp)
    lw a7,  TF_A7(sp)
    lw s2,  TF_S2(sp)
    lw s3,  TF_S3(sp)
    lw s4,  TF_S4(sp)
    lw s5,  TF_S5(sp)
    lw s6,  TF_S6(sp)
    lw s7,  TF_S7(sp)
    lw s8,  TF_S8(sp)
    lw s9,  TF_S9(sp)
    lw s10, TF_S10(sp)
    lw s11, TF_S11(sp)
    lw t3,  TF_T3(sp)
    lw t4,  TF_T4(sp)
    lw t5,  TF_T5(sp)
    lw t6,  TF_T6(sp)

    lw t0,  TF_T0(sp)

    addi sp, sp, TF_SIZE
    sret
