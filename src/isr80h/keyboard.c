#include "keyboard.h"
#include "keyboard/keyboard.h"
#include "task/task.h"

void* isr80h_command10_get_keyboard_layouts(struct interrupt_frame* interrupt_frame)
{
    int size = (int)task_get_stack_item(task_current(), 0);
    char** available_layouts = (char**)task_get_stack_item(task_current(), 1);
    keyboard_get_available_layouts(available_layouts, size);
    return 0;
}

void* isr80h_command11_get_keyboard_layout_count(struct interrupt_frame* interrupt_frame)
{
    return (void*)keyboard_get_layout_count();
}

void* isr80h_command12_get_active_keyboard_layout_id(struct interrupt_frame* interrupt_frame)
{
    int size = (int)task_get_stack_item(task_current(), 0);
    char* buf = (char*)task_get_stack_item(task_current(), 1);
    keyboard_get_active_layout_id(buf, size);
    return 0;
}