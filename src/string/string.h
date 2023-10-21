#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char *ptr);
bool is_digit(char c);
int tonumericdigit(char c);
int strnlen(const char *ptr, int max);

#endif