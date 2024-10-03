#include "keyboard.h"
#include "keyboard/keyboard.h"
#include "task/task.h"

void* isr80h_command10_get_keyboard_languages()
{
    char** available_layouts = (char**)task_get_stack_item(task_current(), 0);
    keyboard_get_available_layouts(available_layouts);
    int foo = 1;
    if (foo)
    {

    }

    return 0;
}