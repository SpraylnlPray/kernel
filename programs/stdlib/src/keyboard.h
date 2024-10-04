#ifndef DANOS_KEYBOARD_H
#define DANOS_KEYBOARD_H

#define KEYBOARD_LAYOUT_ID_LENGTH 6 // 5 characters + null terminator

void get_keyboard_layouts(char** available_layouts, int size);
int get_layout_count();
void get_active_keyboard_layout_id(char* buf, int size);

#endif