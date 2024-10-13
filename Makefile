INCLUDES= -I./src/kernel -I./src
FLAGS= -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

BUILD_DIR=./build
SRC_DIR=./src
BIN_DIR=./bin

C_FILES=$(shell find ${SRC_DIR} -name "*.c")
ASM_FILES=$(shell find ${SRC_DIR} -name "*.asm")
OBJS=$(patsubst ${SRC_DIR}/%.asm,${BUILD_DIR}/%.asm.o,${ASM_FILES}) $(patsubst ${SRC_DIR}/%.c,${BUILD_DIR}/%.o,${C_FILES})
OBJ_FILES=$(filter-out ${BUILD_DIR}/boot/boot.asm.o,${OBJS}) # boot binary is built differently

export PREFIX := $(HOME)/opt/cross
export TARGET := i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

.PHONY: all clean programs show

show:
	@echo c files: ${C_FILES}
	@echo asm files: ${ASM_FILES}
	@echo obj files: ${OBJ_FILES}

all: ./bin/boot/boot.bin ./bin/kernel.bin programs
	@echo "Make all start"
	rm -rf ./bin/os.bin 
	dd if=./bin/boot/boot.bin >> ./bin/os.bin
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

.PHONY:%.dir
%.dir:
	@echo "## $@ start"
	mkdir -p $(dir $@)
	@echo "## $@ finished"

./bin/kernel.bin: ${OBJ_FILES} ./bin/kernel.bin.dir
	@echo "$@ start"
	i686-elf-ld -g -relocatable ${OBJ_FILES} -o ./build/kernelfull.o
	i686-elf-gcc ${FLAGS} -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o
	@echo "$@ finished"

./bin/boot/boot.bin: ./src/boot/boot.asm ./bin/boot/boot.bin.dir
	@echo "$@ start"
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot/boot.bin
	@echo "$@ finished"

${BUILD_DIR}/%.asm.o: ${SRC_DIR}/%.asm ${BUILD_DIR}/%.dir
	@echo "## $@ start"
	nasm -f elf -g $< -o $@
	@echo "## $@ finished"

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c ${BUILD_DIR}/%.dir
	@echo "## $@ start"
	i686-elf-gcc ${INCLUDES} -I$(dir $<) ${FLAGS} -std=gnu99 -c $< -o $@
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