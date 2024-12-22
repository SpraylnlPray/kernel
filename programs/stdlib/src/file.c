#include "danos.h"
int printf(const char *fmt, ...);

int fopen(const char *path, const char *mode)
{
    return danos_fopen(path, mode);
}

int stat(const char *path, struct stat *buf)
{
    return danos_stat(path, buf);
}

int opendir(const char *path)
{
    return danos_opendir(path);
}

struct dirent *readdir(int fd)
{
    return danos_readdir(fd);
}

int closedir(int fd)
{
    return danos_closedir(fd);
}