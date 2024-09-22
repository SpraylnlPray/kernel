#ifndef DANOS_STRING_H
#define DANOS_STRING_H

#include <stdbool.h>

int strlen(const char *ptr);
bool is_digit(char c);
int tonumericdigit(char c);
int strnlen(const char *ptr, int max);
char* strcpy(char* dest, const char* src);
int strnlen_terminator(const char* str, int max, char terminator);
int strncmp(const char* str1, const char* str2, int n);
int istrncmp(const char* s1, const char* s2, int n);
char tolower(char s1);
char* strncpy(char* dest, const char* src, int count);
char* strtok(char* str, const char* delimiters);

#endif