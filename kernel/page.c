#include "page.h"
#include "memlayout.h"
#include "stdio.h"
#include "string.h"

extern char _end[];

struct page {
  struct page *next;
};

static struct page *free_list = NULL;
static u32 ram_start;
static u32 ram_end;

void free_page(void *p) {
  if (!p)
    return;

  u32 addr = (u32)p;

  if (addr < ram_start || addr >= ram_end)
    panic("free_page: out of bounds address");

  if (addr & (PAGE_SIZE - 1))
    panic("free_page: unaligned address");

  if (free_list == (struct page *)p) {
    panic("free_page: double free detected");
  }

  struct page *page = (struct page *)p;
  page->next = free_list;
  free_list = page;
}

void page_init(void) {
  ram_start = (u32)_end;
  ram_end = RAM_END;

  ram_start = (ram_start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

  printf("page allocator: initializing from 0x%x to 0x%x\n", ram_start,
         ram_end);

  for (u32 p = ram_end - PAGE_SIZE; p >= ram_start; p -= PAGE_SIZE) {
    free_page((void *)p);
  }
}

void *alloc_page(void) {
  if (!free_list)
    panic("out of physical memory");

  struct page *p = free_list;
  free_list = free_list->next;

  memset(p, 0, PAGE_SIZE);

  return (void *)p;
}
