#ifndef DANOS_STDLIB_H
#define DANOS_STDLIB_H

#include <stddef.h>
#include <stdbool.h>

#define ASCII_ZERO 48
#define ASCII_NINE 57
#define ASCII_SPACE 32

void *malloc(size_t size);
void free(void *ptr);
char *itoa(int i);
int ctoi(char c);
int atoi(char *c);
bool isdigit(char c);

#endif