#pragma once

#include <libsystem/Types.h>

namespace hegel::arch::x86 {

static inline uint8_t in8(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline uint16_t in16(uint16_t port)
{
    uint16_t data;
    asm volatile("inw %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline uint32_t in32(uint16_t port)
{
    uint32_t data;
    asm volatile("inl %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline void out8(uint16_t port, uint8_t data)
{
    asm volatile("outb %0, %1" : : "a"(data), "d"(port));
}

static inline void out16(uint16_t port, uint16_t data)
{
    asm volatile("outw %0, %1" : : "a"(data), "d"(port));
}

static inline void out32(uint16_t port, uint32_t data)
{
    asm volatile("outl %0, %1" : : "a"(data), "d"(port));
}

static inline void raise_irq1()
{
    asm volatile("int $32");
}

static inline void cli(void) { asm volatile("cli"); }
static inline void sti(void) { asm volatile("sti"); }
static inline void hlt(void) { asm volatile("hlt"); }

extern "C" void load_gdt(uint32_t gdt);
extern "C" void load_idt(uint32_t idt);
extern "C" void store_gdt(void* addr);

}