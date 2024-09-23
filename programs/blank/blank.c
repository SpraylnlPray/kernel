#include "danos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int main(int arg, char** argv)
{
    char str[] = "hello world";
    struct command_argument* root_command = danos_parse_command(str, sizeof(str));

    printf("%s\n", root_command->argument);
    printf("%s\n", root_command->next->argument);

    while(1)
    {
        
    }
    return 0;
}