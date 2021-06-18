; Entry point for the kernel

MULTIBOOT2_HEADER_MAGIC equ 0xE85250D6
MULTIBOOT2_ARCHITECTURE_I386 equ 0
MULTIBOOT2_HEADER_LENGTH equ __multiboot_header_end - __multiboot_header
MULTIBOOT2_CHECKSUM equ -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_ARCHITECTURE_I386 + MULTIBOOT2_HEADER_LENGTH)

MULTIBOOT2_HEADER_TAG_OPTIONAL equ 1

MULTIBOOT2_HEADER_TAG_END equ 0
MULTIBOOT2_HEADER_TAG_INFORMATION_REQUEST equ 1
MULTIBOOT2_HEADER_TAG_ADDRESS equ 2
MULTIBOOT2_HEADER_TAG_ENTRY_ADDRESS equ 3
MULTIBOOT2_HEADER_TAG_CONSOLE_FLAGS equ 4
MULTIBOOT2_HEADER_TAG_FRAMEBUFFER equ 5
MULTIBOOT2_HEADER_TAG_MODULE_ALIGN equ 6
MULTIBOOT2_HEADER_TAG_EFI_BS equ 7
MULTIBOOT2_HEADER_TAG_ENTRY_ADDRESS_EFI32 equ 8
MULTIBOOT2_HEADER_TAG_ENTRY_ADDRESS_EFI64 equ 9
MULTIBOOT2_HEADER_TAG_RELOCATABLE equ 10

section .multiboot
__multiboot_header:

align 4
    dd MULTIBOOT2_HEADER_MAGIC
    dd MULTIBOOT2_ARCHITECTURE_I386
    dd MULTIBOOT2_HEADER_LENGTH
    dd MULTIBOOT2_CHECKSUM

;align 8
;    dw MULTIBOOT2_HEADER_TAG_FRAMEBUFFER
;    dw MULTIBOOT2_HEADER_TAG_OPTIONAL
;    dd 20
;    dd 0
;    dd 0
;    dd 32

align 8
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
    xor ebp, ebp

    push ebx
    push eax

    extern arch_main
    call arch_main

    cli
    hlt
    jmp $-1
.end: