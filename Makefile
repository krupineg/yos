# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
all: run

# Notice how dependencies are built as needed
./build/kernel.bin: ./build/kernel_entry.o ./build/kernel.o
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

./build/kernel_entry.o: ./src/boot/kernel_entry.asm
	nasm $< -f elf -o $@

./build/kernel.o: ./src/kernel/kernel.c
	i686-elf-gcc -ffreestanding -c $< -o $@

# Rule to disassemble the kernel - may be useful to debug
./build/kernel.dis: ./build/kernel.bin
	ndisasm -b 32 $< > $@

./build/bootsect.bin: ./src/boot/boot.asm
	nasm $< -f bin -o $@

./build/os-image.bin: ./build/bootsect.bin ./build/kernel.bin
	cat $^ > $@

run: ./build/os-image.bin
	qemu-system-i386 -fda $<

clean:
	rm ./build/*.bin ./build/*.o ./build/*.dis