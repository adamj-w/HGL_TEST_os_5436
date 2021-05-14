#include "arch/Arch.h"
#include "x86.h"

#include "tasking/x86Thread.h"
#include <kernel/tasking/Thread.h>
#include <kernel/tasking/Process.h>

using namespace hegel::memory;
using namespace hegel::arch::x86;

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

void shutdown()
{
    // Bochs, and older versions of QEMU(than 2.0)
    out16(0xB004, 0x2000);

    // Newer versions of QEMU
    out16(0x604, 0x2000);

    // Virtualbox
    out16(0x4004, 0x3400);
    arch::halt();
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

RefPtr<tasking::Thread> create_thread(RefPtr<tasking::Process> process, tasking::ThreadEntry entry)
{
    return make<x86::x86Thread>(process, entry);
}

}