#ifndef SBI_H
#define SBI_H

#include "types.h"

void sbi_putchar(char c);

void sbi_puts(const char *s);

void sbi_set_timer(u64 stime_value);
#endif
