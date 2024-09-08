section .asm

extern int21h_handler
extern no_interrupt_handler
extern isr80h_handler
extern interrupt_handler

global idt_load
global no_interrupt
global enable_interrupts
global disable_interrupts
global isr80h_wrapper
global interrupt_pointer_table

disable_interrupts:
    cli
    ret

enable_interrupts:
    sti
    ret

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]
    lidt [ebx]

    pop ebp
    ret

%macro interrupt 1
    global int%1
    int%1:
        ; Interrupt frame start
        ; Already pushed to us by the processor upon entry to this interrupt
        ; uint32_t ip
        ; uint32_t cs
        ; uint32_t flags
        ; uint32_t sp;
        ; uint32_t ss;

        ; Pushes the general purpose registers to the stack
        pushad
        ; Interrupt frame end
        push esp
        push dword %1
        call interrupt_handler
        add esp, 8
        popad
        iret
%endmacro

%assign i 0
%rep 512
    interrupt i
%assign i i+1
%endrep

no_interrupt:
    pushad
    call no_interrupt_handler
    popad
    iret

isr80h_wrapper:
    ; Because of type of interrupt the processor will disable interruptsautomatically

    ; Interrupt frame start
    ; Already pushed to us by the processor upon entry to this interrupt
    ; uint32_t ip
    ; uint32_t cs
    ; uint32_t flags
    ; uint32_t sp;
    ; uint32_t ss;

    ; Pushes the general purpose registers to the stack
    pushad

    ; Interrupt frame end

    ; Push the stakc pointer so that we are pointing to the interrupt frame; contains all of the above values
    push esp

    ; eax holds our command, lets push it to the stack for isr80h_handler
    push eax
    call isr80h_handler
    mov dword[tmp_res], eax
    add esp, 8 ; modify stack as we pushed twice 4 bytes before

    ; Restore general purpose registers for userland
    popad

    mov eax, [tmp_res]
    iretd

section .data
; Inside here is the return result from isr80h_handler
tmp_res: dd 0

; Size of address in 32 bit architecture; will be replaced with dd int5 when calling interrupt_array_entry 5 below. int5 label is created above in macro
%macro interrupt_array_entry 1
    dd int%1
%endmacro

interrupt_pointer_table:
%assign i 0
%rep 512
    interrupt_array_entry i
%assign i i+1
%endrep