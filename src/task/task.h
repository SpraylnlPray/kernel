#ifndef TASK_H
#define TASK_H

#include "config.h"
#include "memory/paging/paging.h"
#include "process.h"

struct process;
struct interrupt_frame;

struct task* task_new(struct process* process);
struct task* task_current();
struct task* task_get_next();
int task_free(struct task* task);

int task_init(struct task* task, struct process* process);
int task_switch(struct task* task);
int task_page();

void task_run_first_ever_task();

struct registers
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t esp;
    uint32_t ss;
};


struct task
{
    /**
     * The page directory of the task
     */
    struct paging_4gb_chunk* page_directory;

    // The registers of the task when the task is not running
    struct registers registers;

    // The process of the task
    struct process* process;

    // The next task in the linked list
    struct task* next;

    // Previous task in linked list
    struct task* prev;
};

void restore_general_purpose_registers(struct registers* regs);
void task_return(struct registers* regs); // will drop us into userland
void user_registers();
void task_current_save_state(struct interrupt_frame *frame);
int copy_string_from_task(struct task* task, void* virtual, void* phys, int max);
void* task_get_stack_item(struct task* task, int index);
int task_page_task(struct task* task);
void* task_virtual_address_to_physical(struct task* task, void* virtual_address);
void task_next();

#endif