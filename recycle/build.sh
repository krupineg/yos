mkdir ./build

g++ -fdiagnostics-color=always ./src/kernel/main.cpp -o ./build/main
nasm -f bin ./src/low/boot.asm -o ./build/boot.bin