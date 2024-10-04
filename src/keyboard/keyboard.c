#include "keyboard.h"
#include "status.h"
#include "kernel.h"
#include "task/task.h"
#include "task/process.h"
#include "string/string.h"
#include "classic.h"

static struct keyboard* keyboard_list_head = 0;
static struct keyboard* keyboard_list_last = 0;

void keyboard_init()
{
    keyboard_insert(classic_init());
}

int keyboard_insert(struct keyboard* keyboard)
{
    int res = 0;

    if (keyboard->init == 0)
    {
        res = -DANOS_EINVARG;
        goto out;
    }

    if (keyboard_list_last)
    {
        keyboard_list_last->next = keyboard;
        keyboard_list_last = keyboard;
    }
    else
    {
        keyboard_list_head = keyboard;
        keyboard_list_last = keyboard;
    }

    res = keyboard->init();

out:
    return res;
}

int keyboard_get_layout_count()
{
    struct keyboard* keyboard = keyboard_list_head;
    int count = 0;
    while (keyboard != NULL)
    {
        count += keyboard->layout_count;
        keyboard = keyboard->next;
    }

    return count;
}

void keyboard_get_available_layouts(char** buf, uint32_t size)
{
    struct keyboard* keyboard = keyboard_list_head;
    int count = 0;
    while (keyboard != NULL && count < size)
    {
        for (int i = 0; i < keyboard->layout_count && count < size; i++, count++)
        {
            strncpy(*buf, keyboard->available_layouts[i]->identifier, sizeof(keyboard->available_layouts[i]->identifier));
            buf++;
        }

        keyboard = keyboard->next;
    }
}

static int keyboard_get_tail_index(struct process* process)
{
    return process->keyboard.tail % sizeof(process->keyboard.buffer);
}

void keyboard_backspace(struct process* process)
{
    process->keyboard.tail -= 1;
    int real_index = keyboard_get_tail_index(process);
    process->keyboard.buffer[real_index] = 0x00;
}

void keyboard_set_capslock(struct keyboard* keyboard, KEYBOARD_CAPSLOCK_STATE state)
{
    keyboard->capslock_state = state;
}

void keyboard_set_shift(struct keyboard* keyboard, KEYBOARD_SHIFT_STATE state)
{
    keyboard->shift_state = state;
}

KEYBOARD_CAPSLOCK_STATE keyboard_get_capslock(struct keyboard* keyboard)
{
    return keyboard->capslock_state;
}

KEYBOARD_SHIFT_STATE keyboard_get_shift(struct keyboard* keyboard)
{
    return keyboard->shift_state;
}

void keyboard_push(char c)
{
    struct process* process = process_current();
    if (!process)
    {
        return;
    }

    if (c == 0)
    {
        return;
    }

    int real_index = keyboard_get_tail_index(process);

    if (real_index != 0)
    {
        process->keyboard.buffer[real_index] = c;
    }
    process->keyboard.buffer[real_index] = c;
    process->keyboard.tail++;
}

char keyboard_pop()
{
    if (!task_current())
    {
        return 0;
    }

    struct process* process = task_current()->process;
    int real_index = process->keyboard.head  % sizeof(process->keyboard.buffer);
    char c = process->keyboard.buffer[real_index];
    if (c == 0x00)
    {
        // Nothing to pop
        return 0;
    }

    process->keyboard.buffer[real_index] = 0;
    process->keyboard.head++;
    return c;
}