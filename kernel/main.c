#include "kmalloc.h"
#include "page.h"
#include "proc.h"
#include "stdio.h"
#include "timer.h"
#include "trap.h"
#include "vm.h"

void uputchar(char c) {
  register long a0 __asm__("a0") = c;
  register long a7 __asm__("a7") = 1;
  __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7) : "memory");
}

int ugetchar(void) {
  register long a0 __asm__("a0");
  register long a7 __asm__("a7") = 2;
  __asm__ __volatile__("ecall" : "=r"(a0) : "r"(a7) : "memory");
  return (int)a0;
}

void uputs(const char *s) {
  register long a0 __asm__("a0") = (long)s;
  register long a7 __asm__("a7") = 3;
  __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7) : "memory");
}

static void print_prompt(void) { uputs("\nurbOS shell> "); }

static void print_help(void) {
  uputs("\navailable commands:\n");
  uputs("  help  - displays this list\n");
  uputs("  ping  - responds with pong\n");
  uputs("  clear - clears the terminal\n");
  uputs("  echo  - prints the provided text\n");
}

static void execute_command(char *buf, int len) {
  if (len == 0)
    return;

  if (len == 4 && buf[0] == 'h' && buf[1] == 'e' && buf[2] == 'l' &&
      buf[3] == 'p') {
    print_help();
  } else if (len == 4 && buf[0] == 'p' && buf[1] == 'i' && buf[2] == 'n' &&
             buf[3] == 'g') {
    uputs("\npong\n");
  } else if (len == 5 && buf[0] == 'c' && buf[1] == 'l' && buf[2] == 'e' &&
             buf[3] == 'a' && buf[4] == 'r') {
    uputchar('\033');
    uputchar('[');
    uputchar('2');
    uputchar('J');
    uputchar('\033');
    uputchar('[');
    uputchar('H');
  } else if (len >= 4 && buf[0] == 'e' && buf[1] == 'c' && buf[2] == 'h' &&
             buf[3] == 'o') {
    uputchar('\n');
    if (len > 5 && buf[4] == ' ') {
      uputs(&buf[5]);
    }
    uputchar('\n');
  } else {
    uputs("\nunknown command type help\n");
  }
}

void user_program(void) {
  char buf[64];
  int len = 0;

  uputs("urbOS shell\n");
  print_prompt();

  while (1) {
    int ch = ugetchar();

    if (ch == -1 || ch == 0) {
      continue;
    }

    if (ch == '\r' || ch == '\n') {
      buf[len] = '\0';
      execute_command(buf, len);
      len = 0;
      print_prompt();
    } else if (ch == 8 || ch == 127) {
      if (len > 0) {
        len--;
        uputchar('\b');
        uputchar(' ');
        uputchar('\b');
      }
    } else if (ch >= 32 && ch <= 126) {
      if (len < 63) {
        buf[len++] = (char)ch;
        uputchar((char)ch);
      }
    }
  }
}

void kmain(void) {
  printf("initializng traps \n");
  trap_init();

  printf("initializng timer \n");
  timer_init();

  printf("initializng page allocator \n");
  page_init();

  printf("initializng virutal memory \n");
  kvm_init();

  printf("initializng kernel heap \n");
  kmalloc_init();

  printf("initializng processes \n");
  proc_init();

  printf("launching user process \n");
  run_user_process(user_program);

  while (1) {
    yield();
  }
}
