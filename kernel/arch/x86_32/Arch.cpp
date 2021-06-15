#include "arch/Arch.h"
#include "x86.h"
#include "paging/Paging.h"
#include "device/CGATerminal.h"

#include <libsystem/__plugs__.h>
#include <libsystem/RefPtr.h>

using namespace memory;
using namespace hegel::arch::x86;

extern int __kernel_start;
extern int __kernel_end;

namespace hegel::arch {

void temporary_graphics_init()
{
    RefPtr<CGATerminal> terminal = make<CGATerminal>((void*)0xb8000);
    hegel::plugs::out_stream = terminal.give_ref();
}

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
    return ARCH_PAGE_SIZE;
}


}