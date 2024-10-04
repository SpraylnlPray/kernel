#include "keyboard.h"
#include "danos.h"

void get_keyboard_layouts(char** available_layouts, int size)
{
    danos_get_keyboard_layouts(available_layouts, size);
}

int get_layout_count()
{
    return danos_get_keyboard_layout_count();
}