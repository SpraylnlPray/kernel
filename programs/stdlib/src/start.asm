[BITS 32]

global _start
extern c_start
extern danos_exit

section .asm

_start:
    call c_start
    call danos_exit
    ret