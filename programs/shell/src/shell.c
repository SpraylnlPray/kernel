#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "danos.h"

int main(int argc, char **argv)
{
    print("DanOS v1.0.0\n");
    char *tmp = "ls.elf 0:/src";
    danos_system_run(tmp);
    while (1)
    {
        print("> ");
        char buf[1024];
        danos_terminal_readline(buf, sizeof(buf), true);
        print("\n");
        danos_system_run(buf);
    };
    return 0;
}