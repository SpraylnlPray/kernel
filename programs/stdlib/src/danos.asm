[BITS 32]

global print:function
global getkey:function

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
