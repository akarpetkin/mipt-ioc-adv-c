#include "scheduler.h"
#include <unistd.h>

void scheduler_init(Scheduler *scheduler) {
    scheduler->task_count = 0;
    scheduler->running = 0;
    for (int i = 0; i < MAX_SCHEDULER_TASK_COUNT; i++) {
        scheduler->tasks[i].enabled = 0;
        scheduler->tasks[i].last_execution = clock();
    }
}

void scheduler_add_task(Scheduler *scheduler, TaskFunction func, 
                       void *context, double interval) {
    if (scheduler->task_count < MAX_SCHEDULER_TASK_COUNT) {
        Task *task = &scheduler->tasks[scheduler->task_count++];
        task->function = func;
        task->context = context;
        task->interval = interval;
        task->enabled = 1;
        task->last_execution = clock();
    }
}

void scheduler_run(Scheduler *scheduler) {
    scheduler->running = 1;
    
    while (scheduler->running) {
        clock_t current_time = clock();
        
        for (int i = 0; i < scheduler->task_count; i++) {
            Task *task = &scheduler->tasks[i];
            
            if (task->enabled) {
                double elapsed = get_elapsed_time(task->last_execution);
                
                if (elapsed >= task->interval) {
                    task->function(task->context);
                    task->last_execution = current_time;
                }
            }
        }
        
        usleep(1000);
    }
}

void scheduler_stop(Scheduler *scheduler) {
    scheduler->running = 0;
}

double get_elapsed_time(clock_t last_time) {
    return (double)(clock() - last_time) / CLOCKS_PER_SEC;
}