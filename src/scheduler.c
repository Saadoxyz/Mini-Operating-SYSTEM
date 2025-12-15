// scheduler.c - CPU scheduler implementation
#include "kernel.h"
#include "scheduler.h"

static pcb_t process_table[MAX_PROCESSES];
static int next_pid = 1;
static int current_pid = -1;
static sched_mode_t sched_mode = SCHED_FCFS;
static int time_quantum = 4;
static int current_tick = 0;

// Initialize scheduler
void scheduler_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = PROC_TERMINATED;
    }
    current_tick = 0;
}

// Set scheduling mode
void scheduler_set_mode(sched_mode_t mode) {
    sched_mode = mode;
}

// Set time quantum for RR
void scheduler_set_quantum(int quantum) {
    time_quantum = quantum;
}

// Create new process
int scheduler_create_process(int burst, int priority) {
    // Find free slot
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == -1) {
            process_table[i].pid = next_pid++;
            process_table[i].state = PROC_READY;
            process_table[i].priority = priority;
            process_table[i].burst_time = burst;
            process_table[i].remaining_time = burst;
            process_table[i].arrival_time = current_tick;
            process_table[i].waiting_time = 0;
            process_table[i].turnaround_time = 0;
            process_table[i].time_slice = time_quantum;
            
            return process_table[i].pid;
        }
    }
    return -1;  // No free slot
}

// Kill process
int scheduler_kill_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid) {
            process_table[i].state = PROC_TERMINATED;
            process_table[i].pid = -1;
            if (current_pid == pid) {
                current_pid = -1;
            }
            return 1;
        }
    }
    return 0;
}

// Get process by PID
pcb_t* scheduler_get_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid) {
            return &process_table[i];
        }
    }
    return 0;
}

// Pick next process based on scheduling algorithm
static int pick_next_process() {
    int selected = -1;
    
    if (sched_mode == SCHED_FCFS) {
        // First Come First Serve - pick oldest ready process
        int earliest = 0x7FFFFFFF;
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (process_table[i].pid != -1 && 
                process_table[i].state == PROC_READY &&
                process_table[i].arrival_time < earliest) {
                earliest = process_table[i].arrival_time;
                selected = i;
            }
        }
    } else if (sched_mode == SCHED_RR) {
        // Round Robin - pick next ready process after current
        int start = (current_pid == -1) ? 0 : (current_pid % MAX_PROCESSES + 1);
        for (int count = 0; count < MAX_PROCESSES; count++) {
            int i = (start + count) % MAX_PROCESSES;
            if (process_table[i].pid != -1 && 
                process_table[i].state == PROC_READY) {
                selected = i;
                break;
            }
        }
    } else if (sched_mode == SCHED_PRIORITY) {
        // Priority - pick highest priority ready process
        int highest_priority = -1;
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (process_table[i].pid != -1 && 
                process_table[i].state == PROC_READY &&
                process_table[i].priority > highest_priority) {
                highest_priority = process_table[i].priority;
                selected = i;
            }
        }
    }
    
    return selected;
}

// Scheduler tick - execute one time unit
void scheduler_tick() {
    current_tick++;
    
    // If current process is running, execute it
    if (current_pid != -1) {
        pcb_t* proc = scheduler_get_process(current_pid);
        if (proc && proc->state == PROC_RUNNING) {
            proc->remaining_time--;
            proc->time_slice--;
            
            // Process completed
            if (proc->remaining_time <= 0) {
                proc->state = PROC_TERMINATED;
                proc->turnaround_time = current_tick - proc->arrival_time;
                print("Process ");
                print_int(proc->pid);
                print(" completed (turnaround=");
                print_int(proc->turnaround_time);
                print(")\n");
                current_pid = -1;
            }
            // Time slice expired (RR only)
            else if (sched_mode == SCHED_RR && proc->time_slice <= 0) {
                proc->state = PROC_READY;
                proc->time_slice = time_quantum;
                print("Process ");
                print_int(proc->pid);
                print(" preempted (quantum expired)\n");
                current_pid = -1;
            }
        }
    }
    
    // Pick next process if needed
    if (current_pid == -1) {
        int next = pick_next_process();
        if (next != -1) {
            current_pid = process_table[next].pid;
            process_table[next].state = PROC_RUNNING;
            process_table[next].time_slice = time_quantum;
            print("Process ");
            print_int(current_pid);
            print(" started\n");
        }
    }
    
    // Update waiting time for ready processes
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1 && 
            process_table[i].state == PROC_READY) {
            process_table[i].waiting_time++;
        }
    }
}

