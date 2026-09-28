#ifndef PROC_H
#define PROC_H

#include "trap.h"
#include "types.h"
#include "vm.h"

#define MAX_TASKS 5

enum task_state {
  TASK_UNUSED = 0,
  TASK_RUNNABLE,
  TASK_RUNNING,
  TASK_ZOMBIE,
};

struct task {
  enum task_state state;
  u32 pid;
  pde_t page_table;
  struct trap_frame tf;
};

void proc_init(void);
int task_create(void (*entry)(void));
void schedule(void);
void yield(void);
void exit_task(int status);
struct task *get_current_task(void);

#endif // !PROC_H
