#include <stdarg.h>
#include "stdio.h"
#include "sbi.h"
#include "types.h"

void putc(char c) {
    sbi_putchar(c);
}

void puts(const char *s) {
    sbi_puts(s);
}

static void print_num(u32 num, int base, int is_signed) {
    char buf[32];
    char digits[] = "0123456789abcdef";
    int i = 0;

    if (is_signed && (i32)num < 0) {
        putc('-');
        num = (u32)(-(i32)num);
    }

    if (num == 0) {
        putc('0');
        return;
    }

    while (num > 0) {
        buf[i++] = digits[num % base];
        num /= base;
    }

    while (i > 0) {
        putc(buf[--i]);
    }
}

int vprintf(const char *fmt, va_list ap) {
    int count = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'c': {
                    char c = (char)va_arg(ap, int);
                    putc(c);
                    count++;
                    break;
                }
                case 's': {
                    const char *s = va_arg(ap, const char *);
                    if (!s) s = "(null)";
                    while (*s) {
                        putc(*s++);
                        count++;
                    }
                    break;
                }
                case 'd':
                case 'i': {
                    i32 val = va_arg(ap, i32);
                    print_num((u32)val, 10, 1);
                    break;
                }
                case 'u': {
                    u32 val = va_arg(ap, u32);
                    print_num(val, 10, 0);
                    break;
                }
                case 'x':
                case 'p': {
                    u32 val = va_arg(ap, u32);
                    print_num(val, 16, 0);
                    break;
                }
                case '%': {
                    putc('%');
                    count++;
                    break;
                }
                default: {
                    putc('%');
                    putc(*fmt);
                    count += 2;
                    break;
                }
            }
        } else {
            putc(*fmt);
            count++;
        }
        fmt++;
    }

    return count;
}

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int result = vprintf(fmt, ap);
    va_end(ap);
    return result;
}

void panic(const char *msg) {
    puts("\n!!! KERNEL PANIC !!!\n");
    puts(msg);
    puts("\nSystem halted.\n");

    while (1) {
        __asm__ __volatile__("wfi");
    }
}
