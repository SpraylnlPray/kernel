#include "fs/file.h"
#include "kernel.h"
#include "task/task.h"

void* isr80h_command14_fstat(struct interrupt_frame* interrupt_frame)
{
    return (void*)0; // not implemented yet
}

void* isr80h_command15_fopen(struct interrupt_frame* frame)
{
    char path_buffer[1024];
    char mode_buffer[8];
    void* user_space_mode_buffer = task_get_stack_item(task_current(), 0);
    void* user_space_path_buffer = task_get_stack_item(task_current(), 1);
    copy_string_from_task(task_current(), user_space_path_buffer, path_buffer, sizeof(path_buffer));
    copy_string_from_task(task_current(), user_space_mode_buffer, mode_buffer, sizeof(mode_buffer));
    return (void*)fopen(path_buffer, mode_buffer);
}
