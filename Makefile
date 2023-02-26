# $@ = target file
# $< = first dependency
# $^ = all dependencies
DIRS=build build/boot build/kernel build/drivers
# Nice syntax for file extension replacement
OBJ = $(wildcard build/kernel/*.o build/drivers/*.o)

DRIVERS_SRC=$(wildcard src/drivers/*.c)
DRIVERS_H = $(wildcard src/drivers/*.h)
DRIVERS_OBJ = $(subst src,build, ${DRIVERS_SRC:.c=.o})

KERNEL_SRC=$(wildcard src/kernel/*.c)
KERNEL_H = $(wildcard src/kernel/*.h)
KERNEL_OBJ = $(subst src,build, ${KERNEL_SRC:.c=.o})

CFLAGS=-g#gdb -g -DDEBUG
CC=i686-elf-gcc
LD=i686-elf-ld
GDB=i386-elf-gdb

print-%  : ; @echo $* = $($*)

$(shell mkdir -p $(DIRS))

# First rule is the one executed when no parameters are fed to the Makefile
all: run

run: ./build/os-image.bin
	qemu-system-i386 -fda $<

./build/os-image.bin: ./build/boot/bootsect.bin ./build/kernel/kernel.bin
	cat $^ > $@

./build/kernel/kernel.bin: ./build/boot/kernel_entry.o ${KERNEL_OBJ} ${DRIVERS_OBJ}
	${LD} ${CFLAGS} -o $@ -Ttext 0x1000 $^ --oformat binary

./build/boot/kernel_entry.o: ./src/boot/kernel_entry.asm
	nasm $< -f elf -o $@

./build/kernel/${KERNEL_OBJ}: ${KERNEL_SRC}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

./build/drivers/${DRIVERS_OBJ}: ${DRIVERS_SRC} ${DRIVERS_H}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

# Rule to disassemble the kernel - may be useful to debug
./build/kernel/kernel.dis: ./build/kernel/kernel.bin
	ndisasm -b 32 $< > $@

./build/boot/bootsect.bin: ./src/boot/boot.asm
	nasm $< -f bin -o $@

clean:
	rm -rf ./build/*

# Open the connection to qemu and load our kernel-object file with symbols
debug: ./build/os-image.bin ./build/kernel/kernel.elf
	qemu-system-i386 -s -S -fda ./build/os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file ./build/kernel/kernel.elf"

# Used for debugging purposes
./build/kernel/kernel.elf: ./build/boot/kernel_entry.o ${KERNEL_OBJ} ${DRIVERS_OBJ}
	${LD} ${CFLAGS} -o $@ -Ttext 0x1000 $^