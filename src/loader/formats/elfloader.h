#ifndef ELFLOADER_H
#define ELFLOADER_H

#include <stdint.h>
#include <stddef.h>

#include "elf.h"
#include "config.h"

struct elf_file
{
    char filename[DANOS_MAX_PATH];

    int in_memory_size;

    // pointer to physical memory address where this elf-file is loaded at
    void * elf_memory;

    // virtual base address of this binary
    void * virtual_base_address;

    // The ending virtual address
    void * virtual_end_address;

    // The physical base address
    void* physical_base_address;

    // Physical end address of this binary
    void* physical_end_address;
};

int elf_load(const char* filename, struct elf_file** file_out);
void elf_close(struct elf_file* file);

#endif