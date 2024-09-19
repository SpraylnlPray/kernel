FILES=./build/blank.o 
INCLUDES= -I../stdlib/src
BUILD_DIRS= ./build
FLAGS= -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: directories ${FILES}
	@echo "$@ start"
	i686-elf-gcc -g -T ./linker.ld -o ./blank.elf -ffreestanding -O0 -nostdlib -fpic -g ${FILES} ../stdlib/stdlib.elf
	@echo "$@ finished"

./build/blank.o: ./blank.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./ ${FLAGS} -std=gnu99 -c ./blank.c -o ./build/blank.o
	@echo "$@ finished"

clean:
	rm -rf ${FILES}
	rm -rf ./build
	rm ./blank.elf

directories: $(BUILD_DIRS)

$(BUILD_DIRS):
	@echo "Create build directories: $@"
	@mkdir $@
	@echo "Build directories created: $@"