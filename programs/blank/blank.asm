[BITS 32]

section .asm

global _start

_start:

    push 20
    push 30
    mov eax, 0 ; Which command do we want to run? -> Command 0 (sum) we binded in isr80h.c
    int 0x80
    add esp, 8

    jmp $