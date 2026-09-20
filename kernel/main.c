#include "sbi.h"
#include "stdio.h"

void kmain(void) {
  sbi_puts("URBAN!\n");
  printf("ohio");

  for (;;)
    __asm__ volatile("wfi");
}
