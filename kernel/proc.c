#include "proc.h"
#include "stdio.h"

extern void cpu_switch_to(struct context *old, struct context *next);

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

  int start_idx = (old == &main_task) ?: (old - tasks + 1);

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
