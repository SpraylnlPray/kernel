ORG 0x7c00
bits 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

jmp short start
nop

; FAT16 header
OEMIdentifier               db 'DANOS   ' ; Must be 8 bytes, padding is done with spaces!
BytesPerSector              dw 0x200 ; 512 bytes per sector, changing it doesn't necessarily mean the bps change, the way the disk works is fixed, generally ignored by most kernels
SectorsPerCluster           db 0x80
ReservedSectors             dw 200 ; Reserved for kernel, is plenty, whole kernel will be stored here, not stored in a file
FATCopies                   db 0x02 ; Original + backup
RootDirEntries              dw 0x40
NumSectors                  dw 0x00
MediaType                   db 0xF8
SectorsPerFat               dw 0x100
SectorsPerTrack             dw 0x20
NumberOfHeads               dw 0x40
HiddenSectors               dd 0x00
SectorsBig                  dd 0x773594

; Extended BPB (Dos 4.0)
DriveNumber                 db 0x80
WinNTBit                    db 0x00
Signature                   db 0x29
VolumeID                    dd 0xD105
VolumeIDString              db 'DANOS BOOT ' ; Must be 11 bytes, padding is done with spaces!
SystemIDString              db 'FAT16   ' ; Must be 8 bytes, padding is done with spaces!

start:
    jmp 0:step2

step2:
    cli ; Clear Interrupts
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov sp, 0x7c00
    sti ; Enables Interrupts

.load_protected:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32
    jmp $

; gtd
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:       ; CS should point to this
    dw 0xffff   ; Segment limit first 0-15 bits
    dw 0        ; Base first 0-15 bits
    db 0        ; Base 16-23 bits
    db 0x9a     ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

; offset 0x10
gdt_data:       ; DS, SS, ES, FS, GS
    dw 0xffff   ; Segment limit first 0-15 bits
    dw 0        ; Base first 0-15 bits
    db 0        ; Base 16-23 bits
    db 0x92     ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1 ; starting sector, not 0 cause thats the boot sector
    mov ecx, 100 ; total number of sectors
    mov edi, 0x0100000 ; where to load them into
    call ata_lba_read
    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax ; backup the lba
    ; send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xe0 ; Select the master drive
    mov dx, 0x1f6
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1f2
    out dx, al
    ; Finishe dsending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx
    mov dx, 0x1f3
    out dx, al
    ; Finsihed sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1f4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finsihed sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1f5
    mov eax, ebx ; Restore LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Check if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 Words at a time
    mov ecx, 256 ; 256 words -> 512 bytes -> one sector
    mov dx, 0x1f0
    rep insw ; Stores 256-times from port in dx and stores it in location in edi (set above)
    pop ecx
    loop .next_sector ; Also decrements ecx
    ; End of reading sectors into memory
    ret


times 510-($ - $$) db 0
dw 0xAA55
