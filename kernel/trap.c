#include "trap.h"
#include "stdio.h"
#include "types.h"

extern void trap_entry(void);

void trap_init(void) {
  __asm__ __volatile__("csrw stvec, %0" : : "r"(trap_entry));
}

static void dump_registers(struct trap_frame *tf) {
  printf("  ra: 0x%x   sp: 0x%x   gp: 0x%x   tp: 0x%x\n", tf->ra, tf->sp,
         tf->gp, tf->tp);
  printf("  t0: 0x%x   t1: 0x%x   t2: 0x%x   s0: 0x%x\n", tf->t0, tf->t1,
         tf->t2, tf->s0);
  printf("  a0: 0x%x   a1: 0x%x   a2: 0x%x   a3: 0x%x\n", tf->a0, tf->a1,
         tf->a2, tf->a3);
  printf("  a4: 0x%x   a5: 0x%x   a6: 0x%x   a7: 0x%x\n", tf->a4, tf->a5,
         tf->a6, tf->a7);
  printf("  sepc: 0x%x   stval: 0x%x   scause: 0x%x\n", tf->sepc, tf->stval,
         tf->scause);
}

void handle_trap(struct trap_frame *tf) {
  u32 is_interrupt = (tf->scause & 0x80000000) != 0;
  u32 code = tf->scause & 0x7fffffff;

  if (is_interrupt)
    printf("[TRAP] interrupt code %d\n", code);
  else {
    printf("\n[TRAP] %d at sepc = 0x%x stval = 0x%x\n", code, tf->sepc,
           tf->stval);
    dump_registers(tf);

    if (code == 3) {
      u16 insn = *(u16 *)tf->sepc;
      int insn_len = ((insn & 0x3) == 0x3) ? 4 : 2;

      printf("handling ebreak %d\n", insn_len);
      tf->sepc += insn_len;
      return;
    }

    panic("Unhandled kernel exception!");
  }
}
