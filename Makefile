INCLUDES= -I./src/kernel -I./src
FLAGS= -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

BUILD_DIR=./build
SRC_DIR=./src
BIN_DIR=./bin

BINARIES=./bin/boot/boot.asm.bin ./bin/kernel.bin
C_FILES=$(shell find ${SRC_DIR} -name "*.c")
ASM_FILES=$(shell find ${SRC_DIR} -name "*.asm")
OBJS=$(patsubst ${SRC_DIR}/%.asm,${BUILD_DIR}/%.asm.o,${ASM_FILES}) $(patsubst ${SRC_DIR}/%.c,${BUILD_DIR}/%.c.o,${C_FILES})
OBJ_FILES=$(filter-out ${BUILD_DIR}/boot/boot.asm.o,${OBJS}) # boot.asm is used for boot binary and used differently

export PREFIX := $(HOME)/opt/cross
export TARGET := i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

.PHONY:all
all: ./bin/os.bin

./bin/os.bin: ${BINARIES} programs
	@echo "## $@ start"
	rm -rf ./bin/os.bin
	dd if=./bin/boot/boot.asm.bin >> ./bin/os.bin
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
	@echo "## $@ finished"

./bin/kernel.bin: ${OBJ_FILES}
	@echo "$@ start"
	i686-elf-ld -g -relocatable ${OBJ_FILES} -o ${BUILD_DIR}/kernelfull.o
	i686-elf-gcc ${FLAGS} -T ./src/linker.ld -o ${BIN_DIR}/kernel.bin -ffreestanding -O0 -nostdlib ${BUILD_DIR}/kernelfull.o
	@echo "$@ finished"

${BIN_DIR}/%.asm.bin: ${SRC_DIR}/%.asm ${BIN_DIR}/%.dir
	@echo "## $@ start"
	nasm -f bin $< -o $@
	@echo "## $@ finished"

${BUILD_DIR}/%.asm.o: ${SRC_DIR}/%.asm ${BUILD_DIR}/%.dir
	@echo "## $@ start"
	nasm -f elf -g $< -o $@
	@echo "## $@ finished"

${BUILD_DIR}/%.c.o: ${SRC_DIR}/%.c ${BUILD_DIR}/%.dir
	@echo "## $@ start"
	i686-elf-gcc ${INCLUDES} -I$(dir $<) ${FLAGS} -std=gnu99 -c $< -o $@
	@echo "## $@ finished"

# TODO: Extra include of -I./src/fs
./build/fs/fat/fat16.c.o: ./src/fs/fat/fat16.c ./build/fs/fat/fat16.dir
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./src/fs -I./src/fs/fat ${FLAGS} -std=gnu99 -c ./src/fs/fat/fat16.c -o ./build/fs/fat/fat16.c.o
	@echo "$@ finished"

.PHONY:programs
programs:
	cd ./programs/stdlib && $(MAKE) all
	cd ./programs/blank && $(MAKE) all
	cd ./programs/shell && $(MAKE) all
	cd ./programs/keyboard && $(MAKE) all

.PHONY:programs_clean
programs_clean:
	cd ./programs/stdlib && $(MAKE) clean
	cd ./programs/blank && $(MAKE) clean
	cd ./programs/shell && $(MAKE) clean
	cd ./programs/keyboard && $(MAKE) clean

.PHONY:clean
clean: programs_clean
	@echo "$@ start"
	rm -rf ./bin
	rm -rf ${FILES}
	rm -rf ${BUILD_DIR}
	@echo "$@ finished"

.PHONY:show
show:
	@echo C_FILES: ${C_FILES}
	@echo ASM_FILES: ${ASM_FILES}
	@echo OBJ_FILES: ${OBJ_FILES}
	@echo BINARIES: ${BINARIES}

.PHONY:%.dir
%.dir:
	@echo "## $@ start"
	mkdir -p $(dir $@)
	@echo "## $@ finished"
