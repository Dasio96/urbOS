#include "vm.h"
#include "memlayout.h"
#include "page.h"
#include "stdio.h"

/* Symbole importowane z linker scriptu (kernel.ld) */
extern char __text_start[];
extern char __text_end[];
extern char __rodata_start[];
extern char __rodata_end[];
extern char __data_start[];

static pde_t root_page_table = NULL;

static void map_page(pde_t root, u32 va, u32 pa, u32 flags) {
  u32 vpn1 = (va >> 22) & 0x3FF;
  u32 vpn0 = (va >> 12) & 0x3FF;

  if (!(root[vpn1] & PTE_V)) {
    void *new_pt = alloc_page();
    u32 ptn = ((u32)new_pt >> 12);
    root[vpn1] = (ptn << 10) | PTE_V;
  }

  u32 *pt0 = (u32 *)((root[vpn1] >> 10) << 12);
  u32 ppn = (pa >> 12);

  u32 final_flags = flags | PTE_A;
  if (flags & PTE_W) {
    final_flags |= PTE_D;
  }

  pt0[vpn0] = (ppn << 10) | final_flags | PTE_V;
}

static void map_region(pde_t root, u32 start, u32 end, u32 flags) {
  start = start & ~(PAGE_SIZE - 1);
  end = (end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

  for (u32 addr = start; addr < end; addr += PAGE_SIZE) {
    map_page(root, addr, addr, flags);
  }
}

void kvm_init(void) {
  root_page_table = (pde_t)alloc_page();
  printf("kernel vm: root page table allocated at 0x%x\n",
         (u32)root_page_table);

  u32 text_start = (u32)__text_start;
  u32 text_end = (u32)__text_end;
  u32 rodata_start = (u32)__rodata_start;
  u32 rodata_end = (u32)__rodata_end;
  u32 data_start = (u32)__data_start;
  u32 ram_end = RAM_END;

  printf("kernel vm: mapping .text   [0x%x - 0x%x] (R-X)\n", text_start,
         text_end);
  map_region(root_page_table, text_start, text_end, PTE_R | PTE_X);

  printf("kernel vm: mapping .rodata [0x%x - 0x%x] (R--)\n", rodata_start,
         rodata_end);
  map_region(root_page_table, rodata_start, rodata_end, PTE_R);

  printf("kernel vm: mapping data+   [0x%x - 0x%x] (RW-)\n", data_start,
         ram_end);
  map_region(root_page_table, data_start, ram_end, PTE_R | PTE_W);

  u32 root_ppn = ((u32)root_page_table) >> 12;
  u32 satp_val = SATP_MODE_SV32 | root_ppn;
  printf("kernel vm: enabling sv32 mmu 0x%x\n", satp_val);

  __asm__ __volatile__("csrw satp, %0\n"
                       "sfence.vma\n"
                       :
                       : "r"(satp_val)
                       : "memory");

  printf("kernel vm: paging enabled successfully\n");
}

pde_t create_user_page_table(u32 user_code_va, u32 user_code_pa, u32 code_size,
                             u32 user_stack_pa) {
  pde_t user_root = (pde_t)alloc_page();

  for (int i = 0; i < 1024; i++) {
    user_root[i] = root_page_table[i];
  }

  for (u32 offset = 0; offset < code_size; offset += PAGE_SIZE) {
    map_page(user_root, user_code_va + offset, user_code_pa + offset,
             PTE_U | PTE_R | PTE_X | PTE_W);
  }

  u32 user_stack_va = 0x70000000;
  map_page(user_root, user_stack_va, user_stack_pa, PTE_U | PTE_R | PTE_W);

  return user_root;
}
