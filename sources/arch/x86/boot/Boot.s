; Entry point for the kernel

MULTIBOOT2_HEADER_MAGIC equ 0xE85250D6
MULTIBOOT2_ARCHITECTURE_I386 equ 0
MULTIBOOT2_HEADER_LENGTH equ __multiboot_header_end - __multiboot_header
MULTIBOOT2_CHECKSUM equ -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_ARCHITECTURE_I386 + MULTIBOOT2_HEADER_LENGTH)

MULTIBOOT2_HEADER_TAG_END equ 0

section .multiboot
__multiboot_header:

align 4
    dd MULTIBOOT2_HEADER_MAGIC
    dd MULTIBOOT2_ARCHITECTURE_I386
    dd MULTIBOOT2_HEADER_LENGTH
    dd MULTIBOOT2_CHECKSUM

    dw MULTIBOOT2_HEADER_TAG_END
    dw 0
    dw 8

__multiboot_header_end:

section .bss
align 4096
stack_bottom:
resb 16384
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
    cli
    cld

    mov esp, stack_top
    push 0
    push 0
    mov ebp, esp
    
    push ebx
    push eax

    extern arch_main
    call arch_main

    cli
    hlt
    jmp $-1
.end: