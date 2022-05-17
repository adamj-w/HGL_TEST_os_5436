#include "arch/Arch.h"
#include "x86.h"
#include "paging/Paging.h"
#include "segmentation/Segmentation.h"
#include "device/CGATerminal.h"
#include "device/FPU.h"
#include "interrupts/Interrupts.h"

#include <kernel/proc/Thread.h>

#include <libsystem/__plugs__.h>
#include <libutil/RefPtr.h>

using namespace memory;
using namespace proc;
using namespace hegel::arch::x86;

extern int __kernel_start;
extern int __kernel_end;

namespace hegel::arch {

void disable_interrupts() { x86::cli(); }
void enable_interrupts() { x86::sti(); }

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
    asm volatile("int $127");
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

void save_context(Thread* thread) {
    fpu_save_context(thread);
}

void load_context(Thread* thread) {
    fpu_load_context(thread);
    x86::set_kernel_stack((uintptr_t)thread->kernel_stack + PROCESS_STACK_SIZE);
}

void thread_go(Thread* thread) {
    fpu_init_context(thread);

    // TODO L: set user segments
    InterruptStackFrame stackFrame = {};
    stackFrame.eflags = 0x202;
    stackFrame.eip = (uintptr_t)thread->entry_point;
    stackFrame.ebp = 0;

    stackFrame.cs = HEGEL_KERNEL_CODE_SEG;
    stackFrame.ds = HEGEL_KERNEL_DATA_SEG;
    stackFrame.es = HEGEL_KERNEL_DATA_SEG;
    stackFrame.fs = HEGEL_KERNEL_DATA_SEG;
    stackFrame.gs = HEGEL_KERNEL_DATA_SEG;

    thread->kernel_stack_push(&stackFrame, sizeof(InterruptStackFrame));
}

size_t get_page_size()
{
    return ARCH_PAGE_SIZE;
}


}