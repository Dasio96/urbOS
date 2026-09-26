#include "kmalloc.h"
#include "page.h"
#include "proc.h"
#include "stdio.h"
#include "timer.h"
#include "trap.h"
#include "vm.h"

void worker_a(void) {
  while (1) {
    printf("task a ");
    for (volatile int i = 0; i < 5000000; i++)
      ;
    yield();
  }
}

void worker_b(void) {
  while (1) {
    printf("task b ");
    for (volatile int i = 0; i < 5000000; i++)
      ;
    yield();
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

  task_create(worker_a);
  task_create(worker_b);

  while (1) {
    yield();
  }
}
