// shell.c - Interactive shell implementation
#include "kernel.h"
#include "shell.h"
#include "scheduler.h"
#include "memory.h"

// String functions
int strlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int atoi(const char* str) {
    int result = 0;
    int sign = 1;
    
    if (*str == '-') {
        sign = -1;
        str++;
    }
    
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return sign * result;
}

// Parse command into arguments
void parse_command(char* input, char** args, int* argc) {
    *argc = 0;
    char* ptr = input;
    
    while (*ptr && *argc < MAX_ARGS) {
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        
        if (*ptr == '\0') break;
        
        args[(*argc)++] = ptr;
        
        while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
        
        if (*ptr) *ptr++ = '\0';
    }
    
    args[*argc] = 0;
}

// Command: help
void cmd_help() {
    print("\n");
    set_color(COLOR_CYAN, COLOR_BLACK);
    print("  ===============================================\n");
    set_color(COLOR_YELLOW, COLOR_BLACK);
    print("             MiniOS Command Reference\n");
    set_color(COLOR_CYAN, COLOR_BLACK);
    print("  ===============================================\n\n");
    
    set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print("  >> SYSTEM COMMANDS:\n");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("     help              - Show this help message\n");
    print("     clear             - Clear screen\n");
    print("     echo <text>       - Print text\n\n");
    
    set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    print("  >> PROCESS COMMANDS:\n");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("     ps                - List all processes\n");
    print("     run <burst> <prio>- Create new process\n");
    print("     kill <pid>        - Terminate process\n\n");
    
    set_color(COLOR_LIGHT_MAGENTA, COLOR_BLACK);
    print("  >> SCHEDULER COMMANDS:\n");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("     scheduler mode <fcfs|rr|priority>\n");
    print("     scheduler quantum <n>  - Set time quantum\n");
    print("     scheduler tick    - Simulate clock tick\n\n");
    
    set_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
    print("  >> MEMORY COMMANDS:\n");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("     meminfo           - Show memory stats\n");
    print("     frames            - Show frame table\n");
    print("     allocpages <pid> <n>   - Allocate pages\n");
    print("     access <pid> <page>    - Access a page\n\n");
    
    set_color(COLOR_YELLOW, COLOR_BLACK);
    print("  TIP: ");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("Try 'run 10 5' to create a process!\n\n");
}

// Command: clear
void cmd_clear() {
    clear_screen();
}

// Command: echo
void cmd_echo(char** args, int argc) {
    for (int i = 1; i < argc; i++) {
        print(args[i]);
        if (i < argc - 1) print(" ");
    }
    print("\n");
}

// Command: ps (implemented in scheduler.c)
void cmd_ps() {
    scheduler_list_processes();
}

// Command: run
void cmd_run(char** args, int argc) {
    if (argc < 3) {
        print("Usage: run <burst_time> <priority>\n");
        return;
    }
    
    int burst = atoi(args[1]);
    int priority = atoi(args[2]);
    
    if (burst <= 0 || priority < 0 || priority > 10) {
        print("Error: Invalid parameters\n");
        print("  burst_time must be > 0\n");
        print("  priority must be 0-10\n");
        return;
    }
    
    int pid = scheduler_create_process(burst, priority);
    if (pid >= 0) {
        set_color(COLOR_GREEN, COLOR_BLACK);
        print("Created process PID=");
        print_int(pid);
        print(" burst=");
        print_int(burst);
        print(" priority=");
        print_int(priority);
        print("\n");
        set_color(COLOR_WHITE, COLOR_BLACK);
    } else {
        set_color(COLOR_RED, COLOR_BLACK);
        print("Error: Maximum processes reached\n");
        set_color(COLOR_WHITE, COLOR_BLACK);
    }
}

// Command: kill
void cmd_kill(char** args, int argc) {
    if (argc < 2) {
        print("Usage: kill <pid>\n");
        return;
    }
    
    int pid = atoi(args[1]);
    if (scheduler_kill_process(pid)) {
        set_color(COLOR_YELLOW, COLOR_BLACK);
        print("Process ");
        print_int(pid);
        print(" terminated\n");
        set_color(COLOR_WHITE, COLOR_BLACK);
    } else {
        set_color(COLOR_RED, COLOR_BLACK);
        print("Error: Process not found\n");
        set_color(COLOR_WHITE, COLOR_BLACK);
    }
}

