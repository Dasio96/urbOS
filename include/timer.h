#ifndef TIMER_H
#define TIMER_H

#include "types.h"

#define TIMER_INTERVAL 10000000

void timer_init(void);
void timer_handler(void);
u64 get_time(void);

#endif // !TIMER_H
