#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "list.h"

void initScheduler(List *processList);
void runScheduler();
void printAverageTurnaroundTime();

#endif // SCHEDULER_H
