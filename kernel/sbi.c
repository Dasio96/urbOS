#include "sbi.h"

void sbi_putchar(char c) {
  register long a0 __asm__("a0") = c;
  register long a7 __asm__("a7") = 1;
  __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
}

void sbi_puts(const char *s) {
  while (*s)
    sbi_putchar(*s++);
}

void sbi_set_timer(u64 stime_value) {
  register long a0 __asm__("a0") = (u32)stime_value;
  register long a1 __asm__("a1") = (u32)(stime_value >> 32);
  register long a7 __asm__("a7") = 0;

  __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a7) : "memory");
}
