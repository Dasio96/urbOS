#include "page.h"
#include "stdio.h"

extern char _end[];

#define RAM_END 0x88000000

struct page {
  struct page *next;
};

static struct page *free_list = NULL;

void free_page(void *p) {
  if (!p)
    return;

  struct page *page = (struct page *)p;
  page->next = free_list;
  free_list = page;
}

void page_init(void) {
  u32 start = (u32)_end;
  u32 end = RAM_END;

  start = (start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

  printf("Page allocator: initializing from 0x%x to 0x%x\n", start, end);

  for (u32 p = start; p + PAGE_SIZE <= end; p += PAGE_SIZE) {
    free_page((void *)p);
  }
}

void *alloc_page(void) {
  if (!free_list)
    panic("out of physical memory");

  struct page *p = free_list;
  free_list = free_list->next;

  u8 *ptr = (u8 *)p;
  for (int i = 0; i < PAGE_SIZE - 1; i++) {
    ptr[i] = 0;
  }

  return (void *)p;
}
