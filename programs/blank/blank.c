#include "danos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int main(int arg, char** argv)
{
    struct process_arguments arguments;
    danos_process_get_arguments(&arguments);

    printf("%i %s\n", arguments.argc, arguments.argv[0]);

    while(1)
    {
        
    }
    return 0;
}