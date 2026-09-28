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

void *memmove(void *dst, const void *src, size_t n) {
  u8 *d = (u8 *)dst;
  const u8 *s = (const u8 *)src;
  if (d < s) {
    for (size_t i = 0; i < n; i++) {
      d[i] = s[i];
    }
  } else if (d > s) {
    for (size_t i = n; i > 0; i--) {
      d[i - 1] = s[i - 1];
    }
  }
  return dst;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const u8 *p1 = (const u8 *)s1;
  const u8 *p2 = (const u8 *)s2;
  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] - p2[i];
    }
  }
  return 0;
}

size_t strlen(const char *s) {
  size_t len = 0;
  while (s[len] != '\0') {
    len++;
  }
  return len;
}
