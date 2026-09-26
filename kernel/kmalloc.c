#include "kmalloc.h"
#include "page.h"
#include "stdio.h"

#define ALIGN4(s) (((s) + 3) & ~3)

struct header {
  u32 size;
  u32 is_free;
  struct header *next;
};

#define HEADER_SIZE sizeof(struct header)

static struct header *heap_start = NULL;

void kmalloc_init(void) {
  void *p = alloc_page();
  for (int i = 0; i < 3; i++) {
    alloc_page();
  }

  heap_start = (struct header *)p;
  heap_start->size = PAGE_SIZE * 4;
  heap_start->is_free = 1;
  heap_start->next = NULL;

  printf("kernel heap: initialized at 0x%x, %d bytes\n", (u32)heap_start,
         heap_start->size);
}

void *kmalloc(u32 size) {
  if (size == 0)
    return NULL;

  u32 total_size = ALIGN4(size + HEADER_SIZE);
  struct header *curr = heap_start;

  while (curr) {
    if (curr->is_free && curr->size >= total_size) {
      if (curr->size >= total_size + HEADER_SIZE + 4) {
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

  panic("kmalloc: out of heap memory");
  return NULL;
}

void kfree(void *ptr) {
  if (!ptr)
    return;

  struct header *block = (struct header *)((u8 *)ptr - HEADER_SIZE);
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
