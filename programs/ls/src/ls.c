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

bool list_file(char* path)
{
    printf("list_file\n");
    int fd = fopen(path, "r");
    printf("fd: %i\n", fd);
    return fd > 0;
}

bool list_directory(char* path)
{
    printf("list_directory\n");
    return 0;
}

int main(int argc, char** argv)
{
    print_usage();
    char* path = argv[1];
    
    bool success = list_file(path);
    if (success)
        return 0;
    
    return list_directory(path);
}