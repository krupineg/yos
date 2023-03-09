# $@ = target file
# $< = first dependency
# $^ = all dependencies

TARGET_TEST_PLATFORM=arm64
TARGET_PLATFORM=i386
TARGET_PLATFORM_COMPILER=i686

DIRS=build build/boot build/kernel build/drivers build/drivers/${TARGET_PLATFORM} build_test build_test/drivers build_test/kernel build_test/drivers/${TARGET_PLATFORM}

# Nice syntax for file extension replacement

DRIVERS_SRC=$(wildcard src/drivers/*.c)
DRIVERS_OBJ = $(subst src,build, ${DRIVERS_SRC:.c=.o})
DRIVERS_SRC_PLATFORM=$(wildcard src/drivers/${TARGET_PLATFORM}/*.c)
DRIVERS_OBJ_PLATFORM = $(subst src,build, ${DRIVERS_SRC_PLATFORM:.c=.o})
KERNEL_SRC=$(wildcard src/kernel/*.c)
KERNEL_OBJ = $(subst src,build, ${KERNEL_SRC:.c=.o})
TARGET_PLATFORM_OBJ = ${KERNEL_OBJ} ${DRIVERS_OBJ} ${DRIVERS_OBJ_PLATFORM}

TEST_SRC=$(wildcard test/*.c)
TEST_H=$(wildcard test/*.h)
TEST_OBJ=$(subst test,build_test, ${TEST_SRC:.c=.o})
DRIVERS_TEST_OBJ = $(subst src,build_test, ${DRIVERS_SRC:.c=.o})
KERNEL_TEST_OBJ = $(subst src,build_test, ${KERNEL_SRC:.c=.o})
DRIVERS_SRC_TEST_PLATFORM=$(wildcard src/drivers/${TARGET_TEST_PLATFORM}/*.c)
DRIVERS_OBJ_TEST_PLATFORM = $(subst src,build, ${DRIVERS_SRC_TEST_PLATFORM:.c=.o})
TEST_TARGET_PLATFORM_OBJ = ${KERNEL_TEST_OBJ} ${DRIVERS_TEST_OBJ} ${DRIVERS_OBJ_TEST_PLATFORM}

CFLAGS=-g
CC=${TARGET_PLATFORM_COMPILER}-elf-gcc
LD=${TARGET_PLATFORM_COMPILER}-elf-ld
GDB=${TARGET_PLATFORM}-elf-gdb

TESTCC=gcc-12 -D__VERBOSE__

print-%  : ; @echo $* = $($*)

$(shell mkdir -p $(DIRS))

# First rule is the one executed when no parameters are fed to the Makefile
all: run

run: ./build/os-image.bin
	qemu-system-${TARGET_PLATFORM} -fda $<

./build/os-image.bin: ./build/boot/bootsect.bin ./build/kernel/kernel.bin
	cat $^ > $@

./build/kernel/kernel.bin: ./build/boot/kernel_entry.o ${TARGET_PLATFORM_OBJ}
	${LD} ${CFLAGS} -o $@ -Ttext 0x1000 $^ --oformat binary

./build/boot/kernel_entry.o: ./src/boot/kernel_entry.asm
	nasm $< -f elf -o $@
	
./build/kernel/%.o: ./src/kernel/%.c
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

./build/drivers/%.o: ./src/drivers/%.c 
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

./build/drivers/${TARGET_PLATFORM}/%.o: ./src/drivers/${TARGET_PLATFORM}/%.c
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

# Rule to disassemble the kernel - may be useful to debug
./build/kernel/kernel.dis: ./build/kernel/kernel.bin
	ndisasm -b 32 $< > $@

./build/boot/bootsect.bin: ./src/boot/boot.asm
	nasm $< -f bin -o $@

clean:
	rm -rf ./build/*  & rm -rf ./build_test/*

# Open the connection to qemu and load our kernel-object file with symbols
debug: ./build/os-image.bin ./build/kernel/kernel.elf
	qemu-system-${TARGET_PLATFORM} -s -S -fda ./build/os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file ./build/kernel/kernel.elf"

# Used for debugging purposes
./build/kernel/kernel.elf: ./build/boot/kernel_entry.o ${TARGET_PLATFORM_OBJ}
	${LD} ${CFLAGS} -o $@ -Ttext 0x1000 $^

#testing

test: before_test
	./build_test/test

before_test: ./build_test/test
	chmod +x ./build_test/test

./build_test/test: ./test/test.c
	${TESTCC} -DDEBUG --entry=_test -Wall -nostartfiles -o ./build_test/test ${DRIVERS_SRC} ${KERNEL_SRC} ${DRIVERS_SRC_TEST_PLATFORM} ./test/test.c
