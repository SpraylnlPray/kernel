[BITS 32]

section .asm

global print:function
global danos_getkey:function
global danos_malloc:function
global danos_free:function
global danos_putchar:function
global danos_process_load_start:function
global danos_process_get_arguments:function
global danos_system:function
global danos_exit:function
global danos_get_keyboard_layouts:function
global danos_get_keyboard_layout_count:function
global danos_get_active_keyboard_layout_id:function
global danos_set_active_keyboard_layout: function

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

; int danos_getkey()
danos_getkey:
    push ebp
    mov ebp, esp

    mov eax, 2 ; Command getkey
    int 0x80

    pop ebp
    ret

; void danos_putchar(char c)
danos_putchar:
    push ebp,
    mov ebp, esp

    mov eax, 3 ; Command putchar
    push dword [ebp + 8] ; Variable "c"
    int 0x80
    add esp, 4

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

; void danos_free(void* ptr)
danos_free:
    push ebp
    mov ebp, esp

    mov eax, 5 ; Command free
    push dword[ebp + 8] ; Variable "ptr"
    int 0x80

    add esp, 4

    pop ebp
    ret

; void danos_process_load_start(const char* filename)
danos_process_load_start:
    push ebp
    mov ebp, esp
    
    mov eax, 6 ; Command 6 process load start (starts a process)
    push dword[ebp + 8] ; Variable "filename"
    int 0x80

    add esp, 4

    pop ebp
    ret

; int danos_system(struct command_argument* arguments)
danos_system:
    push ebp
    mov ebp, esp
    
    mov eax, 7 ; Command 7 process_system (runs a system command based on the arguments)
    push dword[ebp + 8] ; Variable "arguments"
    int 0x80

    add esp, 4

    pop ebp
    ret

; void danos_process_get_arguments(struct process_arguments* arguments)
danos_process_get_arguments:
    push ebp
    mov ebp, esp

    mov eax, 8 ; Command 8 Gets the process arugments
    push dword[ebp + 8] ; Variable "arguments"
    int 0x80

    add esp, 4

    pop ebp
    ret

; void danos_exit()
danos_exit:
    push ebp
    mov ebp, esp

    mov eax, 9 ; Command 9 process exit
    int 0x80

    pop ebp
    ret

; void* danos_get_keyboard_layouts(char** buf, int size)
danos_get_keyboard_layouts:
    push ebp
    mov ebp, esp

    mov eax, 10 ; Command 10 get keyboard layouts
    push dword[ebp + 8] ; Variable "size"
    push dword[ebp + 12] ; Variable "buf"
    int 0x80

    add esp, 8

    pop ebp
    ret

; int danos_get_keyboard_layout_count()
danos_get_keyboard_layout_count:
    push ebp
    mov ebp, esp

    mov eax, 11 ; Command 11 get keyboard layout count
    int 0x80

    pop ebp
    ret

; void* danos_get_active_keyboard_layout_id(char* buf, int size)
danos_get_active_keyboard_layout_id:
    push ebp
    mov ebp, esp

    mov eax, 12 ; Command 12 get active layout id
    push dword[ebp + 8] ; Variable "size"
    push dword[ebp + 12] ; Variable "buf"
    int 0x80

    add esp, 8

    pop ebp
    ret

; int danos_set_active_keyboard_layout(char* id)
danos_set_active_keyboard_layout:
    push ebp
    mov ebp, esp

    mov eax, 13 ; Commad 13 set active layout
    push dword[ebp + 8] ; Variable "id"
    int 0x80

    add esp, 4

    pop ebp
    ret
