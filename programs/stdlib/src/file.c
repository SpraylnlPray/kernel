#include "danos.h"
int printf(const char *fmt, ...);

int fopen(const char* path, const char* mode)
{
    printf("stdlib fopen, path: %s mode: %s\n", path, mode);
    return danos_fopen(path, mode);
}