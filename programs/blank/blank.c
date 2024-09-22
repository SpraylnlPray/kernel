#include "danos.h"
#include "stdlib.h"
#include "stdio.h"

int main(int arg, char** argv)
{
    printf("My age is %i\n", 27);
    print("Hello how are you!\n");

    // print(itoa(8763));

    // putchar('Z');

    void* ptr = malloc(512);
    free(ptr);
    ptr = malloc(512);
    void* ptr2 = malloc(512);
    free(ptr2);

    char buf[1024];
    danos_terminal_readline(buf, sizeof(buf), true);

    print(buf);

    // print("Hello world\n");

    while(1)
    {
        
    }
    return 0;
}