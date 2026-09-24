#include "page.h"
#include "stdio.h"
#include "timer.h"
#include "trap.h"
void kmain(void) {
  printf("initializng traps \n");
  trap_init();

  printf("initializng timer \n");
  timer_init();

  printf("initializng page allocator \n");
  page_init();

  void *p1 = alloc_page();
  void *p2 = alloc_page();

  printf("allocated page 1 at 0x%x\n", (u32)p1);
  printf("allocated page 2 at 0x%x\n", (u32)p2);

  free_page(p1);
  void *p3 = alloc_page();
  printf("allocated page 3 0x%x\n", (u32)p3);

  while (1) {
    __asm__ __volatile__("wfi");
  }
}
