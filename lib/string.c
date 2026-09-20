#include "string.h"
#include "types.h"

void *memset(void *dst, int c, size_t n) {
    u8 *p = (u8 *)dst;
    for (size_t i = 0; i < n; i++) {
        p[i] = (u8)c;
    }
    return dst;
}

void *memcpy(void *dst, const void *src, size_t n) {
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dst;
}

size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}