// Command: scheduler
void cmd_scheduler(char** args, int argc) {
    if (argc < 2) {
        print("Usage: scheduler <mode|quantum|tick>\n");
        return;
    }
    
    if (strcmp(args[1], "mode") == 0) {
        if (argc < 3) {
            print("Usage: scheduler mode <fcfs|rr|priority>\n");
            return;
        }
        
        if (strcmp(args[2], "fcfs") == 0) {
            scheduler_set_mode(SCHED_FCFS);
            set_color(COLOR_GREEN, COLOR_BLACK);
            print("Scheduler mode: FCFS\n");
            set_color(COLOR_WHITE, COLOR_BLACK);
        } else if (strcmp(args[2], "rr") == 0) {
            scheduler_set_mode(SCHED_RR);
            set_color(COLOR_GREEN, COLOR_BLACK);
            print("Scheduler mode: Round Robin\n");
            set_color(COLOR_WHITE, COLOR_BLACK);
        } else if (strcmp(args[2], "priority") == 0) {
            scheduler_set_mode(SCHED_PRIORITY);
            set_color(COLOR_GREEN, COLOR_BLACK);
            print("Scheduler mode: Priority\n");
            set_color(COLOR_WHITE, COLOR_BLACK);
        } else {
            print("Unknown mode. Use: fcfs, rr, or priority\n");
        }
    } else if (strcmp(args[1], "quantum") == 0) {
        if (argc < 3) {
            print("Usage: scheduler quantum <value>\n");
            return;
        }
        
        int quantum = atoi(args[2]);
        if (quantum > 0) {
            scheduler_set_quantum(quantum);
            set_color(COLOR_GREEN, COLOR_BLACK);
            print("Time quantum set to ");
            print_int(quantum);
            print("\n");
            set_color(COLOR_WHITE, COLOR_BLACK);
        } else {
            print("Error: Quantum must be > 0\n");
        }
    } else if (strcmp(args[1], "tick") == 0) {
        scheduler_tick();
        set_color(COLOR_CYAN, COLOR_BLACK);
        print("Scheduler tick executed\n");
        set_color(COLOR_WHITE, COLOR_BLACK);
    } else {
        print("Unknown scheduler command\n");
    }
}

// Command: meminfo
void cmd_meminfo() {
    memory_show_info();
}

// Command: frames
void cmd_frames() {
    memory_show_frames();
}

// Command: allocpages
void cmd_allocpages(char** args, int argc) {
    if (argc < 3) {
        print("Usage: allocpages <pid> <count>\n");
        return;
    }
    
    int pid = atoi(args[1]);
    int count = atoi(args[2]);
    
    if (memory_allocate_pages(pid, count)) {
        set_color(COLOR_GREEN, COLOR_BLACK);
        print("Allocated ");
        print_int(count);
        print(" pages to PID ");
        print_int(pid);
        print("\n");
        set_color(COLOR_WHITE, COLOR_BLACK);
    } else {
        set_color(COLOR_RED, COLOR_BLACK);
        print("Error: Could not allocate pages\n");
        set_color(COLOR_WHITE, COLOR_BLACK);
    }
}

// Command: access
void cmd_access(char** args, int argc) {
    if (argc < 3) {
        print("Usage: access <pid> <page>\n");
        return;
    }
    
    int pid = atoi(args[1]);
    int page = atoi(args[2]);
    
    memory_access_page(pid, page);
}

// Execute command
void shell_execute(char* input) {
    char* args[MAX_ARGS];
    int argc;
    
    parse_command(input, args, &argc);
    
    if (argc == 0) return;
    
    // Execute command
    if (strcmp(args[0], "help") == 0) {
        cmd_help();
    } else if (strcmp(args[0], "clear") == 0) {
        cmd_clear();
    } else if (strcmp(args[0], "echo") == 0) {
        cmd_echo(args, argc);
    } else if (strcmp(args[0], "ps") == 0) {
        cmd_ps();
    } else if (strcmp(args[0], "run") == 0) {
        cmd_run(args, argc);
    } else if (strcmp(args[0], "kill") == 0) {
        cmd_kill(args, argc);
    } else if (strcmp(args[0], "scheduler") == 0) {
        cmd_scheduler(args, argc);
    } else if (strcmp(args[0], "meminfo") == 0) {
        cmd_meminfo();
    } else if (strcmp(args[0], "frames") == 0) {
        cmd_frames();
    } else if (strcmp(args[0], "allocpages") == 0) {
        cmd_allocpages(args, argc);
    } else if (strcmp(args[0], "access") == 0) {
        cmd_access(args, argc);
    } else {
        set_color(COLOR_RED, COLOR_BLACK);
        print("Unknown command: ");
        print(args[0]);
        print("\n");
        set_color(COLOR_WHITE, COLOR_BLACK);
        print("Type 'help' for available commands\n");
    }
}

// Main shell loop
void shell_loop() {
    char input[MAX_INPUT];
    int input_pos = 0;
    
    while (1) {
        set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
        print("minios");
        set_color(COLOR_YELLOW, COLOR_BLACK);
        print(" > ");
        set_color(COLOR_WHITE, COLOR_BLACK);
        
        input_pos = 0;
        
        // Read input
        while (1) {
            char c = get_key();
            
            if (c == '\n') {
                print_char('\n');
                input[input_pos] = '\0';
                break;
            } else if (c == '\b') {
                if (input_pos > 0) {
                    input_pos--;
                    print_char('\b');
                }
            } else if (c >= 32 && c <= 126 && input_pos < MAX_INPUT - 1) {
                input[input_pos++] = c;
                print_char(c);
            }
        }
        
        // Execute command
        if (input_pos > 0) {
            shell_execute(input);
        }
    }
}
