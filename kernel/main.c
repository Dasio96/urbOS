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
  while (*s) {
    uputchar(*s++);
  }
}

void user_program(void) {
  char prompt[16];

  prompt[0] = '\n';
  prompt[1] = 'u';
  prompt[2] = 'r';
  prompt[3] = 'b';
  prompt[4] = 'O';
  prompt[5] = 'S';
  prompt[6] = ' ';
  prompt[7] = 's';
  prompt[8] = 'h';
  prompt[9] = 'e';
  prompt[10] = 'l';
  prompt[11] = 'l';
  prompt[12] = '>';
  prompt[13] = ' ';
  prompt[14] = '\0';

  uputs(prompt);

  while (1) {
    int ch = ugetchar();
    if (ch != -1 && ch != 0) {
      uputchar((char)ch);
      if ((char)ch == '\r' || (char)ch == '\n') {
        uputs(prompt);
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
