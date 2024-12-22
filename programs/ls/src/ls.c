#include "danos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "file.h"

void print_usage()
{
    printf("print_usage\n");
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
    printf("list_directory\n");
    return 0;
}

int main(int argc, char **argv)
{
    print_usage();

    struct stat buf;
    int res = stat(argv[1], &buf);
    if (res != 0)
        return -1;

    if (buf.type == FILE_TYPE_DIRECTORY)
    {
        list_directory(argv[1]);
    }
    if (buf.type == FILE_TYPE_FILE)
    {
        list_file(argv[1]);
    }

    return 0;
}