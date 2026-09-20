#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

void putc(char c);
void puts(const char *s);
int vprintf(const char *fmt, va_list ap);
int printf(const char *fmt, ...);
void panic(const char *msg) __attribute__((noreturn));

#endif
