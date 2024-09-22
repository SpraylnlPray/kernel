#include "danos.h"

int danos_getkey_block()
{
    int val = 0;
    do
    {
        val = danos_getkey();
    } while (val == 0);

    return val;
}

void danos_terminal_readline(char* out, int max, bool output_while_typing)
{
    int i = 0;
    for (i = 0; i < max - 1; i++)
    {
        char key = danos_getkey_block();

        // carriage return means we have read the line
        if (key == 13)
        {
            break;
        }

        if (output_while_typing)
        {
            danos_putchar(key);
        }

        // Backspace
        if (key == 0x08 && i >= 1)
        {
            out[i - 1] = 0x00;
            i -= 2; // -2 because we +1 when we go to continue
            continue;
        }

        out[i] = key;
    }

    // Add the null terminator;
    out[i] = 0x00;
}