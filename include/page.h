#ifndef PAGE_H
#define PAGE_H

#include "types.h"

#define PAGE_SIZE 4096

void page_init(void);
void *alloc_page(void);
void free_page(void *p);

#endif // !PAGE_H
