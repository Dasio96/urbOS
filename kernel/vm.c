#include "vm.h"
#include "page.h"
#include "stdio.h"

static pde_t root_page_table = NULL;

static void map_page(pde_t root, u32 va, u32 pa, u32 flags) {
  u32 vpn1 = (va >> 22) & 0x3FF;
  u32 vpn0 = (va >> 12) & 0x3FF;

  if (!(root[vpn1] & PTE_V)) {
    void *new_pt = alloc_page();
    if (!new_pt) {
      panic("vm: failed to allocate page tavble");
    }

    u32 ptn = ((u32)new_pt >> 12);
    root[vpn1] = (ptn << 10) | PTE_V;
  }

  u32 *pt0 = (u32 *)((root[vpn1] >> 10) << 12);
  u32 ppn = (pa >> 12);
  pt0[vpn0] = (ppn << 10) | flags | PTE_V;
}

void kvm_init(void) {
  root_page_table = (pde_t)alloc_page();
  printf("kernel vm: root page table allocated at 0x%x\n",
         (u32)root_page_table);

  u32 ram_start = 0x80000000;
  u32 ram_end = 0x88000000;

  for (u32 addr = ram_start; addr < ram_end; addr += PAGE_SIZE) {
    map_page(root_page_table, addr, addr, PTE_R | PTE_W | PTE_X);
  }

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
