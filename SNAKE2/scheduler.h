#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <time.h>

#define MAX_SCHEDULER_TASK_COUNT 10

typedef void (*TaskFunction)(void *context);

typedef struct {
    TaskFunction function;
    void *context;
    double interval;
    clock_t last_execution;
    int enabled;
} Task;

typedef struct {
    Task tasks[MAX_SCHEDULER_TASK_COUNT];
    int task_count;
    int running;
} Scheduler;

void scheduler_init(Scheduler *scheduler);
void scheduler_add_task(Scheduler *scheduler, TaskFunction func, 
                       void *context, double interval);
void scheduler_run(Scheduler *scheduler);
void scheduler_stop(Scheduler *scheduler);

double get_elapsed_time(clock_t last_time);

#endif