#include "proc.h"
#include "page.h"
#include "stdio.h"
#include "trap.h"
#include "vm.h"

static struct task tasks[MAX_TASKS];
static struct task *current_task = 0;
static u32 next_pid = 1;

void proc_init(void) {
  for (int i = 0; i < MAX_TASKS; i++) {
    tasks[i].pid = 0;
    tasks[i].state = TASK_UNUSED;
  }
  printf("process manager: initialized %d tasks\n", MAX_TASKS);
}

struct task *get_current_task(void) { return current_task; }

int task_create(void (*entry)(void)) {
  struct task *t = 0;

  for (int i = 0; i < MAX_TASKS; i++) {
    if (tasks[i].state == TASK_UNUSED) {
      t = &tasks[i];
      break;
    }
  }

  if (!t) {
    printf("task_create: no free task slots\n");
    return -1;
  }

  void *user_code_pa = alloc_page();
  void *user_stack_pa = alloc_page();

  u8 *src = (u8 *)entry;
  u8 *dst = (u8 *)user_code_pa;
  for (int i = 0; i < PAGE_SIZE; i++) {
    dst[i] = src[i];
  }

  u32 user_code_va = 0x00000000;

  t->page_table = create_user_page_table(user_code_va, (u32)user_code_pa,
                                         PAGE_SIZE, (u32)user_stack_pa);
  t->pid = next_pid++;

  __builtin_memset(&t->tf, 0, sizeof(t->tf));
  t->tf.sepc = user_code_va;
  t->tf.regs[1] = 0x70000000 + PAGE_SIZE;
  u32 sstatus;
  __asm__ __volatile__("csrr %0, sstatus" : "=r"(sstatus));
  sstatus &= ~(1 << 8);
  sstatus |= (1 << 5);
  sstatus &= ~(1 << 1);
  sstatus |= (1 << 18);
  t->tf.sstatus = sstatus;

  t->state = TASK_RUNNABLE;
  printf("task_create: created pid %d\n", t->pid);
  return t->pid;
}

void schedule(void) {
  int start_idx = 0;
  if (current_task) {
    start_idx = (int)(current_task - tasks) + 1;
    if (current_task->state == TASK_RUNNING) {
      current_task->state = TASK_RUNNABLE;
    }
  }

  struct task *next = 0;
  for (int i = 0; i < MAX_TASKS; i++) {
    int idx = (start_idx + i) % MAX_TASKS;
    if (tasks[idx].state == TASK_RUNNABLE) {
      next = &tasks[idx];
      break;
    }
  }

  if (!next) {
    if (current_task && current_task->state == TASK_RUNNING) {
      next = current_task;
    } else {
      printf("\n[SCHEDULER] all tasks finished\n");
      current_task = 0;

      __asm__ __volatile__("csrrs zero, sstatus, %0" : : "r"(1 << 1));

      while (1) {
        __asm__ __volatile__("wfi");
      }
    }
  }

  current_task = next;
  current_task->state = TASK_RUNNING;

  u32 satp_val = SATP_MODE_SV32 | (((u32)current_task->page_table) >> 12);
  __asm__ __volatile__("csrw satp, %0\n"
                       "sfence.vma\n"
                       :
                       : "r"(satp_val)
                       : "memory");

  trap_return(&current_task->tf);
}

void yield(void) { schedule(); }

void exit_task(int status) {
  if (current_task) {
    printf("\n[PROC] pid %d exited with status %d\n", current_task->pid,
           status);
    current_task->state = TASK_ZOMBIE;
  }
  schedule();
}
