#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

struct process;

#define KEYBOARD_CAPSLOCK_ON 1
#define KEYBOARD_CAPSLOCK_OFF 0

#define KEYBOARD_SHIFT_ON 1
#define KEYBOARD_SHIFT_OFF 0

#define MAX_KEYBOARD_LAYOUT_COUNT 20
#define KEYBOARD_LAYOUT_ID_LENGTH 6 // 5 characters + null terminator

typedef int KEYBOARD_CAPSLOCK_STATE;
typedef int KEYBOARD_SHIFT_STATE;

typedef int (*KEYBOARD_INIT_FUNCTION)();


struct keyboard
{
    KEYBOARD_INIT_FUNCTION init;
    char name[20];
    struct keyboard* next;
    KEYBOARD_CAPSLOCK_STATE capslock_state;
    KEYBOARD_SHIFT_STATE shift_state;
    char available_layouts[MAX_KEYBOARD_LAYOUT_COUNT][KEYBOARD_LAYOUT_ID_LENGTH];
    int layout_count;
};

void keyboard_init();
void keyboard_backspace(struct process* process);
void keyboard_push(char c);
char keyboard_pop();
int keyboard_insert(struct keyboard* keyboard);
void keyboard_set_capslock(struct keyboard* keyboard, KEYBOARD_CAPSLOCK_STATE state);
void keyboard_set_shift(struct keyboard* keyboard, KEYBOARD_SHIFT_STATE state);
KEYBOARD_CAPSLOCK_STATE keyboard_get_capslock(struct keyboard* keyboard);
KEYBOARD_SHIFT_STATE keyboard_get_shift(struct keyboard* keyboard);
void keyboard_get_available_layouts(char** buf, uint32_t size);
int keyboard_get_layout_count();

#endif