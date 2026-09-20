#include "sbi.h"

void kmain(void) {
  sbi_puts("URBAN!\n");

  for (;;)
    __asm__ volatile("wfi");
}
