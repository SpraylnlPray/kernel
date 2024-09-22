FILES=./build/start.o ./build/danos.asm.o ./build/stdlib.o ./build/stdio.o ./build/danos.o ./build/string.o ./build/memory.o
INCLUDES=
BUILD_DIRS= ./build
FLAGS= -g -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: directories ${FILES}
	i686-elf-ld -m elf_i386 -relocatable ${FILES} -o ./stdlib.elf

./build/start.o: ./src/start.asm
	@echo "$@ start"
	nasm -f elf ./src/start.asm -o ./build/start.o
	@echo "$@ finished"

./build/danos.asm.o: ./src/danos.asm
	@echo "$@ start"
	nasm -f elf ./src/danos.asm -o ./build/danos.asm.o
	@echo "$@ finished"

./build/danos.o: ./src/danos.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I ./${FLAGS} -std=gnu99 -c ./src/danos.c -o ./build/danos.o
	@echo "$@ finished"

./build/stdlib.o: ./src/stdlib.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I ./${FLAGS} -std=gnu99 -c ./src/stdlib.c -o ./build/stdlib.o
	@echo "$@ finished"

./build/stdio.o: ./src/stdio.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I ./${FLAGS} -std=gnu99 -c ./src/stdio.c -o ./build/stdio.o
	@echo "$@ finished"

./build/string.o: ./src/string.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I ./${FLAGS} -std=gnu99 -c ./src/string.c -o ./build/string.o
	@echo "$@ finished"

./build/memory.o: ./src/memory.c
	@echo "$@ start"
	i686-elf-gcc ${INCLUDES} -I ./${FLAGS} -std=gnu99 -c ./src/memory.c -o ./build/memory.o
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