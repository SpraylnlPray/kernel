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

void get_active_keyboard_layout_id(char* buf, int size)
{
    danos_get_active_keyboard_layout_id(buf, size);
}

int set_active_keyboard_layout(char* id)
{
    return danos_set_active_keyboard_layout(id);
}