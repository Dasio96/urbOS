#include "timer.h"
#include "sbi.h"
#include "stdio.h"

static u64 ticks = 0;

u64 get_time(void) {
  u32 high, low, check;
  do {
    __asm__ __volatile__("csrr %0, timeh" : "=r"(high));
    __asm__ __volatile__("csrr %0, time" : "=r"(low));
    __asm__ __volatile__("csrr %0, timeh" : "=r"(check));
  } while (high != check);

  return ((u64)high << 32) | low;
}

void timer_init(void) {
  u64 now = get_time();

  sbi_set_timer(now + 1000000);

  u32 sie;
  __asm__ __volatile__("csrr %0, sie" : "=r"(sie));
  sie |= (1 << 5);
  __asm__ __volatile__("csrw sie, %0" : : "r"(sie));

  u32 sstatus;
  __asm__ __volatile__("csrr %0, sstatus" : "=r"(sstatus));
  sstatus |= (1 << 1);
  __asm__ __volatile__("csrw sstatus, %0" : : "r"(sstatus));

  u32 sie_check, sstatus_check;
  __asm__ __volatile__("csrr %0, sie" : "=r"(sie_check));
  __asm__ __volatile__("csrr %0, sstatus" : "=r"(sstatus_check));
  printf("timer initialized. sie=0x%x sstatus=0x%x time=%u\n", sie_check,
         sstatus_check, (u32)now);
}

void timer_handler(void) {
  ticks++;
  printf("[TIMER] tick %u\n", (u32)ticks);

  u64 now = get_time();
  sbi_set_timer(now + 1000000);
}
