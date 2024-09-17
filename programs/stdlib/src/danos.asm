[BITS 32]

global print:function

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