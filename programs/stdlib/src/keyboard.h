#ifndef DANOS_KEYBOARD_H
#define DANOS_KEYBOARD_H

#define MAX_KEYBOARD_LAYOUT_COUNT 20
#define KEYBOARD_LAYOUT_ID_LENGTH 6 // 5 characters + null terminator

void get_keyboard_layouts(char** available_layouts, int size);
int get_layout_count();

#endif