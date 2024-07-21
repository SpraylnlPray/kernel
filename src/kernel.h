#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define DANOS_MAX_PATH 108

#define ERRO(x) (void*)(x)
#define ERROR_I(x) (int)(x)
#define ISERR(x) ((int)(x) < 0)

void kernel_main();
void print(const char *str);

#endif