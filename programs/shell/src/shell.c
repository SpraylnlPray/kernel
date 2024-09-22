#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "danos.h"

int main(int argc, char** argv)
{
    print ("DanOS v1.0.0\n");
    while(1)
    {
        print("> ");
        char buf[1024];
        danos_terminal_readline(buf, sizeof(buf), true);
        danos_process_load_start(buf);
        print("\n");

    };
    return 0;
}