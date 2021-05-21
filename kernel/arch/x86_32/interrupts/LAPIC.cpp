#include "LAPIC.h"
#include "PIC.h"

#define LAPIC_EOI 0x00B0
#define LAPIC_SVR 0x00F0

static volatile uint32_t* _lapic = nullptr;

void lapic_found(uintptr_t address) {
    _lapic = reinterpret_cast<uint32_t*>(address);
}

uint32_t lapic_read(uint32_t reg) {
    return *((volatile uint32_t *)(_lapic + reg));
}

void lapic_write(uint32_t reg, uint32_t data) {
    *((volatile uint32_t *)(_lapic + reg)) = data;
}

void lapic_ack()
{
    lapic_write(LAPIC_EOI, 0);
}

void lapic_initialize()
{
    hegel::arch::x86::pic_disable();

    lapic_write(LAPIC_SVR, 0b1 << 8); // LAPIC enable bit
}
