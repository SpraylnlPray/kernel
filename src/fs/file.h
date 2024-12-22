#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include "pparser.h"

typedef unsigned int FILE_SEEK_MODE;
enum
{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

typedef unsigned int FILE_MODE;
enum
{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

enum
{
    FILE_STAT_READ_ONLY = 0x00000001,
};

typedef unsigned int FILE_STAT_FLAGS;

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

struct file_stat
{
    FILE_STAT_FLAGS flags;
    uint32_t filesize;
};

struct disk;
typedef void* (*FS_OPEN_FUNCTION)(struct disk* disk, struct path_part* path, FILE_MODE mode);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk* disk);
typedef int (*FS_READ_FUNCTION)(struct disk* disk, void* private, uint32_t size, uint32_t nmemb, char* out);
typedef int (*FS_SEEK_FUNCTION)(void* private, uint32_t offset, FILE_SEEK_MODE seek_mode);
typedef int (*FS_FSTAT_FUNCTION)(struct disk* disk, void* private, struct file_stat* stat);
typedef int (*FS_CLOSE_FUNCTION)(void* private);
typedef void* (*FS_OPEN_DIR_FUNCTION)(struct disk* disk, struct path_part* path);
typedef struct dirent* (*FS_READ_DIR_FUNCTION)(struct disk* disk, void* private);
typedef int (*FS_CLOSE_DIR_FUNCTION)(void* private);
typedef int (*FS_STAT_FUNCTION)(struct disk* disk, struct path_part* path, struct stat* buf);

struct filesystem
{
    // Filesystem should return zero from resolve if the provided disk is using its filesystem
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;
    FS_READ_FUNCTION read;
    FS_SEEK_FUNCTION seek;
    FS_FSTAT_FUNCTION fstat;
    FS_CLOSE_FUNCTION close;
    FS_OPEN_DIR_FUNCTION opendir;
    FS_READ_DIR_FUNCTION readdir;
    FS_CLOSE_DIR_FUNCTION closedir;
    FS_STAT_FUNCTION stat;

    char name[20];
};

struct file_descriptor
{
    // descriptor index
    int index;
    struct filesystem* filesystem;
    
    // Private data for internal fd
    void* private_data;

    // Disk that the fd should be used on
    struct disk* disk;
};

void fs_init();
int fopen(const char* filename, const char* mode_str);
int fseek(int fd, int offset, FILE_SEEK_MODE whence);
int fread(void* ptr, uint32_t size, uint32_t nmemb, int fd);
int fstat(int fd, struct file_stat* stat);
void fs_insert_filesystem(struct filesystem* filesystem);
struct filesystem* fs_resolve(struct disk* disk);
int fclose(int fd);
int opendir(const char* dirname);
struct dirent* readdir(int fd);
int closedir(int fd);
int stat(const char* path, struct stat *buf);

typedef unsigned int D_TYPE;
enum
{
    DT_REG, // Regular file
    DT_DIR, // Directory
}; // https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html

struct dirent
{
    char* d_name;
    uint8_t d_namelen;
    D_TYPE d_type;
};

#endif