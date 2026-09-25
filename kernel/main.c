#include "page.h"
#include "stdio.h"
#include "timer.h"
#include "trap.h"
#include "vm.h"
void kmain(void) {
  printf("initializng traps \n");
  trap_init();

  printf("initializng timer \n");
  timer_init();

  printf("initializng page allocator \n");
  page_init();

  printf("initializng virutal memory \n");
  kvm_init();
  printf("system running with mmu enabled \n");

  while (1) {
    __asm__ __volatile__("wfi");
  }
}
