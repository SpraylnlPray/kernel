FILES=./build/blank.o 
INCLUDES=
BUILD_DIRS := ./build

all: directories ${FILES}
	@echo "$@ start"
	i686-elf-gcc -g -T ./linker.ld -o ./blank.elf -ffreestanding -O0 -nostdlib -fpic -g ${FILES} ../stdlib/stdlib.elf
	@echo "$@ finished"

./build/blank.o: ./blank.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I./${FLAGS} -std=gnu99 -c ./blank.c -o ./build/blank.o
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