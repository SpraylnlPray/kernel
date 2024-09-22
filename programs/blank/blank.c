#include "danos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int main(int arg, char** argv)
{
    char words[] = "hello how are you";
    const char* token = strtok(words, " ");
    while(token)
    {
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }
    printf("My age is %i\n", 27);
    

    while(1)
    {
        
    }
    return 0;
}