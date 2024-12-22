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

typedef unsigned int D_TYPE;
enum
{
    DT_REG, // Regular file
    DT_DIR, // Directory
}; // https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html

struct dirent
{
    char *d_name;
    uint8_t d_namelen;
    D_TYPE d_type;
};

int fopen(const char *path, const char *mode);
int stat(const char *path, struct stat *buf);
int opendir(const char *path);
struct dirent *readdir(int fd);
int closedir(int fd);

#endif