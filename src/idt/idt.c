#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"
#include "task/task.h"
#include "config.h"
#include "status.h"
#include "task/process.h"

extern void* interrupt_pointer_table[DANOS_TOTAL_INTERRUPTS];

struct idt_desc idt_descriptors[DANOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

static INTERRUPT_CALLBACK_FUNCTION interrupt_callbacks[DANOS_TOTAL_INTERRUPTS];
static ISR80H_COMMMAND isr80h_commands[DANOS_MAX_ISR80H_COMMANDS];

extern void idt_load(struct idtr_desc *ptr);
extern void no_interrupt();
extern void isr80h_wrapper();

void no_interrupt_handler()
{
    outb(0x20, 0x20);
}

void interrupt_handler(int interrupt, struct interrupt_frame* frame)
{
    if (interrupt_callbacks[interrupt] != 0)
    {
        kernel_page();
        task_current_save_state(frame);
        interrupt_callbacks[interrupt](frame);
        task_page();
    }

    outb(0x20, 0x20); // Acknowledgement for interrupt controller
}

void idt_zero()
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void *address)
{
    struct idt_desc *desc = &idt_descriptors[interrupt_no];
    desc->offset_1 = (int32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xee;
    desc->offset_2 = (uint32_t) address >> 16;
}

void idt_handle_exception()
{
    process_terminate(task_current()->process);
    task_next();
}

void idt_clock()
{
    outb(0x20, 0x20); // Acknowledgement for interrupt controller
    task_next(); // Switch to the next task
}

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    for (int i = 0; i < DANOS_TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, interrupt_pointer_table[i]);
    }

    idt_set(0, idt_zero);
    idt_set(0x80, isr80h_wrapper);

    for (int i = 0; i < 0x20; i++) // same interrupt handler for all exceptions at the moment
    {
        idt_register_interrupt_callback(i, idt_handle_exception);
    }

    idt_register_interrupt_callback(0x20, idt_clock);

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}

int idt_register_interrupt_callback(int interrupt, INTERRUPT_CALLBACK_FUNCTION interrupt_callback)
{
    if (interrupt < 0 || interrupt >= DANOS_TOTAL_INTERRUPTS)
    {
        return -DANOS_EINVARG;
    }

    if (interrupt_callbacks[interrupt])
    {
        panic("You are attemtping to overwrite an existing interrupt calback!\n");
    }

    interrupt_callbacks[interrupt] = interrupt_callback;
    return 0;
}

void isr80h_register_command(int command_id, ISR80H_COMMMAND command)
{
    if (command_id < 0 || command_id >= DANOS_MAX_ISR80H_COMMANDS)
    {
        panic("Command is out of bounds!\n");
    }

    if (isr80h_commands[command_id])
    {
        panic("You are attemtping to overwrite an existing command\n");
    }

    isr80h_commands[command_id] = command;
}

void* isr80h_handle_command(int command, struct interrupt_frame *frame)
{
    void* result = 0;

    if (command < 0 || command >= DANOS_MAX_ISR80H_COMMANDS)
    {
        return result;
    }

    ISR80H_COMMMAND command_func = isr80h_commands[command];
    if (!command_func)
    {
        return result;
    }

    result = command_func(frame);
    return result;
}

void *isr80h_handler(int command, struct interrupt_frame *frame)
{
    void *res = 0;
    kernel_page();
    task_current_save_state(frame);
    res = isr80h_handle_command(command, frame);
    task_page();
    return res;
}