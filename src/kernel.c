#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"

#include <stdint.h>
#include <stddef.h>

uint16_t *video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char color)
{
    // 0x0341 would print a green (or whatever) 'A' (hex code for ascii A is 41)
    // Actually the color needs to be in first position
    // but because of endiness they are flipped and the
    // processor will turn it around.
    // This adds the color, moves it to the left and then
    // appends the character we want to print
    return (color << 8) | c;
}

void terminal_putchar(int x, int y, char c, char color)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}

void terminal_writechar(char c, char color)
{
    if (c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, color);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_init()
{
    video_mem = (uint16_t*)(0xB8000);
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while(str[len])
    {
        len++;
    }

    return len;
}

void print(const char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

void kernel_main()
{
    terminal_init();
    print("Hello world\n tessssst");

    // Initialize the heap
    kheap_init();

    // Initialize the interrupt descriptor table
    idt_init();

    // Enable the system interrupts;
    enable_interrupts();
}