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
    bool is_digit;

    va_start(ap, fmt);
    for (p = fmt; *p; p++)
    {
        if (*p != '%')
        {
            putchar(*p);
            continue;
        }

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
            is_digit = isdigit(*p);
            if (!is_digit)
            {
                putchar(*p);
                break;
            }

            // width specifier
            int total_width = 0;
            int val_width = 0;
            int digit_count = 0;
            while (is_digit)
            {
                digit_count++;
                p++;
                is_digit = isdigit(*p);
            }

            for (int i = 0; i < digit_count; i++)
            {
                p--; // move back to the first digit
            }
            
            char *buf = malloc(digit_count + 1);
            strncpy(buf, p, digit_count + 1);
            total_width = atoi(buf);

            for (int i = 0; i < digit_count - 1; i++)
            {
                p++; // jump over the digits one less because we are currently pointing to the first digit
            }

            switch (*++p)
            {
            case 'i':
                ival = va_arg(ap, int);
                char* temp = itoa(ival);
                val_width = strnlen(temp, 1024);
                print(temp);
                break;
            case 's':
                sval = va_arg(ap, char *);
                val_width = strnlen(sval, 1024);
                print(sval);
                break;
            default:
                break;
            }

            if (total_width > val_width)
            {
                for (int i = 0; i < (total_width - val_width); i++)
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