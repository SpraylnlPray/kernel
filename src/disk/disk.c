#include "disk.h"
#include "io/io.h"
#include "memory/memory.h"
#include "config.h"
#include "status.h"

// primary hard disk
struct disk disk;

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

void disk_search_and_init()
{ 
    memset(&disk, 0, sizeof(disk));
    disk.type = DANOS_DISK_TYPE_REAL;
    disk.sector_size = DANOS_SECTOR_SIZE;
    // in a better implementation we would search for a free index and assign that, for now we assign it directly
    disk.id = 0;
    disk.filesystem = fs_resolve(&disk);
}

struct disk* disk_get(int index)
{
    if (index != 0)
        return 0;
    
    return &disk;
}

int disk_read_block(struct disk *idisk, unsigned int lba, int total, void* buf)
{
    if (idisk != &disk)
        return -DANOS_EINVARG;

    return disk_read_sector(lba, total, buf);
}