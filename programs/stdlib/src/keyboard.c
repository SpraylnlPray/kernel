#include "keyboard.h"
#include "danos.h"

char** get_keyboard_layouts(char** available_layouts)
{
    return danos_get_keyboard_layouts(available_layouts);
}