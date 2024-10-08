#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define ERROR(x) (void*)(x)
#define ERROR_I(x) (int)(x)
#define ISERR(x) ((int)(x) < 0)

void kernel_main();
void kernel_page();
void kernel_registers();
void print(const char *str);
void panic(const char* msg);
void terminal_writechar(char c, char color);

#endif