#include "danos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "file.h"

void print_usage()
{
    // printf("print_usage\n");
    return;
}

bool list_file(char *path)
{
    printf("list_file\n");
    int fd = fopen(path, "r");
    printf("fd: %i\n", fd);
    return fd > 0;
}

bool list_directory(char *path)
{
    int res = 0;
    struct dirent *dirent = NULL;
    int fd = opendir(path);
    if (!fd)
    {
        printf("Error opening directory: %s", path);
        res = -1;
        goto out;
    }

    dirent = readdir(fd);
    if (!dirent)
    {
        printf("Error reading directory %s", path);
        res = -1;
        goto out;
    }
    
    while (dirent != NULL)
    {
        printf("%10s %i\n", dirent->d_name, 25);
        dirent = readdir(fd);
    }

out:
    if (fd)
    {
        closedir(fd);
    }

    if (dirent)
    {
        free(dirent);
    }

    return res;
}

int main(int argc, char **argv)
{
    print_usage();

    struct stat buf;
    int res = stat(argv[1], &buf);
    if (res != 0)
    {
        printf("No such file or directory: %s", argv[1]);
        return -1;
    }

    if (buf.type == FILE_TYPE_DIRECTORY)
    {
        return list_directory(argv[1]);
    }
    if (buf.type == FILE_TYPE_FILE)
    {
        return list_file(argv[1]);
    }

    printf("Unknown file type\n");
    return -1;
}