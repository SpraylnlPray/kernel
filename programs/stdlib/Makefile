FILES=./build/start.o 
INCLUDES=
BUILD_DIRS := ./build

all: directories ${FILES}
	i686-elf-ld -m elf_i386 -relocatable ${FILES} -o ./stdlib.elf

./build/start.o: ./src/start.asm
	@echo "$@ start"
	nasm -f elf ./src/start.asm -o ./build/start.o
	@echo "$@ finished"

clean:
	rm -rf ${FILES}
	rm -rf ./build
	rm ./stdlib.elf

directories: $(BUILD_DIRS)

$(BUILD_DIRS):
	@echo "Create build directories: $@"
	@mkdir $@
	@echo "Build directories created: $@"