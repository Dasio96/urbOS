#include "stdio.h"
#include "trap.h"

void kmain(void) {
  printf("initializng traps \n");
  trap_init();

  printf("ebreak trap");
  __asm__ __volatile__("ebreak");

  printf("successfully resumed execution after trap\n");

  while (1) {
    __asm__ __volatile__("wfi");
  }
}
