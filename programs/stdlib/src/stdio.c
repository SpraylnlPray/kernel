#include "stdio.h"
#include "danos.h"

int putchar(int c)
{
    danos_putchar((char)c);
    return 0;
}