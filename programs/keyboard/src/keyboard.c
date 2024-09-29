#include "danos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void print_usage()
{
    printf("Usage:\n");
    printf("\tkeyboard layout set LANGUAGE \t to set the keyboard layout\n");
    printf("\tkeyboard layout \t\t\t\t to show the keyboard layout\n");
}

void handle_layout(int argc, char** argv)
{
    if (argc == 2)
    {
        // Show keyboard layout
        printf("Showing keyboard layout\n");
        return;
    }

    if (argc == 3 && strncmp(argv[2], "set", sizeof("set")) != 0)
    {
        printf("Unknown option: \"%s\"\n", argv[2]);
        print_usage();
        return;
    }

    if (argc == 3 && strncmp(argv[2], "set", sizeof("set")) == 0)
    {
        printf("Missing argument LANGUAGE\n");
        print_usage();
        return;
    }

    if (argc > 4)
    {
        print_usage();
        return;
    }

    printf("Calling kernel to get available languages\n");
    return;
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        print_usage();
        return 0;
    }

    if (strncmp(argv[1], "layout", sizeof("layout")) == 0)
    {
        handle_layout(argc, argv);
        return 0;
    }

    print_usage();
    return 0;
}