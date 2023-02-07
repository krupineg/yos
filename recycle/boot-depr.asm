[org 0x7c00]
    mov bp, 0x8000 ; set the stack safely away from us
    mov sp, bp

    mov bx, 0x9000 ; es:bx = 0x0000:0x9000 = 0x09000
    mov dh, 2 ; read 2 sectors
    ; the bios sets 'dl' for our boot disk number
    ; if you have trouble, use the '-fda' flag: 'qemu -fda file.bin'
    call disk_load

    mov dx, [0x9000] ; retrieve the first loaded word, 0xdada
    call print_hex
    call print_nl

    mov dx, [0x9000 + 512] ; first word from second loaded sector, 0xface
    call print_hex
    call print_nl

    mov bx, HELLO
    call print_string


[org 0x7c00] ; bootloader offset
    mov bp, 0x9000 ; set the stack
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print_string ; This will be written after the BIOS messages

    call switch_to_pm
    jmp $ ; this will actually never be executed
    


%include "./src/low/print_string.asm"
%include "./src/low/print_hex.asm"
%include "./src/low/disk_load.asm"
%include "./src/low/32/print_string32.asm"
%include "./src/low/32/gdt32.asm"
%include "./src/low/32/switch32.asm"

HELLO:
    db 'Hello, World', 0

[bits 32]
BEGIN_PM: ; after the switch we will get here
    mov ebx, MSG_PROT_MODE
    call print_string32 ; Note that this will be written at the top left corner
    jmp $

MSG_REAL_MODE db "Started in 16-bit real mode !!!", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0

times 510-($-$$) db 0
dw 0xaa55
