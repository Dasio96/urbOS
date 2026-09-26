#ifndef PROC_H
#define PROC_H

#include "types.h"

#define MAX_TASKS 5
#define STACK_SIZE 4096

enum task_state {
  TASK_UNUSED = 0,
  TASK_RUNNABLE,
  TASK_RUNNING,
};

struct context {
  u32 ra;
  u32 sp;
  u32 s0;
  u32 s1;
  u32 s2;
  u32 s3;
  u32 s4;
  u32 s5;
  u32 s6;
  u32 s7;
  u32 s8;
  u32 s9;
  u32 s10;
  u32 s11;
};

struct task {
  enum task_state state;
  u32 pid;
  struct context context;
  u8 stack[STACK_SIZE];
};

void proc_init(void);
int task_create(void (*entry)(void));
void schedule(void);
void yield(void);

#endif // !PROC_H
