#ifndef DANOS_FILE_H
#define DANOS_FILE_H

#include <stdint.h>

typedef unsigned int STAT_TYPE;
enum
{
    FILE_TYPE_FILE,
    FILE_TYPE_DIRECTORY,
};

struct stat
{
    uint32_t size;
    STAT_TYPE type;
};

int fopen(const char* path, const char* mode);
int stat(const char *path, struct stat *buf);

#endif