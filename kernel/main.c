#include "stdio.h"
#include "timer.h"
#include "trap.h"

void kmain(void) {
  printf("initializng traps \n");
  trap_init();

  printf("initializng timer \n");
  timer_init();

  printf("system ready");

  while (1) {
    __asm__ __volatile__("wfi");
  }
}
