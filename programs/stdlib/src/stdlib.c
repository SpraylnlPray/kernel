#include "stdlib.h"
#include "danos.h"

void* malloc(size_t size)
{
    return danos_malloc(size);
}

void free(void* ptr)
{
    return danos_free(ptr);
}