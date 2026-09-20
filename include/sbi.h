#ifndef SBI_H
#define SBI_H

/* Wypisz jeden znak przez SBI (legacy console putchar). */
void sbi_putchar(char c);

/* Wypisz napis zakończony zerem. */
void sbi_puts(const char *s);

#endif
