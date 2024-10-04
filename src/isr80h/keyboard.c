#include "keyboard.h"
#include "keyboard/keyboard.h"
#include "task/task.h"

void* isr80h_command10_get_keyboard_layouts()
{
    char** available_layouts = (char**)task_get_stack_item(task_current(), 1);
    int size = (int)task_get_stack_item(task_current(), 0);
    keyboard_get_available_layouts(available_layouts, size);
    return 0;
}

void* isr80h_command11_get_keyboard_layout_count()
{
    return (void*)keyboard_get_layout_count();
}