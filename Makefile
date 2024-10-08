FILES= ./build/kernel.asm.o ./build/gdt/gdt.o ./build/isr80h/keyboard.o ./build/loader/formats/elf.o ./build/loader/formats/elfloader.o ./build/isr80h/misc.o ./build/isr80h/io.o ./build/keyboard/classic.o ./build/keyboard/keyboard.o ./build/isr80h/heap.o  ./build/task/task.o ./build/task/task.asm.o ./build/task/process.o ./build/gdt/gdt.asm.o ./build/kernel.o ./build/task/tss.asm.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/memory/paging/paging.o ./build/memory/paging/paging.asm.o ./build/disk/disk.o ./build/disk/streamer.o ./build/fs/pparser.o ./build/fs/file.o ./build/string/string.o ./build/fs/fat/fat16.o ./build/isr80h/process.o ./build/isr80h/isr80h.o
INCLUDES= -I./src
FLAGS= -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
BUILD_DIRS= ./build ./build/loader ./build/loader/formats ./build/idt ./build/memory ./build/io ./build/isr80h ./build/task ./build/memory/heap ./build/memory/paging ./build/disk ./build/fs ./build/fs/fat ./build/string ./build/gdt ./build/keyboard
BIN_DIRS= ./bin

BUILD_DIR=./build
SRC_DIR=./src

export PREFIX := $(HOME)/opt/cross
export TARGET := i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

.PHONY: all clean directories programs

all: directories ./bin/boot.bin ./bin/kernel.bin programs
	@echo "Make all start"
	rm -rf ./bin/os.bin 
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin
	# TODO: Create /mnt/ dir!
	sudo mount -t vfat ./bin/os.bin /mnt/d
	# Copy a file over
	sudo cp ./hello.txt /mnt/d
	sudo cp ./programs/blank/blank.elf /mnt/d
	sudo cp ./programs/shell/shell.elf /mnt/d
	sudo cp ./programs/keyboard/keyboard.elf /mnt/d
	sudo umount /mnt/d
	@echo "Make all finished"

directories: $(BIN_DIRS)

.PHONY:%.dir
%.dir:
	@echo "## $@ start"
	mkdir -p $(dir $@)
	@echo "## $@ finished"

# $(BUILD_DIRS):
# 	@echo "Create build directories: $@"
# 	@mkdir $@
# 	@echo "Build directories created: $@"

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

${BUILD_DIR}/%.asm.o: ${SRC_DIR}/%.asm ${BUILD_DIR}/%.dir
	@echo "## $@ start"
	nasm -f elf -g $< -o $@
	@echo "## $@ finished"

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c ${BUILD_DIR}/%.dir
	@echo "## $@ start"
	i686-elf-gcc ${INCLUDES} -I$(dir $^) ${FLAGS} -std=gnu99 -c $< -o $@
	@echo "## $@ finished"

# TODO: Extra include of -I./src/fs
./build/fs/fat/fat16.o: ./src/fs/fat/fat16.c ./build/fs/fat/fat16.dir
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/fs -I./src/fs/fat ${FLAGS} -std=gnu99 -c ./src/fs/fat/fat16.c -o ./build/fs/fat/fat16.o
	@echo "$@ finished"

programs:
	cd ./programs/stdlib && $(MAKE) all
	cd ./programs/blank && $(MAKE) all
	cd ./programs/shell && $(MAKE) all
	cd ./programs/keyboard && $(MAKE) all

programs_clean:
	cd ./programs/stdlib && $(MAKE) clean
	cd ./programs/blank && $(MAKE) clean
	cd ./programs/shell && $(MAKE) clean
	cd ./programs/keyboard && $(MAKE) clean

clean: programs_clean
	@echo "$@ start"
	rm -rf ./bin
	rm -rf ${FILES}
	rm -rf ${BUILD_DIR}
	@echo "$@ finished"