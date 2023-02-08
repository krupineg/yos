# $@ = target file
# $< = first dependency
# $^ = all dependencies
DIRS=build build/boot build/kernel
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -g

$(shell mkdir -p $(DIRS))

# First rule is the one executed when no parameters are fed to the Makefile
all: run

run: ./build/os-image.bin
	qemu-system-i386 -fda $<

./build/os-image.bin: ./build/boot/bootsect.bin ./build/kernel/kernel.bin
	cat $^ > $@

./build/kernel/kernel.bin: ./build/boot/kernel_entry.o ./build/kernel/kernel.o
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

./build/boot/kernel_entry.o: ./src/boot/kernel_entry.asm
	nasm $< -f elf -o $@

./build/kernel/kernel.o: ./src/kernel/kernel.c
	i686-elf-gcc -ffreestanding -c $< -o $@

# Rule to disassemble the kernel - may be useful to debug
./build/kernel/kernel.dis: ./build/kernel/kernel.bin
	ndisasm -b 32 $< > $@

./build/boot/bootsect.bin: ./src/boot/boot.asm
	nasm $< -f bin -o $@


clean:
	rm ./build/*.bin ./build/*.o ./build/*.dis