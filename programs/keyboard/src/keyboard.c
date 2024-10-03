#include "danos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "keyboard.h"

void print_usage()
{
    printf("Usage:\n");
    printf("\tkeyboard layout set LANGUAGE \t to set the keyboard layout\n");
    printf("\tkeyboard layout \t\t\t\t to show the keyboard layout\n");
    printf("\tkeyboard layout list \t\t\t\t to list the available layouts\n");
}

void show_layout()
{
    printf("Showing keyboard layout\n");
}

void list_layouts()
{
    printf("Listing layouts\n");
    char** available_layouts = (char**)malloc(sizeof(char*) * MAX_KEYBOARD_LAYOUT_COUNT);
    for (int i = 0; i < MAX_KEYBOARD_LAYOUT_COUNT; i++)
    {
        available_layouts[i] = (char*)malloc(sizeof(char*) * KEYBOARD_LAYOUT_ID_LENGTH);
    }
    get_keyboard_layouts(available_layouts);
    char layout[KEYBOARD_LAYOUT_ID_LENGTH];
    strncpy(layout, *available_layouts, sizeof(layout));
    printf("%s\n", layout);
}

void set_language(char* language)
{
    printf("Setting language");
}

void handle_layout(int argc, char** argv)
{
    // keyboard layout
    if (argc == 2)
    {
        show_layout();
        return;
    }

    // keyboard layout foo
    if (argc == 3 && strncmp(argv[2], "list", sizeof("list")) != 0)
    {
        printf("Unknown option: \"%s\"\n", argv[2]);
        print_usage();
        return;
    }

    // keyboard layout list
    if (argc == 3 && strncmp(argv[2], "list", sizeof("list")) == 0)
    {
        list_layouts();
        return;
    }

    // keyboard layout set
    if (argc == 3 && strncmp(argv[2], "set", sizeof("set")) == 0)
    {
        printf("Missing argument LANGUAGE\n");
        print_usage();
        return;
    }

    // keyboard layout set en_US
    if (argc == 4 && strncmp(argv[2], "set", sizeof("set")) == 0)
    {
        set_language(argv[3]);
        return;
    }

    print_usage();
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