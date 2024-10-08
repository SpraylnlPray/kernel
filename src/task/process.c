#include "process.h"
#include "memory/memory.h"
#include "status.h"
#include "memory/heap/kheap.h"
#include "fs/file.h"
#include "string/string.h"
#include "kernel.h"
#include "memory/paging/paging.h"
#include "loader/formats/elfloader.h"

// Currently running process
struct process* current_process = 0;

static struct process* processes[DANOS_MAX_PROCESSES] = {};

static void process_init(struct process* process)
{
    memset(process, 0, sizeof(struct process));
}

struct process* process_current()
{
    return current_process;
}

struct process* process_get(int process_id)
{
    if (process_id < 0 || process_id >= DANOS_MAX_PROCESSES)
        return NULL;

    return processes[process_id];
}

int process_switch(struct process* process)
{
    // save process state? not necessary right now because they share everything
    // try separate video memory?
    current_process = process;
    return 0;
}

static int process_find_free_allocation_index(struct process* process)
{
    int res = -DANOS_ENOMEM;

    for (int i = 0; i < DANOS_MAX_PROGRAMM_ALLOCATIONS; i++)
    {
        if (process->allocations[i].ptr == 0)
        {
            res = i;
            break;
        }
    }

    return res;
}

void* process_malloc(struct process* process, size_t size)
{
    void* ptr = kzalloc(size);
    if (!ptr)
    {
        goto out_err;
    }

    int index = process_find_free_allocation_index(process);
    if (index < 0)
    {
        goto out_err;
    }

    int res = paging_map_to(process->task->page_directory, ptr, ptr, paging_align_address(ptr + size), PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    if (res < 0)
    {
        goto out_err;
    }
    process->allocations[index].ptr = ptr;
    process->allocations[index].size = size;

    return ptr;

out_err:
    if (ptr)
    {
        kfree(ptr);
    }
    return 0;
}

static bool process_is_process_pointer(struct process* process, void* ptr)
{
    for (int i = 0; i < DANOS_MAX_PROGRAMM_ALLOCATIONS; i++)
    {
        if (process->allocations[i].ptr == ptr)
        {
            return true;
        }
    }

    return false;
}

static void process_allocation_unjoin(struct process* process, void* ptr)
{
    for (int i = 0; i < DANOS_MAX_PROGRAMM_ALLOCATIONS; i++)
    {
        if (process->allocations[i].ptr == ptr)
        {
            process->allocations[i].ptr = 0x00;
            process->allocations[i].size = 0;
        }
    }
}

static struct process_allocation* process_get_allocation_by_addr(void* addr, struct process* process)
{
    for (int i = 0; i < DANOS_MAX_PROGRAMM_ALLOCATIONS; i++)
    {
        if (process->allocations[i].ptr == addr)
            return &process->allocations[i];
    }

    return 0;
}

void process_free(struct process* process, void* ptr)
{
    // unlink the pagres from the process for the give address
    struct process_allocation* allocation = process_get_allocation_by_addr(ptr, process);
    if (!allocation)
    {
        // Oops its not our pointer
        return;
    }

    int res = paging_map_to(process->task->page_directory, allocation->ptr, allocation->ptr, paging_align_address(allocation->ptr + allocation->size), 0x00);
    if (res < 0)
    {
        return;
    }

    process_allocation_unjoin(process, ptr);
    kfree(ptr);
}

static int process_load_binary(const char* filename, struct process* process)
{
    int res = 0;
    void* program_data_ptr = 0x00;
    int fd = fopen(filename, "r");
    if (!fd)
    {
        res = -DANOS_EIO;
        goto out;
    }

    struct file_stat stat;
    res = fstat(fd, &stat);
    if (res != DANOS_ALL_OK)
    {
        goto out;
    }

    program_data_ptr = kzalloc(stat.filesize);
    if (!program_data_ptr)
    {
        res = -DANOS_ENOMEM;
        goto out;
    }

    if (fread(program_data_ptr, stat.filesize, 1, fd) != 1)
    {
        res = DANOS_EIO;
        goto out;
    }

    process->ptr = program_data_ptr;
    process->size = stat.filesize;
    process->filetype = PROCESS_FILE_TYPE_BINARY;

out:
    if (res < 0)
    {
        if (program_data_ptr)
        {
            kfree(program_data_ptr);
        }
    }
    fclose(fd);
    return res;
}

static int process_load_elf(const char* filename, struct process* process)
{
    int res = 0;

    struct elf_file* elf_file = 0;
    res = elf_load(filename, &elf_file);
    if (ISERR(res))
    {
        goto out;
    }

    process->filetype = PROCESS_FILE_TYPE_ELF;
    process->elf_file = elf_file;

out:
    return res;
}

static int process_load_data(const char* filename, struct process* process)
{
    int res = 0;
    res = process_load_elf(filename, process);
    if (res == -DANOS_EINFORMAT)
    {
        res = process_load_binary(filename, process);
    }
    return res;
}

int process_map_binary(struct process* process)
{
    int res = 0;

    // We're giving the user programm access to the memory area that we allocated in process_load_for_slot using kzalloc, using 0x400000
    paging_map_to(process->task->page_directory, (void*) DANOS_PROGRAM_VIRTUAL_ADDRESS, process->ptr, paging_align_address(process->ptr + process->size), PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE);

    return res;
}

static int process_map_elf(struct process* process)
{
    int res = 0;

    struct elf_file* elf_file = process->elf_file;
    struct elf_header* header = elf_header(elf_file);
    struct elf32_phdr* phdrs = elf_pheader(header);
    for (int i = 0; i < header->e_phnum; i++)
    {
        struct elf32_phdr* phdr = &phdrs[i];
        void* phdr_phys_address = elf_phdr_phys_address(elf_file, phdr);
        int flags = PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL;
        if (phdr->p_flags & PF_W)
        {
            flags |= PAGING_IS_WRITEABLE;
        }

        res = paging_map_to(process->task->page_directory, paging_align_to_lower_page((void*)phdr->p_vaddr), paging_align_to_lower_page(phdr_phys_address), paging_align_address(phdr_phys_address + phdr->p_memsz), flags);

        if (ISERR(res))
        {
            break;
        }
    }

    return res;
}

int process_map_memory(struct process* process)
{
    int res = 0;

    switch (process->filetype)
    {
    case PROCESS_FILE_TYPE_ELF:
        res = process_map_elf(process);
        break;
    case PROCESS_FILE_TYPE_BINARY:
        res = process_map_binary(process);
        break;
    default:
        panic("process_map_memory: Invalid filetype\n");
    }

    if (res < 0)
    {
        goto out;
    }

    paging_map_to(process->task->page_directory, (void*) DANOS_PROGRAMM_VIRTUAL_STACK_ADDRESS_END, process->stack, paging_align_address(process->stack + DANOS_USER_PROGRAM_STACK_SIZE), PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE);
out:
    return res;
}

int process_get_free_slot()
{
    for (int i = 0; i < DANOS_MAX_PROCESSES; i++)
    {
        if (processes[i] == 0)
        {
            return i;
        }
    }

    return -DANOS_EISTKN;
}

int process_load(const char* filename, struct process** process)
{
    int res = 0;

    int process_slot = process_get_free_slot();
    if (process_slot < 0)
    {
        res = -DANOS_EISTKN;
        goto out;
    }

    res = process_load_for_slot(filename, process, process_slot);

out:
    return res;
}

int process_load_switch(const char* filename, struct process** process)
{
    int res = process_load(filename, process);
    if (res == 0)
    {
        process_switch(*process);
    }

    return res;
}

int process_load_for_slot(const char* filename, struct process** process, int process_slot)
{
    int res = 0;
    struct task* task = 0;
    struct process* _process;
    void* programm_stack_ptr = 0;

    if (process_get(process_slot) != 0)
    {
        res = -DANOS_EISTKN;
        goto out;
    }

    _process = kzalloc(sizeof(struct process));
    if (!_process)
    {
        res = -DANOS_ENOMEM;
        goto out;
    }

    process_init(_process);
    res = process_load_data(filename, _process);
    if (res < 0)
    {
        goto out;
    }

    programm_stack_ptr = kzalloc(DANOS_USER_PROGRAM_STACK_SIZE);
    if (!programm_stack_ptr)
    {
        res = -DANOS_ENOMEM;
        goto out;
    }

    strncpy(_process->filename, filename, sizeof(_process->filename));
    _process->stack = programm_stack_ptr;
    _process->id = process_slot;

    // Create a task
    task = task_new(_process);
    if (ERROR_I(task) == 0)
    {
        res = ERROR_I(task);
        goto out;
    }

    _process->task = task;

    res = process_map_memory(_process);
    if (res < 0)
    {
        goto out;
    }

    *process = _process;

    // Add process to the array
    processes[process_slot] = _process;    

out:
    if (ISERR(res))
    {
        if (_process && _process->task)
        {
            task_free(_process->task);
        }

        // Free the process data
    }
    return res;
}

void process_get_arguments(struct process* process, int* argc, char*** argv)
{
    *argc = process->arguments.argc;
    *argv = process->arguments.argv;
}

int process_count_command_arguments(struct command_argument* root_argument)
{
    struct command_argument* current = root_argument;
    int i = 0;
    while (current)
    {
        i++;
        current = current->next;
    }

    return i;
}

int process_inject_arguments(struct process* process, struct command_argument* root_argument)
{
    int res = 0;
    struct command_argument* current = root_argument;
    int i = 0;
    int argc = process_count_command_arguments(root_argument);

    if (argc == 0)
    {
        res = -DANOS_EIO;
        goto out;
    }

    char **argv = process_malloc(process, sizeof(const char*) * argc);
    if (!argv)
    {
        res = -DANOS_ENOMEM;
        goto out;
    }

    while (current)
    {
        char* argument_str = process_malloc(process, sizeof(current->argument));
        if (!argument_str)
        {
            res = -DANOS_ENOMEM;
            goto out;
        }

        strncpy(argument_str, current->argument, sizeof(current->argument));
        argv[i] = argument_str;
        current = current->next;
        i++;
    }

    process->arguments.argc = argc;
    process->arguments.argv = argv;

out:
    return res;
}

static int process_terminate_allocations(struct process* process)
{
    for (int i = 0; i < DANOS_MAX_PROGRAMM_ALLOCATIONS; i++)
    {
        process_free(process, process->allocations[i].ptr);
    }

    return 0;
}

int process_free_program_data(struct process* process)
{
    int res = 0;
    switch (process->filetype)
    {
    case PROCESS_FILE_TYPE_BINARY:
        kfree(process->ptr);
        break;
    case PROCESS_FILE_TYPE_ELF:
        elf_close(process->elf_file);
        break;
    default:
        res = -DANOS_EINVARG;
        break;
    }

    return res;
}

void process_switch_to_any()
{
    for (int i = 0; i < DANOS_MAX_PROCESSES; i++)
    {
        if (processes[i])
        {
            process_switch(processes[i]);
            return;
        }
    }

    panic("No processes to switch to!\n");
}

static void process_unlink(struct process* process)
{
    processes[process->id] = 0x00;
    if (current_process == process)
    {
        process_switch_to_any();
    }
}

int process_terminate(struct process* process)
{
    int res = 0;

    res = process_terminate_allocations(process);
    if (res < 0)
    {
        print("Error terminating allocations\n");
        goto out;
    }

    res = process_free_program_data(process);
    if (res < 0)
    {
        print("Error freeing program data\n");
        goto out;
    }
    
    kfree(process->stack);
    task_free(process->task);
    process_unlink(process);

out:
    return res;
}
