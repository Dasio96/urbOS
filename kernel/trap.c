#include "trap.h"
#include "stdio.h"
#include "timer.h"
#include "types.h"

extern void trap_entry(void);

void trap_init(void) {
  __asm__ __volatile__("csrw stvec, %0" : : "r"(trap_entry));
}

static void dump_registers(struct trap_frame *tf) {
  printf("  ra: 0x%x   sp: 0x%x   gp: 0x%x   tp: 0x%x\n", tf->regs[0],
         tf->regs[1], tf->regs[2], tf->regs[3]);

  printf("  t0: 0x%x   t1: 0x%x   t2: 0x%x   s0: 0x%x\n", tf->regs[4],
         tf->regs[5], tf->regs[6], tf->regs[7]);

  printf("  a0: 0x%x   a1: 0x%x   a2: 0x%x   a3: 0x%x\n", tf->regs[9],
         tf->regs[10], tf->regs[11], tf->regs[12]);

  printf("  a4: 0x%x   a5: 0x%x   a6: 0x%x   a7: 0x%x\n", tf->regs[13],
         tf->regs[14], tf->regs[15], tf->regs[16]);

  printf("  sepc: 0x%x   stval: 0x%x   scause: 0x%x\n", tf->sepc, tf->stval,
         tf->scause);
}

void handle_trap(struct trap_frame *tf) {
  u32 is_interrupt = (tf->scause & 0x80000000) != 0;
  u32 code = tf->scause & 0x7fffffff;

  if (is_interrupt) {
    if (code == 5) {
      timer_handler();
    } else {
      printf("[TRAP] unknown interrupt code %d\n", code);
    }
  } else {
    if (code == 8) {
      u32 syscall_id = tf->regs[16];
      if (syscall_id == 1) {
        char c = (char)tf->regs[9];
        printf("%c", c);
      } else if (syscall_id == 2) {
        register long a0 __asm__("a0");
        register long a7 __asm__("a7") = 2;
        __asm__ volatile("ecall" : "=r"(a0) : "r"(a7) : "memory");
        tf->regs[9] = a0;
      } else if (syscall_id == 3) {
        const char *s = (const char *)tf->regs[9];
        printf("%s", s);
      } else {
        printf("[TRAP] unknown syscall id %d\n", syscall_id);
      }

      tf->sepc += 4;
      return;
    }

    if (code == 3) {
      u16 insn = *(u16 *)tf->sepc;
      int insn_len = ((insn & 0x3) == 0x3) ? 4 : 2;

      printf("handling ebreak %d\n", insn_len);
      tf->sepc += insn_len;
      return;
    }

    printf("\n[TRAP] %d at sepc = 0x%x stval = 0x%x\n", code, tf->sepc,
           tf->stval);
    dump_registers(tf);
    panic("unhandled kernel exception");
  }
}
