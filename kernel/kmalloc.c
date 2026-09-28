#include "kmalloc.h"
#include "page.h"
#include "stdio.h"
#include "types.h"

#define ALIGN8(s) (((s) + 7) & ~7)

struct header {
  u32 size;
  u32 is_free;
  struct header *next;
};

#define HEADER_SIZE sizeof(struct header)

static struct header *heap_start = NULL;
static u32 heap_total_size = PAGE_SIZE * 4;
static void *heap_end = NULL;

void kmalloc_init(void) {
  void *p = alloc_page();
  void *last = p;

  for (int i = 1; i < 4; i++) {
    void *next = alloc_page();
    if ((u8 *)next != (u8 *)last + PAGE_SIZE) {
      panic("kmalloc: pages from page allocator are not contiguous");
    }
    last = next;
  }

  heap_start = (struct header *)p;
  heap_start->size = heap_total_size;
  heap_start->is_free = 1;
  heap_start->next = NULL;
  heap_end = (u8 *)heap_start + heap_total_size;

  printf("kernel heap: initialized at 0x%x, %d bytes\n", (u32)heap_start,
         heap_start->size);
}

void *kmalloc(u32 size) {
  if (size == 0)
    return NULL;

  u32 total_size = ALIGN8(size + HEADER_SIZE);

  if (total_size <= size) {
    return NULL;
  }

  struct header *curr = heap_start;

  while (curr) {
    if (curr->is_free && curr->size >= total_size) {
      if (curr->size >= total_size + HEADER_SIZE + 8) {
        struct header *next_block = (struct header *)((u8 *)curr + total_size);
        next_block->size = curr->size - total_size;
        next_block->is_free = 1;
        next_block->next = curr->next;

        curr->size = total_size;
        curr->next = next_block;
      }

      curr->is_free = 0;
      return (void *)((u8 *)curr + HEADER_SIZE);
    }
    curr = curr->next;
  }

  printf("kmalloc: out of memory (requested %d bytes)\n", size);
  return NULL;
}

void kfree(void *ptr) {
  if (!ptr)
    return;

  if (ptr < (void *)heap_start || ptr >= heap_end) {
    panic("kfree: invalid pointer");
  }

  struct header *block = (struct header *)((u8 *)ptr - HEADER_SIZE);

  if (block->is_free) {
    panic("kfree: double free detected");
  }

  block->is_free = 1;

  struct header *curr = heap_start;
  while (curr && curr->next) {
    if (curr->is_free && curr->next->is_free) {
      curr->size += curr->next->size;
      curr->next = curr->next->next;
    } else {
      curr = curr->next;
    }
  }
}

void kmalloc_test(void) {
  printf("kmalloc_test: running...\n");
  void *p1 = kmalloc(12);
  void *p2 = kmalloc(24);
  void *p3 = kmalloc(100);

  if (p1 && p2 && p3) {
    printf("kmalloc_test: allocations ok (p1=0x%x, p2=0x%x, p3=0x%x)\n",
           (u32)p1, (u32)p2, (u32)p3);
  } else {
    panic("kmalloc_test: allocations failed");
  }

  kfree(p2);
  printf("kmalloc_test: freed p2\n");

  kfree(p1);
  kfree(p3);
  printf("kmalloc_test: freed p1 and p3\n");

  if (heap_start->size == heap_total_size && heap_start->is_free) {
    printf("kmalloc_test: passed\n");
  } else {
    panic("kmalloc_test: heap is fragmented or leaked");
  }
}
