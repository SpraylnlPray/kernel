#include "stdlib.h"
#include "danos.h"
#include "string.h"

char *itoa(int i)
{
    static char text[12]; // data is in local data; address stays the same
    int loc = 11;
    text[11] = 0;
    char neg = 1;
    if (i >= 0)
    {
        neg = 0;
        i = -i;
    }

    while (i)
    {
        text[--loc] = '0' - (i % 10);
        i /= 10;
    }

    if (loc == 11)
        text[--loc] = '0';

    if (neg)
        text[--loc] = '-';

    return &text[loc];
}

bool isdigit(char c)
{
    return c >= ASCII_ZERO && c <= ASCII_NINE;
}

int ctoi(char c)
{
    if (!isdigit(c))
        return 0;
    
    return c - ASCII_ZERO;
}

int atoi(char *c)
{
    int res = 0;
    int len = strlen(c);

    for (int i = 0; i < len; i++)
    {
        if (!isdigit(c[i]))
            return 0;
        
        int val = ctoi(c[i]);
        res = res * 10;
        res += val;
    }

    return res;
}



void *malloc(size_t size)
{
    return danos_malloc(size);
}

void free(void *ptr)
{
    return danos_free(ptr);
}