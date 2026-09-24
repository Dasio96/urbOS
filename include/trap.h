#ifndef TRAP_H
#define TRAP_H

#define REGBYTES 4

#define TF_RA (0 * REGBYTES)
#define TF_SP (1 * REGBYTES)
#define TF_GP (2 * REGBYTES)
#define TF_TP (3 * REGBYTES)
#define TF_T0 (4 * REGBYTES)
#define TF_T1 (5 * REGBYTES)
#define TF_T2 (6 * REGBYTES)
#define TF_S0 (7 * REGBYTES)
#define TF_S1 (8 * REGBYTES)
#define TF_A0 (9 * REGBYTES)
#define TF_A1 (10 * REGBYTES)
#define TF_A2 (11 * REGBYTES)
#define TF_A3 (12 * REGBYTES)
#define TF_A4 (13 * REGBYTES)
#define TF_A5 (14 * REGBYTES)
#define TF_A6 (15 * REGBYTES)
#define TF_A7 (16 * REGBYTES)
#define TF_S2 (17 * REGBYTES)
#define TF_S3 (18 * REGBYTES)
#define TF_S4 (19 * REGBYTES)
#define TF_S5 (20 * REGBYTES)
#define TF_S6 (21 * REGBYTES)
#define TF_S7 (22 * REGBYTES)
#define TF_S8 (23 * REGBYTES)
#define TF_S9 (24 * REGBYTES)
#define TF_S10 (25 * REGBYTES)
#define TF_S11 (26 * REGBYTES)
#define TF_T3 (27 * REGBYTES)
#define TF_T4 (28 * REGBYTES)
#define TF_T5 (29 * REGBYTES)
#define TF_T6 (30 * REGBYTES)

#define TF_SSTATUS (31 * REGBYTES)
#define TF_SEPC (32 * REGBYTES)
#define TF_STVAL (33 * REGBYTES)
#define TF_SCAUSE (34 * REGBYTES)

#define TF_SIZE 144

#ifndef __ASSEMBLER__

#include "types.h"

struct trap_frame {
  u32 ra;
  u32 sp;
  u32 gp;
  u32 tp;
  u32 t0;
  u32 t1;
  u32 t2;
  u32 s0;
  u32 s1;
  u32 a0;
  u32 a1;
  u32 a2;
  u32 a3;
  u32 a4;
  u32 a5;
  u32 a6;
  u32 a7;
  u32 s2;
  u32 s3;
  u32 s4;
  u32 s5;
  u32 s6;
  u32 s7;
  u32 s8;
  u32 s9;
  u32 s10;
  u32 s11;
  u32 t3;
  u32 t4;
  u32 t5;
  u32 t6;
  u32 sstatus;
  u32 sepc;
  u32 stval;
  u32 scause;
};

void trap_init(void);
void handle_trap(struct trap_frame *tf);

#endif // !__ASSEMBLER__

#endif // !TRAP_H
