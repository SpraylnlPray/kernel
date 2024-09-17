#ifndef DANOS_H
#define DANOS_H

#include <stddef.h>

void print(const char* message);
int getkey();
void* danos_malloc(size_t size);
void danos_free(void* ptr);
#endif