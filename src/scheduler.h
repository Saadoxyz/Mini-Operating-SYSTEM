// scheduler.h - Process scheduler interface
#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_PROCESSES 64

// Process states
typedef enum {
    PROC_NEW,
    PROC_READY,
    PROC_RUNNING,
    PROC_WAITING,
    PROC_TERMINATED
} proc_state_t;

// Scheduling modes
typedef enum {
    SCHED_FCFS = 0,
    SCHED_RR = 1,
    SCHED_PRIORITY = 2
} sched_mode_t;

// Process Control Block
typedef struct {
    int pid;
    proc_state_t state;
    int priority;
    int burst_time;
    int remaining_time;
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    int time_slice;
} pcb_t;

// Scheduler functions
void scheduler_init();
void scheduler_set_mode(sched_mode_t mode);
void scheduler_set_quantum(int quantum);
int scheduler_create_process(int burst, int priority);
int scheduler_kill_process(int pid);
void scheduler_tick();
void scheduler_list_processes();
pcb_t* scheduler_get_process(int pid);

#endif
