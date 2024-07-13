#ifndef CONFIG_H
#define CONFIG_H

#define DANOS_TOTAL_INTERRUPTS 512

#define KERNEL_CODE_SELECTOR 0x08 // code seg from kernel.asm
#define KERNEL_DATA_SELECTOR 0x10

// 100MB heap size
#define DANOS_HEAP_SIZE_BYTES 104857600
#define DANOS_HEAP_BLOCK_SIZE 4096
#define DANOS_HEAP_ADDRESS 0X01000000
#define DANOS_HEAP_TABLE_ADDRESS 0X00007E00

#define DANOS_SECTOR_SIZE 512

#define DANOS_MAX_FILESYSTEMS 12
#define DANOS_MAX_FILE_DESCRIPTORS 512

#endif