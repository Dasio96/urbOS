#include "kmalloc.h"
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

  printf("initializng kernel heap \n");
  kmalloc_init();

  u32 *a = (u32 *)kmalloc(sizeof(u32));
  u8 *b = (u8 *)kmalloc(100);
  printf("Allocated 'a' (4 B) at: 0x%x\n", (u32)a);
  printf("Allocated 'b' (100 B) at: 0x%x\n", (u32)b);

  *a = 0xDEADBEEF;
  printf("value at 'a': 0x%x\n", *a);

  kfree(a);
  u32 *c = (u32 *)kmalloc(sizeof(u32));
  printf("Allocated 'c' after freeing 'a': 0x%x\n", (u32)c);

  while (1) {
    __asm__ __volatile__("wfi");
  }
}