// List all processes
void scheduler_list_processes() {
    const char* state_names[] = {"NEW", "READY", "RUN", "WAIT", "DONE"};
    const char* mode_names[] = {"FCFS", "Round-Robin", "Priority"};
    
    print("\n");
    set_color(COLOR_CYAN, COLOR_BLACK);
    print("  ===============================================\n");
    set_color(COLOR_YELLOW, COLOR_BLACK);
    print("              Process Status Table\n");
    set_color(COLOR_CYAN, COLOR_BLACK);
    print("  ===============================================\n");
    
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("  Scheduler Mode: ");
    set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print(mode_names[sched_mode]);
    if (sched_mode == SCHED_RR) {
        print(" (quantum=");
        print_int(time_quantum);
        print(")");
    }
    print("\n\n");
    
    set_color(COLOR_DARK_GREY, COLOR_BLACK);
    print("  +-----+----------+------+-------+--------+------+\n");
    set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    print("  | PID |  State   | Prio | Burst | Remain | Wait |\n");
    set_color(COLOR_DARK_GREY, COLOR_BLACK);
    print("  +-----+----------+------+-------+--------+------+\n");
    
    int count = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1 && 
            process_table[i].state != PROC_TERMINATED) {
            
            set_color(COLOR_DARK_GREY, COLOR_BLACK);
            print("  | ");
            
            set_color(COLOR_YELLOW, COLOR_BLACK);
            if (process_table[i].pid < 10) print(" ");
            print_int(process_table[i].pid);
            
            set_color(COLOR_DARK_GREY, COLOR_BLACK);
            print("  | ");
            
            // Color based on state
            if (process_table[i].state == PROC_RUNNING) {
                set_color(COLOR_GREEN, COLOR_BLACK);
            } else if (process_table[i].state == PROC_READY) {
                set_color(COLOR_CYAN, COLOR_BLACK);
            } else {
                set_color(COLOR_DARK_GREY, COLOR_BLACK);
            }
            
            print(state_names[process_table[i].state]);
            int len = 0;
            const char* s = state_names[process_table[i].state];
            while (s[len]) len++;
            for (int j = len; j < 8; j++) print(" ");
            
            set_color(COLOR_DARK_GREY, COLOR_BLACK);
            print(" |  ");
            
            set_color(COLOR_WHITE, COLOR_BLACK);
            print_int(process_table[i].priority);
            if (process_table[i].priority < 10) print(" ");
            
            set_color(COLOR_DARK_GREY, COLOR_BLACK);
            print("  |   ");
            
            set_color(COLOR_WHITE, COLOR_BLACK);
            if (process_table[i].burst_time < 10) print(" ");
            print_int(process_table[i].burst_time);
            
            set_color(COLOR_DARK_GREY, COLOR_BLACK);
            print("  |    ");
            
            set_color(COLOR_WHITE, COLOR_BLACK);
            if (process_table[i].remaining_time < 10) print(" ");
            print_int(process_table[i].remaining_time);
            
            set_color(COLOR_DARK_GREY, COLOR_BLACK);
            print("  |  ");
            
            set_color(COLOR_WHITE, COLOR_BLACK);
            if (process_table[i].waiting_time < 10) print(" ");
            print_int(process_table[i].waiting_time);
            
            set_color(COLOR_DARK_GREY, COLOR_BLACK);
            print("  |\n");
            
            count++;
        }
    }
    
    set_color(COLOR_DARK_GREY, COLOR_BLACK);
    print("  +-----+----------+------+-------+--------+------+\n");
    
    if (count == 0) {
        set_color(COLOR_YELLOW, COLOR_BLACK);
        print("       No active processes\n");
    } else {
        set_color(COLOR_WHITE, COLOR_BLACK);
        print("       Total: ");
        set_color(COLOR_GREEN, COLOR_BLACK);
        print_int(count);
        set_color(COLOR_WHITE, COLOR_BLACK);
        print(" process(es)\n");
    }
    
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("\n");
}
