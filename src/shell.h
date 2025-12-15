// shell.h - Shell interface
#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT 256
#define MAX_ARGS 16

// Shell functions
void shell_loop();
void shell_execute(char* input);
void parse_command(char* input, char** args, int* argc);

// Built-in commands
void cmd_help();
void cmd_clear();
void cmd_echo(char** args, int argc);
void cmd_ps();
void cmd_run(char** args, int argc);
void cmd_kill(char** args, int argc);
void cmd_scheduler(char** args, int argc);
void cmd_meminfo();
void cmd_frames();
void cmd_allocpages(char** args, int argc);
void cmd_access(char** args, int argc);

#endif
