#include "danos.h"
#include "stdlib.h"

int main(int arg, char** argv)
{
    print("Hello how are you!\n");

    print(itoa(8763));

    void* ptr = malloc(512);
    free(ptr);
    ptr = malloc(512);
    void* ptr2 = malloc(512);

    while(1)
    {
        if (getkey() != 0)
        {
            print("key was pressed!\n");
        }
    }
    return 0;
}