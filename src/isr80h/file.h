#ifndef FILE_H
#define FILE_H

struct interrupt_frame;
void *isr80h_command14_stat(struct interrupt_frame *interrupt_frame);
void *isr80h_command15_fopen(struct interrupt_frame *frame);
void *isr80h_command16_opendir(struct interrupt_frame *frame);
void *isr80h_command17_readdir(struct interrupt_frame *frame);
void *isr80h_command18_closedir(struct interrupt_frame *frame);

#endif