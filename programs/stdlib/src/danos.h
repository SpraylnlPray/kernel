#ifndef DANOS_H
#define DANOS_H

// #include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct command_argument
{
    char argument[512];
    struct command_argument* next;
};

struct process_arguments
{
    int argc;
    char** argv;
};

void print(const char* message);
int danos_getkey();
void* danos_malloc(size_t size);
void danos_free(void* ptr);
void danos_putchar(char c);
int danos_getkey_block();
void danos_terminal_readline(char* out, int max, bool output_while_typing);
void danos_process_load_start(const char* filename);
struct command_argument* danos_parse_command(const char* command, int max);
void danos_process_get_arguments(struct process_arguments* arguments);
int danos_system(struct command_argument* arguments);
int danos_system_run(const char* command);
void danos_exit();
void* danos_get_keyboard_layouts(char** buf, int size);
int danos_get_keyboard_layout_count();
void* danos_get_active_keyboard_layout_id(char* buf, int size);
int danos_set_active_keyboard_layout(char* id);

#endif