#ifndef ISR80H_KEYBOARD_H
#define ISR80H_KEYBOARD_H

#include <stdint.h>
struct interrupt_frame;

void* isr80h_command10_get_keyboard_layouts(struct interrupt_frame* interrupt_frame);
void* isr80h_command11_get_keyboard_layout_count(struct interrupt_frame* interrupt_frame);
void* isr80h_command12_get_active_keyboard_layout_id(struct interrupt_frame* interrupt_frame);
void* isr80h_command13_set_active_keyboard_layout(struct interrupt_frame* interrupt_frame);

#endif