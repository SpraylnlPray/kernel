#ifndef DANOS_H
#define DANOS_H

#include <stddef.h>
#include <stdbool.h>

struct command_argument
{
    char argument[512];
    struct command_argument* next;
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

#endif