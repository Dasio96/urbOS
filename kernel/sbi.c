#include "sbi.h"

void sbi_putchar(char c) {
  register long a0 __asm__("a0") = c;
  register long a7 __asm__("a7") = 1; /* legacy: console putchar */
  __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
}

void sbi_puts(const char *s) {
  while (*s)
    sbi_putchar(*s++);
}
