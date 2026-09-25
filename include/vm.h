#ifndef VM_H
#define VM_H

#include "types.h"

#define PTE_V (1 << 0)
#define PTE_R (1 << 1)
#define PTE_W (1 << 2)
#define PTE_X (1 << 3)
#define PTE_U (1 << 4)
#define PTE_G (1 << 5)
#define PTE_A (1 << 6)
#define PTE_D (1 << 7)

#define SATP_MODE_SV32 (1U << 31)

typedef u32 pte_t;
typedef u32 *pde_t;

void kvm_init(void);

#endif // !VM_H
