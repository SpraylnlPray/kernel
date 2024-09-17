#include "danos.h"

int main(int arg, char** argv)
{
    print("Hello how are you!\n");

    while(1)
    {
        if (getkey() != 0)
        {
            print("key was pressed!\n");
        }
    }
    return 0;
}