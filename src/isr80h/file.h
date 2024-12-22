#ifndef FILE_H
#define FILE_H

struct interrupt_frame;
void *isr80h_command14_stat(struct interrupt_frame *interrupt_frame);
void *isr80h_command15_fopen(struct interrupt_frame *frame);

#endif