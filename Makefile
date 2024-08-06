FILES := ./build/kernel.asm.o ./build/gdt/gdt.o ./build/task/task.o ./build/task/task.asm.o ./build/task/process.o ./build/gdt/gdt.asm.o ./build/kernel.o ./build/task/tss.asm.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/memory/paging/paging.o ./build/memory/paging/paging.asm.o ./build/disk/disk.o ./build/disk/streamer.o ./build/fs/pparser.o ./build/fs/file.o ./build/string/string.o ./build/fs/fat/fat16.o
INCLUDES := -I./src
FLAGS := -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
BUILD_DIRS := ./build ./build/idt ./build/memory ./build/io ./build/task ./build/memory/heap ./build/memory/paging ./build/disk ./build/fs ./build/fs/fat ./build/string ./build/gdt
BIN_DIRS := ./bin

export PREFIX := $(HOME)/opt/cross
export TARGET := i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

.PHONY: all clean directories

all: directories ./bin/boot.bin ./bin/kernel.bin
	@echo "Make all start"
	rm -rf ./bin/os.bin 
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin
	# TODO: Create /mnt/ dir!
	sudo mount -t vfat ./bin/os.bin /mnt/d
	# Copy a file over
	sudo cp ./hello.txt /mnt/d
	sudo umount /mnt/d
	@echo "Make all finished"

directories: $(BUILD_DIRS) $(BIN_DIRS)

$(BUILD_DIRS):
	@echo "Create build directories: $@"
	@mkdir $@
	@echo "Build directories created: $@"

$(BIN_DIRS):
	@echo "Create bin directories: $@"
	mkdir -p $@
	@echo "Bin directories created: $@"

./bin/kernel.bin: ${FILES}
	@echo "$@ start"
	i686-elf-ld -g -relocatable ${FILES} -o ./build/kernelfull.o
	i686-elf-gcc ${FLAGS} -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o
	@echo "$@ finished"

./bin/boot.bin: ./src/boot/boot.asm
	@echo "$@ start"
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin
	@echo "$@ finished"

./build/kernel.asm.o: ./src/kernel.asm
	@echo "$@ start"
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o
	@echo "$@ finished"

./build/kernel.o: ./src/kernel.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o
	@echo "$@ finished"

./build/idt/idt.asm.o: ./src/idt/idt.asm
	@echo "$@ start"
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o
	@echo "$@ finished"

./build/idt/idt.o: ./src/idt/idt.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/idt -I./src ${FLAGS} -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o
	@echo "$@ finished"

./build/gdt/gdt.o: ./src/gdt/gdt.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/gdt -I./src ${FLAGS} -std=gnu99 -c ./src/gdt/gdt.c -o ./build/gdt/gdt.o
	@echo "$@ finished"

./build/gdt/gdt.asm.o: ./src/gdt/gdt.asm
	@echo "$@ start"
	nasm -f elf -g ./src/gdt/gdt.asm -o ./build/gdt/gdt.asm.o
	@echo "$@ finished"

./build/memory/memory.o: ./src/memory/memory.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/memory ${FLAGS} -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o
	@echo "$@ finished"

./build/io/io.asm.o: ./src/io/io.asm
	@echo "$@ start"
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o
	@echo "$@ finished"

./build/task/tss.asm.o: ./src/task/tss.asm
	@echo "$@ start"
	nasm -f elf -g ./src/task/tss.asm -o ./build/task/tss.asm.o
	@echo "$@ finished"

./build/task/task.asm.o: ./src/task/task.asm
	@echo "$@ start"
	nasm -f elf -g ./src/task/task.asm -o ./build/task/task.asm.o
	@echo "$@ finished"

./build/task/process.o: ./src/task/process.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/task ${FLAGS} -std=gnu99 -c ./src/task/process.c -o ./build/task/process.o
	@echo "$@ finished"

./build/task/task.o: ./src/task/task.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/task ${FLAGS} -std=gnu99 -c ./src/task/task.c -o ./build/task/task.o
	@echo "$@ finished"

./build/memory/heap/heap.o: ./src/memory/heap/heap.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/memory/heap ${FLAGS} -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o
	@echo "$@ finished"

./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/memory/heap ${FLAGS} -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o
	@echo "$@ finished"

./build/memory/paging/paging.o: ./src/memory/paging/paging.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/memory/paging ${FLAGS} -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o
	@echo "$@ finished"

./build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	@echo "$@ start"
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o
	@echo "$@ finished"

./build/disk/disk.o: ./src/disk/disk.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/disk ${FLAGS} -std=gnu99 -c ./src/disk/disk.c -o ./build/disk/disk.o
	@echo "$@ finished"

./build/disk/streamer.o: ./src/disk/streamer.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/disk ${FLAGS} -std=gnu99 -c ./src/disk/streamer.c -o ./build/disk/streamer.o
	@echo "$@ finished"

./build/fs/pparser.o: ./src/fs/pparser.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/fs ${FLAGS} -std=gnu99 -c ./src/fs/pparser.c -o ./build/fs/pparser.o
	@echo "$@ finished"

./build/string/string.o: ./src/string/string.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/string ${FLAGS} -std=gnu99 -c ./src/string/string.c -o ./build/string/string.o
	@echo "$@ finished"

./build/fs/file.o: ./src/fs/file.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/fs ${FLAGS} -std=gnu99 -c ./src/fs/file.c -o ./build/fs/file.o
	@echo "$@ finished"

./build/fs/fat/fat16.o: ./src/fs/fat/fat16.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/fs -I./src/fs/fat ${FLAGS} -std=gnu99 -c ./src/fs/fat/fat16.c -o ./build/fs/fat/fat16.o
	@echo "$@ finished"

clean:
	@echo "$@ start"
	rm -rf ./bin
	rm -rf ${FILES}
	rm -rf ./build
	@echo "$@ finished"