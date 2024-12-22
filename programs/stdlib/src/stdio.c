#include "stdio.h"

#include "danos.h"
#include "stdlib.h"
#include "string.h"
#include <stdarg.h>
#include <stdbool.h>

int putchar(int c)
{
    danos_putchar((char)c);
    return 0;
}

int printf(const char *fmt, ...)
{
    va_list ap;
    const char *p;
    char *sval;
    int ival;

    va_start(ap, fmt);
    for (p = fmt; *p; p++)
    {
        if (*p != '%')
        {
            putchar(*p);
            continue;
        }

        // When modifying the cases, also modify further down
        switch (*++p)
        {
        case 'i': // %i
            ival = va_arg(ap, int);
            print(itoa(ival));
            break;
        case 's':
            sval = va_arg(ap, char *);
            print(sval);
            break;
        default:
            if (!isdigit(*p))
            {
                putchar(*p);
                break;
            }

            // width specifier
            int min_width = 0;
            int value_width = 0;
            int digit_count = 0;
            do
            {
                digit_count++;
                p++;
            } while (isdigit(*p));

            for (int i = 0; i < digit_count; i++)
            {
                p--; // move back to the first digit
            }

            char *buf = malloc(digit_count + 1);
            strncpy(buf, p, digit_count + 1);
            min_width = atoi(buf);
            // free(buf); // TODO: Somehow this causes a crash

            for (int i = 0; i < digit_count - 1; i++)
            {
                p++; // jump over the digits, one less because we are currently pointing to the first digit
            }

            // TODO: find a better solution for this, its mostly duplicated from above
            switch (*++p)
            {
            case 'i':
                ival = va_arg(ap, int);
                char *temp = itoa(ival);
                value_width = strnlen(temp, 1024);
                print(temp);
                break;
            case 's':
                sval = va_arg(ap, char *);
                value_width = strnlen(sval, 1024);
                print(sval);
                break;
            default:
                break;
            }

            // Add the required padding
            if (min_width > value_width)
            {
                for (int i = 0; i < (min_width - value_width); i++)
                {
                    putchar(' ');
                }
            }
            break;
        }
    }

    va_end(ap);
    return 0;
}