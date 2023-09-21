#include "io/io.h"

int disk_read_sector(int lba, int total, void *buffer)
{
    outb(0x1f6, (lba >> 24) | 0xe0);
    outb(0x1f2, total);
    outb(0x1f3, (unsigned char)(lba & 0xff));
    outb(0x1f4, (unsigned char)(lba >> 8));
    outb(0x1f5, (unsigned char)(lba >> 16));
    outb(0x1f7, 0x20);

    unsigned short *ptr = (unsigned short*)buffer; // because we'll read 2 bytes at a time

    for (int b = 0; b < total; b++)
    {
        // Wait until controller is ready to read from
        char c = insb(0x1f7);
        while(!(c & 0x08))
        {
            c = insb(0x1f7);
        }

        // Copy from hard disk to memory
        for (int i = 0; i < 256; i++)
        {
            *ptr = insw(0x1f0); // as we casted it to short, we'll change two bytes at a time
            ptr++;
        }        
    }

    return 0;
}