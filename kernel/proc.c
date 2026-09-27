#include "proc.h"
#include "page.h"
#include "stdio.h"
#include "trap.h"
#include "vm.h"

extern void cpu_switch_to(struct context *old, struct context *next);
extern void trap_return(struct trap_frame *tf);

static struct task tasks[MAX_TASKS];
static struct task *current_task = NULL;
static int current_pid = 0;
static struct task main_task;

void proc_init(void) {
  for (int i = 0; i < MAX_TASKS; i++) {
    tasks[i].state = TASK_UNUSED;
    tasks[i].pid = 0;
  }

  main_task.state = TASK_RUNNING;
  main_task.pid = 0;
  current_task = &main_task;
}

int task_create(void (*entry)(void)) {
  struct task *t = NULL;
  for (int i = 0; i < MAX_TASKS; i++) {
    if (tasks[i].state == TASK_UNUSED) {
      t = &tasks[i];
      break;
    }
  }

  if (!t) {
    printf("task_create: no free tasks slots\n");
    return -1;
  }

  t->pid = ++current_pid;
  t->state = TASK_RUNNABLE;

  t->context.ra = (u32)entry;
  t->context.sp = (u32)&t->stack[STACK_SIZE];

  printf("process manager: created task pid %d\n", t->pid);
  return t->pid;
}

void schedule(void) {
  struct task *old = current_task;
  struct task *next = NULL;

  int start_idx = (old == &main_task) ? 0 : (old - tasks + 1);

  for (int i = 0; i < MAX_TASKS; i++) {
    int idx = (start_idx + i) % MAX_TASKS;
    if (tasks[idx].state == TASK_RUNNABLE) {
      next = &tasks[idx];
      break;
    }
  }

  if (!next || next == old)
    return;

  if (old->state == TASK_RUNNING)
    old->state = TASK_RUNNABLE;

  next->state = TASK_RUNNING;
  current_task = next;

  cpu_switch_to(&old->context, &next->context);
}

void yield(void) { schedule(); }

void run_user_process(void (*user_code)(void)) {
  void *user_stack_pa = alloc_page();
  void *user_code_pa = alloc_page();

  u8 *src = (u8 *)user_code;
  u8 *dst = (u8 *)user_code_pa;
  for (int i = 0; i < 256; i++) {
    dst[i] = src[i];
  }

  u32 user_code_va = 0x00000000;

  pde_t user_pt = create_user_page_table(user_code_va, (u32)user_code_pa,
                                         PAGE_SIZE, (u32)user_stack_pa);

  u32 satp_val = SATP_MODE_SV32 | (((u32)user_pt) >> 12);
  __asm__ __volatile__("csrw satp, %0\n"
                       "sfence.vma\n"
                       :
                       : "r"(satp_val)
                       : "memory");

  struct trap_frame tf;
  __builtin_memset(&tf, 0, sizeof(tf));

  tf.sepc = user_code_va;
  tf.regs[1] = 0x70000000 + PAGE_SIZE;
  u32 sstatus;
  __asm__ __volatile__("csrr %0, sstatus" : "=r"(sstatus));
  sstatus &= ~(1 << 8);
  sstatus |= (1 << 5);
  sstatus &= ~(1 << 1);
  sstatus |= (1 << 18);
  tf.sstatus = sstatus;

  printf("kernel: switching to user mode at sepc=0x%x, sp=0x%x...\n", tf.sepc,
         tf.regs[1]);

  __asm__ __volatile__("csrc sstatus, %0" : : "r"(1 << 1));

  trap_return(&tf);
}
