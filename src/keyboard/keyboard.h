#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

struct process;

#define KEYBOARD_CAPSLOCK_ON 1
#define KEYBOARD_CAPSLOCK_OFF 0

#define KEYBOARD_SHIFT_ON 1
#define KEYBOARD_SHIFT_OFF 0

#define MAX_KEYBOARD_LAYOUT_COUNT 20 // TODO: Find better solution for this
#define KEYBOARD_LAYOUT_ID_LENGTH 6 // 5 characters + null terminator

typedef int KEYBOARD_CAPSLOCK_STATE;
typedef int KEYBOARD_SHIFT_STATE;

typedef int (*KEYBOARD_INIT_FUNCTION)();

struct keyboard_layout
{
    char identifier[KEYBOARD_LAYOUT_ID_LENGTH];
    uint8_t scan_set_default[82];
    uint8_t scan_set_shift[82];
};

struct keyboard
{
    KEYBOARD_INIT_FUNCTION init;
    char name[20];
    struct keyboard* next;
    KEYBOARD_CAPSLOCK_STATE capslock_state;
    KEYBOARD_SHIFT_STATE shift_state;
    struct keyboard_layout* available_layouts[MAX_KEYBOARD_LAYOUT_COUNT];
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
void keyboard_add_layout(struct keyboard *keyboard, struct keyboard_layout *layout);
void keyboard_set_active_layout(struct keyboard_layout* layout);
void keyboard_get_active_layout_id(char* buf, uint32_t size);
struct keyboard_layout* keyboard_get_active_layout();
int keyboard_set_layout(char* layout_id);

#endif