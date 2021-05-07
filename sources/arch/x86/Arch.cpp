#include <arch/Arch.h>
#include <arch/x86/x86.h>

using namespace hegel::memory;

extern int __kernel_start;
extern int __kernel_end;

namespace hegel::arch {

void stop() 
{
    do
    {
        x86::cli();
        x86::hlt();
    } while (1);
}

void idle()
{
    do
    {
        x86::hlt();
    } while (1);
}

void halt()
{
    x86::hlt();
}

void yield()
{
    x86::raise_irq1();
}

size_t get_page_size()
{
    return 4096;
}

memory::MemoryRegion get_kernel_region()
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(&__kernel_start);
    size_t size = &__kernel_end - &__kernel_start;

    return MemoryRegion::create_around_non_aligned_address(addr, size);
}

}