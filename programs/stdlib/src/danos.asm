[BITS 32]

section .asm

global print:function
global getkey:function
global danos_malloc:function
global danos_free:function

; void print(const char* message)
print:
    push ebp
    mov ebp, esp

    push dword[ebp + 8] ; we pushed bp 4 bytes + return address
    mov eax, 1 ; Command print
    int 0x80

    add esp, 4 ; cause pushing uses 4 bytes, restore stack

    pop ebp
    ret

; int getkey()
getkey:
    push ebp

    mov ebp, esp
    mov eax, 2 ; Command getkey
    int 0x80

    pop ebp
    ret

; void* danos_malloc(size_t size)
danos_malloc:
    push ebp
    mov ebp, esp

    mov eax, 4 ; Command malloc
    push dword[ebp + 8] ; Variable "size"
    int 0x80

    add esp, 4

    pop ebp
    ret

J; void danos_free(void* ptr)
danos_free:
    push ebp
    mov ebp, esp

    mov eax, 5 ; Command free
    push dword[ebp + 8] ; Variable "ptr"
    int 0x80

    add esp, 4

    pop ebp
    ret