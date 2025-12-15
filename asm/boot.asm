; boot.asm - Multiboot bootloader
section .multiboot
align 4
    dd 0x1BADB002                          ; Magic number
    dd 0x00000003                          ; Flags
    dd -(0x1BADB002 + 0x00000003)         ; Checksum

section .bss
align 16
stack_bottom:
    resb 16384                             ; 16KB stack
stack_top:

section .text
global start
extern kernel_main

start:
    mov esp, stack_top                     ; Set up stack
    call kernel_main                       ; Call C kernel
.hang:
    cli
    hlt
    jmp .hang
