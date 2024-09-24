#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "string/string.h"
#include "disk/streamer.h"
#include "fs/file.h"
#include "gdt/gdt.h"
#include "config.h"
#include "memory/memory.h"
#include "task/tss.h"
#include "task/task.h"
#include "task/process.h"
#include "status.h"
#include "isr80h/isr80h.h"
#include "keyboard/keyboard.h"

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

void terminal_backspace()
{
    if (terminal_row == 0 && terminal_col == 0)
    {
        return;
    }

    if (terminal_col == 0)
    {
        terminal_row -= 1;
        terminal_col = VGA_WIDTH;
    }

    terminal_col -= 1;
    terminal_writechar(' ', 15);
    if(!terminal_col)
    {
        terminal_row -= 1;
        terminal_col = VGA_WIDTH;
    }
    terminal_col -= 1;
}

void terminal_writechar(char c, char color)
{
    if (c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    if (c == 0x08)
    {
        terminal_backspace();
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

void print(const char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

static struct paging_4gb_chunk *kernel_chunk = 0;

void panic(const char* msg)
{
    print(msg);
    while(1) {}
}

void kernel_page()
{
    kernel_registers();
    paging_switch(kernel_chunk);
}

struct tss tss;

struct gdt gdt_real[DANOS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[DANOS_TOTAL_GDT_SEGMENTS] = {
    {.base = 0x00, .limit = 0x00, .type = 0x00 },             // NULL Segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x9a },       // Kernel code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x92 },       // Kernel data segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf8 },       // User code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf2 },       // User data segment
    {.base = (uint32_t)&tss, .limit = sizeof(tss), .type = 0xe9 }, // TSS Segment
};

void kernel_main()
{
    terminal_init();

    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, DANOS_TOTAL_GDT_SEGMENTS);
    // Load the GDT
    gdt_load(gdt_real, sizeof(gdt_real));

    // Initialize the heap
    kheap_init();

    // Initialize filesystems
    fs_init();

    // Search and initialize disk
    disk_search_and_init();

    // Initialize the interrupt descriptor table
    idt_init();

    // Setup the TSS
    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000; // Where kernel stack is located, when we go from userland to kernel land, sp is changed to this
    tss.ss0 = KERNEL_DATA_SELECTOR;

    // Load TSS
    tss_load(0x28); // Is offset in gdt_real

    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    // Switch to kernel paging chunk
    paging_switch(kernel_chunk);

    // Enable paging
    enable_paging();

    // Register kernel commands
    isr80h_register_commands();

    // Initialized all the system keyboards
    keyboard_init();

    struct process* process = 0;
    int res = process_load_switch("0:/blank.elf", &process);
    if (res != DANOS_ALL_OK)
    {
        panic("Failed to load blank.elf\n");
    }

    struct command_argument argument;
    strcpy(argument.argument, "Testing!");
    process_inject_arguments(process, &argument);

    argument.next = 0x00;

    res = process_load_switch("0:/blank.elf", &process);
    if (res != DANOS_ALL_OK)
    {
        panic("Failed to load blank.elf\n");
    }

    strcpy(argument.argument, "abc!");
    process_inject_arguments(process, &argument);

    argument.next = 0x00;

    task_run_first_ever_task();

    while(1) {}
}