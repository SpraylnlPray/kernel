#include "keyboard.h"
#include "io/io.h"
#include "classic.h"
#include "kernel.h"
#include "idt/idt.h"
#include "task/task.h"
#include <stdint.h>
#include <stddef.h>

#define CLASSIC_KEYBOARD_CAPSLOCK 0x3A

static uint8_t keyboard_scan_set_one[] = {
    0x00, 0x1B, '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '-', '=',
    0x08, '\t', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '[', ']',
    0x0d, 0x00, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ';', '\'', '`', 
    0x00, '\\', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', ',', '.', '/', 0x00, '*',
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0', '.'
};

int classic_keyboard_init();
void classic_keyboard_handle_interrupt();

struct keyboard classic_keyboard = {
    .name = {"Classic"},
    .init = classic_keyboard_init,
};

int classic_keyboard_init()
{
    idt_register_interrupt_callback(ISR_KEYBOARD_INTERRUPT, classic_keyboard_handle_interrupt);
    keyboard_set_capslock(&classic_keyboard, KEYBOARD_CAPSLOCK_OFF);
    keyboard_set_shift(&classic_keyboard, KEYBOARD_SHIFT_OFF);
    outb(PS2_PORT, PS2_COMMAND_ENABLE_FIRST_PORT); // Enables first PS/2 port
    return 0;
}

uint8_t handle_capslock(char c)
{
    if (keyboard_get_capslock(&classic_keyboard) == KEYBOARD_CAPSLOCK_ON)
    {
        return c;
    }

    if (c >= 'A' && c <= 'Z')
    {
        c += 32;
    }

    return c;
}

uint8_t handle_shift(char c)
{
    if (keyboard_get_shift(&classic_keyboard) == KEYBOARD_SHIFT_OFF)
    {
        return c;
    }

    if (c >= 'A' && c <= 'Z')
    {
        c += 32;
        return c;
    }

    if (c >= 'a' && c <= 'z')
    {
        c -= 32;
        return c;
    }

    return c;
}

uint8_t classic_keyboard_scancode_to_char(uint8_t scancode)
{
    size_t size_of_keyboard_set_one = sizeof(keyboard_scan_set_one) / sizeof(uint8_t);
    if (scancode > size_of_keyboard_set_one)
    {
        return 0;
    }

    char c = keyboard_scan_set_one[scancode];
    c = handle_capslock(c);
    c = handle_shift(c);

    return c;
}

void classic_keyboard_handle_interrupt()
{
    kernel_page();

    uint8_t scancode = 0;
    scancode = insb(KEYBOARD_INPUT_PORT);
    insb(KEYBOARD_INPUT_PORT); // ignores some bytes

    if (scancode == CLASSIC_KEYBOARD_LSHIFT_PRESSED || scancode == CLASSIC_KEYBOARD_RSHIFT_PRESSED)
    {
        keyboard_set_shift(&classic_keyboard, KEYBOARD_SHIFT_ON);
        return;
    }

    if (scancode == CLASSIC_KEYBOARD_LSHIFT_RELEASED || scancode == CLASSIC_KEYBOARD_RSHIFT_RELEASED)
    {
        keyboard_set_shift(&classic_keyboard, KEYBOARD_SHIFT_OFF);
        return;
    }

    if (scancode & CLASSIC_KEYBOARD_KEY_RELEASED)
    {
        return;
    }

    if (scancode == CLASSIC_KEYBOARD_CAPSLOCK)
    {
        KEYBOARD_CAPSLOCK_STATE old_state = keyboard_get_capslock(&classic_keyboard);
        keyboard_set_capslock(&classic_keyboard, old_state == KEYBOARD_CAPSLOCK_ON ? KEYBOARD_CAPSLOCK_OFF : KEYBOARD_CAPSLOCK_ON);
    }

    uint8_t c = classic_keyboard_scancode_to_char(scancode);
    if (c != 0)
    {
        keyboard_push(c);
    }

    task_page();
}

struct keyboard* classic_init()
{
    return &classic_keyboard;
}