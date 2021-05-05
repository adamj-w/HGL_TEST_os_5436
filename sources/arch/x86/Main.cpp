#include <libc/stdint.h>

#include "device/SerialStream.h"

using namespace hegel::arch::x86;

extern "C" void arch_main(uint32_t multiboot_magic, uintptr_t multiboot_addr)
{
    auto serial = SerialStream(SerialPort::COM1);

    ((void)multiboot_magic);
    ((void)multiboot_addr);
}